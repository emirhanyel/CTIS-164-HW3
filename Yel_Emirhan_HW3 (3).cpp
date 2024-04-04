/*********
   CTIS164 - The Dance of The Planets
----------
STUDENT : Emirhan Yel 22103407
SECTION : 01
HOMEWORK: #3
----------
PROBLEMS: // Since I do not have enough space to display (F1,F2,F3) switch buttons in the buttom part of the screen,
//           I put them to the UP-LEFT side of the game screen
----------
ADDITIONAL FEATURES:
// 1) I added menu and gameover screen, if you press spacebar in the menu, game opens. If you press F6 in the game, game finishes and gameover screen will be
 //  displayed. If you press F5 in the gameover screen game will be restarted, else if you press F7 it will return to the menu, else you press ESC it will
 // exit the program

 // display_menu() and display_gameover is the functions that I mentioned above
*********/

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include<time.h>
#include"vec.h"


#define WINDOW_WIDTH  1000
#define WINDOW_HEIGHT 700

#define TIMER_PERIOD  16 // Period for the timer.               //300       //1000      //16
#define TIMER_ON         1 // 0:disable timer, 1:enable timer   //1        //0         //1

#define D2R 0.0174532

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
bool ON = true, OFF = false;
int  winWidth, winHeight; // current Window width and height

#define MENU 0
#define RUN 1
#define GAMEOVER 2

int radius_of_suns = 30;
int radius_of_sources = 15;

bool activeTimer = OFF;

float angle_a, angle_b, angle_c;
int speed_a, speed_b, speed_c;

int direction_a, direction_b, direction_c;
int dirA, dirB, dirC;
int state_of_game = MENU;

typedef struct {
    float r, g, b;
} clr_t;

typedef struct {
    vec_t   pos;
    clr_t color;
} lightt_t;

typedef struct {
    vec_t pos;
    vec_t N;
} sun_t;

int r_a = 80;
int r_b = 115;
int r_c = 150;
lightt_t light1[3];
lightt_t light2[3];
lightt_t light3[3];
lightt_t light4[3];

bool layer_a_create;
bool layer_b_create;
bool layer_c_create;

//Calculations of light affect

clr_t adding_color(clr_t color1, clr_t color2) {
    clr_t tempp = { color1.r + color2.r, color1.g + color2.g, color1.b + color2.b };
    return tempp;
}

clr_t mulColor(clr_t colorr, float num) {
    clr_t tempp = { num * colorr.r, num * colorr.g, num * colorr.b };
    return tempp;
}

double distanceImpact(double d)
{
    return (-1.0 / 350.0) * d + 1.0;
}


clr_t calculate_color(lightt_t source, sun_t v)
{
    vec_t L = subV(source.pos, v.pos);
    vec_t uL = unitV(L);
    float factor = distanceImpact(magV(L)) * dotP(uL, v.N);
    return mulColor(source.color, factor);
}


