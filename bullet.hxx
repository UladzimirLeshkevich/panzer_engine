#pragma once
#include "engine.hxx"

class bullet : public person
{
public:
    bullet();
    void  set_damage(float value);
    float get_damage();
    void  set_x_speed(float value);
    float get_x_speed();
    void  set_y_speed(float value);
    float get_y_speed();
    void  set_in_clip();
    void  get_out_of_clip();
    bool  bullet_in_clip();

private:
    float damage  = 1.0f;
    float speed_x = 0.0f;
    float speed_y = 0.0f;
    bool  in_clip = true;
};
