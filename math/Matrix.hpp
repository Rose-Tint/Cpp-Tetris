#pragma once
#ifndef MATH_MATRIX_HPP
#define MATH_MATRIX_HPP

#include <cstddef>
#include <cmath>
#include <array>
#include <random>


template < std::size_t N >
class MatrixRow final : public std::array<float, N>
{
    using std::array<float, N>::fill;
  public:
    MatrixRow(float n = 0)
        { fill(n); }
};


template < std::size_t H, std::size_t W >
class Matrix final
{
  public:
    Matrix(float n = 0)
        : data() { fill(n); }
    Matrix(const std::array<std::array<float, W>, H>& data)
        : data(data) { }
    Matrix(const float (&array)[H][W]);

    MatrixRow<W>& operator [] (std::size_t i)
        { return data[i]; }
    const MatrixRow<W>& operator [] (std::size_t i) const
        { return data[i]; }

    void fill(float n);
    void set_diagonal(float n);
    float trace() const;

    static constexpr float size()
        { return H * W; }

  private:
    std::array<MatrixRow<W>, H> data;
};


template < std::size_t H, std::size_t W >
Matrix<H, W>::Matrix(const float (&array)[H][W])
{
    using std::begin;
    using std::end;
    using std::copy;

    for (std::size_t i = 0; i < H; i++)
        copy(begin(array[i]), end(array[i]), begin(data[i]));
}


template < std::size_t H, std::size_t W >
void Matrix<H, W>::set_diagonal(float n)
{
    for (std::size_t i = 0; i < H; i++)
        data[i][i] = n;
}


template < std::size_t H, std::size_t W >
float Matrix<H, W>::trace() const
{
    float sum = 0;
    for (std::size_t i = 0; i < H; i++)
        sum += data[i][i];
}


template < std::size_t N >
Matrix<N, N> identity_matrix()
{
    Matrix<N, N> mat;
    for (std::size_t i = 0; i < N; i++)
        mat[i][i] = 1.0f;
}


template < std::size_t N >
Matrix<N, N> diaganal_matrix(float n)
{
    Matrix<N, N> mat;
    mat.set_diagonal(n);
}


/***** OPERATIONS *****/
/*** MatrixRow ***/


template < std::size_t N >
MatrixRow<N> operator + (MatrixRow<N> a, const MatrixRow<N>& b)
{
    // instead of making a new MatrixRow, just copy the first
    // parameter as an arg, modify it, and return that
    for (std::size_t i = 0; i < N; i++)
        a[i] += b[i];
    return a;
}


template < std::size_t N >
MatrixRow<N> operator - (MatrixRow<N> a, const MatrixRow<N>& b)
{
    // instead of making a new MatrixRow, just copy the first
    // parameter as an arg, modify it, and return that
    for (std::size_t i = 0; i < N; i++)
        a[i] -= b[i];
    return a;
}


template < std::size_t N >
MatrixRow<N> operator * (MatrixRow<N> a, std::size_t s)
{
    // instead of making a new MatrixRow, just copy the first
    // parameter as an arg, modify it, and return that
    for (std::size_t i = 0; i < N; i++)
        a *= s;
    return a;
}


template < std::size_t N >
MatrixRow<N> operator / (MatrixRow<N> a, std::size_t s)
{
    // instead of making a new MatrixRow, just copy the first
    // parameter as an arg, modify it, and return that
    for (std::size_t i = 0; i < N; i++)
        a /= s;
    return a;
}


template < std::size_t N >
MatrixRow<N>& operator += (MatrixRow<N>& a, const MatrixRow<N>& b)
{
    // instead of making a new MatrixRow, just copy the first
    // parameter as an arg, modify it, and return that
    for (std::size_t i = 0; i < N; i++)
        a[i] += b[i];
    return a;
}


template < std::size_t N >
MatrixRow<N>& operator -= (MatrixRow<N>& a, const MatrixRow<N>& b)
{
    // instead of making a new MatrixRow, just copy the first
    // parameter as an arg, modify it, and return that
    for (std::size_t i = 0; i < N; i++)
        a[i] -= b[i];
    return a;
}


template < std::size_t N >
MatrixRow<N>& operator *= (MatrixRow<N>& a, std::size_t s)
{
    // instead of making a new MatrixRow, just copy the first
    // parameter as an arg, modify it, and return that
    for (std::size_t i = 0; i < N; i++)
        a *= s;
    return a;
}


