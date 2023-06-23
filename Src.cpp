
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <cmath>

#define WINDOW_WIDTH 1600
#define WINDOW_HEIGHT 800
#define PI 3.14159265358979323846
#define DEG2RAD(deg) (deg * PI / 180)

// Adjusted values for the solar system simulation
GLfloat sunRadius = 10;
static float time = 0;
static float speed = 20;
static unsigned int spacecraft; // Display lists base index.

GLfloat planetSpeed[] = {
	1.0 / 0.24, // Mercury
	1.0 / 0.62, // Venus
	1.0,		// Earth
	.9,			// Mars
	.8,			// Jupiter
	.7,			// Saturn
	.6,			// Uranus
	.5			// Neptune
};

//  values for the planet colors
// Adjusted values for the colors of the planets
GLfloat planetsColors[8][3] = {
	// Mercury - gray
	{0.6f, 0.6f, 0.6f},
	// Venus - yellow
	{1.0f, 0.9f, 0.6f},
	// Earth - blue and green
	{0.0f, 0.5f, 1.0f},
	// Mars - reddish orange
	{1.0f, 0.4f, 0.1f},
	// Jupiter - orange and white
	{1.0f, 0.8f, 0.5f},
	// Saturn - yellow and brown
	{1.0f, 0.9f, 0.6f},
	// Uranus - blue
	{0.6f, 0.8f, 1.0f},
	// Neptune - blue and green
	{0.4f, 0.5f, 1.0f},
};

float spacecraftX = 0;	   // spacecraft position in the x-axis
float spacecraftY = 0;	   // spacecraft position in the y-axis
float spacecraftZ = -10;   // spacecraft position in the z-axis
float spacecraftYaw = 0;   // spacecraft orientation in the y-axis
float spacecraftPitch = 0; // spacecraft orientation in the x-axis
float spacecraftRoll = 0;  // spacecraft orientation in the z-axis
float spacecraftSpeed = 1; // spacecraft speed

struct Planet
{
	float x, y, z;		  // position of the planet
	float radius;		  // radius of the planet
	GLfloat color[3];	  // color for the planet
	GLfloat specular[4];  // specular color for the planet
	GLfloat shininess[1]; // shininess value for the planet
};
// adjust the values for the planets
Planet mercury = {
	0.0, 0.0, -20, 0.8, {planetsColors[0][0], planetsColors[0][1], planetsColors[0][2]}, {1.0, 1.0, 1.0, 1.0}, {20.0}};

Planet venus = {
	0.0, 0.0, -32.0, 0.8, {planetsColors[1][0], planetsColors[1][1], planetsColors[1][2]}, {1.0, 1.0, 1.0, 1.0}, {35.0}};

Planet earth = {
	0.0, 0.0, -48.0, 1.0, {planetsColors[2][0], planetsColors[2][1], planetsColors[2][2]}, {0.8, 0.8, 1.0, 1.0}, {40.0}};

Planet mars = {
	0.0, 0.0, -64.0, 0.8, {planetsColors[3][0], planetsColors[3][1], planetsColors[3][2]}, {1.0, 0.8, 0.8, 1.0}, {30.0}};

Planet jupiter = {
	0.0, 0.0, -80.0, 3.5, {planetsColors[4][0], planetsColors[4][1], planetsColors[4][2]}, {1.0, 1.0, 1.0, 1.0}, {60.0}};

Planet saturn = {
	0.0, 0.0, -112.0, 3.2, {planetsColors[5][0], planetsColors[5][1], planetsColors[5][2]}, {1.0, 1.0, 1.0, 1.0}, {50.0}};

Planet uranus = {
	0.0, 0.0, -128.0, 2.0, {planetsColors[6][0], planetsColors[6][1], planetsColors[6][2]}, {1.0, 1.0, 1.0, 1.0}, {45.0}};

