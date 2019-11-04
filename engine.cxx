#include "engine.hxx"
#include <iostream>

PFNGLCREATESHADERPROC glCreateShader = nullptr;
PFNGLSHADERSOURCEPROC glShaderSource = nullptr;
PFNGLCOMPILESHADERPROC glCompileShader = nullptr;
PFNGLGETSHADERIVPROC glGetShaderiv = nullptr;
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog = nullptr;
PFNGLDELETESHADERPROC glDeleteShader = nullptr;
PFNGLCREATEPROGRAMPROC glCreateProgram = nullptr;
PFNGLATTACHSHADERPROC glAttachShader = nullptr;
PFNGLBINDATTRIBLOCATIONPROC glBindAttribLocation = nullptr;
PFNGLLINKPROGRAMPROC glLinkProgram = nullptr;
PFNGLGETPROGRAMIVPROC glGetProgramiv = nullptr;
PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog = nullptr;
PFNGLDELETEPROGRAMPROC glDeleteProgram = nullptr;
PFNGLUSEPROGRAMPROC glUseProgram = nullptr;
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer = nullptr;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray = nullptr;
PFNGLVALIDATEPROGRAMPROC glValidateProgram = nullptr;
PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation = nullptr;
PFNGLUNIFORM1IPROC glUniform1i = nullptr;
PFNGLACTIVETEXTUREPROC glActiveTexture_ = nullptr;
PFNGLUNIFORM4FVPROC glUniform4fv = nullptr;
PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray = nullptr;

float width = 100.0f;
float height = 100.0f;
std::string resources = "res";
GLuint texture_id = 0;
int location;

float mouse_x_pos = 0.f;
float mouse_y_pos = 0.f;

bool mouse_click = false;
int engine::sound_channel = 0;

//==========================================================================
template <typename T>
static void load_gl_func(const char *func_name, T &result)
{
    void *gl_pointer = SDL_GL_GetProcAddress(func_name);
    result = reinterpret_cast<T>(gl_pointer);
    if (nullptr == gl_pointer)
    {
        throw std::runtime_error(std::string("can't load GL function") +
                                 func_name);
    }
    result = reinterpret_cast<T>(gl_pointer);
}

//====================== ENGINE ===============================================
void engine::set_resources_path(std::string str)
{
    resources = str;
}

//==========================================================================
void engine::set_window_width(int w)
{
    width = w;
}

//==========================================================================
void engine::set_window_height(int h)
{
    height = h;
}

//==========================================================================
bool engine::initialize(std::string screen_mode_type)
{
    if (!(screen_mode_type == FULL_SCREEN) &&
        !(screen_mode_type == WINDOW_MODE))
    {
        std::cerr << "screen_mode_type  \n!!"
                  << "must be FULL_SCREEN or WINDOW_MODE\n";
        return EXIT_FAILURE;
    }

    if ((SDL_Init(SDL_INIT_EVERYTHING)) != 0)
    {
        const char *err_message = SDL_GetError();
        std::cerr << "error: failed call SDL_Init: " << err_message
                  << std::endl;
        return EXIT_FAILURE;
    }

    window = SDL_CreateWindow("Panzer Engine Demo", SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED, width, height,
                              ::SDL_WINDOW_OPENGL);
    if (screen_mode_type == FULL_SCREEN)
    {
        SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
    }

    // SDL_ShowCursor(SDL_DISABLE);

    if (window == nullptr)
    {
        std::cerr << "error: failed call SDL_CreateWindow: " << std::endl;
        SDL_Quit();
        return EXIT_FAILURE;
    }

    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    SDL_assert(gl_context != nullptr);

    load_gl_func("glCreateShader", glCreateShader);
    load_gl_func("glShaderSource", glShaderSource);
    load_gl_func("glCompileShader", glCompileShader);
    load_gl_func("glGetShaderiv", glGetShaderiv);
    load_gl_func("glGetShaderInfoLog", glGetShaderInfoLog);
    load_gl_func("glDeleteShader", glDeleteShader);
    load_gl_func("glCreateProgram", glCreateProgram);
    load_gl_func("glAttachShader", glAttachShader);
    load_gl_func("glBindAttribLocation", glBindAttribLocation);
    load_gl_func("glLinkProgram", glLinkProgram);
    load_gl_func("glGetProgramiv", glGetProgramiv);
    load_gl_func("glGetProgramInfoLog", glGetProgramInfoLog);
    load_gl_func("glDeleteProgram", glDeleteProgram);
    load_gl_func("glUseProgram", glUseProgram);
    load_gl_func("glVertexAttribPointer", glVertexAttribPointer);
    load_gl_func("glEnableVertexAttribArray", glEnableVertexAttribArray);
    load_gl_func("glValidateProgram", glValidateProgram);
    load_gl_func("glGetUniformLocation", glGetUniformLocation);
    load_gl_func("glUniform1i", glUniform1i);
    load_gl_func("glActiveTexture", glActiveTexture_);
    load_gl_func("glUniform4fv", glUniform4fv);
    load_gl_func("glDisableVertexAttribArray", glDisableVertexAttribArray);

    old_create_shader();

    return true;
}

