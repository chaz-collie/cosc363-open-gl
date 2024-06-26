#include <iostream>
#include <cmath> 
#include <GL/freeglut.h>
#include "loadTGA.h"
using namespace std;

GLUquadricObj*	q;
GLuint txId[2];

float theta = 0.0f;
float r_move = 0.0f;
float angle=0, look_x, look_z=-1., eye_x, eye_z;
float red = 1.0f;
float green = 0.0f;
float blue = 0.0f;
float alienY = 0;
float ufoY = 0.0f;
float ufoLaunchY = 25.0f;
float legAngle = 0.0f;
float armAngle = 0.0f;
float sunY = 0;
float sunX = -25;

bool isLaunching = false;
bool increasing = true;
bool jump = false;

void loadTexture(void)				
{
	glGenTextures(3, txId); 	// Create 2 texture ids
	glBindTexture(GL_TEXTURE_2D, txId[0]);  //Use this texture
    loadTGA("floor.tga");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	//Set texture parameters
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	

	glBindTexture(GL_TEXTURE_2D, txId[1]);  //Use this texture
    loadTGA("sky1.tga");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	//Set texture parameters
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	

	glBindTexture(GL_TEXTURE_2D, txId[3]);  //Use this texture
    loadTGA("sun.tga");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	//Set texture parameters
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	

	glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
}

void updateUFOPosition(int value) //launches the ufo
{
    if (ufoY < ufoLaunchY) {
        ufoY += 0.1f;
        glutPostRedisplay();
        glutTimerFunc(50, updateUFOPosition, 0);
    } else {
        isLaunching = false;
    }
}

void launchUFO() //sets bool so known ufo is launching
{
    isLaunching = true;
    glutTimerFunc(50, updateUFOPosition, 0);
}

void updateColor(int value) //chnages the colour values to chnage colours of ufo lights
{
    if (increasing) {
        red += 0.01f;
        if (red >= 1.0f) {
            red = 0.1f;
            increasing = false;
        }
    } else {
        red -= 0.01f;
        if (red <= 0.0f) {
            red = 0.1f;
            increasing = true;
        }
    }
    glutPostRedisplay();
}


void move() //moves the alien
{
    if(isLaunching){
        if(jump) {
            alienY += 0.01f;
            if (alienY > 1.0f){
                jump = false;
            }
        }else {
            alienY -= 0.02f;
            if (alienY < 0.0f) {
                jump = true;
            }
        }
    
    } else {
        alienY = 0;
        theta += 0.1f;
        armAngle = 25.0f * sin(theta * 0.4f);
        legAngle = 30.0f * sin(theta * 0.4f);
        if (theta == 360.f){
            theta = 0;
        }
    }
    glutPostRedisplay(); // Update the display
}

void sunPath() //parabolic path the sun follows in the sky
{   if (sunX < 37){
    sunX += 0.01f;
    sunY = -(1.0f / 32.0f) * sunX * sunX  + 40.0f;

    } else {
        sunX = -37;
    }

    glutPostRedisplay();
}

void floor()//makes a floor with given texture
{
    float size = 200.0f;
    glColor3f(1, 1, 1);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, txId[0]);

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, size);
    glVertex3f(-size / 2, -1, -size / 2);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-size / 2, -1, size / 2);
    glTexCoord2f(size,  0.0);
    glVertex3f(size / 2, -1, size / 2);
    glTexCoord2f(size, size);
    glVertex3f(size / 2, -1, -size / 2);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

void skydome() //makes a skydome
{
    float radius = 150.0f;
    int slices = 30;
    int stacks = 30;

    glColor3f(1, 1, 1);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, txId[1]);

    glBegin(GL_QUADS);
    for (int i = 0; i < slices; ++i) {
        for (int j = 0; j < stacks; ++j) {
            float theta1 = (float)i / (float)slices * 2.0f * M_PI;
            float theta2 = (float)(i + 1) / (float)slices * 2.0f * M_PI;
            float phi1 = (float)j / (float)stacks * M_PI;
            float phi2 = (float)(j + 1) / (float)stacks * M_PI;
            float x1 = radius * sin(theta1) * sin(phi1);
            float y1 = radius * cos(phi1);
            float z1 = radius * cos(theta1) * sin(phi1);
            float x2 = radius * sin(theta2) * sin(phi1);
            float y2 = radius * cos(phi1);
            float z2 = radius * cos(theta2) * sin(phi1);
            float x3 = radius * sin(theta2) * sin(phi2);
            float y3 = radius * cos(phi2);
            float z3 = radius * cos(theta2) * sin(phi2);
            float x4 = radius * sin(theta1) * sin(phi2);
            float y4 = radius * cos(phi2);
            float z4 = radius * cos(theta1) * sin(phi2);
            float s1 = (float)i / (float)slices;
            float t1 = (float)j / (float)stacks;
            float s2 = (float)(i + 1) / (float)slices;
            float t2 = (float)(j + 1) / (float)stacks;
            glTexCoord2f(s1, t1); glVertex3f(x1, y1, z1);
            glTexCoord2f(s2, t1); glVertex3f(x2, y2, z2);
            glTexCoord2f(s2, t2); glVertex3f(x3, y3, z3);
            glTexCoord2f(s1, t2); glVertex3f(x4, y4, z4);
        }
    }
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

