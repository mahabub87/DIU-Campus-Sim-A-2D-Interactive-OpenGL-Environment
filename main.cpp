#include <stdio.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <math.h>
#define PI 3.14159265358979323846

/// ====    ANIMATION PART START  =========
static float txCloud=0, txCar3=0, txCar2=0, txPlane=0;

// Day/Night mode: 0=day, 1=night
static int isNight = 0;
// Sun/Moon position (x oscillates)
static float sunMoonY = 180.0f;
static float dayAngle = 0.0f; // 0..360 controls sun arc

// Bangladesh flag animation
static float flagWave = 0.0f;
static float flagPole = 90.0f; // flag pole x position

// Wheel spin angle
static float wheelAngle = 0.0f;
static float birdX[6] = {-20, -10, 0, 10, 20, 30};
static float birdY[6] = {120, 125, 118, 122, 128, 115};
static float birdWing[6] = {0, 0.5f, 1.0f, 1.5f, 0.3f, 0.8f};
static float birdSpeed[6] = {0.0035f, 0.0028f, 0.0032f, 0.0025f, 0.0030f, 0.0027f};

// Human walk cycle angle (radians)
static float walkCycle = 0.0f;

// Walking humans � positions, directions, speeds
// Group 1: near gate (originally at x=50..55, y=22)
static float hX[4] = {50.0f, 51.0f, 53.0f, 55.0f};
static float hY[4] = {22.0f, 22.0f, 22.0f, 22.0f};
static float hSpd[4] = {0.0025f, 0.0020f, 0.0018f, 0.0022f};
static int   hDir[4] = {1, -1, 1, -1};
static int   hShirtR[4] = {0,   0,  51, 0};
static int   hShirtG[4] = {51, 51, 125,25};
static int   hShirtB[4] = {51, 51,   0,51};

// Group 2: near building (originally at x=33..35, y=20)
static float h2X[2] = {33.0f, 35.0f};
static float h2Y[2] = {20.0f, 20.0f};
static float h2Spd[2] = {0.015f, 0.019f};
static int   h2Dir[2] = {1, -1};
static int   h2ShirtR[2] = {0,   102};
static int   h2ShirtG[2] = {25,   51};
static int   h2ShirtB[2] = {51,    0};

// human() and human4() � fixed guards / bystanders
// human()  at x=27, human4() at x=-7  (near gate / innovation lab)

void objMove()
{
    txCar3 -= 0.010f;
    txCar2 -= 0.011f;
    txCloud += 0.0035f;
    txPlane += 0.01f;
    wheelAngle += 3.0f;
    if(wheelAngle > 360.0f) wheelAngle -= 360.0f;

    if(txCar3 <= -155) txCar3 = 25;
    if(txCar2 <= -50)  txCar2 = 140;
    if(txPlane >= 140) txPlane = -55;
    if(txCloud >= 155) txCloud = -130;

    // Flag wave
    flagWave += 0.006f;
    if(flagWave > 2.0f*PI) flagWave -= 2.0f*PI;

    // Walk cycle
    walkCycle += 0.05f;
    if(walkCycle > 0.0f*PI) walkCycle -= 0.0f*PI;

    // Move group-1 humans left/right, bounce at road edges
    for(int i=0;i<4;i++){
        hX[i] += hSpd[i] * hDir[i];
        if(hX[i] > 120.0f){ hDir[i] = -1; }
        if(hX[i] <  -5.0f){ hDir[i] =  1; }
    }
    // Move group-2 humans
    for(int i=0;i<2;i++){
        h2X[i] += h2Spd[i] * h2Dir[i];
        if(h2X[i] > 120.0f){ h2Dir[i] = -1; }
        if(h2X[i] <  -5.0f){ h2Dir[i] =  1; }
    }

    // Birds
    for(int i=0;i<6;i++){
        birdX[i] += birdSpeed[i];
        birdWing[i] += 0.12f;
        if(birdWing[i] > 2.0f*PI) birdWing[i] -= 2.0f*PI;
        if(birdX[i] > 145) birdX[i] = -30;
    }

    // Sun rises from right horizon, sets on left
    // dayAngle: 0=sunrise (right), 180=sunset (left)
    dayAngle += 0.004f;
    if(dayAngle >= 180.0f){
        dayAngle = 0.0f; // reset to sunrise
    }

    glutPostRedisplay();
}

void object_control(unsigned char key, int x, int y)
{
    switch(key)
    {
    case 'm': // start animation
        glutIdleFunc(objMove);
        break;
    case 's': // stop animation
        glutIdleFunc(NULL);
        break;
    case 'd': // toggle day/night
        isNight = !isNight;
        glutPostRedisplay();
        break;
    }
}

// ====    ANIMATION PART END  =========


void circleSolid(float x, float y, float radius) ///   for BUS's tier and flag's circle and flowers
{
    int triangleAmount = 100;
    GLfloat twicePi = 2.0f * PI;
    glBegin(GL_TRIANGLE_FAN);
    for(int i = 0; i <= triangleAmount; i++)
    {
        glVertex2f(x + (radius * cos(i *  twicePi / triangleAmount)), y + (radius * sin(i * twicePi / triangleAmount)));
    }
    glEnd();
}

void plane()
{
    glPushMatrix();
    glTranslatef(txPlane, 0, 0);

    // ---- FUSELAGE (main body) ----
    // Lower belly
    glBegin(GL_QUADS);
    glColor3ub(220,220,235);
    glVertex3f(-8,109,0);
    glVertex3f(16,109,0);
    glVertex3f(16,112,0);
    glVertex3f(-8,111,0);
    glEnd();

    // Upper fuselage
    glBegin(GL_QUADS);
    glColor3ub(245,245,255);
    glVertex3f(-8,111,0);
    glVertex3f(16,112,0);
    glVertex3f(16,114,0);
    glVertex3f(-8,113,0);
    glEnd();

    // Green stripe (Biman Bangladesh livery)
    glBegin(GL_QUADS);
    glColor3ub(0,153,76);
    glVertex3f(-8,112.5f,0);
    glVertex3f(16,113,0);
    glVertex3f(16,113.5f,0);
    glVertex3f(-8,113.0f,0);
    glEnd();

    // Red stripe
    glBegin(GL_QUADS);
    glColor3ub(200,0,0);
    glVertex3f(-8,112.0f,0);
    glVertex3f(16,112.5f,0);
    glVertex3f(16,113.0f,0);
    glVertex3f(-8,112.5f,0);
    glEnd();

    // ---- NOSE (pointed front) ----
    glBegin(GL_TRIANGLES);
    glColor3ub(200,200,215);
    glVertex3f(16,109,0);
    glVertex3f(20,111,0);
    glVertex3f(16,114,0);
    glEnd();

    // Nose cone tip
    glBegin(GL_TRIANGLES);
    glColor3ub(160,160,180);
    glVertex3f(18,110.5f,0);
    glVertex3f(20,111,0);
    glVertex3f(18,112.5f,0);
    glEnd();

    // Cockpit window
    glBegin(GL_QUADS);
    glColor3ub(150,200,230);
    glVertex3f(15,112,0);
    glVertex3f(17,112.5f,0);
    glVertex3f(17,113.5f,0);
    glVertex3f(15,114,0);
    glEnd();
    // Cockpit frame
    glBegin(GL_LINES);
    glColor3ub(80,80,100);
    glVertex3f(15,112,0); glVertex3f(17,112.5f,0);
    glEnd();
    glBegin(GL_LINES);
    glColor3ub(80,80,100);
    glVertex3f(17,112.5f,0); glVertex3f(17,113.5f,0);
    glEnd();
    glBegin(GL_LINES);
    glColor3ub(80,80,100);
    glVertex3f(17,113.5f,0); glVertex3f(15,114,0);
    glEnd();

    // ---- CABIN WINDOWS ----
    for(int i=0;i<6;i++){
        float wx = 13.5f - i*3.5f;
        glBegin(GL_QUADS);
        glColor3ub(180,220,245);
        glVertex3f(wx,    112.2f,0);
        glVertex3f(wx+1.2f,112.2f,0);
        glVertex3f(wx+1.2f,113.5f,0);
        glVertex3f(wx,    113.5f,0);
        glEnd();
        glBegin(GL_LINES);
        glColor3ub(120,140,160);
        glVertex3f(wx,112.2f,0); glVertex3f(wx+1.2f,112.2f,0);
        glEnd();
        glBegin(GL_LINES);
        glColor3ub(120,140,160);
        glVertex3f(wx+1.2f,112.2f,0); glVertex3f(wx+1.2f,113.5f,0);
        glEnd();
        glBegin(GL_LINES);
        glColor3ub(120,140,160);
        glVertex3f(wx+1.2f,113.5f,0); glVertex3f(wx,113.5f,0);
        glEnd();
        glBegin(GL_LINES);
        glColor3ub(120,140,160);
        glVertex3f(wx,113.5f,0); glVertex3f(wx,112.2f,0);
        glEnd();
    }

    // ---- TAIL (vertical stabilizer) ----
    glBegin(GL_QUADS);
    glColor3ub(0,153,76); // green
    glVertex3f(-8,113,0);
    glVertex3f(-5,113,0);
    glVertex3f(-5,118,0);
    glVertex3f(-8,118,0);
    glEnd();
    // white on tail
    glBegin(GL_QUADS);
    glColor3ub(255,255,255);
    glVertex3f(-8,115,0);
    glVertex3f(-5,115,0);
    glVertex3f(-5,118,0);
    glVertex3f(-8,118,0);
    glEnd();
    // Red circle on tail
    glColor3ub(200,0,0);
    circleSolid(-6.5f,116.5f,0.9f);

    // Tail horizontal stabilizer
    glBegin(GL_QUADS);
    glColor3ub(220,220,235);
    glVertex3f(-9,109.5f,0);
    glVertex3f(-6,109.5f,0);
    glVertex3f(-5,111,0);
    glVertex3f(-9,111,0);
    glEnd();
    // Lower stabilizer
    glBegin(GL_QUADS);
    glColor3ub(220,220,235);
    glVertex3f(-9,112,0);
    glVertex3f(-5,112,0);
    glVertex3f(-5,111,0);
    glVertex3f(-9,110.5f,0);
    glEnd();

    // ---- MAIN WINGS ----
    // Upper wing surface
    glBegin(GL_QUADS);
    glColor3ub(200,200,215);
    glVertex3f(0,111,0);
    glVertex3f(8,111,0);
    glVertex3f(4,106,0);
    glVertex3f(-4,106,0);
    glEnd();
    // Wing top line
    glBegin(GL_LINE_LOOP);
    glColor3ub(150,150,170);
    glVertex3f(0,111,0);
    glVertex3f(8,111,0);
    glVertex3f(4,106,0);
    glVertex3f(-4,106,0);
    glEnd();

    // Wing tip (winglet)
    glBegin(GL_TRIANGLES);
    glColor3ub(0,153,76);
    glVertex3f(-4,106,0);
    glVertex3f(-5,105,0);
    glVertex3f(-3,106,0);
    glEnd();

    // ---- ENGINES ----
    // Engine 1 under wing
    glBegin(GL_QUADS);
    glColor3ub(80,80,90);
    glVertex3f(2,109,0);
    glVertex3f(6,109,0);
    glVertex3f(6,110,0);
    glVertex3f(2,110,0);
    glEnd();
    glColor3ub(50,50,60);
    circleSolid(2.2f,109.5f,0.5f);
    // Engine 2 under wing
    glBegin(GL_QUADS);
    glColor3ub(80,80,90);
    glVertex3f(-1,108,0);
    glVertex3f(2,108,0);
    glVertex3f(2,109,0);
    glVertex3f(-1,109,0);
    glEnd();
    glColor3ub(50,50,60);
    circleSolid(-0.8f,108.5f,0.4f);

    glPopMatrix();
}

