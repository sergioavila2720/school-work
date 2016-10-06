/*
Sergio Avila 
ID: 108975197
Csci: 172

Compile: g++ project02.cpp -o run2 -lGL -lGLU -lglut
run:     ./run2

In this program we will create two three objects; a teapot, a cube and a sphere.
we will make use of the glut library and some functions to create those objects. 
As specified in the prompt we have x, y, z values to put them in the window. we will be able 
to zoom in and out specific objects and rotate them as well. 
*/



#include <string.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <iostream>

using namespace std;

bool WireFrame= false;

const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };


/* GLUT callback Handlers */

/*Made these variables global because I'm going to need
  them for zooming in and out and rotate the values of the objects*/
GLdouble T_trans = 2.0;
GLdouble C_trans = -3.0;
GLdouble S_trans = -1.0;
GLdouble T_rotate = 0.0;
GLdouble C_rotate = 0.0;
GLdouble S_rotate = 0.0;

/*need these flags to be able to combine keys and special keys*/
bool T_flag = false;
bool C_flag = false;
bool S_flag = false;


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

static void display(void)
{

    double yRotationAngle;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(0,5,10,0.0,0.0,0.0,0.0,1.0,0.0);

    if(WireFrame)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);		//Draw Our Mesh In Wireframe Mesh
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);		//Toggle WIRE FRAME

    // your code here

    // random colors for the objects
    glPushMatrix();
    glTranslatef(0.0, 0.0, T_trans);
    glRotatef(T_rotate, 0.0, 5.0, 0.0);
    glColor3f(1.0, 0.0, 0.0);
    glutSolidTeapot(1.5);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(4.0, 0.0, C_trans);
    glRotatef (C_rotate, 0.0, 5.0, 0.0);
    glColor3f(0.0, 1.0, 0.0);
    glutSolidCube(3.0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-3.5, 0.0, S_trans);
    glRotatef (S_rotate, 0.0, 5.0, 0.0);
    glColor3f(0.0, 0.0, 0.804);
    glutSolidSphere(1.3, 24, 24);
    glPopMatrix();
    glutSwapBuffers();
}

// function for regular key detection
static void key(unsigned char key, int x, int y)
{  


    switch (key)
    {
        case 27 :
        case 'q':
            exit(0);
            break;

        case 't':
            T_flag = true;
            C_flag = false;
            S_flag = false;
            break;

        case 'c':
            C_flag = true;
            T_flag = false;
            S_flag = false;
            break;

        case 's':
            S_flag = true;
            T_flag = false;
            C_flag = false;
            break;
    }
    
}

// function for special key detection
void Specialkeys(int key, int x, int y)
{
    
    switch(key)
    {
        case GLUT_KEY_UP:
            if (T_flag) T_trans += 0.5; 
            else if(C_flag) C_trans += 0.5; 
            else if(S_flag) S_trans += 0.5; 
            break;
    

        case GLUT_KEY_DOWN:
            if (T_flag) T_trans -= 0.5; 
            else if(C_flag) C_trans -= 0.5; 
            else if(S_flag) S_trans -= 0.5; 
            break;


        case GLUT_KEY_LEFT:
            if (T_flag) T_rotate += 5.0; 
            else if(C_flag) C_rotate += 5.0; 
            else if(S_flag) S_rotate += 5.0; 
            break;
        

        case GLUT_KEY_RIGHT:
            if (T_flag) T_rotate -= 5.0; 
            else if(C_flag) C_rotate -= 5.0; 
            else if(S_flag) S_rotate -= 5.0; 
            break;
        
    }     
   
  glutPostRedisplay();
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
    glutSpecialFunc(Specialkeys);

    glutIdleFunc(idle);
    glutMainLoop();

    return EXIT_SUCCESS;
}
