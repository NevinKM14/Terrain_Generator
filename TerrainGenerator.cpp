/* Nevin Mahilal's Assignment 3
*  Student Number: 1054694
*  Platform: Ubuntu 14.0
*  Compiler: Used makefile (the one found in the Boilerplate zip file on Avenue)
*  Extra Feature 1: Midpoint Displacement Algorithm (Additional terrain algorithm #1)
*  Extra Feature 2: Smooth Filter Algorithm (Additional terrain algorithm #2)
*/


#include <GL/glut.h>
#include <GL/gl.h>
#include <iostream>
#include <stdio.h> 
#include <stdlib.h>    
#include <time.h>
#include <math.h>

using namespace std;

int n;
int loopLimit = 400;
float heightmap[300][300];
float faceNormals[300][300][3];
float vertexNormals[300][300][3];

float camPos[] = {-50,150,100}; //Starting Camera position
float normX, normY, normZ;
float grayScale = 0.0;
int rCounter = 0; int wCounter = 0; int lCounter = 0; int gCounter = 0;
float maxHeight = 0; float minHeight = 0;

float light_pos[] = {-150.0,150.0,100,1.0};
float light_pos2[] = {150.0,150.0,-100,1.0};

float amb0[4] = {1, 1, 1, 1};
float diff0[4] = {1, 0, 0, 1};
float spec0[4] = {1, 1, 1, 1};

float amb2[4] = {1, 1, 1, 1};
float diff2[4] = {1, 0, 0, 1};
float spec2[4] = {1, 1, 1, 1};

float m_amb[] = {0.22, 0.11, 0.03, 1.0};
float m_diff[] = {0.78, 0.57, 0.11, 1.0};
float m_spec[] = {0.99, 0.91, 0.81, 1.0};
float shiny = 5.0;

void Normalize(float vx, float vy, float vz) {

	double length;
	length = sqrt((vx*vx) + (vy*vy) + (vz*vz));

	normX = vx / length;
	normY = vy / length;
	normZ = vz / length;
}

void computeFaceNorm() 
{

	float v1[3],v2[3],vx,vy,vz;

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{	
			v1[0] = i+1 - i;
			v1[1] = heightmap[i+1][j] - heightmap[i][j];;
			v1[2] = j - j; 

			v2[0] = i - i;
			v2[1] = heightmap[i][j+1] - heightmap[i][j];
			v2[2] = j+1 - j; 

			vx = v1[1] * v2[2] - v1[2] * v2[1];
			vy = v1[2] * v2[0] - v1[0] * v2[2];
			vz = v1[0] * v2[1] - v1[1] * v2[0];

			Normalize(vx,vy,vz);

			faceNormals[i][j][0] = normX;
			faceNormals[i][j][1] = normY;
			faceNormals[i][j][2] = normZ;
		}
	}
}

