#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <math.h>
#define LOG_ENABLED

/* Constant Variables */
#define MAX_ENEMY 5
#define MAX_BULLET 4
#define MAX_EBULLET 8
#define MAX_LIVES 6
#define MAX_ULTI 10
#define MAX_MINION 4
const int FPS = 70;
const int SCREEN_W = 800;
const int SCREEN_H = 600;
const int RESERVE_SAMPLES = 4;
const float MAX_COOLDOWN = 0.2;
const float MAX_E_COOLDOWN = 0.5;
const float BOSS_COOLDOWN = 10;
const float MAX_ULTI_COOLDOWN = 2;
const float DEAD_TIME = 1;
enum
{
    SCENE_MENU = 1,
    SCENE_START = 2,
    SCENE_SETTINGS = 3,
    SCENE_LOSS = 4,
    STAGE_2 = 5,
    STAGE_3 = 6,
    BOSS_STAGE = 7,
    SCENE_TRANSITION_12 = 8,
    SCENE_TRANSITION_23 = 9,
    SCENE_TRANSITION_34 = 10,
    SCENE_WIN = 11
};

/* Event variables */
int active_scene;
int before_scene;
bool key_state[ALLEGRO_KEY_MAX];
bool *mouse_state;
int mouse_x, mouse_y;
// TODO: More variables to store input states such as joysticks, ...

/* Variables for allegro basic routines. */
ALLEGRO_DISPLAY* game_display;
ALLEGRO_EVENT_QUEUE* game_event_queue;
ALLEGRO_TIMER* game_update_timer;

/* Shared resources*/
ALLEGRO_FONT* font_pirulen_32;
ALLEGRO_FONT* font_pirulen_24;
ALLEGRO_FONT* font_pirulen_12;
ALLEGRO_FONT* font_deltarune_16;
ALLEGRO_FONT* font_MyGame_50;
ALLEGRO_FONT* font_gameover_45;
ALLEGRO_FONT* font_gameover_55;
ALLEGRO_FONT* font_gameover_100;
ALLEGRO_FONT* font_darkfuture_50;
// TODO: More shared resources or data that needed to be accessed
// across different scenes.

/* Menu Scene resources*/
ALLEGRO_BITMAP* main_img_background;
ALLEGRO_BITMAP* img_settings;
ALLEGRO_BITMAP* img_settings2;
ALLEGRO_SAMPLE* main_bgm;
ALLEGRO_SAMPLE_ID main_bgm_id;
int highscore = 0;

/* Start Scene resources*/
ALLEGRO_BITMAP* start_img_background;
ALLEGRO_BITMAP* start_img_plane;
ALLEGRO_BITMAP* plane2_img;
ALLEGRO_BITMAP* plane3_img;
ALLEGRO_BITMAP* plane4_img;
ALLEGRO_BITMAP* plane5_img;
ALLEGRO_BITMAP* start_img_enemy;
ALLEGRO_SAMPLE* start_bgm;
ALLEGRO_SAMPLE_ID start_bgm_id;
ALLEGRO_BITMAP* img_bullet;
ALLEGRO_BITMAP* img_ebullet;
ALLEGRO_BITMAP* img_ulti;
ALLEGRO_SAMPLE* ulti_bgm;
ALLEGRO_SAMPLE_ID ulti_bgm_id;
ALLEGRO_BITMAP* img_heart;
ALLEGRO_BITMAP* extra_heart_img;
ALLEGRO_SAMPLE* bullet_sound_bgm;
ALLEGRO_SAMPLE* bullet_clashing_bgm;
ALLEGRO_SAMPLE_ID bullet_clashing_bgm_id;
int increasing_speed_enemy = 0;
int increasing_speed_bullet = 0;
int bought_speed_bullet = 0;
long long int b_score = 0;
int b_money = 0;
int lost_health = 0;
int ulti_counter = 0;
int scene_counter;

/* Settings scene resources */
ALLEGRO_BITMAP* settings_img_background;
ALLEGRO_BITMAP* back_icon_img;
ALLEGRO_BITMAP* arrow_icon_img;
ALLEGRO_BITMAP* plane_icon_img;
ALLEGRO_BITMAP* plane2_icon_img;
ALLEGRO_BITMAP* plane3_icon_img;
ALLEGRO_BITMAP* plane4_icon_img;
ALLEGRO_BITMAP* plane5_icon_img;
ALLEGRO_BITMAP* character_choose_img;
ALLEGRO_BITMAP* lock_icon_img;
ALLEGRO_SAMPLE* settings_bgm;
ALLEGRO_SAMPLE_ID settings_bgm_id;
int plane1 = 1;
int plane2 = 0;
int plane3 = 0;
int plane4 = 0;
int plane5 = 0;
int plane_speed;
int bullet_speed;
int ori_hp;
int money_buff = 1;
int score_buff = 1;

/* stage 2 scene resources */
ALLEGRO_BITMAP* stage2_img_backgroud;
ALLEGRO_BITMAP* fire_icon_img;
ALLEGRO_BITMAP* enemy_stage2_img;

/* stage 3 scene resources */
ALLEGRO_BITMAP* stage3_img_backgroud;

/* boss stage resources */
ALLEGRO_BITMAP* boss_stage_img_backgroud;
ALLEGRO_BITMAP* boss_img;
ALLEGRO_BITMAP* boss_minion_img;
ALLEGRO_BITMAP* boss_laser_img;
ALLEGRO_SAMPLE* boss_battle_bgm;
ALLEGRO_SAMPLE_ID boss_battle_bgm_id;

/* transition resources */
ALLEGRO_BITMAP* hearticon_img;
ALLEGRO_BITMAP* randomicon_img;
ALLEGRO_BITMAP* speedicon_img;
ALLEGRO_BITMAP* speedicon2_img;
ALLEGRO_BITMAP* ebullet_icon_img;
ALLEGRO_BITMAP* bullet_icon;
ALLEGRO_SAMPLE* transision_bgm;
ALLEGRO_SAMPLE_ID transision_bgm_id;
ALLEGRO_SAMPLE* before_boss_bgm;
ALLEGRO_SAMPLE_ID before_boss_bgm_id;
ALLEGRO_SAMPLE* bought_bgm;
ALLEGRO_SAMPLE_ID bought_bgm_id;
ALLEGRO_BITMAP* boss_icon_img;
ALLEGRO_BITMAP* boss_minion_icon_img;
ALLEGRO_BITMAP* warning_img;

/* collision sound */
ALLEGRO_SAMPLE* enemy_die_bgm;
ALLEGRO_SAMPLE_ID enemy_die_bgm_id;
ALLEGRO_SAMPLE* plane_die_bgm;
ALLEGRO_SAMPLE_ID plane_die_bgm_id;
ALLEGRO_SAMPLE* boss_damaged_bgm;
ALLEGRO_SAMPLE_ID boss_damaged_bgm_id;
ALLEGRO_SAMPLE* laser_bgm;
ALLEGRO_SAMPLE_ID laser_bgm_id;

/* win and loss scene resources */
ALLEGRO_SAMPLE* win_bgm;
ALLEGRO_SAMPLE_ID win_bgm_id;
ALLEGRO_SAMPLE* loss_bgm;
ALLEGRO_SAMPLE_ID loss_bgm_id;

/* Object declaration */
typedef struct
{
    float x, y;
    float w, h;
    float vx, vy;
    int hp;
    int money;
    long long int score;
    bool hidden;
    ALLEGRO_BITMAP* img;
} MovableObject;

/* Object variables */
MovableObject plane;
MovableObject text;
MovableObject enemies[MAX_ENEMY];
MovableObject bullets[MAX_BULLET];
MovableObject e_bullets[MAX_EBULLET];
MovableObject hearts[MAX_LIVES];
MovableObject ulti[MAX_ULTI];
MovableObject boss;
MovableObject boss_laser;
MovableObject boss_minion[MAX_MINION];
double last_shoot_timestamp;
double last_died_timestamp;
double last_attack_timestamp;
double last_ulti_timestamp;
double last_laser_timestamp;

/* Declare function prototypes. */
void allegro5_init(void);
void game_init(void);
void game_start_event_loop(void);
void game_update(void);
void game_draw(void);
void game_destroy(void);
void game_change_scene(int next_scene);
void draw_movable_object(MovableObject obj);
ALLEGRO_BITMAP *load_bitmap_resized(const char *filename, int w, int h);
bool pnt_in_rect(int px, int py, int x, int y, int w, int h);
void on_key_down(int keycode);
void on_mouse_down(int btn, int x, int y);

/* Declare function prototypes for debugging. */
void game_abort(const char* format, ...);
void game_log(const char* format, ...);
void game_vlog(const char* format, va_list arg);

int main(int argc, char** argv)
{
    srand(time(NULL));

    allegro5_init();
    game_log("Allegro5 initialized");
    game_log("Game begin");

    game_init();
    game_log("Game initialized");

    game_draw();

    game_log("Game start event loop");
    game_start_event_loop();

    game_log("Game end");
    game_destroy();

    return 0;
}

void allegro5_init(void)
{
    if (!al_init())
        game_abort("failed to initialize allegro");

    // Initialize add-ons.
    if (!al_init_primitives_addon())
        game_abort("failed to initialize primitives add-on");
    if (!al_init_font_addon())
        game_abort("failed to initialize font add-on");
    if (!al_init_ttf_addon())
        game_abort("failed to initialize ttf add-on");
    if (!al_init_image_addon())
        game_abort("failed to initialize image add-on");
    if (!al_install_audio())
        game_abort("failed to initialize audio add-on");
    if (!al_init_acodec_addon())
        game_abort("failed to initialize audio codec add-on");
    if (!al_reserve_samples(RESERVE_SAMPLES))
        game_abort("failed to reserve samples");
    if (!al_install_keyboard())
        game_abort("failed to install keyboard");
    if (!al_install_mouse())
        game_abort("failed to install mouse");
    // TODO: Initialize other addons such as video, ...

    game_display = al_create_display(SCREEN_W, SCREEN_H);
    if (!game_display)
        game_abort("failed to create display");
    al_set_window_title(game_display, "I2P(I)_2019 Final Project <108062182>");

    game_update_timer = al_create_timer(1.0f / FPS);
    if (!game_update_timer)
        game_abort("failed to create timer");

    game_event_queue = al_create_event_queue();
    if (!game_event_queue)
        game_abort("failed to create event queue");

    const unsigned m_buttons = al_get_mouse_num_buttons();
    game_log("There are total %u supported mouse buttons", m_buttons);
    mouse_state = malloc((m_buttons + 1) * sizeof(bool));
    memset(mouse_state, false, (m_buttons + 1) * sizeof(bool));

    al_register_event_source(game_event_queue, al_get_display_event_source(game_display));
    al_register_event_source(game_event_queue, al_get_timer_event_source(game_update_timer));
    al_register_event_source(game_event_queue, al_get_keyboard_event_source());
    al_register_event_source(game_event_queue, al_get_mouse_event_source());
    // TODO: Register other event sources such as timer, video, ...

    al_start_timer(game_update_timer);
}

void game_init(void)
{
    /* Shared resources*/
    font_pirulen_32 = al_load_font("pirulen.ttf", 32, 0);
    if (!font_pirulen_32)
        game_abort("failed to load font: pirulen.ttf with size 32");

    font_pirulen_24 = al_load_font("pirulen.ttf", 24, 0);
    if (!font_pirulen_24)
        game_abort("failed to load font: pirulen.ttf with size 24");

    font_deltarune_16 = al_load_font("deltarune.ttf", 16, 0);
    if(!font_deltarune_16)
        game_abort("failed to load font: deltarune.ttf with size 16");

    font_MyGame_50 = al_load_font("MyGame.ttf", 50, 0);
    if(!font_MyGame_50)
        game_abort("failed to load font: MyGame.ttf with size 50");

    font_gameover_45 = al_load_font("gameover.ttf", 45, 0);
    if(!font_gameover_45)
        game_abort("failed to load font: gameover.ttf with size 45");

    font_gameover_55 = al_load_font("gameover.ttf", 55, 0);
    if(!font_gameover_55)
        game_abort("failed to load font: gameover.ttf with size 55");

    font_gameover_100 = al_load_font("gameover.ttf", 100, 0);
    if(!font_gameover_100)
        game_abort("failed to load font: gameover.ttf with size 100");

    font_pirulen_12 = al_load_font("pirulen.ttf", 12, 0);
    if(!font_pirulen_12)
        game_abort("failed to load font: pirulen.ttf with size 12");

    font_darkfuture_50 = al_load_font("Dark Future Italic.ttf", 50, 0);
    if(!font_darkfuture_50)
        game_abort("failed to load font: Dark Future Italic.ttf with size 50");

    /* Menu Scene resources*/
    main_img_background = load_bitmap_resized("main-bg.jpg", SCREEN_W, SCREEN_H);

    main_bgm = al_load_sample("S31-Night Prowler.ogg");
    if (!main_bgm)
        game_abort("failed to load audio: S31-Night Prowler.ogg");

    img_settings = al_load_bitmap("settings.png");
    if (!img_settings)
        game_abort("failed to load image: settings.png");

    img_settings2 = al_load_bitmap("settings2.png");
    if (!img_settings2)
        game_abort("failed to load image: settings2.png");

    /* Start Scene resources*/
    start_img_background = load_bitmap_resized("start-bg.jpg", SCREEN_W, SCREEN_H);

    bullet_sound_bgm = al_load_sample("bullet_sound.ogg");
    if(!bullet_sound_bgm)
        game_abort("failed to load audio: bullet_sound.ogg");

    start_img_plane = al_load_bitmap("plane.png");
    if (!start_img_plane)
        game_abort("failed to load image: plane.png");

    plane2_img = al_load_bitmap("r_plane2.png");
    if(!plane2_img)
        game_abort("failed to load image: r_plane2.png");

    plane3_img = al_load_bitmap("r_plane3.png");
    if(!plane3_img)
        game_abort("failed to load image: r_plane3.png");

    plane4_img = al_load_bitmap("r_plane4.png");
    if(!plane4_img)
        game_abort("failed to load image: r_plane4.png");

    plane5_img = al_load_bitmap("r_plane5.png");
    if(!plane2_img)
        game_abort("failed to load image: r_plane5.png");

    start_img_enemy = al_load_bitmap("smallfighter0006.png");
    if (!start_img_enemy)
        game_abort("failed to load image: smallfighter0006.png");

    start_bgm = al_load_sample("mythica.ogg");
    if (!start_bgm)
        game_abort("failed to load audio: mythica.ogg");

    img_bullet = al_load_bitmap("image12.png");
    if (!img_bullet)
        game_abort("failed to load image: image12.png");

    img_ebullet = al_load_bitmap("ebullet.png");
    if(!img_ebullet)
        game_abort("failed to load image: ebullet.png");

    img_heart = al_load_bitmap("heart.png");
    if(!img_heart)
        game_abort("failed to load image: heart.png");

    extra_heart_img = al_load_bitmap("green_heart.png");
    if(!extra_heart_img)
        game_abort("failed to load image: green_heart.png");

    img_ulti = al_load_bitmap("ultimate.png");
    if(!img_ulti)
        game_abort("failed to load image: ultimate.png");

    ulti_bgm = al_load_sample("ulti_sound.ogg");
    if(!ulti_bgm)
        game_abort("failed to load sample: ulti_sound.ogg");

    bullet_clashing_bgm = al_load_sample("bullet_clashing.ogg");
    if(!bullet_clashing_bgm)
        game_abort("failed to load sample: bullet_clashing.ogg");

    /* Settings resources */
    settings_img_background = load_bitmap_resized("settings-bg.jpg", SCREEN_W, SCREEN_H);

    back_icon_img = al_load_bitmap("back_icon.png");
    if(!back_icon_img)
        game_abort("failed to load image: back_icon.png");

    arrow_icon_img = al_load_bitmap("arrow_icon.png");
    if(!arrow_icon_img)
        game_abort("failed to load image: arrow_icon.png");

    plane_icon_img = al_load_bitmap("plane-icon.png");
    if(!plane_icon_img)
        game_abort("failed to load image: plane-icon.png");

    plane2_icon_img = al_load_bitmap("plane-2.png");
    if(!plane2_icon_img)
        game_abort("failed to load image: plane-2.png");

    plane3_icon_img = al_load_bitmap("plane-3.png");
    if(!plane3_icon_img)
        game_abort("failed to load image: plane-3.png");

    plane4_icon_img = al_load_bitmap("plane-4.png");
    if(!plane4_icon_img)
        game_abort("failed to load image: plane-4.png");

    plane5_icon_img = al_load_bitmap("plane-5.png");
    if(!plane5_icon_img)
        game_abort("failed to load image: plane-5.png");

    character_choose_img = al_load_bitmap("settings_frame.png");
    if(!character_choose_img)
        game_abort("failed to load image: settings_frame.png");

    lock_icon_img = al_load_bitmap("lock_icon.png");
    if(!lock_icon_img)
        game_abort("failed to load image: lock_icon.png");

    settings_bgm = al_load_sample("settings.ogg");
    if(!settings_bgm)
        game_abort("failed to load sample: settings.ogg");

    before_boss_bgm = al_load_sample("before_boss.ogg");
    if(!before_boss_bgm)
        game_abort("failed to load sample: before_boss.ogg");

    /* Stage 2 resources */
    stage2_img_backgroud = load_bitmap_resized("stage2-bg.jpg", SCREEN_W, SCREEN_H);

    fire_icon_img = al_load_bitmap("fire-icon.png");
    if(!fire_icon_img)
        game_abort("failed to load image: fire-icon.png");

    enemy_stage2_img = al_load_bitmap("enemy-2.png");
    if(!enemy_stage2_img)
        game_abort("failed to load image: enemy-2.png");

    /* stage 3 resources */
    stage3_img_backgroud = load_bitmap_resized("stage3-bg.jpg", SCREEN_W, SCREEN_H);

    /* boss resources */
    boss_stage_img_backgroud = load_bitmap_resized("boss-scene.jpg", SCREEN_W, SCREEN_H);

    boss_img = al_load_bitmap("boss.png");
    if(!boss_img)
        game_abort("failed to load image: boss.png");

    boss_laser_img = al_load_bitmap("boss_laser.png");
    if(!boss_laser_img)
        game_abort("failed to load image: boss_laser.png");

    boss_minion_img = al_load_bitmap("boss_minion.png");
    if(!boss_minion_img)
        game_abort("failed to load image: boss_minion.png");

    boss_battle_bgm = al_load_sample("boss_stage_sound.ogg");
    if(!boss_battle_bgm)
        game_abort("failed to load image: boss_stage_sound.ogg");

    /* transition resources */
    speedicon_img = al_load_bitmap("speedicon.png");
    if(!speedicon_img)
        game_abort("failed to load image: speedicon.png");

    speedicon2_img = al_load_bitmap("smallfighter-icon.png");
    if(!speedicon2_img)
        game_abort("failed to load image: smallfighter-icon.png");

    ebullet_icon_img = al_load_bitmap("ebullet-icon.png");
    if(!ebullet_icon_img)
        game_abort("failed to load image: ebullet-icon.png");

    bullet_icon = al_load_bitmap("bullet-icon.png");
    if(!bullet_icon)
        game_abort("failed to load image: bullet-icon.png");

    randomicon_img = al_load_bitmap("random-icon.png");
    if(!randomicon_img)
        game_abort("failed to load image: random-icon.png");

    hearticon_img = al_load_bitmap("heart-icon.png");
    if(!hearticon_img)
        game_abort("failed to load image: heart-icon.png");

    transision_bgm = al_load_sample("transision_sound.ogg");
    if(!transision_bgm)
        game_abort("failed to load sample: transision_sound.ogg");

    bought_bgm = al_load_sample("bought.ogg");
    if(!bought_bgm)
        game_abort("failed to load sample: bought.ogg");

    boss_icon_img = al_load_bitmap("boss_icon.png");
    if(!boss_icon_img)
        game_abort("failed to load image: boss_icon.png");

    boss_minion_icon_img = al_load_bitmap("boss_minion_icon.png");
    if(!boss_minion_icon_img)
        game_abort("failed to load image: boss_minion_icon.png");

    warning_img = al_load_bitmap("warning.png");
    if(!warning_img)
        game_abort("failed to load image: warning.png");

    /* collision sound resources */
    enemy_die_bgm = al_load_sample("enemy_die.ogg");
    if(!enemy_die_bgm)
        game_abort("failed to load sample: enemy_die.ogg");

    plane_die_bgm = al_load_sample("die_sound.ogg");
    if(!plane_die_bgm)
        game_abort("failed to load sample: die_sound.ogg");

    boss_damaged_bgm = al_load_sample("boss_damaged.ogg");
    if(!boss_damaged_bgm)
        game_abort("failed to load sample: boss_damaged.ogg");

    laser_bgm = al_load_sample("laser_sound.ogg");
    if(!laser_bgm)
        game_abort("failed to load sample: laser_sound.ogg");

    /* win and loss condition */
    win_bgm = al_load_sample("winning_sound.ogg");
    if(!win_bgm)
        game_abort("failed to load sample: winning_sound.ogg");

    loss_bgm = al_load_sample("lossing_sound.ogg");
    if(!loss_bgm)
        game_abort("failed to load sample: lossing_sound.ogg");

    // Change to first scene.
    game_change_scene(SCENE_MENU);
}

