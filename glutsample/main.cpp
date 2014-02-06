/*
 * OpenGLSamples (openglsamples.sf.net) Examples
 * VC++ users should create a Win32 Console project and link
 * the program with glut32.lib, glu32.lib, opengl32.lib
 *
 * GLUT can be downloaded from http://www.xmission.com/~nate/glut.html
 * OpenGL is by default installed on your system.
 * For an installation of glut on windows for MS Visual Studio 2010 see: http://nafsadh.wordpress.com/2010/08/20/glut-in-ms-visual-studio-2010-msvs10/
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#include <math.h>
#define KEY_ESCAPE 27
#define PI 3.141592
#define D360 (PI*2)
#define A2R(a) (D360*(a/360))

typedef float* p2d;

static int beziersmoothness = 100;
static int pointnum=20;
static float linewidth = 1;
static p2d* points;
static unsigned int exlinesnum=10;
static p2d** exlines;


static int moonsmoothness = 30;
static int moonpnum=6;
static float moonradius = 0.4;
static p2d* moonpoints;
static unsigned int exmoonlinesnum=10;
static p2d** moonexlines;


p2d createp2d(float x, float y)
{
		p2d p = (p2d)malloc(sizeof(float)*2);
		
		p[0]=x;
		p[1]=y;
		
		return p;
}

void markAt(p2d *arr, unsigned int num)
{
		int i;
		static float ra=0;
		ra +=0.1f;
		for(i=0;i<num;i++)
		{
				glPushMatrix();
				
				
				glTranslatef(arr[i][0], arr[i][1], 0);
				glRotatef(ra, 0, 0, 1);
				glBegin(GL_TRIANGLES);
				
				glColor3f(0.0f,0.0f,1.0f);
				
				glVertex3f( 0.0f, 0.1f * sqrt(3)*(2.f/3), 0.0f);
				glColor3f(0.0f,1.f,0.0f);
				glVertex3f(-0.1f,-0.1f * sqrt(3)*(1.f/3), 0.0f);
				glColor3f(1.0f,0.0f,0.0f);
				glVertex3f( 0.1f,-0.1f * sqrt(3)*(1.f/3), 0.0f);
				
				glEnd();
				
				glPopMatrix();
		}
}
p2d linearInterpolate(p2d p1, p2d p2, float f)
{
		return createp2d(p1[0]*(1-f) + p2[0]*f, p1[1]*(1-f) + p2[1]*f );
}
p2d copyp(p2d p)
{
		return createp2d(p[0], p[1]);
}
p2d* copyp2darr(p2d* arr, unsigned int num)
{
		p2d* ret = (p2d*)malloc(sizeof(p2d)*num);
		
		int i=0;
		for(i=0;i<num;i++)
		{
				ret[i] = createp2d(arr[i][0], arr[i][1]);
		}
		
		return ret;
}
void releasp2d(p2d p)
{
		free(p);

}
void releasep2darr(p2d* arr, unsigned int num)
{
		int i;
		for(i=0;i<num;i++)
				releasp2d(arr[i]);
		free(arr);

}

p2d bezierInterpolation(p2d* arr, unsigned int pnum, float f)
{
		unsigned int linecnt = pnum-1;
		p2d* tmparr;
		p2d tmpp;
		int i;
		
		tmparr = copyp2darr(arr, pnum);
		
		while(linecnt > 0)
		{
				for(i=1;i<linecnt+1;i++)
				{
						tmpp = linearInterpolate(tmparr[i-1], tmparr[i], f);
						releasp2d(tmparr[i-1]);
						tmparr[i-1]=tmpp;
				}
				
				--linecnt;
		}
		
		tmpp = copyp(tmparr[0]);
		releasep2darr(tmparr, pnum);
		return tmpp;
}

void drawBezier(p2d* bezierpoints, unsigned int num, int smooth)
{
		int i;
		p2d tmpp;
		glBegin(GL_LINE_STRIP);
		
		for(i=0;i<=smooth;i++)
		{
				tmpp = bezierInterpolation(bezierpoints, num, ((float)i)/smooth);
				
				glVertex3f(tmpp[0], tmpp[1], 0);
				releasp2d(tmpp);
		}
		glEnd();
}
typedef struct {
		int width;
		int height;
		char* title;
		
		float field_of_view_angle;
		float z_near;
		float z_far;
} glutWindow;

glutWindow win;

void update()
{
		int i=0, j;
		int randres;
		int tmp = -1;
		
		for (i=0;i<exlinesnum;i++)
		{
				exlines[i] = copyp2darr(points, pointnum);
		}
		
		for (i=0;i<pointnum;i++)
		{
				randres = rand()%2;
				tmp = powf(-1, (float)randres);
				
				points[i][1] += tmp*0.04f;
				
				for(j=0;j<exlinesnum;j++)
				{
						//exlines[j][i][1] += tmp* 0.04*(pow(j*5,1 + ((float)j/exlinesnum)*0.5)) * ((i!=pointnum-1) & 1);
						exlines[j][i][1] += tmp* 0.04*(j*1) * ((i%(pointnum-1)) & 1);
				}
		}
		
		
		for (i=0;i<exmoonlinesnum;i++)
		{
				moonexlines[i] = copyp2darr(moonpoints, moonpnum);
		}
		
		for (i=0;i<moonpnum;i++)
		{
				randres = rand()%2;
				tmp = powf(-1, (float)randres);
				
				moonpoints[i][0] += tmp*0.01f;
				
				for(j=0;j<exmoonlinesnum;j++)
				{

						moonexlines[j][i][0] += tmp* 0.02*(pow(j*1,1 + ((float)j/exmoonlinesnum)*0.5))* ((i%(moonpnum-1)) & 1);
				}
		}
}

void drawfinish()
{
		int i;
		for(i=0;i<exlinesnum;i++)
		{
				releasep2darr(exlines[i], pointnum);
		}
		
		
		for(i=0;i<exmoonlinesnum;i++)
		{
				releasep2darr(moonexlines[i], moonpnum);
		}
}
void display()
{
		int i;
		update();
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		     // Clear Screen and Depth Buffer
		glLoadIdentity();
		glTranslatef(0.0f,0.0f,-10.0f);
		
		/*
		 * Triangle code starts here
		 * 3 verteces, 3 colors.
		 */
		
		//markAt(points, pointnum);
		//markAt(moonpoints, moonpnum);
		
		glPushMatrix();
		glTranslatef(-3, 1, 0);
		
		glColor3f(0.9, 0.9, 0.3);
		drawBezier(moonpoints, moonpnum, moonsmoothness);
		for(i=0;i<exmoonlinesnum;i++)
				drawBezier(moonexlines[i], moonpnum, moonsmoothness);
		glPopMatrix();
		
		glColor3f(0.2, 0.3, 0.6);
		drawBezier(points, pointnum, beziersmoothness);
		for(i=0;i<exlinesnum;i++)
				drawBezier(exlines[i], pointnum, beziersmoothness);
		
		
		glutSwapBuffers();
		drawfinish();
}