//==========================================================================
bool engine::events()
{

    SDL_PollEvent(&test_event);

    if (test_event.type == SDL_QUIT)
    {
        return false;
    }
    if (test_event.type == SDL_KEYDOWN)
    {
        switch (test_event.key.keysym.sym)
        {
        case (SDLK_w):
            key_W_flag = true;
            speed_to_up = speed_to_up_value;
            break;
        case SDLK_s:
            key_S_flag = true;
            speed_to_down = -speed_to_down_value;
            break;
        case SDLK_a:
            key_A_flag = true;
            speed_to_left = -speed_to_left_value;
            break;
        case SDLK_d:
            key_D_flag = true;
            speed_to_right = speed_to_right_value;
            break;
        case SDLK_LCTRL:
            key_LCTRL_flag = true;
            break;
        case SDLK_SPACE:
            key_SPACE_flag = true;
            break;
        case SDLK_RETURN:
            key_ENTER_flag = true;
            break;
        case SDLK_ESCAPE:
            key_Esc_flag = true;
            return false;
        case (SDLK_q):
            key_Q_flag = true;
            break;
        case (SDLK_e):
            key_E_flag = true;
            break;
        default:
            break;
        }
    }
    else if (test_event.type == SDL_KEYUP)
    {
        switch (test_event.key.keysym.sym)
        {
        case SDLK_w:
            key_W_flag = false;
            speed_to_up = 0.0f;
            break;
        case SDLK_s:
            key_S_flag = false;
            speed_to_down = 0.0f;
            break;
        case SDLK_a:
            key_A_flag = false;
            speed_to_left = 0.0f;
            break;
        case SDLK_d:
            key_D_flag = false;
            speed_to_right = 0.0f;
            break;
        case SDLK_LCTRL:
            key_LCTRL_flag = false;
            break;
        case SDLK_SPACE:
            key_SPACE_flag = false;
            break;
        case SDLK_RETURN:
            key_ENTER_flag = false;
            break;
        case (SDLK_q):
            key_Q_flag = false;
            break;
        case (SDLK_e):
            key_E_flag = false;
            break;
        default:
            break;
        }
    }
    //===== mouse ====
    else if (test_event.type == SDL_MOUSEBUTTONDOWN)
    {
        if (test_event.button.button == SDL_BUTTON_LEFT)
        {
            int w = 0;
            int h = 0;
            SDL_GetWindowSize(window, &w, &h);

            key_MOUSE_flag = true;
            // pr_mouse_x_pos = mouse_x_pos;
            // pr_mouse_y_pos = mouse_y_pos;
            mouse_y_pos = test_event.button.y;
            mouse_x_pos = test_event.button.x;
            mouse_click = true;
        }
    }
    if (test_event.type == SDL_MOUSEBUTTONUP)
    {
        if (test_event.button.button == SDL_BUTTON_LEFT)
        {
            mouse_click = false;
        }
    }

    return true;
}

//==========================================================================
void engine::swap_buffers()
{
    SDL_GL_SwapWindow(window);
    glClearColor(0.95f, 0.95f, 1.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

//==========================================================================
Mix_Music *engine::load_music(std::string filename)
{
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);

    std::string sound_parth_and_name = resources + filename;

    music = Mix_LoadMUS(sound_parth_and_name.data());

    if (!music)
    {
        std::cerr << "music file " << sound_parth_and_name << " is not loaded\n"
                  << Mix_GetError() << "\n";
    }

    return music;
}

//==========================================================================
void engine::play_music(Mix_Music *music, int volume)
{
    // loop -1  - endless
    // loop = 0 - not looping
    Mix_VolumeMusic(volume);
    Mix_PlayMusic(music, -1);
}

//==========================================================================
Mix_Chunk *engine::load_sound(std::string filename)
{
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);

    std::string sound_parth_and_name = resources + filename;

    music_chunk = Mix_LoadWAV(sound_parth_and_name.data());

    if (!music_chunk)
    {
        std::cerr << "music_chunk " << sound_parth_and_name
                  << " is not loaded\n"
                  << Mix_GetError() << "\n";
    }
    return music_chunk;
}

//==========================================================================
void engine::pause_music()
{
    Mix_PauseMusic();
}

//==========================================================================
void engine::resume_music()
{
    Mix_ResumeMusic();
}

//==========================================================================
void engine::play_sound(Mix_Chunk *music_chunk, int volume)
{
    Mix_VolumeChunk(music_chunk, volume);
    Mix_PlayChannel(-1, music_chunk, 0);
}

//==========================================================================
void engine::play_sound(Mix_Chunk *music_chunk, int volume, int channel)
{
    Mix_VolumeChunk(music_chunk, volume);
    Mix_PlayChannel(channel, music_chunk, 0);
}

//==========================================================================
void engine::play_sound(Mix_Chunk *music_chunk, int volume, int channel,
                        int loop)
{
    Mix_VolumeChunk(music_chunk, volume);
    Mix_PlayChannel(channel, music_chunk, loop);
}

//==========================================================================
void engine::stop_music()
{
    Mix_HaltMusic();
}

//==========================================================================
void engine::stop_sound(int channel)
{
    Mix_HaltChannel(channel);
}

//==========================================================================
bool engine::key_W_pressed()
{
    return key_W_flag;
}

//==========================================================================
bool engine::key_S_pressed()
{
    return key_S_flag;
}

//==========================================================================
bool engine::key_A_pressed()
{
    return key_A_flag;
}

//==========================================================================
bool engine::key_D_pressed()
{
    return key_D_flag;
}

//==========================================================================
bool engine::key_SPACE_pressed()
{
    return key_SPACE_flag;
}

//==========================================================================
bool engine::key_LCTRL_pressed()
{
    return key_LCTRL_flag;
}

//==========================================================================
bool engine::key_ENTER_pressed()
{
    return key_ENTER_flag;
}