void innovationLab()
{
    glBegin(GL_QUADS);//shade grey
    glColor3ub(96,96,96);
    glVertex3i(-21,40,0);
    glVertex3i(-6,40,0);
    glVertex3i(-6,46,0);
    glVertex3i(-21,46,0);
    glEnd();

    glBegin(GL_QUADS);//shade grey+green 1
    glColor3ub(0,204,0);
    glVertex3i(-23,40,0);
    glVertex3i(-21,40,0);
    glVertex3i(-21,46,0);
    glVertex3i(-23,46,0);
    glEnd();

    glBegin(GL_QUADS);//shade grey+green 2
    glColor3ub(0,204,0);
    glVertex3f(-6.5,40,0);
    glVertex3f(-4.5,40,0);
    glVertex3f(-4.5,46,0);
    glVertex3f(-6.5,46,0);
    glEnd();


    glBegin(GL_QUADS);//green rectangle 1
    glColor3ub(0,204,0);
    glVertex3i(-7,40,0);
    glVertex3i(-9,40,0);
    glVertex3i(-9,43,0);
    glVertex3i(-7,43,0);
    glEnd();

    glBegin(GL_QUADS);//green rectangle 2
    glColor3ub(0,204,0);
    glVertex3i(-11,46,0);
    glVertex3i(-9,46,0);
    glVertex3i(-9,43,0);
    glVertex3i(-11,43,0);
    glEnd();

    glBegin(GL_QUADS);//green rectangle 3
    glColor3ub(0,204,0);
    glVertex3i(-11,40,0);
    glVertex3i(-13,40,0);
    glVertex3i(-13,43,0);
    glVertex3i(-11,43,0);
    glEnd();

    glBegin(GL_QUADS);//green rectangle 4
    glColor3ub(0,204,0);
    glVertex3i(-15,46,0);
    glVertex3i(-13,46,0);
    glVertex3i(-13,43,0);
    glVertex3i(-15,43,0);
    glEnd();

    glBegin(GL_QUADS);//green rectangle 5
    glColor3ub(0,204,0);
    glVertex3i(-15,40,0);
    glVertex3i(-17,40,0);
    glVertex3i(-17,43,0);
    glVertex3i(-15,43,0);
    glEnd();

    glBegin(GL_QUADS);//green rectangle 6
    glColor3ub(0,204,0);
    glVertex3i(-19,46,0);
    glVertex3i(-17,46,0);
    glVertex3i(-17,43,0);
    glVertex3i(-19,43,0);
    glEnd();
    glBegin(GL_QUADS);//green rectangle 7
    glColor3ub(0,204,0);
    glVertex3i(-19,40,0);
    glVertex3i(-21,40,0);
    glVertex3i(-21,43,0);
    glVertex3i(-19,43,0);
    glEnd();



    glBegin(GL_LINES);//horizontal shade
    glColor3ub(0,0,0);
    glVertex3i(-23,43,0);
    glVertex3f(-4.5,43,0);
    glEnd();

    glBegin(GL_LINES);//horiz line 1
    glColor3ub(0,0,0);
    glVertex3i(-23,40,0);
    glVertex3f(-4.5,40,0);
    glEnd();

    glBegin(GL_LINES);//horizontal line 2
    glColor3ub(0,0,0);
    glVertex3i(-23,46,0);
    glVertex3f(-4.5,46,0);
    glEnd();

    glBegin(GL_LINES);//horizontal line 3
    glColor3ub(0,0,0);
    glVertex3i(-23,40,0);
    glVertex3i(-23,46,0);
    glEnd();

    glBegin(GL_LINES);//horizontal line 4
    glColor3ub(0,0,0);
    glVertex3i(-21,40,0);
    glVertex3i(-21,46,0);
    glEnd();

    glBegin(GL_LINES);//horizontal line 5
    glColor3ub(0,0,0);
    glVertex3i(-19,40,0);
    glVertex3i(-19,46,0);
    glEnd();

    glBegin(GL_LINES);//horizontal line 6
    glColor3ub(0,0,0);
    glVertex3i(-17,40,0);
    glVertex3i(-17,46,0);
    glEnd();

    glBegin(GL_LINES);//horizontal line 7
    glColor3ub(0,0,0);
    glVertex3i(-15,40,0);
    glVertex3i(-15,46,0);
    glEnd();

    glBegin(GL_LINES);//horizontal line 8
    glColor3ub(0,0,0);
    glVertex3i(-13,40,0);
    glVertex3i(-13,46,0);
    glEnd();

    glBegin(GL_LINES);//horizontal line 9
    glColor3ub(0,0,0);
    glVertex3i(-11,40,0);
    glVertex3i(-11,46,0);
    glEnd();

    glBegin(GL_LINES);//horizontal line 10
    glColor3ub(0,0,0);
    glVertex3i(-9,40,0);
    glVertex3i(-9,46,0);
    glEnd();

    glBegin(GL_LINES);//horizontal line 11
    glColor3ub(0,0,0);
    glVertex3i(-7,40,0);
    glVertex3i(-7,46,0);
    glEnd();


    glBegin(GL_LINES);//horizontal line 12
    glColor3ub(0,0,0);
    glVertex3i(-5,40,0);
    glVertex3i(-5,46,0);
    glEnd();



    glBegin(GL_QUADS);//body building up part
    glColor3ub(192,192,192);
    glVertex3i(-22,32,0);
    glVertex3f(-5.5,32,0);
    glVertex3f(-5.5,40,0);
    glVertex3i(-22,40,0);
    glEnd();

    glBegin(GL_LINES);//body building up part line 1
    glColor3ub(0,0,0);
    glVertex3i(-22,32,0);
    glVertex3i(-22,40,0);
    glEnd();

    glBegin(GL_LINES);//body building up part line 2
    glColor3ub(0,0,0);
    glVertex3f(-5.5,32,0);
    glVertex3f(-5.5,40,0);
    glEnd();

    glBegin(GL_LINES);//body building up part line 3
    glColor3ub(255,255,255);
    glVertex3f(-13.5,32,0);
    glVertex3f(-13.5,40,0);
    glEnd();

    glBegin(GL_LINES);//body building up part line 4
    glColor3ub(255,255,255);
    glVertex3f(-17.75,32,0);
    glVertex3f(-17.75,40,0);
    glEnd();

    glBegin(GL_LINES);//body building up part line 5
    glColor3ub(255,255,255);
    glVertex3f(-9.25,32,0);
    glVertex3f(-9.25,40,0);
    glEnd();




    glBegin(GL_QUADS);//body building 2
    glColor3ub(204,0,0);
    glVertex3f(-22.5,25,0);
    glVertex3i(-5,25,0);
    glVertex3i(-5,32,0);
    glVertex3f(-22.5,32,0);
    glEnd();
//left door//
    glBegin(GL_QUADS); //main door
    glColor3ub(0,153,153);
    glVertex3f(-19, 25, 0);
    glVertex3f(-16, 25, 0);
    glVertex3f(-16, 33, 0);
    glVertex3f(-19, 33, 0);
    glEnd();
    glBegin(GL_LINES); //main door line 1
    glColor3ub(0,0,0);
    glVertex3f(-19, 25, 0);
    glVertex3f(-16, 25, 0);
    glEnd();
    glBegin(GL_LINES); //main door line 2
    glColor3ub(0,0,0);
    glVertex3f(-16, 25, 0);
    glVertex3f(-16, 33, 0);
    glEnd();
    glBegin(GL_LINES); //main doorline 3
    glColor3ub(0,0,0);
    glVertex3f(-16, 33, 0);
    glVertex3f(-19, 33, 0);
    glEnd();
    glBegin(GL_LINES); //main doorline 4
    glColor3ub(0,0,0);
    glVertex3f(-19, 25, 0);
    glVertex3f(-19, 33, 0);
    glEnd();

    glBegin(GL_LINES); //main doorline 5
    glColor3ub(0,0,0);
    glVertex3f(-17.5, 25, 0);
    glVertex3f(-17.5, 33, 0);
    glEnd();

    //right door//

    glBegin(GL_QUADS); //main door
    glColor3ub(0,153,153);
    glVertex3f(-11, 25, 0);
    glVertex3f(-8, 25, 0);
    glVertex3f(-8, 33, 0);
    glVertex3f(-11, 33, 0);
    glEnd();
    glBegin(GL_LINES); //main door line 1
    glColor3ub(0,0,0);
    glVertex3f(-11, 25, 0);
    glVertex3f(-8, 25, 0);
    glEnd();
    glBegin(GL_LINES); //main door line 2
    glColor3ub(0,0,0);
    glVertex3f(-8, 25, 0);
    glVertex3f(-8, 33, 0);
    glEnd();
    glBegin(GL_LINES); //main doorline 3
    glColor3ub(0,0,0);
    glVertex3f(-8, 33, 0);
    glVertex3f(-11, 33, 0);
    glEnd();
    glBegin(GL_LINES); //main doorline 4
    glColor3ub(0,0,0);
    glVertex3f(-11, 25, 0);
    glVertex3f(-11, 33, 0);
    glEnd();

    glBegin(GL_LINES); //main doorline 5
    glColor3ub(0,0,0);
    glVertex3f(-9.5, 25, 0);
    glVertex3f(-9.5, 33, 0);
    glEnd();

}


void guardroom()
{
    glBegin(GL_QUADS); //guard room base
    glColor3ub(192,192,192);
    glVertex3i(38, 25, 0);
    glVertex3i(44, 25, 0);
    glVertex3i(44, 35, 0);
    glVertex3i(38, 35, 0);
    glEnd();

    glBegin(GL_QUADS); //guard room window base
    glColor3ub(0,0,0);
    glVertex3i(39, 32, 0);
    glVertex3i(43, 32, 0);
    glVertex3i(43, 34, 0);
    glVertex3i(39, 34, 0);
    glEnd();

    glBegin(GL_QUADS); //guard room window base 2
    glColor3ub(255,204,255);
    glVertex3i(40, 33, 0);
    glVertex3i(42, 33, 0);
    glVertex3i(42, 34, 0);
    glVertex3i(40, 34, 0);
    glEnd();

    glBegin(GL_QUADS); //guard room window base 3
    glColor3ub(0,153,153);
    glVertex3i(40, 27, 0);
    glVertex3i(42, 27, 0);
    glVertex3i(42, 31, 0);
    glVertex3i(40, 31, 0);
    glEnd();

    glBegin(GL_LINES); //window line 1
    glColor3ub(0,0,0);
    glVertex3i(40, 27, 0);
    glVertex3i(42, 27, 0);
    glEnd();
    glBegin(GL_LINES); //window line 2
    glColor3ub(0,0,0);
    glVertex3i(42, 27, 0);
    glVertex3i(42, 31, 0);
    glEnd();
    glBegin(GL_LINES); //window line 3
    glColor3ub(0,0,0);
    glVertex3i(42, 31, 0);
    glVertex3i(40, 31, 0);
    glEnd();
    glBegin(GL_LINES); //window line 4
    glColor3ub(0,0,0);
    glVertex3i(40, 31, 0);
    glVertex3i(40, 27, 0);
    glEnd();
    glBegin(GL_LINES); //window line 5
    glColor3ub(0,0,0);
    glVertex3i(41, 27, 0);
    glVertex3i(41, 31, 0);
    glEnd();

}


// ===== ANIMATED HUMAN =====
// Draws a walking human at world position (x, y).
// shirtR/G/B = shirt colour.  phase = walk phase offset so
// each person is at a different step.
// dir = 1 facing right, -1 facing left (flips via scale).
void drawHuman(float x, float y, int shirtR, int shirtG, int shirtB,
               float phase, int dir)
{
    float sw = sinf(walkCycle + phase);   // swing value -1..+1
    float maxSwing = 5.0f;               // max limb angle in degrees

    // ---- body pivot at hip level (x, y+3) ----
    glPushMatrix();
    glTranslatef(x, y, 0);
    // Flip horizontally for left-facing humans
    glScalef((float)dir, 1.0f, 1.0f);

    // --- HEAD ---
    // Neck
    glColor3ub(255,178,102);
    glBegin(GL_QUADS);
    glVertex2f(0.3f, 8.5f);
    glVertex2f(0.7f, 8.5f);
    glVertex2f(0.7f, 9.2f);
    glVertex2f(0.3f, 9.2f);
    glEnd();
    // Head circle (polygon)
    glColor3ub(255,178,102);
    glBegin(GL_POLYGON);
    for(int i=0;i<16;i++){
        float a = i*2.0f*PI/16.0f;
        glVertex2f(0.5f + 0.55f*cosf(a), 9.85f + 0.65f*sinf(a));
    }
    glEnd();
    // Hair
    glColor3ub(40,20,10);
    glBegin(GL_POLYGON);
    for(int i=0;i<10;i++){
        float a = PI + i*PI/9.0f;
        glVertex2f(0.5f + 0.55f*cosf(a), 9.85f + 0.70f*sinf(a));
    }
    glEnd();
    // Eyes
    glColor3ub(30,30,30);
    glBegin(GL_POLYGON);
    for(int i=0;i<8;i++){float a=i*2*PI/8; glVertex2f(0.62f+0.07f*cosf(a),9.85f+0.08f*sinf(a));}
    glEnd();

    // --- TORSO (shirt) ---
    glColor3ub(shirtR, shirtG, shirtB);
    glBegin(GL_QUADS);
    glVertex2f(0.0f, 5.5f);
    glVertex2f(1.0f, 5.5f);
    glVertex2f(1.0f, 8.5f);
    glVertex2f(0.0f, 8.5f);
    glEnd();
    // Collar
    glColor3ub(230,230,230);
    glBegin(GL_TRIANGLES);
    glVertex2f(0.35f, 8.5f);
    glVertex2f(0.65f, 8.5f);
    glVertex2f(0.5f,  7.8f);
    glEnd();

    // --- LEFT ARM (swings forward with right leg) ---
    glPushMatrix();
    glTranslatef(0.05f, 8.2f, 0);
    glRotatef( sw * maxSwing, 0, 0, 1);
    glColor3ub(shirtR, shirtG, shirtB);
    glBegin(GL_QUADS);
    glVertex2f(-0.2f, 0.0f);
    glVertex2f( 0.1f, 0.0f);
    glVertex2f( 0.1f,-2.0f);
    glVertex2f(-0.2f,-2.0f);
    glEnd();
    // Forearm
    glColor3ub(255,178,102);
    glBegin(GL_QUADS);
    glVertex2f(-0.15f,-2.0f);
    glVertex2f( 0.05f,-2.0f);
    glVertex2f( 0.05f,-3.5f);
    glVertex2f(-0.15f,-3.5f);
    glEnd();
    glPopMatrix();

    // --- RIGHT ARM (swings back when left arm forward) ---
    glPushMatrix();
    glTranslatef(0.95f, 8.2f, 0);
    glRotatef(-sw * maxSwing, 0, 0, 1);
    glColor3ub(shirtR, shirtG, shirtB);
    glBegin(GL_QUADS);
    glVertex2f(-0.1f, 0.0f);
    glVertex2f( 0.2f, 0.0f);
    glVertex2f( 0.2f,-2.0f);
    glVertex2f(-0.1f,-2.0f);
    glEnd();
    glColor3ub(255,178,102);
    glBegin(GL_QUADS);
    glVertex2f(-0.05f,-2.0f);
    glVertex2f( 0.15f,-2.0f);
    glVertex2f( 0.15f,-3.5f);
    glVertex2f(-0.05f,-3.5f);
    glEnd();
    glPopMatrix();

    // --- WAIST / BELT ---
    glColor3ub(80,50,20);
    glBegin(GL_QUADS);
    glVertex2f(0.0f, 5.2f);
    glVertex2f(1.0f, 5.2f);
    glVertex2f(1.0f, 5.6f);
    glVertex2f(0.0f, 5.6f);
    glEnd();

    // --- LEFT LEG (swings back when right arm forward) ---
    glPushMatrix();
    glTranslatef(0.2f, 5.2f, 0);
    glRotatef(-sw * maxSwing, 0, 0, 1);
    // Thigh
    glColor3ub(40,40,100);
    glBegin(GL_QUADS);
    glVertex2f(-0.2f, 0.0f);
    glVertex2f( 0.2f, 0.0f);
    glVertex2f( 0.2f,-2.0f);
    glVertex2f(-0.2f,-2.0f);
    glEnd();
    // Shin
    glColor3ub(50,50,120);
    glBegin(GL_QUADS);
    glVertex2f(-0.18f,-2.0f);
    glVertex2f( 0.18f,-2.0f);
    glVertex2f( 0.18f,-3.8f);
    glVertex2f(-0.18f,-3.8f);
    glEnd();
    // Shoe
    glColor3ub(30,20,10);
    glBegin(GL_POLYGON);
    glVertex2f(-0.25f,-3.8f);
    glVertex2f( 0.4f, -3.8f);
    glVertex2f( 0.4f, -4.1f);
    glVertex2f(-0.25f,-4.1f);
    glEnd();
    glPopMatrix();

    // --- RIGHT LEG (swings forward) ---
    glPushMatrix();
    glTranslatef(0.8f, 5.2f, 0);
    glRotatef( sw * maxSwing, 0, 0, 1);
    glColor3ub(40,40,100);
    glBegin(GL_QUADS);
    glVertex2f(-0.2f, 0.0f);
    glVertex2f( 0.2f, 0.0f);
    glVertex2f( 0.2f,-2.0f);
    glVertex2f(-0.2f,-2.0f);
    glEnd();
    glColor3ub(50,50,120);
    glBegin(GL_QUADS);
    glVertex2f(-0.18f,-2.0f);
    glVertex2f( 0.18f,-2.0f);
    glVertex2f( 0.18f,-3.8f);
    glVertex2f(-0.18f,-3.8f);
    glEnd();
    glColor3ub(30,20,10);
    glBegin(GL_POLYGON);
    glVertex2f(-0.25f,-3.8f);
    glVertex2f( 0.4f, -3.8f);
    glVertex2f( 0.4f, -4.1f);
    glVertex2f(-0.25f,-4.1f);
    glEnd();
    glPopMatrix();

    glPopMatrix(); // end body transform
}