void InitialGlobals()
{
    layer_a_create = ON;
    layer_b_create = ON;
    layer_c_create = ON;
    speed_a = 1 + (rand() % 15);
    angle_a = rand() % 360;
    direction_a = rand() % 2;

    speed_b = 1 + (rand() % 15);
    angle_b = rand() % 360;
    direction_b = rand() % 2;

    speed_c = 1 + (rand() % 15);
    angle_c = rand() % 360;
    direction_c = rand() % 2;


    //all layer a light sources' initial values
    light1[0].color = { 1,0,0 };
    light1[0].pos = { -250 + r_a * cos(angle_a * D2R) , 175 + r_a * sin(angle_a * D2R) };

    light2[0].color = { 1,0,0 };
    light2[0].pos = { 250 + r_a * cos(angle_a * D2R) , 175 + r_a * sin(angle_a * D2R) };

    light3[0].color = { 1,0,0 };
    light3[0].pos = { -250 + r_a * cos(angle_a * D2R) , -175 + r_a * sin(angle_a * D2R) };

    light4[0].color = { 1,0,0 };
    light4[0].pos = { 250 + r_a * cos(angle_a * D2R) , -175 + r_a * sin(angle_a * D2R) };




    //all layer b light sources' initial values
    light1[1].color = { 0,1,0 };
    light1[1].pos = { -250 + r_b * cos(angle_a * D2R) , 175 + r_a * sin(angle_a * D2R) };

    light2[1].color = { 0,1,0 };
    light2[1].pos = { 250 + r_b * cos(angle_a * D2R) , 175 + r_a * sin(angle_a * D2R) };

    light3[1].color = { 0,1,0 };
    light3[1].pos = { -250 + r_b * cos(angle_a * D2R) , -175 + r_a * sin(angle_a * D2R) };

    light4[1].color = { 0,1,0 };
    light4[1].pos = { 250 + r_b * cos(angle_a * D2R) , -175 + r_a * sin(angle_a * D2R) };


    //all layer c light sources' initial values
    light1[2].color = { 0,0,1 };
    light1[2].pos = { -250 + r_c * cos(angle_c * D2R) , 175 + r_c * sin(angle_c * D2R) };

    light2[2].color = { 0,0,1 };
    light2[2].pos = { 250 + r_c * cos(angle_c * D2R) , 175 + r_c * sin(angle_c * D2R) };

    light3[2].color = { 0,0,1 };
    light3[2].pos = { -250 + r_c * cos(angle_c * D2R) , -175 + r_c * sin(angle_c * D2R) };

    light4[2].color = { 0,0,1 };
    light4[2].pos = { 250 + r_c * cos(angle_c * D2R) , -175 + r_c * sin(angle_c * D2R) };

}

//
// to draw circle, center at (x,y)
// radius r
//
void circle(int x, int y, int r)
{
#define PI 3.1415
    float angle;
    glBegin(GL_POLYGON);
    for (int i = 0; i < 100; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
    }
    glEnd();
}

void circle_wire(int x, int y, int r)
{
#define PI 3.1415
    float angle;

    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 100; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
    }
    glEnd();
}

void print(int x, int y, const char* string, void* font)
{
    int len, i;

    glRasterPos2f(x, y);
    len = (int)strlen(string);
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(font, string[i]);
    }
}

// display text with variables.
// vprint(-winWidth / 2 + 10, winHeight / 2 - 20, GLUT_BITMAP_8_BY_13, "ERROR: %d", numClicks);
void vprint(int x, int y, void* font, const char* string, ...)
{
    va_list ap;
    va_start(ap, string);
    char str[1024];
    vsprintf_s(str, string, ap);
    va_end(ap);

    int len, i;
    glRasterPos2f(x, y);
    len = (int)strlen(str);
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(font, str[i]);
    }
}

// vprint2(-50, 0, 0.35, "00:%02d", timeCounter);
void vprint2(int x, int y, float size, const char* string, ...) {
    va_list ap;
    va_start(ap, string);
    char str[1024];
    vsprintf_s(str, string, ap);
    va_end(ap);
    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(size, size, 1);

    int len, i;
    len = (int)strlen(str);
    for (i = 0; i < len; i++)
    {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
    }
    glPopMatrix();
}

//
// To display onto window using OpenGL commands
//


void light_sources_layer_a()
{

    //first lightning simulation part
    glColor3f(light1[0].color.r, light1[0].color.g, light1[0].color.b);
    circle(light1[0].pos.x, light1[0].pos.y, radius_of_sources);


    // second lighning simulation part
    /*glColor3f(1, 0, 0);
    circle(80 * cos(angle_a * D2R) + 250, 80 * sin(angle_a * D2R) + 175, 15);*/
    glColor3f(light2[0].color.r, light2[0].color.g, light2[0].color.b);
    circle(light2[0].pos.x, light2[0].pos.y, radius_of_sources);

    //third lightning simulation part
    glColor3f(light3[0].color.r, light3[0].color.g, light3[0].color.b);
    circle(light3[0].pos.x, light3[0].pos.y, radius_of_sources);


    //fourth lightning simulation part
    glColor3f(light4[0].color.r, light4[0].color.g, light4[0].color.b);
    circle(light4[0].pos.x, light4[0].pos.y, radius_of_sources);


}

