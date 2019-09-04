#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_opengl_glext.h>

#include <chrono>
#include <cmath>
#include <string>
#include <vector>

extern float       width;
extern float       height;
extern std::string resources;
extern GLuint      texture_id;
extern int         location;

const GLenum      VERTEX_SHADER   = GL_VERTEX_SHADER;
const GLenum      FRAGMENT_SHADER = GL_FRAGMENT_SHADER;
const std::string INVERT_TEXTURE  = "Invert texture";
const std::string FULL_SCREEN     = "Full screen";
const std::string WINDOW_MODE     = "Window mode";

extern float mouse_x_pos;
extern float mouse_y_pos;
extern bool  mouse_click;

class timer;
class engine;

struct point
{
    float x = 0.f;
    float y = 0.f;
};

struct vertex
{
    float x = 0.f;
    float y = 0.f;

    float tx = 0.f; // textures
    float ty = 0.f;
};

struct triangle
{
    triangle()
    {
        v[0] = vertex();
        v[1] = vertex();
        v[2] = vertex();
    }
    vertex v[3];
};

struct rectangle
{
    rectangle()
    {
        v[0] = vertex();
        v[1] = vertex();
        v[2] = vertex();
        v[3] = vertex();

        // direction relative to the x-axis
        v[4] = vertex();
    }
    vertex v[5];
};

const float pi = 3.1415926f;

struct mouse_position
{
    float x = 0;
    float y = 0;
};

//====================== ENGINE ===============================================
class engine
{
public:
    // engine();
    //~engine();
    void set_resources_path(std::string str);

    void set_window_width(int w);
    void set_window_height(int h);
    bool initialize(std::string screen_mode_type);
    bool events();
    void swap_buffers();

    //=== sound ===
    Mix_Music* load_music(std::string filename);
    void       play_music(Mix_Music* music, int volume);
    void       pause_music();
    void       resume_music();
    void       stop_music();
    Mix_Chunk* load_sound(std::string filename);
    void       play_sound(Mix_Chunk* music, int volume);
    void       play_sound(Mix_Chunk* music_chunk, int volume, int channel);
    void play_sound(Mix_Chunk* music_chunk, int volume, int channel, int loop);
    void stop_sound(int channel);

    //=== keyboard ===
    bool key_W_pressed();
    bool key_S_pressed();
    bool key_A_pressed();
    bool key_D_pressed();
    bool key_SPACE_pressed();
    bool key_LCTRL_pressed();
    bool key_ENTER_pressed();
    bool key_Esc_pressed();
    bool key_Q_pressed();
    bool key_E_pressed();

    //=== openGL ===
    // void create_shader(const std::string shader_src, const GLenum
    // SHADER_TYPE);
    void  old_create_shader();
    void  render_triangle(const triangle& t);
    void  scale_to_screen(triangle& t);
    void  scale_to_screen(rectangle& r);
    float get_k_screen();

    rectangle create_rectangle(float center_x, float center_y, float width_x,
                               float height_y);
    rectangle create_rectangle(float center_x, float center_y, float width_x,
                               float height_y, std::string invert);
    rectangle create_noscale_rectangle(float center_x, float center_y,
                                       float width_x, float height_y);
    rectangle create_noscale_rectangle(float center_x, float center_y,
                                       float width_x, float height_y,
                                       std::string invert);

    void render_rectangle(const rectangle& r);

    //=== Textures ===
    GLuint load_image(std::string filename);
    void   render_textured_rectangle(const rectangle& r, GLint texture_number);

    //=== Transformations ===
    void trans_matrix(float fdeltaX, float fdeltaY, rectangle& r);
    void rotate_matrix(float frotate_angle, rectangle& r);
    void scale_triangle(float fscale_valueX, float fscale_valueY, triangle& r);
    void scale_rectangle(float fscale_valueX, float fscale_valueY,
                         rectangle& r);