// Wrappers kept for backward compatibility with display() calls
void human()
{
    // Standing guard near gate � faces right, slight idle sway only
    drawHuman(27.0f, 22.0f, 0, 51, 51, 0.0f, 1);
}

void human4()
{
    // Standing guard near innovation lab � faces left
    drawHuman(-7.0f, 22.0f, 0, 0, 51, PI, -1);
}

// Called from display with moving array data
void humanDy(float x, float y, int bA, int bB, int bC, int dir)
{
    float phase = fmodf(x * 1.3f, 2.0f*PI);
    drawHuman(x, y, bA, bB, bC, phase, dir);
}


void mediumTree2()
{
    glBegin(GL_POLYGON); //
    glColor3ub(0, 102, 0);
    glVertex3i(109, 29, 0);//1
    glVertex3i(112, 29, 0);
    glVertex3i(110, 30, 0);
    glVertex3i(112, 31, 0);
    glVertex3i(114, 29, 0); //5
    glVertex3i(115, 31, 0);
    glVertex3i(116, 33, 0);
    glVertex3i(114, 33, 0);
    glVertex3i(113, 35, 0);
    glVertex3i(111, 34, 0); //10
    glVertex3i(108, 35, 0);
    glVertex3i(106, 34, 0);
    glVertex3i(103, 33, 0);
    glVertex3i(101, 30, 0);
    glVertex3i(103, 28, 0); //15
    glVertex3i(105, 29, 0);
    glVertex3i(108, 29, 0);
    glEnd();

    glBegin(GL_QUADS); //
    glColor3ub(51, 0, 0);
    glVertex3i (108, 25, 0);
    glVertex3f (108.5, 25, 0);
    glVertex3f (108.5, 30, 0);
    glVertex3i (108, 30, 0);
    glEnd();

    glBegin(GL_LINES); //
    glColor3ub(51, 0, 0);
    glVertex3f(108, 30, 0);
    glVertex3f(104, 32, 0);
    glEnd();

    glBegin(GL_LINES); //
    glColor3ub(51, 0, 0);
    glVertex3f(108, 30, 0);
    glVertex3f(113, 32, 0);
    glEnd();

    glBegin(GL_LINES); //
    glColor3ub(51, 0, 0);
    glVertex3f(108, 30, 0);
    glVertex3f(109, 33, 0);
    glEnd();

    glBegin(GL_LINES); //
    glColor3ub(51, 0, 0);
    glVertex3f(108, 30, 0);
    glVertex3f(107, 32, 0);
    glEnd();

}


void mediumTree()
{
    glBegin(GL_POLYGON); //
    glColor3ub(0, 153, 0);
    glVertex3i(45, 34, 0);//1
    glVertex3i(48, 34, 0);
    glVertex3i(46, 35, 0);
    glVertex3i(48, 36, 0);
    glVertex3i(50, 34, 0); //5
    glVertex3i(51, 36, 0);
    glVertex3i(52, 38, 0);
    glVertex3i(50, 38, 0);
    glVertex3i(49, 40, 0);
    glVertex3i(47, 39, 0); //10
    glVertex3i(44, 40, 0);
    glVertex3i(42, 39, 0);
    glVertex3i(39, 38, 0);
    glVertex3i(37, 35, 0);
    glVertex3i(39, 33, 0); //15
    glVertex3i(41, 34, 0);
    glVertex3i(44, 34, 0);
    glEnd();

    glBegin(GL_QUADS); //
    glColor3ub(51, 0, 0);
    glVertex3i (44, 32, 0);
    glVertex3f (44.5, 32, 0);
    glVertex3f (44.5, 35, 0);
    glVertex3i (44, 35, 0);
    glEnd();

    glBegin(GL_LINES); //
    glColor3ub(51, 0, 0);
    glVertex3f(44, 35, 0);
    glVertex3f(40, 37, 0);
    glEnd();

    glBegin(GL_LINES); //
    glColor3ub(51, 0, 0);
    glVertex3f(44, 35, 0);
    glVertex3f(49, 37, 0);
    glEnd();

    glBegin(GL_LINES); //
    glColor3ub(51, 0, 0);
    glVertex3f(44, 35, 0);
    glVertex3f(45, 38, 0);
    glEnd();

    glBegin(GL_LINES); //
    glColor3ub(51, 0, 0);
    glVertex3f(44, 35, 0);
    glVertex3f(43, 37, 0);
    glEnd();

    glColor3ub(255,0,0);
    circleSolid(42,38,0.25);
    circleSolid(41,37,0.25);
    circleSolid(40,38,0.25);
    circleSolid(39,36,0.25);
    circleSolid(40,36,0.25);
    circleSolid(42,36,0.25);
    circleSolid(43,36,0.25);
    circleSolid(43,38,0.25);
    circleSolid(44,39,0.25);
    circleSolid(45,39,0.25);
    circleSolid(44,38,0.25);
    circleSolid(44,36,0.25);
    circleSolid(44,35,0.25);
    circleSolid(46,35,0.25);
    circleSolid(47,35,0.25);
    circleSolid(48,36,0.25);
    circleSolid(49,35,0.25);
    circleSolid(50,36,0.25);
    circleSolid(51,37,0.25);
    circleSolid(49,37,0.25);
    circleSolid(48,38,0.25);
    circleSolid(47,38,0.25);
    circleSolid(49,39,0.25);
    circleSolid(44,38,0.25);
    circleSolid(45,36,0.25);
    circleSolid(44,35,0.25);
    circleSolid(47,35,0.25);
    circleSolid(46,38,0.25);



}

void bushGrass(int x, int y)
{
    glBegin(GL_POLYGON); //
    glColor3ub(0, 102, 0);
    glVertex3i(x, y, 0);//1  // init 66, 22 x, y
    glVertex3i(x+1, y-1, 0);
    glVertex3i(x+2, y+1, 0);
    glVertex3i(x+2, y+3, 0);
    glVertex3i(x+1, y+4, 0); //5
    glVertex3i(x+1, y+5, 0);
    glVertex3i(x-2, y+5, 0);
    glVertex3i(x-3, y+4, 0);
    glVertex3i(x-4, y+3, 0);
    glVertex3i(x-4, y+1, 0);//10
    glVertex3i(x-3, y-1, 0);
    glVertex3i(x-3, y, 0);
    glVertex3i(x-1, y+3, 0);
    glEnd();

    glBegin(GL_LINES);   // init 66, 22 x, y
    glColor3ub(51, 25, 0);
    glVertex3f(x-1, y-2, 0);
    glVertex3f(x-1, y+2, 0);
    glEnd();

    glBegin(GL_LINES);
    glColor3ub(51, 25, 0);
    glVertex3f(x-1, y-2, 0);
    glVertex3f(x-3, y+1, 0);
    glEnd();

    glBegin(GL_LINES);
    glColor3ub(51, 25, 0);
    glVertex3f(x-1, y-2, 0);
    glVertex3f(x+1, y+1, 0);
    glEnd();

    glBegin(GL_LINES);
    glColor3ub(51, 25, 0);
    glVertex3f(x-1, y-2, 0);
    glVertex3f(x-2, y+1, 0);
    glEnd();

    glBegin(GL_LINES);
    glColor3ub(51, 25, 0);
    glVertex3f(x-1, y-2, 0);
    glVertex3f(x, y+1, 0);
    glEnd();
}

void grass(float x, float y)
{
    glBegin(GL_LINES); //
    glColor3ub(0, 102, 0);
    glVertex3f(x, y, 0); // 5, 20  initial
    glVertex3f(x, y+3, 0);
    glEnd();

    glBegin(GL_LINES); //
    glColor3ub(0, 102, 0);
    glVertex3f(x, y, 0);
    glVertex3f(x-1.5, y+2, 0);
    glEnd();

    glBegin(GL_LINES); //
    glColor3ub(0, 102, 0);
    glVertex3f(x, y, 0);
    glVertex3f(x+1.5, y+2, 0);
    glEnd();

    glBegin(GL_LINES); //
    glColor3ub(0, 102, 0);
    glVertex3f(x, y, 0);
    glVertex3f(x-0.5, y+2, 0);
    glEnd();

    glBegin(GL_LINES); //
    glColor3ub(0, 102, 0);
    glVertex3f(x, y, 0);
    glVertex3f(x+0.5, y+2, 0);
    glEnd();
}

void bigTree()
{
    glBegin(GL_POLYGON); //
    glColor3ub(0, 102, 0);
    glVertex3i(37, 30, 0);//1
    glVertex3i(39, 31, 0);
    glVertex3i(40, 33, 0);
    glVertex3i(39, 33, 0);
    glVertex3i(41, 35, 0);//5
    glVertex3i(43, 33, 0);
    glVertex3i(43, 35, 0);
    glVertex3i(44, 35, 0);
    glVertex3i(42, 37, 0);
    glVertex3i(43, 41, 0);//10
    glVertex3i(41, 42, 0);
    glVertex3i(38, 41, 0);
    glVertex3i(37, 43, 0);
    glVertex3i(34, 42, 0);
    glVertex3i(31, 43, 0);//15
    glVertex3i(28, 37, 0);
    glVertex3i(28, 35, 0);
    glVertex3i(27, 36, 0);
    glVertex3i(28, 31, 0);
    glVertex3i(32, 33, 0);//20
    glVertex3i(33, 32, 0);
    glEnd();

    glBegin(GL_QUADS); // left
    glColor3ub(51, 0, 0);
    glVertex3i (30, 25, 0);
    glVertex3f (30.5, 25, 0);
    glVertex3f (30.5, 33, 0);
    glVertex3i (30, 33, 0);
    glEnd();

    glBegin(GL_QUADS); // middle
    glColor3ub(51, 0, 0);
    glVertex3i (34, 25, 0);
    glVertex3f (34.5, 25, 0);
    glVertex3f (34.5, 34, 0);
    glVertex3i (34, 34, 0);
    glEnd();

    glBegin(GL_QUADS); // right
    glColor3ub(51, 0, 0);
    glVertex3i (36, 25, 0);
    glVertex3f (36.5, 25, 0);
    glVertex3f (36.5, 34, 0);
    glVertex3i (36, 34, 0);
    glEnd();


    glBegin(GL_LINES); // left l
    glColor3ub(51, 0, 0);
    glVertex3f(36.5, 31, 0);
    glVertex3f(40, 34, 0);
    glEnd();

    glBegin(GL_LINES); // left l
    glColor3ub(51, 0, 0);
    glVertex3f(36, 31, 0);
    glVertex3f(35.5, 33, 0);
    glEnd();

    glBegin(GL_LINES); // middle l
    glColor3ub(51, 0, 0);
    glVertex3f(34.5, 33, 0);
    glVertex3f(36, 35.5, 0);
    glEnd();

    glBegin(GL_LINES); // middle l
    glColor3ub(51, 0, 0);
    glVertex3f(34, 32, 0);
    glVertex3f(33, 35, 0);
    glEnd();

    glBegin(GL_LINES); // right l
    glColor3ub(51, 0, 0);
    glVertex3f(30.5, 33, 0);
    glVertex3f(32, 34, 0);
    glEnd();

    glBegin(GL_LINES); // right l
    glColor3ub(51, 0, 0);
    glVertex3f(30, 33, 0);
    glVertex3f(29.5, 34.5, 0);
    glEnd();
}