//==========================================================================
bool engine::key_Esc_pressed()
{
    return key_Esc_flag;
}

//==========================================================================
bool engine::key_Q_pressed()
{
    return key_Q_flag;
}

//==========================================================================
bool engine::key_E_pressed()
{
    return key_E_flag;
}

//==========================================================================
void engine::old_create_shader()
{
    GLuint vert_shader = glCreateShader(GL_VERTEX_SHADER);

    std::string vertex_shader_src =
        "attribute vec2 a_position;"
        "attribute vec2 a_tex_coord;"
        "varying vec2 v_tex_coord;"
        "void         main()"
        "{v_tex_coord = a_tex_coord;"
        "gl_Position = vec4(a_position, 0.0, 1.0);}";

    const char *source = vertex_shader_src.data();
    glShaderSource(vert_shader, 1, &source, nullptr);

    glCompileShader(vert_shader);

    GLint compiled_status = 0;
    glGetShaderiv(vert_shader, GL_COMPILE_STATUS, &compiled_status);

    if (compiled_status == 0)
    {
        std::cerr << "vertex shader compilation faild !!"
                  << "\n";
        glDeleteShader(vert_shader);
    }

    // create fragment shader
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

    std::string fragment_shader_src =
        "varying vec2 v_tex_coord;"
        "uniform sampler2D s_texture;"
        "void main()"
        "{gl_FragColor = texture2D(s_texture, v_tex_coord);}";
    source = fragment_shader_src.data();
    glShaderSource(fragment_shader, 1, &source, nullptr);

    glCompileShader(fragment_shader);

    compiled_status = 0;
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &compiled_status);

    if (compiled_status == 0)
    {
        std::cerr << "fragment shader compilation faild !!"
                  << "\n";
        glDeleteShader(fragment_shader);
    }

    program = glCreateProgram();

    glAttachShader(program, vert_shader);
    glAttachShader(program, fragment_shader);

    glBindAttribLocation(program, 0, "a_position");
    glLinkProgram(program);
    glUseProgram(program);
    location = glGetUniformLocation(program, "s_texture");
    if (location < 0)
    {
        std::cerr << "unuform problem !!"
                  << "\n";
    }
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

//==========================================================================
void engine::render_triangle(const triangle &t)
{
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), &t.v[0]);
    glEnableVertexAttribArray(0);
    glValidateProgram(program);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

//==========================================================================
void engine::scale_to_screen(triangle &t)
{
    float k = height / width;
    t.v[0].x = t.v[0].x * k;
    t.v[1].x = t.v[1].x * k;
    t.v[2].x = t.v[2].x * k;
}

//==========================================================================
void engine::scale_to_screen(rectangle &r)
{
    float k = height / width;
    r.v[0].x = r.v[0].x * k;
    r.v[1].x = r.v[1].x * k;
    r.v[2].x = r.v[2].x * k;
    r.v[3].x = r.v[3].x * k;
}

//==========================================================================
float engine::get_k_screen()
{
    return k_screen = height / width;
}

//==========================================================================
// safe CENTER coordinates of the rectangle during autoscaling
rectangle engine::create_rectangle(float center_x, float center_y,
                                   float width_x, float height_y)
{
    // vertex_x, vertex_y --  top-left coordinates
    // 0 top left / -1 -2  anticlockwise / 3 bottom-right

    //  0 ____2
    //  \    \
    //  \____\
    //  1     3

    rectangle r;

    // set direction to the x-axis
    r.v[4].x = center_x;
    r.v[4].y = center_y + height_y / 2;

    float k = height / width;

    r.v[0].x = center_x - (width_x / 2) * k;
    r.v[0].y = center_y + (height_y / 2);

    r.v[1].x = center_x - (width_x / 2) * k;
    r.v[1].y = center_y - (height_y / 2);

    r.v[2].x = center_x + (width_x / 2) * k;
    r.v[2].y = center_y + (height_y / 2);

    r.v[3].x = center_x + (width_x / 2) * k;
    r.v[3].y = center_y - (height_y / 2);

    r.v[0].tx = 0.f;
    r.v[0].ty = 0.f;

    r.v[1].tx = 0.f;
    r.v[1].ty = 1.f;

    r.v[2].tx = 1.f;
    r.v[2].ty = 0.f;

    r.v[3].tx = 1.f;
    r.v[3].ty = 1.f;

    return r;
}

//==========================================================================
// safe CENTER coordinates of the rectangle during autoscaling RELOADED
rectangle engine::create_rectangle(float center_x, float center_y,
                                   float width_x, float height_y,
                                   std::string invert)
{
    // vertex_x, vertex_y --  top-left coordinates
    // 0 top left / -1 -2  anticlockwise / 3 bottom-right

    //  0 ____2
    //  \    \
    //  \____\
    //  1     3

    float k = height / width;

    rectangle r;

    // set direction to the x-axis
    r.v[4].x = center_x;
    r.v[4].y = center_y + height_y / 2;

    r.v[0].x = center_x - (width_x / 2) * k;
    r.v[0].y = center_y + (height_y / 2);

    r.v[1].x = center_x - (width_x / 2) * k;
    r.v[1].y = center_y - (height_y / 2);

    r.v[2].x = center_x + (width_x / 2) * k;
    r.v[2].y = center_y + (height_y / 2);

    r.v[3].x = center_x + (width_x / 2) * k;
    r.v[3].y = center_y - (height_y / 2);

    if (invert == INVERT_TEXTURE)
    {
        r.v[0].tx = 1.f;
        r.v[0].ty = 0.f;

        r.v[1].tx = 1.f;
        r.v[1].ty = 1.f;

        r.v[2].tx = 0.f;
        r.v[2].ty = 0.f;

        r.v[3].tx = 0.f;
        r.v[3].ty = 1.f;
    }
    else
    {
        std::cerr << "incorrect sign of inversion  \n!!"
                  << "must be INVERT_TEXTURE\n";
    }

    return r;
}