void game_start_event_loop(void)
{
    bool done = false;
    ALLEGRO_EVENT event;
    int redraws = 0;
    while (!done)
    {
        al_wait_for_event(game_event_queue, &event);
        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            game_log("Window close button clicked");
            done = true;
        }
        else if (event.type == ALLEGRO_EVENT_TIMER)
        {
            if (event.timer.source == game_update_timer)
                redraws++;
        }
        else if (event.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            game_log("Key with keycode %d down", event.keyboard.keycode);
            key_state[event.keyboard.keycode] = true;
            on_key_down(event.keyboard.keycode);
        }
        else if (event.type == ALLEGRO_EVENT_KEY_UP)
        {
            game_log("Key with keycode %d up", event.keyboard.keycode);
            key_state[event.keyboard.keycode] = false;
        }
        else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
        {
            game_log("Mouse button %d down at (%d, %d)", event.mouse.button, event.mouse.x, event.mouse.y);
            mouse_state[event.mouse.button] = true;
            on_mouse_down(event.mouse.button, event.mouse.x, event.mouse.y);
        }
        else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
        {
            game_log("Mouse button %d up at (%d, %d)", event.mouse.button, event.mouse.x, event.mouse.y);
            mouse_state[event.mouse.button] = false;
        }
        else if (event.type == ALLEGRO_EVENT_MOUSE_AXES)
        {
            if (event.mouse.dx != 0 || event.mouse.dy != 0)
            {
                game_log("Mouse move to (%d, %d)", event.mouse.x, event.mouse.y);
                mouse_x = event.mouse.x;
                mouse_y = event.mouse.y;
            }
            else if (event.mouse.dz != 0)
            {
                game_log("Mouse scroll at (%d, %d) with delta %d", event.mouse.x, event.mouse.y, event.mouse.dz);
            }
        }
        // TODO: Process more events and call callbacks by adding more
        // entries inside Scene.

        if (redraws > 0 && al_is_event_queue_empty(game_event_queue))
        {
            if (redraws > 1)
                game_log("%d frame(s) dropped", redraws - 1);
            game_update();
            game_draw();
            redraws = 0;
        }
    }
}

