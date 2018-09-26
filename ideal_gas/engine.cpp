#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include "engine.h"


void shell_t::init_bodies(size_t n_body_, double vmax)
{
    if(body_arr != nullptr)
        delete[] body_arr;
    n_body = n_body_;
    body_arr = new body_t[n_body_] {};

    srand(time(NULL));
    for(size_t i = 0; i < n_body; i++) {
        body_arr[i].x = (double) rand() \
        / RAND_MAX * (field.xmax - field.xmin) + field.xmin;
        body_arr[i].y = (double) rand() \
        / RAND_MAX * (field.ymax - field.ymin) + field.ymin;
        body_arr[i].z = (double) rand() \
        / RAND_MAX * (field.zmax - field.zmin) + field.zmin;

        body_arr[i].vx = ((double) rand() / RAND_MAX - 0.5) * 2 * vmax;
        body_arr[i].vy = ((double) rand() / RAND_MAX - 0.5) * 2 * vmax;
        body_arr[i].vz = ((double) rand() / RAND_MAX - 0.5) * 2 * vmax;
    }
}

/////////////////// Physics /////////////////////

void shell_t::process_step(double dt)
{
    timer += dt;
    process_coord(dt);
    process_collisions();
}


void shell_t::process_coord(double dt)
{
    for(size_t i = 0; i < n_body; i++) {
        body_arr[i].x += (body_arr[i].vx * dt);
        body_arr[i].y += (body_arr[i].vy * dt);
        body_arr[i].z += (body_arr[i].vz * dt);
    }

    field.xmin += (field.vxmin * dt);
}


void shell_t::process_collisions()
{
    /// solution is:
    /// a = (n, v1 - v2)/(n,n)
    /// v1' = v1 + a * n
    /// v2' = v2 - a * n

    double nx, ny, nz, sclr_nn;
    double min_sclr_nn = 4 * body_r * body_r;
    double alpha;

    for(size_t i = 0; i < n_body; i++) {

        /// Collisions with walls
        if(body_arr[i].x < field.xmin) {
            body_arr[i].x += (field.xmin - body_arr[i].x);
            sumary_hit_impulse -= body_arr[i].vx;
            body_arr[i].vx = -body_arr[i].vx + 2 * field.vxmin;
        }
        if(body_arr[i].x > field.xmax) {
            body_arr[i].x += (field.xmax - body_arr[i].x);
            sumary_hit_impulse += body_arr[i].vx;
            body_arr[i].vx *= (-1);
        }
        if(body_arr[i].y < field.ymin) {
            body_arr[i].y += (field.ymin - body_arr[i].y);
            sumary_hit_impulse -= body_arr[i].vy;
            body_arr[i].vy *= (-1);
        }
        if(body_arr[i].y > field.ymax) {
            body_arr[i].y += (field.ymax - body_arr[i].y);
            sumary_hit_impulse += body_arr[i].vy;
            body_arr[i].vy *= (-1);
        }
        if(body_arr[i].z < field.zmin) {
            body_arr[i].z += (field.zmin - body_arr[i].z);
            sumary_hit_impulse -= body_arr[i].vz;
            body_arr[i].vz *= (-1);
        }
        if(body_arr[i].z > field.zmax) {
            body_arr[i].z += (field.zmax - body_arr[i].z);
            sumary_hit_impulse += body_arr[i].vz;
            body_arr[i].vz *= (-1);
        }

        /// Particles collisions
        for(size_t n = i + 1; n < n_body; n++) {
            /// if particles are closer than 2r(dont use sqrt)...
            /// this part is slow
            nx = body_arr[i].x - body_arr[n].x;
            ny = body_arr[i].y - body_arr[n].y;
            nz = body_arr[i].z - body_arr[n].z;
            sclr_nn = nx * nx + ny * ny + nz * nz;

            if(sclr_nn < min_sclr_nn) {
                alpha = (nx * (body_arr[n].vx - body_arr[i].vx) \
                       + ny * (body_arr[n].vy - body_arr[i].vy) \
                       + nz * (body_arr[n].vz - body_arr[i].vz)) / sclr_nn;

                body_arr[i].vx += (alpha * nx);
                body_arr[n].vx -= (alpha * nx);

                body_arr[i].vy += (alpha * ny);
                body_arr[n].vy -= (alpha * ny);

                body_arr[i].vz += (alpha * nz);
                body_arr[n].vz -= (alpha * nz);

                alpha = sqrt(min_sclr_nn / sclr_nn) - 1;

                body_arr[i].x += (nx * alpha);
                body_arr[i].y += (nx * alpha);
                body_arr[i].z += (nx * alpha);

                body_arr[n].x -= (nx * alpha);
                body_arr[n].y -= (nx * alpha);
                body_arr[n].z -= (nx * alpha);
            }
        }

    }
}


/////////////////// Measures ////////////////////

double shell_t::meas_capacity()
{
    return (field.xmax - field.xmin + 2 * body_r)   \
         * (field.ymax - field.ymin + 2 * body_r)   \
         * (field.zmax - field.zmin + 2 * body_r);
}


double shell_t::meas_square()
{
    return 2 * ((field.xmax - field.xmin + 2 * body_r)  \
             * ((field.ymax - field.ymin + 2 * body_r)  \
             + (field.zmax - field.zmin + 2 * body_r))  \
             + (field.ymax - field.ymin + 2 * body_r)   \
             * (field.zmax - field.zmin + 2 * body_r));
}

#define BODY(arg) body_arr[i].arg

double shell_t::meas_rmsv()
{
    double rmsv = 0;
    for(size_t i = 0; i < n_body; i++)
        rmsv += (BODY(vx) * BODY(vx) + BODY(vy) * BODY(vy) + BODY(vz) * BODY(vz));
    return sqrt(rmsv / n_body);
}

double shell_t::meas_ev()
{
    double ev = 0;
    for(size_t i = 0; i < n_body; i++)
        ev += sqrt(BODY(vx) * BODY(vx) + BODY(vy) * BODY(vy) + BODY(vz) * BODY(vz));
    return ev / n_body;
}


void shell_t::begin_meas_pressure()
{
    timer_pr_start = timer;
    sumary_hit_impulse = 0;
}


double shell_t::meas_pressure()
{
    if(timer == timer_pr_start)
        return 0;

    double pressure = 2 * sumary_hit_impulse    \
    / ((timer - timer_pr_start) * meas_square());;
    timer_pr_start = 0;
    sumary_hit_impulse = 0;
    return pressure;
}


size_t* shell_t::get_v_distr(size_t *arr, double v_max, int n_seg)
{
    double v = 0;
    double step = v_max / n_seg;

    for(size_t i = 0; i < n_body; i++) {
        v = body_arr[i].vx * body_arr[i].vx \
          + body_arr[i].vy * body_arr[i].vy \
          + body_arr[i].vz * body_arr[i].vz;
        v = sqrt(v);
        if((int) (v / step) < n_seg)
            arr[(int) (v / step)]++;
    }

    return arr;
}