//==========================================================================
// NOT SCALE TO SCREEN !!!
rectangle engine::create_noscale_rectangle(float center_x, float center_y,
                                           float width_x, float height_y)
{
    rectangle r;

    // set direction to the x-axis
    r.v[4].x = center_x;
    r.v[4].y = center_y + height_y / 2;

    r.v[0].x = center_x - (width_x / 2);
    r.v[0].y = center_y + (height_y / 2);

    r.v[1].x = center_x - (width_x / 2);
    r.v[1].y = center_y - (height_y / 2);

    r.v[2].x = center_x + (width_x / 2);
    r.v[2].y = center_y + (height_y / 2);

    r.v[3].x = center_x + (width_x / 2);
    r.v[3].y = center_y - (height_y / 2);

    r.v[0].tx = 0.f;
    r.v[0].ty = 0.f;

    r.v[1].tx = 0.f;
    r.v[1].ty = 1.f;

    r.v[2].tx = 1.f;
    r.v[2].ty = 0.f;

    r.v[3].tx = 1.f;
    r.v[3].ty = 1.f;

    return r;
}

//==========================================================================
// NOT SCALE TO SCREEN !!! RELOADED
rectangle engine::create_noscale_rectangle(float center_x, float center_y,
                                           float width_x, float height_y,
                                           std::string invert)
{
    rectangle r;

    // set direction to the x-axis
    r.v[4].x = center_x;
    r.v[4].y = center_y + height_y / 2;

    r.v[0].x = center_x - (width_x / 2);
    r.v[0].y = center_y + (height_y / 2);

    r.v[1].x = center_x - (width_x / 2);
    r.v[1].y = center_y - (height_y / 2);

    r.v[2].x = center_x + (width_x / 2);
    r.v[2].y = center_y + (height_y / 2);

    r.v[3].x = center_x + (width_x / 2);
    r.v[3].y = center_y - (height_y / 2);

    if (invert == INVERT_TEXTURE)
    {
        r.v[0].tx = 1.f;
        r.v[0].ty = 0.f;

        r.v[1].tx = 1.f;
        r.v[1].ty = 1.f;

        r.v[2].tx = 0.f;
        r.v[2].ty = 0.f;

        r.v[3].tx = 0.f;
        r.v[3].ty = 1.f;
    }
    else
    {
        std::cerr << "incorrect sign of inversion  \n!!"
                  << "must be INVERT_TEXTURE\n";
    }

    return r;
}

//==========================================================================
void engine::render_rectangle(const rectangle &r)
{
    glUniform1i(location, 0); //??

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), &r.v[0]);
    glEnableVertexAttribArray(0);
    glValidateProgram(program);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

//==========================================================================
GLuint engine::load_image(std::string filename)
{
    std::string image_parth_and_name = resources + filename;
    const char *file = image_parth_and_name.data();
    SDL_Surface *img = IMG_Load(file);

    if (!img)
    {
        std::cerr << "image file " << image_parth_and_name << " is not loaded\n"
                  << IMG_GetError() << "\n";
    }

    texture_id++;
    // glActiveTexture_(GL_TEXTURE0 + texture_id);

    // glGenTextures(1, &texture_id);
    glGenTextures(1, &texture_id);

    // glGenTextures(1, tex_id_array);
    glActiveTexture_(GL_TEXTURE0 + texture_id);

    glBindTexture(GL_TEXTURE_2D, texture_id);

    GLint mipmap_level = 0;
    GLint border = 0;
    glTexImage2D(GL_TEXTURE_2D, mipmap_level, GL_RGBA, img->w, img->h, border,
                 GL_RGBA, GL_UNSIGNED_BYTE, img->pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    //glBindTexture(GL_TEXTURE_2D, 0); //cleanup = no textures, black treangles

    return texture_id;
}

//==========================================================================
void engine::render_textured_rectangle(const rectangle &r, GLint texture_number)
{
    glUniform1i(location, 0 + texture_number);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), &r.v[0]);
    glEnableVertexAttribArray(0);

    // texture coordinates
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), &r.v[0].tx);
    glEnableVertexAttribArray(1);
    glValidateProgram(program);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

//==========================================================================
void engine::trans_matrix(float fdeltaX, float fdeltaY, rectangle &r)
{
    r.v[0].x = (1 * r.v[0].x + 0 * r.v[0].y + fdeltaX * 1);
    r.v[0].y = (0 * r.v[0].x + 1 * r.v[0].y + fdeltaY * 1);

    r.v[1].x = (1 * r.v[1].x + 0 * r.v[1].y + fdeltaX * 1);
    r.v[1].y = (0 * r.v[1].x + 1 * r.v[1].y + fdeltaY * 1);

    r.v[2].x = (1 * r.v[2].x + 0 * r.v[2].y + fdeltaX * 1);
    r.v[2].y = (0 * r.v[2].x + 1 * r.v[2].y + fdeltaY * 1);

    r.v[3].x = (1 * r.v[3].x + 0 * r.v[3].y + fdeltaX * 1);
    r.v[3].y = (0 * r.v[3].x + 1 * r.v[3].y + fdeltaY * 1);

    r.v[4].x = (1 * r.v[4].x + 0 * r.v[4].y + fdeltaX * 1);
    r.v[4].y = (0 * r.v[4].x + 1 * r.v[4].y + fdeltaY * 1);
}

