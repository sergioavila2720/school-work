/*
Sergio Avila 
ID: 108975197
Csci: 172
In this program we make use of the GLUT libraries to make a fractal 
I played around with the code to try and see what everything does
because i have never used GLUT before.
Compile: g++ project01.cpp -o run1 -lGL -lGLU -lglut
run:     ./run1
*/

#include <string.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
/* GLUT callback Handlers */

int n = 200000;
double Px, Py;
double tempx, tempy;

void resize(int width, int height)
{
    if(width<=height)
        glViewport(0,(GLsizei) (height-width)/2,(GLsizei) width,(GLsizei) width);

    else
        glViewport((GLsizei) (width-height)/2 ,0 ,(GLsizei) height,(GLsizei) height);
}


void init()
{
 glMatrixMode(GL_PROJECTION);
 glLoadIdentity();
 glOrtho(-2.5,2.75,10.5,0, -1.0, 1.0); // adjusted for suitable viewport

}

void display(void)
{
  glClear (GL_COLOR_BUFFER_BIT); 
  // changed the background color to white
  glClearColor ( 1.0, 1.0, 1.0, 1.0 );       // clear display screen
  // changed leaf color to green 
  glColor3f(0.133, 0.545, 0.133);

  //implementing the algorithm given on prompt
  int num;
    for(int i = 0; i < n; i++)
    {
        num = rand()%100+1;
        //implementing the  < 85 chance with f2
        if(num < 85)
        {
            Px = 0.85 * tempx + 0.04 * tempy;
            Py = -0.04 * tempx + 0.85 * tempy + 1.6;
        }
        //implementing the  < 92 chance with f3
        else if(num < 92)
        {
            Px = 0.2 * tempx - 0.26 * tempy;
            Py = 0.23 * tempx + 0.22 * tempy + 1.6;
        }
        //implementing the < 99 chance with f4
        else if(num < 99)
        {
            Px = -0.15 * tempx + 0.28 * tempy;
            Py = 0.26 * tempx + 0.24 * tempy + 0.44;
        }
        //else we implemeting f1
        else
        {
            Px = 0;
            Py = 0.16 * tempy;
        }

        //starting to plot the points given
        glBegin(GL_POINTS);
        glVertex3f(Px, Py, 0);
        //ending
        glEnd();

        tempx = Px;
        tempy = Py;
    }

  glFlush ();                           // clear buffer
}


void key(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27 :                       // esc key to exit
        case 'q':
            exit(0);
            break;
    }

    glutPostRedisplay();
}


/* Program entry point */

int main(int argc, char *argv[])
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
   glutInitWindowSize (250, 250);                //window screen
   glutInitWindowPosition (100, 100);            //window position
   glutCreateWindow ("Program1");                //program title
   init();
   glutDisplayFunc(display);                     //callback function for display
   glutReshapeFunc(resize);                      //callback for reshape
   glutKeyboardFunc(key);                        //callback function for keyboard
   glutMainLoop();                               //loop

    return EXIT_SUCCESS;
}