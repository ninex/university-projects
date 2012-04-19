#include <windows.h>											// Header File For Windows
#include <gl\gl.h>												// Header File For The OpenGL32 Library
#include <gl\glu.h>												// Header File For The GLu32 Library
#include <stdio.h>
#include <math.h>
#include "NeHeGL.h"												// Header File For NeHeGL

#pragma comment( lib, "opengl32.lib" )							// Search For OpenGL32.lib While Linking
#pragma comment( lib, "glu32.lib" )								// Search For GLu32.lib While Linking

#ifndef CDS_FULLSCREEN											// CDS_FULLSCREEN Is Not Defined By Some
#define CDS_FULLSCREEN 4										// Compilers. By Defining It This Way,
#endif															// We Can Avoid Errors

GL_Window*	g_window;
Keys*		g_keys;
struct Point3f
{
GLfloat x, y, z;
};
// User Defined Variables
float		angle,zoom,lowest,lrot, rot1;												// Used To Rotate The Triangles
int			lod;											// Counter Variables
float map[257][257];
Point3f norm[256][256];
Point3f norm2[256][256];

GLfloat LightAmbient[]= { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat LightPosition[]= { 0.0f, 0.0f, 2.0f, 1.0f };

float frand(){
      return (float)rand()/RAND_MAX/2;
}

float noise(int range){
    float thres = (float)range/257;
    float temp = frand();
    return temp*thres - (temp*thres)/2;
}

void calculateNormal(int i, int j){
    float a,b,c,x,y,z;
    a = j*map[i+1][j] - map[i][j]*j;
    b = -(i*map[i+1][j] - map[i][j]*(i+1));
    c = i*j - j*(i+1);
    
    x = b*map[i+1][j+1] - c*(j+1);
    y = - (a*map[i+1][j+1] - c*(i+1));
    z = a*(j+1) - b*(i+1);
    
    norm[i][j].x = x / sqrt(pow(x,2) + pow(y,2) + pow(z,2));
    norm[i][j].y = y/ sqrt(pow(x,2) + pow(y,2) + pow(z,2));
    norm[i][j].z = z/ sqrt(pow(x,2) + pow(y,2) + pow(z,2));
    
    a = j*map[i][j+1] - map[i][j]*(j+1);
    b = -(i*map[i][j+1] - map[i][j]*(i));
    c = i*(j+1) - j*i;
    
    x = b*map[i+1][j+1] - c*(j+1);
    y = - (a*map[i+1][j+1] - c*(i+1));
    z = a*(j+1) - b*(i+1);
    
    norm2[i][j].x = x / sqrt(pow(x,2) + pow(y,2) + pow(z,2));
    norm2[i][j].y = y/ sqrt(pow(x,2) + pow(y,2) + pow(z,2));
    norm2[i][j].z = z/ sqrt(pow(x,2) + pow(y,2) + pow(z,2));
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
     for (i=0;i<256;i++){
         for (j=0;j<256;j++){
             calculateNormal(i,j);
         }
     }
     lowest = 1;
     for (i=0;i<257;i++){
         for (j=0;j<257;j++){
             if (map[i][j] < lowest){
                lowest = map[i][j];
             }
         }
     }
}


BOOL Initialize (GL_Window* window, Keys* keys)					// Any GL Init Code & User Initialiazation Goes Here
{
	g_window	= window;
	g_keys		= keys;

	// Start Of User Initialization
	angle		= 40.0f;											// Set Starting Angle To Zero
	zoom        = -2.0f;
    rot1 = 0;
    lod = 1;
    lrot = 0.f;
    setupTerrain();
    
    
    

	glClearColor (0.0f, 0.0f, 0.0f, 0.5f);						// Black Background
	glClearDepth (1.0f);										// Depth Buffer Setup
	glDepthFunc (GL_LEQUAL);									// The Type Of Depth Testing (Less Or Equal)
	glEnable (GL_DEPTH_TEST);									// Enable Depth Testing
	glShadeModel (GL_SMOOTH);									// Select Smooth Shading
	glHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);			// Set Perspective Calculations To Most Accurate
    
    glLightfv(GL_LIGHT1, GL_DIFFUSE, LightAmbient); // Setup The Ambient Light
    glEnable(GL_LIGHT1);
	
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
		angle += 5.0f;						// Update angle Based On The Clock
	}
	if (g_keys->keyDown [VK_DOWN] == TRUE)						// Is F1 Being Pressed?
	{
		angle -= 5.0f;						// Update angle Based On The Clock
	}
	
	if (g_keys->keyDown [VK_LEFT] == TRUE)						// Is F1 Being Pressed?
	{
		lrot += 1.0f;						// Update angle Based On The Clock
	}
	if (g_keys->keyDown [VK_RIGHT] == TRUE)						// Is F1 Being Pressed?
	{
		lrot -= 1.0f;						// Update angle Based On The Clock
	}
	
    if (g_keys->keyDown [VK_HOME] == TRUE)						// Is F1 Being Pressed?
	{
		zoom -= 0.5f;						// Update angle Based On The Clock
	}
	if (g_keys->keyDown [VK_END] == TRUE)						// Is F1 Being Pressed?
	{
		zoom += 0.5f;						// Update angle Based On The Clock
	}
	if (g_keys->keyDown [VK_SUBTRACT] == TRUE)						// Is F1 Being Pressed?
	{
        if (lod < 64){
    		lod *= 2;						// Update angle Based On The Clock
        }
	}
	if (g_keys->keyDown [VK_ADD] == TRUE)						// Is F1 Being Pressed?
	{
		if (lod > 1){
            lod /= 2;						// Update angle Based On The Clock
        }
	}
}

