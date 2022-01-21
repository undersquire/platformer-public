#include "Math.h"

#include <algorithm>
#include <cmath>
#include <iostream>

bool Rectangle::Intersects(const Vector2<float> &point) const noexcept
{
    return (point.x >= this->pos.x && point.x <= this->pos.x + this->size.x) &&
           (point.y >= this->pos.y && point.y <= this->pos.y + this->size.y);
}

bool Rectangle::Intersects(const Rectangle &other) const noexcept
{
    return ((this->pos.x < other.pos.x + other.size.x) && (this->pos.x + this->size.x > other.pos.x) &&
            (this->pos.y < other.pos.y + other.size.y) && (this->pos.y + this->size.y > other.pos.y));
}

bool Rectangle::Intersects(const Vector2<float> &origin, const Vector2<float> &direction, Vector2<float> &contact_point,
                           Vector2<float> &contact_normal, float &hit_near) const noexcept
{
    contact_normal = {0, 0};
    contact_point = {0, 0};

    Vector2<float> invdir = 1.0f / direction;

    Vector2<float> near = (this->pos - origin) * invdir;
    Vector2<float> far = (this->pos + this->size - origin) * invdir;

    if (std::isnan(far.y) || std::isnan(far.x))
        return false;
    if (std::isnan(near.y) || std::isnan(near.x))
        return false;

    if (near.x > far.x)
        std::swap(near.x, far.x);
    if (near.y > far.y)
        std::swap(near.y, far.y);

    if (near.x > far.y || near.y > far.x)
        return false;

    hit_near = std::max(near.x, near.y);

    float hit_far = std::min(far.x, far.y);

    if (hit_far < 0)
        return false;

    contact_point = origin + hit_near * direction;

    if (near.x > near.y)
    {
        if (invdir.x < 0)
            contact_normal = {1, 0};
        else
            contact_normal = {-1, 0};
    }
    else if (near.x < near.y)
    {
        if (invdir.y < 0)
            contact_normal = {0, 1};
        else
            contact_normal = {0, -1};
    }
    else
    {
        if (invdir.y < 0)
            contact_normal = {0, 1};
        else
            contact_normal = {0, -1};
    }

    return true;
}

bool Rectangle::Intersects(const Rectangle &other, const Vector2<float> &velocity, const float deltaTime,
                           Vector2<float> &contact_point, Vector2<float> &contact_normal,
                           float &contact_time) const noexcept
{
    if (velocity.x == 0 && velocity.y == 0)
        return false;

    Rectangle expanded;

    expanded.pos = this->pos - other.size / 2;
    expanded.size = this->size + other.size;

    if (expanded.Intersects(other.pos + other.size / 2, velocity * deltaTime, contact_point, contact_normal,
                            contact_time))
        return (contact_time >= -0.01f && contact_time < 1.f);
    else
        return false;
}

bool Rectangle::Intersects(const Vector2<float> &dyn_velocity, const Rectangle &other, const Vector2<float> &velocity,
                           const float deltaTime, Vector2<float> &contact_point, Vector2<float> &contact_normal,
                           float &contact_time) const noexcept
{
    Rectangle expanded;

    expanded.pos = this->pos - other.size / 2;
    expanded.size = this->size + other.size;

    if (expanded.Intersects(other.pos + other.size / 2, (velocity - dyn_velocity) * deltaTime, contact_point,
                            contact_normal, contact_time))
        return (contact_time >= -0.01f && contact_time < 1.f);
    else
        return false;
}