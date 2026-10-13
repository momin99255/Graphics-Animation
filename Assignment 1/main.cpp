//#include<windows.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include<bits/stdc++.h>

static int slices = 16;
static int stacks = 16;
float degreeX = 0;
float degreeY = 0;
float degreeZ = 0;

//global variables for translation and zoom
float tx = -2.0;
float ty = 0.0;
float tz = -7.0;
float zoom = 1;

/* GLUT callback Handlers */

static void resize(int width, int height)
{
    const float ar = (float) width / (float) height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-ar, ar, -1.0, 1.0, 2.0, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
}
void Quads()
{
    glBegin(GL_QUADS);

    //red front
    glColor3f(1,0,0);
    glVertex3f(0.0,1.0,0.0);
    glVertex3f(1.0,1.0,0.0);
    glVertex3f(1.0,0.0,0.0);
    glVertex3f(0.0,0.0,0.0);

    //yellow left
    glColor3f(1,1,0);
    glVertex3f(0.0,1.0,1.0);
    glVertex3f(0.0,1.0,0.0);
    glVertex3f(0.0,0.0,0.0);
    glVertex3f(0.0,0.0,1.0);

    //blue back
    glColor3d(0,0,1);
    glVertex3f(1.0,1.0,1.0);
    glVertex3f(0.0,1.0,1.0);
    glVertex3f(0.0,0.0,1.0);
    glVertex3f(1.0,0.0,1.0);

    //green right
    glColor3f(0,1,0);
    glVertex3f(1.0,1.0,0.0);
    glVertex3f(1.0,1.0,1.0);
    glVertex3f(1.0,0.0,1.0);
    glVertex3f(1.0,0.0,0.0);


    //cyan bottom
    glColor3f(1,0,1);
    glVertex3f(1.0,0.0,1.0);
    glVertex3f(0.0,0.0,1.0);
    glVertex3f(0.0,0.0,0.0);
    glVertex3f(1.0,0.0,0.0);

    //violet top
    glColor3f(0,1,1);
    glVertex3f(1.0,1.0,0.0);
    glVertex3f(0.0,1.0,0.0);
    glVertex3f(0.0,1.0,1.0);
    glVertex3f(1.0,1.0,1.0);
    glEnd();

}


void chair()
{

  //seat
  glPushMatrix();
  glScalef(3.0,0.2,3.0);
  Quads();
  glPopMatrix();

  //back
  glPushMatrix();
  glScalef(3.0,3.0,0.2);
  Quads();
  glPopMatrix();

  //1st leg (left-back)
  glPushMatrix();
  glTranslated(0,-3.0,0);
  glScalef(0.5,3.0,0.2);
  Quads();
  glPopMatrix();

  //2nd leg (left-front)
  glPushMatrix();
  glTranslated(0,-3.0,2.8);
  glScalef(0.5,3.0,0.2);
  Quads();
  glPopMatrix();

  //3rd leg (right-back)
  glPushMatrix();
  glTranslated(2.5,-3.0,0);
  glScalef(0.5,3.0,0.2);
  Quads();
  glPopMatrix();

  //4th 1st leg (right-front)
  glPushMatrix();
  glTranslated(2.5,-3.0,2.8);
  glScalef(0.5,3.0,0.2);
  Quads();
  glPopMatrix();

}


void tabe ()
{

   //stand
   glPushMatrix();
   glScalef(4, .2, 2);
   Quads();
   glPopMatrix();

   //front left
   glPushMatrix();
   glTranslated(.3 , -2.3, 1.7);
   glScalef(.2, 2.3, .2);
   Quads();
   glPopMatrix();

   //front right
   glPushMatrix();
   glTranslated(3.5 , -2.3, 1.7);
   glScalef(.2, 2.3, .2);
   Quads();
   glPopMatrix();

   //back left
   glPushMatrix();
   glTranslated(.3 , -2.3, .3);
   glScalef(.2, 2.3, .2);
   Quads();
   glPopMatrix();

   //front right
   glPushMatrix();
   glTranslated(3.5 , -2.3, .3);
   glScalef(.2, 2.3, .2);
   Quads();
   glPopMatrix();

}

void circle(double r, double g, double b)
{
    glColor3f(r,g,b);
    glBegin(GL_POLYGON);

    for(int i=0; i<= 360 ; i++)
    {
        double theta = 2*3.1416* i/360;
        glVertex3f(cos(theta), sin(theta), 0);
    }
    glEnd();

}

void robot()
{

   //Torso
   glPushMatrix();
   glTranslated(.5, -.5, 0);
   glScalef(2, 2 , 1);
   Quads();
   glPopMatrix();

   //head
   glPushMatrix();
   glTranslated(.9, 1.5, -.2);
   glScalef(1.2, .8 , 1.5);
   Quads();
   glPopMatrix();

   //left leg
   glPushMatrix();
   glTranslated(.6, -2.5, .15);
   glScalef(.5, 2 , .7);
   Quads();
   glPopMatrix();

   //right leg
   glPushMatrix();
   glTranslated(1.9, -2.5, .15);
   glScalef(.5, 2 , .7);
   Quads();
   glPopMatrix();

   //left hand
   glPushMatrix();
   glTranslated(0, 0, .2);
   glScalef(.4, 1.3 , .7);
   Quads();
   glPopMatrix();

   //right hand
   glPushMatrix();
   glTranslated(2.6, 0, .2);
   glScalef(.5, 1.3 , .7);
   Quads();
   glPopMatrix();

}

