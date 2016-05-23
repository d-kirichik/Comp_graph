#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <stdio.h>

float angle = 0.5f;

static void error_callback(int error, const char* description){
    fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (key == GLFW_KEY_R && action == GLFW_PRESS)
		angle = -angle;		
}

void draw(){
	   	glClear(GL_COLOR_BUFFER_BIT);
		glRotatef(angle, 0.f, 0.f, 1.f);

	glBegin(GL_QUADS);

	glColor3f(1.0, 0.0, 0.0);
    glVertex2f(-0.5f, 0.5f);  
    glColor3f(0.f, 1.f, 0.f);
    glVertex2f( 0.5f, 0.5f);  
    glColor3f(0.f, 0.f, 1.f);
    glVertex2f( 0.5f,-0.5f); 
    glVertex2f(-0.5f,-0.5f);
    glEnd();
}

void init_window(GLFWwindow* window){
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
}

int main(void){
    GLFWwindow* window;

    glfwSetErrorCallback(error_callback);

    if (!glfwInit())      
        exit(EXIT_FAILURE);

    window = glfwCreateWindow(640, 480, "Lab1", NULL, NULL);
    if (!window){
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window); 
    glfwSwapInterval(1); 

    glfwSetKeyCallback(window, key_callback);
	
	init_window(window);
	
    while (!glfwWindowShouldClose(window)){
 		draw();       
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}