Planet neptune = {
	0.0, 0.0, -144.0, 2.0, {planetsColors[7][0], planetsColors[7][1], planetsColors[7][2]}, {0.8, 0.8, 1.0, 1.0}, {40.0}};

Planet planets[] = {mercury, venus, earth, mars, jupiter, saturn, uranus, neptune};
void drawSphere(Planet planet, int i)
{

	glPushMatrix();
	glRotatef(time * planetSpeed[i], 0, 1, 0); // rotate the planet around the sun
	glTranslatef(planet.x, 0, planet.z);
	glRotatef(time * planetSpeed[i] * 10, 0, 1, 0); // rotate the planet around its axis
	glMaterialfv(GL_FRONT, GL_DIFFUSE, planet.color);
	glMaterialfv(GL_FRONT, GL_SPECULAR, planet.specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, planet.shininess);
	GLfloat sun_emission[] = {0.0, 0.0, 0.0, 0.0};
	glMaterialfv(GL_FRONT, GL_EMISSION, sun_emission);
	glutSolidSphere(planet.radius, 20, 20);

	glPopMatrix();
}
void draw_rings_for_plants(int i)
{
	glPushMatrix();
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f); // Align the orbits with the XY plane
	// Draw the orbit as a wireframe circle
	GLfloat sun_emission[] = {0.0, 0.0, 0.0, 1.0};
	glMaterialfv(GL_FRONT, GL_EMISSION, sun_emission);
	GLfloat default_ambient[] = {0.2f, 0.2f, 0.2f, 1.0f};
	glMaterialfv(GL_FRONT, GL_AMBIENT, default_ambient);
	GLfloat default_diffuse[] = {0.8f, 0.8f, 0.8f, 1.0f};
	glMaterialfv(GL_FRONT, GL_DIFFUSE, default_diffuse);
	GLfloat default_specular[] = {0.0f, 0.0f, 0.0f, 1.0f};
	glMaterialfv(GL_FRONT, GL_SPECULAR, default_specular);

	glBegin(GL_LINE_LOOP);
	for (int j = 0; j < 360; j++)
	{
		float angle = DEG2RAD(j);
		float x = planets[i].z * cos(angle);
		float y = planets[i].z * sin(angle);
		glColor3f(0.4f, 0.4f, 0.4f);
		glVertex3f(x, y, 0.0f);
	}
	glEnd();
	glPopMatrix();
}

