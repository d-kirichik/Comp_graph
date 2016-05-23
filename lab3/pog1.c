#include <stdio.h>
#include <GLFW/glfw3.h>
#include <GL/glu.h>
#include <math.h>
#include <stdlib.h>
#define PI 3.14159265358979323846

void keyboard_callback(GLFWwindow *window, int key, int scancode,
  int action, int mods);

int rotate_y = 0;
int rotate_x = 0;
int only_lines = 0;
float scale = 1.0;
float MIN_SCALE = 0.1;
float MAX_SCALE = 5;
int delta=50;
float R=3;
struct Vertex {
	double x,y,z;
};


struct Vertex myFunction(float t, float alpha) {
	struct Vertex v;
	v.x=R*cos(t)*sin(alpha);
	v.y=R*cos(t)*cos(alpha);
	v.z=R*sin(t)/2.0;
	return v;
}

void getPoints(struct Vertex** Points) {
	double t=2*PI/delta, alpha=PI/delta;
	int i,j;
	for (j=0; j<=delta; j++) {
		for (i=0; i<=delta; i++) {
			Points[j][i] = myFunction(t*j, alpha*i);
		}
	}
}

void drawFunction() {
	struct Vertex** Points = (struct Vertex**)malloc(delta*sizeof(struct Vertex*));
	int k;
	for (k=0; k<delta+1; k++) {
		Points[k] = (struct Vertex*)malloc(delta*sizeof(struct Vertex));
	}
	getPoints(Points);
	struct Vertex v1;
	struct Vertex v2;
	int i,j;
	for (j=0; j<=delta; j++) {
		glBegin(GL_QUAD_STRIP);
		for (i=0; i<=delta; i++) {
			v1 = Points[j%delta][i];
			v2 = Points[(j+1)%delta][i];
			glVertex3d(v1.x, v1.y, v1.z);
			glVertex3d(v2.x, v2.y, v2.z);
		}
		v1 = Points[j%delta][delta-1];
		v2 = Points[(j+1)%delta][delta-1];
		glVertex3d(v1.x, v1.y, v1.z);
		glVertex3d(v2.x, v2.y, v2.z);
		glEnd();
	}
	for (k=0; k<delta+1; k++) {
		free(Points[k]);
	}
	free(Points);
}

void drawCube() {
	glBegin(GL_POLYGON); // Yellow side - FRONT
      glColor3f(1.0,  1.0,  0.0);
      glVertex3f( 0.2, -0.2, -0.2);
      glVertex3f( 0.2,  0.2, -0.2);
      glVertex3f(-0.2,  0.2, -0.2);
      glVertex3f(-0.2, -0.2, -0.2);
    glEnd();
   
    glBegin(GL_POLYGON); // White side - BACK
      glColor3f(1.0,  1.0, 1.0);
      glVertex3f( 0.2, -0.2, 0.2);
      glVertex3f( 0.2,  0.2, 0.2);
      glVertex3f(-0.2,  0.2, 0.2);
      glVertex3f(-0.2, -0.2, 0.2);
    glEnd();
    
    glBegin(GL_POLYGON); // Purple side - RIGHT
      glColor3f(1.0,  0.0,  1.0);
      glVertex3f(0.2, -0.2, -0.2);
      glVertex3f(0.2,  0.2, -0.2);
      glVertex3f(0.2,  0.2,  0.2);
      glVertex3f(0.2, -0.2,  0.2);
    glEnd();
   
    glBegin(GL_POLYGON); // Green side - LEFT
      glColor3f(0.0,  1.0,  0.0);
      glVertex3f(-0.2, -0.2,  0.2);
      glVertex3f(-0.2,  0.2,  0.2);
      glVertex3f(-0.2,  0.2, -0.2);
      glVertex3f(-0.2, -0.2, -0.2);
    glEnd();
   
    glBegin(GL_POLYGON); // Blue side - TOP
      glColor3f(0.0,  0.0,  1.0);
      glVertex3f( 0.2,  0.2,  0.2);
      glVertex3f( 0.2,  0.2, -0.2);
      glVertex3f(-0.2,  0.2, -0.2);
      glVertex3f(-0.2,  0.2,  0.2);
    glEnd();
   
    glBegin(GL_POLYGON); // Red side - BOTTOM
      glColor3f(1.0,  0.0,  0.0);
      glVertex3f( 0.2, -0.2, -0.2);
      glVertex3f( 0.2, -0.2,  0.2);
      glVertex3f(-0.2, -0.2,  0.2);
      glVertex3f(-0.2, -0.2, -0.2);
    glEnd();
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	//glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if ((float)width/height > 1) {
		glOrtho(-10.0 * width/height, 10.0*width/height, -10.0, 10.0, -20, 20);
	}
	else
		glOrtho(-10.0, 10.0, -10.0*height/width, 10.0 *height/width, -20.0, 20.0);
	//glOrtho(-width/128.0,width/128.0,-height/128.0,height/128.0,-64,64);
	GLfloat matr[16] = {
		1,		0,		0,	0,
		0, 		1, 		0,	0,
		-cos(PI/4), 	-sin(PI/4),	1,	0,
		0, 		0, 		0,	1
		};
	glMultMatrixf(matr);
}

int main(int argc, char const *argv[])
{
  GLFWwindow *window;

  if (!glfwInit())
    return -1;

  window = glfwCreateWindow(520, 520, "Cubes", NULL, NULL);
  
  if (!window)
  {
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  glEnable(GL_DEPTH_TEST);
  glfwSetKeyCallback(window, keyboard_callback);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  framebuffer_size_callback(window, 520, 520);
  
  while (!glfwWindowShouldClose(window))
  {
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
	
	//draw dynamic cube
	glPushMatrix();
	glTranslatef(-0.5f, 0.0f, 0.0f);
    glRotatef(rotate_x, 1.0, 0.0, 0.0);
    glRotatef(rotate_y, 0.0, 1.0, 0.0);
    glScalef(scale,scale,scale);
    drawFunction();
    
   	glPopMatrix();
	
	//draw static cube
	glPushMatrix();
	glTranslatef(2.0f, 0.0f, 0.0f);
    //glRotatef(45, 1.0, 0.0, 0.0);
    //glRotatef(35.624, 0.0, 1.0, 0.0);
	drawCube();
	glPopMatrix();
	
	if (only_lines)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glScalef(0.0,0.0,7.0);
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}

void keyboard_callback(GLFWwindow* window, int key, int scancode,
  int action, int mods)
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);

  if (key == GLFW_KEY_UP && action == GLFW_PRESS)
    rotate_x += 10;
  if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
    rotate_x -= 10;
  if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
    rotate_y += 10;
  if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
    rotate_y -= 10;
  if (key == GLFW_KEY_L && action == GLFW_PRESS)
    only_lines=(only_lines+1)%2;
  if (key == GLFW_KEY_KP_ADD && action == GLFW_PRESS) {
  	if (scale < MAX_SCALE)
  		scale *=1.1;
  }
  if (key == GLFW_KEY_KP_SUBTRACT && action == GLFW_PRESS) {
  	if (scale > MIN_SCALE)
  		scale /=1.1;
  }
  if (key == GLFW_KEY_M && action == GLFW_PRESS) {
  	delta*=1.5;
  }
  if (key == GLFW_KEY_N && action == GLFW_PRESS) {
  	delta/=1.5;
  }
}