void robot1()
{
    // === TORSO ===
    glPushMatrix();
    glTranslatef(-0.75f, -0.5f, -0.75f);
    glScalef(1.5f, 2.0f, 1.5f);
    Quads();
    glPopMatrix();

    // === HEAD ===
    glPushMatrix();
    glTranslatef(-0.6f, 1.6f, -0.6f);
    glScalef(1.2f, 1.2f, 1.2f);
    Quads();
    glPopMatrix();

//    // === LEFT EYE ===
//    glPushMatrix();
//    glTranslatef(-0.35f, 2.05f, -0.75f);  // front face of head (z=-0.75)
//    glScalef(0.12f, 0.12f, 0.01f);
//    glColor3f(0,1,1);
//    circle(1, 1, 0);                        // yellow
//    glPopMatrix();
//
//    // === RIGHT EYE ===
//    glPushMatrix();
//    glTranslatef(0.15f, 2.05f, -0.75f);
//    glScalef(0.12f, 0.12f, 0.01f);
//    circle(1, 1, 0);                        // yellow
//    glPopMatrix();

    // === LEFT ARM ===
    glPushMatrix();
    glTranslatef(-1.25f, -0.5f, -0.6f);
    glScalef(0.4f, 1.8f, 1.2f);
    Quads();
    glPopMatrix();

    // === RIGHT ARM ===
    glPushMatrix();
    glTranslatef(0.85f, -0.5f, -0.6f);
    glScalef(0.4f, 1.8f, 1.2f);
    Quads();
    glPopMatrix();

    // === LEFT LEG ===
    glPushMatrix();
    glTranslatef(-0.65f, -2.8f, -0.6f);
    glScalef(0.6f, 2.3f, 1.2f);
    Quads();
    glPopMatrix();

    // === RIGHT LEG ===
    glPushMatrix();
    glTranslatef(0.15f, -2.8f, -0.6f);
    glScalef(0.6f, 2.3f, 1.2f);
    Quads();
    glPopMatrix();

    // === LEFT FOOT ===
    glPushMatrix();
    glTranslatef(-0.75f, -2.85f, -0.9f);
    glScalef(0.8f, 0.15f, 1.5f);
    Quads();
    glPopMatrix();

    // === RIGHT FOOT ===
    glPushMatrix();
    glTranslatef(0.05f, -2.85f, -0.9f);
    glScalef(0.8f, 0.15f, 1.5f);
    Quads();
    glPopMatrix();

    // === ANTENNA STICK ===
    glPushMatrix();
    glTranslatef(-0.05f, 2.4f, -0.3f);
    glScalef(0.15f, 0.6f, 0.15f);
    Quads();
    glPopMatrix();

    // === ANTENNA BALL (circle) ===
    glPushMatrix();
    glTranslatef(0.025f, 3.1f, -0.3f);
    glScalef(0.18f, 0.18f, 0.18f);
    circle(1, 0, 0);                        // red
    glPopMatrix();
}


static void display(void)
{
    const double t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    const double a = t*90.0;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3d(1,0,0);


        //glPushMatrix();

//    glTranslated(-3, -.5, -6);
//        glTranslated(0,0,-10);
//        glRotated(degreeX,0.5,0,0);
//        glRotated(degreeY,0,.5,0);
//        glRotated(degreeZ,0,0,.5);
//        glScalef(1,.7,.7);
//        // Quads();
//
//        glTranslated(3, .5, 6);

    glPushMatrix();
        glTranslated(tx,ty,tz);
        glRotated(degreeX,.5,0,0);
        glRotated(degreeY,0,.5,0);
        glRotated(degreeZ,0,0,.5);
        glScalef(zoom,zoom,zoom);
        //Quads();
        //chair();
        //tabe();
        robot();

//    glPushMatrix();
//    glTranslated(2, 2, 1);
//    glScalef(1,1,3);
//    circle(0,1,0);
//    glPopMatrix();
//    glPopMatrix();


    glPopMatrix();

    glutSwapBuffers();
}



static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27 :
        case 'q':
            exit(0);
            break;

        case 'a':
            degreeX+=2.5;
            break;

        case 'b':
            degreeX-=2.5;
            break;

        case 'c':
            degreeY+=2.5;
            break;

        case 'd':
            degreeY-=2.5;
            break;

        case 'e':
            degreeZ+=2.5;
            break;

        case 'f':
            degreeZ-=2.5;
            break;

        case '-':
            if (slices>3 && stacks>3)
            {
                slices--;
                stacks--;
            }
            break;
        // Translation Controls
        case 'i': ty += 0.2; break;
        case 'j': ty -= 0.2; break;
        case 'k': tx -= 0.2; break;
        case 'l': tx += 0.2; break;
        case 'm': tz += 0.2; break;
        case 'n': tz -= 0.2; break;

        // Scaling (Zoom)
        case '+': zoom += 0.05; break;
        case '_': zoom -= 0.05; break;
    }

    glutPostRedisplay();
}

static void idle(void)
{
    glutPostRedisplay();
}

const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };

/* Program entry point */

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(620,480);
    glutInitWindowPosition(100,100);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("lab 1 task");

    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutIdleFunc(idle);

    glClearColor(1,1,1,1);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);



    glutMainLoop();

    return EXIT_SUCCESS;
}