void wave() //makes a parbola with quads, used as launchpad
{
    glColor3f(0.5f, 0.5f, 0.5f);
    float step = 0.2f; 
    float xStart = -4.0f;
    float xEnd = 4.0f;
    float yStart = -4.0f;
    float yEnd = 4.0f;

    glBegin(GL_QUADS);
    for (float x = xStart; x < xEnd; x += step) {
        for (float y = yStart; y < yEnd; y += step) {
            glVertex3f(x, -(x * x * 0.05), y); 
            glVertex3f(x + step, -((x + step) * (x + step) * 0.05f), y);
            glVertex3f(x + step, -((x + step) * (x + step) * 0.05f), y + step);
            glVertex3f(x, -(x * x * 0.05f), y + step);
        }
    }
    glEnd();
}

void ufo() // ufo model
{	
	glTranslatef(0, ufoY, -10); //postion we want it

	glColor3f(0, 0.5, 1);//window
	glPushMatrix();
	  glTranslatef(0, 0, -10);
	  glScalef(1, 0.8, 1);
	  glutSolidSphere(1, 20, 20);
	glPopMatrix();

	glColor3f(1, 1, 0);//body
	glPushMatrix();
	  glTranslatef(0, 0,-10);
	  glScalef(1.2, 0.2, 1.2);
	  glutSolidSphere(2, 20, 20);
	glPopMatrix();

	//lights, which hopefull flash
	glColor3f(red, green, blue);
	glPushMatrix();
	  glTranslatef(0, 0.2,-8);
	  glScalef(0.2, 0.2, 0.2);
	  glutSolidSphere(1, 20, 20);
	glPopMatrix();

	glPushMatrix();
	  glTranslatef(0, 0.2,-12);
	  glScalef(0.2, 0.2, 0.2);
	  glutSolidSphere(1, 20, 20);
	glPopMatrix();

	glPushMatrix();
	  glTranslatef(-2, 0.2,-10);
    glPushMatrix();

    glPopMatrix();;
	  glScalef(0.2, 0.2, 0.2);
	  glutSolidSphere(1, 20, 20);
	glPopMatrix();

	glPushMatrix();
	  glTranslatef(2, 0.2,-10);
	  glScalef(0.2, 0.2, 0.2);
	  glutSolidSphere(1, 20, 20);
    glPopMatrix();
    glutTimerFunc(1000, updateColor, 0);

    // spotlight underneath the ufo which shine down when launched
    //light setup
    float light_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    float light_diffuse[] = { 1.0f, 0.0f, 1.0f, 1.0f };
    float light_specular[] = { 1.0f, 0.0f, 1.0f, 1.0f };
    float light_position[] = { 0.0f, -0.2f, -10.0f, 1.0f };
    float spot_direction[] = { 0.0f, -1.0f, 0.0f };
    glEnable(GL_LIGHT0);

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);
    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 15.0f);
    glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 2.0f);
    glEnable(GL_LIGHT0);

}

void sun() //sun model
{
	glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, txId[3]);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);	
	glPushMatrix();
	    glTranslatef(sunX, sunY, -100.0);
        glRotatef(20, -1, 0, 0);
	    gluSphere ( q, 2.0, 20, 20 );
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);

    sunPath();
}

