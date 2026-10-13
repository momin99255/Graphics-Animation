//#include<windows.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include<bits/stdc++.h> //library for math

//globbal variables for zoom
//static int slices = 16;
//static int stacks = 16;


//global variables for translation and zoom
float tx = -2.0;
float ty = 0.0;
float tz = -7.0;
float zoom = 1;

//global variables for rotation
float degreeX = 15;
float degreeY = -20;
float degreeZ = 0;

//global variables for camera control
float angle = 0;
double ex = 0;
double ey = 0;
double ez = 5;

//global variables for quad color
float r,g,b;

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

//parameter quad()
void Quads(float r, float g, float b)
{
    glBegin(GL_QUADS);

    glColor3f(r,g,b);
    glVertex3f(0.0,1.0,0.0);
    glVertex3f(1.0,1.0,0.0);
    glVertex3f(1.0,0.0,0.0);
    glVertex3f(0.0,0.0,0.0);

    glColor3f(r,g,b);
    glVertex3f(0.0,1.0,1.0);
    glVertex3f(0.0,1.0,0.0);
    glVertex3f(0.0,0.0,0.0);
    glVertex3f(0.0,0.0,1.0);

    glColor3f(r,g,b);
    glVertex3f(1.0,1.0,1.0);
    glVertex3f(0.0,1.0,1.0);
    glVertex3f(0.0,0.0,1.0);
    glVertex3f(1.0,0.0,1.0);

    glColor3f(r,g,b);
    glVertex3f(1.0,1.0,0.0);
    glVertex3f(1.0,1.0,1.0);
    glVertex3f(1.0,0.0,1.0);
    glVertex3f(1.0,0.0,0.0);

    glColor3f(r,g,b);
    glVertex3f(1.0,0.0,1.0);
    glVertex3f(0.0,0.0,1.0);
    glVertex3f(0.0,0.0,0.0);
    glVertex3f(1.0,0.0,0.0);

    glColor3f(r,g,b);
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
  Quads(1,1,1);
  glPopMatrix();

  //back
  glPushMatrix();
  glScalef(3.0,3.0,0.2);
  Quads(0,1,0);
  glPopMatrix();

  //1st leg
  glPushMatrix();
  glTranslated(0,-3.0,0);
  glScalef(0.5,3.0,0.2);
  Quads(1,0,0);
  glPopMatrix();

  //2nd leg
  glPushMatrix();
  glTranslated(0,-3.0,2.8);
  glScalef(0.5,3.0,0.2);
  Quads(1,0,0);
  glPopMatrix();

  //3rd leg
  glPushMatrix();
  glTranslated(2.5,-3.0,0);
  glScalef(0.5,3.0,0.2);
  Quads(1,0,0);
  glPopMatrix();

  //4th leg
  glPushMatrix();
  glTranslated(2.5,-3.0,2.8);
  glScalef(0.5,3.0,0.2);
  Quads(1,0,0);
  glPopMatrix();


    const GLfloat mat_ambient3[]    = { 0.1f, 0.1f, 1.0f, 1.0f };
    const GLfloat mat_diffuse3[]    = { 1.0f, 1.0f, 1.0f, 1.0f };
    const GLfloat mat_specular3[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
    const GLfloat high_shininess3[] = { 100.0f };

    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient3);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse3);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular3);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess3);


}

void cylinder(double r, double g, double b)
{
    //Body
    glColor3f(r, g, b);
    glBegin(GL_QUAD_STRIP);
    for(int i = 0; i <= 360; i++)
    {
        double theta = 2 * 3.1416 * i / 360;
        float x = cos(theta);
        float y = sin(theta);
        glVertex3f(x, 0, y);
        glVertex3f(x, 2.5, y);
    }
    glEnd();

    //Top Cap
    glBegin(GL_POLYGON);
    for(int i = 360; i >= 0; i--)
    {
        double theta = 2 * 3.1416 * i / 360;
        glVertex3f(cos(theta), 2.5, sin(theta));
    }
    glEnd();

    //Bottom Cap
    glBegin(GL_POLYGON);
    for(int i = 0; i <= 360; i++)
    {
        double theta = 2 * 3.1416 * i / 360;
        glVertex3f(cos(theta), 0, sin(theta));
    }
    glEnd();
}


void spherical(GLdouble x, GLdouble y,GLdouble z,GLdouble rotan, GLdouble rx, GLdouble ry,GLdouble rz, GLdouble sx, GLdouble sy,GLdouble sz)
{
       //glColor3f(0.6, 0, 0);
       glPushMatrix();
       glTranslated(x,y,z);
       glRotated(rotan,rx,ry,rz);
       glScaled(sx,sy,sz);
       glutWireSphere(4.0, 50, 50);
       glPopMatrix();

}