//==========================================================================
void engine::rotate_matrix(float frotate_angle, rectangle &r)
{
    float x0 = r.v[0].x;
    float y0 = r.v[0].y;

    float x1 = r.v[1].x;
    float y1 = r.v[1].y;

    float x2 = r.v[2].x;
    float y2 = r.v[2].y;

    float x3 = r.v[3].x;
    float y3 = r.v[3].y;

    float x4 = r.v[4].x;
    float y4 = r.v[4].y;

    r.v[0].x = (cos(frotate_angle * pi / 180) * x0 -
                sin(frotate_angle * pi / 180) * y0 + 0 * 1);
    r.v[0].y = (sin(frotate_angle * pi / 180) * x0 +
                cos(frotate_angle * pi / 180) * y0 + 0 * 1);

    r.v[1].x = (cos(frotate_angle * pi / 180) * x1 -
                sin(frotate_angle * pi / 180) * y1 + 0 * 1);
    r.v[1].y = (sin(frotate_angle * pi / 180) * x1 +
                cos(frotate_angle * pi / 180) * y1 + 0 * 1);

    r.v[2].x = (cos(frotate_angle * pi / 180) * x2 -
                sin(frotate_angle * pi / 180) * y2 + 0 * 1);
    r.v[2].y = (sin(frotate_angle * pi / 180) * x2 +
                cos(frotate_angle * pi / 180) * y2 + 0 * 1);

    r.v[3].x = (cos(frotate_angle * pi / 180) * x3 -
                sin(frotate_angle * pi / 180) * y3 + 0 * 1);
    r.v[3].y = (sin(frotate_angle * pi / 180) * x3 +
                cos(frotate_angle * pi / 180) * y3 + 0 * 1);

    r.v[4].x = (cos(frotate_angle * pi / 180) * x4 -
                sin(frotate_angle * pi / 180) * y4 + 0 * 1);
    r.v[4].y = (sin(frotate_angle * pi / 180) * x4 +
                cos(frotate_angle * pi / 180) * y4 + 0 * 1);
}

//==========================================================================
void engine::scale_triangle(float fscale_valueX, float fscale_valueY,
                            triangle &r)
{
    float x0 = r.v[0].x;
    float y0 = r.v[0].y;

    float x1 = r.v[1].x;
    float y1 = r.v[1].y;

    float x2 = r.v[2].x;
    float y2 = r.v[2].y;

    r.v[0].x = (fscale_valueX * x0 + 0 * y0 + 0 * 1);
    r.v[0].y = (0 * x0 + fscale_valueY * y0 + 0 * 1);

    r.v[1].x = (fscale_valueX * x1 + 0 * y1 + 0 * 1);
    r.v[1].y = (0 * x1 + fscale_valueY * y1 + 0 * 1);

    r.v[2].x = (fscale_valueX * x2 + 0 * y2 + 0 * 1);
    r.v[2].y = (0 * x2 + fscale_valueY * y2 + 0 * 1);
}

//==========================================================================
void engine::scale_rectangle(float fscale_valueX, float fscale_valueY,
                             rectangle &r)
{
    float x0 = r.v[0].x;
    float y0 = r.v[0].y;

    float x1 = r.v[1].x;
    float y1 = r.v[1].y;

    float x2 = r.v[2].x;
    float y2 = r.v[2].y;

    float x3 = r.v[3].x;
    float y3 = r.v[3].y;

    float x4 = r.v[4].x;
    float y4 = r.v[4].y;

    r.v[0].x = (fscale_valueX * x0 + 0 * y0 + 0 * 1);
    r.v[0].y = (0 * x0 + fscale_valueY * y0 + 0 * 1);

    r.v[1].x = (fscale_valueX * x1 + 0 * y1 + 0 * 1);
    r.v[1].y = (0 * x1 + fscale_valueY * y1 + 0 * 1);

    r.v[2].x = (fscale_valueX * x2 + 0 * y2 + 0 * 1);
    r.v[2].y = (0 * x2 + fscale_valueY * y2 + 0 * 1);

    r.v[3].x = (fscale_valueX * x3 + 0 * y3 + 0 * 1);
    r.v[3].y = (0 * x3 + fscale_valueY * y3 + 0 * 1);

    r.v[4].x = (fscale_valueX * x4 + 0 * y4 + 0 * 1);
    r.v[4].y = (0 * x4 + fscale_valueY * y4 + 0 * 1);
}

//==========================================================================
point engine::from_screen_to_opengl(const float &x_input, const float &y_input)
{
    point tmp;

    tmp.x = (x_input - width / 2) / (width / 2);
    tmp.y = (height / 2 - y_input) / (height / 2);

    return tmp;
}

//==========================================================================
point engine::from_screen_to_opengl2(const float &x_input, const float &y_input)
{
    point tmp;
    float k = width / height;

    tmp.x = (x_input - width / 2) / (width / 2) * k;
    tmp.y = (height / 2 - y_input) / (height / 2);

    return tmp;
}

//==========================================================================
void engine::pause_ms(int ms)
{
    SDL_Delay(ms);
}

//====================== PERSON ==============================================

person::person() {}