void alien(float angleOffset, float forwardAngle) //alien model
{
    //working out where to place them so they are in a circle
    float radius = 5.0f; 
    float angle = theta + angleOffset;
    float x = radius * cos(angle * M_PI / 180.0);
    float z = (radius * sin(angle * M_PI / 180.0) - 15);
    float rotateAngle = atan2(x, z) * 180 / M_PI;


    glTranslatef(x, alienY, z);

    glPushMatrix();
    glLoadIdentity();
    glRotatef(rotateAngle, 0, 1, 0);
    glTranslatef(0, 0, -15);
    glPopMatrix();

    glColor3f(0, 0.4, 0);
    glPushMatrix(); //head
        glTranslatef(0, 0.5, -5);
        glScalef(0.2, 0.2, 0.2);
        glutSolidSphere(1, 20, 20);
    glPopMatrix();

    glColor3f(0, 0, 0);
    glPushMatrix(); //eyes
        glTranslatef(0.12f, 0.5f, -4.8f);
        glScalef(0.05, 0.05, 0.05);
        glutSolidSphere(1,20,20);
    glPopMatrix();

    glPushMatrix(); //eyes
        glTranslatef(-0.12f, 0.5, -4.8f);
        glScalef(0.05, 0.05, 0.05);
        glutSolidSphere(1,20,20);
    glPopMatrix();
    
	glColor3f(0, 0.4, 0);//body
	glPushMatrix();
	  glTranslatef(0, 0, -5);
	  glScalef(0.25, 0.44, 0.25);
	  glutSolidSphere(1, 20, 20);
	glPopMatrix();

    glPushMatrix(); //right leg
        glTranslatef(0.1, -0.5, -5);
        glRotatef(legAngle, 1.0f, 0.0f, 0.0f);
        glScalef(0.05, 0.3, 0.05);
        glutSolidSphere(1, 20, 20);
    glPopMatrix();

    glPushMatrix(); //left leg
        glTranslatef(-0.1, -0.5, -5);
        glRotatef(-legAngle, 1.0f, 0.0f, 0.0f);
        glScalef(0.05, 0.3, 0.05);
        glutSolidSphere(1, 20, 20);
    glPopMatrix();

	glPushMatrix(); //right arm
        glTranslatef(0.2, 0, -5);
        glRotatef(armAngle, 1, 0, 0);
        if (jump) {
            glRotatef(-45, 0, 0, 1);
        } else {
            glRotatef(20, 0, 0, 1);
        }
        glScalef(0.05, 0.4, 0.05);
        glutSolidSphere(1, 20, 20);
    glPopMatrix();

    glPushMatrix();//left arm
        glTranslatef(-0.2, 0, -5);
        glRotatef(-armAngle, 1, 0, 0);
        if (jump) {
            glRotatef(225, 0, 0, 1);
        }else {
            glRotatef(340, 0, 0, 1);
        }
        glScalef(0.05, 0.4, 0.05);
        glutSolidSphere(1, 20, 20);
    glPopMatrix();

    move();
}

void mountains(float x, float z) //mountains  for the background of the scene
{
    glPushMatrix();
    float rangeWidth = 200.0f;
    float peakHeight = 10.0f;
    float slopeHeight = 20.0f;
    glTranslatef(x, 10.0f, z);
    glColor3f(0.5f, 0.5f, 0.5f);
    for (float x = -rangeWidth / 2.0f; x < rangeWidth / 2.0f; x += 20.0f) {
        glBegin(GL_TRIANGLES);
        glVertex3f(x, 0.0f, 0.0f);
        glVertex3f(x + 10.0f, peakHeight, 0.0f);
        glVertex3f(x + 20.0f, 0.0f, 0.0f);
        glEnd();

        glBegin(GL_QUADS);
        glVertex3f(x, 0.0f, 0.0f);
        glVertex3f(x, -slopeHeight, 20.0f);
        glVertex3f(x + 20.0f, -slopeHeight, 20.0f);
        glVertex3f(x + 20.0f, 0.0f, 0.0f);
        glEnd();
    }

    glPopMatrix();
}