void computeVertexNorm()
{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			/* normals for the four corners */
			if (i == 0 && j == 0)
			{
			// normalize at end in for loop
				vertexNormals[i][j][0] = faceNormals[i][j][0];
				vertexNormals[i][j][1] = faceNormals[i][j][1];
				vertexNormals[i][j][2] = faceNormals[i][j][2];
			}
			else if (i == n-1 && j == n-1)
			{
				vertexNormals[i][j][0] = faceNormals[n-2][n-2][0];
				vertexNormals[i][j][1] = faceNormals[n-2][n-2][1];
				vertexNormals[i][j][2] = faceNormals[n-2][n-2][2];
			}
			else if(j == 0 && i == n-1)
			{
				vertexNormals[i][j][0] = faceNormals[n-2][j][0];
				vertexNormals[i][j][1] = faceNormals[n-2][j][1];
				vertexNormals[i][j][2] = faceNormals[n-2][j][2];
			}
			else if(j == n-1 && i == 0)
			{
				vertexNormals[i][j][0] = faceNormals[i][n-2][0];
				vertexNormals[i][j][1] = faceNormals[i][n-2][1];
				vertexNormals[i][j][2] = faceNormals[i][n-2][2];
			}

			/* normals for the borders */		
			else if(i == 0)
			{
				vertexNormals[0][j][0] = (faceNormals[0][j][0] + faceNormals[0][j-1][0])/2;
				vertexNormals[0][j][1] = (faceNormals[0][j][1] + faceNormals[0][j-1][1])/2;
				vertexNormals[0][j][2] = (faceNormals[0][j][2] + faceNormals[0][j-1][2])/2;
			}
			else if(j == 0)
			{
				vertexNormals[i][0][0] = (faceNormals[i][0][0] + faceNormals[i-1][0][0])/2;
				vertexNormals[i][0][1] = (faceNormals[i][0][1] + faceNormals[i-1][0][1])/2;
				vertexNormals[i][0][2] = (faceNormals[i][0][2] + faceNormals[i-1][0][2])/2;
			}
			else if(i == n-1)
			{
				vertexNormals[i][j][0] = (faceNormals[n-2][j][0] + faceNormals[n-2][j-1][0])/2;
				vertexNormals[i][j][1] = (faceNormals[n-2][j][1] + faceNormals[n-2][j-1][1])/2;
				vertexNormals[i][j][2] = (faceNormals[n-2][j][2] + faceNormals[n-2][j-1][2])/2;
			}
			else if(j == n-1)
			{
				vertexNormals[i][j][0] = (faceNormals[i][n-2][0] + faceNormals[i-1][n-2][0])/2;
				vertexNormals[i][j][1] = (faceNormals[i][n-2][1] + faceNormals[i-1][n-2][1])/2;
				vertexNormals[i][j][2] = (faceNormals[i][n-2][2] + faceNormals[i-1][n-2][2])/2;
			}
			/* normals for the inner vertices*/
			else
			{
				vertexNormals[i][j][0] = (faceNormals[i][j][0] + faceNormals[i][j-1][0] + faceNormals[i-1][j-1][0] + 						faceNormals[i-1][j][0])/4;
				vertexNormals[i][j][1] = (faceNormals[i][j][1] + faceNormals[i][j-1][1] + faceNormals[i-1][j-1][1] +
					faceNormals[i-1][j][1])/4;
				vertexNormals[i][j][2] = (faceNormals[i][j][2] + faceNormals[i][j-1][2] + faceNormals[i-1][j-1][2] +
					faceNormals[i-1][j][2])/4;
			}
			
		}
	}

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
				Normalize(vertexNormals[i][j][0],vertexNormals[i][j][1],vertexNormals[i][j][2]);
				vertexNormals[i][j][0] = normX; 
				vertexNormals[i][j][1] = normY;
				vertexNormals[i][j][2] = normZ;
		}
	}
}


void special(int key, int x, int y)
{
	/* arrow key presses move the camera */
	switch(key)
	{
		case GLUT_KEY_LEFT:
			camPos[0]-=1;
			break;

		case GLUT_KEY_RIGHT:
			camPos[0]+=1;
			break;

		case GLUT_KEY_UP:
			camPos[2] -= 1;
			break;

		case GLUT_KEY_DOWN:
			camPos[2] += 1;
			break;
		
		case GLUT_KEY_HOME:
			camPos[1] += 1;
			break;

		case GLUT_KEY_END:
			camPos[1] -= 1;
			break;

	}
	glutPostRedisplay();
}

void init(void)
{
	
	glClearColor(0, 0, 0, 0);
	glColor3f(1, 1, 1);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, 1, 1, 1000);
	srand (time(NULL));
	
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, amb0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diff0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, spec0);

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, m_amb);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_diff);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m_spec);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shiny);

	glEnable(GL_LIGHT1);
	glLightfv(GL_LIGHT1, GL_POSITION, light_pos2);
	glLightfv(GL_LIGHT1, GL_AMBIENT, amb2);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diff2);
	glLightfv(GL_LIGHT1, GL_SPECULAR, spec2);

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, m_amb);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_diff);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m_spec);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shiny);
	
}

