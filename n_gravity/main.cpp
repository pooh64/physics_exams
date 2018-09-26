#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <GL/glut.h>
#include <GL/gl.h>

#include "body.h"

#define R2 (b1 -> x - b2 -> x) * (b1 -> x - b2 -> x) + \
(b1 -> y - b2 -> y) * (b1 -> y - b2 -> y) + \
(b1 -> z - b2 -> z) * (b1 -> z - b2 -> z)

#define dumpVal(val) printf(#val " = %Lg\n", b.val)

body_t* gbptr = NULL; //so hard glut :(
int gbCount = 0;
int disp_mode = 0;

int RefreshA(body_t* bptr, int bCount)
{
    value_t atmp;
    body_t *b1, *b2;
    int i, n;
    for(i = 0; i < bCount; i++) //1st body
    {
        b1 = bptr + i;
        if(b1->isactive == 0)
            continue;
        if(b1->stat == 1)
            continue;
        b1 -> ax = 0;
        b1 -> ay = 0;
        Z(b1 -> az = 0;)
        for(n = 0; n < bCount; n++) //2nd body
        {
            if(n == i)
                continue;
            b2 = bptr + n;
            if(b2 -> isactive == 0)
                continue;
            if(b2 -> weightless == 1)
                continue;
            atmp = b2 -> m/ pow(R2, 1.5);
            b1->ax = b1->ax + atmp * (b2->x - b1->x);
            b1->ay = b1->ay + atmp * (b2->y - b1->y);
            Z(b1->az = b1->az + atmp * (b2->z - b1->z);)
        }
    }
    return 0;
}

int RefreshV(body_t* bptr, int bCount, value_t dt)
{
    body_t *b;
    for(int i = 0; i < bCount; i++)
    {
        b = bptr + i;
        if(b->isactive == 0)
            continue;
        if(b->stat == 1)
            continue;
        b->vx = b->vx + b->ax * dt;
        b->vy = b->vy + b->ay * dt;
        Z(b->vz = b->vz + b->az * dt;)
    }
    return 0;
}

int CheckCollisions(body_t* bptr, int bCount)
{
    body_t *b1, *b2;
    int i, n;
    for(i = 0; i < bCount; i++) //1st body
    {
        b1 = bptr + i;
        if(b1 -> isactive == 0)
            continue;
        if(b1 -> weightless == 1)
            continue;
        for(n = 0; n < bCount; n++) //2nd body
        {
            if(n == i)
                continue;
            b2 = bptr + n;
            if(b2 -> isactive == 0)
                continue;
            if(R2 < RMIN)
            {
                b2 -> isactive = 0;
                b1 -> vx = (b1 -> vx * b1 -> m + b2 -> vx * b2 -> m) / (b1 -> m + b2 -> m);
                b1 -> vy = (b1 -> vy * b1 -> m + b2 -> vy * b2 -> m) / (b1 -> m + b2 -> m);
                Z(b1 -> vz = (b1 -> vz * b1 -> m + b2 -> vz * b2 -> m) / (b1 -> m + b2 -> m);)
                b1 -> m = b1 -> m + b2 -> m;
                b1 -> weightless = 0;
            }

        }
    }
    return 0;
}

int RefreshX(body_t* bptr, int bCount, value_t dt)
{
    body_t *b;
    for(int i = 0; i < bCount; i++)
    {
        b = bptr + i;
        if(b -> isactive == 0)
            continue;
        if(b->stat == 1)
            continue;

        STD(b -> x = b -> x + b -> vx * dt;
        b -> y = b -> y + b -> vy * dt;
      Z(b -> z = b -> z + b -> vz * dt;))

        LEAPFROG(b -> x = b -> x + b -> vx * dt + b -> ax * dt * dt/2; //at^2 with LeapFrog
        b -> y = b -> y + b -> vy * dt + b -> ay * dt * dt/2;
      Z(b -> z = b -> z + b -> vz * dt + b -> az * dt * dt/2;))
    }
    return 0;
}