void special(int key, int x, int y) //makes the camera move when arrow keys are pressed
{
    float floorSize = 75.0f;

    //seeing if keys have been pressed
    if (key == GLUT_KEY_LEFT)
        angle -= 0.05;
    else if (key == GLUT_KEY_RIGHT)
        angle += 0.05;
    else if (key == GLUT_KEY_DOWN)
    {
        float new_eye_x = eye_x - 0.1 * sin(angle);
        float new_eye_z = eye_z + 0.1 * cos(angle);

        // check if it gets to the boundries of the map
        if (new_eye_x < -floorSize || new_eye_x > floorSize || new_eye_z < -floorSize || new_eye_z > floorSize)
        {
            eye_x = 0.0f;
            eye_z = 0.0f;
        }
        else
        {
            eye_x = new_eye_x;
            eye_z = new_eye_z;
        }
    }
    else if (key == GLUT_KEY_UP)
    {
        float new_eye_x = eye_x + 0.1 * sin(angle);
        float new_eye_z = eye_z - 0.1 * cos(angle);

        if (new_eye_x < -floorSize || new_eye_x > floorSize || new_eye_z < -floorSize || new_eye_z > floorSize)
        {
            eye_x = 0.0f;
            eye_z = 0.0f;
        }
        else
        {
            eye_x = new_eye_x;
            eye_z = new_eye_z;
        }
    }

    look_x = eye_x + 100 * sin(angle);
    look_z = eye_z - 100 * cos(angle);
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
	// if space is pressed launch the ship
    switch (key) {
        case 32: 
            if (!isLaunching) {
                launchUFO();
            }
            break;
    }
}

void timer(int value) //timer func
{
    glutTimerFunc(100, timer, 0);
    glutPostRedisplay();
}

void initialise()
{ 
    //set up lighting, in diagonal corners to light up everything
    float grey[4] = {0.1, 0.1, 0.1, 1.}; //light’s ambient color
    float white[4] = {0.5, 0.5, 0.5, 1.}; // diffuse and spec color
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHT2);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT2, GL_AMBIENT, grey);
    glLightfv(GL_LIGHT1, GL_AMBIENT, grey);
    glLightfv(GL_LIGHT2, GL_SPECULAR, white);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT1, GL_SPECULAR, white);
    glEnable(GL_COLOR_MATERIAL);

    //quadratic setup for the sun model
    q = gluNewQuadric();
	gluQuadricDrawStyle (q, GLU_FILL );
	gluQuadricNormals	(q, GLU_SMOOTH );
    gluQuadricTexture (q, GL_TRUE);

	glClearColor(1, 1, 1, 1);
	glEnable(GL_DEPTH_TEST);
	loadTexture();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, 1.0, 10.0, 1000.0);
}


void display() 
{
    float light[4] = {-100, 50, 100, 1};
    float light2[4] = {100, 50, -100, 1};
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);    
    glMatrixMode(GL_PROJECTION);                        
    glLoadIdentity();
    gluPerspective(45., 1., 1., 1000.);

    glMatrixMode(GL_MODELVIEW);                                
    glLoadIdentity();
    gluLookAt(eye_x, 0, eye_z,  look_x, 0, look_z,   0, 1, 0);
	glLightfv(GL_LIGHT2, GL_POSITION, light);
    glLightfv(GL_LIGHT1, GL_POSITION, light2);
    glEnable(GL_COLOR_MATERIAL);

    //generate the textures with no light
    glDisable(GL_LIGHTING);
	floor();
	skydome();
    sun();
    glEnable(GL_LIGHTING);

    //spawns mountains on each side of the plane
    glPushMatrix();
    mountains(0, -90);
    glRotatef(180,0,1,0);
    mountains(0, -90);
    glPopMatrix();

    glPushMatrix();
    glRotatef(90,0,1,0);
    mountains(0, -90);
    glRotatef(180,0,1,0);
    mountains(0, -90);
    glPopMatrix();

    //spawn ufo
    glPushMatrix();
    glTranslatef(0,0.2f,0);
    ufo();
    glPopMatrix();

    //evenly spreads aliens around the ufo
    float angleIncrement = 360.0f / 4;
    for (int i = 0; i < 4; ++i) {
        float angleOffset = i * angleIncrement;
        float forwardAngle = angleOffset; 
        glPushMatrix();
        alien(angleOffset, forwardAngle);
        glPopMatrix();
    }
    glPopMatrix();

    //parabola to be used as launchpad (also for spotlight to reflect onto)
    glPushMatrix();
    glTranslatef(0,-0.5f,-19.5f);
    glScalef(0.8,1,0.8);
    wave();
    glPopMatrix();
     
    glutSwapBuffers();
    glFlush();                              
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB| GLUT_DEPTH);
   glutInitWindowSize (800, 800); 
   glutInitWindowPosition (10, 10);
   glutCreateWindow ("Assignment 1 cco128");
   initialise();
   glutSpecialFunc(special);
   glutKeyboardFunc(keyboard);
   glutTimerFunc(100, timer, 0);
   glutDisplayFunc(display); 
   glutMainLoop();
   return 0;
}