void light_sources_layer_b()
{

    //first lightning simulation part
    glColor3f(light1[1].color.r, light1[1].color.g, light1[1].color.b);
    circle(light1[1].pos.x, light1[1].pos.y, radius_of_sources);


    // second lighning simulation part
    /*glColor3f(1, 0, 0);
    circle(80 * cos(angle_a * D2R) + 250, 80 * sin(angle_a * D2R) + 175, 15);*/
    glColor3f(light2[1].color.r, light2[1].color.g, light2[1].color.b);
    circle(light2[1].pos.x, light2[1].pos.y, radius_of_sources);

    //third lightning simulation part
    glColor3f(light3[1].color.r, light3[1].color.g, light3[1].color.b);
    circle(light3[1].pos.x, light3[1].pos.y, radius_of_sources);


    //fourth lightning simulation part
    glColor3f(light4[1].color.r, light4[1].color.g, light4[1].color.b);
    circle(light4[1].pos.x, light4[1].pos.y, radius_of_sources);


}

void light_sources_layer_c()
{

    //first lightning simulation part
    glColor3f(light1[2].color.r, light1[2].color.g, light1[2].color.b);
    circle(light1[2].pos.x, light1[2].pos.y, radius_of_sources);


    // second lighning simulation part
    /*glColor3f(1, 0, 0);
    circle(80 * cos(angle_a * D2R) + 250, 80 * sin(angle_a * D2R) + 175, 15);*/
    glColor3f(light2[2].color.r, light2[2].color.g, light2[2].color.b);
    circle(light2[2].pos.x, light2[2].pos.y, radius_of_sources);

    //third lightning simulation part
    glColor3f(light3[2].color.r, light3[2].color.g, light3[2].color.b);
    circle(light3[2].pos.x, light3[2].pos.y, radius_of_sources);


    //fourth lightning simulation part
    glColor3f(light4[2].color.r, light4[2].color.g, light4[2].color.b);
    circle(light4[2].pos.x, light4[2].pos.y, radius_of_sources);


}


void up_left_window_1()
{
    //up-left and the first lightning simulation part

    //The Sun
    glBegin(GL_TRIANGLE_FAN);

    glColor3f(0, 0, 0);
    glVertex2f(-250, 175);

    for (int z = 0; z <= 360; z++)
    {
        vec_t N = { cos(z * D2R) * radius_of_suns, sin(z * D2R) * radius_of_suns };
        vec_t uN = unitV(N);


        sun_t P = { { -250 + cos(z * D2R) * radius_of_suns, 175 + sin(z * D2R) * radius_of_suns }, { uN } };

        clr_t res = { 0, 0, 0 };
        for (int i = 0; i < 3; i++) {
            res = adding_color(res, calculate_color(light1[i], P));
        }

        float fin_r, fin_g, fin_b;

        if (layer_a_create)
            fin_r = res.r;
        else
            fin_r = 0;


        if (layer_b_create)
            fin_g = res.g;
        else
            fin_g = 0;


        if (layer_c_create)
            fin_b = res.b;
        else
            fin_b = 0;

        glColor3f(fin_r, fin_g, fin_b);
        glVertex2f(-250 + cos(z * D2R) * radius_of_suns, 175 + sin(z * D2R) * radius_of_suns);

    }

    glEnd();

    //layer a
    glColor3f(1, 1, 1);
    circle_wire(-250, 175, 80);

    //layer b
    glColor3f(1, 1, 1);
    circle_wire(-250, 175, 115);

    //layer c
    glColor3f(1, 1, 1);
    circle_wire(-250, 175, 150);
}

