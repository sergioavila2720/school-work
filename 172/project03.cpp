/*
Sergio Avila 
ID: 108975197
Csci: 172

Compile: g++ project03.cpp -o run3 -lGL -lGLU -lglut
run:     ./run2

In this program we will use Quartenions. we will rotate the teapot from last project 
but we will use quartenions to do it. we wont use the built in function.
*/



#include <string.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <iostream>
#include <math.h>

using namespace std;

#define PI 3.14159

bool WireFrame= false;

const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };

//create matrix for each dimencion 
GLfloat matrix_X[16];
GLfloat matrix_Y[16];
GLfloat matrix_Z[16];
GLfloat x, y, z, w;


static GLint Rotate_X = 0;
static GLint Rotate_Y = 0;
static GLint Rotate_Z = 0;

static void resize(int width, int height)
{
     double Ratio;

   if(width<=height)
     {
         glViewport(0,(GLsizei) (height-width)/2,(GLsizei) width,(GLsizei) width);
         Ratio =  height/width;
      }
    else
      {
        glViewport((GLsizei) (width-height)/2 ,0 ,(GLsizei) height,(GLsizei) height);
        Ratio = width /height;
      }

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	gluPerspective (50.0f,Ratio,0.1f, 100.0f);
}

void CreateFromAxisAngle(GLfloat X, GLfloat Y, GLfloat Z, GLfloat degree){

    // convert degrees to radians
    GLfloat angle = (GLfloat)((degree / 180.0f) * PI);
    // calculate half angle for sin
    GLfloat result = (GLfloat)sin(angle / 2.0f);
    //calculate half angle for cos
    w = (GLfloat)cos(angle / 2.0f);

    // get quaternion i, j, k
    x = (GLfloat)(X * result);
    y = (GLfloat)(Y * result);
    z = (GLfloat)(Z * result);
}

// here we create the quartenion rotation matrix 
void CreateMatrix(GLfloat *pMatrix){

    //first row
    pMatrix[0] = 1.0f - 2.0f * (y*y + z*z);
    pMatrix[1] = 2.0f * (x*y + z*w);
    pMatrix[2] = 2.0f * (x*z - y*w);
    pMatrix[3] = 0.0f;

    //second row
    pMatrix[4] = 2.0f * (x*y - z*w);
    pMatrix[5] = 1.0f - 2.0f * (x*x + z*z);
    pMatrix[6] = 2.0f * (z*y + x*w);
    pMatrix[7] = 0.0f;

    //third row
    pMatrix[8] = 2.0f * (x*z + y*w);
    pMatrix[9] = 2.0f * (y*z - x*w);
    pMatrix[10] = 1.0f - 2.0f * (x*x + y*y);
    pMatrix[11] = 0.0f;

    //fourth row
    pMatrix[12] = 0.0f;
    pMatrix[13] = 0.0f;
    pMatrix[14] = 0.0f;
    pMatrix[15] = 1.0f;

}

static void display(void)
{

    double yRotationAngle;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(0,5,10,0.0,0.0,0.0,0.0,1.0,0.0);

    if(WireFrame)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);      //Draw Our Mesh In Wireframe Mesh
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);      //Toggle WIRE FRAME


    // dependencies for matrix_x
    CreateMatrix(matrix_X);
    CreateFromAxisAngle(1,0,0,Rotate_X);
    glMultMatrixf(matrix_X);

    // dependencies for matrix_y
    CreateMatrix(matrix_Y);
    CreateFromAxisAngle(0,1,0,Rotate_Y);
    glMultMatrixf(matrix_Y);

    // dependencies for matrix_z
    CreateMatrix(matrix_Z);
    CreateFromAxisAngle(0,0,1,Rotate_Z);
    glMultMatrixf(matrix_Z);

    // random colors for the objects
    glPushMatrix();
    glColor3f(0.0, 1.0, 0.0);
    glTranslated(0.0, 0.0, 0.0);
    glutSolidTeapot(1.5);
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

        case 'w':
            Rotate_X = (Rotate_X + 5) % 360;
            break;

        case 's':
            Rotate_X = (Rotate_X - 5) % 360;
            break;

        case 'a':
            Rotate_Y = (Rotate_Y + 5) % 360;
            break;

        case 'd':
            Rotate_Y = (Rotate_Y -5) % 360;
            break;

        case 'z':
            Rotate_Z = (Rotate_Z + 5) % 360;
            break;

        case 'x':
            Rotate_Z = (Rotate_Z - 5) % 360;
    }
    
}
static void idle(void)
{
    glutPostRedisplay();
}

static void init(void)
{
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);

    glEnable(GL_DEPTH_TEST);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glShadeModel(GL_SMOOTH);

    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);
}


/* Program entry point */

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);

    glutInitWindowSize(800,600);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("GLUT Shapes");
    init();
    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    //glutSpecialFunc(Specialkeys);

    glutIdleFunc(idle);
    glutMainLoop();

    return EXIT_SUCCESS;
}
