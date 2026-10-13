//#include<windows.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include<bits/stdc++.h>


float degreeX = 0;
float degreeY = 0;
float degreeZ = 0;
float angle = 0;


double ex = 0;
double ey = 0;
double ez = 5;

/* GLUT callback Handlers */

static void resize(int width, int height)
{
    const float ar = (float) width / (float) height;
  glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-ar, ar, -1.0, 1.0, 2.0, 100.0);

    //glMatrixMode(GL_MODELVIEW);
    //glLoadIdentity() ;

}



// ---------------- CUBE ----------------
void cube()
{
    glBegin(GL_QUADS);

//red front
    //glColor3f(1,0,0);
    glVertex3f(0.0,1.0,0.0);
    glVertex3f(1.0,1.0,0.0);
    glVertex3f(1.0,0.0,0.0);
    glVertex3f(0.0,0.0,0.0);

    //yellow left

    //glColor3f(1,1,0);
    glVertex3f(0.0,1.0,1.0);
    glVertex3f(0.0,1.0,0.0);
    glVertex3f(0.0,0.0,0.0);
    glVertex3f(0.0,0.0,1.0);

    //blue back
   // glColor3d(0,0,1);
    glVertex3f(1.0,1.0,1.0);
    glVertex3f(0.0,1.0,1.0);
    glVertex3f(0.0,0.0,1.0);
    glVertex3f(1.0,0.0,1.0);

//green right

    //glColor3f(0,1,0);
    glVertex3f(1.0,1.0,0.0);
    glVertex3f(1.0,1.0,1.0);
    glVertex3f(1.0,0.0,1.0);
    glVertex3f(1.0,0.0,0.0);



    //cyan bottom

    //glColor3f(1,0,1);
    glVertex3f(1.0,0.0,1.0);
    glVertex3f(0.0,0.0,1.0);
    glVertex3f(0.0,0.0,0.0);
    glVertex3f(1.0,0.0,0.0);

    //violet top

   //glColor3f(0,1,1);
    glVertex3f(1.0,1.0,0.0);
    glVertex3f(0.0,1.0,0.0);
    glVertex3f(0.0,1.0,1.0);
    glVertex3f(1.0,1.0,1.0);
    glEnd();

}


// ---------------- TABLE ----------------
void table()
{
glColor3f(0.5f, 0.3f, 0.1f);
    glPushMatrix();

    const GLfloat mat_ambient1[]    = { 0.7f, 0.2f, 0.0f, 1.0f };
    const GLfloat mat_diffuse1[]    = { 0.7f, 0.2f, 0.0f, 1.0f };
    const GLfloat mat_specular1[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
    const GLfloat high_shininess1[] = { 100.0f };


    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient1);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse1);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular1);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess1);



//Top
    // glColor3f(1,.3,.3);
    glTranslated(-3,-.5,-6);
    glScalef(6,.5,4);
    cube();
    glPopMatrix();

//left corner
    glPushMatrix();
    //glColor3f(1,.3,.3);
    glTranslated(-3,-6,-6);
    glScalef(.5,6,.5);
    cube();
    glPopMatrix();

    //right corner
    glPushMatrix();
    // glColor3f(1,.3,.3);
    glTranslated(2.5,-6,-6);
    glScalef(.5,6,.5);
    cube();
    glPopMatrix();




    //left corner
    glPushMatrix();
    // glColor3f(1,.3,.3);
    glTranslated(-3,-6,-3);
    glScalef(.5,6,.5);
    cube();
    glPopMatrix();

    //right corner
    glPushMatrix();
    //glColor3f(1,.3,.3);
    glTranslated(2.5,-6,-3);
    glScalef(.5,6,.5);
    cube();
    glPopMatrix();


}

// ---------------- CIRCLE ---------------
//circle bottom

