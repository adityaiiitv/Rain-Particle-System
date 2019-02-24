// Author: Aditya Prakash
// Physics data from these links
// https://hypertextbook.com/facts/1999/MichaelKodransky.shtml
// http://wxguys.ssec.wisc.edu/2013/09/10/how-fast-do-raindrops-fall/
// https://www.engineersedge.com/physics/viscosity_of_air_dynamic_and_kinematic_14483.htm
// 
#include <iostream>
#include <GL/glut.h>
#include <GL/gl.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
using namespace std;

#define	Ground_Side_Length 30
#define Number_Of_Particles 2000
#define Rain 1
#define	Hail 2
#define Gravity -10
#define Mass 0.004
#define Radius 0.002
#define Frame_Rate 30
#define Viscosity 0.11

class Particle
{
	public:
	bool isAlive;		// Is alive or not
	float life;			// Lifespan
	float decayRate;	// Decay rate
	float xCoordinate;	// Coordinates
	float yCoordinate;
	float zCoordinate;
	float yVelocity;	// Vertical Velocity
};

// Particle System Declare
Particle Particle_System[Number_Of_Particles];

// Ground variables
float r = 0.5; float g = 0.35; float b = 0.05;
float ground_Mat[Ground_Side_Length][Ground_Side_Length][3];	// x, y, z
float ground_RGBA[Ground_Side_Length][Ground_Side_Length][4];	// R, G, B and accumulation of water
float ground_Depth = -10.0;										// y

// Camera Attributes
float speedFactor = 1100;
float initial_Velocity = 0.0;
float distanceToGround = -60.0;
float pan_sideways = 4.0;
float pan_up_down = 0.0;

// Particle Attributes
float terminal_velocity;
float particleSize = 0.1;
int particle_Type = 1;
bool isTerminalVelocity = false;

void groundReset()
{
	for (int i = 0; i < Ground_Side_Length; i++)
	{
		for (int j = 0; j < Ground_Side_Length; j++)
		{
			ground_Mat[i][j][0] = i - 10.0;
			ground_Mat[i][j][1] = ground_Depth;
			ground_Mat[i][j][2] = j - 10.0;
			ground_RGBA[i][j][0] = r; ground_RGBA[i][j][1] = g; ground_RGBA[i][j][2] = b;
			ground_RGBA[i][j][3] = 0.0;
		}
	}
}

// Initialize Particles
void initializeParticles(int i)
{
	Particle_System[i].isAlive = true;
	Particle_System[i].life = 1.0;
	Particle_System[i].decayRate = (rand() % 100) / 1000.0f + 0.001f;

	Particle_System[i].xCoordinate = (rand() % Ground_Side_Length) - 10;
	Particle_System[i].yCoordinate = 10.0;
	Particle_System[i].zCoordinate = (rand() % Ground_Side_Length) - 10;
	Particle_System[i].yVelocity = initial_Velocity;
}

void initialize()
{
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);

	// Set ground
	groundReset();

	// Initialize particles
	for (int i = 0; i < Number_Of_Particles; i++)
	{
		initializeParticles(i);
	}

	// Physics
	float force_gravity = Mass * Gravity;
	terminal_velocity = force_gravity / (6 * 3.14 * Viscosity * Radius);
}

void keyPressHandler(unsigned char key, int x, int y) 
{
	if (key == '1') 
	{	
		// Rain
  		particle_Type = Rain;
  		glutPostRedisplay();
	}
	if (key == '2') 
	{	
		// Hail
		groundReset();
  		particle_Type = Hail;
  		glutPostRedisplay();
	}
	if (key == 't' || key == 'T')
	{
		// Hail
		isTerminalVelocity = !isTerminalVelocity;
	}
	if (key == '=') 
	{	
		// Increase Particle Size
		if (particleSize < 1)
		{
			particleSize += 0.01;
		}
	}
	if (key == '-') 
	{	
		// Decrease Particle Size
		if (particleSize > 0.1)
		{
			particleSize -= 0.01;
		}
	}
	if (key == '.')
	{
		// Fast speed
		if (speedFactor > 60)
		{
			speedFactor -= 50;
		}
		//cout << speedFactor << "\n";
	}
	if (key == ',') 
	{	
		// Slow speed
		if (speedFactor < 2100)
		{
			speedFactor += 50;
		}
		//cout << speedFactor << "\n";
	}
	if (key == 'w' || key == 'W')
	{
		distanceToGround += 1.0;
	}
	if (key == 's' || key == 'S')
	{
		distanceToGround -= 1.0;
	}
	if (key == 'a' || key == 'A')
	{
		pan_sideways -= 1.0;
	}
	if (key == 'd' || key == 'D')
	{
		pan_sideways += 1.0;
	}
	if (key == 'q' || key == 'Q')
	{
		pan_up_down += 1.0;
	}
	if (key == 'e' || key == 'E')
	{
		pan_up_down -= 1.0;
	}
}