    //=== Recalculation from screen to opengl coordinate system
    point from_screen_to_opengl(const float& x_input, const float& y_input);
    point from_screen_to_opengl2(const float& x_input, const float& y_input);

    //===== PAUSE ====
    void pause_ms(int ms);

    static int sound_channel;

private:
    SDL_Window* window = nullptr;
    SDL_Event   test_event;

    //=== screen aspect ratio ===
    float k_screen = 1.0f; //= height / width;

    //==== mooving speed X Y ====
    float speed_to_right       = 0.0f;
    float speed_to_left        = 0.0f;
    float speed_to_up          = 0.0f;
    float speed_to_down        = 0.0f;
    float speed_to_right_value = 0.0f;
    float speed_to_left_value  = 0.0f;
    float speed_to_up_value    = 0.0f;
    float speed_to_down_value  = 0.0f;

    //=== sound varibles ===
    Mix_Music* music;
    Mix_Chunk* music_chunk;
    // int channel;

    //=== keyboard flags===
    bool key_W_flag     = false;
    bool key_S_flag     = false;
    bool key_A_flag     = false;
    bool key_D_flag     = false;
    bool key_SPACE_flag = false;
    bool key_LCTRL_flag = false;
    bool key_ENTER_flag = false;
    bool key_Esc_flag   = false;
    bool key_Q_flag     = false;
    bool key_E_flag     = false;
    bool key_MOUSE_flag = false;

    //=== openGL ===
    GLuint program = 0;
};

//====================== PERSON ==============================================
class person : public engine
{
public:
    person();

    person(float center_x, float center_y, float width_x, float height_y);

    person(float center_x, float center_y, float width_x, float height_y,
           std::string invert);

    ~person();

    void set_geometry(float center_x, float center_y, float width_x,
                      float height_y);

    void copy_geometry(person& input);

    void set_noscale_geometry(float center_x, float center_y, float width_x,
                              float height_y);

    void set_geometry(float center_x, float center_y, float width_x,
                      float height_y, std::string invert);

    void set_x_speed(float sp_x);
    void set_y_speed(float sp_y);

    void move();
    void move(float speed);

    void render();
    void render_with_texture();
    void render_with_texture(int number);

    rectangle& get_geometry();

    void   set_texture(std::string filename);
    void   set_texture(GLuint texture_id);
    GLuint get_texture();

    void set_texture_to_sprite(std::string filename);

    int get_from_sprite(int num);

    std::vector<GLuint>& get_sprite();

    float get_x_center();
    float get_y_center();

    bool mouse_botton_pressed();

    void normalize_vector(point& v);

    float mouse_click_angle();
    float current_angle();
    float return_angle(const point input_point);
    float rotate_direction(const point input);

    point get_direction_vector();

    void  set_health(float value);
    void  make_damage(float value);
    float get_health();
    void  set_render_marker(bool value);
    bool  get_render_marker();

private:
    bool                render_this = true;
    float               health      = 10.0f;
    std::vector<GLuint> person_sprite;
    rectangle           person_geometry;
    point               person_direction;
    float               speed_x           = 0.0f;
    float               speed_y           = 0.0f;
    GLuint              person_texture_id = 0;

    float previous_angle = 0.0f;
};

//====================== ANIMATION ============================================
class animation
{
public:
    animation();
    ~animation();

    void set_fps(float fps_value);
    void set_number_of_frames(float f);
    void restart();
    int  current_frame_number(float delta_time);
    void set_last_frame(int lf);
    int  get_last_frame();

private:
    int   number_of_frames; // textures number in one animation
    float fps;
    float current_time = 0.f;
    int   last_frame   = 0;
};

//====================== TIMER ================================================
class timer
{
public:
    timer();
    void   reset();
    double elapsed() const;

private:
    using clock_t  = std::chrono::high_resolution_clock;
    using second_t = std::chrono::duration<double, std::ratio<1>>;

    std::chrono::time_point<clock_t> m_beg;
};

//====================== CHECK THE COLLISION FUNCTION =========================

int collision_check(person& a, person& b);