//==========================================================================
person::person(float center_x, float center_y, float width_x, float height_y)
{
    this->person_geometry =
        engine::create_rectangle(center_x, center_y, width_x, height_y);
}

//==========================================================================
person::person(float center_x, float center_y, float width_x, float height_y,
               std::string invert)
{
    this->person_geometry =
        engine::create_rectangle(center_x, center_y, width_x, height_y, invert);
}

//==========================================================================
person::~person() {}

//==========================================================================
void person::set_geometry(float center_x, float center_y, float width_x,
                          float height_y)
{
    this->person_direction.x = center_x;
    this->person_direction.y = center_y + height_y / 2;

    this->person_geometry =
        engine::create_rectangle(center_x, center_y, width_x, height_y);
}

//==========================================================================
void person::copy_geometry(person &input)
{
    this->person_geometry = input.get_geometry();
}

//==========================================================================
void person::set_noscale_geometry(float center_x, float center_y, float width_x,
                                  float height_y)
{
    this->person_direction.x = center_x;
    this->person_direction.y = center_y + height_y / 2;

    this->person_geometry =
        engine::create_noscale_rectangle(center_x, center_y, width_x, height_y);
}

//==========================================================================
void person::set_geometry(float center_x, float center_y, float width_x,
                          float height_y, std::string invert)
{
    this->person_geometry =
        engine::create_rectangle(center_x, center_y, width_x, height_y, invert);
}

//==========================================================================
void person::set_x_speed(float sp_x)
{
    this->speed_x = sp_x;
}

//==========================================================================
void person::set_y_speed(float sp_y)
{
    this->speed_y = sp_y;
}

//==========================================================================
void person::move()
{
    trans_matrix(speed_x, speed_y, person_geometry);
}

//==========================================================================
void person::move(float speed)
{
    point tmp;
    tmp.x = person_geometry.v[4].x - 0.0f;
    tmp.y = person_geometry.v[4].y - 0.0f;
    normalize_vector(tmp);
    trans_matrix(tmp.x * speed, tmp.y * speed, person_geometry);
}

//==========================================================================
void person::render()
{
    render_rectangle(person_geometry);
}

//==========================================================================
void person::render_with_texture()
{
    render_textured_rectangle(person_geometry, person_texture_id);
}

//==========================================================================
void person::render_with_texture(int number)
{
    render_textured_rectangle(person_geometry, number);
}

//==========================================================================
rectangle &person::get_geometry()
{
    return this->person_geometry;
}

//==========================================================================
void person::set_texture(std::string filename)
{
    this->person_texture_id = load_image(filename);
}

//==========================================================================
void person::set_texture(GLuint texture_id)
{
    this->person_texture_id = texture_id;
}

//==========================================================================
GLuint person::get_texture()
{
    return this->person_texture_id;
}

//==========================================================================
void person::set_texture_to_sprite(std::string filename)
{
    person_sprite.push_back(load_image(filename));
}

//==========================================================================
int person::get_from_sprite(int num)
{
    int number;
    number = person_sprite.at(num);
    return number;
}

//==========================================================================
std::vector<GLuint> &person::get_sprite()
{
    return person_sprite;
}

//==========================================================================
float person::get_x_center()
{
    float x_tmp;
    if (person_geometry.v[0].x < 0 && person_geometry.v[2].x > 0)
    {
        x_tmp = person_geometry.v[0].x +
                ((person_geometry.v[2].x - person_geometry.v[0].x) / 2);
    }
    else
    {
        x_tmp =
            person_geometry.v[0].x +
            abs((abs(person_geometry.v[2].x) - abs(person_geometry.v[0].x)) /
                2);
    }
    return x_tmp;
}

//==========================================================================
float person::get_y_center()
{
    float y_tmp;
    if (person_geometry.v[0].y > 0 && person_geometry.v[1].y < 0)
    {
        y_tmp = person_geometry.v[0].y +
                ((person_geometry.v[1].y - person_geometry.v[0].y) / 2);
    }
    else
    {

        y_tmp =
            person_geometry.v[0].y -
            abs((abs(person_geometry.v[1].y) - abs(person_geometry.v[0].y)) /
                2);
    }

    return y_tmp;
}

//==========================================================================
bool person::mouse_botton_pressed()
{
    return mouse_click;
}

//==========================================================================
void person::normalize_vector(point &v)
{
    point tmp;
    tmp.x = v.x / sqrt(v.x * v.x + v.y * v.y);
    tmp.y = v.y / sqrt(v.x * v.x + v.y * v.y);
    v.x = tmp.x;
    v.y = tmp.y;
}

//==========================================================================
float person::mouse_click_angle()
{
    float result_angle = 0.f;
    point tmp = from_screen_to_opengl2(mouse_x_pos, mouse_y_pos);
    // angel between x axis and mouse click vector

    float alfa = (acos((abs(tmp.x)) / sqrt(tmp.x * tmp.x + tmp.y * tmp.y))) *
                 (180.0 / M_PI);

    if (tmp.x > 0 && tmp.y > 0)
    {
        result_angle = 90.f - alfa;
    }
    else if (tmp.x > 0 && tmp.y < 0)
    {
        result_angle = 90.f + alfa;
    }
    else if (tmp.x < 0 && tmp.y < 0)
    {
        result_angle = 270.f - alfa;
    }
    else if (tmp.x < 0 && tmp.y > 0)
    {
        result_angle = 270.f + alfa;
    }

    return result_angle;
}

