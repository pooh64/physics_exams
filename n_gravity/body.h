#ifndef BODY_H
#define BODY_H
typedef long double value_t;

#define WINSIZE 1000
#define RMIN 8
#define ROTATE_SPEED 0.15
#define DT 0.1
#define TMAX 3
#define POINTS_COUNT 100

//#define ROTATE_EN
//#define COORD_EN
#define Z_EN
//#define CLEAR_EN
#define LEAPFROG_EN

#ifdef LEAPFROG_EN
#define LEAPFROG(code) code
#define STD(code)
#else
#define LEAPFROG(code)
#define STD(code) code
#endif

#ifdef COORD_EN
#define COORD(code) code
#else
#define COORD(code)
#endif

#ifdef ROTATE_EN
#define ROTATE(code) code
#else
#define ROTATE(code)
#endif

#ifdef Z_EN
#define Z(code) code
#else
#define Z(code)
#endif

#ifdef CLEAR_EN
#define CLEAR(code) code
#else
#define CLEAR(code)
#endif

struct body_t
{
    value_t x, y, z, vx, vy, vz, m, ax, ay, az;
    short isactive;
    short stat;
    short weightless;
};

int RandInit(body_t* bptr, int bCount);
int Example1(body_t* b, int bCount);
int Test(body_t* b);
int ExampleTriangle(body_t* b);
int PrecTest(body_t* b);
int GenearteCluster(body_t* b, int bCount, float Rmax, float x, float y, float z);
int Kepler3(body_t* b);
int Body31(body_t* b);
int Lagr(body_t *b);
int ExampleT(body_t* b);

#endif