void maxMinValues()
{

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			if (heightmap[i][j] > maxHeight)
			{
				maxHeight = heightmap[i][j];
			}

			if (heightmap[i][j] < minHeight)
			{
				minHeight = heightmap[i][j];
			}
		}
	}	
}

void drawQuads()
{
	maxMinValues();
	float grayScale;
	if (wCounter == 0 || wCounter == 2)
	{
		for (int i = 0; i < n-1; i++)
		{
			for (int j = 0; j < n; j++)
			{	
				 
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);	
				glBegin(GL_QUADS);	
			
					grayScale = (heightmap[i][j] - minHeight)/(maxHeight-minHeight);
					glColor3f(grayScale, grayScale, grayScale);
					glNormal3f(vertexNormals[i][j][0], vertexNormals[i][j][1], vertexNormals[i][j][2]); 		
					glVertex3i(i, heightmap[i][j], j);

					grayScale = (heightmap[i][j+1] - minHeight)/(maxHeight-minHeight);
					glColor3f(grayScale, grayScale, grayScale); 
					glNormal3f(vertexNormals[i][j+1][0], vertexNormals[i][j+1][1], vertexNormals[i][j+1][2]); 
					glVertex3i(i, heightmap[i][j+1], j+1);
					
					grayScale = (heightmap[i+1][j+1] - minHeight)/(maxHeight-minHeight);				
					glColor3f(grayScale, grayScale, grayScale);
					glNormal3f(vertexNormals[i+1][j+1][0], vertexNormals[i+1][j+1][1], vertexNormals[i+1][j+1][2]);
					glVertex3i(i+1, heightmap[i+1][j+1], j+1);

					grayScale = (heightmap[i+1][j] - minHeight)/(maxHeight-minHeight);
					glColor3f(grayScale, grayScale, grayScale); 
					glNormal3f(vertexNormals[i+1][j][0], vertexNormals[i+1][j][1], vertexNormals[i+1][j][2]);
					glVertex3i(i+1, heightmap[i+1][j], j);

				glEnd();
			}
		}
	}

	if (wCounter == 1 || wCounter == 2)
	{
		for (int i = 0; i < n-1; i++)
		{
			for (int j = 0; j < n; j++)
			{	
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		 		glBegin(GL_QUADS);		
					glColor3f(1, 1, 1);
					glVertex3i(i, heightmap[i][j], j);
					glColor3f(1, 1, 1); 
					glVertex3i(i, heightmap[i][j+1], j+1);
					glColor3f(1, 1, 1);
					glVertex3i(i+1, heightmap[i+1][j+1], j+1);
					glColor3f(1, 1, 1); 
					glVertex3i(i+1, heightmap[i+1][j], j);
				glEnd();
			}
		}
	}

}	


/*
	Fault Algorithm:
	for each point (tx,tz) in the terrain do {
		if (a*tx + b*tz - c > 0) 
			height(tx,tz) += displacement;
		else
			height(tx,tz) -= displacement;
	}
*/
void faultAlgorithm()
{
	float displacement = 1.0*(rand())/RAND_MAX;
	float v = rand();
	float a = sin(v);
	float b = cos(v);
	float d = sqrt(n*n + n*n);
	// rand() / RAND_MAX gives a random number between 0 and 1.
	// therefore c will be a random number between -d/2 and d/2
	float c = ((float)rand() / RAND_MAX) * d - d/2;

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			int x = i - n/2;
			int z = j - n/2;
			if(a*x + b*z -c > 0)
			{
				 heightmap[i][j] += displacement;	
			}
			else
			{	
				 heightmap[i][j] -= displacement;
			}
		}
	}
	

}

