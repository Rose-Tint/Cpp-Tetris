#pragma once
#ifndef ANIMATIONS_HPP
#define ANIMATIONS_HPP

#include <chrono>
#include <thread>
#include <array>
#include <random>
#include <cmath>

#include "Frame.hpp"
#include "graphics.hpp"
#include "argparse.hpp"
#include "math/Vector.hpp"


template < std::size_t H, std::size_t W >
void rotating_torus(Frame<H, W>& frame)
{
    using std::cos, std::sin;

    static constexpr const char out_chars[] = ".,-~:;=!*#$@";
    static constexpr const float theta_spacing = 0.07;
    static constexpr const float phi_spacing = 0.02;
    static constexpr const float R1 = 1;
    static constexpr const float R2 = 2;
    static constexpr const float K2 = 5;
    static constexpr const float K1 = W * K2 * 3 / (8 * (R1 + R2));
    static const float pi = std::atan(1) * 4;

    float a = 0, b = 0;
    float z_recip = 0;
    float cos_a = cos(a), sin_a = sin(a);
    float cos_b = cos(b), sin_b = sin(b);
    float cos_theta = 0,  sin_theta = 0;
    float cos_phi = 0, sin_phi = 0;
    float lumin = 0;
    int lumin_idx = 0;
    char shade = 0;
    std::array<std::array<float, W>, H> z_buffer;
    Vector circle;
    Vector vect;
    Vector proj;

    for (auto& row : z_buffer)
        row.fill(0);

    while (1)
    {
        for (float theta = 0; theta < 2 * pi; theta += theta_spacing)
        {
            cos_theta = cos(theta);
            sin_theta = sin(theta);

            for (float phi = 0; phi < 2 * pi; phi += phi_spacing)
            {
                cos_phi = cos(phi);
                sin_phi = sin(phi);

                circle.x() = R1 + R2 * cos_theta;
                circle.y() = R1 * sin_theta;

                vect.x() =
                    circle.x()
                  * (cos_b * cos_phi + sin_a * sin_b * sin_phi)
                  - circle.y() * cos_a * sin_b;
                vect.y() =
                    circle.x()
                  * (sin_b * cos_phi - sin_a * cos_b * sin_phi)
                  - circle.y() * cos_a * sin_b;
                vect.z() = K2 + cos_a * circle.x() * sin_phi + circle.y() * sin_a;

                proj.x() = (W / 2) + ((K1 * vect.x()) / vect.z());
                proj.y() = (H / 2) + ((K1 * vect.y()) / vect.z());
                proj.z() = vect.z();

                lumin = cos_phi * cos_theta * cos_b
                      - cos_a * cos_theta * sin_phi
                      - sin_a * sin_theta + cos_b
                      * (cos_a * sin_theta - cos_theta * sin_a * sin_phi);

                z_recip = 1 / proj.z();

                if (lumin > 0)
                {
                    if (z_buffer.at(proj.y()).at(proj.x()) < z_recip)
                    {
                        z_buffer[proj.y()][proj.x()] = z_recip;
                        lumin_idx = int(std::floor(lumin)) * 8;
                        shade = out_chars[lumin_idx];
                        frame.set(shade, proj.x(), proj.y());
                    }
                }
            }
        }
        a += 0.04;
        b += 0.02;
    }
};


// finished for now.
template < std::size_t H, std::size_t W >
void dvd_ss(Frame<H, W>& frame, const ArgInfo&)
{
    using UnifDistrib = std::uniform_real_distribution<float>;
    using std::chrono_literals::operator ""ms;

    static constexpr std::size_t dvdHeight = 3;
    static constexpr std::size_t dvdWidth = 5;

    std::random_device rd;
    std::mt19937 gen(rd());

    UnifDistrib rand_vel(0.5, 1.5);
    Vector dir(rand_vel(gen) * 2, rand_vel(gen));

    UnifDistrib rand_x_pos(1, W - dvdHeight - std::abs(dir.x()));
    UnifDistrib rand_y_pos(1, H - dvdWidth - std::abs(dir.y()));

    Rectangle dvd(dvdHeight, dvdWidth, rand_x_pos(gen), rand_y_pos(gen));
    const Frame<dvdHeight, dvdWidth> dvd_frame({
        { '+','-','-','-','+' },
        { '|','D','V','D','|' },
        { '+','-','-','-','+' },
    });

    while (1)
    {
        if (dvd.x() <= 0 || dvd.x() + dvd.width() >= W - 1)
            dir.reverse_x();

        if (dvd.y() <= 0 || dvd.y() + dvd.height() >= H - 1)
            dir.reverse_y();

        frame.clear(dvd.x() + 1, dvd.y() + 1, dvd.right() + 1, dvd.bottom() + 1);
        move_rect(dir, dvd);
        frame.draw(std::ceil(dvd.x()), std::ceil(dvd.y()), dvd_frame);

        std::this_thread::sleep_for(175ms);
    }
}

#endif