void circle(double r, double g, double b)
{
    glColor3f(r,g,b);
    glBegin(GL_POLYGON);

    for(int i = 0; i<=360; i++)
    {
        double theta = 2*3.1416* i/360;
        glVertex3f(cos(theta), sin(theta), 0);

    }
    glEnd();


//circle top


    glColor3f(1,0,0);
    glBegin(GL_POLYGON);

    for(int i = 0; i<=360; i++)
    {
        double theta = 2*3.1416* i/360;
        glVertex3f(cos(theta), sin(theta), 0.8);

    }
    glEnd();

    glColor3f(1, 0, 0); // blue
    glBegin(GL_QUAD_STRIP);
    for(int i = 0; i <= 360; i++)
    {
        double theta  = 2 * 3.1416 * i / 360;

        float x = cos(theta);
        float y =  sin(theta);

        glVertex3f(x, y, 0);        // bottom
        glVertex3f(x, y, 0.8);   // top
    }
    glEnd();


}





void wings (GLdouble rot)
{
    glPushMatrix();



    glTranslated(0,0,0);
    glRotated(rot,0,0,1);
    glRotatef(angle, 0,0,1);

    const GLfloat mat_ambient3[]    = { 0.0f, 0.0f, 1.0f, 1.0f };
    const GLfloat mat_diffuse3[]    = { 0.0f, 0.0f, 1.0f, 1.0f };
    const GLfloat mat_specular3[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
    const GLfloat high_shininess3[] = { 100.0f };

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,   mat_ambient3);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,   mat_diffuse3);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  mat_specular3);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, high_shininess3);


    glScalef(1.8,.3,0.05);
    cube();
    glPopMatrix();

}



void fan()
{
    glPushMatrix();

    glTranslated(0,0,-4);

     glPushMatrix();
    glTranslated(0,0,-0.25);
     glScalef(0.3,2,0.3);
     cube();
     glPopMatrix();

         // fan circle
    glTranslated(0.1,2,0);
    glPushMatrix();
    glTranslated(0,0,-0.3);
    glScalef(0.5,0.5,0.5);
    circle(0,0,1);
    glPopMatrix();


    wings(0);
    wings(120);
    wings(240);

glPopMatrix();



}

void spin()
{
    angle += 0.05;
    if(angle>360)
        angle -= 360;
    glutPostRedisplay();
}





void spherical(GLdouble tx, GLdouble ty,GLdouble tz,GLdouble rotan, GLdouble rx, GLdouble ry,GLdouble rz, GLdouble sx, GLdouble sy,GLdouble sz)
{

    glPushMatrix();

    const GLfloat mat_ambient4[]    = { 0.0f, 0.0f, 0.2f, 1.0f };
    const GLfloat mat_diffuse4[]    = { 0.0f, 0.0f, 1.0f, 0.6f };
    const GLfloat mat_specular4[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
    const GLfloat high_shininess4[] = { 100.0f };

    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient4);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse4);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular4);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess4);



    glTranslated(tx,ty,tz);
    glRotated(rotan,rx,ry,rz);
    glScaled(sx,sy,sz);
    glutSolidSphere(4.0, 50, 50); //glutSolidSphere(radius, slices, stacks)
    glPopMatrix();


}


void jack()
{

////3 scaled sphere

    spherical(-12,5,8,90,1,0,0,1,.1,.1);
    spherical(-12,5,8,90,0,1,0,1,.1,.1);
    spherical(-12,5,8,90,0,0,1,1,.1,.1);

////6 sphere corner


    spherical(-15.5,5,8,0,0,0,0,.2,.2,.2);
     spherical(-8.5,5,8,0,0,0,0,.2,.2,.2);
      spherical(-12,5,12,0,0,0,0,.2,.2,.2);
       spherical(-12,5,3.7,0,0,0,0,.2,.2,.2);
        spherical(-12,9,8,0,0,0,0,.2,.2,.2);
         spherical(-12,1,8,0,0,0,0,.2,.2,.2);


}




void tableelements()
{
      //scale(.7,.7,.7);

    table();
    glPushMatrix();
    glTranslated(-1.5,0,0);
    fan();
    glPopMatrix();

    //glTranslated(25,-1,-10);
    glScaled(0.3,0.3,0.3);
    glTranslated(14,13,-18);
    glRotated(60,1,0,1);
    jack();
}


