#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "body.h"

int RandInit(body_t* b, int bCount)
{
    value_t v = 0.15;
    for(int i = 0; i < bCount; i++)
    {
        b[i].isactive = 1;
        b[i].x = value_t(rand()) / RAND_MAX * WINSIZE - WINSIZE/2;
        b[i].y = value_t(rand()) / RAND_MAX * WINSIZE - WINSIZE/2;
        Z(b[i].z = value_t(rand()) / RAND_MAX * WINSIZE - WINSIZE/2;)
        b[i].vx = (value_t(rand()) / RAND_MAX - 0.5) * v;
        b[i].vy = (value_t(rand()) / RAND_MAX - 0.5) * v;
        Z(b[i].vz = (value_t(rand()) / RAND_MAX - 0.5) * v;)
        //b[i].m = rand()%2 - 0.5; //dark mass WOAH
        b[i].m = 1;
    }
    return 0;
}

int Example1(body_t* b, int bCount)
{
    b[0].isactive = 1;
    b[1].isactive = 1;
    b[2].isactive = 1;
    b[3].isactive = 1;

    value_t v = 0.015;
    value_t m = 1;

    b[0].x = 50 - 500;
    b[0].y = 500 - 500;
    b[0].vy = v;
    b[0].m = m;

    b[1].x = 500 - 500;
    b[1].y = 950 - 500;
    b[1].vx = v;
    b[1].m = m;

    b[2].x = 950 - 500;
    b[2].y = 500 - 500;
    b[2].vy = -v;
    b[2].m = m;

    b[3].x = 500 - 500;
    b[3].y = 50 - 500;
    b[3].vx = -v;
    b[3].m = m;

    return 0;
}

int ExampleTriangle(body_t* b)
{
    for(int n = 0; n < 3; n++)
    {
        b[n].isactive = 1;
        //b[n].x = WINSIZE * sqrt(double(rand()) / RAND_MAX) - WINSIZE/2;
        //b[n].y = WINSIZE * sqrt(double(rand()) / RAND_MAX) - WINSIZE/2;
        b[n].m = 1;
    }

    b[1].x = 200;
    b[1].y = -300;
    b[2].x = -300;
    b[2].y = -300;

    b[0].x = 274;
    b[0].y = 317;
    return 0;
}

int ExampleT(body_t* b)
{
    for(int n = 0; n < 2; n++)
    {
        b[n].isactive = 1;
        //b[n].x = WINSIZE * sqrt(double(rand()) / RAND_MAX) - WINSIZE/2;
        //b[n].y = WINSIZE * sqrt(double(rand()) / RAND_MAX) - WINSIZE/2;
        b[n].m = 1;
    }

    b[1].m = 2;

    b[1].x = 0;
    b[1].y = 300;
    b[1].vx = -0.01;
    b[1].vy = 0.01;

    b[0].vx = 0.001;
    b[0].x = 0;
    b[0].y = -300;
    return 0;
}



int Test(body_t* b)
{
    b[0].isactive = 1;
    b[1].x = 300;
    b[1].y = 0;
    return 0;
}

int Body31(body_t *b)
{
    b[0].isactive = 1;
    b[1].isactive = 1;
    b[2].isactive = 1;

    b[0].m = 100;
    b[0].stat = 0;
    b[1].m = 3;
    b[2].weightless = 1;

    b[1].x = 400;
    b[1].vy = 0.5;

    b[2].x = 370;
    b[2].vy = 0.85;

    b[0].vy = - b[1].vy * b[1].m / b[0].m;
    return 0;
}

int Lagr(body_t *b)
{
    b[0].isactive = 1;
    b[1].isactive = 1;
    b[2].isactive = 1;

    b[0].m = 100;
    b[0].stat = 1;
    b[2].weightless = 1;

    b[1].x = 400;
    b[1].vy = 0.5;

    b[2].x = 380;
    b[2].vy = b[1].vy * b[2].x / b[1].x;

    b[1].m = b[0].m * (1/pow(b[2].x, 2)- b[2].x/pow(b[1].x, 3)) * pow(b[1].x - b[2].x, 2) * 1;

    b[0].vy = - b[1].vy * b[1].m / b[0].m;
    return 0;
}

int Kepler3(body_t *b)
{
    for(int i = 1; i < POINTS_COUNT; i++)
    {
        b[(int) i].isactive = 1;
        b[(int) i].weightless = 1;
        b[(int) i].y = -450;
        b[(int) i].z = 0;
        b[(int) i].vx = sqrt(2*(0.01 - 1/pow(pow(2,i - 1)*1000, (double)2/3) / 2)); //(200)^(3/2)
        //b[i].vx = 0.12;
        b[(int) i].m = 1;
    }
    b[0].isactive = 1;
    b[0].stat = 1;

    b[0].y = -350;
    b[0].m = 1;
    return 0;
}

int PrecTest(body_t* b)
{
    b[0].isactive = 1;
    b[1].isactive = 1;
    b[0].m = 1000;
    b[1].m = 1;
    b[0].vx = -0.0003;
    b[1].vx = 0.3;
    b[0].x = 0;
    b[0].y = -300;
    b[1].x = 0;
    b[1].y = 400;
    return 0;
}

int GenearteCluster(body_t* b, int bCount, float Rmax, float x, float y, float z)
{
    value_t v = 0.7;
    for(int i = 0; i < bCount; i++)
    {
        b[i].isactive = 1;
        //float R = double(rand()) / RAND_MAX * Rmax;
        float R = Rmax;
        float a1 = double(rand()) / RAND_MAX;
        float a2 = double(rand()) / RAND_MAX;
        //printf("%g\n", R);
        b[i].x = cos(2 * M_PI * a1) * cos(2 * M_PI * a2) * R + x;
        printf("%Lg\n", b[i].x);
        b[i].y = cos(2 * M_PI * a1) * sin(2 * M_PI * a2) * R + y;
        Z(b[i].z = sin(2 * M_PI * a1) * R + z;)
        b[i].vx = (double(rand()) / RAND_MAX - 0.5) * v;
        b[i].vy = (double(rand()) / RAND_MAX - 0.5) * v;
        Z(b[i].vz = (double(rand()) / RAND_MAX - 0.5) * v;)
        b[i].m = rand()%2 - 0.5; //dar
        b[i].m = 1;
    }
    return 0;
}