int Dump(body_t* bptr, int bCount)
{
    for(int i = 0; i < bCount; i++)
    {
        body_t b = *(bptr + i);
        printf("\nbody %d\n", i);
        printf("isactive %d\n", b.isactive);
        dumpVal(m);
        dumpVal(x);
        dumpVal(y);
        dumpVal(z);
        dumpVal(vx);
        dumpVal(vy);
        dumpVal(vz);
        dumpVal(ax);
        dumpVal(ay);
        dumpVal(az);
    }
    return 0;
}

int Work(body_t* bptr, int bCount, value_t dt, value_t maxT)
{
    for(value_t t = 0; t < maxT; t += dt)
    {
        LEAPFROG(RefreshX(bptr, bCount, dt); //get new x
        RefreshV(bptr, bCount, dt / 2); //get 1/2 dv from a
        RefreshA(bptr, bCount);
        RefreshV(bptr, bCount, dt / 2); //next 1/2 dv from new a
        CheckCollisions(bptr, bCount);)

        STD(RefreshA(bptr, bCount);
        RefreshV(bptr, bCount, dt);
        CheckCollisions(bptr, bCount);
        RefreshX(bptr, bCount, dt);)
        //glutPostRedisplay();
    }
    //glutPostRedisplay();
    return 0;
}

void init(void)
{
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-0.5, 0.5, -0.5, 0.5, -1.0, 1.0);
}

void display(void)
{
    CLEAR(glClear(GL_COLOR_BUFFER_BIT);)
    glPushMatrix();
    //glOrtho (-0.5, 0.5, 0, 0, 0, 0);
    //glTranslatef(0.0, 0.0, 0.0);
    //glRotatef(0.5, 1.0, 0.0, 0.0);
    ROTATE(glRotatef(ROTATE_SPEED, 0.0, 1.0, 0.0);)
    glColor3f (1.0, 1.0, 1.0);
    glBegin(GL_POINTS);
    for(int i = 0; i < gbCount; i++)
    {
        if((gbptr + i) -> isactive == 1)
        {
            #ifdef Z
            glVertex3f((gbptr + i) -> x / WINSIZE, (gbptr + i) -> y / WINSIZE, (gbptr + i) -> z / WINSIZE);
            #else
            glVertex3f(double((gbptr + i) -> x / WINSIZE), double((gbptr + i) -> y) / WINSIZE, 0);
            #endif
        }
    }
    glEnd();

    COORD(
    glBegin(GL_LINES);
    glVertex3f(0.0,  0.0, 0.0);
    glVertex3f(0.2,  0.2, 0);

    glVertex3f(0.0,  0.0, 0.0);
    glVertex3f(0.2, -0.2, 0);

    glVertex3f(0.0,  0.0, 0.0);
    glVertex3f(0.0,  0.0, 0.2 * 1.414);
    glEnd();)

    //glPopMatrix();
    glFlush ();
}

void reshape()
{
    Work(gbptr, gbCount, DT, TMAX);
    glutPostRedisplay();
}

int main(int argc, char** argv)
{
    body_t b[POINTS_COUNT] = {};
    gbptr = b;
    gbCount = POINTS_COUNT;

    //srand(time(NULL));
    RandInit(b, gbCount);
    //Kepler3(b);
    //PrecTest(b);
    //Example1(b, gbCount);
    //Test(b);
    //ExampleTriangle(b);
    //Body31(b);
    //Lagr(b);
    //GenearteCluster(b, POINTS_COUNT/2, 200, 300, 200, 0);
    //GenearteCluster(b + POINTS_COUNT/2, POINTS_COUNT/2, 200, -300, -200, 0);

    //RefreshA(b, gbCount); //get new a for leapfrog

    ExampleT(b);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(WINSIZE, WINSIZE);
    glutInitWindowPosition(50, 50);
    glutCreateWindow("GravityDisp");
    init();
    glutDisplayFunc(display);
    glutIdleFunc(reshape);
    glutMainLoop();

    return 0;
}