void drawSolarSystem(float time, bool v2)
{
	GLfloat light_position[] = {0, 0, 2, 1.0};
	GLfloat light_ambient[] = {0.5, 0.5, 0.5, 1.0};
	GLfloat light_diffuse[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat light_specular[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat light_shininess[] = {100.0};

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_FRONT, GL_SHININESS, light_shininess);

	// draw the sun
	glPushMatrix();
	glRotatef(0, 0, 1, 0); // rotate the sun
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glTranslatef(light_position[0], light_position[1], light_position[2]);
	GLfloat sun_emission[] = {1.0f, 1.0f, .3, 1.0};
	glMaterialfv(GL_FRONT, GL_EMISSION, sun_emission);
	glutSolidSphere(sunRadius, 20, 20);

	glPopMatrix();

	for (int i = 0; i < 8; i++)
	{
		drawSphere(planets[i], i);

		if (v2)
		{
			draw_rings_for_plants(i);
		}
	}

	//// draw the moon around earth
	glPushMatrix();
	glRotatef(time * planetSpeed[2], 0, 1, 0); // rotate the moon around the earth
	glTranslatef(earth.x, 0, earth.z);
	glRotatef(time * planetSpeed[2] * 10, 0, 1, 0); // rotate the moon around its axis
	glTranslatef(1.5, 0.0, 0.0);
	glColor3f(0.6f, 0.6f, 0.6f);
	glutSolidSphere(0.25, 20, 20);
	glPopMatrix();
	// Draw ring for saturn
	glPushMatrix();
	glRotatef(time * planetSpeed[5], 0, 1, 0);
	glTranslatef(saturn.x, 0, saturn.z);
	glRotatef(time * planetSpeed[5] * 10, 0, 1, 0); // rotate the planet around its axis
	glMaterialfv(GL_FRONT, GL_DIFFUSE, planets[5].color);
	glutSolidTorus(0.5, 5.0, 20, 20);
	glPopMatrix();
}

static float prev_time = 0;

void spinDisplay()
{
	time += speed * (glutGet(GLUT_ELAPSED_TIME) - prev_time) / 1000;
	prev_time = glutGet(GLUT_ELAPSED_TIME); // to indicate refresh rate
	glutPostRedisplay();
}
void drawSpaceCraft()
{
	// Draw the cone representing the camera position
	glPushMatrix();
	glRotatef(180.0, 0.0, 1.0, 0.0); // To make the spacecraft point down the $z$-axis initially.
	glColor3f(1.0, 1.0, 1.0);
	glutWireCone(5.0, 10.0, 10, 10);
	glPopMatrix();
}
void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(spacecraftX - 10 * sin(DEG2RAD(spacecraftYaw)), 0, spacecraftZ - 10 * cos(DEG2RAD(spacecraftYaw)),
			  spacecraftX - 11 * sin(DEG2RAD(spacecraftYaw)), 0, spacecraftZ - 11 * cos(DEG2RAD(spacecraftYaw)), 0, 1, 0); // camera position

	drawSolarSystem(time, false);

	glViewport(WINDOW_WIDTH / 2 + 100, 50, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 + 50);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	// camera position

	gluLookAt(0, 50, 150, 0, 0, 0, 0, 0, -1);

	drawSolarSystem(time, true);
	// Draw spacecraft.
	glPushMatrix();
	glTranslatef(spacecraftX, 0.0, spacecraftZ);
	glRotatef(spacecraftYaw, 0.0, 1.0, 0.0);
	glCallList(spacecraft);
	glPopMatrix();
	glutSwapBuffers();
}

void reshape(int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(100, (float)width / height, 1, 1000); // FOV, aspect, near, far
}

void init()
{
	spacecraft = glGenLists(1);
	glNewList(spacecraft, GL_COMPILE);
	drawSpaceCraft();
	glEndList();
	glEnable(GL_DEPTH_TEST);
	glClearColor(0, 0, 0, 1);

	// enable lighting
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	GLfloat material_ambient[] = {0.2, 0.2, 0.2, 1.0};
	GLfloat material_diffuse[] = {1, 1, 0, 1.0};
	GLfloat material_specular[] = {0.5, 0.5, 0.5, 1.0};
	GLfloat material_shininess[] = {100.0};
	glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, material_shininess);

	glEnable(GL_CULL_FACE); // This line enables face culling, where back-facing polygons are not rendered. This optimization technique can improve rendering performance.
	glCullFace(GL_BACK);
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w': // move forward
		spacecraftZ -= spacecraftSpeed * cos(DEG2RAD(spacecraftYaw));
		spacecraftX -= spacecraftSpeed * sin(DEG2RAD(spacecraftYaw));

		glutPostRedisplay();
		break;
	case 's': // move backward
		spacecraftZ += spacecraftSpeed * cos(DEG2RAD(spacecraftYaw));
		spacecraftX += spacecraftSpeed * sin(DEG2RAD(spacecraftYaw));

		glutPostRedisplay();
		break;
	case 'a': // turn left
		spacecraftYaw += 5;
		if (spacecraftYaw > 360.0)
			spacecraftYaw -= 360.0;
		glutPostRedisplay();
		break;
	case 'd': // turn right
		spacecraftYaw -= 5;
		if (spacecraftYaw < 0.0)
			spacecraftYaw += 360.0;
		glutPostRedisplay();
		break;
	}
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("Solar System");
	glutDisplayFunc(display);
	glutIdleFunc(spinDisplay);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);

	glewExperimental = GL_TRUE;
	init();
	glutMainLoop();
	return 0;
}