void up_right_window_2()
{

    //up-right and the second lightning simulation part


    //The Sun
    glBegin(GL_TRIANGLE_FAN);

    glColor3f(0, 0, 0);
    glVertex2f(250, 175);

    for (int z = 0; z <= 360; z++)
    {

        vec_t N = { cos(z * D2R) * radius_of_suns, sin(z * D2R) * radius_of_suns };
        vec_t uN = unitV(N);


        sun_t P = { { 250 + cos(z * D2R) * radius_of_suns, 175 + sin(z * D2R) * radius_of_suns }, { uN } };

        clr_t res = { 0, 0, 0 };
        for (int i = 0; i < 3; i++) {
            res = adding_color(res, calculate_color(light2[i], P));
        }

        float fin_r, fin_g, fin_b;

        if (layer_a_create)
            fin_r = res.r;
        else
            fin_r = 0;


        if (layer_b_create)
            fin_g = res.g;
        else
            fin_g = 0;


        if (layer_c_create)
            fin_b = res.b;
        else
            fin_b = 0;

        glColor3f(fin_r, fin_g, fin_b);
        glVertex2f(250 + cos(z * D2R) * radius_of_suns, 175 + sin(z * D2R) * radius_of_suns);

    }

    glEnd();


    //layer a
    glColor3f(1, 1, 1);
    circle_wire(250, 175, 80);

    //layer b
    glColor3f(1, 1, 1);
    circle_wire(250, 175, 115);

    //layer c
    glColor3f(1, 1, 1);
    circle_wire(250, 175, 150);


}
void down_left_window_3()
{
    //down-left and the third lightning simulation part

    //The Sun
    glBegin(GL_TRIANGLE_FAN);

    glColor3f(0, 0, 0);
    glVertex2f(-250, -175);

    for (int z = 0; z <= 360; z++)
    {
        vec_t N = { cos(z * D2R) * radius_of_suns, sin(z * D2R) * radius_of_suns };
        vec_t uN = unitV(N);


        sun_t P = { { -250 + cos(z * D2R) * radius_of_suns, -175 + sin(z * D2R) * radius_of_suns }, { uN } };

        clr_t res = { 0, 0, 0 };
        for (int i = 0; i < 3; i++) {
            res = adding_color(res, calculate_color(light3[i], P));
        }

        float fin_r, fin_g, fin_b;

        if (layer_a_create)
            fin_r = res.r;
        else
            fin_r = 0;


        if (layer_b_create)
            fin_g = res.g;
        else
            fin_g = 0;


        if (layer_c_create)
            fin_b = res.b;
        else
            fin_b = 0;

        glColor3f(fin_r, fin_g, fin_b);
        glVertex2f(-250 + cos(z * D2R) * radius_of_suns, -175 + sin(z * D2R) * radius_of_suns);

    }

    glEnd();

    //layer a
    glColor3f(1, 1, 1);
    circle_wire(-250, -175, 80);

    //layer b
    glColor3f(1, 1, 1);
    circle_wire(-250, -175, 115);

    //layer c
    glColor3f(1, 1, 1);
    circle_wire(-250, -175, 150);
}


void down_right_window_4()
{

    //down-right and the fourth lightning simulation part

    //The Sun
    glBegin(GL_TRIANGLE_FAN);

    glColor3f(0, 0, 0);
    glVertex2f(250, -175);

    for (int z = 0; z <= 360; z++)
    {
        vec_t N = { cos(z * D2R) * radius_of_suns, sin(z * D2R) * radius_of_suns };
        vec_t uN = unitV(N);


        sun_t P = { { 250 + cos(z * D2R) * radius_of_suns, -175 + sin(z * D2R) * radius_of_suns }, { uN } };

        clr_t res = { 0, 0, 0 };
        for (int i = 0; i < 3; i++) {
            res = adding_color(res, calculate_color(light4[i], P));
        }

        float fin_r, fin_g, fin_b;

        if (layer_a_create)
            fin_r = res.r;
        else
            fin_r = 0;


        if (layer_b_create)
            fin_g = res.g;
        else
            fin_g = 0;


        if (layer_c_create)
            fin_b = res.b;
        else
            fin_b = 0;

        glColor3f(fin_r, fin_g, fin_b);
        glVertex2f(250 + cos(z * D2R) * radius_of_suns, -175 + sin(z * D2R) * radius_of_suns);

    }

    glEnd();



    //layer a
    glColor3f(1, 1, 1);
    circle_wire(250, -175, 80);

    //layer b
    glColor3f(1, 1, 1);
    circle_wire(250, -175, 115);

    //layer c
    glColor3f(1, 1, 1);
    circle_wire(250, -175, 150);
}