void standwall(GLdouble tx, GLdouble ty,GLdouble tz,GLdouble rotan, GLdouble rx, GLdouble ry,GLdouble rz, GLdouble sx, GLdouble sy,GLdouble sz)

{
    glPushMatrix();


    const GLfloat mat_ambient2[]    = { 0.4f, 1.0f, 0.4f, 1.0f };
const GLfloat mat_diffuse2[]    = { 0.4f, 1.0f, 0.8f, 1.0f };
const GLfloat mat_specular2[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess2[] = { 100.0f };

    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient2);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse2);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular2);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess2);
    glTranslated(tx,ty,tz);
        glRotated(rotan,rx,ry,rz);

    glScalef(sx,sy,sz);
    cube();
  glPopMatrix();

}


void wall()
{
//
 standwall(-10,-7,-10,0,0,0,0,18,.1,10);

 standwall(-10,-7,-10,0,0,0,0,.1,14,10);

 //back
 standwall(-10,-7,-10,0,0,0,0,18,14,.1);


}











static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
    gluLookAt(ex, ey, ez, 0, 0, 0, 0, 1, 0); //gluLookAt(ex, ey, ez,centerX, centerY, centerZ, upX, upY, upZ);

    glTranslated(0,0,-15);

    glTranslated(-3, -0.5, -6);

    // Step 2: rotate
    glRotated(degreeX,1,0,0);
    glRotated(degreeY,0,1,0);
    glRotated(degreeZ,0,0,1);

    // Step 3: move back
    glTranslated(3, 0.5, 6);
glPushMatrix();
wall();
glPopMatrix();
    tableelements();




    glutSwapBuffers();
}


static void key(unsigned char key, int x, int y)
{
 switch (key)
    {
        case 27: exit(0);

        case 'x': degreeX+=5; break;
        case 'X': degreeX-=5; break;

        case 'y': degreeY+=5; break;
        case 'Y': degreeY-=5; break;

        case 'z': degreeZ+=5; break;
        case 'Z': degreeZ-=5; break;




          case 'c':
            ex++;
            break;
        case 'd':
            ey++;
            break;
        case 'e':
            ez++;
            break;

        case 'C':
            ex--;
            break;
        case 'D':
            ey--;
            break;
        case 'E':
            ez--;
            break;


    case '0':
        glDisable(GL_LIGHT0);
        break;
    case '1':
        glEnable(GL_LIGHT0);
        break;

    case '8':
        glDisable(GL_LIGHT1);
        break;
    case '7':
        glEnable(GL_LIGHT1);
        break;



    }
    glutPostRedisplay();
}




     // Set lighting intensity and color
void initLighting()
{
    glEnable(GL_LIGHTING);

    GLfloat whiteAmbient[]  = {0.3f, 0.3f, 0.3f, 1.0f};
    GLfloat whiteDiffuse[]  = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat whiteSpecular[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat whitePosition[] = {5.0f, 3.0f, 7.0f, 1.0f};


    glLightfv(GL_LIGHT0, GL_AMBIENT,  whiteAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  whiteDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, whiteSpecular);
    glLightfv(GL_LIGHT0, GL_POSITION, whitePosition);




    GLfloat yellowAmbient[]  = {0.2f, 0.2f, 0.0f, 1.0f};
    GLfloat yellowDiffuse[]  = {1.0f, 1.0f, 0.5f, 1.0f};
    GLfloat yellowSpecular[] = {1.0f, 1.0f, 0.0f, 1.0f};
    GLfloat yellowPosition[] = {-5.0f, 5.0f, 2.0f, 1.0f};

    glLightfv(GL_LIGHT1, GL_AMBIENT,  yellowAmbient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE,  yellowDiffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, yellowSpecular);
    glLightfv(GL_LIGHT1, GL_POSITION, yellowPosition);


}




/* Program entry point */

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(640,480);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("Table fan");

    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutIdleFunc(spin);

    glClearColor(1,1,1,1);
    glEnable(GL_DEPTH_TEST);

    //glEnable(GL_NORMALIZE);
    //glEnable(GL_COLOR_MATERIAL);
    initLighting();



    glutMainLoop();

    return EXIT_SUCCESS;
}






