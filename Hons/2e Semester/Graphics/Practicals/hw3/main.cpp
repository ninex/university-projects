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
#define ZOOM_INC 4.0f/256
#define ROT_INC 2.f
#define PI 3.14
GL_Window*	g_window;
Keys*		g_keys;
struct Point3f
{
GLfloat x, y, z;
};
// User Defined Variables
float		angle,zoom,lowest,lrot, rot1,camx,camy,pox,poy;												// Used To Rotate The Triangles
int			lod,py,px;
float map[257][257];
Point3f norm[256][256];
Point3f norm2[256][256];
bool top;

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
    
     map[0][0] = 0;//frand();
     map[0][256]= 0;//frand();
     map[256][0]= 0;//frand();
     map[256][256] = 0;//frand();
      
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
	angle		= 0.1;											// Set Starting Angle To Zero
	zoom        = -2.0f;
    rot1 = 0;
    lod = 1;
    lrot = 2.f;
    top = false;
    camx = 0;
    camy = 0;
    pox = 0;
    poy = 0;
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
    
    if ((g_keys->keyDown [119] == TRUE) || (g_keys->keyDown [87] == TRUE))
	{
		camy -= 0.01f/(milliseconds);
	}
	//S
	if ((g_keys->keyDown [115] == TRUE) || (g_keys->keyDown [83] == TRUE)) 
	{
		camy += 0.01f/(milliseconds);
	}
	
	if ((g_keys->keyDown [97] == TRUE) || (g_keys->keyDown [65] == TRUE))
	{
		camx += 0.01f/(milliseconds);
	}
	if ((g_keys->keyDown [100] == TRUE) || (g_keys->keyDown [68] == TRUE))
	{
		camx -= 0.01f/(milliseconds);
	}
	
    if (g_keys->keyDown [VK_HOME] == TRUE)						// Is F1 Being Pressed?
	{
		zoom -= ZOOM_INC;						// Update angle Based On The Clock
	}
	if (g_keys->keyDown [VK_END] == TRUE)						// Is F1 Being Pressed?
	{
		zoom += ZOOM_INC;						// Update angle Based On The Clock
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
            lod /= 2;						
        }
	}
	if ((g_keys->keyDown [84] == TRUE) || (g_keys->keyDown [116] == TRUE))
	{
		if (top){
            top = false;						
        }else{
              top = true;
        }
	}
	
	if (g_keys->keyDown [VK_LEFT] == TRUE) {
       angle -= ROT_INC;// / (milliseconds);
    }
    if (g_keys->keyDown [VK_RIGHT] == TRUE) {
       angle += ROT_INC;// / (milliseconds);
    }
	
	camx < -0.5 ? camx = -0.5:0;
    camx > 0.5 ? camx = 0.5:0;
    
	camy < -0.5 ? camy = -0.5:0;
    camy > 0.5 ? camy = 0.5:0;
    
    angle >= 360 ? angle -= 360:0;
    angle < 0 ? angle += 360:0;
    
    
}

float dp(float ax,float ay,float bx,float by){
      return ax*bx + ay*by;
}

void drawColor(float col){
    if (col < 0.15){
        glColor3f(0,0,col*6);
    }else{
        if (col < 0.2){
            glColor3f(col*10/3,col*5/3,0);
        }else{
                glColor3f(0,col*4/3,0);
        }
    }
}

void Draw (void)
{
	float col,fi,fj,flod,camz;

    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear Screen And Depth Buffer
	glLoadIdentity ();											// Reset The Modelview Matrix

		
    if (top){glTranslatef (0, 0, zoom);  
        glColor3f(1,0,0);
        glBegin(GL_LINES);
            glVertex3f( camx, camy, 1);
            glVertex3f( camx+0.1, tan(PI/4)*(camx+0.1), 1);
        glEnd();
        glBegin(GL_LINES);
            glVertex3f( camx, camy, 1);
            glVertex3f( camx-0.1, tan(3.0*PI/4.0)*(camx-0.1), 1);
        glEnd();
         
    }else{
          glRotatef (-90, 1.f,0.f,0.0f);
	      
          camz = map[(int)((camx+0.5)*256)][(int)((camy+0.5)*256)]+0.1;
	      glTranslatef (camx,camy, -camz);
    }
   
	glRotatef(angle,0.f,0.f,1.0f);
    float ax,ay,bx,by,tang,x1,x2;

    tang = 45 - angle;


    if (tang >= 360){
              tang = tang - 360;
    }
    if (tang < 0){
              tang = tang + 360;
    }
    tang = tang*PI/180;

    x1 = camx + 1;
    x2 = camx - 1;
    
    if ((tang > PI/2) && (tang < 3*PI/2)){
              x1 = -x1;
    }
    
    if ((tang > 0) && (tang < PI)){
              x2 = -x2;
    }

    //left
    ax = x2;ay = tan(tang + PI/2)*(x2);
    //right
    bx = x1;by = tan(tang)*(x1);
    
    float nx1,ny1,nx2,ny2;
    nx1 = by;
    ny1 = -bx;
    nx2 = ay;
    ny2 = -ax;

	int i,j;
	glBegin (GL_TRIANGLES);
    for (i=0;i<256;i+=lod){
        for (j=0;j<256;j+=lod){
	        
	        fi = ((float)i)/256-0.5;
	        fj = ((float)j)/256-0.5;
	        flod = (float)lod;
	        if ( (dp(fi,fj,nx1,ny1) < 0) && (dp(fi,fj,nx2,ny2) < 0)){

               
                    col = map[i][j];
                    drawColor(col);	glVertex3f( fi, fj, col);
                    
                    col = map[i+lod][j];
                    drawColor(col);	glVertex3f( fi+flod/256,fj, col);
            		
            		col = map[i+lod][j+lod];
            		drawColor(col);	glVertex3f( fi+flod/256,fj+flod/256,col);
            		
        
                    col = map[i][j];
                    drawColor(col);	glVertex3f( fi, fj, col);
            		
            		col = map[i+lod][j+lod];
            		drawColor(col);	glVertex3f( fi+flod/256,fj+flod/256,col);
            		
                    col = map[i][j+lod];
            		drawColor(col);	glVertex3f(fi,fj+flod/256, col);
              }	
        }
    }
    glEnd ();
	glFlush ();													// Flush The GL Rendering Pipeline
}
    