void tree2(int x, int y, int a, int b, int c)
{
    glBegin(GL_QUADS);  //p1
    glColor3ub(51, 0, 0);
    glVertex3i (x, y-10, 0); // -10, 25
    glVertex3i (x+2, y-10, 0);
    glVertex3i (x+2, y+5, 0);
    glVertex3i (x, y+5, 0);
    glEnd();

    glBegin(GL_TRIANGLES); //p2
    glColor3ub(a, b, c);
    glVertex3i(x-5, y+5, 0);
    glVertex3i(x+7, y+5, 0);
    glVertex3i(x+1, y+12, 0);
    glEnd();

    glBegin(GL_TRIANGLES); //p3
    glColor3ub(a, b, c);
    glVertex3i(x-5, y+8, 0);
    glVertex3i(x+7, y+8, 0);
    glVertex3i(x+1, y+15, 0);
    glEnd();

    glBegin(GL_TRIANGLES); //p4
    glColor3ub(a, b, c);
    glVertex3i(x-5, y+11, 0);
    glVertex3i(x+7, y+11, 0);
    glVertex3i(x+1, y+18, 0);
    glEnd();
}

void tree(int x, int y)
{
    glBegin(GL_QUADS);  //p1
    glColor3ub(51, 0, 0);
    glVertex3i (x, y, 0); // -10, 25
    glVertex3i (x+2, y, 0);
    glVertex3i (x+2, y+5, 0);
    glVertex3i (x, y+5, 0);
    glEnd();

    glBegin(GL_TRIANGLES); //p2
    glColor3ub(0, 102, 0);
    glVertex3i(x-5, y+5, 0);
    glVertex3i(x+7, y+5, 0);
    glVertex3i(x+1, y+12, 0);
    glEnd();

    glBegin(GL_TRIANGLES); //p3
    glColor3ub(0, 102, 0);
    glVertex3i(x-5, y+8, 0);
    glVertex3i(x+7, y+8, 0);
    glVertex3i(x+1, y+15, 0);
    glEnd();

    glBegin(GL_TRIANGLES); //p4
    glColor3ub(0, 102, 0);
    glVertex3i(x-5, y+11, 0);
    glVertex3i(x+7, y+11, 0);
    glVertex3i(x+1, y+18, 0);
    glEnd();
}

///======= ======  play ground START  =========
void playGround()
{
    glBegin(GL_QUADS); // main field
    glColor3ub(0, 153, 0);
    glVertex3i (-25, -25, 0);
    glVertex3i (200, -25, 0);
    glVertex3i (200, 20, 0);
    glVertex3i (5, 20, 0);
    glEnd();

    glBegin(GL_LINES); // main field down side line
    glColor3ub(255, 255, 204);
    glVertex3i (-15, -20, 0);
    glVertex3i (117, -20, 0);
    glEnd();

    glBegin(GL_LINES);// main field up side line
    glColor3ub(255,255,204);
    glVertex3i (7, 15, 0);
    glVertex3i (125, 15, 0);
    glEnd();

    glBegin(GL_LINES);// main field left side line
    glColor3ub(255,255,204);
    glVertex3i (-15, -20, 0);
    glVertex3i (7, 15, 0);
    glEnd();


    glBegin(GL_LINES);// main field right side line
    glColor3ub(255,255,204);
    glVertex3i (117, -20, 0);
    glVertex3i (125, 15, 0);
    glEnd();


    glBegin(GL_LINES);// main field middle line
    glColor3ub(255,255,204);
    glVertex3i (51, -20, 0);
    glVertex3i (65, 15, 0);
    glEnd();

    glBegin(GL_LINES);// main field left side D box down line
    glColor3ub(255,255,204);
    glVertex3i (-12, -15, 0);
    glVertex3i (10, -15, 0);
    glEnd();

    glBegin(GL_LINES);// main field left side D box up line
    glColor3ub(255,255,204);
    glVertex3i (4, 10, 0);
    glVertex3i (25, 10, 0);
    glEnd();

    glBegin(GL_LINES);// main field left side D box right line
    glColor3ub(255,255,204);
    glVertex3i (10, -15, 0);
    glVertex3i (25, 10, 0);
    glEnd();

    glBegin(GL_LINES);// main field right side D box up line
    glColor3ub(255,255,204);
    glVertex3i (105, 10, 0);
    glVertex3i (124, 10, 0);
    glEnd();

    glBegin(GL_LINES);// main field right side D box down line
    glColor3ub(255,255,204);
    glVertex3i (97, -15, 0);
    glVertex3i (118, -15, 0);
    glEnd();

    glBegin(GL_LINES);// main field right side D box left line
    glColor3ub(255,255,204);
    glVertex3i (105, 10, 0);
    glVertex3i (97, -15, 0);
    glEnd();

    glBegin(GL_LINES);// main field left side down penalty line
    glColor3ub(255,255,204);
    glVertex3i (-7, -7, 0);
    glVertex3i (-2,-7 , 0);
    glEnd();

    glBegin(GL_LINES);// main field left side up penalty line
    glColor3ub(255,255,204);
    glVertex3i (-2, 1, 0);
    glVertex3i (3,1 , 0);
    glEnd();

    glBegin(GL_LINES);// main field left side right penalty line
    glColor3ub(255,255,204);
    glVertex3i (-2, -7, 0);
    glVertex3i (3,1 , 0);
    glEnd();

    glBegin(GL_LINES);// main field right side up penalty box down line
    glColor3ub(255,255,204);
    glVertex3i (116, 1, 0);
    glVertex3i (122, 1, 0);
    glEnd();

    glBegin(GL_LINES);// main field right side down penalty left line
    glColor3ub(255,255,204);
    glVertex3i (114, -7, 0);
    glVertex3i (120, -7, 0);
    glEnd();

    glBegin(GL_LINES);// main field right side left penalty line
    glColor3ub(255,255,204);
    glVertex3i (116, 1, 0);
    glVertex3i (114,-7 , 0);
    glEnd();

    glBegin(GL_LINES);// main field left side down goalpostline
    glColor3ub(255,255,204);
    glVertex3i (-7, -7, 0);
    glVertex3i (-7,-2 , 0);
    glEnd();

    glBegin(GL_LINES);// main field left side up goalpost line
    glColor3ub(255,255,204);
    glVertex3i (-2, 1, 0);
    glVertex3i (-2, 6 , 0);
    glEnd();

    glBegin(GL_LINES);// main field left side  goalpost bar line
    glColor3ub(255,255,204);
    glVertex3i (-7, -2, 0);
    glVertex3i (-2, 6 , 0);
    glEnd();


    glBegin(GL_LINES);// main field left side support goalpost bar line
    glColor3ub(255,255,204);
    glVertex3i (-7, -2, 0);
    glVertex3i (-9, -4 , 0);
    glEnd();


    glBegin(GL_LINES);// main field left side support 2 goalpost bar line
    glColor3ub(255,255,204);
    glVertex3i (-9, -4, 0);
    glVertex3i (-4, 4 , 0);
    glEnd();

    glBegin(GL_LINES);// main field left side support 3 goalpost bar line
    glColor3ub(255,255,204);
    glVertex3i (-4, 4, 0);
    glVertex3i (-2, 6 , 0);
    glEnd();

    glBegin(GL_LINES);// main field left side support 4 goalpost bar line
    glColor3ub(255,255,204);
    glVertex3i (-9, -4, 0);
    glVertex3i (-9, -8 , 0);
    glEnd();

    glBegin(GL_LINES);// main field left side support 5 goalpost bar line
    glColor3ub(255,255,204);
    glVertex3i (-9, -8, 0);
    glVertex3i (-7, -7 , 0);
    glEnd();

    glBegin(GL_LINES);// main field left side net 1 goalpost bar line
    glColor3ub(255,255,204);
    glVertex3i (-9, -7, 0);
    glVertex3i (-7, -6 , 0);
    glEnd();

    glBegin(GL_LINES);// main field left side net 2 goalpost bar line
    glColor3ub(255,255,204);
    glVertex3i (-9, -6, 0);
    glVertex3i (-7, -5 , 0);
    glEnd();

    glBegin(GL_LINES);// main field left side net 3 goalpost bar line
    glColor3ub(255,255,204);
    glVertex3i (-9, -5, 0);
    glVertex3i (-7, -4 , 0);
    glEnd();

    glBegin(GL_LINES);// main field left side net 4 goalpost bar line
    glColor3ub(255,255,204);
    glVertex3i (-9, -4, 0);
    glVertex3i (-7, -3 , 0);
    glEnd();
    glBegin(GL_LINES);// main field left side net 5 goalpost bar line
    glColor3ub(255,255,204);
    glVertex3i (-8, -7, 0);
    glVertex3i (-8, -3 , 0);
    glEnd();

    glBegin(GL_LINES);// main field left side net 6 goalpost bar line
    glColor3ub(255,255,204);
    glVertex3f (-8.5, -8, 0);
    glVertex3f (-8.5, -3 , 0);
    glEnd();

    glBegin(GL_LINES);// main field left side net 7 goalpost bar line
    glColor3ub(255,255,204);
    glVertex3f (-7.5, -7, 0);
    glVertex3f (-7.5, -3 , 0);
    glEnd();


    glBegin(GL_LINES);// main field right side up goalpost down line
    glColor3ub(255,255,204);
    glVertex3i (122, 6, 0);
    glVertex3i (122, 1, 0);
    glEnd();

    glBegin(GL_LINES);// main field right side down goalpost line
    glColor3ub(255,255,204);
    glVertex3i (120, -1, 0);
    glVertex3i (120, -7, 0);
    glEnd();

    glBegin(GL_LINES);// main field right side left goalpost line
    glColor3ub(255,255,204);
    glVertex3i (122, 6, 0);
    glVertex3i (120,-1 , 0);
    glEnd();

    glBegin(GL_LINES);// main field right side support 1 goalpost line
    glColor3ub(255,255,204);
    glVertex3i (120, -1, 0);
    glVertex3i (122,-2 , 0);
    glEnd();

    glBegin(GL_LINES);// main field right side net 1 goalpost line
    glColor3ub(255,255,204);
    glVertex3i (120, 0, 0);
    glVertex3i (122,-1 , 0);
    glEnd();

    glBegin(GL_LINES);// main field right side net 2 goalpost line
    glColor3ub(255,255,204);
    glVertex3i (120, 1, 0);
    glVertex3i (123,0 , 0);
    glEnd();

    glBegin(GL_LINES);// main field right side net 3 goalpost line
    glColor3ub(255,255,204);
    glVertex3i (121, 2, 0);
    glVertex3i (123,1 , 0);
    glEnd();

    glBegin(GL_LINES);// main field right side net 4 goalpost line
    glColor3ub(255,255,204);
    glVertex3i (121, 3, 0);
    glVertex3i (123,2, 0);
    glEnd();

    glBegin(GL_LINES);// main field right side net 5 goalpost line
    glColor3ub(255,255,204);
    glVertex3f (121.5, 4, 0);
    glVertex3f (123.5,3 , 0);
    glEnd();

    glBegin(GL_LINES);// main field right side net 6 goalpost line
    glColor3ub(255,255,204);
    glVertex3f (121.8, 5, 0);
    glVertex3f (123.8,4 , 0);
    glEnd();


    glBegin(GL_LINES);// main field right side support 2 goalpost line
    glColor3ub(255,255,204);
    glVertex3i (122, 6, 0);
    glVertex3i (124,5 , 0);
    glEnd();

    glBegin(GL_LINES);// main field right side support 3 goalpost line
    glColor3ub(255,255,204);
    glVertex3i (122, -2, 0);
    glVertex3i (124,5 , 0);
    glEnd();

    glBegin(GL_LINES);// main field right side net 1 goalpost line
    glColor3ub(255,255,204);
    glVertex3i (122, -3, 0);
    glVertex3i (124,4 , 0);
    glEnd();



    glBegin(GL_LINES);// main field right side net 2 goalpost line
    glColor3ub(255,255,204);
    glVertex3i (122, -4, 0);
    glVertex3i (124,3 , 0);
    glEnd();

    glBegin(GL_LINES);// main field right side net 3 goalpost line
    glColor3ub(255,255,204);
    glVertex3i (122, -5, 0);
    glVertex3i (124,2, 0);
    glEnd();

    glBegin(GL_LINES);// main field right side net 4 goalpost line
    glColor3ub(255,255,204);
    glVertex3i (122, -6, 0);
    glVertex3i (124,1 , 0);
    glEnd();

    glBegin(GL_LINES);// main field right side support 4 goalpost line
    glColor3ub(255,255,204);
    glVertex3i (122,-2, 0);
    glVertex3i (122,-7 , 0);
    glEnd();

    glBegin(GL_LINES);// main field right side support 5 goalpost line
    glColor3ub(255,255,204);
    glVertex3i (124, 5 , 0);
    glVertex3i (124,0 , 0);
    glEnd();

    glBegin(GL_LINES);// main field right side support 6 goalpost line
    glColor3ub(255,255,204);
    glVertex3i (124, 0, 0);
    glVertex3i (122,-7 , 0);
    glEnd();

    glBegin(GL_LINES);// main field right side support 7 goalpost line
    glColor3ub(255,255,204);
    glVertex3i (122, -7, 0);
    glVertex3i (120,-7 , 0);
    glEnd();

    glBegin(GL_LINES);// main field right side net goalpost line
    glColor3ub(255,255,204);
    glVertex3i (122, -6, 0);
    glVertex3i (120,-6 , 0);
    glEnd();

    glBegin(GL_LINES);// main field right side net goalpost line
    glColor3ub(255,255,204);
    glVertex3i (122, -5, 0);
    glVertex3i (120,-5 , 0);
    glEnd();

    glBegin(GL_LINES);// main field right side net goalpost line
    glColor3ub(255,255,204);
    glVertex3i (122, -4, 0);
    glVertex3i (120,-4 , 0);
    glEnd();

    glBegin(GL_LINES);// main field right side net goalpost line
    glColor3ub(255,255,204);
    glVertex3i (122, -3, 0);
    glVertex3i (120,-3 , 0);
    glEnd();

    glBegin(GL_LINES);// main field right side net goalpost line
    glColor3ub(255,255,204);
    glVertex3i (122, -3, 0);
    glVertex3i (120,-2 , 0);
    glEnd();

    glBegin(GL_LINES);// main field right side net 2 goalpost line
    glColor3ub(255,255,204);
    glVertex3f (121.5, -7, 0);
    glVertex3f (121.5,-2 , 0);
    glEnd();
    glBegin(GL_LINES);// main field right side net 3 goalpost line
    glColor3ub(255,255,204);
    glVertex3f (120.5, -7, 0);
    glVertex3f (120.5,-2 , 0);
    glEnd();

    glBegin(GL_LINES);// main field right side net goalpost line
    glColor3ub(255,255,204);
    glVertex3i (121, -7, 0);
    glVertex3i (121,-2 , 0);
    glEnd();
}
void midCircle()
{
    float theta, x = 58.5, y = -2, r = 6;
    glBegin(GL_LINES);
    for(int i = 0; i < 360; i++)
    {
        theta = i * PI / 180;
        glVertex2f(x + r * cos(theta), y + r * sin(theta));

    }
    glEnd();
}