void game_update(void)
{
    if (active_scene == SCENE_START || active_scene == STAGE_2 || active_scene == STAGE_3)
    {
        int i;

        plane.vx = plane.vy = 0;
        for(i = 0; i < MAX_LIVES; i++)
        {
            hearts[i].vx = 0;
            hearts[i].vy = 0;
        }

        if (key_state[ALLEGRO_KEY_UP] || key_state[ALLEGRO_KEY_W])
        {
            plane.vy -= 1;
            hearts[0].vy -= 1;
            hearts[1].vy -= 1;
            hearts[2].vy -= 1;
            hearts[3].vy -= 1;
            hearts[4].vy -= 1;
            hearts[5].vy -= 1;
        }
        if (key_state[ALLEGRO_KEY_DOWN] || key_state[ALLEGRO_KEY_S])
        {
            plane.vy += 1;
            hearts[0].vy += 1;
            hearts[1].vy += 1;
            hearts[2].vy += 1;
            hearts[3].vy += 1;
            hearts[4].vy += 1;
            hearts[5].vy += 1;
        }
        if (key_state[ALLEGRO_KEY_LEFT] || key_state[ALLEGRO_KEY_A])
        {
            plane.vx -= 1;
            hearts[0].vx -= 1;
            hearts[1].vx -= 1;
            hearts[2].vx -= 1;
            hearts[3].vx -= 1;
            hearts[4].vx -= 1;
            hearts[5].vx -= 1;
        }
        if (key_state[ALLEGRO_KEY_RIGHT] || key_state[ALLEGRO_KEY_D])
        {
            plane.vx += 1;
            hearts[0].vx += 1;
            hearts[1].vx += 1;
            hearts[2].vx += 1;
            hearts[3].vx += 1;
            hearts[4].vx += 1;
            hearts[5].vx += 1;
        }

        plane.y += plane.vy  * plane_speed * (plane.vx ? 0.71f : 1);
        plane.x += plane.vx * plane_speed * (plane.vy ? 0.71f : 1);

        for(i = 0; i < MAX_LIVES; i++)
        {
            hearts[i].y += hearts[i].vy  * plane_speed * (hearts[i].vx ? 0.71f : 1);
            hearts[i].x += hearts[i].vx * plane_speed * (hearts[i].vy ? 0.71f : 1);
        }

        // Border of Plane.
        if (plane.x - plane.w/2 < 0)
        {
            plane.x = plane.w/2;
            hearts[0].x = plane.x - plane.w/2;
            hearts[1].x = plane.x;
            hearts[2].x = plane.x + plane.w/2;
            hearts[3].x = plane.x - plane.w/2;
            hearts[4].x = plane.x;
            hearts[5].x = plane.x + plane.w/2;
        }
        else if (plane.x + plane.w/2> SCREEN_W)
        {
            plane.x = SCREEN_W - plane.w/2;
            hearts[0].x = plane.x - plane.w/2;
            hearts[1].x = plane.x;
            hearts[2].x = plane.x + plane.w/2;
            hearts[3].x = plane.x - plane.w/2;
            hearts[4].x = plane.x;
            hearts[5].x = plane.x + plane.w/2;
        }
        if (plane.y - plane.h/2 < 0)
        {
            plane.y = plane.h/2;
            hearts[0].y = plane.y + plane.h/2 + 10;
            hearts[1].y = plane.y + plane.h/2 + 10;
            hearts[2].y = plane.y + plane.h/2 + 10;
            hearts[3].y = plane.y + plane.h/2 + 10;
            hearts[4].y = plane.y + plane.h/2 + 10;
            hearts[5].y = plane.y + plane.h/2 + 10;
        }
        else if (plane.y + plane.h/2 + 10 > SCREEN_H)
        {
            plane.y = SCREEN_H - plane.w/2 - 10;
            hearts[0].y = plane.y + plane.h/2 + 10;
            hearts[1].y = plane.y + plane.h/2 + 10;
            hearts[2].y = plane.y + plane.h/2 + 10;
            hearts[3].y = plane.y + plane.h/2 + 10;
            hearts[4].y = plane.y + plane.h/2 + 10;
            hearts[5].y = plane.y + plane.h/2 + 10;
        }

        // Plane resurrection
        double now_dead = al_get_time();
        if(now_dead - last_died_timestamp >= DEAD_TIME)
        {
            plane.hidden = false;
        }

        //Bullet coordinate
        for (i = 0; i < MAX_BULLET; i++)
        {
            if (bullets[i].hidden == true)
                continue;

            bullets[i].x += bullets[i].vx;
            bullets[i].y += bullets[i].vy;

            if (bullets[i].y < 0)
                bullets[i].hidden = true;
        }

        double now = al_get_time();
        if (key_state[ALLEGRO_KEY_SPACE] && now - last_shoot_timestamp >= MAX_COOLDOWN)
        {
            if(!plane.hidden)
            {
                for (i = 0; i < MAX_BULLET; i++)
                {
                    if (bullets[i].hidden == true)
                        break;
                }

                if(plane1 == 1 || plane2 == 1 || plane4 == 1 || plane5 == 1)
                {
                    if (i < MAX_BULLET)
                    {
                        last_shoot_timestamp = now;
                        bullets[i].hidden = false;
                        al_play_sample(bullet_sound_bgm, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
                        bullets[i].x = plane.x;
                        bullets[i].y = plane.y;
                    }
                }

                if(plane3 == 1)
                {
                    if (i < MAX_BULLET)
                    {
                        last_shoot_timestamp = now;
                        bullets[i].hidden = false;
                        al_play_sample(bullet_sound_bgm, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
                        bullets[i].x = plane.x;
                        bullets[i].y = plane.y - plane.h/2;

                        if(i+1 < MAX_BULLET)
                        {
                            bullets[i+1].hidden = false;
                            al_play_sample(bullet_sound_bgm, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
                            bullets[i+1].x = plane.x - plane.w/2;
                            bullets[i+1].y = plane.y - plane.h/2;
                        }

                        if(i+2 < MAX_BULLET)
                        {
                            bullets[i+2].hidden = false;
                            al_play_sample(bullet_sound_bgm, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
                            bullets[i+2].x = plane.x + plane.w/2;
                            bullets[i+2].y = plane.y - plane.h/2;
                        }

                    }
                }
            }

        }

        /* plane's ultimate */
        for (i = 0; i < MAX_ULTI; i++)
        {
            if (ulti[i].hidden == true)
                continue;

            ulti[i].x += ulti[i].vx;
            ulti[i].y += ulti[i].vy;

            if (ulti[i].y < 0)
                ulti[i].hidden = true;
        }

        double ulti_time = al_get_time();
        if (key_state[ALLEGRO_KEY_F] && ulti_counter > 0 && ulti_time - last_ulti_timestamp >= MAX_ULTI_COOLDOWN)
        {
            if(!plane.hidden)
            {
                for (i = 0; i < MAX_ULTI; i++)
                {
                    if (ulti[i].hidden == true)
                        break;
                }

                if (i < MAX_ULTI)
                {
                    last_ulti_timestamp = ulti_time;
                    ulti[i].hidden = false;
                    al_play_sample(ulti_bgm, 1, 0.0, 2.5, ALLEGRO_PLAYMODE_ONCE, &ulti_bgm_id);
                    ulti[i].x = plane.x;
                    ulti[i].y = plane.y + plane.w/2;
                }

                ulti_counter -= 1;
            }

        }

        /* Enemy's coordinate */
        for (i = 0; i < MAX_ENEMY; i++)
        {
            if (enemies[i].hidden == true)
                continue;

            enemies[i].x += enemies[i].vx;
            enemies[i].y += enemies[i].vy;

            if (enemies[i].y > SCREEN_H)
                enemies[i].hidden = true;
        }

        for (i = 0; i < MAX_ENEMY; i++)
        {
            if (enemies[i].hidden == true)
                break;
        }

        if (i < MAX_ENEMY)
        {
            enemies[i].hidden = false;
            enemies[i].x = enemies[i].w / 2 + (float)rand() / RAND_MAX * (SCREEN_W - enemies[i].w);
            enemies[i].y = 0;
        }

        int j;
        /* Enemies bullet */
        if(scene_counter != 0)
        {
            for(i = 0; i < MAX_EBULLET; i++)
            {
                if(e_bullets[i].hidden == true)
                    continue;

                e_bullets[i].x += e_bullets[i].vx;
                e_bullets[i].y += e_bullets[i].vy;

                if(e_bullets[i].y >= SCREEN_H)
                    e_bullets[i].hidden = true;
            }

            double now_attack = al_get_time();
            for(i = 0; i < MAX_ENEMY; i++)
            {
                if(!enemies[i].hidden && now_attack - last_attack_timestamp >= MAX_E_COOLDOWN)
                {
                    for(j = 0; j < MAX_EBULLET; j++)
                    {
                        if(e_bullets[j].hidden)
                            break;
                    }

                    if(j < MAX_EBULLET)
                    {
                        last_attack_timestamp = now_attack;
                        e_bullets[j].hidden = false;
                        e_bullets[j].x = enemies[j].x;
                        e_bullets[j].y = enemies[j].y + enemies[j].h/2;
                    }
                }
            }
        }


        /* Collision detection */
        for(i = 0; i < MAX_BULLET; i++)
        {
            if(!bullets[i].hidden)
            {
                for(j = 0; j < MAX_ENEMY; j++)
                {
                    if(!enemies[j].hidden)
                    {
                        if(bullets[i].x > (enemies[j].x - enemies[j].w/2) && bullets[i].x < (enemies[j].x + enemies[j].w/2)
                                && bullets[i].y > (enemies[j].y - enemies[j].h/2) && bullets[i].y < (enemies[j].y + enemies[j].h/2))
                        {
                            al_play_sample(enemy_die_bgm, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, &enemy_die_bgm_id);
                            plane.score += (100*score_buff);
                            plane.money += (10*money_buff);
                            bullets[i].hidden = true;
                            enemies[j].hidden = true;
                            break;
                        }
                    }
                }
            }
        }

        double now_plane = al_get_time();
        for(i = 0; i < MAX_ENEMY; i++)
        {
            if(!plane.hidden)
            {
                if(!enemies[i].hidden)
                {
                    if(enemies[i].x - enemies[i].w/2 < plane.x + plane.w/2 && enemies[i].x + enemies[i].w/2 > plane.x - plane.w/2
                            && enemies[i].y - enemies[i].h/2 < plane.y + plane.h/2 && enemies[i].y + enemies[i].h/2 > plane.y - plane.h/2)
                    {
                        last_died_timestamp = now_plane;
                        al_play_sample(plane_die_bgm, 1, 0.0, 3.0, ALLEGRO_PLAYMODE_ONCE, &plane_die_bgm_id);
                        plane.hidden = true;
                        plane.hp -= 1;
                        break;
                    }
                }
            }
        }

        now_plane = al_get_time();
        for(i = 0; i < MAX_EBULLET; i++)
        {
            if(!plane.hidden)
            {
                if(!e_bullets[i].hidden)
                {
                    if(e_bullets[i].x > (plane.x - plane.w/2) && e_bullets[i].x < (plane.x + plane.w/2)
                            && e_bullets[i].y > (plane.y - plane.h/2) && e_bullets[i].y < (plane.y + plane.h/2))
                    {
                        last_died_timestamp = now_plane;
                        al_play_sample(plane_die_bgm, 1, 0.0, 3.0, ALLEGRO_PLAYMODE_ONCE, &plane_die_bgm_id);
                        e_bullets[i].hidden = true;
                        plane.hidden = true;
                        plane.hp -= 1;
                        break;
                    }
                }
            }
        }

        if(scene_counter > 0)
        {
            for(i = 0; i  < MAX_BULLET; i++)
            {
                if(!bullets[i].hidden)
                {
                    for(j = 0; j < MAX_EBULLET; j++)
                    {
                        if(!e_bullets[j].hidden)
                        {
                            if(bullets[i].x - bullets[i].w/2 < e_bullets[j].x + e_bullets[j].w/2 && bullets[i].x + bullets[i].w/2 > e_bullets[j].x - e_bullets[j].w/2
                                    && bullets[i].y - bullets[i].h/2 < e_bullets[j].y + e_bullets[j].h/2 && bullets[i].y + bullets[i].h/2 > e_bullets[j].y - e_bullets[j].h/2)
                            {
                                al_play_sample(bullet_clashing_bgm, 1, 0.0, 1.5, ALLEGRO_PLAYMODE_ONCE, &bullet_clashing_bgm_id);
                                bullets[i].hidden = true;
                                e_bullets[j].hidden = true;
                                plane.money += (5*money_buff);
                                break;
                            }
                        }
                    }
                }
            }
        }


        for(i = 0; i < MAX_ENEMY; i++)
        {
            if(!enemies[i].hidden)
            {
                for(j = 0; j < MAX_ULTI; j++)
                {
                    if(!ulti[j].hidden)
                    {
                        if(ulti[j].x + ulti[j].w/2 > enemies[i].x - enemies[i].w/2 && ulti[j].x - ulti[j].w/2 < enemies[i].x + enemies[i].w/2
                                && ulti[j].y + ulti[j].h/2 > enemies[i].y - enemies[i].h/2 && ulti[j].y - ulti[j].h/2 < enemies[i].y + enemies[i].h/2)
                        {
                            enemies[i].hidden = true;
                            plane.score += (100*score_buff);
                            plane.money += (10*money_buff);
                        }
                    }
                }
            }
        }

        for(i = 0; i < MAX_EBULLET; i++)
        {
            if(!e_bullets[i].hidden)
            {
                for(j = 0; j < MAX_ULTI; j++)
                {
                    if(!ulti[j].hidden)
                    {
                        if(ulti[j].x + ulti[j].w/2 > e_bullets[i].x - e_bullets[i].w/2 && ulti[j].x - ulti[j].w/2 < e_bullets[i].x + e_bullets[i].w/2
                                && ulti[j].y + ulti[j].h/2 > e_bullets[i].y - e_bullets[i].h/2 && ulti[j].y - ulti[j].h/2 < e_bullets[i].y + e_bullets[i].h/2)
                        {
                            e_bullets[i].hidden = true;
                        }
                    }
                }
            }
        }

        /* losing condition */
        if(plane.hp <= 25)
            hearts[5].hidden = true;
        if(plane.hp <= 20)
            hearts[4].hidden = true;
        if(plane.hp <= 15)
            hearts[3].hidden = true;
        if(plane.hp <= 10)
            hearts[2].hidden = true;
        if(plane.hp <= 5)
            hearts[0].hidden = true;
        if(plane.hp <= 0)
        {
            hearts[1].hidden = true;
            game_change_scene(SCENE_LOSS);
        }

        /* wining condition */
        if(plane.score >= 2000 && scene_counter < 1)
        {
            b_score = plane.score;
            b_money = plane.money;
            lost_health = ori_hp - plane.hp;
            scene_counter += 1;
            game_change_scene(SCENE_TRANSITION_12);
        }

        if(plane.score >= 4000 && scene_counter < 2)
        {
            b_score = plane.score;
            b_money = plane.money;
            lost_health = ori_hp - plane.hp;
            scene_counter += 1;
            game_change_scene(SCENE_TRANSITION_23);
        }

        if(plane.score >= 6000 && scene_counter < 3)
        {
            b_score = plane.score;
            b_money = plane.money;
            lost_health = ori_hp - plane.hp;
            scene_counter += 1;
            game_change_scene(SCENE_TRANSITION_34);
        }


    }
    else if (active_scene == BOSS_STAGE)
    {
        int i;

        plane.vx = plane.vy = 0;
        for(i = 0; i < MAX_LIVES; i++)
        {
            hearts[i].vx = 0;
            hearts[i].vy = 0;
        }

        if (key_state[ALLEGRO_KEY_UP] || key_state[ALLEGRO_KEY_W])
        {
            plane.vy -= 1;
            hearts[0].vy -= 1;
            hearts[1].vy -= 1;
            hearts[2].vy -= 1;
            hearts[3].vy -= 1;
            hearts[4].vy -= 1;
            hearts[5].vy -= 1;
        }
        if (key_state[ALLEGRO_KEY_DOWN] || key_state[ALLEGRO_KEY_S])
        {
            plane.vy += 1;
            hearts[0].vy += 1;
            hearts[1].vy += 1;
            hearts[2].vy += 1;
            hearts[3].vy += 1;
            hearts[4].vy += 1;
            hearts[5].vy += 1;
        }
        if (key_state[ALLEGRO_KEY_LEFT] || key_state[ALLEGRO_KEY_A])
        {
            plane.vx -= 1;
            hearts[0].vx -= 1;
            hearts[1].vx -= 1;
            hearts[2].vx -= 1;
            hearts[3].vx -= 1;
            hearts[4].vx -= 1;
            hearts[5].vx -= 1;
        }
        if (key_state[ALLEGRO_KEY_RIGHT] || key_state[ALLEGRO_KEY_D])
        {
            plane.vx += 1;
            hearts[0].vx += 1;
            hearts[1].vx += 1;
            hearts[2].vx += 1;
            hearts[3].vx += 1;
            hearts[4].vx += 1;
            hearts[5].vx += 1;
        }

        plane.y += plane.vy  * plane_speed * (plane.vx ? 0.71f : 1);
        plane.x += plane.vx * plane_speed * (plane.vy ? 0.71f : 1);

        for(i = 0; i < MAX_LIVES; i++)
        {
            hearts[i].y += hearts[i].vy  * plane_speed * (hearts[i].vx ? 0.71f : 1);
            hearts[i].x += hearts[i].vx * plane_speed * (hearts[i].vy ? 0.71f : 1);
        }

        // Border of Plane.
        if (plane.x - plane.w/2 < 0)
        {
            plane.x = plane.w/2;
            hearts[0].x = plane.x - plane.w/2;
            hearts[1].x = plane.x;
            hearts[2].x = plane.x + plane.w/2;
            hearts[3].x = plane.x - plane.w/2;
            hearts[4].x = plane.x;
            hearts[5].x = plane.x + plane.w/2;
        }
        else if (plane.x + plane.w/2> SCREEN_W)
        {
            plane.x = SCREEN_W - plane.w/2;
            hearts[0].x = plane.x - plane.w/2;
            hearts[1].x = plane.x;
            hearts[2].x = plane.x + plane.w/2;
            hearts[3].x = plane.x - plane.w/2;
            hearts[4].x = plane.x;
            hearts[5].x = plane.x + plane.w/2;
        }
        if (plane.y - plane.h/2 < 0)
        {
            plane.y = plane.h/2;
            hearts[0].y = plane.y + plane.h/2 + 10;
            hearts[1].y = plane.y + plane.h/2 + 10;
            hearts[2].y = plane.y + plane.h/2 + 10;
            hearts[3].y = plane.y + plane.h/2 + 10;
            hearts[4].y = plane.y + plane.h/2 + 10;
            hearts[5].y = plane.y + plane.h/2 + 10;
        }
        else if (plane.y + plane.h/2 + 10 > SCREEN_H)
        {
            plane.y = SCREEN_H - plane.w/2 - 10;
            hearts[0].y = plane.y + plane.h/2 + 10;
            hearts[1].y = plane.y + plane.h/2 + 10;
            hearts[2].y = plane.y + plane.h/2 + 10;
            hearts[3].y = plane.y + plane.h/2 + 10;
            hearts[4].y = plane.y + plane.h/2 + 10;
            hearts[5].y = plane.y + plane.h/2 + 10;
        }


        // Plane resurrection
        double now_dead = al_get_time();
        if(now_dead - last_died_timestamp >= DEAD_TIME)
        {
            plane.hidden = false;
        }

        //Bullet coordinate
        for (i = 0; i < MAX_BULLET; i++)
        {
            if (bullets[i].hidden == true)
                continue;

            bullets[i].x += bullets[i].vx;
            bullets[i].y += bullets[i].vy;

            if (bullets[i].y < 0)
                bullets[i].hidden = true;
        }

        double now = al_get_time();
        if (key_state[ALLEGRO_KEY_SPACE] && now - last_shoot_timestamp >= MAX_COOLDOWN)
        {
            if(!plane.hidden)
            {
                for (i = 0; i < MAX_BULLET; i++)
                {
                    if (bullets[i].hidden == true)
                        break;
                }

                if(plane1 == 1 || plane2 == 1 || plane4 == 1 || plane5 == 1)
                {
                    if (i < MAX_BULLET)
                    {
                        last_shoot_timestamp = now;
                        bullets[i].hidden = false;
                        al_play_sample(bullet_sound_bgm, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
                        bullets[i].x = plane.x;
                        bullets[i].y = plane.y;
                    }
                }

                if(plane3 == 1)
                {
                    if (i < MAX_BULLET)
                    {
                        last_shoot_timestamp = now;
                        bullets[i].hidden = false;
                        al_play_sample(bullet_sound_bgm, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
                        bullets[i].x = plane.x;
                        bullets[i].y = plane.y - plane.h/2;

                        if(i+1 < MAX_BULLET)
                        {
                            bullets[i+1].hidden = false;
                            al_play_sample(bullet_sound_bgm, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
                            bullets[i+1].x = plane.x - plane.w/2;
                            bullets[i+1].y = plane.y - plane.h/2;
                        }

                        if(i+2 < MAX_BULLET)
                        {
                            bullets[i+2].hidden = false;
                            al_play_sample(bullet_sound_bgm, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
                            bullets[i+2].x = plane.x + plane.w/2;
                            bullets[i+2].y = plane.y - plane.h/2;
                        }

                    }
                }

            }

        }

        /* plane's ultimate */
        for (i = 0; i < MAX_ULTI; i++)
        {
            if (ulti[i].hidden == true)
                continue;

            ulti[i].x += ulti[i].vx;
            ulti[i].y += ulti[i].vy;

            if (ulti[i].y < 0)
                ulti[i].hidden = true;
        }

        double ulti_time = al_get_time();
        if (key_state[ALLEGRO_KEY_F] && ulti_counter > 0 && ulti_time - last_ulti_timestamp >= MAX_ULTI_COOLDOWN)
        {
            if(!plane.hidden)
            {
                for (i = 0; i < MAX_ULTI; i++)
                {
                    if (ulti[i].hidden == true)
                        break;
                }

                if (i < MAX_ULTI)
                {
                    last_ulti_timestamp = ulti_time;
                    al_play_sample(ulti_bgm, 1, 0.0, 2.5, ALLEGRO_PLAYMODE_ONCE, &ulti_bgm_id);
                    ulti[i].hidden = false;
                    ulti[i].x = plane.x;
                    ulti[i].y = plane.y + plane.w/2;
                }

                ulti_counter -= 1;
            }

        }

        /* Boss movement */
        boss.x += boss.vx;
        boss.y += boss.vy;

        if(boss.x - boss.w/2 > SCREEN_W)
            boss.x = -100;

        /* Boss skill */
        double laser_time = al_get_time();
        if(laser_time - last_laser_timestamp >= BOSS_COOLDOWN && boss_laser.hidden)
        {
            last_laser_timestamp = laser_time;
            boss_laser.hidden = false;
            al_play_sample(laser_bgm, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, &laser_bgm_id);
            boss_laser.x = boss.x;
            boss_laser.y = boss.y + boss.h/2;
        }

        if(!boss_laser.hidden)
        {
            boss_laser.x += boss_laser.vx;
            boss_laser.y += boss_laser.vy;
        }

        if(boss_laser.y - boss_laser.w/2 >= SCREEN_H)
            boss_laser.hidden = true;

        /* Boss minions movement */
        for (i = 0; i < MAX_MINION; i++)
        {
            if (boss_minion[i].hidden == true)
                continue;

            boss_minion[i].x += boss_minion[i].vx;
            boss_minion[i].y += boss_minion[i].vy;

            if(boss_minion[i].x + boss_minion[i].w/2 < 0 || boss_minion[i].x - boss_minion[i].w/2 > SCREEN_W
                    || boss_minion[i].y + boss_minion[i].h/2 < 0 || boss_minion[i].y - boss_minion[i].h/2  > SCREEN_H )
            {
                boss_minion[i].x = boss_minion[i].w / 2 + (float)rand() / RAND_MAX * (SCREEN_W - boss_minion[i].w);
                boss_minion[i].y = 180;
            }
        }

        for (i = 0; i < MAX_MINION; i++)
        {
            if (boss_minion[i].hidden == true)
                break;
        }

        if (i < MAX_MINION)
        {
            boss_minion[i].hidden = false;
            boss_minion[i].x = boss_minion[i].w / 2 + (float)rand() / RAND_MAX * (SCREEN_W - boss_minion[i].w);
            boss_minion[i].y = 0;
        }

        /* Collision detection */
        double now_plane = al_get_time();
        for(i = 0; i < MAX_MINION; i++)
        {
            if(!plane.hidden)
            {
                if(!boss_minion[i].hidden)
                {
                    if(boss_minion[i].x - boss_minion[i].w/2 < plane.x + plane.w/2 && boss_minion[i].x + boss_minion[i].w/2 > plane.x - plane.w/2
                            && boss_minion[i].y - boss_minion[i].h/2 < plane.y + plane.h/2 && boss_minion[i].y + boss_minion[i].h/2 > plane.y - plane.h/2)
                    {
                        last_died_timestamp = now_plane;
                        al_play_sample(plane_die_bgm, 1, 0.0, 3.0, ALLEGRO_PLAYMODE_ONCE, &plane_die_bgm_id);
                        plane.hidden = true;
                        plane.hp -= 1;
                        break;
                    }
                }
            }
        }

        now_plane = al_get_time();
        if(!boss_laser.hidden)
        {
            if(!plane.hidden)
            {
                if(boss_laser.x + boss_laser.w/2 - 75 > plane.x - plane.w/2 && boss_laser.x - boss_laser.w/2 + 75 < plane.x + plane.w/2
                        && boss_laser.y + boss_laser.h/2 > plane.y - plane.h/2 && boss_laser.y - boss_laser.h/2 < plane.y + plane.h/2)
                {
                    last_died_timestamp = now_plane;
                    al_play_sample(plane_die_bgm, 1, 0.0, 3.0, ALLEGRO_PLAYMODE_ONCE, &plane_die_bgm_id);
                    plane.hidden = true;
                    plane.hp -= 3;
                }
            }
        }

        int j;
        for(i = 0; i < MAX_BULLET; i++)
        {
            if(!bullets[i].hidden)
            {
                for(j = 0; j < MAX_MINION; j++)
                {
                    if(!boss_minion[j].hidden)
                    {
                        if(bullets[i].x > (boss_minion[j].x - boss_minion[j].w/2) && bullets[i].x < (boss_minion[j].x + boss_minion[j].w/2)
                                && bullets[i].y > (boss_minion[j].y - boss_minion[j].h/2) && bullets[i].y < (boss_minion[j].y + boss_minion[j].h/2))
                        {
                            plane.score += (100*score_buff);
                            plane.money += (10*money_buff);
                            bullets[i].hidden = true;
                            boss_minion[j].hidden = true;
                            al_play_sample(enemy_die_bgm, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, &enemy_die_bgm_id);
                            break;
                        }
                    }
                }
            }
        }

        for(i = 0; i < MAX_MINION; i++)
        {
            if(!boss_minion[i].hidden)
            {
                for(j = 0; j < MAX_ULTI; j++)
                {
                    if(!ulti[j].hidden)
                    {
                        if(ulti[j].x + ulti[j].w/2 > boss_minion[i].x - boss_minion[i].w/2 && ulti[j].x - ulti[j].w/2 < boss_minion[i].x + boss_minion[i].w/2
                                && ulti[j].y + ulti[j].h/2 > boss_minion[i].y - boss_minion[i].h/2 && ulti[j].y - ulti[j].h/2 < boss_minion[i].y + boss_minion[i].h/2)
                        {
                            boss_minion[i].hidden = true;
                            plane.score += (100*score_buff);
                            plane.money += (10*money_buff);
                        }
                    }
                }
            }
        }

        for(i = 0; i < MAX_BULLET; i++)
        {
            if(!bullets[i].hidden)
            {
                if(!boss.hidden)
                {
                    if(boss.x - boss.w/2 < bullets[i].x + bullets[i].w/2 && boss.x + boss.w/2 > bullets[i].x - bullets[i].w/2
                            && boss.y - boss.h/2 < bullets[i].y + bullets[i].h/2 && boss.y + boss.h/2 > bullets[i].y - bullets[i].h/2)
                    {
                        al_play_sample(boss_damaged_bgm, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, &boss_damaged_bgm_id);
                        bullets[i].hidden = true;
                        boss.hp -= 1;
                        break;
                    }
                }
            }
        }

        for(i = 0; i < MAX_ULTI; i++)
        {
            if(!ulti[i].hidden)
            {
                if(!boss.hidden)
                {
                    if(boss.x < ulti[i].x + ulti[i].w/2 && boss.x > ulti[i].x - ulti[i].w/2
                            && boss.y < ulti[i].y + ulti[i].h/2 && boss.y > ulti[i].y - ulti[i].h/2)
                    {
                        al_play_sample(boss_damaged_bgm, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, &boss_damaged_bgm_id);
                        boss.hp -= 10;
                        ulti[i].hidden = true;
                    }
                }
            }
        }

        for(i = 0; i < MAX_ULTI; i++)
        {
            if(!ulti[i].hidden)
            {
                if(ulti[i].y + ulti[i].h/2 <= 0)
                {
                    ulti[i].hidden = true;
                }
            }
        }

        for(i = 0; i < MAX_MINION; i++)
        {
            if(!boss_laser.hidden)
            {
                if(!boss_minion[i].hidden)
                {
                    if(boss_minion[i].x - boss_minion[i].w/2 < boss_laser.x + boss_laser.w/2 - 75 && boss_minion[i].x + boss_minion[i].w/2 > boss_laser.x - boss_laser.w/2 + 75
                            && boss_minion[i].y - boss_minion[i].h/2 < boss_laser.y + boss_laser.h/2 && boss_minion[i].y + boss_minion[i].h/2 > boss_laser.y - boss_laser.h/2)
                    {
                        boss_minion[i].hidden = true;
                        break;
                    }
                }
            }
        }

        if (!plane.hidden)
        {
            if (!boss.hidden)
            {
                if (boss.x + boss.w/2 > plane.x - plane.w/2 && boss.x - boss.w/2 < plane.x + plane.w/2
                    && boss.y + boss.h/2 > plane.y - plane.h/2 && boss.y - boss.h/2 < plane.y + plane.h/2)
                {
                    plane.hidden = true;
                    plane.hp -= 30;
                }
            }
        }

        /* losing condition */
        if(plane.hp <= 25)
            hearts[5].hidden = true;
        if(plane.hp <= 20)
            hearts[4].hidden = true;
        if(plane.hp <= 15)
            hearts[3].hidden = true;
        if(plane.hp <= 10)
            hearts[2].hidden = true;
        if(plane.hp <= 5)
            hearts[0].hidden = true;
        if(plane.hp <= 0)
        {
            hearts[1].hidden = true;
            game_change_scene(SCENE_LOSS);
        }

        /* winning condition */
        if(boss.hp <= 0)
        {
            game_change_scene(SCENE_WIN);
        }
    }
}

void game_draw(void)
{
    if (active_scene == SCENE_MENU)
    {
        al_draw_bitmap(main_img_background, 0, 0, 0);
        al_draw_text(font_pirulen_32, al_map_rgb(255, 255, 255), SCREEN_W / 2, 30, ALLEGRO_ALIGN_CENTER, "Space Shooter");
        al_draw_text(font_pirulen_24, al_map_rgb(255, 255, 255), 20, SCREEN_H - 50, 0, "Press enter key to start");
        al_draw_textf(font_pirulen_24, al_map_rgb(255, 255, 255), 20, SCREEN_H - 80, 0, "Highscore : %d", highscore);

        if (pnt_in_rect(mouse_x, mouse_y, SCREEN_W - 48, 10, 38, 38))
            al_draw_bitmap(img_settings2, SCREEN_W - 48, 10, 0);
        else
            al_draw_bitmap(img_settings, SCREEN_W - 48, 10, 0);
    }

    else if (active_scene == SCENE_START)
    {
        int i;
        al_draw_bitmap(start_img_background, 0, 0, 0);
        al_draw_textf(font_deltarune_16, al_map_rgb(0, 0, 0), 20, SCREEN_H - 50, 0, "score: %lld", plane.score);
        al_draw_textf(font_deltarune_16, al_map_rgb(0, 0, 0), 20, SCREEN_H - 70, 0, "Health remaining: %d", plane.hp);
        al_draw_textf(font_gameover_45, al_map_rgb(0, 0, 0), 20, SCREEN_H - 100, 0, "$%d", plane.money);

        for (i = 0; i < MAX_BULLET; i++)
            draw_movable_object(bullets[i]);
        draw_movable_object(plane);
        for(i = 0; i < MAX_LIVES; i++)
            draw_movable_object(hearts[i]);
        for (i = 0; i < MAX_ENEMY; i++)
            draw_movable_object(enemies[i]);

    }

    else if (active_scene == STAGE_2)
    {
        int i;
        al_draw_bitmap(stage2_img_backgroud, 0, 0, 0);
        al_draw_textf(font_deltarune_16, al_map_rgb(255, 255, 255), 20, SCREEN_H - 50, 0, "score: %lld", plane.score);
        al_draw_textf(font_deltarune_16, al_map_rgb(255, 255, 255), 20, SCREEN_H - 70, 0, "Health remaining: %d", plane.hp);
        al_draw_textf(font_gameover_45, al_map_rgb(255, 255, 255), 20, SCREEN_H - 100, 0, "$%d", plane.money);

        for (i = 0; i < MAX_BULLET; i++)
            draw_movable_object(bullets[i]);
        draw_movable_object(plane);
        for(i = 0; i < MAX_LIVES; i++)
            draw_movable_object(hearts[i]);
        for (i = 0; i < MAX_ENEMY; i++)
            draw_movable_object(enemies[i]);
        for(i = 0; i < MAX_EBULLET; i++)
            draw_movable_object(e_bullets[i]);
        for(i = 0; i < MAX_ULTI; i++)
            draw_movable_object(ulti[i]);

        if(ulti_counter >= 1)
        {
            al_draw_text(font_pirulen_12, al_map_rgb(100, 100, 100), plane.x, plane.y + plane.h/2 + 20, ALLEGRO_ALIGN_CENTER, "Press F to ulti");
            al_draw_bitmap(fire_icon_img, 55, SCREEN_H/2 + 205, 0);
            al_draw_textf(font_deltarune_16, al_map_rgb(255, 255, 255), 80, SCREEN_H/2 + 210, ALLEGRO_ALIGN_LEFT, "x%d", ulti_counter);
        }

    }

    else if(active_scene == STAGE_3)
    {
        int i;
        al_draw_bitmap(stage3_img_backgroud, 0, 0, 0);
        al_draw_textf(font_deltarune_16, al_map_rgb(255, 255, 255), 20, SCREEN_H - 50, 0, "score: %lld", plane.score);
        al_draw_textf(font_deltarune_16, al_map_rgb(255, 255, 255), 20, SCREEN_H - 70, 0, "Health remaining: %d", plane.hp);
        al_draw_textf(font_gameover_45, al_map_rgb(255, 255, 255), 20, SCREEN_H - 100, 0, "$%d", plane.money);

        for (i = 0; i < MAX_BULLET; i++)
            draw_movable_object(bullets[i]);
        draw_movable_object(plane);
        for(i = 0; i < MAX_LIVES; i++)
            draw_movable_object(hearts[i]);
        for (i = 0; i < MAX_ENEMY; i++)
            draw_movable_object(enemies[i]);
        for(i = 0; i < MAX_EBULLET; i++)
            draw_movable_object(e_bullets[i]);
        for(i = 0; i < MAX_ULTI; i++)
            draw_movable_object(ulti[i]);

        if(ulti_counter >= 1)
        {
            al_draw_text(font_pirulen_12, al_map_rgb(100, 100, 100), plane.x, plane.y + plane.h/2 + 20, ALLEGRO_ALIGN_CENTER, "Press F to ulti");
            al_draw_bitmap(fire_icon_img, 55, SCREEN_H/2 + 205, 0);
            al_draw_textf(font_deltarune_16, al_map_rgb(255, 255, 255), 80, SCREEN_H/2 + 210, ALLEGRO_ALIGN_LEFT, "x%d", ulti_counter);
        }
    }

    else if(active_scene == BOSS_STAGE)
    {
        int i;
        al_draw_bitmap(boss_stage_img_backgroud, 0, 0, 0);
        al_draw_textf(font_deltarune_16, al_map_rgb(255, 255, 255), 20, SCREEN_H - 50, 0, "score: %lld", plane.score);
        al_draw_textf(font_deltarune_16, al_map_rgb(255, 255, 255), 20, SCREEN_H - 70, 0, "Health remaining: %d", plane.hp);
        al_draw_textf(font_gameover_45, al_map_rgb(255, 255, 255), 20, SCREEN_H - 100, 0, "$%d", plane.money);
        al_draw_textf(font_gameover_45, al_map_rgb(255, 255, 255), SCREEN_W-5, SCREEN_H-50, ALLEGRO_ALIGN_RIGHT, "BOSS HP: %d", boss.hp);
        al_draw_filled_rectangle(SCREEN_W-(3*boss.hp), SCREEN_H-10, SCREEN_W-5, SCREEN_H-20, al_map_rgb(255, 0, 0));

        draw_movable_object(boss);
        draw_movable_object(boss_laser);
        for(i = 0; i < MAX_MINION; i++)
            draw_movable_object(boss_minion[i]);
        draw_movable_object(plane);
        for (i = 0; i < MAX_BULLET; i++)
            draw_movable_object(bullets[i]);
        for(i = 0; i < MAX_LIVES; i++)
            draw_movable_object(hearts[i]);
        for(i = 0; i < MAX_ULTI; i++)
            draw_movable_object(ulti[i]);

        if(ulti_counter >= 1)
        {
            al_draw_text(font_pirulen_12, al_map_rgb(100, 100, 100), plane.x, plane.y + plane.h/2 + 20, ALLEGRO_ALIGN_CENTER, "Press F to ulti");
            al_draw_bitmap(fire_icon_img, 55, SCREEN_H/2 + 205, 0);
            al_draw_textf(font_deltarune_16, al_map_rgb(255, 255, 255), 80, SCREEN_H/2 + 210, ALLEGRO_ALIGN_LEFT, "x%d", ulti_counter);
        }
    }

    else if (active_scene == SCENE_TRANSITION_12)
    {
        al_draw_bitmap(stage2_img_backgroud, 0, 0, 0);
        al_draw_text(font_gameover_100, al_map_rgb(0, 0, 0), SCREEN_W/2, SCREEN_H/2 - 120, ALLEGRO_ALIGN_CENTER, "STAGE 1 COMPLETED");
        al_draw_text(font_gameover_100, al_map_rgb(0, 0, 0), SCREEN_W/2, SCREEN_H/2 - 60, ALLEGRO_ALIGN_CENTER, "1 out of 4 stages");
        al_draw_line(SCREEN_W/2, SCREEN_H/2 + 10, SCREEN_W/2, SCREEN_H/2 + 180, al_map_rgb(0, 0, 0), 5);
        al_draw_text(font_gameover_55, al_map_rgb(0, 0, 0), SCREEN_W/2 - 120, SCREEN_H/2 + 10, ALLEGRO_ALIGN_CENTER, "Enemy's Upgrade");
        al_draw_text(font_gameover_55, al_map_rgb(0, 0, 0), SCREEN_W/2 + 120, SCREEN_H/2 + 10, ALLEGRO_ALIGN_CENTER, "Plane's Upgrade");
        al_draw_bitmap(speedicon2_img, SCREEN_W/2 - 150, SCREEN_H/2 + 50, 0);
        al_draw_textf(font_gameover_45, al_map_rgb(0, 0, 0), SCREEN_W/2 - 80, SCREEN_H/2 + 70, ALLEGRO_ALIGN_CENTER, "Speed + %d", increasing_speed_enemy);
        al_draw_bitmap(ebullet_icon_img, SCREEN_W/2 - 170, SCREEN_H/2 + 130, 0);
        al_draw_textf(font_gameover_45, al_map_rgb(0, 0, 0), SCREEN_W/2 - 80, SCREEN_H/2 + 160, ALLEGRO_ALIGN_CENTER, "speed + %d", increasing_speed_bullet);
        al_draw_text(font_gameover_45, al_map_rgb(0, 0, 0), SCREEN_W/2 - 80, SCREEN_H/2 + 140, ALLEGRO_ALIGN_CENTER, "Present");

        /* my plane's upgrade option */
        if(pnt_in_rect(mouse_x, mouse_y, SCREEN_W/2 + 20, SCREEN_H/2 + 50, 200, 30))
            al_draw_filled_rectangle(SCREEN_W/2 + 20, SCREEN_H/2 + 50, SCREEN_W/2 + 220, SCREEN_H/2 + 80, al_map_rgb(100, 100, 100));
        else
            al_draw_filled_rectangle(SCREEN_W/2 + 20, SCREEN_H/2 + 50, SCREEN_W/2 + 220, SCREEN_H/2 + 80, al_map_rgb(150, 150, 150));

        al_draw_bitmap(hearticon_img, SCREEN_W/2 + 20, SCREEN_H/2 + 45, 0);
        al_draw_text(font_gameover_45, al_map_rgb(0, 0, 0), SCREEN_W/2 + 125, SCREEN_H/2 + 50, ALLEGRO_ALIGN_CENTER, "Health++");
        al_draw_text(font_gameover_45, al_map_rgb(0, 0, 0), SCREEN_W/2 + 200, SCREEN_H/2 + 50, ALLEGRO_ALIGN_CENTER, "$50");

        if(pnt_in_rect(mouse_x, mouse_y, SCREEN_W/2 + 20, SCREEN_H/2 + 100, 200, 30))
            al_draw_filled_rectangle(SCREEN_W/2 + 20, SCREEN_H/2 + 100, SCREEN_W/2 + 220, SCREEN_H/2 + 130, al_map_rgb(100, 100, 100));
        else
            al_draw_filled_rectangle(SCREEN_W/2 + 20, SCREEN_H/2 + 100, SCREEN_W/2 + 220, SCREEN_H/2 + 130, al_map_rgb(150, 150, 150));

        al_draw_bitmap(bullet_icon, SCREEN_W/2 + 20, SCREEN_H/2 + 105, 0);
        al_draw_text(font_gameover_45, al_map_rgb(0, 0, 0), SCREEN_W/2 + 125, SCREEN_H/2 + 100, ALLEGRO_ALIGN_CENTER, "Speed++");
        al_draw_text(font_gameover_45, al_map_rgb(0, 0, 0), SCREEN_W/2 + 200, SCREEN_H/2 + 100, ALLEGRO_ALIGN_CENTER, "$100");

        if(pnt_in_rect(mouse_x, mouse_y, SCREEN_W/2 + 20, SCREEN_H/2 + 150, 200, 30))
            al_draw_filled_rectangle(SCREEN_W/2 + 20, SCREEN_H/2 + 150, SCREEN_W/2 + 220, SCREEN_H/2 + 180, al_map_rgb(100, 100, 100));
        else
            al_draw_filled_rectangle(SCREEN_W/2 + 20, SCREEN_H/2 + 150, SCREEN_W/2 + 220, SCREEN_H/2 + 180, al_map_rgb(150, 150, 150));

        al_draw_bitmap(randomicon_img, SCREEN_W/2 + 20, SCREEN_H/2 + 150, 0);
        al_draw_text(font_gameover_45, al_map_rgb(0, 0, 0), SCREEN_W/2 + 125, SCREEN_H/2 + 150, ALLEGRO_ALIGN_CENTER, "Mystery box");
        al_draw_text(font_gameover_45, al_map_rgb(0, 0, 0), SCREEN_W/2 + 200, SCREEN_H/2 + 150, ALLEGRO_ALIGN_CENTER, "$150");

        /* money tracker */
        al_draw_textf(font_pirulen_12, al_map_rgb(0, 0, 0), SCREEN_W/2 + 120, SCREEN_H/2 + 185, ALLEGRO_ALIGN_CENTER, "Remaining Money: $%d", plane.money);

        /* settings */
        if (pnt_in_rect(mouse_x, mouse_y, SCREEN_W - 48, 10, 38, 38))
            al_draw_bitmap(img_settings2, SCREEN_W - 48, 10, 0);
        else
            al_draw_bitmap(img_settings, SCREEN_W - 48, 10, 0);
    }

    else if (active_scene == SCENE_TRANSITION_23)
    {
        al_draw_bitmap(stage3_img_backgroud, 0, 0, 0);
        al_draw_text(font_gameover_100, al_map_rgb(255, 255, 255), SCREEN_W/2, SCREEN_H/2 - 120, ALLEGRO_ALIGN_CENTER, "STAGE 2 COMPLETED");
        al_draw_text(font_gameover_100, al_map_rgb(255, 255, 255), SCREEN_W/2, SCREEN_H/2 - 60, ALLEGRO_ALIGN_CENTER, "2 out of 4 stages");
        al_draw_line(SCREEN_W/2, SCREEN_H/2 + 10, SCREEN_W/2, SCREEN_H/2 + 180, al_map_rgb(255, 255, 255), 5);
        al_draw_text(font_gameover_55, al_map_rgb(255, 255, 255), SCREEN_W/2 - 120, SCREEN_H/2 + 10, ALLEGRO_ALIGN_CENTER, "Enemy's Upgrade");
        al_draw_text(font_gameover_55, al_map_rgb(255, 255, 255), SCREEN_W/2 + 120, SCREEN_H/2 + 10, ALLEGRO_ALIGN_CENTER, "Plane's Upgrade");
        al_draw_bitmap(speedicon2_img, SCREEN_W/2 - 150, SCREEN_H/2 + 50, 0);
        al_draw_textf(font_gameover_45, al_map_rgb(255, 255, 255), SCREEN_W/2 - 80, SCREEN_H/2 + 70, ALLEGRO_ALIGN_CENTER, "Speed + %d", increasing_speed_enemy);
        al_draw_bitmap(ebullet_icon_img, SCREEN_W/2 - 170, SCREEN_H/2 + 130, 0);
        al_draw_textf(font_gameover_45, al_map_rgb(255, 255, 255), SCREEN_W/2 - 80, SCREEN_H/2 + 140, ALLEGRO_ALIGN_CENTER, "speed + %d", increasing_speed_bullet);
        al_draw_text(font_gameover_45, al_map_rgb(255, 255, 255), SCREEN_W/2 - 70, SCREEN_H/2 +160, ALLEGRO_ALIGN_CENTER, " Camo Bullets");

        /* my plane's upgrade option */
        if(pnt_in_rect(mouse_x, mouse_y, SCREEN_W/2 + 20, SCREEN_H/2 + 50, 200, 30))
            al_draw_filled_rectangle(SCREEN_W/2 + 20, SCREEN_H/2 + 50, SCREEN_W/2 + 220, SCREEN_H/2 + 80, al_map_rgb(100, 100, 100));
        else
            al_draw_filled_rectangle(SCREEN_W/2 + 20, SCREEN_H/2 + 50, SCREEN_W/2 + 220, SCREEN_H/2 + 80, al_map_rgb(150, 150, 150));

        al_draw_bitmap(hearticon_img, SCREEN_W/2 + 20, SCREEN_H/2 + 45, 0);
        al_draw_text(font_gameover_45, al_map_rgb(255, 255, 255), SCREEN_W/2 + 125, SCREEN_H/2 + 50, ALLEGRO_ALIGN_CENTER, "Health++");
        al_draw_text(font_gameover_45, al_map_rgb(255, 255, 255), SCREEN_W/2 + 200, SCREEN_H/2 + 50, ALLEGRO_ALIGN_CENTER, "$50");

        if(pnt_in_rect(mouse_x, mouse_y, SCREEN_W/2 + 20, SCREEN_H/2 + 100, 200, 30))
            al_draw_filled_rectangle(SCREEN_W/2 + 20, SCREEN_H/2 + 100, SCREEN_W/2 + 220, SCREEN_H/2 + 130, al_map_rgb(100, 100, 100));
        else
            al_draw_filled_rectangle(SCREEN_W/2 + 20, SCREEN_H/2 + 100, SCREEN_W/2 + 220, SCREEN_H/2 + 130, al_map_rgb(150, 150, 150));

        al_draw_bitmap(bullet_icon, SCREEN_W/2 + 20, SCREEN_H/2 + 105, 0);
        al_draw_text(font_gameover_45, al_map_rgb(255, 255, 255), SCREEN_W/2 + 125, SCREEN_H/2 + 100, ALLEGRO_ALIGN_CENTER, "Speed++");
        al_draw_text(font_gameover_45, al_map_rgb(255, 255, 255), SCREEN_W/2 + 200, SCREEN_H/2 + 100, ALLEGRO_ALIGN_CENTER, "$100");

        if(pnt_in_rect(mouse_x, mouse_y, SCREEN_W/2 + 20, SCREEN_H/2 + 150, 200, 30))
            al_draw_filled_rectangle(SCREEN_W/2 + 20, SCREEN_H/2 + 150, SCREEN_W/2 + 220, SCREEN_H/2 + 180, al_map_rgb(100, 100, 100));
        else
            al_draw_filled_rectangle(SCREEN_W/2 + 20, SCREEN_H/2 + 150, SCREEN_W/2 + 220, SCREEN_H/2 + 180, al_map_rgb(150, 150, 150));

        al_draw_bitmap(randomicon_img, SCREEN_W/2 + 20, SCREEN_H/2 + 150, 0);
        al_draw_text(font_gameover_45, al_map_rgb(255, 255, 255), SCREEN_W/2 + 125, SCREEN_H/2 + 150, ALLEGRO_ALIGN_CENTER, "Mystery box");
        al_draw_text(font_gameover_45, al_map_rgb(255, 255, 255), SCREEN_W/2 + 200, SCREEN_H/2 + 150, ALLEGRO_ALIGN_CENTER, "$150");

        /* money tracker */
        al_draw_textf(font_pirulen_12, al_map_rgb(255, 255, 255), SCREEN_W/2 + 120, SCREEN_H/2 + 185, ALLEGRO_ALIGN_CENTER, "Remaining Money: $%d", plane.money);

        /* settings */
        if (pnt_in_rect(mouse_x, mouse_y, SCREEN_W - 48, 10, 38, 38))
            al_draw_bitmap(img_settings2, SCREEN_W - 48, 10, 0);
        else
            al_draw_bitmap(img_settings, SCREEN_W - 48, 10, 0);
    }

    else if (active_scene == SCENE_TRANSITION_34)
    {
        al_draw_bitmap(boss_stage_img_backgroud, 0, 0, 0);
        al_draw_text(font_gameover_100, al_map_rgb(0, 0, 0), SCREEN_W/2, SCREEN_H/2 - 120, ALLEGRO_ALIGN_CENTER, "STAGE 3 COMPLETED");
        al_draw_text(font_gameover_100, al_map_rgb(0, 0, 0), SCREEN_W/2, SCREEN_H/2 - 60, ALLEGRO_ALIGN_CENTER, "3 out of 4 stages");
        al_draw_line(SCREEN_W/2, SCREEN_H/2 + 10, SCREEN_W/2, SCREEN_H/2 + 180, al_map_rgb(0, 0, 0), 5);
        al_draw_text(font_gameover_55, al_map_rgb(0, 0, 0), SCREEN_W/2 - 120, SCREEN_H/2 + 10, ALLEGRO_ALIGN_CENTER, "Enemy's Upgrade");
        al_draw_text(font_gameover_55, al_map_rgb(0, 0, 0), SCREEN_W/2 + 120, SCREEN_H/2 + 10, ALLEGRO_ALIGN_CENTER, "Plane's Upgrade");

        /* to change */
        al_draw_bitmap(boss_icon_img, SCREEN_W/2 - 240, SCREEN_H/2 + 50, 0);
        al_draw_text(font_gameover_45, al_map_rgb(0, 0, 0), SCREEN_W/2 - 80, SCREEN_H/2 + 70, ALLEGRO_ALIGN_CENTER, "BOSS + LASER");
        al_draw_bitmap(boss_minion_icon_img, SCREEN_W/2 - 240, SCREEN_H/2 + 130, 0);
        al_draw_text(font_gameover_45, al_map_rgb(0, 0, 0), SCREEN_W/2 - 80, SCREEN_H/2 + 140, ALLEGRO_ALIGN_CENTER, "Minions +4");
        /* to change */

        al_draw_bitmap(warning_img, SCREEN_W/2 -125, SCREEN_H/2 - 275, 0);
        al_draw_text(font_gameover_100, al_map_rgb(255, 0, 0), SCREEN_W/2, SCREEN_H/2 - 240, ALLEGRO_ALIGN_CENTER, "BOSS FIGHT INCOMING");

        /* my plane's upgrade option */
        if(pnt_in_rect(mouse_x, mouse_y, SCREEN_W/2 + 20, SCREEN_H/2 + 50, 200, 30))
            al_draw_filled_rectangle(SCREEN_W/2 + 20, SCREEN_H/2 + 50, SCREEN_W/2 + 220, SCREEN_H/2 + 80, al_map_rgb(100, 100, 100));
        else
            al_draw_filled_rectangle(SCREEN_W/2 + 20, SCREEN_H/2 + 50, SCREEN_W/2 + 220, SCREEN_H/2 + 80, al_map_rgb(150, 150, 150));

        al_draw_bitmap(hearticon_img, SCREEN_W/2 + 20, SCREEN_H/2 + 45, 0);
        al_draw_text(font_gameover_45, al_map_rgb(0, 0, 0), SCREEN_W/2 + 125, SCREEN_H/2 + 50, ALLEGRO_ALIGN_CENTER, "Health++");
        al_draw_text(font_gameover_45, al_map_rgb(0, 0, 0), SCREEN_W/2 + 200, SCREEN_H/2 + 50, ALLEGRO_ALIGN_CENTER, "$50");

        if(pnt_in_rect(mouse_x, mouse_y, SCREEN_W/2 + 20, SCREEN_H/2 + 100, 200, 30))
            al_draw_filled_rectangle(SCREEN_W/2 + 20, SCREEN_H/2 + 100, SCREEN_W/2 + 220, SCREEN_H/2 + 130, al_map_rgb(100, 100, 100));
        else
            al_draw_filled_rectangle(SCREEN_W/2 + 20, SCREEN_H/2 + 100, SCREEN_W/2 + 220, SCREEN_H/2 + 130, al_map_rgb(150, 150, 150));

        al_draw_bitmap(bullet_icon, SCREEN_W/2 + 20, SCREEN_H/2 + 105, 0);
        al_draw_text(font_gameover_45, al_map_rgb(0, 0, 0), SCREEN_W/2 + 125, SCREEN_H/2 + 100, ALLEGRO_ALIGN_CENTER, "Speed++");
        al_draw_text(font_gameover_45, al_map_rgb(0, 0, 0), SCREEN_W/2 + 200, SCREEN_H/2 + 100, ALLEGRO_ALIGN_CENTER, "$100");

        if(pnt_in_rect(mouse_x, mouse_y, SCREEN_W/2 + 20, SCREEN_H/2 + 150, 200, 30))
            al_draw_filled_rectangle(SCREEN_W/2 + 20, SCREEN_H/2 + 150, SCREEN_W/2 + 220, SCREEN_H/2 + 180, al_map_rgb(100, 100, 100));
        else
            al_draw_filled_rectangle(SCREEN_W/2 + 20, SCREEN_H/2 + 150, SCREEN_W/2 + 220, SCREEN_H/2 + 180, al_map_rgb(150, 150, 150));

        al_draw_bitmap(randomicon_img, SCREEN_W/2 + 20, SCREEN_H/2 + 150, 0);
        al_draw_text(font_gameover_45, al_map_rgb(0, 0, 0), SCREEN_W/2 + 125, SCREEN_H/2 + 150, ALLEGRO_ALIGN_CENTER, "Mystery box");
        al_draw_text(font_gameover_45, al_map_rgb(0, 0, 0), SCREEN_W/2 + 200, SCREEN_H/2 + 150, ALLEGRO_ALIGN_CENTER, "$150");

        /* money tracker */
        al_draw_textf(font_pirulen_12, al_map_rgb(0, 0, 0), SCREEN_W/2 + 120, SCREEN_H/2 + 185, ALLEGRO_ALIGN_CENTER, "Remaining Money: $%d", plane.money);

        /* settings */
        if (pnt_in_rect(mouse_x, mouse_y, SCREEN_W - 48, 10, 38, 38))
            al_draw_bitmap(img_settings2, SCREEN_W - 48, 10, 0);
        else
            al_draw_bitmap(img_settings, SCREEN_W - 48, 10, 0);
    }

    else if (active_scene == SCENE_SETTINGS)
    {
        al_draw_bitmap(settings_img_background, 0, 0, 0);

        /* Back button */
        if(pnt_in_rect(mouse_x, mouse_y, 20, 20, al_get_bitmap_width(back_icon_img), al_get_bitmap_height(back_icon_img)))
            al_draw_bitmap(back_icon_img, 20, 20, 0);
        else
            al_draw_bitmap(arrow_icon_img, 20, 20, 0);

        al_draw_text(font_gameover_100, al_map_rgb(0, 0, 0), SCREEN_W/2, 0, ALLEGRO_ALIGN_CENTER, "Choose Your Fighter");

        /* plane option */
        al_draw_bitmap(plane_icon_img, 160, SCREEN_H - 110, 0);
        al_draw_bitmap(plane2_icon_img, 240, SCREEN_H - 230, 0);
        al_draw_bitmap(plane3_icon_img, 340, SCREEN_H - 130, 0);
        al_draw_bitmap(plane4_icon_img, 460, SCREEN_H - 230, 0);
        al_draw_bitmap(plane5_icon_img, 540, SCREEN_H - 130, 0);

        /* plane selected */
        if(plane1 == 1)
            al_draw_text(font_pirulen_12, al_map_rgb(255, 128, 0), 200, SCREEN_H-80, ALLEGRO_ALIGN_CENTER, "Selected");

        if(plane2 == 1)
            al_draw_text(font_pirulen_12, al_map_rgb(255, 128, 0), 290, SCREEN_H - 140, ALLEGRO_ALIGN_CENTER, "Selected");

        if(plane3 == 1)
            al_draw_text(font_pirulen_12, al_map_rgb(255, 128, 0), 400, SCREEN_H - 70, ALLEGRO_ALIGN_CENTER, "Selected");

        if(plane4 == 1)
            al_draw_text(font_pirulen_12, al_map_rgb(255, 128, 0), 505, SCREEN_H - 155, ALLEGRO_ALIGN_CENTER, "Selected");

        if(plane5 == 1)
            al_draw_text(font_pirulen_12, al_map_rgb(255, 128, 0), 595, SCREEN_H - 50, ALLEGRO_ALIGN_CENTER, "Selected");

        /* plane open and locked */
        if(highscore <= 6000)
            al_draw_bitmap(lock_icon_img, 277, SCREEN_H - 140, 0);
        if(highscore <= 9000)
            al_draw_bitmap(lock_icon_img, 390, SCREEN_H - 75, 0);
        if(highscore <= 12000)
            al_draw_bitmap(lock_icon_img, 490, SCREEN_H - 155, 0);
        if(highscore <= 15000)
            al_draw_bitmap(lock_icon_img, 582, SCREEN_H - 75, 0);

        /* plane description */
        if(pnt_in_rect(mouse_x, mouse_y, 160, SCREEN_H-110, al_get_bitmap_width(plane_icon_img), al_get_bitmap_height(plane_icon_img)))
        {
            al_draw_bitmap(character_choose_img, 0, 30, 0);
            al_draw_text(font_darkfuture_50, al_map_rgb(255, 77, 0), 80, 85, ALLEGRO_ALIGN_LEFT, "The Original");
            al_draw_line(80, 145, 470, 145, al_map_rgb(255, 255, 0), 5);
            al_draw_filled_rectangle(80, 155, 470, 305, al_map_rgb(255, 255, 255));
            al_draw_text(font_gameover_55, al_map_rgb(255, 77, 0), 80, 155, ALLEGRO_ALIGN_LEFT, "ATTRIBUTES");
            al_draw_text(font_gameover_45, al_map_rgb(255, 77, 0), 80, 185, ALLEGRO_ALIGN_LEFT, "Hp : ");
            al_draw_text(font_gameover_45, al_map_rgb(255, 77, 0), 250, 185, ALLEGRO_ALIGN_CENTER, "NORMAL");
            al_draw_text(font_gameover_45, al_map_rgb(255, 77, 0), 80, 215, ALLEGRO_ALIGN_LEFT, "Speed : ");
            al_draw_text(font_gameover_45, al_map_rgb(255, 77, 0), 250, 215, ALLEGRO_ALIGN_CENTER, "NORMAL");
            al_draw_text(font_gameover_45, al_map_rgb(255, 77, 0), 80, 245, ALLEGRO_ALIGN_LEFT, "Bullet per shoot : ");
            al_draw_text(font_gameover_45, al_map_rgb(255, 77, 0), 250, 245, ALLEGRO_ALIGN_CENTER, "NORMAL");
            al_draw_text(font_gameover_45, al_map_rgb(255, 77, 0), 80, 275, ALLEGRO_ALIGN_LEFT, "Bullet Speed : ");
            al_draw_text(font_gameover_45, al_map_rgb(255, 77, 0), 250, 275, ALLEGRO_ALIGN_CENTER, "NORMAL");
            al_draw_line(80, 315, 470, 315, al_map_rgb(255, 255, 0), 5);
            al_draw_text(font_gameover_55, al_map_rgb(255, 77, 0), 80, 330, ALLEGRO_ALIGN_LEFT, "\"Nothings better than the ORI!\"");
        }

        if(pnt_in_rect(mouse_x, mouse_y, 240, SCREEN_H - 230, al_get_bitmap_width(plane2_icon_img), al_get_bitmap_height(plane2_icon_img)))
        {
            al_draw_bitmap(character_choose_img, 50, 30, 0);
            al_draw_text(font_darkfuture_50, al_map_rgb(0, 0, 0), 130, 85, ALLEGRO_ALIGN_LEFT, "The Silhouette");
            al_draw_line(130, 145, 520, 145, al_map_rgb(0, 0, 0), 5);
            al_draw_filled_rectangle(130, 155, 520, 305, al_map_rgb(255, 255, 255));
            al_draw_text(font_gameover_55, al_map_rgb(0, 0, 0), 130, 155, ALLEGRO_ALIGN_LEFT, "ATTRIBUTES");
            al_draw_text(font_gameover_45, al_map_rgb(0, 0, 0), 130, 185, ALLEGRO_ALIGN_LEFT, "Hp : ");
            al_draw_text(font_gameover_45, al_map_rgb(0, 0, 0), 300, 185, ALLEGRO_ALIGN_CENTER, "LOW");
            al_draw_text(font_gameover_45, al_map_rgb(0, 0, 0), 130, 215, ALLEGRO_ALIGN_LEFT, "Speed : ");
            al_draw_text(font_gameover_45, al_map_rgb(0, 0, 0), 300, 215, ALLEGRO_ALIGN_CENTER, "DOUBLED");
            al_draw_text(font_gameover_45, al_map_rgb(0, 0, 0), 130, 245, ALLEGRO_ALIGN_LEFT, "Bullet per shoot : ");
            al_draw_text(font_gameover_45, al_map_rgb(0, 0, 0), 300, 245, ALLEGRO_ALIGN_CENTER, "NORMAL");
            al_draw_text(font_gameover_45, al_map_rgb(0, 0, 0), 130, 275, ALLEGRO_ALIGN_LEFT, "Bullet Speed : ");
            al_draw_text(font_gameover_45, al_map_rgb(0, 0, 0), 300, 275, ALLEGRO_ALIGN_CENTER, "S. INCREASE");
            al_draw_line(130, 315, 520, 315, al_map_rgb(0, 0, 0), 5);
            al_draw_text(font_gameover_55, al_map_rgb(0, 0, 0), 130, 330, ALLEGRO_ALIGN_LEFT, "\"Dark reaches further than the light.\"");

            /* LOCKED CONDITON */
            if(highscore <= 6000)
            {
                al_draw_text(font_gameover_55, al_map_rgb(255, 0, 0), 450, 200, ALLEGRO_ALIGN_CENTER, "LOCKED");
                al_draw_text(font_gameover_55, al_map_rgb(255, 0, 0), 450, 225, ALLEGRO_ALIGN_CENTER, "HS < 6000");
            }

        }

        if(pnt_in_rect(mouse_x, mouse_y, 340, SCREEN_H - 130, al_get_bitmap_width(plane3_icon_img), al_get_bitmap_height(plane3_icon_img)))
        {
            al_draw_bitmap(character_choose_img, 100, 30, 0);
            al_draw_text(font_darkfuture_50, al_map_rgb(0, 200, 0), 180, 85, ALLEGRO_ALIGN_LEFT, "Unmovable Hulk");
            al_draw_line(180, 145, 570, 145, al_map_rgb(0, 255, 0), 5);
            al_draw_filled_rectangle(180, 155, 570, 305, al_map_rgb(255, 255, 255));
            al_draw_text(font_gameover_55, al_map_rgb(0, 200, 0), 180, 155, ALLEGRO_ALIGN_LEFT, "ATTRIBUTES");
            al_draw_text(font_gameover_45, al_map_rgb(0, 200, 0), 180, 185, ALLEGRO_ALIGN_LEFT, "Hp : ");
            al_draw_text(font_gameover_45, al_map_rgb(0, 200, 0), 350, 185, ALLEGRO_ALIGN_CENTER, "DOUBLED");
            al_draw_text(font_gameover_45, al_map_rgb(0, 200, 0), 180, 215, ALLEGRO_ALIGN_LEFT, "Speed : ");
            al_draw_text(font_gameover_45, al_map_rgb(0, 200, 0), 350, 215, ALLEGRO_ALIGN_CENTER, "VERY LOW");
            al_draw_text(font_gameover_45, al_map_rgb(0, 200, 0), 180, 245, ALLEGRO_ALIGN_LEFT, "Bullet per shoot : ");
            al_draw_text(font_gameover_45, al_map_rgb(0, 200, 0), 350, 245, ALLEGRO_ALIGN_CENTER, "TRIPLE");
            al_draw_text(font_gameover_45, al_map_rgb(0, 200, 0), 180, 275, ALLEGRO_ALIGN_LEFT, "Bullet Speed : ");
            al_draw_text(font_gameover_45, al_map_rgb(0, 200, 0), 350, 275, ALLEGRO_ALIGN_CENTER, "NORMAL");
            al_draw_line(180, 315, 570, 315, al_map_rgb(0, 255, 0), 5);
            al_draw_text(font_gameover_55, al_map_rgb(0, 200, 0), 180, 330, ALLEGRO_ALIGN_LEFT, "\"The bigger the better!\"");

            /* LOCKED CONDITON */
            if(highscore <= 9000)
            {
                al_draw_text(font_gameover_55, al_map_rgb(255, 0, 0), 500, 200, ALLEGRO_ALIGN_CENTER, "LOCKED");
                al_draw_text(font_gameover_55, al_map_rgb(255, 0, 0), 500, 225, ALLEGRO_ALIGN_CENTER, "HS < 9000");
            }
        }

        if(pnt_in_rect(mouse_x, mouse_y, 450, SCREEN_H - 230, al_get_bitmap_width(plane4_icon_img), al_get_bitmap_height(plane4_icon_img)))
        {
            al_draw_bitmap(character_choose_img, 150, 30, 0);
            al_draw_text(font_darkfuture_50, al_map_rgb(0, 120, 255), 230, 85, ALLEGRO_ALIGN_LEFT, "Aerial X");
            al_draw_line(230, 145, 620, 145, al_map_rgb(0, 255, 255), 5);
            al_draw_filled_rectangle(230, 155, 620, 305, al_map_rgb(255, 255, 255));
            al_draw_text(font_gameover_55, al_map_rgb(0, 120, 255), 230, 155, ALLEGRO_ALIGN_LEFT, "ATTRIBUTES");
            al_draw_text(font_gameover_45, al_map_rgb(0, 120, 255), 230, 185, ALLEGRO_ALIGN_LEFT, "Hp : ");
            al_draw_text(font_gameover_45, al_map_rgb(0, 120, 255), 400, 185, ALLEGRO_ALIGN_CENTER, "VERY LOW");
            al_draw_text(font_gameover_45, al_map_rgb(0, 120, 255), 230, 215, ALLEGRO_ALIGN_LEFT, "Speed : ");
            al_draw_text(font_gameover_45, al_map_rgb(0, 120, 255), 400, 215, ALLEGRO_ALIGN_CENTER, "VERY HIGH");
            al_draw_text(font_gameover_45, al_map_rgb(0, 120, 255), 230, 245, ALLEGRO_ALIGN_LEFT, "Bullet per shoot : ");
            al_draw_text(font_gameover_45, al_map_rgb(0, 120, 255), 400, 245, ALLEGRO_ALIGN_CENTER, "NORMAL");
            al_draw_text(font_gameover_45, al_map_rgb(0, 120, 255), 230, 275, ALLEGRO_ALIGN_LEFT, "Bullet Speed : ");
            al_draw_text(font_gameover_45, al_map_rgb(0, 120, 255), 400, 275, ALLEGRO_ALIGN_CENTER, "HIGH");
            al_draw_line(230, 315, 620, 315, al_map_rgb(0, 255, 255), 5);
            al_draw_text(font_gameover_55, al_map_rgb(0, 120, 255), 230, 330, ALLEGRO_ALIGN_LEFT, "\"Nothings better than the ORI!\"");

            /* LOCKED CONDITON */
            if(highscore <= 12000)
            {
                al_draw_text(font_gameover_55, al_map_rgb(255, 0, 0), 550, 200, ALLEGRO_ALIGN_CENTER, "LOCKED");
                al_draw_text(font_gameover_55, al_map_rgb(255, 0, 0), 550, 225, ALLEGRO_ALIGN_CENTER, "HS < 12000");
            }
        }

        if(pnt_in_rect(mouse_x, mouse_y, 540, SCREEN_H - 130, al_get_bitmap_width(plane5_icon_img), al_get_bitmap_height(plane5_icon_img)))
        {
            al_draw_bitmap(character_choose_img, 200, 30, 0);
            al_draw_text(font_darkfuture_50, al_map_rgb(255, 0, 255), 280, 85, ALLEGRO_ALIGN_LEFT, "Pink Money");
            al_draw_line(280, 145, 670, 145, al_map_rgb(120, 0, 255), 5);
            al_draw_filled_rectangle(280, 155, 670, 305, al_map_rgb(255, 255, 255));
            al_draw_text(font_gameover_55, al_map_rgb(255, 0, 255), 280, 155, ALLEGRO_ALIGN_LEFT, "ATTRIBUTES");
            al_draw_text(font_gameover_45, al_map_rgb(255, 0, 255), 280, 185, ALLEGRO_ALIGN_LEFT, "Hp : ");
            al_draw_text(font_gameover_45, al_map_rgb(255, 0, 255), 450, 185, ALLEGRO_ALIGN_CENTER, "NORMAL");
            al_draw_text(font_gameover_45, al_map_rgb(255, 0, 255), 280, 215, ALLEGRO_ALIGN_LEFT, "Speed : ");
            al_draw_text(font_gameover_45, al_map_rgb(255, 0, 255), 450, 215, ALLEGRO_ALIGN_CENTER, "S. INCREASE");
            al_draw_text(font_gameover_45, al_map_rgb(255, 0, 255), 280, 245, ALLEGRO_ALIGN_LEFT, "Bullet per shoot : ");
            al_draw_text(font_gameover_45, al_map_rgb(255, 0, 255), 450, 245, ALLEGRO_ALIGN_CENTER, "NORMAL");
            al_draw_text(font_gameover_45, al_map_rgb(255, 0, 255), 280, 275, ALLEGRO_ALIGN_LEFT, "Bullet Speed : ");
            al_draw_text(font_gameover_45, al_map_rgb(255, 0, 255), 450, 275, ALLEGRO_ALIGN_CENTER, "S. INCREASE");
            al_draw_line(280, 315, 670, 315, al_map_rgb(120, 0, 255), 5);
            al_draw_text(font_gameover_55, al_map_rgb(255, 0, 255), 280, 330, ALLEGRO_ALIGN_LEFT, "\"Pink brings out the bling\"");

            al_draw_text(font_gameover_45, al_map_rgb(255, 0, 0), 650, 155, ALLEGRO_ALIGN_RIGHT, "x2 MONEY");
            al_draw_text(font_gameover_45, al_map_rgb(255, 0, 0), 650, 185, ALLEGRO_ALIGN_RIGHT, "x2 SCORE");

            /* LOCKED CONDITON */
            if(highscore <= 15000)
            {
                al_draw_text(font_gameover_55, al_map_rgb(255, 0, 0), 600, 230, ALLEGRO_ALIGN_CENTER, "LOCKED");
                al_draw_text(font_gameover_55, al_map_rgb(255, 0, 0), 600, 255, ALLEGRO_ALIGN_CENTER, "HS < 15000");
            }
        }
    }

    else if (active_scene == SCENE_LOSS)
    {
        al_draw_bitmap(start_img_background, 0, 0, 0);
        al_draw_text(font_gameover_100, al_map_rgb(0, 0, 0), SCREEN_W/2, SCREEN_H/2 - 180, ALLEGRO_ALIGN_CENTER, "YOU LOSE");
        al_draw_line(SCREEN_W/2 - 150, SCREEN_H/2 - 120, SCREEN_W/2 + 150, SCREEN_H/2 - 120, al_map_rgb(0, 0, 0), 5);
        al_draw_text(font_gameover_55, al_map_rgb(0, 0, 0), SCREEN_W/2 - 150, SCREEN_H/2 - 110, ALLEGRO_ALIGN_LEFT, "Total Score : ");
        al_draw_text(font_gameover_55, al_map_rgb(0, 0, 0), SCREEN_W/2 - 150, SCREEN_H/2 - 75, ALLEGRO_ALIGN_LEFT, "Excess Money : ");
        al_draw_textf(font_gameover_55, al_map_rgb(0, 0, 0), SCREEN_W/2 + 150, SCREEN_H/2 - 110, ALLEGRO_ALIGN_RIGHT, "%d", plane.score);
        al_draw_textf(font_gameover_55, al_map_rgb(0, 0, 0), SCREEN_W/2 + 150, SCREEN_H/2 - 75, ALLEGRO_ALIGN_RIGHT, "%d", plane.money);
        al_draw_line(SCREEN_W/2 + 50, SCREEN_H/2 - 25, SCREEN_W/2 + 150, SCREEN_H/2 - 25, al_map_rgb(215, 215, 215), 5);
        al_draw_text(font_gameover_100, al_map_rgb(0, 0, 0), SCREEN_W/2 + 165, SCREEN_H/2 - 63, ALLEGRO_ALIGN_CENTRE, "+");
        al_draw_text(font_gameover_55, al_map_rgb(0, 0, 0), SCREEN_W/2 - 150, SCREEN_H/2 - 10, ALLEGRO_ALIGN_LEFT, "Final Score : ");
        al_draw_textf(font_gameover_55, al_map_rgb(0, 0, 0), SCREEN_W/2 + 150, SCREEN_H/2 - 10, ALLEGRO_ALIGN_RIGHT, "%d", plane.score+plane.money);
        al_draw_line(SCREEN_W/2 - 150, SCREEN_H/2 + 35, SCREEN_W/2 + 150, SCREEN_H/2 + 35, al_map_rgb(0, 0, 0), 5);

        if(plane.score + plane.money > highscore)
        {
            al_draw_text(font_gameover_55, al_map_rgb(0, 0, 0), SCREEN_W/2, SCREEN_H/2 + 40, ALLEGRO_ALIGN_CENTER, "NEW HIGHSCORE !!!");
        }

        al_draw_text(font_gameover_55, al_map_rgb(0, 0, 0), SCREEN_W/2, SCREEN_H - 100, ALLEGRO_ALIGN_CENTER, "Press ENTER key to restart");
        al_draw_text(font_gameover_55, al_map_rgb(0, 0, 0), SCREEN_W/2, SCREEN_H - 50, ALLEGRO_ALIGN_CENTER, "Press ESC key to go back to main menu");
    }

    else if (active_scene == SCENE_WIN)
    {
        al_draw_bitmap(boss_stage_img_backgroud, 0, 0, 0);
        al_draw_text(font_gameover_100, al_map_rgb(255, 255, 255), SCREEN_W/2, SCREEN_H/2 - 180, ALLEGRO_ALIGN_CENTER, "YOU WIN");
        al_draw_line(SCREEN_W/2 - 150, SCREEN_H/2 - 120, SCREEN_W/2 + 150, SCREEN_H/2 - 120, al_map_rgb(255, 255, 255), 5);
        al_draw_text(font_gameover_55, al_map_rgb(255, 255, 255), SCREEN_W/2 - 150, SCREEN_H/2 - 110, ALLEGRO_ALIGN_LEFT, "Total Score : ");
        al_draw_text(font_gameover_55, al_map_rgb(255, 255, 255), SCREEN_W/2 - 150, SCREEN_H/2 - 75, ALLEGRO_ALIGN_LEFT, "Excess Money : ");
        al_draw_textf(font_gameover_55, al_map_rgb(255, 255, 255), SCREEN_W/2 + 150, SCREEN_H/2 - 110, ALLEGRO_ALIGN_RIGHT, "%d", plane.score);
        al_draw_textf(font_gameover_55, al_map_rgb(255, 255, 255), SCREEN_W/2 + 150, SCREEN_H/2 - 75, ALLEGRO_ALIGN_RIGHT, "%d", plane.money);
        al_draw_line(SCREEN_W/2 + 50, SCREEN_H/2 - 25, SCREEN_W/2 + 150, SCREEN_H/2 - 25, al_map_rgb(215, 215, 215), 5);
        al_draw_text(font_gameover_100, al_map_rgb(215, 215, 215), SCREEN_W/2 + 165, SCREEN_H/2 - 63, ALLEGRO_ALIGN_CENTRE, "+");
        al_draw_text(font_gameover_55, al_map_rgb(255, 255, 255), SCREEN_W/2 - 150, SCREEN_H/2 - 10, ALLEGRO_ALIGN_LEFT, "Final Score : ");
        al_draw_textf(font_gameover_55, al_map_rgb(255, 255, 255), SCREEN_W/2 + 150, SCREEN_H/2 - 10, ALLEGRO_ALIGN_RIGHT, "%d", plane.score+plane.money);
        al_draw_line(SCREEN_W/2 - 150, SCREEN_H/2 + 35, SCREEN_W/2 + 150, SCREEN_H/2 + 35, al_map_rgb(255, 255, 255), 5);

        if(plane.score + plane.money > highscore)
        {
            al_draw_text(font_gameover_55, al_map_rgb(255, 255, 255), SCREEN_W/2, SCREEN_H/2 + 40, ALLEGRO_ALIGN_CENTER, "NEW HIGHSCORE !!!");
        }

        al_draw_text(font_gameover_55, al_map_rgb(255, 255, 255), SCREEN_W/2, SCREEN_H - 50, ALLEGRO_ALIGN_CENTER, "Press ENTER key to go back to main menu");
    }

    al_flip_display();
}

void game_destroy(void)
{
    al_destroy_font(font_pirulen_32);
    al_destroy_font(font_pirulen_24);
    al_destroy_font(font_pirulen_12);
    al_destroy_font(font_deltarune_16);
    al_destroy_font(font_MyGame_50);
    al_destroy_font(font_gameover_100);
    al_destroy_font(font_gameover_45);
    al_destroy_font(font_gameover_55);
    al_destroy_font(font_darkfuture_50);

    /* Menu Scene resources*/
    al_destroy_bitmap(main_img_background);
    al_destroy_sample(main_bgm);
    al_destroy_bitmap(img_settings);
    al_destroy_bitmap(img_settings2);

    /* Start Scene resources*/
    al_destroy_bitmap(start_img_background);
    al_destroy_bitmap(start_img_plane);
    al_destroy_bitmap(plane2_img);
    al_destroy_bitmap(plane3_img);
    al_destroy_bitmap(plane4_img);
    al_destroy_bitmap(plane5_img);
    al_destroy_bitmap(start_img_enemy);
    al_destroy_sample(start_bgm);
    al_destroy_bitmap(img_bullet);
    al_destroy_bitmap(img_ebullet);
    al_destroy_bitmap(img_heart);
    al_destroy_bitmap(extra_heart_img);
    al_destroy_bitmap(img_ulti);
    al_destroy_sample(ulti_bgm);
    al_destroy_sample(bullet_sound_bgm);
    al_destroy_sample(bullet_clashing_bgm);
    al_destroy_timer(game_update_timer);
    al_destroy_event_queue(game_event_queue);
    al_destroy_display(game_display);
    free(mouse_state);

    /* Settings scene resources */
    al_destroy_bitmap(settings_img_background);
    al_destroy_bitmap(back_icon_img);
    al_destroy_bitmap(arrow_icon_img);
    al_destroy_bitmap(plane_icon_img);
    al_destroy_bitmap(plane2_icon_img);
    al_destroy_bitmap(plane3_icon_img);
    al_destroy_bitmap(plane4_icon_img);
    al_destroy_bitmap(plane5_icon_img);
    al_destroy_bitmap(character_choose_img);
    al_destroy_bitmap(lock_icon_img);
    al_destroy_sample(settings_bgm);

    /* stage 2 resources */
    al_destroy_bitmap(stage2_img_backgroud);
    al_destroy_bitmap(fire_icon_img);
    al_destroy_bitmap(enemy_stage2_img);

    /* stage 3 resources */
    al_destroy_bitmap(stage3_img_backgroud);

    /* boss scene resources */
    al_destroy_bitmap(boss_stage_img_backgroud);
    al_destroy_bitmap(boss_img);
    al_destroy_bitmap(boss_minion_img);
    al_destroy_bitmap(boss_laser_img);
    al_destroy_sample(boss_battle_bgm);

    /* trasition resources */
    al_destroy_bitmap(randomicon_img);
    al_destroy_bitmap(hearticon_img);
    al_destroy_bitmap(speedicon_img);
    al_destroy_bitmap(speedicon2_img);
    al_destroy_bitmap(ebullet_icon_img);
    al_destroy_bitmap(bullet_icon);
    al_destroy_sample(transision_bgm);
    al_destroy_sample(before_boss_bgm);
    al_destroy_sample(bought_bgm);
    al_destroy_bitmap(boss_icon_img);
    al_destroy_bitmap(boss_minion_icon_img);
    al_destroy_bitmap(warning_img);

    /* collision sound */
    al_destroy_sample(enemy_die_bgm);
    al_destroy_sample(plane_die_bgm);
    al_destroy_sample(boss_damaged_bgm);
    al_destroy_sample(laser_bgm);

    /* win and losss scene */
    al_destroy_sample(win_bgm);
    al_destroy_sample(loss_bgm);
}

void game_change_scene(int next_scene)
{
    game_log("Change scene from %d to %d", active_scene, next_scene);

    if (active_scene == SCENE_MENU)
    {
        al_stop_sample(&main_bgm_id);
        game_log("stop audio (bgm)");
    }

    else if (active_scene == SCENE_START || active_scene == STAGE_2 || active_scene == STAGE_3)
    {
        al_stop_sample(&start_bgm_id);
        game_log("stop audio (bgm)");
    }

    else if (active_scene == BOSS_STAGE)
    {
        al_stop_sample(&boss_battle_bgm_id);
        game_log("stop audio (bgm)");
    }

    else if (active_scene == SCENE_WIN)
    {
        al_stop_sample(&win_bgm_id);
        game_log("stop audio (bgm)");
    }

    else if (active_scene == SCENE_LOSS)
    {
        al_stop_sample(&loss_bgm_id);
        game_log("stop audio (bgm)");
    }

    else if (active_scene == SCENE_TRANSITION_12 || active_scene == SCENE_TRANSITION_23)
    {
        al_stop_sample(&transision_bgm_id);
        game_log("failed to load hello (bgm)");
    }

    else if (active_scene == SCENE_SETTINGS)
    {
        al_stop_sample(&settings_bgm_id);
        game_log("failed to load (bgm)");
    }

    else if (active_scene == SCENE_TRANSITION_34)
    {
        al_stop_sample(&before_boss_bgm_id);
        game_log("failed to load (bgm)");
    }

    before_scene = active_scene;
    active_scene = next_scene;

    if (active_scene == SCENE_MENU)
    {
        if (!al_play_sample(main_bgm, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &main_bgm_id))
            game_abort("failed to play audio (bgm)");
    }

    if (active_scene == SCENE_START)
    {
        int i;

        if(plane1 == 1)
        {
            plane.img = start_img_plane;
            plane.w = al_get_bitmap_width(plane.img);
            plane.h = al_get_bitmap_height(plane.img);
            plane.hp = 15;
            plane.score = 0;
            plane.money = 0;
            plane_speed = 4;
            bullet_speed = 0;
            ori_hp = 15;
        }

        if(plane2 == 1)
        {
            plane.img = plane2_img;
            plane.w = al_get_bitmap_width(plane2_img);
            plane.h = al_get_bitmap_height(plane2_img);
            plane.hp = 10;
            plane.score = 0;
            plane.money = 0;
            plane_speed = 6;
            bullet_speed = 2;
            ori_hp = 10;
        }

        if(plane3 == 1)
        {
            plane.img = plane3_img;
            plane.w = al_get_bitmap_width(plane3_img);
            plane.h = al_get_bitmap_height(plane3_img);
            plane.hp = 30;
            plane.score = 0;
            plane.money = 0;
            plane_speed = 2;
            bullet_speed = 0;
            ori_hp = 30;
        }

        if(plane4 == 1)
        {
            plane.img = plane4_img;
            plane.w = al_get_bitmap_width(plane4_img);
            plane.h = al_get_bitmap_height(plane4_img);
            plane.hp = 5;
            plane.score = 0;
            plane.money = 0;
            plane_speed = 8;
            bullet_speed = 4;
            ori_hp = 5;
        }

        if(plane5 == 1)
        {
            plane.img = plane5_img;
            plane.w = al_get_bitmap_width(plane5_img);
            plane.h = al_get_bitmap_height(plane5_img);
            plane.hp = 15;
            plane.score = 0;
            plane.money = 0;
            plane_speed = 5;
            bullet_speed = 0;
            ori_hp = 15;
            money_buff = 2;
            score_buff = 2;
        }

        plane.x = 400;
        plane.y = 500;
        plane.hidden = false;

        for(i = 0; i < 3; i++)
        {
            hearts[i].img = img_heart;
            hearts[i].w = al_get_bitmap_width(img_heart);
            hearts[i].h = al_get_bitmap_height(img_heart);
            hearts[i].hidden = false;
        }

        for(i = 3; i < MAX_LIVES; i++)
        {
            hearts[i].img = extra_heart_img;
            hearts[i].w = al_get_bitmap_width(extra_heart_img);
            hearts[i].h = al_get_bitmap_height(extra_heart_img);
            hearts[i].hidden = false;
        }

        if(plane.hp > 25)
        {
            hearts[0].x = plane.x - plane.w/2;
            hearts[0].y = plane.y + plane.h/2 + 10;
            hearts[1].x = plane.x;
            hearts[1].y = plane.y + plane.h/2 + 10;
            hearts[2].x = plane.x + plane.w/2;
            hearts[2].y = plane.y + plane.h/2 + 10;
            hearts[3].x = plane.x - plane.w/2;
            hearts[3].y = plane.y + plane.h/2 + 10;
            hearts[4].x = plane.x;
            hearts[4].y = plane.y + plane.h/2 + 10;
            hearts[5].x = plane.x + plane.w/2;
            hearts[5].y = plane.y + plane.h/2 + 10;
        }

        else if(plane.hp > 20)
        {
            hearts[0].x = plane.x - plane.w/2;
            hearts[0].y = plane.y + plane.h/2 + 10;
            hearts[1].x = plane.x;
            hearts[1].y = plane.y + plane.h/2 + 10;
            hearts[2].x = plane.x + plane.w/2;
            hearts[2].y = plane.y + plane.h/2 + 10;
            hearts[3].x = plane.x - plane.w/2;
            hearts[3].y = plane.y + plane.h/2 + 10;
            hearts[4].x = plane.x;
            hearts[4].y = plane.y + plane.h/2 + 10;
        }

        else if(plane.hp > 15)
        {
            hearts[0].x = plane.x - plane.w/2;
            hearts[0].y = plane.y + plane.h/2 + 10;
            hearts[1].x = plane.x;
            hearts[1].y = plane.y + plane.h/2 + 10;
            hearts[2].x = plane.x + plane.w/2;
            hearts[2].y = plane.y + plane.h/2 + 10;
            hearts[3].x = plane.x - plane.w/2;
            hearts[3].y = plane.y + plane.h/2 + 10;
        }

        else if(plane.hp > 10)
        {
            hearts[0].x = plane.x - plane.w/2;
            hearts[0].y = plane.y + plane.h/2 + 10;
            hearts[1].x = plane.x;
            hearts[1].y = plane.y + plane.h/2 + 10;
            hearts[2].x = plane.x + plane.w/2;
            hearts[2].y = plane.y + plane.h/2 + 10;
        }

        else if(plane.hp > 5)
        {
            hearts[0].x = plane.x - plane.w/2;
            hearts[0].y = plane.y + plane.h/2 + 10;
            hearts[1].x = plane.x;
            hearts[1].y = plane.y + plane.h/2 + 10;
        }

        else if(plane.hp > 0)
        {
            hearts[1].x = plane.x;
            hearts[1].y = plane.y + plane.h/2 + 10;
        }

        for (i = 0; i < MAX_ENEMY; i++)
        {
            enemies[i].img = start_img_enemy;
            enemies[i].w = al_get_bitmap_width(start_img_enemy);
            enemies[i].h = al_get_bitmap_height(start_img_enemy);
            enemies[i].x = enemies[i].w / 2 + (float)rand() / RAND_MAX * (SCREEN_W - enemies[i].w);
            enemies[i].y = 80;
            enemies[i].vx = 0;
            enemies[i].vy = 1 + increasing_speed_enemy;
        }

        for (i = 0; i < MAX_BULLET; i++)
        {
            bullets[i].w = al_get_bitmap_width(img_bullet);
            bullets[i].h = al_get_bitmap_height(img_bullet);
            bullets[i].img = img_bullet;
            bullets[i].vx = 0;
            bullets[i].vy = -3 - bullet_speed;
            bullets[i].hidden = true;
        }

        increasing_speed_bullet += 3;
        increasing_speed_enemy += 1;
        if (!al_play_sample(start_bgm, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &start_bgm_id))
            game_abort("failed to play audio (bgm)");
    }

    if (active_scene == STAGE_2 || active_scene == STAGE_3)
    {
        int i;

        if(plane1 == 1)
        {
            plane.img = start_img_plane;
            plane.w = al_get_bitmap_width(plane.img);
            plane.h = al_get_bitmap_height(plane.img);
            plane.hp = 15 - lost_health;
            plane.score = 0 + b_score;
            plane.money = 0 + b_money;
            plane_speed = 4;
            bullet_speed = 0;
            ori_hp = 15;
        }

        if(plane2 == 1)
        {
            plane.img = plane2_img;
            plane.w = al_get_bitmap_width(plane2_img);
            plane.h = al_get_bitmap_height(plane2_img);
            plane.hp = 10 - lost_health;
            plane.score = 0 + b_score;
            plane.money = 0 + b_money;
            plane_speed = 6;
            bullet_speed = 2;
            ori_hp = 10;
        }

        if(plane3 == 1)
        {
            plane.img = plane3_img;
            plane.w = al_get_bitmap_width(plane3_img);
            plane.h = al_get_bitmap_height(plane3_img);
            plane.hp = 30 - lost_health;
            plane.score = 0 + b_score;
            plane.money = 0 + b_money;
            plane_speed = 2;
            bullet_speed = 0;
            ori_hp = 30;
        }

        if(plane4 == 1)
        {
            plane.img = plane4_img;
            plane.w = al_get_bitmap_width(plane4_img);
            plane.h = al_get_bitmap_height(plane4_img);
            plane.hp = 5 - lost_health;
            plane.score = 0 + b_score;
            plane.money = 0 + b_money;
            plane_speed = 8;
            bullet_speed = 4;
            ori_hp = 5;
        }

        if(plane5 == 1)
        {
            plane.img = plane5_img;
            plane.w = al_get_bitmap_width(plane5_img);
            plane.h = al_get_bitmap_height(plane5_img);
            plane.hp = 15 - lost_health;
            plane.score = 0 + b_score;
            plane.money = 0 + b_money;
            plane_speed = 5;
            bullet_speed = 0;
            ori_hp = 15;
        }

        plane.x = 400;
        plane.y = 500;
        plane.hidden = false;

        for(i = 0; i < 3; i++)
        {
            hearts[i].img = img_heart;
            hearts[i].w = al_get_bitmap_width(img_heart);
            hearts[i].h = al_get_bitmap_height(img_heart);
            hearts[i].hidden = false;
        }

        for(i = 3; i < MAX_LIVES; i++)
        {
            hearts[i].img = extra_heart_img;
            hearts[i].w = al_get_bitmap_width(extra_heart_img);
            hearts[i].h = al_get_bitmap_height(extra_heart_img);
            hearts[i].hidden = false;
        }

        if(plane.hp > 25)
        {
            hearts[0].x = plane.x - plane.w/2;
            hearts[0].y = plane.y + plane.h/2 + 10;
            hearts[1].x = plane.x;
            hearts[1].y = plane.y + plane.h/2 + 10;
            hearts[2].x = plane.x + plane.w/2;
            hearts[2].y = plane.y + plane.h/2 + 10;
            hearts[3].x = plane.x - plane.w/2;
            hearts[3].y = plane.y + plane.h/2 + 10;
            hearts[4].x = plane.x;
            hearts[4].y = plane.y + plane.h/2 + 10;
            hearts[5].x = plane.x + plane.w/2;
            hearts[5].y = plane.y + plane.h/2 + 10;
        }

        else if(plane.hp > 20)
        {
            hearts[0].x = plane.x - plane.w/2;
            hearts[0].y = plane.y + plane.h/2 + 10;
            hearts[1].x = plane.x;
            hearts[1].y = plane.y + plane.h/2 + 10;
            hearts[2].x = plane.x + plane.w/2;
            hearts[2].y = plane.y + plane.h/2 + 10;
            hearts[3].x = plane.x - plane.w/2;
            hearts[3].y = plane.y + plane.h/2 + 10;
            hearts[4].x = plane.x;
            hearts[4].y = plane.y + plane.h/2 + 10;
        }

        else if(plane.hp > 15)
        {
            hearts[0].x = plane.x - plane.w/2;
            hearts[0].y = plane.y + plane.h/2 + 10;
            hearts[1].x = plane.x;
            hearts[1].y = plane.y + plane.h/2 + 10;
            hearts[2].x = plane.x + plane.w/2;
            hearts[2].y = plane.y + plane.h/2 + 10;
            hearts[3].x = plane.x - plane.w/2;
            hearts[3].y = plane.y + plane.h/2 + 10;
        }

        else if(plane.hp > 10)
        {
            hearts[0].x = plane.x - plane.w/2;
            hearts[0].y = plane.y + plane.h/2 + 10;
            hearts[1].x = plane.x;
            hearts[1].y = plane.y + plane.h/2 + 10;
            hearts[2].x = plane.x + plane.w/2;
            hearts[2].y = plane.y + plane.h/2 + 10;
        }

        else if(plane.hp > 5)
        {
            hearts[0].x = plane.x - plane.w/2;
            hearts[0].y = plane.y + plane.h/2 + 10;
            hearts[1].x = plane.x;
            hearts[1].y = plane.y + plane.h/2 + 10;
        }

        else if(plane.hp > 0)
        {
            hearts[1].x = plane.x;
            hearts[1].y = plane.y + plane.h/2 + 10;
        }

        for (i = 0; i < MAX_ENEMY; i++)
        {
            enemies[i].img = start_img_enemy;
            enemies[i].w = al_get_bitmap_width(start_img_enemy);
            enemies[i].h = al_get_bitmap_height(start_img_enemy);
            enemies[i].x = enemies[i].w / 2 + (float)rand() / RAND_MAX * (SCREEN_W - enemies[i].w);
            enemies[i].y = 80;
            enemies[i].vx = 0;
            enemies[i].vy = 1 + increasing_speed_enemy;
        }

        for (i = 0; i < MAX_BULLET; i++)
        {
            bullets[i].w = al_get_bitmap_width(img_bullet);
            bullets[i].h = al_get_bitmap_height(img_bullet);
            bullets[i].img = img_bullet;
            bullets[i].vx = 0;
            bullets[i].vy = -3 - bought_speed_bullet - bullet_speed;
            bullets[i].hidden = true;
        }

        for(i = 0; i < MAX_ULTI; i++)
        {
            ulti[i].img = img_ulti;
            ulti[i].w = al_get_bitmap_width(img_ulti);
            ulti[i].h = al_get_bitmap_height(img_ulti);
            ulti[i].vx = 0;
            ulti[i].vy = -5;
            ulti[i].hidden = true;
        }

        for(i = 0; i < MAX_EBULLET; i++)
        {
            e_bullets[i].w = al_get_bitmap_width(img_ebullet);
            e_bullets[i].h = al_get_bitmap_height(img_ebullet);
            e_bullets[i].img = img_ebullet;
            e_bullets[i].vx = 0;
            e_bullets[i].vy = 0 + increasing_speed_bullet;
            e_bullets[i].hidden = true;
        }


        increasing_speed_bullet += 3;
        increasing_speed_enemy += 1;
        if (!al_play_sample(start_bgm, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &start_bgm_id))
            game_abort("failed to play audio (bgm)");
    }

    if (active_scene == BOSS_STAGE)
    {
        int i;

        if(plane1 == 1)
        {
            plane.img = start_img_plane;
            plane.w = al_get_bitmap_width(plane.img);
            plane.h = al_get_bitmap_height(plane.img);
            plane.hp = 15 - lost_health;
            plane.score = 0 + b_score;
            plane.money = 0 + b_money;
            plane_speed = 4;
            bullet_speed = 0;
            ori_hp = 15;
        }

        if(plane2 == 1)
        {
            plane.img = plane2_img;
            plane.w = al_get_bitmap_width(plane2_img);
            plane.h = al_get_bitmap_height(plane2_img);
            plane.hp = 10 - lost_health;
            plane.score = 0 + b_score;
            plane.money = 0 + b_money;
            plane_speed = 6;
            bullet_speed = 2;
            ori_hp = 10;
        }

        if(plane3 == 1)
        {
            plane.img = plane3_img;
            plane.w = al_get_bitmap_width(plane3_img);
            plane.h = al_get_bitmap_height(plane3_img);
            plane.hp = 30 - lost_health;
            plane.score = 0 + b_score;
            plane.money = 0 + b_money;
            plane_speed = 2;
            bullet_speed = 0;
            ori_hp = 30;
        }

        if(plane4 == 1)
        {
            plane.img = plane4_img;
            plane.w = al_get_bitmap_width(plane4_img);
            plane.h = al_get_bitmap_height(plane4_img);
            plane.hp = 5 - lost_health;
            plane.score = 0 + b_score;
            plane.money = 0 + b_money;
            plane_speed = 8;
            bullet_speed = 4;
            ori_hp = 5;
        }

        if(plane5 == 1)
        {
            plane.img = plane5_img;
            plane.w = al_get_bitmap_width(plane5_img);
            plane.h = al_get_bitmap_height(plane5_img);
            plane.hp = 15 - lost_health;
            plane.score = 0 + b_score;
            plane.money = 0 + b_money;
            plane_speed = 5;
            bullet_speed = 0;
            ori_hp = 15;
        }


        plane.x = 400;
        plane.y = 500;
        plane.hidden = false;

        for(i = 0; i < 3; i++)
        {
            hearts[i].img = img_heart;
            hearts[i].w = al_get_bitmap_width(img_heart);
            hearts[i].h = al_get_bitmap_height(img_heart);
            hearts[i].hidden = false;
        }

        for(i = 3; i < MAX_LIVES; i++)
        {
            hearts[i].img = extra_heart_img;
            hearts[i].w = al_get_bitmap_width(extra_heart_img);
            hearts[i].h = al_get_bitmap_height(extra_heart_img);
            hearts[i].hidden = false;
        }

        if(plane.hp > 25)
        {
            hearts[0].x = plane.x - plane.w/2;
            hearts[0].y = plane.y + plane.h/2 + 10;
            hearts[1].x = plane.x;
            hearts[1].y = plane.y + plane.h/2 + 10;
            hearts[2].x = plane.x + plane.w/2;
            hearts[2].y = plane.y + plane.h/2 + 10;
            hearts[3].x = plane.x - plane.w/2;
            hearts[3].y = plane.y + plane.h/2 + 10;
            hearts[4].x = plane.x;
            hearts[4].y = plane.y + plane.h/2 + 10;
            hearts[5].x = plane.x + plane.w/2;
            hearts[5].y = plane.y + plane.h/2 + 10;
        }

        else if(plane.hp > 20)
        {
            hearts[0].x = plane.x - plane.w/2;
            hearts[0].y = plane.y + plane.h/2 + 10;
            hearts[1].x = plane.x;
            hearts[1].y = plane.y + plane.h/2 + 10;
            hearts[2].x = plane.x + plane.w/2;
            hearts[2].y = plane.y + plane.h/2 + 10;
            hearts[3].x = plane.x - plane.w/2;
            hearts[3].y = plane.y + plane.h/2 + 10;
            hearts[4].x = plane.x;
            hearts[4].y = plane.y + plane.h/2 + 10;
        }

        else if(plane.hp > 15)
        {
            hearts[0].x = plane.x - plane.w/2;
            hearts[0].y = plane.y + plane.h/2 + 10;
            hearts[1].x = plane.x;
            hearts[1].y = plane.y + plane.h/2 + 10;
            hearts[2].x = plane.x + plane.w/2;
            hearts[2].y = plane.y + plane.h/2 + 10;
            hearts[3].x = plane.x - plane.w/2;
            hearts[3].y = plane.y + plane.h/2 + 10;
        }

        else if(plane.hp > 10)
        {
            hearts[0].x = plane.x - plane.w/2;
            hearts[0].y = plane.y + plane.h/2 + 10;
            hearts[1].x = plane.x;
            hearts[1].y = plane.y + plane.h/2 + 10;
            hearts[2].x = plane.x + plane.w/2;
            hearts[2].y = plane.y + plane.h/2 + 10;
        }

        else if(plane.hp > 5)
        {
            hearts[0].x = plane.x - plane.w/2;
            hearts[0].y = plane.y + plane.h/2 + 10;
            hearts[1].x = plane.x;
            hearts[1].y = plane.y + plane.h/2 + 10;
        }

        else if(plane.hp > 0)
        {
            hearts[1].x = plane.x;
            hearts[1].y = plane.y + plane.h/2 + 10;
        }

        for (i = 0; i < MAX_BULLET; i++)
        {
            bullets[i].w = al_get_bitmap_width(img_bullet);
            bullets[i].h = al_get_bitmap_height(img_bullet);
            bullets[i].img = img_bullet;
            bullets[i].vx = 0;
            bullets[i].vy = -3 - bought_speed_bullet - bullet_speed;
            bullets[i].hidden = true;
        }

        for(i = 0; i < MAX_ULTI; i++)
        {
            ulti[i].img = img_ulti;
            ulti[i].w = al_get_bitmap_width(img_ulti);
            ulti[i].h = al_get_bitmap_height(img_ulti);
            ulti[i].vx = 0;
            ulti[i].vy = -5;
            ulti[i].hidden = true;
        }

        boss.img = boss_img;
        boss.w = al_get_bitmap_width(boss_img);
        boss.h = al_get_bitmap_height(boss_img);
        boss.hidden = false;
        boss.vx = 5;
        boss.vy = 0;
        boss.hp = 100;

        /* boss laser */
        boss_laser.img = boss_laser_img;
        boss_laser.w = al_get_bitmap_width(boss_laser_img);
        boss_laser.h = al_get_bitmap_height(boss_laser_img);
        boss_laser.hidden = true;
        boss_laser.vx = 0;
        boss_laser.vy = 5;

        for(i = 0; i < MAX_MINION; i++)
        {
            boss_minion[i].img = boss_minion_img;
            boss_minion[i].w = al_get_bitmap_width(boss_minion_img);
            boss_minion[i].h = al_get_bitmap_height(boss_minion_img);
            boss_minion[i].x = enemies[i].w / 2 + (float)rand() / RAND_MAX * (SCREEN_W - enemies[i].w);
            boss_minion[i].y = 180;
        }

        /* boss minions speed */
        boss_minion[0].vx = 3;
        boss_minion[0].vy = 3;

        boss_minion[1].vx = -3;
        boss_minion[1].vy = 3;

        boss_minion[2].vx = 0;
        boss_minion[2].vy = 3;

        boss_minion[3].vx = 0;
        boss_minion[3].vy = 3;

        if(!al_play_sample(boss_battle_bgm, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &boss_battle_bgm_id))
            game_abort("failed to play sample: boss_battle.ogg");
    }

    if (active_scene == SCENE_WIN)
    {
        if(!al_play_sample(win_bgm, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &win_bgm_id))
            game_abort("failed to play audio (bgm)");
    }

    if (active_scene == SCENE_LOSS)
    {
        if(!al_play_sample(loss_bgm, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &loss_bgm_id))
            game_abort("failed to play (bgm)");
    }

    if (active_scene == SCENE_TRANSITION_12 || active_scene == SCENE_TRANSITION_23)
    {
        if(!al_play_sample(transision_bgm, 1, 0, 1.0, ALLEGRO_PLAYMODE_LOOP, &transision_bgm_id))
            game_abort("failed to play hey (bgm)");
    }

    if (active_scene == SCENE_SETTINGS)
    {
        if(!al_play_sample(settings_bgm, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &settings_bgm_id))
            game_abort("failed to load (bgm)");
    }

    if (active_scene == SCENE_TRANSITION_34)
    {
        if(!al_play_sample(before_boss_bgm, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &before_boss_bgm_id))
            game_abort("failed to load (bgm)");
    }
}

void on_key_down(int keycode)
{
    if (active_scene == SCENE_MENU)
    {
        if (keycode == ALLEGRO_KEY_ENTER)
            game_change_scene(SCENE_START);

        /* cheat button */
        if (keycode == ALLEGRO_KEY_1)
            game_change_scene(SCENE_WIN);
        if (keycode == ALLEGRO_KEY_2)
            game_change_scene(SCENE_LOSS);
        if (keycode == ALLEGRO_KEY_3)
        {
            scene_counter = 1;
            increasing_speed_bullet = 2;
            increasing_speed_enemy = 1;
            plane.money = 1000;
            game_change_scene(SCENE_TRANSITION_12);
        }
        if (keycode == ALLEGRO_KEY_4)
        {
            scene_counter = 2;
            increasing_speed_bullet = 4;
            increasing_speed_enemy = 2;
            plane.money = 1000;
            game_change_scene(SCENE_TRANSITION_23);
        }
        if (keycode == ALLEGRO_KEY_5)
        {
            plane.money = 10000;
            game_change_scene(SCENE_TRANSITION_34);
        }
    }
    if (active_scene == SCENE_LOSS)
    {
        if (keycode == ALLEGRO_KEY_ENTER)
        {
            if(plane.score + plane.money > highscore)
                highscore = plane.score + plane.money;
            b_money = 0;
            lost_health = 0;
            b_score = 0;
            increasing_speed_bullet = 0;
            increasing_speed_enemy = 0;
            bought_speed_bullet = 0;
            ulti_counter = 0;
            scene_counter = 0;
            game_change_scene(SCENE_START);
        }
        else if (keycode == ALLEGRO_KEY_ESCAPE)
        {
            if(plane.score + plane.money > highscore)
                highscore = plane.score + plane.money;
            b_money = 0;
            lost_health = 0;
            b_score = 0;
            increasing_speed_bullet = 0;
            increasing_speed_enemy = 0;
            bought_speed_bullet = 0;
            ulti_counter = 0;
            scene_counter = 0;
            game_change_scene(SCENE_MENU);
        }
    }
    if (active_scene == SCENE_TRANSITION_12)
    {
        if (keycode == ALLEGRO_KEY_ENTER)
            game_change_scene(STAGE_2);
    }
    if (active_scene == SCENE_TRANSITION_23)
    {
        if(keycode == ALLEGRO_KEY_ENTER)
            game_change_scene(STAGE_3);
    }
    if (active_scene == SCENE_TRANSITION_34)
    {
        if(keycode == ALLEGRO_KEY_ENTER)
            game_change_scene(BOSS_STAGE);
    }
    if (active_scene == SCENE_WIN)
    {
        if(keycode == ALLEGRO_KEY_ENTER)
        {
            if(plane.score + plane.money > highscore)
                highscore = plane.score + plane.money;
            b_money = 0;
            lost_health = 0;
            b_score = 0;
            increasing_speed_bullet = 0;
            increasing_speed_enemy = 0;
            bought_speed_bullet = 0;
            ulti_counter = 0;
            scene_counter = 0;
            game_change_scene(SCENE_MENU);
        }
    }
}

void on_mouse_down(int btn, int x, int y)
{
    if (active_scene == SCENE_MENU)
    {
        if (btn == true)
        {
            if (pnt_in_rect(x, y, SCREEN_W - 48, 10, 38, 38))
                game_change_scene(SCENE_SETTINGS);
        }
    }

    if (active_scene == SCENE_SETTINGS)
    {
        if (btn == true)
        {
            if(pnt_in_rect(x, y, 20, 20, al_get_bitmap_width(back_icon_img), al_get_bitmap_height(back_icon_img)))
                game_change_scene(before_scene);
            if(pnt_in_rect(x, y, 160, SCREEN_H-110, al_get_bitmap_width(plane_icon_img), al_get_bitmap_height(plane_icon_img)))
            {
                plane1 = 1;
                plane2 = plane3 = plane4 = plane5 = 0;
            }

            if(pnt_in_rect(x, y, 240, SCREEN_H - 230, al_get_bitmap_width(plane2_icon_img), al_get_bitmap_height(plane2_icon_img)) && highscore > 6000)
            {
                plane2 = 1;
                plane1 = plane3 = plane4 = plane5 = 0;
            }

            if(pnt_in_rect(x, y, 340, SCREEN_H - 130, al_get_bitmap_width(plane3_icon_img), al_get_bitmap_height(plane3_icon_img)) && highscore > 9000)
            {
                plane3 = 1;
                plane2 = plane1 = plane4 = plane5 = 0;
            }

            if(pnt_in_rect(x, y, 450, SCREEN_H - 230, al_get_bitmap_width(plane4_icon_img), al_get_bitmap_height(plane4_icon_img)) && highscore > 12000)
            {
                plane4 = 1;
                plane2 = plane3 = plane1 = plane5 = 0;
            }

            if(pnt_in_rect(x, y, 540, SCREEN_H - 130, al_get_bitmap_width(plane5_icon_img), al_get_bitmap_height(plane5_icon_img)) && highscore > 15000)
            {
                plane5 = 1;
                plane2 = plane3 = plane4 = plane1 = 0;
            }
        }
    }

    if (active_scene == SCENE_TRANSITION_12 || active_scene == SCENE_TRANSITION_23 || active_scene == SCENE_TRANSITION_34)
    {
        if (btn == true)
        {
            if (pnt_in_rect(x, y, SCREEN_W/2 + 20, SCREEN_H/2 + 50, 200, 30) && plane.money - 50 >= 0)
            {
                al_play_sample(bought_bgm, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, &bought_bgm_id);
                lost_health -= 1;
                plane.money -= 50;
                b_money -= 50;
            }

            if (pnt_in_rect(x, y, SCREEN_W/2 + 20, SCREEN_H/2 + 100, 200, 30) && plane.money - 100 >= 0)
            {
                al_play_sample(bought_bgm, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, &bought_bgm_id);
                bought_speed_bullet += 3;
                plane.money -= 100;
                b_money -= 100;
            }

            if (pnt_in_rect(x, y,  SCREEN_W/2 + 20, SCREEN_H/2 + 150, 200, 30) && plane.money - 150 >= 0)
            {
                al_play_sample(bought_bgm, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, &bought_bgm_id);
                ulti_counter += 1;
                plane.money -= 150;
                b_money -= 150;
            }

            if (pnt_in_rect(x, y, SCREEN_W - 48, 10, 38, 38))
                game_change_scene(SCENE_SETTINGS);
        }
    }
}

void draw_movable_object(MovableObject obj)
{
    if (obj.hidden)
        return;
    al_draw_bitmap(obj.img, round(obj.x - obj.w / 2), round(obj.y - obj.h / 2), 0);
}

ALLEGRO_BITMAP *load_bitmap_resized(const char *filename, int w, int h)
{
    ALLEGRO_BITMAP* loaded_bmp = al_load_bitmap(filename);

    if (!loaded_bmp)
        game_abort("failed to load image: %s", filename);

    ALLEGRO_BITMAP *resized_bmp = al_create_bitmap(w, h);
    ALLEGRO_BITMAP *prev_target = al_get_target_bitmap();

    if (!resized_bmp)
        game_abort("failed to create bitmap when creating resized image: %s", filename);

    al_set_target_bitmap(resized_bmp);
    al_draw_scaled_bitmap(loaded_bmp, 0, 0,
                          al_get_bitmap_width(loaded_bmp),
                          al_get_bitmap_height(loaded_bmp),
                          0, 0, w, h, 0);
    al_set_target_bitmap(prev_target);
    al_destroy_bitmap(loaded_bmp);

    game_log("resized image: %s", filename);

    return resized_bmp;
}

bool pnt_in_rect(int px, int py, int x, int y, int w, int h)
{
    if(x <= px && px <= x+w && y <= py && py <= y+h)
        return 1;
    else
        return 0;
}


// +=================================================================+
// | Code below is for debugging purpose, it's fine to remove it.    |
// | Deleting the code below and removing all calls to the functions |
// | doesn't affect the game.                                        |
// +=================================================================+

void game_abort(const char* format, ...)
{
    va_list arg;
    va_start(arg, format);
    game_vlog(format, arg);
    va_end(arg);
    fprintf(stderr, "error occured, exiting after 2 secs");
    // Wait 2 secs before exiting.
    al_rest(2);
    // Force exit program.
    exit(1);
}

void game_log(const char* format, ...)
{
#ifdef LOG_ENABLED
    va_list arg;
    va_start(arg, format);
    game_vlog(format, arg);
    va_end(arg);
#endif
}

void game_vlog(const char* format, va_list arg)
{
#ifdef LOG_ENABLED
    static bool clear_file = true;
    vprintf(format, arg);
    printf("\n");
    // Write log to file for later debugging.
    FILE* pFile = fopen("log.txt", clear_file ? "w" : "a");
    if (pFile)
    {
        vfprintf(pFile, format, arg);
        fprintf(pFile, "\n");
        fclose(pFile);
    }
    clear_file = false;
#endif
}