void assignmentObjects()
{

    //Cylinder
    glPushMatrix();
         glTranslated(1.0, 0.2, 1.2);
        glScalef(0.4, 0.6, 0.4);
        cylinder(0.8, 0.8, 0.8);
        glColor3f(0.4, 0.4, 0.4);
    glPopMatrix();

   //Cube
    glPushMatrix();
        glTranslated(1.7, 0.2, 0.7);
        glRotated(20, 0,-5.0,0);
        glScalef(0.8, 0.8, 0.8);
        Quads(0.7,.7,.7);
    glPopMatrix();

    //Sphere
    glPushMatrix();
        glTranslated(1.5, 0.6, 2.1);
        glRotated(angle, 0, 1, 0);
        glScalef(0.12, 0.12, 0.12);
        glColor3f(0.6, 0.6, 0.6);
        spherical(0,0,0, 0,0,0,0, 1,1,1);
    glPopMatrix();

    //continuous rotating object (Own Axis + Orbit around Chair)
    glPushMatrix();
        glRotatef(angle, 0.0, 1.0, 0.0);
        glTranslated(4.5, 3, 0.0);
        glRotatef(angle * 3.0, 0.0, 1.0, 0.0);
        glScalef(0.15, 0.15, 0.15);
        glColor3f(1.0, 0.1, 0.1);
        glutWireSphere(4.0, 15, 15);
    glPopMatrix();


    const GLfloat mat_ambient3[]    = { 0.1f, 0.1f, 1.0f, 1.0f };
    const GLfloat mat_diffuse3[]    = { 1.0f, 1.0f, 1.0f, 1.0f };
    const GLfloat mat_specular3[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
    const GLfloat high_shininess3[] = { 100.0f };

    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient3);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse3);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular3);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess3);



}

void spin()
{
    angle += 0.5;
    if(angle>360)
        angle -= 360;
    glutPostRedisplay();
}



static void display(void)
{
    const double t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    const double a = t*90.0;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3d(1,0,0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;

    //Camera Control
    gluLookAt(ex, ey, ez, 0, 0, 0, 0, 1, 0);

    glPushMatrix();
        glTranslated(tx,ty,tz);
        glRotated(degreeX,.5,0,0);
        glRotated(degreeY,0,5,0);
        glRotated(degreeZ,0,0,.5);
        glScalef(zoom,zoom,zoom);
        //Quads();
        chair();
        assignmentObjects();


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

        case 'A':
            degreeX-=2.5;
            break;

        case 'b':
            degreeY+=2.5;
            break;

        case 'B':
            degreeY-=2.5;
            break;

        case 'c':
            degreeZ+=2.5;
            break;

        case 'C':
            degreeZ-=2.5;
            break;

//        case '-':
//            if (slices>3 && stacks>3)
//            {
//                slices--;
//                stacks--;
//            }
//            break;

        // Translation Controls
        case 'i': tx += 0.2; break;
        case 'I': tx -= 0.2; break;
        case 'j': ty += 0.2; break;
        case 'J': ty -= 0.2; break;
        case 'k': tz += 0.2; break;
        case 'K': tz -= 0.2; break;

        // Scaling (Zoom)
        case '+': zoom += 0.05; break;
        case '_': zoom -= 0.05; break;

        // Camera control
        case 'x': ex++;break;
        case 'X': ex--;break;
        case 'y': ey++;break;
        case 'Y': ey--;break;
        case 'z': ez++;break;
        case 'Z': ez--;break;
        case '1':glEnable(GL_LIGHT0);break;
        case '0':glDisable(GL_LIGHT0);break;
        case '8':glEnable(GL_LIGHT1);break;
        case '7':glDisable(GL_LIGHT1);break;

    }

    glutPostRedisplay();
}



void initLighting()
{
    glEnable(GL_LIGHTING);

    GLfloat whiteAmbient[]  = {0.1f, 0.1f, 0.1f, 1.0f};
    GLfloat whiteDiffuse[]  = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat whiteSpecular[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat whitePosition[] = {5.0f, 3.0f, 7.0f, 1.0f};


    glLightfv(GL_LIGHT0, GL_AMBIENT,  whiteAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  whiteDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, whiteSpecular);
    glLightfv(GL_LIGHT0, GL_POSITION, whitePosition);


    GLfloat yellowAmbient[]  = {0.1f, 0.1f, 0.0f, 1.0f};
    GLfloat yellowDiffuse[]  = {1.0f, 1.0f, 0.0f, 1.0f};
    GLfloat yellowSpecular[] = {1.0f, 1.0f, 0.0f, 1.0f};
    GLfloat yellowPosition[] = {5.0f, 3.0f, 7.0f, 1.0f};

    glLightfv(GL_LIGHT1, GL_AMBIENT,  yellowAmbient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE,  yellowDiffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, yellowSpecular);
    glLightfv(GL_LIGHT1, GL_POSITION, yellowPosition);


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

    glutCreateWindow("2023100000511");

    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutIdleFunc(spin);

    glClearColor(1,1,1,1);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_NORMALIZE);

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    initLighting();



    glutMainLoop();

    return EXIT_SUCCESS;
}
