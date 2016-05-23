#include <math.h>
#include <stdio.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>

int H=500,W=800;


void drawST()
{
	
	glLoadIdentity();
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	glRotatef(15, 1, 0, 0);
	glRotatef(15, 0, 1, 0);

	GLfloat vertices1[] =
	{
		-4,-4,-4,    4,-4,-4,    4,-4, 4,   -4,-4, 4,
		-4,-4,-4,   -4,-4, 4,   -4, 4, 4,   -4, 4,-4,
		-4,-4,-4,   -4, 4,-4,    4, 4,-4,    4,-4,-4,
		 4, 4, 4,    4, 4,-4,   -4, 4,-4,   -4, 4, 4,
		 4, 4, 4,    4, 4,-4,    4,-4,-4,    4,-4, 4,
		 4, 4, 4,    4,-4, 4,   -4,-4, 4,   -4, 4, 4
	};
    
	GLfloat colors1[] =
	{
		1, 0, 1,   1, 0, 1,   1, 0, 1,   1, 0, 1,
		1, 1, 0,   1, 1, 0,   1, 1, 0,   1, 1, 0,
		0, 1, 1,   0, 1, 1,   0, 1, 1,   0, 1, 1,
		0, 1, 0,   0, 1, 0,   0, 1, 0,   0, 1, 0,
		1, 0, 0,   1, 0, 0,   1, 0, 0,   1, 0, 0,
		0, 0, 1,   0, 0, 1,   0, 0, 1,   0, 0, 1,
	};

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vertices1);
	glColorPointer(3, GL_FLOAT, 0, colors1);
    
	glDrawArrays(GL_QUADS, 0, 24);
	
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void draw()
{
	int j,iterations=100;
	glClear(GL_ACCUM_BUFFER_BIT);
	glDrawBuffer(GL_BACK);
	glReadBuffer(GL_BACK);
	drawST();
	glAccum(GL_ACCUM, 0);
	glDrawBuffer(GL_FRONT);
        glAccum(GL_RETURN, 0);
	glDrawBuffer(GL_BACK);
	glAccum(GL_RETURN, 0);
	
}

static void resize_callback(GLFWwindow* window, int width, int height)
{	
	H=height;
	W=width;
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-W/50, W/50, -H/50, H/50, -30.0, 30.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

static void keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	
}

int main()
{
    if(!glfwInit())
	{
		printf("glfwInit failed\n");
		return -1;
	}
	
	
	GLFWwindow* window;
	glfwWindowHint(GLFW_ACCUM_RED_BITS, 8);
	glfwWindowHint(GLFW_ACCUM_GREEN_BITS, 8);
	glfwWindowHint(GLFW_ACCUM_BLUE_BITS, 8);
	glfwWindowHint(GLFW_ACCUM_ALPHA_BITS, 8);
	window = glfwCreateWindow(800, 500, "Test", NULL, NULL);
/*	glfwWindowHint(GLFW_ACCUM_RED_BITS, 8);
	glfwWindowHint(GLFW_ACCUM_GREEN_BITS, 8);
	glfwWindowHint(GLFW_ACCUM_BLUE_BITS, 8);
	glfwWindowHint(GLFW_ACCUM_ALPHA_BITS, 8);*/

	int acc=-1,col=-1;
	acc=glfwGetWindowAttrib(window,GL_ACCUM_RED_BITS);
	col=glfwGetWindowAttrib(window,GL_RED_BITS);
	printf("Accum=%d\nColor=%d\n\n",acc,col);
	if (window == NULL)
	{
		printf("glfwOpenWindow failed.\n");
		glfwTerminate();
		return -2;
	}
	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, keyboard_callback);
	glfwSetFramebufferSizeCallback(window, resize_callback);
	
	resize_callback(window, W , H );
	//glEnable(GL_BLEND);
	draw();
	glfwSwapBuffers(window);
	while (!glfwWindowShouldClose(window))
	{
		

		//glfwSwapBuffers(window);

		glfwPollEvents();
		//glfwWaitEvents();
	}

	glfwDestroyWindow(window);

	// clean up and exit
	glfwTerminate();

	return 0;
}