void drawColor(float col){
    if (col < 0.25){
        glColor3f(0,0,col);
    }else{
        if (col < 0.35){
            glColor3f(col*2/3,col/3,0);
        }else{
                glColor3f(0,col*4/3,0);
        }
    }
}

void Draw (void)
{
	float col,fi,fj,flod;
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear Screen And Depth Buffer
	glLoadIdentity ();											// Reset The Modelview Matrix
	
	glTranslatef (0, 0, zoom);							// Translate 6 Units Into The Screen
	glRotatef (rot1, 0.f,1.f,0.0f);
	glTranslatef (-0.5f, -0.5f, 0);							// Translate 6 Units Into The Screen
	glRotatef (angle, -1.0f, 0.0f, 0.0f);						// Rotate On The Y-Axis By angle
	
	
	int i,j;
	
    for (i=0;i<256;i+=lod){
        for (j=0;j<256;j+=lod){
	        
	        fi = ((float)i)/256;
	        fj = ((float)j)/256;
	        flod = (float)lod;
            glBegin (GL_TRIANGLES);								// Begin Drawing Triangles
                    col = map[i][j];
                    glNormal3f(norm[i][j].x, norm[i][j].y, norm[i][j].z);
                    drawColor(col);	glVertex3f( fi, fj, col);
                    
                    col = map[i+lod][j];
                    drawColor(col);	glVertex3f( fi+flod/256,fj, col);
            		
            		col = map[i+lod][j+lod];
            		drawColor(col);	glVertex3f( fi+flod/256,fj+flod/256,col);
            		
                    
            		
            //glEnd ();
            //glBegin (GL_TRIANGLES);								// Begin Drawing Triangles
                    col = map[i][j];
                    //glNormal3f(0.f, 1.f, 0.0f);
                    glNormal3f(norm2[i][j].x, norm2[i][j].y, norm2[i][j].z);
                    drawColor(col);	glVertex3f( fi, fj, col);
                    
            		
            		col = map[i+lod][j+lod];
            		drawColor(col);	glVertex3f( fi+flod/256,fj+flod/256,col);
            		
                    col = map[i][j+lod];
            		drawColor(col);	glVertex3f(fi,fj+flod/256, col);
            		
            		
            glEnd ();
        }
        											// Done Drawing Triangles
            
    }
	       glBegin (GL_QUADS);
                glNormal3f(0.f,0.f,1.0f);
                drawColor(0.1f);
                
                glVertex3f( 0.f, 0.f, lowest);
                glVertex3f( 1.0f, 0.f, lowest);
                glVertex3f( 1.0f, 1.0f, lowest);
                glVertex3f( 0.f, 1.0f, lowest);
                
            glEnd();
            //left and right sides
            for (i = 0;i<257;i+=256){
                for (j=0;j<256;j+=lod){
                    fi = ((float)i)/256;
    	            fj = ((float)j)/256;
                    glBegin(GL_QUADS);
                
                        drawColor(0.f);
                        col = map[i][j];
                        glVertex3f(fi,fj,lowest);
                
                        col = map[i][j];
                        drawColor(col);
                        glVertex3f(fi,fj,col);
                    
                        col = map[i][j+lod];
                        drawColor(col);
                        glVertex3f(fi,fj+flod/256,col);
                    
                        drawColor(0.f);
                        col = map[i][j+lod];
                        glVertex3f(fi,fj+flod/256,lowest);
                
                    glEnd();
                }
            }
            
            //top and bottom sides
            
            for (j = 0;j<257;j+=256){
                for (i=0;i<256;i+=lod){
                    fi = ((float)i)/256;
	                fj = ((float)j)/256;
                    glBegin(GL_QUADS);
                        col = map[i][j];
                        drawColor(col);
                        glVertex3f(fi,fj,lowest);
                
                        col = map[i][j];
                        drawColor(col);
                        glVertex3f(fi,fj,col);
                    
                        col = map[i+lod][j];
                        drawColor(col);
                        glVertex3f(fi+flod/256,fj,col);
                    
                    
                        col = map[i+lod][j];
                        drawColor(col);
                        glVertex3f(fi+flod/256,fj,lowest);
                
                    glEnd();
                }
            }
    rot1+=lrot;
	glFlush ();													// Flush The GL Rendering Pipeline
}
    
