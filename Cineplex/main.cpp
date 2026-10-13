#include <GL/glut.h>
#include <math.h>

float rotateCCTV = 0.0, cameraAngle = 0.0;
float eyeX = 0.0, eyeY = 8.0, eyeZ = 30.0;
bool light0 = true, light1 = true;

// Material setup for Realistic Surfaces
void setMaterial(float r, float g, float b, float shine) {
    GLfloat mat_ambient[] = { r*0.2f, g*0.2f, b*0.2f, 1.0f };
    GLfloat mat_diffuse[] = { r, g, b, 1.0f };
    GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat mat_shininess[] = { shine }; // High shine for realistic reflections

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}

// Condition 4: Highly Complex Chair
void drawLuxuryChair(float x, float z) {
    glPushMatrix();
    glTranslatef(x, 0, z);

    // Seat Base
    setMaterial(0.4, 0.0, 0.0, 50.0); // Velvet Red
    glPushMatrix();
    glScalef(2.5, 0.6, 2.5);
    glutSolidCube(1.0);
    glPopMatrix();

    // Curved Backrest (Multiple segments for "Real" look)
    glPushMatrix();
    glTranslatef(0, 1.8, -1.0);
    glScalef(2.5, 3.0, 0.6);
    glutSolidCube(1.0);
    glPopMatrix();

    // Side Armrests
    setMaterial(0.1, 0.1, 0.1, 100.0); // Polished Plastic
    glPushMatrix();
    glTranslatef(1.3, 0.8, 0);
    glScalef(0.4, 1.5, 2.2);
    glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-1.3, 0.8, 0);
    glScalef(0.4, 1.5, 2.2);
    glutSolidCube(1.0);
    glPopMatrix();

    glPopMatrix();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Condition 9: Viewing Coordinate (Camera)
    gluLookAt(eyeX, eyeY, eyeZ, 0, 4, 0, 0, 1, 0);

    // Condition 7: Multi-Light Control
    if (light0) glEnable(GL_LIGHT0); else glDisable(GL_LIGHT0);
    if (light1) glEnable(GL_LIGHT1); else glDisable(GL_LIGHT1);

    // --- The Floor (Condition 8: Wood/Carpet) ---
    setMaterial(0.15, 0.1, 0.1, 10.0);
    glBegin(GL_QUADS);
    glNormal3f(0, 1, 0);
    glVertex3f(-20, 0, -20); glVertex3f(20, 0, -20);
    glVertex3f(20, 0, 20); glVertex3f(-20, 0, 20);
    glEnd();

    // --- The IMAX Screen (Bright Emission) ---
    GLfloat emi[] = {1.0, 1.0, 1.0, 1.0};
    glMaterialfv(GL_FRONT, GL_EMISSION, emi); // Screen nijei light dibe
    glBegin(GL_QUADS);
    glVertex3f(-14, 3, -19.9); glVertex3f(14, 3, -19.9);
    glVertex3f(14, 14, -19.9); glVertex3f(-14, 14, -19.9);
    glEnd();
    GLfloat no_emi[] = {0, 0, 0, 1};
    glMaterialfv(GL_FRONT, GL_EMISSION, no_emi);

    // --- Window/External View (Condition 6) ---
    setMaterial(0.0, 0.4, 0.7, 120.0); // Glassy Blue
    glBegin(GL_QUADS);
    glVertex3f(-19.9, 6, -5); glVertex3f(-19.9, 12, -5);
    glVertex3f(-19.9, 12, 5); glVertex3f(-19.9, 6, 5);
    glEnd();

    // Drawing Rows of Luxury Chairs
    for(float i=-12; i<=12; i+=6)
        for(float j=-5; j<=15; j+=8)
            drawLuxuryChair(i, j);

    // Condition 5: Continuous Rotating CCTV
    glPushMatrix();
    glTranslatef(18, 18, -18);
    glRotatef(rotateCCTV, 0, 1, 0);
    setMaterial(0.6, 0.6, 0.6, 100.0);
    glutSolidSphere(1.0, 20, 20);
    glPopMatrix();

    glutSwapBuffers();
}

void init() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_NORMALIZE);
    glShadeModel(GL_SMOOTH); // Smooth shading for realism

    // Ceiling Light
    GLfloat light0_pos[] = { 0, 20, 0, 1 };
    GLfloat white[] = { 1, 1, 1, 1 };
    glLightfv(GL_LIGHT0, GL_POSITION, light0_pos);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, white);

    // Screen Light (Blueish Tint)
    GLfloat light1_pos[] = { 0, 5, -15, 1 };
    GLfloat blue[] = { 0.2, 0.2, 0.8, 1 };
    glLightfv(GL_LIGHT1, GL_POSITION, light1_pos);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, blue);
}

void keyboard(unsigned char key, int x, int y) {
    if(key == 'w') eyeZ -= 1.0;
    if(key == 's') eyeZ += 1.0;
    if(key == '1') light0 = !light0;
    if(key == '2') light1 = !light1;
    glutPostRedisplay();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (float)w/h, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

void idle() {
    rotateCCTV += 1.2;
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1200, 800);
    glutCreateWindow("Realistic IMAX Cineplex - Momin");
    init();
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle);
    glutMainLoop();
    return 0;
}
