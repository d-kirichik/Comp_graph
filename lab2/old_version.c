#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GLFW/glfw3.h>
#include <linmath.h>
#include "gluPerspective.h"

int polygon_mode = GL_FALSE;
float alpha = 0;
float temp[16];
vec3 rotation_axis = {1.f, 1.f, 1.f};
vec3 translation = {0.f, 0.f, 0.f};
vec3 scale = {1.f, 1.f, 1.f};

void callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    if(action == GLFW_PRESS && key == GLFW_KEY_ESCAPE)
            glfwSetWindowShouldClose(window, GL_TRUE);
    if(action == GLFW_PRESS && key == GLFW_KEY_R)
        alpha += 15;
    if(action == GLFW_PRESS && key == GLFW_KEY_UP)
        translation[1] += 0.1f;
    if(action == GLFW_PRESS && key == GLFW_KEY_DOWN)
        translation[1] -= 0.1f;
    if(action == GLFW_PRESS && key == GLFW_KEY_LEFT)
        translation[0] -= 0.1f;
    if(action == GLFW_PRESS && key == GLFW_KEY_RIGHT)
        translation[0] += 0.1f;
    if(action == GLFW_PRESS && key == GLFW_KEY_S){
        scale[0] *= 1.1f;
        scale[1] *= 1.1f;
        scale[2] *= 1.1f;
    }
    if(action == GLFW_PRESS && key == GLFW_KEY_C){
        polygon_mode = !polygon_mode;
    }    
}

GLFWwindow* initWindow(){
    if(!glfwInit()){
        return NULL;
    }
    GLFWwindow* window = glfwCreateWindow(1920, 1080, "Lab2", NULL, NULL);
    if(window == NULL){
        glfwTerminate();
        return NULL;
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, callback);
    glEnable(GL_DEPTH_TEST); 
    glEnable(GL_NORMALIZE);
    return window;
}

void set_projection_matrix (float width, float height){
    glMatrixMode(GL_PROJECTION_MATRIX);
    glLoadIdentity();
    gluPerspective(temp, 50, (float)width / (float)height, 1, 700);
    glLoadMatrixf(temp);
}

void set_modelview_matrix(){
    glMatrixMode(GL_MODELVIEW_MATRIX);
    glTranslatef(0,0,-20);
}

void drawCube(int center_x, int center_y, int center_z){
	
    if (polygon_mode == GL_TRUE)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);   
    
    glBegin(GL_POLYGON);
    glColor3f(1.0, 0.0, 1.0);
    glVertex3f(center_x - 1.0f, center_y-1.0f, center_z-1.0f);
    glVertex3f(center_x - 1.0f, center_y + 1.0f, center_z-1.0f);
    glVertex3f(center_x + 1.0f, center_y + 1.0f, center_z-1.0f);
    glVertex3f(center_x + 1.0f, center_y-1.0f, center_z-1.0f);
    glEnd();
 
    glBegin(GL_POLYGON);
    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(center_x-1.0f, center_y-1.0f, center_z-1.0f);
    glVertex3f(center_x + 1.0f, center_y-1.0f, center_z-1.0f);
    glVertex3f(center_x + 1.0f, center_y-1.0f, center_z + 1.0f);
    glVertex3f(center_x + -1.0f, center_y-1.0f, center_z + 1.0f);
    glEnd();
 
    glBegin(GL_POLYGON);
    glColor3f(0.1, 1.0, 0.1);
    glVertex3f(center_x-1.0f, center_y-1.0f, center_z-1.0f);
    glVertex3f(center_x-1.0f, center_y-1.0f, center_z + 1.0f);
    glVertex3f(center_x-1.0f, center_y + 1.0f, center_z + 1.0f);
    glVertex3f(center_x-1.0f, center_y + 1.0f, center_z-1.0f);
    glEnd();
 
    glBegin(GL_POLYGON);
    glColor3f(0.0, 1.0, 1.0);
    glVertex3f(center_x-1.0f, center_y-1.0f, center_z + 1.0f);
    glVertex3f(center_x + 1.0f, center_y-1.0f, center_z + 1.0f);
    glVertex3f(center_x + 1.0f, center_y + 1.0f, center_z + 1.0f);
    glVertex3f(center_x-1.0f, center_y + 1.0f, center_z + 1.0f);
    glEnd();
 
    glBegin(GL_POLYGON);
    glColor3f(1.0, 1.0, 1.0);
    glVertex3f(center_x-1.0f, center_y + 1.0f, center_z-1.0f);
    glVertex3f(center_x-1.0f, center_y + 1.0f, center_z + 1.0f);
    glVertex3f(center_x + 1.0f, center_y + 1.0f, center_z + 1.0f);
    glVertex3f(center_x + 1.0f, center_y + 1.0f, center_z-1.0f);
    glEnd();
 
    glBegin(GL_POLYGON);
    glColor3f(1.0, 1.0, 0.0);
    glVertex3f(center_x + 1.0f, center_y-1.0f, center_z-1.0f);
    glVertex3f(center_x + 1.0f, center_y + 1.0f, center_z-1.0f);
    glVertex3f(center_x + 1.0f, center_y + 1.0f, center_z + 1.0f);
    glVertex3f(center_x + 1.0f, center_y-1.0f, center_z + 1.0f);
    glEnd();
 
    glFlush();
}

void draw(){
    glPushMatrix(); 
    glRotatef(alpha,rotation_axis[0],rotation_axis[1], rotation_axis[2]);
    glTranslatef(translation[0], translation[1], translation[2]);
    glScalef(scale[0], scale[1], scale[2]);
    drawCube(0, 0, 0);
    glPopMatrix();
}

int main(int argc, char** argv){
    GLFWwindow* window = initWindow();
    int width, height;
    while(!glfwWindowShouldClose(window)){
        glfwGetWindowSize(window, &width, &height);
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        set_projection_matrix(width, height);
        set_modelview_matrix(); 
        draw();
        drawCube(-10, -10, -10);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }    
   
    glfwDestroyWindow(window);
    glfwTerminate();
    
    return 0;
}
