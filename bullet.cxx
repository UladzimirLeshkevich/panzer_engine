#include "bullet.hxx"
#include <iostream>

bullet::bullet() {}

void bullet::set_damage(float value)
{
    damage = value;
}

float bullet::get_damage()
{
    return damage;
}

void bullet::set_x_speed(float value)
{
    speed_x = value;
}

float bullet::get_x_speed()
{
    return speed_x;
}

void bullet::set_y_speed(float value)
{
    speed_y = value;
}

float bullet::get_y_speed()
{
    return speed_y;
}

void bullet::set_in_clip()
{
    in_clip = true;
}

void bullet::get_out_of_clip()
{
    in_clip = false;
}

bool bullet::bullet_in_clip()
{
    return in_clip;
}
