#pragma once
#ifndef MATH_VECTOR_HPP
#define MATH_VECTOR_HPP

#include <cmath>

#include "../math/Matrix.hpp"


class Vector final
{
    friend Vector operator + (const Vector& a, const Vector& b);
    friend Vector operator - (const Vector& a, const Vector& b);
    friend bool operator == (const Vector& a, const Vector& b);
    friend bool operator != (const Vector& a, const Vector& b);
    friend Vector& operator += (Vector& a, const Vector& b);
    friend Vector& operator -= (Vector& a, const Vector& b);
    friend Vector& operator *= (Vector& a, float s);
    friend Vector& operator /= (Vector& a, float s);

  public:
    Vector(float x = 0, float y = 0, float z = 0)
        : _x(x), _y(y), _z(z) { }
    Vector(const MatrixRow<1>& v);
    Vector(const MatrixRow<2>& v);
    Vector(const MatrixRow<3>& v);
    Vector(const Matrix<1, 1>& m);
    Vector(const Matrix<1, 2>& m);
    Vector(const Matrix<1, 3>& m);
    Vector(const Matrix<2, 1>& m);
    Vector(const Matrix<3, 1>& m);

    operator MatrixRow<1>();
    operator MatrixRow<2>();
    operator MatrixRow<3>();
    operator Matrix<1, 1>();
    operator Matrix<1, 2>();
    operator Matrix<1, 3>();
    operator Matrix<2, 1>();
    operator Matrix<3, 1>();

    Vector operator - () const; // opposite
    Vector operator * (float s) const;
    Vector operator / (float s) const;

    void reverse_x() { _x = -_x; }
    void reverse_y() { _y = -_y; }
    void reverse_z() { _z = -_z; }
    void opposite() { _x = -_x; _y = -_y; _z = -_z; }

    float& x() { return _x; }
    float& y() { return _y; }
    float& z() { return _z; }
    float x() const { return _x; }
    float y() const { return _y; }
    float z() const { return _z; }

    void normalize();
    void invert();
    bool is_opposite_of(const Vector& other) const;

    float magnitude() const;
    float length() const;
    float size() const;

  private:
    float _x;
    float _y;
    float _z;
};


const Vector unitVector =
    ([]() -> Vector
        { Vector v; v.normalize(); return v; })();
const Vector zeroVector(0, 0);

#endif