void LDboxCircle()
{
    float theta, x = 103, y = -2, r = 5;
    glBegin(GL_LINES);
    for(int i = 90; i < 230; i++)
    {
        theta = i * PI / 180;
        glVertex2f(x + r * cos(theta), y + r * sin(theta));

    }
    glEnd();
}

void RDboxCircle()
{
    float theta, x = 18, y = -2, r = 4;
    glBegin(GL_LINES);
    for(int i = -120; i < 65; i++)
    {
        theta = i * PI / 180;
        glVertex2f(x + r * cos(theta), y + r * sin(theta));
    }
    glEnd();
}

//======= ====== play ground END  ============

void drawFiledCircle(float cx, float cy, float r, int num_segments) /// for clouds
{
    glBegin(GL_POLYGON);
    glColor3ub(244, 244, 244);
    for (int ii = 1; ii <= num_segments; ii++)
    {
        float theta = 2.0f * PI * float(ii) / float(num_segments);//get the current angle
        float x = r * cosf(theta); //calculate the x component
        float y = r * sinf(theta); //calculate the y component
        glVertex2f(x + cx, y + cy); //output vertex
    }
    glEnd();
}

void cloud()
{
    glPushMatrix();
    glTranslatef(txCloud, 0, 0);

    //cloud 1
    drawFiledCircle(80, 110, 5, 22);
    drawFiledCircle(85, 110, 5, 22);
    drawFiledCircle(83, 115, 5, 22);

    //cloud2
    drawFiledCircle(100, 130, 5, 22);
    drawFiledCircle(105, 130, 5, 22);
    drawFiledCircle(103, 135, 5, 22);


    //cloud3
    drawFiledCircle(50, 130, 5, 22);
    drawFiledCircle(55, 130, 5, 22);
    drawFiledCircle(53, 135, 5, 22);

    //cloud4
    drawFiledCircle(10, 130, 5, 22);
    drawFiledCircle(15, 130, 5, 22);
    drawFiledCircle(13, 135, 5, 22);

    drawFiledCircle(4, 130, 5, 22);
    drawFiledCircle(9, 130, 5, 22);
    drawFiledCircle(7, 135, 5, 22);

    //cloud5
    drawFiledCircle(-10, 110, 5, 22);
    drawFiledCircle(-5, 110, 5, 22);
    drawFiledCircle(-7, 115, 5, 22);

    glPopMatrix();
}

void road()
{
    glBegin(GL_QUADS); //r 1
    glColor3ub(255, 204, 153);
    glVertex3i (-25, 20, 0);
    glVertex3i (200, 20, 0);
    glVertex3i (200, 25, 0);
    glVertex3i (-25, 25, 0);
    glEnd();

    glBegin(GL_LINES);
    glColor3ub(0,0,0);
    glVertex3i (-25, 20, 0);
    glVertex3i (200, 20, 0);
    glEnd();

    glBegin(GL_LINES);
    glColor3ub(0,0,0);
    glVertex3i (200, 25, 0);
    glVertex3i (-25, 25, 0);
    glEnd();

    glBegin(GL_QUADS); //r 2
    glColor3ub(255, 204, 153);
    glVertex3i (10, 25, 0);
    glVertex3i (-25, -25, 0);
    glVertex3i (-25, -20, 0);
    glVertex3i (5, 25, 0);
    glEnd();

    glBegin(GL_LINES);
    glColor3ub(0,0,0);
    glVertex3i (10, 25, 0);
    glVertex3i (-25, 25, 0);
    glEnd();

    glBegin(GL_LINES);
    glColor3ub(0,0,0);
    glVertex3i (5, 25, 0);
    glVertex3i (-25, 25, 0);
    glEnd();

    glBegin(GL_LINES);
    glColor3ub(0,0,0);
    glVertex3i (5, 20, 0);
    glVertex3i (10, 20, 0);
    glEnd();

}

void mosque()
{

    glBegin(GL_QUADS);//1 no
    glColor3ub(128,128,128);
    glVertex3i(86, 25, 0);
    glVertex3i(103, 25, 0);
    glVertex3i(108, 45, 0);
    glVertex3i(81, 45, 0);

    glEnd();
    glBegin(GL_QUADS);//3 no
    glColor3ub(100,10,0);
    glVertex3i(89, 25, 0);
    glVertex3i(100, 25, 0);
    glVertex3i(100, 39,0);
    glVertex3i(89, 39,0);
    glEnd();

    glBegin(GL_QUADS);//2 no
    glColor3ub(0,0,0);
    glVertex3f(88.5, 41, 0);
    glVertex3f(100.5, 41, 0);
    glVertex3f(100.5, 43, 0);
    glVertex3f(88.5, 43, 0);
    glEnd();

    glBegin(GL_QUADS);//4 no
    glColor3ub(255,255,224);
    glVertex3i(91, 25, 0);
    glVertex3i(98, 25, 0);
    glVertex3i(98, 35, 0);
    glVertex3i(91, 35, 0);
    glEnd();

    glBegin(GL_QUADS);//5 no
    glColor3ub(154,44,48);
    glVertex3i(84, 25, 0);
    glVertex3i(105, 25, 0);
    glVertex3i(105, 27, 0);
    glVertex3i(84, 27, 0);
    glEnd();

    glBegin(GL_QUADS);//10 no
    glColor3ub(192,192,192);
    glVertex3i(80, 45, 0);
    glVertex3i(109, 45, 0);
    glVertex3i(109, 48, 0);
    glVertex3i(80, 48, 0);
    glEnd();

    glBegin(GL_LINES);//10 no line 1
    glColor3ub(0,0,0);
    glVertex3i(80, 45, 0);
    glVertex3i(109, 45, 0);
    glEnd();
    glBegin(GL_LINES);//10 no line 2
    glColor3ub(0,0,0);
    glVertex3i(109, 45, 0);
    glVertex3i(109, 48, 0);
    glEnd();
    glBegin(GL_LINES);//10 no line 3
    glColor3ub(0,0,0);
    glVertex3i(109, 48, 0);
    glVertex3i(80, 48, 0);
    glEnd();

    glBegin(GL_LINES);//10 no line 4
    glColor3ub(0,0,0);
    glVertex3i(80, 45, 0);
    glVertex3i(80, 48, 0);
    glEnd();

    glBegin(GL_QUADS);//4(middle portion rectangle) no
    glColor3ub(0,128,128);
    glVertex3i(93, 27, 0);
    glVertex3i(96, 27, 0);
    glVertex3i(96, 30, 0);
    glVertex3i(93, 30, 0);
    glEnd();

    glBegin(GL_TRIANGLES);//4(middle portion triangle) no
    glColor3ub(0,128,128);
    glVertex3i(93, 30, 0);
    glVertex3i(96, 30, 0);
    glVertex3f(94.5, 33, 0);
    glEnd();

// ==================== Moszid  END =====================
}


// ========== BANGLADESH FLAG ==========
void bangladeshFlag()
{
    float px = 20.0f;
    float py = 65.0f; // top of building
    float fw = 8.0f;  // flag width
    float fh = 5.0f;  // flag height
    float wAmp = 0.4f; // wave amplitude

    // Flag pole
    glBegin(GL_QUADS);
    glColor3ub(80,60,20);
    glVertex3f(px,      py,     0);
    glVertex3f(px+.4f, py,     0);
    glVertex3f(px+0.4f, py+20,  0);
    glVertex3f(px,      py+14,  0);
    glEnd();

    // Flag body (green) with wave using triangle strip
    // We approximate the wave with a quad mesh
    int segs = 10;
    float segW = fw / segs;

    // Draw green background of flag with wave
    glBegin(GL_TRIANGLE_STRIP);
    glColor3ub(0,150,60);
    for(int i=0;i<=segs;i++){
        float fx = px + 0.4f + i*segW;
        float wave = wAmp * sinf(flagWave + i*0.7f);
        glVertex3f(fx, py+12+fh+wave,     0);
        glVertex3f(fx, py+12+wave,         0);
    }
    glEnd();

    // Red circle in center of flag
    // Find center wave offset
    float cWave = wAmp * sinf(flagWave + segs*0.35f);
    glColor3ub(210,30,30);
    circleSolid(px + 0.4f + fw*0.45f, py+12+fh*0.5f+cWave, 1.6f);
}

// ========== BIRDS ==========
void drawBird(float bx, float by, float wingPhase)
{
    float wFlap = sinf(wingPhase) * 1.0f;
    // Left wing
    glBegin(GL_TRIANGLES);
    glColor3ub(30,30,30);
    glVertex3f(bx,    by,       0);
    glVertex3f(bx-2,  by+wFlap, 0);
    glVertex3f(bx-0.5f, by,     0);
    glEnd();
    // Right wing
    glBegin(GL_TRIANGLES);
    glColor3ub(30,30,30);
    glVertex3f(bx,      by,       0);
    glVertex3f(bx+2,    by+wFlap, 0);
    glVertex3f(bx+0.5f, by,       0);
    glEnd();
    // Body
    glBegin(GL_TRIANGLES);
    glColor3ub(50,50,50);
    glVertex3f(bx-0.5f, by,       0);
    glVertex3f(bx+0.5f, by,       0);
    glVertex3f(bx,      by-0.8f,  0);
    glEnd();
}

void allBirds()
{
    for(int i=0;i<6;i++){
        drawBird(birdX[i], birdY[i], birdWing[i]);
    }
}

// ========== SUN & MOON ==========
// dayAngle: 0 = just risen from right horizon, 180 = just set on left horizon
// Arc: right side of screen = angle 0, peak = angle 90, left = angle 180
// arcX goes from right (130) to left (-25) as angle goes 0->180
// arcY peaks at the top when angle=90

void drawSun(float cx, float cy)
{
    // Outer glow (warm orange)
    glColor3ub(255,200,60);
    circleSolid(cx, cy, 6.5f);
    // Mid glow (yellow)
    glColor3ub(255,230,80);
    circleSolid(cx, cy, 5.0f);
    // Core (bright white-yellow)
    glColor3ub(255,255,150);
    circleSolid(cx, cy, 3.5f);
    // Rays � rotating slowly with dayAngle
    glColor3ub(255,210,40);
    float rayRot = dayAngle * PI / 180.0f * 2.0f; // full spin over arc
    for(int i=0;i<12;i++){
        float a = rayRot + i * PI / 6.0f;
        float innerR = 5.5f;
        float outerR = 9.0f + 1.5f*(i%2); // alternating long/short
        glLineWidth(1.5f);
        glBegin(GL_LINES);
        glVertex3f(cx + innerR*cosf(a), cy + innerR*sinf(a), 0);
        glVertex3f(cx + outerR*cosf(a), cy + outerR*sinf(a), 0);
        glEnd();
        glLineWidth(1.0f);
    }
    // Horizon glow when near horizon (angle < 15 or > 165)
    if(dayAngle < 18.0f || dayAngle > 162.0f){
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(1.0f, 0.5f, 0.1f, 0.3f);
        circleSolid(cx, cy, 14.0f);
        glDisable(GL_BLEND);
    }
}

