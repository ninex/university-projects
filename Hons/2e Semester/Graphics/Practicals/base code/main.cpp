

#include <windows.h>											// Header File For Windows
#include <gl\gl.h>												// Header File For The OpenGL32 Library
#include <gl\glu.h>												// Header File For The GLu32 Library
#include <stdio.h>
#include "NeHeGL.h"												// Header File For NeHeGL

#pragma comment( lib, "opengl32.lib" )							// Search For OpenGL32.lib While Linking
#pragma comment( lib, "glu32.lib" )								// Search For GLu32.lib While Linking

#ifndef CDS_FULLSCREEN											// CDS_FULLSCREEN Is Not Defined By Some
#define CDS_FULLSCREEN 4										// Compilers. By Defining It This Way,
#endif															// We Can Avoid Errors

GL_Window*	g_window;
Keys*		g_keys;

// User Defined Variables
float		angle,zoom;												// Used To Rotate The Triangles
int			rot1, rot2;											// Counter Variables
float map[257][257];
//float 

float frand(){
      return (float)rand()/RAND_MAX/2;
}

float noise(int range){
    float thres = (float)range/257;
    float temp = frand();
    return temp*thres - (temp*thres)/2;
}

void midp(int x1,int y1, int x2, int y2){
     if ((x2-x1) <= 1){
        return;
     }
     
     // top
     if (map[(x1+x2)/2][y1] < 0){
        map[(x1+x2)/2][y1] = (map[x1][y1]+map[x2][y1])/2 + noise(x2-x1);
        map[(x1+x2)/2][y1] < 0 ? map[(x1+x2)/2][y1] = 0:0;
        map[(x1+x2)/2][y1] > 1 ? map[(x1+x2)/2][y1] = 1:0;
     }
     //right
     if (map[x2][(y1+y2)/2] < 0){
        map[x2][(y1+y2)/2] = (map[x2][y1]+map[x2][y2])/2 + noise(x2-x1);
        map[x2][(y1+y2)/2] < 0 ? map[x2][(y1+y2)/2] = 0:0;
        map[x2][(y1+y2)/2] > 1 ? map[x2][(y1+y2)/2] = 1:0;
     }
     //bottom
     if (map[(x1+x2)/2][y2] < 0){
        map[(x1+x2)/2][y2] = (map[x2][y2]+map[x1][y2])/2 + noise(x2-x1);
        map[(x1+x2)/2][y2] < 0 ? map[(x1+x2)/2][y2] = 0:0;
        map[(x1+x2)/2][y2] > 1 ? map[(x1+x2)/2][y2] = 1:0;
     }
     //left
     if (map[x1][(y1+y2)/2] < 0){
        map[x1][(y1+y2)/2] = (map[x1][y2]+map[x1][y1])/2+  + noise(x2-x1);
        map[x1][(y1+y2)/2] < 0 ? map[x1][(y1+y2)/2] = 0:0;
        map[x1][(y1+y2)/2] > 1 ? map[x1][(y1+y2)/2] = 1:0;
     }
     //middle
     map[(x1+x2)/2][(y1+y2)/2] = (map[x1][y1]+map[x2][y1]+map[x2][y2]+map[x1][y2])/4 + noise(x2-x1);     
     map[(x1+x2)/2][(y1+y2)/2] < 0 ? map[(x1+x2)/2][(y1+y2)/2] = 0:0;
     map[(x1+x2)/2][(y1+y2)/2] > 1 ? map[(x1+x2)/2][(y1+y2)/2] = 1:0;
     
     midp(x1,y1,(x1+x2)/2,(y1+y2)/2);
     midp((x1+x2)/2,y1,x2,(y1+y2)/2);
     midp(x1,(y1+y2)/2,(x1+x2)/2,y2);
     midp((x1+x2)/2,(y1+y2)/2,x2,y2);
}


void setupTerrain(){
     int i,j;
     //clear map
     for (i=0;i<257;i++){
         for (j=0;j<257;j++){
             map[i][j] = -1.0f;
         }
     }
     //setup initial corners
    
     map[0][0] = frand();
     map[0][256]= frand();
     map[256][0]= frand();
     map[256][256] = frand();
      
     midp(0,0,256,256);
}



