#include "bullet.hxx"
#include "engine.hxx"
#include <iostream>
#include <memory>
#include <vector>

int main(int /*argc*/, char * /*argv*/[])
{
    //--------- start engine -------------------
    engine game_engine;

    game_engine.set_window_height(720);
    game_engine.set_window_width(1080);

    // game_engine.set_window_height(1080);
    // game_engine.set_window_width(1920);

    game_engine.set_resources_path("res/");
    game_engine.initialize(WINDOW_MODE);
    //-------------------------------------------
    timer bullets_in;
    float bullets_in_timer_value;

    std::vector<std::unique_ptr<bullet>> bullets_pool;
    for (int i = 0; i < 4000; ++i)
    {
        //bullets_pool.push_back(std::unique_ptr<bullet>(new bullet));
        //bullets_pool.emplace_back(std::unique_ptr<bullet>(new bullet));//??
        bullets_pool.emplace_back(new bullet); //faster and more safe
        //Instead of taking a value_type it takes a variadic list of arguments,
        //so that means that you can now perfectly forward the arguments
        //and construct directly an object into a container
        //without a temporary at all.
    }

    //------------------------------------------------
    bullet bullet_gage, b2;
    bullet_gage.set_geometry(0.0f, 0.24f, 0.05f, 0.1f);
    point bullet_direction;
    float bullet_speed_value = 0.15f;
    bool bullet_is_fly = false;
    float bullet_delta_time = 0.0f;
    timer bullet_timer;

    //----------------------------------------------------------------
    bool rotate_flag = false;
    float angle_to_zero = 0.0f;

    //----------------------------------------------------------------
    person base;
    base.set_geometry(0.0f, 0.0f, 0.4f, 0.492857f);
    base.set_texture("panzer_base.png");
    float panzer_base_angle_to_zero = 0.0f;

    //----------------------------------------------------------------
    person topleft, topright, backleft, backright;
    topleft.set_noscale_geometry(-0.18f, 0.18f, 0.133333f, 0.25685f);
    topleft.scale_to_screen(topleft.get_geometry());
    topleft.set_texture("up_left.png");
    topleft.set_texture_to_sprite("up_left.png");
    topleft.set_texture_to_sprite("back_left.png");

    topright.set_noscale_geometry(0.18f, 0.18f, 0.133333f, 0.25685f);
    topright.scale_to_screen(topright.get_geometry());
    topright.set_texture("up_left.png");
    topright.set_texture_to_sprite("up_left.png");
    topright.set_texture_to_sprite("back_left.png");

    backleft.set_noscale_geometry(-0.18f, -0.13f, 0.133333f, 0.25685f);
    backleft.scale_to_screen(backleft.get_geometry());
    backleft.set_texture("up_left.png");
    backleft.set_texture_to_sprite("up_left.png");
    backleft.set_texture_to_sprite("back_left.png");

    backright.set_noscale_geometry(0.18f, -0.13f, 0.133333f, 0.25685f);
    backright.scale_to_screen(backright.get_geometry());
    backright.set_texture("up_left.png");
    backright.set_texture_to_sprite("up_left.png");
    backright.set_texture_to_sprite("back_left.png");

    //----------------------------------------------------------------
    person tower;
    tower.set_geometry(0.0f, 0.05f, 0.2f, 0.246428f);
    tower.set_texture("panzer_tower.png");
    float tower_angle_to_zero = 0.0f;
    rectangle tower_geometry;
    point tower_direction;
    float mouse_click_angle = 0.0f;
    float rotation_direction = 0.0f;

    //----------------------------------------------------------------
    rectangle back;
    back = game_engine.create_noscale_rectangle(0.0f, 0.0f, 2.0f, 2.0f);
    // auto back_t = game_engine.load_image("22.png");

    //----------------------------------------------------------------
    person someobject(0.5f, 0.7f, 0.5f, 0.5f);
    someobject.set_texture("robot_1t.png");
    person someobject2(-0.5f, -0.7f, 0.5f, 0.5f);
    someobject2.set_texture("robot_2.png");
    person someobject3(-1.5f, -1.7f, 0.5f, 0.5f);
    someobject3.set_texture("robot_1.png");
    person someobject4(0.5f, 1.7f, 0.5f, 0.5f);
    someobject4.set_texture("robot_2.png");
    person someobject5(1.0f, 0.7f, 0.5f, 0.5f);
    someobject5.set_texture("robot_2.png");
    person someobject6(1.0f, -0.7f, 0.5f, 0.5f);
    someobject6.set_texture("robot_1.png");
    person someobject7(1.5f, -0.7f, 0.5f, 0.5f);
    someobject7.set_texture("robot_2.png");

    std::vector<person *> world_objects;
    world_objects.push_back(&someobject);
    world_objects.push_back(&someobject2);
    world_objects.push_back(&someobject3);
    world_objects.push_back(&someobject4);
    world_objects.push_back(&someobject5);
    world_objects.push_back(&someobject6);
    world_objects.push_back(&someobject7);
    //----------------------------------------------------------------

    bool move_the_world = false;
    point world_dir;
    float x_speed = 0.0f;
    float y_speed = 0.0f;

    // animation
    animation run;
    run.set_fps(8);
    run.set_number_of_frames(2);
    int run_number = 0;
    timer ani_timer;
    float delta_time = 0.f;

    // animation backleft
    animation backleft_run;
    backleft_run.set_fps(8);
    backleft_run.set_number_of_frames(2);
    int backleft_run_number = 0;

    // animation topright
    animation topright_run;
    topright_run.set_fps(8);
    topright_run.set_number_of_frames(2);
    int topright_run_number = 0;

    // animation topright
    animation backright_run;
    backright_run.set_fps(8);
    backright_run.set_number_of_frames(2);
    int backright_run_number = 0;

    //-------- sound ------------
    auto engine_sound = game_engine.load_music("engine.mp3");
    game_engine.play_music(engine_sound, 10);
    auto tower_sound = game_engine.load_sound("tower.mp3");
    auto move_sound = game_engine.load_sound("move2.mp3");
    auto turn_sound = game_engine.load_sound("turn.mp3");
    auto gun = game_engine.load_sound("gun_fire.mp3");
    bool laser_is_playing = false;
    bool play_gun_sound = false;

    bool sound_is_playing = false;
    bool sound_is_playing2 = false;

    bool base_is_rotating = false;

    //----- ALL SPEEDS ----------
    float speed_delta_time = 0.0f;
    timer speed_timer;
    float move_speed_value = 0.02f;

    float base_delta_time = 0.0f;
    timer base_timer;
    float tower_delta_time = 0.0f;
    timer tower_timer;

    auto bullet_texture = game_engine.load_image("bullet3.png");
    //---------------------------------------------
    //only one texture in the memory
    for (auto &it : bullets_pool)
    {
        it->set_texture(bullet_texture);
    }

    //create many textures in the memory
    //    for (auto &it : bullets_pool)
    //    {
    //        it->set_texture("bullet3.png");
    //    }

    // =============== GAME LOOP ===================
    bool loop = true;
    while (loop)
    {
        loop = game_engine.events();

        //========== RENDER ============
        // game_engine.render_textured_rectangle(back, back_t);
        topleft.render_with_texture();
        topright.render_with_texture();
        backleft.render_with_texture();
        backright.render_with_texture();
        //===============================

        //============ TIMERS ====================
        delta_time = ani_timer.elapsed();
        speed_delta_time = speed_timer.elapsed();
        base_delta_time = base_timer.elapsed();
        tower_delta_time = tower_timer.elapsed();
        bullet_delta_time = bullet_timer.elapsed();
        bullets_in_timer_value = bullets_in.elapsed();
        //=========================================

        //============ MOVE THE WORLD begin ==============
        if (game_engine.key_W_pressed())
        {
            move_the_world = true;

            rectangle tmp = base.get_geometry();
            world_dir.x = tmp.v[4].x - 0.0f;
            world_dir.y = tmp.v[4].y - 0.0f;
            base.normalize_vector(world_dir);
            x_speed = world_dir.x * (-move_speed_value);
            y_speed = world_dir.y * (-move_speed_value);

            //=== animation ===
            run.restart();
            run_number = run.current_frame_number(delta_time);
            topleft.render_with_texture(topleft.get_from_sprite(run_number));

            backleft_run.restart();
            backleft_run_number = backleft_run.current_frame_number(delta_time);
            backleft.render_with_texture(
                backleft.get_from_sprite(backleft_run_number));

            topright_run.restart();
            topright_run_number = topright_run.current_frame_number(delta_time);
            topright.render_with_texture(
                topright.get_from_sprite(topright_run_number));

            backright_run.restart();
            backright_run_number =
                backright_run.current_frame_number(delta_time);
            backright.render_with_texture(
                backright.get_from_sprite(backright_run_number));
        }
        else if (game_engine.key_S_pressed())
        {
            move_the_world = true;

            rectangle tmp = base.get_geometry();
            world_dir.x = tmp.v[4].x - 0.0f;
            world_dir.y = tmp.v[4].y - 0.0f;
            base.normalize_vector(world_dir);
            x_speed = world_dir.x * (move_speed_value);
            y_speed = world_dir.y * (move_speed_value);

            //=== animation ===
            run.restart();
            run_number = run.current_frame_number(delta_time);
            topleft.render_with_texture(topleft.get_from_sprite(run_number));

            backleft_run.restart();
            backleft_run_number = backleft_run.current_frame_number(delta_time);
            backleft.render_with_texture(
                backleft.get_from_sprite(backleft_run_number));

            topright_run.restart();
            topright_run_number = topright_run.current_frame_number(delta_time);
            topright.render_with_texture(
                topright.get_from_sprite(topright_run_number));

            backright_run.restart();
            backright_run_number =
                backright_run.current_frame_number(delta_time);
            backright.render_with_texture(
                backright.get_from_sprite(backright_run_number));
        }
        else
        {
            move_the_world = false;
            game_engine.stop_sound(3);
            sound_is_playing = false;
        }

        if (move_the_world)
        {
            if (!sound_is_playing)
            {
                game_engine.play_sound(move_sound, 30, 3);
                sound_is_playing = true;
            }

            //===== moving worlds objects =====
            if (speed_delta_time >= 0.04f)
            {
                speed_delta_time = 0.f;
                speed_timer.reset();

                for (auto &it : world_objects)
                {
                    it->trans_matrix(x_speed, y_speed, it->get_geometry());
                }
            }
        }
        //============ MOVE THE WORLD end ==============

        //============ ROTATE THE TOWER begin ==============
        if (tower.mouse_botton_pressed())
        {
            mouse_click_angle = tower.mouse_click_angle();
            rotation_direction = tower.rotate_direction(tower_direction);
            rotate_flag = true;

            game_engine.play_sound(tower_sound, 90, 2);
        }

        if (rotate_flag)
        {
            float increment = 0.95f * rotation_direction;

            if (tower_delta_time >= 0.02f)
            {
                tower_delta_time = 0.f;
                tower_timer.reset();
                if (!(tower.return_angle(tower_direction) > mouse_click_angle &&
                      tower.return_angle(tower_direction) <
                          (mouse_click_angle + abs(increment * 2.0f))))
                {
                    angle_to_zero += increment;
                    tower_angle_to_zero += increment;

                    bullet_gage.set_noscale_geometry(0.0f, 0.24f, 0.05f, 0.1f);
                    bullet_gage.rotate_matrix(tower_angle_to_zero,
                                              bullet_gage.get_geometry());
                    bullet_gage.scale_to_screen(bullet_gage.get_geometry());

                    tower.set_noscale_geometry(0.0f, 0.05f, 0.2f, 0.246428f);
                    tower.rotate_matrix(tower_angle_to_zero,
                                        tower.get_geometry());
                    tower.scale_to_screen(tower.get_geometry());

                    tower_geometry = tower.get_geometry();
                    tower_direction.x = tower_geometry.v[4].x;
                    tower_direction.y = tower_geometry.v[4].y;
                }
                else
                {
                    game_engine.stop_sound(2);
                    rotate_flag = false;
                }
            }
        }
        //============ ROTATE THE TOWER end ==============

        //============ ROTATE THE BASE begin ==============
        if (game_engine.key_D_pressed())
        {
            base_is_rotating = true;

            if (base_delta_time >= 0.03f)
            {
                base_delta_time = 0.f;
                base_timer.reset();

                float increment = -0.8f;
                panzer_base_angle_to_zero += increment;
                tower_angle_to_zero += increment;

                base.set_noscale_geometry(0.0f, 0.0f, 0.4f, 0.492857f);
                base.rotate_matrix(panzer_base_angle_to_zero,
                                   base.get_geometry());
                base.scale_to_screen(base.get_geometry());

                bullet_gage.set_noscale_geometry(0.0f, 0.24f, 0.05f, 0.1f);
                bullet_gage.rotate_matrix(tower_angle_to_zero,
                                          bullet_gage.get_geometry());
                bullet_gage.scale_to_screen(bullet_gage.get_geometry());

                tower.set_noscale_geometry(0.0f, 0.05f, 0.2f, 0.246428f);
                tower.rotate_matrix(tower_angle_to_zero, tower.get_geometry());
                tower.scale_to_screen(tower.get_geometry());
                tower_geometry = tower.get_geometry();
                tower_direction.x = tower_geometry.v[4].x;
                tower_direction.y = tower_geometry.v[4].y;

                topleft.set_noscale_geometry(-0.18f, 0.18f, 0.133333f,
                                             0.25685f);
                topleft.rotate_matrix(panzer_base_angle_to_zero,
                                      topleft.get_geometry());
                topleft.scale_to_screen(topleft.get_geometry());

                topright.set_noscale_geometry(0.18f, 0.18f, 0.133333f,
                                              0.25685f);
                topright.rotate_matrix(panzer_base_angle_to_zero,
                                       topright.get_geometry());
                topright.scale_to_screen(topright.get_geometry());

                backleft.set_noscale_geometry(-0.18f, -0.13f, 0.133333f,
                                              0.25685f);
                backleft.rotate_matrix(panzer_base_angle_to_zero,
                                       backleft.get_geometry());
                backleft.scale_to_screen(backleft.get_geometry());

                backright.set_noscale_geometry(0.18f, -0.13f, 0.133333f,
                                               0.25685f);
                backright.rotate_matrix(panzer_base_angle_to_zero,
                                        backright.get_geometry());
                backright.scale_to_screen(backright.get_geometry());
            }

            //=== animation ===
            run.restart();
            run_number = run.current_frame_number(delta_time);
            topleft.render_with_texture(topleft.get_from_sprite(run_number));

            backleft_run.restart();
            backleft_run_number = backleft_run.current_frame_number(delta_time);
            backleft.render_with_texture(
                backleft.get_from_sprite(backleft_run_number));
        }
        else if (game_engine.key_A_pressed())
        {
            base_is_rotating = true;

            if (base_delta_time >= 0.03f)
            {
                base_delta_time = 0.f;
                base_timer.reset();

                float increment = 0.8f;
                panzer_base_angle_to_zero += increment;
                tower_angle_to_zero += increment;

                base.set_noscale_geometry(0.0f, 0.0f, 0.4f, 0.492857f);
                base.rotate_matrix(panzer_base_angle_to_zero,
                                   base.get_geometry());
                base.scale_to_screen(base.get_geometry());

                bullet_gage.set_noscale_geometry(0.0f, 0.24f, 0.05f, 0.1f);
                bullet_gage.rotate_matrix(tower_angle_to_zero,
                                          bullet_gage.get_geometry());
                bullet_gage.scale_to_screen(bullet_gage.get_geometry());

                tower.set_noscale_geometry(0.0f, 0.05f, 0.2f, 0.246428f);
                tower.rotate_matrix(tower_angle_to_zero, tower.get_geometry());
                tower.scale_to_screen(tower.get_geometry());
                tower_geometry = tower.get_geometry();
                tower_direction.x = tower_geometry.v[4].x;
                tower_direction.y = tower_geometry.v[4].y;

                topleft.set_noscale_geometry(-0.18f, 0.18f, 0.133333f,
                                             0.25685f);
                topleft.rotate_matrix(panzer_base_angle_to_zero,
                                      topleft.get_geometry());
                topleft.scale_to_screen(topleft.get_geometry());

                topright.set_noscale_geometry(0.18f, 0.18f, 0.133333f,
                                              0.25685f);
                topright.rotate_matrix(panzer_base_angle_to_zero,
                                       topright.get_geometry());
                topright.scale_to_screen(topright.get_geometry());

                backleft.set_noscale_geometry(-0.18f, -0.13f, 0.133333f,
                                              0.25685f);
                backleft.rotate_matrix(panzer_base_angle_to_zero,
                                       backleft.get_geometry());
                backleft.scale_to_screen(backleft.get_geometry());

                backright.set_noscale_geometry(0.18f, -0.13f, 0.133333f,
                                               0.25685f);
                backright.rotate_matrix(panzer_base_angle_to_zero,
                                        backright.get_geometry());
                backright.scale_to_screen(backright.get_geometry());
            }

            //=== animation ===
            topright_run.restart();
            topright_run_number = topright_run.current_frame_number(delta_time);
            topright.render_with_texture(
                topright.get_from_sprite(topright_run_number));

            backright_run.restart();
            backright_run_number =
                backright_run.current_frame_number(delta_time);
            backright.render_with_texture(
                backright.get_from_sprite(backright_run_number));
        }
        else
        {
            base_is_rotating = false;
            game_engine.stop_sound(4);
            sound_is_playing2 = false;
        }

        if (base_is_rotating)
        {
            if (!sound_is_playing2)
            {
                game_engine.play_sound(turn_sound, 90, 4);
                sound_is_playing2 = true;
            }
        }
        //============ ROTATE THE BASE end ==============

        //=========== FIRE begin ===============
        if (game_engine.key_SPACE_pressed())
        {
            play_gun_sound = true;
            if (bullets_in_timer_value >= 0.2f)
            {
                bullets_in_timer_value = 0.f;
                bullets_in.reset();
                for (auto &it : bullets_pool)
                {
                    if (it->bullet_in_clip())
                    {
                        it->get_out_of_clip();
                        it->copy_geometry(bullet_gage);
                        it->normalize_vector(bullet_direction);
                        bullet_direction = it->get_direction_vector();

                        it->set_x_speed(bullet_direction.x *
                                        bullet_speed_value);
                        it->set_y_speed(bullet_direction.y *
                                        bullet_speed_value);

                        break;
                    }
                }
            } // time

            bullet_is_fly = true;
        }
        else
        {
            play_gun_sound = false;
            game_engine.stop_sound(5);
            laser_is_playing = false;
        }

        if (bullet_is_fly)
        {
            if (bullet_delta_time >= 0.04f)
            {
                bullet_delta_time = 0.f;
                bullet_timer.reset();

                for (auto &it : bullets_pool)
                {
                    if (!it->bullet_in_clip())
                    {
                        if (move_the_world)
                        {
                            it->trans_matrix(it->get_x_speed() + x_speed,
                                             it->get_y_speed() + y_speed,
                                             it->get_geometry());
                        }
                        else
                        {
                            it->trans_matrix(it->get_x_speed(),
                                             it->get_y_speed(),
                                             it->get_geometry());
                        }
                    }
                }
            }
        }

        if (play_gun_sound)
        {
            if (!laser_is_playing)
            {
                game_engine.play_sound(gun, 20, 5, -1); //-1 is the loop
                laser_is_playing = true;
            }
        }
        //=========== FIRE end ===============

        //============ RENDER ================

        base.render_with_texture();
        tower.render_with_texture();

        for (auto &it : world_objects)
        {
            if (it->get_render_marker())
                it->render_with_texture();
        }

        //=== CHECK COLLISION AND RENDER ==========
        for (auto &it : bullets_pool)
        {
            if (!it->bullet_in_clip())
            {
                if (collision_check(*it, someobject))
                {
                    it->set_in_clip();
                    someobject.make_damage(it->get_damage());

                    if (someobject.get_health() <= 0.0f)
                    {
                        someobject.set_render_marker(false);
                        someobject.set_geometry(0.0f, 0.0f, 0.0f, 0.0f);
                    }
                }
                else
                {
                    it->render_with_texture();
                }
            }
        }
        //========================================

        game_engine.swap_buffers();

    } // GAME LOOP END

    return EXIT_SUCCESS;
}