void drawMoon(float cx, float cy)
{
    // Full moon disk (pale yellow)
    glColor3ub(255,255,255);
    circleSolid(cx, cy, 4.5f);
    // Crescent shadow � offset circle, colored to match night sky (same as basicColors night)
    glColor3ub(12,12,38);
    circleSolid(cx+2.2f, cy+0.5f, 3.8f);
    // Surface craters (small circles)
    glColor3ub(200,200,140);
    circleSolid(cx-1.0f, cy+1.5f, 0.5f);
    circleSolid(cx-2.0f, cy-0.8f, 0.35f);
    circleSolid(cx-0.5f, cy-1.2f, 0.4f);
    // Glow halo
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.9f,0.9f,0.7f,0.15f);
    circleSolid(cx, cy, 8.0f);
    glDisable(GL_BLEND);
    // Stars near moon
    glColor3ub(255,255,200);
    float s[6][2]={{9,4},{-7,7},{11,-4},{-6,-6},{6,11},{-10,2}};
    for(int i=0;i<6;i++) circleSolid(cx+s[i][0], cy+s[i][1], 0.28f);
}

void sunMoon()
{
    // dayAngle 0..180: sun rises from RIGHT horizon, arcs up, sets LEFT
    // X: from 125 (right) down to -20 (left) linearly
    // Y: parabolic arc, peak at angle=90

    float rad = dayAngle * PI / 180.0f; // 0..PI
    // X: right=125 when rad=0, left=-20 when rad=PI
    float arcX = 125.0f - 145.0f * (rad / PI);
    // Y: horizon level = 28, peak = 140; sine arc
    float arcY = 28.0f + 112.0f * sinf(rad);

    if(!isNight){
        drawSun(arcX, arcY);
        // Sky colour tint changes near horizon
        if(dayAngle < 20.0f || dayAngle > 160.0f){
            // Sunrise/sunset orange sky band
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glBegin(GL_QUADS);
            glColor4f(1.0f,0.45f,0.1f,0.25f);
            glVertex3i(-25,25,0);
            glVertex3i(130,25,0);
            glColor4f(1.0f,0.3f,0.05f,0.0f);
            glVertex3i(130,80,0);
            glVertex3i(-25,80,0);
            glEnd();
            glDisable(GL_BLEND);
        }
    } else {
        drawMoon(arcX, arcY);
    }
}

// ========== REALISTIC BUS (replaces flat bus look) ==========
// Draw a more realistic bus with rounded corners using polygons, proper headlights, etc.
void realisticBus(float ox, float oy)
{
    // ox,oy = base position, tx_offset already applied by caller via pushMatrix

    float x = ox, y = oy;
    // Body
    glBegin(GL_POLYGON);
    glColor3ub(100,150,60);
    glVertex3f(x,    y+2,  0);
    glVertex3f(x,    y+12, 0);  // left back top
    glVertex3f(x+1,  y+13, 0);  // back rounded top
    glVertex3f(x+19, y+13, 0);  // front top
    glVertex3f(x+20, y+12, 0);  // front top corner
    glVertex3f(x+20, y+2,  0);  // front bottom
    glEnd();
    // Lower skirt (darker)
    glBegin(GL_QUADS);
    glColor3ub(0,120,40);
    glVertex3f(x,    y,   0);
    glVertex3f(x+20, y,   0);
    glVertex3f(x+20, y+2, 0);
    glVertex3f(x,    y+2, 0);
    glEnd();

    // Window strip
    glBegin(GL_QUADS);
    glColor3ub(180,220,245);
    glVertex3f(x+2,  y+9,  0);
    glVertex3f(x+19, y+9,  0);
    glVertex3f(x+19, y+12, 0);
    glVertex3f(x+2,  y+12, 0);
    glEnd();

    // Window dividers
    glColor3ub(0,100,30);
    for(int i=0;i<6;i++){
        glBegin(GL_LINES);
        glVertex3f(x+2+i*2.8f, y+9,  0);
        glVertex3f(x+2+i*2.8f, y+12, 0);
        glEnd();
    }

    // Front windshield
    glBegin(GL_QUADS);
    glColor3ub(160,210,240);
    glVertex3f(x+18, y+7,  0);
    glVertex3f(x+20, y+7,  0);
    glVertex3f(x+20, y+12, 0);
    glVertex3f(x+18, y+12, 0);
    glEnd();

    // Door
    glBegin(GL_QUADS);
    glColor3ub(0,140,50);
    glVertex3f(x+2, y+3, 0);
    glVertex3f(x+5, y+3, 0);
    glVertex3f(x+5, y+9, 0);
    glVertex3f(x+2, y+9, 0);
    glEnd();
    glBegin(GL_LINES);
    glColor3ub(0,80,20);
    glVertex3f(x+3.5f, y+3, 0);
    glVertex3f(x+3.5f, y+9, 0);
    glEnd();

    // Front headlight (yellow)
    glColor3ub(255,230,50);
    circleSolid(x+19.5f, y+4,  0.7f);
    // Rear light (red)
    glColor3ub(220,30,30);
    circleSolid(x+0.5f,  y+4,  0.7f);

    // Exhaust pipe
    glBegin(GL_QUADS);
    glColor3ub(80,80,80);
    glVertex3f(x+0.5f, y,    0);
    glVertex3f(x+2,    y,    0);
    glVertex3f(x+2,    y+0.8f,0);
    glVertex3f(x+0.5f, y+0.8f,0);
    glEnd();

    // Wheels (with spinning spokes)
    float centers[2] = {x+4, x+16};
    for(int w=0;w<2;w++){
        // Tire
        glColor3ub(30,30,30);
        circleSolid(centers[w], y-1, 2.0f);
        // Rim
        glColor3ub(200,200,200);
        circleSolid(centers[w], y-1, 1.2f);
        // Hub
        glColor3ub(150,150,150);
        circleSolid(centers[w], y-1, 0.4f);
        // Spokes
        glColor3ub(120,120,120);
        for(int s=0;s<5;s++){
            float a = wheelAngle*PI/180.0f + s*2*PI/5.0f;
            glBegin(GL_LINES);
            glVertex3f(centers[w],           y-1,            0);
            glVertex3f(centers[w]+1.1f*cosf(a), y-1+1.1f*sinf(a), 0);
            glEnd();
        }
    }
}



void car2()
{
    // Bus 2 � animated left, base at 8, moved by txCar2
    glPushMatrix();
    glTranslatef(txCar2, 0, 0);
    realisticBus(8, 23);
    glPopMatrix();
}

void car3()
{
    // Bus 3 � animated right-to-left, base at 108, moved by txCar3
    glPushMatrix();
    glTranslatef(txCar3, 0, 0);
    realisticBus(108, 23);
    glPopMatrix();
}

void basicColors()
{

    // Sky gradient � switches between day and night
    glBegin(GL_QUADS);
    if(!isNight){
        glColor3f(0.7f, 1.0f, 1.0f);
        glVertex3i (-25, 25, 0);
        glVertex3i (130, 25, 0);
        glColor3f(0.4f, 0.6f, 1.0f);
        glVertex3i (130, 150, 0);
        glVertex3i (-25, 150, 0);
    } else {
        glColor3f(0.05f, 0.05f, 0.15f);
        glVertex3i (-25, 25, 0);
        glVertex3i (130, 25, 0);
        glColor3f(0.02f, 0.02f, 0.10f);
        glVertex3i (130, 150, 0);
        glVertex3i (-25, 150, 0);
    }
    glEnd();

    // Night: draw stars
    if(isNight){
        glColor3ub(255,255,200);
        float starPos[20][2] = {
            {10,140},{25,135},{40,142},{55,138},{70,145},
            {85,132},{100,140},{115,136},{-10,138},{5,128},
            {30,145},{50,130},{75,140},{90,128},{120,142},
            {-5,130},{45,148},{80,135},{60,142},{35,128}
        };
        for(int i=0;i<20;i++){
            circleSolid(starPos[i][0], starPos[i][1], 0.25f);
        }
    }


    glBegin(GL_QUADS);  // between p16 and p18
    glColor3ub(153, 0, 0);
    glVertex3i (34, 31, 0);
    glVertex3i (54, 31, 0);
    glVertex3i (54, 58, 0);
    glVertex3i (34, 58, 0);
    glEnd();

    glBegin(GL_QUADS);  // between p16 and p29
    glColor3ub(153, 0, 0);
    glVertex3i (55, 31, 0);
    glVertex3i (70, 31, 0);
    glVertex3i (70, 45, 0);
    glVertex3i (55, 46, 0);
    glEnd();

    glBegin(GL_QUADS);  // between p30 and p31
    glColor3ub(153, 0, 0);
    glVertex3i (70, 31, 0);
    glVertex3i (75, 31, 0);
    glVertex3i (75, 35, 0);
    glVertex3i (70, 35, 0);
    glEnd();

    glBegin(GL_QUADS);  // between p6 and p18
    glColor3ub(51, 0, 0);
    glVertex3i (24, 30, 0);
    glVertex3i (34, 30, 0);
    glVertex3i (33, 59, 0);
    glVertex3i (24, 63, 0);
    glEnd();


    glBegin(GL_QUADS);  // between p6 and p26
    glColor3ub(153, 0, 0);
    glVertex3i (24, 30, 0);
    glVertex3i (30, 30, 0);
    glVertex3i (30, 55, 0);
    glVertex3i (24, 55, 0);
    glEnd();

    glBegin(GL_QUADS);  // between p25 and p26
    glColor3ub(0, 102, 102);
    glVertex3i (30, 30, 0);
    glVertex3i (31, 30, 0);
    glVertex3i (31, 54, 0);
    glVertex3i (30, 55, 0);
    glEnd();

    glBegin(GL_QUADS);  // between p16 extra and p28 extra
    glColor3ub(153, 0, 0);
    glVertex3i (55, 45, 0);
    glVertex3i (65, 45, 0);
    glVertex3i (65, 50, 0);
    glVertex3i (55, 50, 0);
    glEnd();

    glBegin(GL_QUADS);  // middle gate
    glColor3ub(255, 255, 204);
    glVertex3i (45, 25, 0);
    glVertex3i (54, 25, 0);
    glVertex3i (54, 30, 0);
    glVertex3i (45, 30, 0);
    glEnd();

    glBegin(GL_TRIANGLES);  // left side field of road
    glColor3ub(0, 204, 0);
    glVertex3i (2, 20, 0);
    glVertex3i (-25, 20, 0);
    glVertex3i (-25, -25, 0);
    glEnd();
}


/// ==================== AB-01  START =====================
void sideLines()
{
    glBegin(GL_LINES);  // 2nd floors >>>>
    glColor3ub(0,0,0);
    glVertex3f(70, 37, 0);
    glVertex3f(75, 37, 0);
    glEnd();

    glBegin(GL_LINES);
    glColor3ub(0,0,0);
    glVertex3f(70, 36, 0);
    glVertex3f(70, 37, 0);
    glEnd();

    glBegin(GL_LINES);
    glColor3ub(0,0,0);
    glVertex3f(71, 36, 0);
    glVertex3f(71, 37, 0);
    glEnd();

    glBegin(GL_LINES);
    glColor3ub(0,0,0);
    glVertex3f(72, 36, 0);
    glVertex3f(72, 37, 0);
    glEnd();

    glBegin(GL_LINES);
    glColor3ub(0,0,0);
    glVertex3f(73, 36, 0);
    glVertex3f(73, 37, 0);
    glEnd();

    glBegin(GL_LINES);
    glColor3ub(0,0,0);
    glVertex3f(74, 36, 0);
    glVertex3f(74, 37, 0);
    glEnd();

    glBegin(GL_LINES);  // 2nd floor's <<<<
    glColor3ub(0,0,0);
    glVertex3f(75, 36, 0);
    glVertex3f(75, 37, 0);
    glEnd();

    glBegin(GL_LINES); //1st floor >>>>
    glColor3ub(0,0,0);
    glVertex3f(76, 32, 0);
    glVertex3f(80, 32, 0);
    glEnd();

    glBegin(GL_LINES);
    glColor3ub(0,0,0);
    glVertex3f(76, 31, 0);
    glVertex3f(76, 32, 0);
    glEnd();

    glBegin(GL_LINES);
    glColor3ub(0,0,0);
    glVertex3f(77, 31, 0);
    glVertex3f(77, 32, 0);
    glEnd();

    glBegin(GL_LINES);
    glColor3ub(0,0,0);
    glVertex3f(78, 31, 0);
    glVertex3f(78, 32, 0);
    glEnd();

    glBegin(GL_LINES);
    glColor3ub(0,0,0);
    glVertex3f(79, 31, 0);
    glVertex3f(79, 32, 0);
    glEnd();

    glBegin(GL_LINES);
    glColor3ub(0,0,0);
    glVertex3f(80, 31, 0);
    glVertex3f(80, 32, 0);
    glEnd();
}