void initialize ()
{
		int i;
		float angle=(30.f/360)*2*(3.141592);
		float range = 8;
		glMatrixMode(GL_PROJECTION);												// select projection matrix
		glViewport(0, 0, win.width, win.height);									// set the viewport
		glMatrixMode(GL_PROJECTION);												// set matrix mode
		glLoadIdentity();															// reset projection matrix
		GLfloat aspect = (GLfloat) win.width / win.height;
		gluPerspective(win.field_of_view_angle, aspect, win.z_near, win.z_far);		// set up a perspective projection matrix
		glMatrixMode(GL_MODELVIEW);													// specify which matrix is the current matrix
		glShadeModel( GL_SMOOTH );
		glClearDepth( 1.0f );														// specify the clear value for the depth buffer
		glEnable( GL_DEPTH_TEST );
		glDepthFunc( GL_LEQUAL );
		glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );						// specify implementation-specific hints
		glClearColor(0.0, 0.0, 0.0, 1.0);											// specify clear values for the color buffers

		points = (p2d*)malloc(sizeof(p2d)*pointnum);
		
		for (i=0; i<pointnum; i++)
		{
				points[i] = createp2d(linewidth * (i-pointnum/2), powf(-1,  (float)(i%2))* abs( ((float)(rand()%((int)range*10)))/(range*10)*range - range/2));
		}
		exlines = (p2d**)malloc(sizeof(p2d*)*exlinesnum);
		
		
		moonpoints = (p2d*)malloc(sizeof(p2d)*moonpnum);
		for(i=0;i<moonpnum;i++)
		{
				moonpoints[i] = createp2d(cosf(angle + A2R(60.f)*i) * moonradius, sinf(angle+A2R(60.f)*i) * moonradius);
				printf("%f %f\n", moonpoints[i][0], moonpoints[i][1]);
		}

		moonpoints[0][1]*=0.8;
		moonpoints[moonpnum-1][1]*=0.8;
		moonpoints[0][0]*=0.5;
		moonpoints[moonpnum-1][0]*=0.5;
		
		moonpoints[2][0] *= 1.4f;
		moonpoints[3][0] *= 1.4f;
		moonexlines = (p2d**)malloc(sizeof(p2d*)*exmoonlinesnum);
}


void keyboard ( unsigned char key, int mousePositionX, int mousePositionY )
{
		switch ( key )
		{
				case KEY_ESCAPE:
						//exit ( 0 );
						
						break;
						
				default:
						break;
		}
}

int main(int argc, char **argv)
{
		int i;
		// set window values
		win.width = 2048;
		win.height = 512;
		win.title = "OpenGL/GLUT Example. Visit http://openglsamples.sf.net ";
		win.field_of_view_angle = 45;
		win.z_near = 1.0f;
		win.z_far = 500.0f;
		
		// initialize and run program
		glutInit(&argc, argv);                                      // GLUT initialization
		glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );  // Display Mode
		glutInitWindowSize(win.width,win.height);					// set window size
		glutCreateWindow(win.title);								// create Window
		glutDisplayFunc(display);									// register Display Function
		glutIdleFunc( display );									// register Idle Function
		glutKeyboardFunc( keyboard );								// register Keyboard Handler
		initialize();
		glutMainLoop();												// run GLUT mainloop
		
		releasep2darr(points, pointnum);
		for(i=0;i<exlinesnum;i++)
				releasep2darr(exlines[i], pointnum);
		free(exlines);
		return 0;
}