void background()
{

    //The label which contains my name, my id and the homework number
    glColor3ub(0, 110.0, 0);
    glRectf(410, 350, 500, 285);
    glColor3ub(239.0, 176.0, 0);
    vprint(413, 330, GLUT_BITMAP_8_BY_13, "Emirhan Yel");
    vprint(413, 310, GLUT_BITMAP_8_BY_13, "22103407");
    vprint(413, 290, GLUT_BITMAP_8_BY_13, "Homework 3");


    //coordinate system lines which divides the screen four equal pieces

    //The X-Line
    glBegin(GL_LINES);
    glColor3f(1, 1, 1);
    glVertex2f(-500, 0);
    glVertex2f(500, 0);
    glLineWidth(5);
    glEnd();

    //The Y-Line
    glBegin(GL_LINES);
    glColor3f(1, 1, 1);
    glVertex2f(0, -350);
    glVertex2f(0, 350);
    glLineWidth(5);
    glEnd();



    //On-Off & re-start texts
    glColor3ub(239.0, 176.0, 0);


    vprint(-495, 335, GLUT_BITMAP_9_BY_15, "F1(Red):");
    if (layer_a_create)
        vprint(-420, 335, GLUT_BITMAP_9_BY_15, "ON");
    else
        vprint(-420, 335, GLUT_BITMAP_9_BY_15, "OFF");


    vprint(-495, 310, GLUT_BITMAP_9_BY_15, "F2(Green):");
    if (layer_b_create)
        vprint(-400, 310, GLUT_BITMAP_9_BY_15, "ON");
    else
        vprint(-400, 310, GLUT_BITMAP_9_BY_15, "OFF");


    vprint(-495, 285, GLUT_BITMAP_9_BY_15, "F3(Blue):");
    if (layer_c_create)
        vprint(-410, 285, GLUT_BITMAP_9_BY_15, "ON");
    else
        vprint(-410, 285, GLUT_BITMAP_9_BY_15, "OFF");


    vprint(-495, 35, GLUT_BITMAP_9_BY_15, "F4(Animation):");
    if (activeTimer)
        vprint(-365, 35, GLUT_BITMAP_9_BY_15, "ON");
    else
        vprint(-365, 35, GLUT_BITMAP_9_BY_15, "OFF");


    vprint(-495, 10, GLUT_BITMAP_9_BY_15, "F6:(Finish the game)");

}

void display_gameover()
{
    glColor3f(75.0 / 255, 1.0, 1.0);
    vprint(-80, 290, GLUT_BITMAP_TIMES_ROMAN_24, "GAME  IS  OVER!");

    glColor3f(1, 75.0 / 255, 1);
    vprint(-80, 100, GLUT_BITMAP_HELVETICA_18, "RETURN TO THE MENU");

    glColor3f(1, 75.0 / 255, 1);
    vprint(-70, 50, GLUT_BITMAP_HELVETICA_18, "RESTART THE GAME");

    glColor3f(1, 75.0 / 255, 1);
    vprint(-5, 0, GLUT_BITMAP_HELVETICA_18, "EXIT");

    glColor3f(75.0 / 255, 75.0 / 255, 1);
    vprint(-110, -230, GLUT_BITMAP_8_BY_13, "<<Press F7 to Return to The menu>>");

    glColor3f(75.0 / 255, 75.0 / 255, 1);
    vprint(-65, -260, GLUT_BITMAP_8_BY_13, "<<Press F5 to Restart>>");

    glColor3f(75.0 / 255, 75.0 / 255, 1);
    vprint(-55, -290, GLUT_BITMAP_8_BY_13, "<<Press ESC to EXIT>>");
}