//==========================================================================
float person::current_angle()
{
    float result_angle = 0.f;

    // angel between x axis and rectangle direction vector
    float k_r = width / height; // scale to screen compensation for angle!!
    float alfa =
        (acos((abs(person_geometry.v[4].x * k_r)) /
              sqrt(person_geometry.v[4].x * k_r * person_geometry.v[4].x * k_r +
                   person_geometry.v[4].y * person_geometry.v[4].y))) *
        (180.0 / M_PI);

    if (person_geometry.v[4].x > 0 && person_geometry.v[4].y > 0)
    {
        result_angle = 90.f - alfa;
    }
    else if (person_geometry.v[4].x > 0 && person_geometry.v[4].y < 0)
    {
        result_angle = 90.f + alfa;
    }
    else if (person_geometry.v[4].x < 0 && person_geometry.v[4].y < 0)
    {
        result_angle = 270.f - alfa;
    }
    else if (person_geometry.v[4].x < 0 && person_geometry.v[4].y > 0)
    {
        result_angle = 270.f + alfa;
    }

    return result_angle;
}

//==========================================================================
float person::return_angle(const point input_point)
{
    float result_angle = 0.f;

    // angel between x axis and input_point
    float alfa =
        (acos((abs(input_point.x)) / sqrt(input_point.x * input_point.x +
                                          input_point.y * input_point.y))) *
        (180.0 / M_PI);

    if (input_point.x > 0 && input_point.y > 0)
    {
        result_angle = 90.f - alfa;
    }
    else if (input_point.x > 0 && input_point.y < 0)
    {
        result_angle = 90.f + alfa;
    }
    else if (input_point.x < 0 && input_point.y < 0)
    {
        result_angle = 270.f - alfa;
    }
    else if (input_point.x < 0 && input_point.y > 0)
    {
        result_angle = 270.f + alfa;
    }

    return result_angle;
}

//==========================================================================
float person::rotate_direction(const point input)
{
    float result = 0.0f;
    point tmp = from_screen_to_opengl(mouse_x_pos, mouse_y_pos);

    bool old_vector_in_one = false;
    bool old_vector_in_two = false;
    bool old_vector_in_three = false;
    bool old_vector_in_four = false;

    bool new_vector_in_one = false;
    bool new_vector_in_two = false;
    bool new_vector_in_three = false;
    bool new_vector_in_four = false;

    //===== old vector sector =====
    if (input.y > 0 && input.x > 0)
    {
        old_vector_in_one = true;
    }
    else if (input.y < 0 && input.x > 0)
    {
        old_vector_in_two = true;
    }
    else if (input.y < 0 && input.x < 0)
    {
        old_vector_in_three = true;
    }
    else if (input.y > 0 && input.x < 0)
    {
        old_vector_in_four = true;
    }
    //===== new vector sector =====
    if (tmp.y > 0 && tmp.x > 0)
    {
        new_vector_in_one = true;
    }
    else if (tmp.y < 0 && tmp.x > 0)
    {
        new_vector_in_two = true;
    }
    else if (tmp.y < 0 && tmp.x < 0)
    {
        new_vector_in_three = true;
    }
    else if (tmp.y > 0 && tmp.x < 0)
    {
        new_vector_in_four = true;
    }

    float new_alfa =
        (acos((abs(tmp.x)) / sqrt(tmp.x * tmp.x + tmp.y * tmp.y))) *
        (180.0 / M_PI);

    // angel between x axis and last_direction vector
    float old_alfa =
        (acos((abs(input.x)) / sqrt(input.x * input.x + input.y * input.y))) *
        (180.0 / M_PI);

    if (input.x == 0.f)
    {
        if (new_vector_in_one || new_vector_in_two)
        {
            result = -1.0f;
        }
        else
        {
            result = 1.0f;
        }
        return result;
    }
    // all vectors in the sector ONE (I)
    else if (old_vector_in_one && new_vector_in_one)
    {
        if (new_alfa > old_alfa)
        {
            result = 1.0f;
        }
        else
        {
            result = -1.0f;
        }
    }
    // all vectors in the sector TWO (II)
    else if (old_vector_in_two && new_vector_in_two)
    {
        if (new_alfa > old_alfa)
        {
            result = -1.0f;
        }
        else
        {
            result = 1.0f;
        }
    }
    // all vectors in the sector THREE (III)
    else if (old_vector_in_three && new_vector_in_three)
    {
        if (new_alfa > old_alfa)
        {
            result = 1.0f;
        }
        else
        {
            // std::cout << "3 result = -1.0f\n";
            result = -1.0f;
        }
    }
    // all vectors in the sector FOUR (IV)
    else if (old_vector_in_four && new_vector_in_four)
    {
        if (new_alfa > old_alfa)
        {
            result = -1.0f;
        }
        else
        {
            result = 1.0f;
        }
    }
    // I and II
    else if (old_vector_in_one && new_vector_in_two)
    {
        result = -1.0f;
    }
    else if (new_vector_in_one && old_vector_in_two)
    {
        result = 1.0f;
    }
    // I and IV
    else if (old_vector_in_one && new_vector_in_four)
    {
        result = 1.0f;
    }
    else if (new_vector_in_one && old_vector_in_four)
    {
        result = -1.0f;
    }
    // II and III
    else if (old_vector_in_two && new_vector_in_three)
    {
        result = -1.0f;
    }
    else if (new_vector_in_two && old_vector_in_three)
    {
        result = 1.0f;
    }
    // III and IV
    else if (old_vector_in_three && new_vector_in_four)
    {
        result = -1.0f;
    }
    else if (new_vector_in_three && old_vector_in_four)
    {
        result = 1.0f;
    }
    // I and III
    else if (old_vector_in_one && new_vector_in_three)
    {
        if (new_alfa >= old_alfa)
        {
            result = -1.0f;
        }
        else
        {
            result = 1.0f;
        }
    }
    else if (old_vector_in_three && new_vector_in_one)
    {
        if (new_alfa >= old_alfa)
        {
            result = -1.0f;
        }
        else
        {
            result = 1.0f;
        }
    }
    // II and IV
    else if (old_vector_in_two && new_vector_in_four)
    {
        if (new_alfa > old_alfa)
        {
            result = 1.0f;
        }
        else
        {
            result = -1.0f;
        }
    }
    else if (new_vector_in_two && old_vector_in_four)
    {
        if (new_alfa > old_alfa)
        {
            result = 1.0f;
        }
        else
        {
            result = -1.0f;
        }
    }
    return result;
}