void windowLines()
{
    glBegin(GL_LINES); //p8's lines ======== >>>
    glColor3ub(0,0,0);
    glVertex3f(15.5, 31, 0);
    glVertex3f(15.5, 34, 0);
    glEnd();

    glBegin(GL_LINES);
    glColor3ub(0,0,0);
    glVertex3i(16, 31, 0);
    glVertex3i(16, 34, 0);
    glEnd();

    glBegin(GL_LINES);
    glColor3ub(0,0,0);
    glVertex3f(16.5, 31, 0);
    glVertex3f(16.5, 34, 0);
    glEnd();

    glBegin(GL_LINES);
    glColor3ub(0,0,0);
    glVertex3i(17, 31, 0);
    glVertex3i(17, 34, 0);
    glEnd();

    glBegin(GL_LINES); //p8's lines =========== <<<
    glColor3ub(0,0,0);
    glVertex3f(17.5, 31, 0);
    glVertex3f(17.5, 34, 0);
    glEnd();

    glBegin(GL_LINES); //p9's lines ======== >>>
    glColor3ub(0,0,0);
    glVertex3f(15.5, 37, 0);
    glVertex3f(15.5, 40, 0);
    glEnd();

    glBegin(GL_LINES);
    glColor3ub(0,0,0);
    glVertex3i(16, 37, 0);
    glVertex3i(16, 40, 0);
    glEnd();

    glBegin(GL_LINES);
    glColor3ub(0,0,0);
    glVertex3f(16.5, 37, 0);
    glVertex3f(16.5, 40, 0);
    glEnd();

    glBegin(GL_LINES);
    glColor3ub(0,0,0);
    glVertex3i(17, 37, 0);
    glVertex3i(17, 40, 0);
    glEnd();

    glBegin(GL_LINES); //p9's lines ===========  <<<
    glColor3ub(0,0,0);
    glVertex3f(17.5, 37, 0);
    glVertex3f(17.5, 40, 0);
    glEnd();

    glBegin(GL_LINES); //p10's lines ======== >>>
    glColor3ub(0,0,0);
    glVertex3f(15.5, 42, 0);
    glVertex3f(15.5, 45, 0);
    glEnd();

    glBegin(GL_LINES);
    glColor3ub(0,0,0);
    glVertex3i(16, 42, 0);
    glVertex3i(16, 45, 0);
    glEnd();

    glBegin(GL_LINES);
    glColor3ub(0,0,0);
    glVertex3f(16.5, 42, 0);
    glVertex3f(16.5, 45, 0);
    glEnd();

    glBegin(GL_LINES);
    glColor3ub(0,0,0);
    glVertex3i(17, 42, 0);
    glVertex3i(17, 45, 0);
    glEnd();

    glBegin(GL_LINES); //p10's lines ===========  <<<
    glColor3ub(0,0,0);
    glVertex3f(17.5, 42, 0);
    glVertex3f(17.5, 45, 0);
    glEnd();

    glBegin(GL_LINES); //p11's lines ======== >>>
    glColor3ub(0,0,0);
    glVertex3f(15.5, 47, 0);
    glVertex3f(15.5, 50.5, 0);
    glEnd();

    glBegin(GL_LINES);
    glColor3ub(0,0,0);
    glVertex3i(16, 47, 0);
    glVertex3f(16, 50.5, 0);
    glEnd();

    glBegin(GL_LINES);
    glColor3ub(0,0,0);
    glVertex3f(16.5, 47, 0);
    glVertex3f(16.5, 50.5, 0);
    glEnd();

    glBegin(GL_LINES);
    glColor3ub(0,0,0);
    glVertex3i(17, 47, 0);
    glVertex3f(17, 50.5, 0);
    glEnd();

    glBegin(GL_LINES); //p11's lines ===========  <<<
    glColor3ub(0,0,0);
    glVertex3f(17.5, 47, 0);
    glVertex3f(17.5, 50.5, 0);
    glEnd();

    glBegin(GL_LINES); //p12's lines ======== >>>
    glColor3ub(0,0,0);
    glVertex3f(15.5, 52, 0);
    glVertex3f(15.5, 55.5, 0);
    glEnd();

    glBegin(GL_LINES);
    glColor3ub(0,0,0);
    glVertex3i(16, 52, 0);
    glVertex3f(16, 55.5, 0);
    glEnd();

    glBegin(GL_LINES);
    glColor3ub(0,0,0);
    glVertex3f(16.5, 52, 0);
    glVertex3f(16.5, 55.5, 0);
    glEnd();

    glBegin(GL_LINES);
    glColor3ub(0,0,0);
    glVertex3i(17, 52, 0);
    glVertex3f(17, 55.5, 0);
    glEnd();

    glBegin(GL_LINES); //p12's lines ===========  <<<
    glColor3ub(0,0,0);
    glVertex3f(17.5, 52, 0);
    glVertex3f(17.5, 55.5, 0);
    glEnd();

    glBegin(GL_LINES); //p12's extra lines ======== >>>
    glColor3ub(0,0,0);
    glVertex3f(15.5, 57, 0);
    glVertex3f(15.5, 60.5, 0);
    glEnd();

    glBegin(GL_LINES);
    glColor3ub(0,0,0);
    glVertex3i(16, 57, 0);
    glVertex3f(16, 60.5, 0);
    glEnd();

    glBegin(GL_LINES);
    glColor3ub(0,0,0);
    glVertex3f(16.5, 57, 0);
    glVertex3f(16.5, 60.5, 0);
    glEnd();

    glBegin(GL_LINES);
    glColor3ub(0,0,0);
    glVertex3i(17, 57, 0);
    glVertex3f(17, 60.5, 0);
    glEnd();

    glBegin(GL_LINES); //p12's extra lines ===========  <<<
    glColor3ub(0,0,0);
    glVertex3f(17.5, 57, 0);
    glVertex3f(17.5, 60.5, 0);
    glEnd();

}

void gateLines()
{
    glBegin(GL_LINES); //p1's upper line
    glColor3ub(0,0,0);
    glVertex3i(8, 30, 0);
    glVertex3i(44, 30, 0);
    glEnd();

    glBegin(GL_LINES); //p1's right line
    glColor3ub(0,0,0);
    glVertex3i(44, 24, 0);
    glVertex3i(44, 30, 0);
    glEnd();

    glBegin(GL_LINES); //p2's upper line
    glColor3ub(0,0,0);
    glVertex3i(55, 30, 0);
    glVertex3i(85, 30, 0);
    glEnd();

    glBegin(GL_LINES); //p2's left line
    glColor3ub(0,0,0);
    glVertex3i(55, 24, 0);
    glVertex3i(55, 30, 0);
    glEnd();


    glBegin(GL_LINES); //gate head line =========
    glColor3ub(0,0,0);
    glVertex3i(45, 25, 0);
    glVertex3i(45, 30, 0);
    glEnd();

    glBegin(GL_LINES); //gate head line 2
    glColor3ub(0,0,0);
    glVertex3i(45, 30, 0);
    glVertex3i(46, 31, 0);
    glEnd();

    glBegin(GL_LINES); //gate head line 3
    glColor3ub(0,0,0);
    glVertex3i(46, 31, 0);
    glVertex3i(45, 32, 0);
    glEnd();

    glBegin(GL_LINES); //gate head line 4
    glColor3ub(0,0,0);
    glVertex3i(45, 32, 0);
    glVertex3i(44, 32, 0);
    glEnd();

    glBegin(GL_LINES); //gate head line 5
    glColor3ub(0,0,0);
    glVertex3i(44, 32, 0);
    glVertex3i(43, 31, 0);
    glEnd();

    glBegin(GL_LINES); //gate head line 6
    glColor3ub(0,0,0);
    glVertex3i(43, 31, 0);
    glVertex3i(44, 30, 0);
    glEnd();

    glBegin(GL_LINES); //gate head line ========
    glColor3ub(0,0,0);
    glVertex3i(54, 25, 0);
    glVertex3i(54, 30, 0);
    glEnd();


    glBegin(GL_LINES); //gate 2 head line 2
    glColor3ub(0,0,0);
    glVertex3i(55, 30, 0);
    glVertex3i(56, 31, 0);
    glEnd();

    glBegin(GL_LINES); //gate 2 head line 3
    glColor3ub(0,0,0);
    glVertex3i(56, 31, 0);
    glVertex3i(55, 32, 0);
    glEnd();

    glBegin(GL_LINES); //gate 2 head line 4
    glColor3ub(0,0,0);
    glVertex3i(55, 32, 0);
    glVertex3i(54, 32, 0);
    glEnd();

    glBegin(GL_LINES); //gate 2 head line 5
    glColor3ub(0,0,0);
    glVertex3i(54, 32, 0);
    glVertex3i(53, 31, 0);
    glEnd();

    glBegin(GL_LINES); //gate 2 head line 6
    glColor3ub(0,0,0);
    glVertex3i(53, 31, 0);
    glVertex3i(54, 30, 0);
    glEnd();

    //====== middle part of Gate >>

    glBegin(GL_LINES); // hor
    glColor3ub(0,0,0);
    glVertex3i(45, 30, 0);
    glVertex3i(54, 30, 0);
    glEnd();

    glBegin(GL_LINES); //ver
    glColor3ub(0,0,0);
    glVertex3f(45.5, 30, 0);
    glVertex3f(45.5, 25, 0);
    glEnd();

    glBegin(GL_LINES); //ver
    glColor3ub(0,0,0);
    glVertex3i(46, 30, 0);
    glVertex3i(46, 25, 0);
    glEnd();

    glBegin(GL_LINES); //ver
    glColor3ub(0,0,0);
    glVertex3f(46.5, 30, 0);
    glVertex3f(46.5, 25, 0);
    glEnd();

    glBegin(GL_LINES); //ver
    glColor3ub(0,0,0);
    glVertex3i(47, 30, 0);
    glVertex3i(47, 25, 0);
    glEnd();

    glBegin(GL_LINES); //ver
    glColor3ub(0,0,0);
    glVertex3f(47.5, 30, 0);
    glVertex3f(47.5, 25, 0);
    glEnd();

    glBegin(GL_LINES); //ver
    glColor3ub(0,0,0);
    glVertex3i(48, 30, 0);
    glVertex3i(48, 25, 0);
    glEnd();

    glBegin(GL_LINES); //ver
    glColor3ub(0,0,0);
    glVertex3f(48.5, 30, 0);
    glVertex3f(48.5, 25, 0);
    glEnd();

    glBegin(GL_LINES); //ver
    glColor3ub(0,0,0);
    glVertex3i(49, 30, 0);
    glVertex3i(49, 25, 0);
    glEnd();

    glBegin(GL_LINES); //ver
    glColor3ub(0,0,0);
    glVertex3f(49.5, 30, 0);
    glVertex3f(49.5, 25, 0);
    glEnd();

    glBegin(GL_LINES); //ver
    glColor3ub(0,0,0);
    glVertex3i(50, 30, 0);
    glVertex3i(50, 25, 0);
    glEnd();

    glBegin(GL_LINES); //ver
    glColor3ub(0,0,0);
    glVertex3f(50.5, 30, 0);
    glVertex3f(50.5, 25, 0);
    glEnd();

    glBegin(GL_LINES); //ver
    glColor3ub(0,0,0);
    glVertex3i(51, 30, 0);
    glVertex3i(51, 25, 0);
    glEnd();

    glBegin(GL_LINES); //ver
    glColor3ub(0,0,0);
    glVertex3f(51.5, 30, 0);
    glVertex3f(51.5, 25, 0);
    glEnd();

    glBegin(GL_LINES); //ver
    glColor3ub(0,0,0);
    glVertex3i(52, 30, 0);
    glVertex3i(52, 25, 0);
    glEnd();

    glBegin(GL_LINES); //ver
    glColor3ub(0,0,0);
    glVertex3f(52.5, 30, 0);
    glVertex3f(52.5, 25, 0);
    glEnd();

    glBegin(GL_LINES); //ver
    glColor3ub(0,0,0);
    glVertex3i(53, 30, 0);
    glVertex3i(53, 25, 0);
    glEnd();

    glBegin(GL_LINES); //ver
    glColor3ub(0,0,0);
    glVertex3f(53.5, 30, 0);
    glVertex3f(53.5, 25, 0);
    glEnd();

    glBegin(GL_LINES); //dia
    glColor3ub(0,0,0);
    glVertex3i(45, 30, 0);
    glVertex3i(54, 25, 0);
    glEnd();

    glBegin(GL_LINES); //dia
    glColor3ub(0,0,0);
    glVertex3i(45, 25, 0);
    glVertex3i(54, 30, 0);
    glEnd();
}