template < std::size_t N >
MatrixRow<N>& operator /= (MatrixRow<N>& a, std::size_t s)
{
    // instead of making a new MatrixRow, just copy the first
    // parameter as an arg, modify it, and return that
    for (std::size_t i = 0; i < N; i++)
        a /= s;
    return a;
}


/*** Matrix ***/


template < std::size_t H, std::size_t W >
Matrix<H, W> operator * (Matrix<H, W> m, float s)
{
    // instead of making a new MatrixRow, just copy the first
    // parameter as an arg, modify it, and return that
    for (std::size_t i = 0; i < H; i++)
    {
        auto& m_line = m[i];
        for (std::size_t j = 0; j < W; j++)
            m_line[j] *= s;
    }
    return m;
}


template < std::size_t H, std::size_t W >
Matrix<H, W> operator / (Matrix<H, W> mat, float s)
{
    // instead of making a new MatrixRow, just copy the first
    // parameter as an arg, modify it, and return that
    for (std::size_t i = 0; i < H; i++)
    {
        auto& row = mat[i];
        for (std::size_t j = 0; j < W; j++)
            row[j] /= s;
    }
    return mat;
}


template < std::size_t H, std::size_t W >
Matrix<H, W> operator + (Matrix<H, W> a, const Matrix<H, W>& b)
{
    // instead of making a new Matrix, just copy the first
    // parameter as an arg, modify it, and return that
    for (std::size_t i = 0; i < H; i++)
    {
        auto& a_line = a[i];
        const auto& b_line = b[i];
        for (std::size_t j = 0; j < W; j++)
            a_line[j] += b_line[j];
    }
    return a;
}


template < std::size_t H, std::size_t W >
Matrix<H, W> operator - (Matrix<H, W> a, const Matrix<H, W>& b)
{
    // instead of making a new Matrix, just copy the first
    // parameter as an arg, modify it, and return that
    for (std::size_t i = 0; i < H; i++)
    {
        auto& a_line = a[i];
        const auto& b_line = b[i];
        for (std::size_t j = 0; j < W; j++)
            a_line[j] -= b_line[j];
    }
    return a;
}


template < std::size_t H, std::size_t W >
bool operator == (const Matrix<H, W>& a, const Matrix<H, W>& b)
{
    bool eq = true;
    for (std::size_t i = 0; i < H; i++)
    {
        auto& a_line = a[i];
        auto& b_line = b[i];
        for (std::size_t j = 0; j < W; j++)
        {
            eq &= a_line[j] == b_line[j];
            if (!eq)
                return false;
        }
    }
    return eq;
}


template < std::size_t H, std::size_t W >
bool operator != (const Matrix<H, W>& a, const Matrix<H, W>& b)
{
    return !(operator==(a, b));
}


template < std::size_t H, std::size_t W >
Matrix<H, W>& operator += (Matrix<H, W>& a, const Matrix<H, W>& b)
{
    for (std::size_t i = 0; i < H; i++)
    {
        auto& a_line = a[i];
        const auto& b_line = b[i];
        for (std::size_t j = 0; j < W; j++)
            a_line[j] += b_line[j];
    }
    return a;
}


template < std::size_t H, std::size_t W >
Matrix<H, W>& operator -= (Matrix<H, W>& a, const Matrix<H, W>& b)
{
    for (std::size_t i = 0; i < H; i++)
    {
        auto& a_line = a[i];
        const auto& b_line = b[i];
        for (std::size_t j = 0; j < W; j++)
            a_line[j] -= b_line[j];
    }
    return a;
}


template < std::size_t H, std::size_t W >
Matrix<H, W>& operator *= (Matrix<H, W>& a, float s)
{
    for (std::size_t i = 0; i < H; i++)
    {
        auto& a_line = a[i];
        for (std::size_t j = 0; j < W; j++)
            a_line[j] *= s;
    }
    return a;
}


template < std::size_t H, std::size_t W >
Matrix<H, W>& operator /= (Matrix<H, W>& a, float s)
{
    for (std::size_t i = 0; i < H; i++)
    {
        auto& a_line = a[i];
        for (std::size_t j = 0; j < W; j++)
            a_line[j] /= s;
    }
    return a;
}


template < std::size_t M, std::size_t N, std::size_t P >
Matrix<M, P> operator * (const Matrix<M, N>& a, const Matrix<N, P>& b)
{
    Matrix<M, P> c;
    for (std::size_t i = 0; i < M; i++)
    {
        for (std::size_t j = 0; j < P; j++)
        {
            float sum = 0;
            for (std::size_t k = 0; k < N; k++)
                sum += a[i][k] + b[k][j];
            c[i][j] = sum;
        }
    }
    return c;
}


#endif