void midPointDispAlgorithm(){
	for (int i=0; i<n; i++){
		for (int j=0; j<n; j++){
			heightmap[i+1][j+1] = (heightmap[i][j] + heightmap[i+1][j] + heightmap[i+2][j] + heightmap[i][j+1] + heightmap[i+2][j+1] + heightmap[i][j+2] + heightmap[i+1][j+2] + heightmap[i+2][j+2])/8;
		}
	}
    
}
/*Smooth Filter Algorithm*/
void smoothFilter()
{
	float k = 0.75;

	// Rows, left to right 
	for(int i = 1; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{		
			heightmap[i][j] = heightmap[i-1][j] * (1-k) + heightmap[i][j] * k;
		}
	}

	// Rows, right to left
	for(int i = n-2; i < -1; i--)
	{
		for (int j = 0; j < n; j++)
		{		
			heightmap[i][j] = heightmap[i+1][j] * (1-k) + heightmap[i][j] * k;
		}
	}

	// Columns, bottom to top
	for(int i = 0; i < n; i++)
	{
		for (int j = 1; j < n; j++)
		{		
			heightmap[i][j] = heightmap[i][j-1] * (1-k) + heightmap[i][j] * k;
		}
	}

	// Columns, top to bottom
	for(int i = 0; i < n; i++)
	{
		for (int j = n; j < -1; j--)
		{		
			heightmap[i][j] = heightmap[i][j+1] * (1-k) + heightmap[i][j] * k;
		}
	}

}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
		
	gluLookAt(camPos[0], camPos[1], camPos[2], n/2,0,n/2, 0,1,0);

	if (rCounter == 0)
	{
		for (int i = 0; i < loopLimit; i++)
		{
			faultAlgorithm();
		}

		midPointDispAlgorithm();

		smoothFilter();
		smoothFilter();
		smoothFilter();

		computeFaceNorm();
		computeVertexNorm();
		rCounter++;
	}


	drawQuads();
	glutSwapBuffers();	
}

void keyboard(unsigned char key, int x, int y)
{

	/* key presses move the cube, if it isn't at the extents (hard-coded here) */
	switch (key)
	{
		case 'q':
		case 'Q':
		case 27:
			exit (0);
			break;
		case 'w':
		case 'W':			
			wCounter++;
			if (wCounter == 3)
			{
				wCounter = 0;
			}			
			break;
		case 'r':
		case 'R':
			rCounter = 0;
			for (int i = 0; i < n; i++)
			{
				for (int j = 0; j < n; j++)
				{
					heightmap[i][j] = 0;
				}
			}
			break;
		case 'l':
		case 'L':
			lCounter++;
			if (lCounter % 2 == 0)
			{				
				glDisable(GL_LIGHTING);
			}			
			else
			{
				glEnable(GL_LIGHTING);
			}
			break;
		case 'g':
		case 'G':
			gCounter++;
			if (gCounter % 2 == 0)
			{
				glShadeModel(GL_FLAT);
			}
			else
			{
				glShadeModel(GL_SMOOTH);
			}
	}
	glutPostRedisplay();
}

/* main function - program entry point */
int main(int argc, char** argv)
{

	cout << "Enter Width of Grid Between 50 to 300 (length will be the same value): " << endl;
	cin >> n;

	cout << "----------- MENU COMMANDS -----------" << endl;
	cout << "R KEY------------GENERATE NEW TERRAIN" << endl;
	cout << "L KEY ----------------TOGGLE LIGHTING" << endl;
	cout << "W KEY ---------------TOGGLE WIRE MODE" << endl;
	cout << "G KEY --------------TOGGLE SHADE MODE" << endl;
	cout << "ARROW KEYS ------------ROTATE TERRAIN" << endl;
	cout << "HOME KEY -----------ZOOM INTO TERRAIN" << endl;
	cout << "END KEY ----------ZOOM OUT OF TERRAIN" << endl;
	cout << "Q KEY/ESC------------------------EXIT" << endl;

	glutInit(&argc, argv);		//starts up GLUT
	
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Nevin Mahilal's Terrain Generator");	//creates the window
	
	glutDisplayFunc(display);	//registers "display" as the display callback function
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);
	
   	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);​

	init();

	glutMainLoop();				//starts the event loop

	return(0);					//return may not be necessary on all compilers
}
