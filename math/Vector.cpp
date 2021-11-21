#include "../math/Vector.hpp"

#include <string>
#include <stdexcept>


inline float inverse_sqrt(float n)
{
    long i;
    float x2, y;

    x2 = n / 2;
    y  = n;
    // cast to long pointer to allow bitshift
    i  = *((long*)&y);          // evil flop hack
    i  = 0x5f3759df - (i >> 1); // what the fuck?
    y  = *((float*)&i);
    y  *= 1.5f - (x2 * y * y);

    return y;
}


Vector operator + (const Vector& a, const Vector& b)
    { return { a._x + b._x, a._y + b._y, a._z + b._z }; }

Vector operator - (const Vector& a, const Vector& b)
    { return { a._x - b._x, a._y - b._y, a._z - b._z }; }

bool operator == (const Vector& a, const Vector& b)
    { return a._x == b._x && a._y == b._y && a._z == b._z; }

bool operator != (const Vector& a, const Vector& b)
    { return a._x != b._x || a._y != b._y || a._z != b._z; }

Vector& operator += (Vector& a, const Vector& b)
{
    a._x += b._x;
    a._y += b._y;
    a._z += b._z;
    return a;
}

Vector& operator -= (Vector& a, const Vector& b)
{
    a._x -= b._x;
    a._y -= b._y;
    a._z -= b._z;
    return a;
}

Vector& operator *= (Vector& a, std::float_t s)
{
    a._x *= s;
    a._y *= s;
    a._z *= s;
    return a;
}

Vector& operator /= (Vector& a, std::float_t s)
{
    a._x /= s;
    a._y /= s;
    a._z /= s;
    return a;
}


Vector Vector::operator - () const
    { return { -_x, -_y, -_z }; }


Vector Vector::operator * (std::float_t s) const
    { return { _x * s, _y * s, _z * s }; }


Vector Vector::operator / (std::float_t s) const
    { return { _x / s, _y / s, _z / s }; }

void Vector::normalize()
{
    const float scale = inverse_sqrt((_x*_x) + (_y*_y) + (_z*_z));
    _x *= scale;
    _y *= scale;
    _z *= scale;
}


void Vector::invert()
{
    _x = -_x;
    _y = -_y;
    _z = -_z;
}


bool Vector::is_opposite_of(const Vector& other) const
{
    return _x == -(other._x) && _y == -(other._y) && _z == -(other._z);
}


std::float_t Vector::magnitude() const
{
    return std::sqrt((_x*_x) + (_y*_y) + (_z*_z));
}


std::float_t Vector::length() const
{
    return std::sqrt((_x*_x) + (_y*_y) + (_z*_z));
}


std::float_t Vector::size() const
{
    return std::sqrt((_x*_x) + (_y*_y) + (_z*_z));
}