void display_menu() {

    glColor3f(255, 173, 0);
    vprint(-100, 310, GLUT_BITMAP_HELVETICA_18, "Emirhan Yel - 22103407");

    glColor3f(255, 173, 0);
    vprint(-165, 260, GLUT_BITMAP_HELVETICA_18, "Homework 3 - The Dance of The Planets");


    glColor3f(33.0 / 255, 46.0 / 255, 82.0 / 255);
    vprint(-130, 60, GLUT_BITMAP_TIMES_ROMAN_24, "START  TO  THE  GAME");

    glColor3f(33.0 / 255, 46.0 / 255, 82.0 / 255);
    vprint(-30, -30, GLUT_BITMAP_TIMES_ROMAN_24, "EXIT");

    glColor3f(255, 173, 0);
    vprint(-125, -260, GLUT_BITMAP_HELVETICA_18, "<<Press  SPACEBAR  to  Start>>");

    glColor3f(255, 173, 0);
    vprint(-90, -310, GLUT_BITMAP_HELVETICA_18, "<<Press  ESC  to  Exit>>");


}

void RunState()
{

    background();

    up_left_window_1();
    up_right_window_2();
    down_left_window_3();
    down_right_window_4();

    if (layer_a_create)
        light_sources_layer_a();


    if (layer_b_create)
        light_sources_layer_b();


    if (layer_c_create)
        light_sources_layer_c();
}

