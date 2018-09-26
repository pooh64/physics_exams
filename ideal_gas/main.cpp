#include <cstdio>
#include "engine.h"
#include <GL/glut.h>
#include <GL/gl.h>

shell_t *DRAW_SHELL = nullptr;
FILE *DUMP_FILE = nullptr;

void experiment_dump();
void experiment_simple_dump();

//----------------------------------------------
const size_t STABLE_STEPS = 0;
const size_t STEPS_PER_FRAME = 1;
const double DT = 0.002;
const size_t N_PART = 10000;
const double RADIUS = 0.005;
const double V_MAX = 1;

/// Velocity distribution
const int DISTR_SEG = 500;
const double DISTR_MAX = 5;
size_t DISTR[DISTR_SEG] = {};
//----------------------------------------------

void experimet_init(shell_t *shell)
{
    /// Init field
    shell->field.xmin = 0;
    shell->field.ymin = 0;
    shell->field.zmin = 0;

    shell->field.xmax = 1;
    shell->field.ymax = 1;
    shell->field.zmax = 1;

    /// Init partciles
    shell->init_bodies(N_PART, V_MAX);
    shell->body_r = RADIUS;

    for(size_t i = 0; i < STABLE_STEPS; i++)
        shell -> process_step(DT);

    shell->field.vxmin = 0.005;
}

void experiment_frame()
{
    DRAW_SHELL -> begin_meas_pressure();

    for(size_t i = 0; i < STEPS_PER_FRAME; i++) {
        DRAW_SHELL -> process_step(DT);
        /// DRAW_SHELL -> get_v_distr(DISTR, DISTR_MAX, DISTR_SEG);
    }

    experiment_simple_dump();
}

void experiment_simple_dump()
{
    double rmsv = DRAW_SHELL -> meas_rmsv();
    double ev   = DRAW_SHELL -> meas_ev();
    double P    = DRAW_SHELL -> meas_pressure();
    double V    = DRAW_SHELL -> meas_capacity();
    double timer = DRAW_SHELL -> timer;

    printf("t=%E Ev2=%E Ev=%E P=%E V=%E\n", timer, rmsv, ev, P, V);
}

void experiment_dump()
{
    double rmsv = DRAW_SHELL -> meas_rmsv();
    double ev   = DRAW_SHELL -> meas_ev();

    printf("time %lg <v> %lg\n", DRAW_SHELL -> timer, ev);
    fprintf(DUMP_FILE, "%lg;%lg;;", rmsv, DRAW_SHELL -> timer);

    /**for(int i = 0; i < DISTR_SEG; i++){
        fprintf(DUMP_FILE, "%zu;", DISTR[i]);
        DISTR[i] = 0;
    }

    fprintf(DUMP_FILE, "\n");*/

}


/////////////////// GL graphics /////////////////

void init(void)
{
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-0.05, 1.05, -0.05, 1.05, -1.0, 1.0);
}

void reshape(void)
{
    experiment_frame();
    glutPostRedisplay();
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    ///glPushMatrix();
    glColor3f (1.0, 1.0, 1.0);
    glBegin(GL_POINTS);

    for(size_t i = 0; i < DRAW_SHELL -> n_body; i++) {
        glVertex3f(double((DRAW_SHELL -> body_arr[i]).x),   \
                   double((DRAW_SHELL -> body_arr[i]).y), 0);
    }
    glEnd();

    glBegin(GL_LINES);
    glVertex3f(DRAW_SHELL -> field.xmin - DRAW_SHELL -> body_r, \
               DRAW_SHELL -> field.ymin - DRAW_SHELL -> body_r, 0.0);
    glVertex3f(DRAW_SHELL -> field.xmax + DRAW_SHELL -> body_r, \
               DRAW_SHELL -> field.ymin - DRAW_SHELL -> body_r, 0.0);

    glVertex3f(DRAW_SHELL -> field.xmax + DRAW_SHELL -> body_r, \
               DRAW_SHELL -> field.ymin - DRAW_SHELL -> body_r, 0.0);
    glVertex3f(DRAW_SHELL -> field.xmax + DRAW_SHELL -> body_r, \
               DRAW_SHELL -> field.ymax + DRAW_SHELL -> body_r, 0.0);

    glVertex3f(DRAW_SHELL -> field.xmax + DRAW_SHELL -> body_r, \
               DRAW_SHELL -> field.ymax + DRAW_SHELL -> body_r, 0.0);
    glVertex3f(DRAW_SHELL -> field.xmin - DRAW_SHELL -> body_r, \
               DRAW_SHELL -> field.ymax + DRAW_SHELL -> body_r, 0.0);

    glVertex3f(DRAW_SHELL -> field.xmin - DRAW_SHELL -> body_r, \
               DRAW_SHELL -> field.ymax + DRAW_SHELL -> body_r, 0.0);
    glVertex3f(DRAW_SHELL -> field.xmin - DRAW_SHELL -> body_r, \
               DRAW_SHELL -> field.ymin - DRAW_SHELL -> body_r, 0.0);

    glEnd();

    ///glPopMatrix();
    glFlush ();
}

int main(int argc, char **argv)
{
    shell_t shell;
    DRAW_SHELL = &shell;
    DUMP_FILE = fopen("experiment.csv", "w");

    experimet_init(&shell);

    /// Graphics
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(1000, 1000);
    glutInitWindowPosition(900, 50);
    glutCreateWindow("Experiment");
    init();
    glutDisplayFunc(display);
    glutIdleFunc(reshape);
    glutMainLoop();

    fclose(DUMP_FILE);
    return 0;
}