void abOne()
{

    glBegin(GL_QUADS);  // p3
    glColor3ub(51, 0, 0);
    glVertex3i (10, 30, 0);
    glVertex3i (20, 30, 0);
    glVertex3i (20, 64, 0);
    glVertex3i (10, 64, 0);
    glEnd();

    glBegin(GL_QUADS);  // p4
    glColor3ub(224, 224, 224);
    glVertex3i (10, 30, 0);
    glVertex3i (11, 30, 0);
    glVertex3i (11, 64, 0);
    glVertex3i (10, 64, 0);
    glEnd();

    glBegin(GL_LINES); //line of p4
    glColor3ub(0, 0, 0);
    glVertex3f(10, 30, 0);
    glVertex3f(10, 65, 0);
    glEnd();

    glBegin(GL_QUADS);  // p5
    glColor3ub(224, 224, 224);
    glVertex3i (20, 30, 0);
    glVertex3i (22, 30, 0);
    glVertex3i (22, 64, 0);
    glVertex3i (20, 64, 0);
    glEnd();

    glBegin(GL_QUADS);  // p6
    glColor3ub(192, 192, 192);
    glVertex3i (22, 30, 0);
    glVertex3i (24, 30, 0);
    glVertex3i (24, 61, 0);
    glVertex3i (22, 65, 0);
    glEnd();


    glBegin(GL_QUADS);  // p7
    glColor3ub(224, 224, 224);
    glVertex3i (10, 64, 0);
    glVertex3i (22, 64, 0);
    glVertex3i (22, 65, 0);
    glVertex3i (10, 65, 0);
    glEnd();


    glBegin(GL_LINES); //line of p7
    glColor3ub(0, 0, 0);
    glVertex3f(10, 65, 0);
    glVertex3f(22, 65, 0);
    glEnd();

    glBegin(GL_QUADS);  // p8
    glColor3ub(192, 192, 192);
    glVertex3i (15, 31, 0);
    glVertex3i (18, 31, 0);
    glVertex3i (18, 34, 0);
    glVertex3i (15, 34, 0);
    glEnd();

    glBegin(GL_QUADS);  // p9
    glColor3ub(192, 192, 192);
    glVertex3i (15, 37, 0);
    glVertex3i (18, 37, 0);
    glVertex3i (18, 40, 0);
    glVertex3i (15, 40, 0);
    glEnd();

    glBegin(GL_QUADS);  // p10
    glColor3ub(192, 192, 192);
    glVertex3i (15, 42, 0);
    glVertex3i (18, 42, 0);
    glVertex3i (18, 45, 0);
    glVertex3i (15, 45, 0);
    glEnd();

    glBegin(GL_QUADS);  // p11
    glColor3ub(192, 192, 192);
    glVertex3i (15, 47, 0);
    glVertex3i (18, 47, 0);
    glVertex3i (18, 50, 0);
    glVertex3i (15, 50, 0);
    glEnd();

    glBegin(GL_QUADS);  // p12
    glColor3ub(192, 192, 192);
    glVertex3i (15, 52, 0);
    glVertex3i (18, 52, 0);
    glVertex3i (18, 55, 0);
    glVertex3i (15, 55, 0);
    glEnd();

    glBegin(GL_QUADS);  // p12 extra
    glColor3ub(192, 192, 192);
    glVertex3i (15, 57, 0);
    glVertex3i (18, 57, 0);
    glVertex3i (18, 60, 0);
    glVertex3i (15, 60, 0);
    glEnd();


    glBegin(GL_QUADS);  // p13
    glColor3ub(192, 192, 192);
    glVertex3i (24, 61, 0);
    glVertex3i (34, 58, 0);
    glVertex3i (33, 61, 0);
    glVertex3i (22, 65, 0);
    glEnd();

    glBegin(GL_LINES); //line of p13
    glColor3ub(0, 0, 0);
    glVertex3f(33, 61, 0);
    glVertex3f(22, 65, 0);
    glEnd();

    glBegin(GL_LINES); //line of p5 and 6
    glColor3ub(0, 0, 0);
    glVertex3f(22, 30, 0);
    glVertex3f(22, 65, 0);
    glEnd();


    glBegin(GL_QUADS);  // p14
    glColor3ub(224, 224, 224);
    glVertex3i (34, 58, 0);
    glVertex3i (55, 58, 0);
    glVertex3i (55, 61, 0);
    glVertex3i (33, 61, 0);
    glEnd();

    glBegin(GL_LINES); //line of p14
    glColor3ub(0, 0, 0);
    glVertex3f(33, 61, 0);
    glVertex3f(55, 61, 0);
    glEnd();


    glBegin(GL_QUADS);  // p15
    glColor3ub(192, 192, 192);
    glVertex3i (55, 50, 0);
    glVertex3i (61, 50, 0);
    glVertex3i (61, 58, 0);
    glVertex3i (55, 61, 0);
    glEnd();

    glBegin(GL_LINES); //line of p15
    glColor3ub(0, 0, 0);
    glVertex3f(55, 61, 0);
    glVertex3f(61, 58, 0);
    glEnd();

    glBegin(GL_LINES); //line of p15
    glColor3ub(0, 0, 0);
    glVertex3f(61, 58, 0);
    glVertex3f(61, 50, 0);
    glEnd();

    glBegin(GL_QUADS);  // p16
    glColor3ub(224, 224, 224);
    glVertex3i (54, 30, 0);
    glVertex3i (55, 30, 0);
    glVertex3i (55, 58, 0);
    glVertex3i (54, 58, 0);
    glEnd();


    glBegin(GL_QUADS);  // p17
    glColor3ub(224, 224, 224);
    glVertex3i (44, 30, 0);
    glVertex3i (45, 30, 0);
    glVertex3i (45, 58, 0);
    glVertex3i (44, 58, 0);
    glEnd();

    glBegin(GL_QUADS);  // p18
    glColor3ub(224, 224, 224);
    glVertex3i (33, 30, 0);
    glVertex3i (34, 30, 0);
    glVertex3i (34, 58, 0);
    glVertex3i (33, 61, 0);
    glEnd();

    glBegin(GL_QUADS);  // p19
    glColor3ub(224, 224, 224);
    glVertex3i (24, 45, 0);
    glVertex3i (54, 45, 0);
    glVertex3i (54, 46, 0);
    glVertex3i (24, 46, 0);
    glEnd();

    glBegin(GL_QUADS);  // p19 extra
    glColor3ub(224, 224, 224);
    glVertex3i (24, 50, 0);
    glVertex3i (54, 50, 0);
    glVertex3i (54, 51, 0);
    glVertex3i (24, 51, 0);
    glEnd();

    glBegin(GL_QUADS);  // p20
    glColor3ub(224, 224, 224);
    glVertex3i (24, 40, 0);
    glVertex3i (70, 40, 0);
    glVertex3i (70, 41, 0);
    glVertex3i (24, 41, 0);
    glEnd();

    glBegin(GL_QUADS);  // p21
    glColor3ub(224, 224, 224);
    glVertex3i (24, 35, 0);
    glVertex3i (81, 35, 0);
    glVertex3i (80, 36, 0);
    glVertex3i (24, 36, 0);
    glEnd();

    glBegin(GL_QUADS);  // p22
    glColor3ub(224, 224, 224);
    glVertex3i (24, 30, 0);
    glVertex3i (85, 30, 0);
    glVertex3i (85, 31, 0);
    glVertex3i (24, 31, 0);
    glEnd();

    glBegin(GL_QUADS);  // p23
    glColor3ub(224, 224, 224);
    glVertex3i (24, 55, 0);
    glVertex3i (30, 55, 0);
    glVertex3i (30, 56, 0);
    glVertex3i (24, 56, 0);
    glEnd();

    glBegin(GL_QUADS);  // p24
    glColor3ub(192, 192, 192);
    glVertex3i (30, 55, 0);
    glVertex3i (31, 54, 0);
    glVertex3f (31.5, 54, 0);
    glVertex3i (30, 56, 0);
    glEnd();

    glBegin(GL_QUADS);  // p25
    glColor3ub(224, 224, 224);
    glVertex3i (31, 30, 0);
    glVertex3f(31.5, 30, 0);
    glVertex3f(31.5, 54, 0);
    glVertex3i (31, 54, 0);
    glEnd();

    glBegin(GL_QUADS);  // p26
    glColor3ub(224, 224, 224);
    glVertex3i (29, 30, 0);
    glVertex3i (30, 30, 0);
    glVertex3i (30, 55, 0);
    glVertex3i (29, 55, 0);
    glEnd();


    glBegin(GL_QUADS);  // p28
    glColor3ub(224, 224, 224);
    glVertex3i (51, 45, 0);
    glVertex3i (70, 45, 0);
    glVertex3i (69, 46, 0);
    glVertex3i (51, 46, 0);
    glEnd();

    glBegin(GL_QUADS);  // p28 extra
    glColor3ub(224, 224, 224);
    glVertex3i (61, 50, 0);
    glVertex3i (66, 50, 0);
    glVertex3i (65, 51, 0);
    glVertex3i (61, 51, 0);
    glEnd();

    glBegin(GL_QUADS);  // p29
    glColor3ub(160, 160, 160);
    glVertex3i (69, 36, 0);
    glVertex3i (70, 36, 0);
    glVertex3i (70, 45, 0);
    glVertex3i (69, 45, 0);
    glEnd();

    glBegin(GL_QUADS);  // p29 extra
    glColor3ub(160, 160, 160);
    glVertex3i (65, 46, 0);
    glVertex3i (66, 46, 0);
    glVertex3i (66, 50, 0);
    glVertex3i (65, 50, 0);
    glEnd();

    glBegin(GL_QUADS);  // p30
    glColor3ub(160, 160, 160);
    glVertex3i (75, 31, 0);
    glVertex3i (76, 31, 0);
    glVertex3i (76, 39, 0);
    glVertex3i (75, 39, 0);
    glEnd();

    glBegin(GL_QUADS);  // p31
    glColor3ub(160, 160, 160);
    glVertex3i (80, 31, 0);
    glVertex3i (81, 31, 0);
    glVertex3i (81, 35, 0);
    glVertex3i (80, 35, 0);
    glEnd();

    glBegin(GL_QUADS);  // p32
    glColor3ub(224, 224, 224);
    glVertex3i (70, 39, 0);
    glVertex3i (76, 39, 0);
    glVertex3i (75, 40, 0);
    glVertex3i (70, 40, 0);
    glEnd();

    glBegin(GL_QUADS);  // gate pl 1
    glColor3ub(96, 96, 96);
    glVertex3i (44, 25, 0);
    glVertex3i (45, 25, 0);
    glVertex3i (45, 30, 0);
    glVertex3i (44, 30, 0);
    glEnd();

    glBegin(GL_QUADS);  // gate pl 1
    glColor3ub(96, 96, 96);
    glVertex3i (54, 25, 0);
    glVertex3i (55, 25, 0);
    glVertex3i (55, 30, 0);
    glVertex3i (54, 30, 0);
    glEnd();

    glBegin(GL_POLYGON); //gate's head 1
    glColor3ub(96, 96, 96);
    glVertex3i(44, 30, 0);
    glVertex3i(45, 30, 0);
    glVertex3i(46, 31, 0);
    glVertex3i(45, 32, 0);
    glVertex3i(44, 32, 0);
    glVertex3i(43, 31, 0);
    glEnd();

    glBegin(GL_POLYGON); //gate's head 2
    glColor3ub(96, 96, 96);
    glVertex3i(54, 30, 0);
    glVertex3i(55, 30, 0);
    glVertex3i(56, 31, 0);
    glVertex3i(55, 32, 0);
    glVertex3i(54, 32, 0);
    glVertex3i(53, 31, 0);
    glEnd();


    bigTree();
    mediumTree();


    glBegin(GL_QUADS);  // p1
    glColor3ub(102, 102,0);
    glVertex3i (8, 25, 0);
    glVertex3i (45, 25, 0);
    glVertex3i (45, 30, 0);
    glVertex3i (8, 30, 0);
    glEnd();

    glBegin(GL_QUADS);  // p2
    glColor3ub(102, 102,0);
    glVertex3i (54, 25, 0);
    glVertex3i (87, 25, 0);
    glVertex3i (87, 30, 0);
    glVertex3i (54, 30, 0);
    glEnd();


// ==================== AB-01  END =====================

}

//==================== AB-01  END =====================

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    basicColors();
    sunMoon();       // animated sun or moon
    abOne();
    windowLines();
    sideLines();
    gateLines();
    mosque();
    guardroom();
    innovationLab();
    road();
    playGround();
    midCircle();
    LDboxCircle();
    RDboxCircle();

    plane();
    cloud();

    allBirds();      // animated bird flock
    bangladeshFlag(); // waving BD flag on building

    grass(-20, 15);
    grass(-15, 13);
    grass(-10, 14);
    grass(-19, 8);
    grass(-24, 7);
    grass(-13, 7);
    grass(-17, 3);
    grass(-14, -1);
    grass(-20, -4);
    grass(-5, 15);
    grass(-7, 9);
    grass(-22, 1);
    grass(-22, -9);

    mediumTree2();

    tree(0, 25);
    tree2(3, 35, 0, 51, 0);
    tree(6, 25);

    tree(125, 25);
    tree2(118, 35, 0, 102, 0);
    tree(114, 25);

    human();
    human4();

    // Group 1 � walking near gate area
    for(int i=0;i<4;i++)
        humanDy(hX[i], hY[i], hShirtR[i], hShirtG[i], hShirtB[i], hDir[i]);

    bushGrass(-23, 27);


    car2();
    car3();

    // Group 2 � walking near building
    for(int i=0;i<2;i++)
        humanDy(h2X[i], h2Y[i], h2ShirtR[i], h2ShirtG[i], h2ShirtB[i], h2Dir[i]);

    tree2(-3, 30, 0, 51, 0);

    bushGrass(-10, 5);
    bushGrass(-3, 15);
    bushGrass(-17, -5);
    tree(-25, 0);

    bushGrass(-10, 19);
    bushGrass(-20, 19);
    bushGrass(10, 19);
    bushGrass(20, 19);
    bushGrass(30, 19);
    bushGrass(40, 19);
    bushGrass(50, 19);
    bushGrass(60, 19);
    bushGrass(70, 19);
    bushGrass(80, 19);
    bushGrass(90, 19);
    bushGrass(100, 19);
    bushGrass(110, 19);
    bushGrass(120, 19);
    bushGrass(128, 19);

    // Night overlay: semi-transparent dark tint on ground scene
    if(isNight){
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBegin(GL_QUADS);
        glColor4f(0.0f, 0.0f, 0.15f, 0.45f);
        glVertex3i(-25,-25,0);
        glVertex3i(200,-25,0);
        glVertex3i(200,150,0);
        glVertex3i(-25,150,0);
        glEnd();
        glDisable(GL_BLEND);

        // Street lights glow
        float lampX[4]={15,40,70,100};
        for(int i=0;i<4;i++){
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glColor4f(1.0f,0.9f,0.5f,0.25f);
            circleSolid(lampX[i], 24.0f, 5.0f);
            glDisable(GL_BLEND);
            glColor3ub(255,230,100);
            circleSolid(lampX[i], 24.0f, 0.6f);
        }
    }

    glutSwapBuffers();
}
void init (void)
{
    glClearColor (0, 0, 0, 0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-25, 130, -25, 150, -10.0, 10.0);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_BLEND); // disable by default, enable only when needed
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize (1080, 720);
    glutInitWindowPosition (100, 100);
    glutCreateWindow ("Computer Graphics - DIU Campus");
    init ();

    glutDisplayFunc(display);
    glutKeyboardFunc(object_control);

    glutMainLoop();
    return 0;
}