void display() {
    //
    // clear window to black
    //
    glClearColor(96.0 / 255, 96.0 / 255, 96.0 / 255, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    switch (state_of_game)
    {
    case MENU:display_menu();
        break;

    case RUN: RunState();
        break;

    case GAMEOVER: display_gameover();
        break;
    }



    glutSwapBuffers();
}

//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
void onKeyDown(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);

    if (key == ' ' && state_of_game == MENU)
    {
        state_of_game = RUN;
        activeTimer = ON;
    }

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyDown(int key, int x, int y)
{
    // Write your codes here.
    switch (key) {
    case GLUT_KEY_UP: up = true; break;
    case GLUT_KEY_DOWN: down = true; break;
    case GLUT_KEY_LEFT: left = true; break;
    case GLUT_KEY_RIGHT: right = true; break;
    }

    if (key == GLUT_KEY_F1 && state_of_game == RUN)
    {

        layer_a_create = !layer_a_create;
    }

    if (key == GLUT_KEY_F2 && state_of_game == RUN)
    {

        layer_b_create = !layer_b_create;
    }

    if (key == GLUT_KEY_F3 && state_of_game == RUN)
    {

        layer_c_create = !layer_c_create;
    }


    if (key == GLUT_KEY_F6 && state_of_game == RUN)
    {
        //it finishes the game and goes to gameover screen
        state_of_game = GAMEOVER;
    }

    if (key == GLUT_KEY_F5 && state_of_game == GAMEOVER)
    {
        //restart from the gameover screen
        InitialGlobals();
        state_of_game = RUN;
        activeTimer = ON;
    }

    if (key == GLUT_KEY_F7 && state_of_game == GAMEOVER)
    {
        //gameover to menu
        state_of_game = MENU;
    }


    if (key == GLUT_KEY_F4 && state_of_game == RUN)
    {
        //animation on/off
        activeTimer = !activeTimer;
    }

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyUp(int key, int x, int y)
{
    // Write your codes here.
    switch (key) {
    case GLUT_KEY_UP: up = false; break;
    case GLUT_KEY_DOWN: down = false; break;
    case GLUT_KEY_LEFT: left = false; break;
    case GLUT_KEY_RIGHT: right = false; break;
    }

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// When a click occurs in the window,
// It provides which button
// buttons : GLUT_LEFT_BUTTON , GLUT_RIGHT_BUTTON
// states  : GLUT_UP , GLUT_DOWN
// x, y is the coordinate of the point that mouse clicked.
//
void onClick(int button, int stat, int x, int y)
{
    // Write your codes here.



    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// This function is called when the window size changes.
// w : is the new width of the window in pixels.
// h : is the new height of the window in pixels.
//
void onResize(int w, int h)
{
    winWidth = w;
    winHeight = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    display(); // refresh window.
}

void onMoveDown(int x, int y) {
    // Write your codes here.



    // to refresh the window it calls display() function   
    glutPostRedisplay();
}

// GLUT to OpenGL coordinate conversion:
//   x2 = x1 - winWidth / 2
//   y2 = winHeight / 2 - y1
void onMove(int x, int y) {
    // Write your codes here.



    // to refresh the window it calls display() function
    glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v) {

    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
    // Write your codes here.

    if (state_of_game == RUN && activeTimer)
    {

        //moving direction of a
        if (direction_a)
            dirA = 1;
        else
            dirA = -1;

        //moving direction of b
        if (direction_b)
            dirB = 1;
        else
            dirB = -1;

        //moving direction of c
        if (direction_c)
            dirC = 1;
        else
            dirC = -1;

        angle_a += speed_a;
        if (angle_a > 360)
            angle_a -= 360;


        angle_b += speed_b;
        if (angle_b > 360)
            angle_b -= 360;

        angle_c += speed_c;
        if (angle_c > 360)
            angle_c -= 360;

        //move of the light sources in the layer a
        light1[0].pos.x = -250 + r_a * cos(dirA * angle_a * D2R);
        light1[0].pos.y = 175 + r_a * sin(dirA * angle_a * D2R);

        light2[0].pos.x = 250 + r_a * cos(dirA * angle_a * D2R);
        light2[0].pos.y = 175 + r_a * sin(dirA * angle_a * D2R);

        light3[0].pos.x = -250 + r_a * cos(dirA * angle_a * D2R);
        light3[0].pos.y = -175 + r_a * sin(dirA * angle_a * D2R);

        light4[0].pos.x = +250 + r_a * cos(dirA * angle_a * D2R);
        light4[0].pos.y = -175 + r_a * sin(dirA * angle_a * D2R);

        //move of the light sources in the layer b
        light1[1].pos.x = -250 + r_b * cos(dirB * angle_b * D2R);
        light1[1].pos.y = 175 + r_b * sin(dirB * angle_b * D2R);

        light2[1].pos.x = 250 + r_b * cos(dirB * angle_b * D2R);
        light2[1].pos.y = 175 + r_b * sin(dirB * angle_b * D2R);

        light3[1].pos.x = -250 + r_b * cos(dirB * angle_b * D2R);
        light3[1].pos.y = -175 + r_b * sin(dirB * angle_b * D2R);

        light4[1].pos.x = +250 + r_b * cos(dirB * angle_b * D2R);
        light4[1].pos.y = -175 + r_b * sin(dirB * angle_b * D2R);

        //move of the light sources in the layer c
        light1[2].pos.x = -250 + r_c * cos(dirC * angle_c * D2R);
        light1[2].pos.y = 175 + r_c * sin(dirC * angle_c * D2R);

        light2[2].pos.x = 250 + r_c * cos(dirC * angle_c * D2R);
        light2[2].pos.y = 175 + r_c * sin(dirC * angle_c * D2R);

        light3[2].pos.x = -250 + r_c * cos(dirC * angle_c * D2R);
        light3[2].pos.y = -175 + r_c * sin(dirC * angle_c * D2R);

        light4[2].pos.x = +250 + r_c * cos(dirC * angle_c * D2R);
        light4[2].pos.y = -175 + r_c * sin(dirC * angle_c * D2R);


    }
    // to refresh the window it calls display() function
    glutPostRedisplay(); // display()

}
#endif

void Init() {

    // Smoothing shapes
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    InitialGlobals();
}

void main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    //glutInitWindowPosition(100, 100);
    glutCreateWindow("The Dance of The Planets");

    glutDisplayFunc(display);
    glutReshapeFunc(onResize);

    //
    // keyboard registration
    //
    glutKeyboardFunc(onKeyDown);
    glutSpecialFunc(onSpecialKeyDown);

    glutKeyboardUpFunc(onKeyUp);
    glutSpecialUpFunc(onSpecialKeyUp);

    //
    // mouse registration
    //
    glutMouseFunc(onClick);
    glutMotionFunc(onMoveDown);
    glutPassiveMotionFunc(onMove);

#if  TIMER_ON == 1
    // timer event
    srand(time(NULL));
    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
#endif

    Init();

    glutMainLoop();
}