//==========================================================================
point person::get_direction_vector()
{
    point result;
    float k = height / width;
    result.x = person_geometry.v[4].x * k - 0.0f;
    result.y = person_geometry.v[4].y - 0.0f;

    return result;
}

//==========================================================================
void person::set_health(float value)
{
    health = value;
}

//==========================================================================
void person::make_damage(float value)
{
    health -= value;
}

//==========================================================================
float person::get_health()
{
    return health;
}

//==========================================================================
void person::set_render_marker(bool value)
{
    render_this = value;
}

//==========================================================================
bool person::get_render_marker()
{
    return render_this;
}

//====================== ANIMATION ==========================================

animation::animation(){};
animation::~animation(){};

//==========================================================================
void animation::set_fps(float fps_value)
{
    fps = fps_value;
}

//==========================================================================
void animation::set_number_of_frames(float f)
{
    number_of_frames = f;
}

//==========================================================================
void animation::restart()
{
    current_time = 0.f;
}

//==========================================================================
int animation::current_frame_number(float delta_time)
{
    current_time += delta_time;

    float one_frame_delta = 1.f / fps;

    size_t how_may_frames_from_start =
        static_cast<size_t>(current_time / one_frame_delta);

    size_t current_frame_index = how_may_frames_from_start % number_of_frames;

    return current_frame_index;
}

//==========================================================================
void animation::set_last_frame(int lf)
{
    last_frame = lf;
}

//==========================================================================
int animation::get_last_frame()
{
    return last_frame;
}

//====================== TIMER ================================================
timer::timer()
    : m_beg(clock_t::now())
{
}

//==========================================================================
void timer::reset()
{
    m_beg = clock_t::now();
}

//==========================================================================
double timer::elapsed() const
{
    return std::chrono::duration_cast<second_t>(clock_t::now() - m_beg).count();
}

//====================== CHECK THE COLLISION FUNCTION =========================

int collision_check(person &a, person &b)
{
    float a_0_x = a.get_geometry().v[0].x;
    float a_0_y = a.get_geometry().v[0].y;

    float a_1_x = a.get_geometry().v[1].x;
    float a_1_y = a.get_geometry().v[1].y;

    float a_2_x = a.get_geometry().v[2].x;
    float a_2_y = a.get_geometry().v[2].y;

    float a_3_x = a.get_geometry().v[3].x;
    float a_3_y = a.get_geometry().v[3].y;

    float b_0_x = b.get_geometry().v[0].x;
    float b_0_y = b.get_geometry().v[0].y;

    float b_1_x = b.get_geometry().v[1].x;
    float b_1_y = b.get_geometry().v[1].y;

    float b_2_x = b.get_geometry().v[2].x;
    float b_2_y = b.get_geometry().v[2].y;

    float b_3_x = b.get_geometry().v[3].x;
    float b_3_y = b.get_geometry().v[3].y;

    int collision_type;

    //  collision_type == 1
    //
    //     0 ____2
    //     \  a  \
    //     \_____\
    //     1     3
    //  0 ____2
    //  \  b  \
    //  \_____\
    //  1     3

    if ((a_1_y < b_0_y) && (a_1_y > b_1_y) && (a_1_x > b_0_x) &&
        (a_1_x < b_2_x))
    {
        return collision_type = 1;
    }

    //  collision_type == 1
    //
    //  0 ____2
    //  \  a  \
    //  \_____\
    //  1     3
    //     0 ____2
    //     \  b  \
    //     \_____\
    //     1     3

    if ((a_1_y < b_0_y) && (a_1_y > b_1_y) && (a_3_x > b_0_x) &&
        (a_3_x < b_2_x))
    {
        return collision_type = 1;
    }

    //  collision_type == 2
    //
    //     0 ____2
    //     \  b  \
    //     \_____\
    //     1     3
    //  0 ____2
    //  \  a  \
    //  \_____\
    //  1     3

    if ((a_0_y > b_1_y) && (a_0_y < b_0_y) && (a_2_x > b_1_x) &&
        (a_2_x < b_3_x))
    {
        return collision_type = 2;
    }

    //  collision_type == 2
    //
    //  0 ____2
    //  \  b  \
    //  \_____\
    //  1     3
    //     0 ____2
    //     \  a  \
    //     \_____\
    //     1     3

    if ((a_0_y > b_1_y) && (a_0_y < b_0_y) && (a_0_x > b_1_x) &&
        (a_0_x < b_3_x))
    {
        return collision_type = 2;
    }

    return collision_type = 0;
}