// For Rain
void drawRain() {
	float x, y, z;
	for (int i = 0; i < Number_Of_Particles; i++) 
	{
		if (Particle_System[i].isAlive) 
		{
			x = Particle_System[i].xCoordinate;
			y = Particle_System[i].yCoordinate;
			z = Particle_System[i].zCoordinate + distanceToGround;

			// Draw particles
			glColor3f(0.5, 0.5, 1.0);
			glBegin(GL_LINES);
			glVertex3f(x, y, z);
			glVertex3f(x, y + particleSize, z);
			glEnd();

			// Adjust speed
			if (!isTerminalVelocity)
			{
				Particle_System[i].yCoordinate += Particle_System[i].yVelocity / speedFactor;
				Particle_System[i].yVelocity += Gravity / 10;
			}
			else
			{
				Particle_System[i].yCoordinate += Particle_System[i].yVelocity;
				Particle_System[i].yVelocity = terminal_velocity / Frame_Rate;
			}

			// Decay
			Particle_System[i].life -= Particle_System[i].decayRate;

			if (Particle_System[i].yCoordinate <= -10)
			{
				int zi = z - distanceToGround + 10;
				int xi = x + 10;
				ground_RGBA[zi][xi][0] = 0.5; ground_RGBA[zi][xi][1] = 0.5; ground_RGBA[zi][xi][2] = 1.0;
				ground_RGBA[zi][xi][3] = 1.1;
				if (ground_RGBA[zi][xi][3] > 1.0)
				{
					ground_Mat[xi][zi][1] += 0.01;
				}
				Particle_System[i].life = -1.0;
			}

			// Revive the particle
			if (Particle_System[i].life < 0.0) 
			{
				initializeParticles(i);
			}
		}
	}
}

// For Hail
void drawHail() 
{
	float x, y, z;
	for (int i = 0; i < Number_Of_Particles; i++) 
	{
		if (Particle_System[i].isAlive) 
		{
			x = Particle_System[i].xCoordinate;
			y = Particle_System[i].yCoordinate;
			z = Particle_System[i].zCoordinate + distanceToGround;

			// Draw particles
			glColor3f(1.0, 1.0, 1.0);
			glPushMatrix();
			glTranslatef(x, y, z);
			float adjustedSize = particleSize / 5;
			glutSolidSphere(adjustedSize, 20, 20);
			glPopMatrix();

			// Update values
			if (Particle_System[i].yCoordinate <= -9.8) 
			{
				Particle_System[i].yVelocity = Particle_System[i].yVelocity * -0.5;
			}
			// Adjust speed
			if (!isTerminalVelocity)
			{
				Particle_System[i].yCoordinate += Particle_System[i].yVelocity / speedFactor;
				Particle_System[i].yVelocity += Gravity / 10;
			}
			else
			{
				Particle_System[i].yCoordinate += Particle_System[i].yVelocity;
				Particle_System[i].yVelocity = terminal_velocity / Frame_Rate;
			}

			// Decay
			Particle_System[i].life -= Particle_System[i].decayRate;

			// Revive the particle
			if (Particle_System[i].life < 0.0 || Particle_System[i].yCoordinate <= -11 || Particle_System[i].yCoordinate >= 11)
			{
				initializeParticles(i);
			}
		}
	}
}

// Draw Particles
void drawParticleSystem( ) 
{
	float x, y, z;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotatef(pan_sideways, 0.0, 1.0, 0.0);
	glRotatef(pan_up_down, 1.0, 0.0, 0.0);

	// Ground
	glColor3f(r, g, b);
	glBegin(GL_QUADS);
	for (int i = 0; i <29; i++) 
	{
		for (int j = 0; j <29; j++) 
		{
			glColor3fv(ground_RGBA[i][j]);
			glVertex3f(ground_Mat[j][i][0],
			ground_Mat[j][i][1],
			ground_Mat[j][i][2] + distanceToGround);
			glColor3fv(ground_RGBA[i][j+1]);
			glVertex3f(ground_Mat[j+1][i][0],
			ground_Mat[j+1][i][1],
			ground_Mat[j+1][i][2] + distanceToGround);
			glColor3fv(ground_RGBA[i+1][j+1]);
			glVertex3f(ground_Mat[j+1][i+1][0],
			ground_Mat[j+1][i+1][1],
			ground_Mat[j+1][i+1][2] + distanceToGround);
			glColor3fv(ground_RGBA[i+1][j]);
			glVertex3f(ground_Mat[j][i+1][0],
			ground_Mat[j][i+1][1],
			ground_Mat[j][i+1][2] + distanceToGround);
		}
	}
	glEnd();
	// Draw particles
	if (particle_Type == 1) 
	{
		drawRain();
	}
	else
	{
		drawHail();
	}
	glutSwapBuffers();
}

void reshape(int w, int h) 
{
	if (h == 0)
	{
		h = 1;
	}
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, w / h, .1, 200);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void Redisplay() 
{
	glutPostRedisplay();
}

int main (int argc, char** argv) 
{
	int Window_Width = 1024, Window_Height = 768;	// Display Window Size
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(Window_Width, Window_Height);
	glutCreateWindow("");
	initialize();
	glutDisplayFunc(drawParticleSystem);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyPressHandler);
	glutIdleFunc(Redisplay);
	glutMainLoop();
	return 0;
}