BOOL Initialize (GL_Window* window, Keys* keys)					// Any GL Init Code & User Initialiazation Goes Here
{
	g_window	= window;
	g_keys		= keys;

	// Start Of User Initialization
	angle		= 0.0f;											// Set Starting Angle To Zero
	zoom        = -2.0f;
    setupTerrain();
    /*GLfloat LightAmbient[]= { 0.5f, 0.5f, 0.5f, 1.0f };
    GLfloat LightPosition[]= { 0.0f, 0.0f, 2.0f, 1.0f };
    glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient); // Setup The Ambient Light*/

	glClearColor (0.0f, 0.0f, 0.0f, 0.5f);						// Black Background
	glClearDepth (1.0f);										// Depth Buffer Setup
	glDepthFunc (GL_LEQUAL);									// The Type Of Depth Testing (Less Or Equal)
	glEnable (GL_DEPTH_TEST);									// Enable Depth Testing
	glShadeModel (GL_SMOOTH);									// Select Smooth Shading
	glHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);			// Set Perspective Calculations To Most Accurate
    
    //glEnable(GL_LIGHT1);
	
    return TRUE;												// Return TRUE (Initialization Successful)
}

void Deinitialize (void)										// Any User DeInitialization Goes Here
{
}

void Update (DWORD milliseconds)								// Perform Motion Updates Here
{
	if (g_keys->keyDown [VK_ESCAPE] == TRUE)					// Is ESC Being Pressed?
	{
		TerminateApplication (g_window);						// Terminate The Program
	}

	if (g_keys->keyDown [VK_F1] == TRUE)						// Is F1 Being Pressed?
	{
		ToggleFullscreen (g_window);							// Toggle Fullscreen Mode
	}
    if (g_keys->keyDown [VK_SPACE] == TRUE)						// Is F1 Being Pressed?
	{
		setupTerrain();
	}
    
    if (g_keys->keyDown [VK_UP] == TRUE)						// Is F1 Being Pressed?
	{
		angle += (float)(milliseconds) / 5.0f;						// Update angle Based On The Clock
	}
	if (g_keys->keyDown [VK_DOWN] == TRUE)						// Is F1 Being Pressed?
	{
		angle -= (float)(milliseconds) / 5.0f;						// Update angle Based On The Clock
	}
	
    if (g_keys->keyDown [VK_HOME] == TRUE)						// Is F1 Being Pressed?
	{
		zoom -= 0.5f;						// Update angle Based On The Clock
	}
	if (g_keys->keyDown [VK_END] == TRUE)						// Is F1 Being Pressed?
	{
		zoom += 0.5f;						// Update angle Based On The Clock
	}
}

void Draw (void)
{
	float col,fi,fj;
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear Screen And Depth Buffer
	glLoadIdentity ();											// Reset The Modelview Matrix
	glTranslatef (-0.5f, -0.5f, zoom);							// Translate 6 Units Into The Screen
	glRotatef (angle, -1.0f, 0.0f, 0.0f);						// Rotate On The Y-Axis By angle
	
	int i,j;
	
    for (i=0;i<256;i++){
        for (j=0;j<256;j++){
	        
	        fi = ((float)i)/256;
	        fj = ((float)j)/256;
            glBegin (GL_TRIANGLES);								// Begin Drawing Triangles
                    col = map[i][j];
                    glNormal3f(0.f, 0.f, 1.0f);
                    glColor3f (col, col, col);	glVertex3f( fi, fj, 0.f);
                    
                    col = map[i+1][j];
                    glNormal3f(0.f, 0.f, 1.0f);
            		glColor3f (col, col, col);	glVertex3f( fi+1.0/256,fj, 0.f);
            		
            		col = map[i+1][j+1];
            		glNormal3f(0.f, 0.f, 1.0f);
            		glColor3f (col, col, col);	glVertex3f( fi+1.0/256,fj+1.0/256,0.f);
            		
                    
            		
            glEnd ();
            glBegin (GL_TRIANGLES);								// Begin Drawing Triangles
                    col = map[i][j];
                    glNormal3f(0.f, 0.f, 1.0f);
                    glColor3f (col, col, col);	glVertex3f( fi, fj, 0.f);
                    
            		
            		col = map[i+1][j+1];
            		glNormal3f(0.f, 0.f, 1.0f);
            		glColor3f (col, col, col);	glVertex3f( fi+1.0/256,fj+1.0/256,0.f);
            		
                    col = map[i][j+1];
                    glNormal3f(0.f, 0.f, 1.0f);
            		glColor3f (col, col, col);	glVertex3f(fi,fj+1.0/256, 0.f);
            		
            		
            glEnd ();											// Done Drawing Triangles											// Done Drawing Triangles
        }
    }
	

	glFlush ();													// Flush The GL Rendering Pipeline
}
