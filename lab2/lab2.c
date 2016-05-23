#include <GLFW/glfw3.h>
#include <linmath.h>
#include <stdlib.h>
#include <stdio.h>

float ratio;
float alpha = 0;
int width, height;
int polygon_mode = GL_FALSE;
vec3 rotation_axis = {1.f, 1.f, 1.f};
vec3 translation = {0.f, 0.f, 0.f};
vec3 scale = {1.f, 1.f, 1.f};


void drawCube(float center_x, float center_y, float center_z){

    if (polygon_mode == GL_TRUE){
        glDisable(GL_CULL_FACE);
	    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
	else{
        glEnable(GL_CULL_FACE); 
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);   
    }

    glBegin(GL_POLYGON);
    glColor3f(1.0, 0.0, 1.0);
    glVertex3f(center_x - 0.15f, center_y-0.15f, center_z-0.15f);
    glVertex3f(center_x - 0.15f, center_y + 0.15f, center_z-0.15f);
    glVertex3f(center_x + 0.15f, center_y + 0.15f, center_z-0.15f);
    glVertex3f(center_x + 0.15f, center_y-0.15f, center_z-0.15f);
    glEnd();
 
    glBegin(GL_POLYGON);
    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(center_x-0.15f, center_y-0.15f, center_z-0.15f);
    glVertex3f(center_x + 0.15f, center_y-0.15f, center_z-0.15f);
    glVertex3f(center_x + 0.15f, center_y-0.15f, center_z + 0.15f);
    glVertex3f(center_x + -0.15f, center_y-0.15f, center_z + 0.15f);
    glEnd();
 
    glBegin(GL_POLYGON);
    glColor3f(0.1, 1.0, 0.1);
    glVertex3f(center_x-0.15f, center_y-0.15f, center_z-0.15f);
    glVertex3f(center_x-0.15f, center_y-0.15f, center_z + 0.15f);
    glVertex3f(center_x-0.15f, center_y + 0.15f, center_z + 0.15f);
    glVertex3f(center_x-0.15f, center_y + 0.15f, center_z-0.15f);
    glEnd();
 
    glBegin(GL_POLYGON);
    glColor3f(0.0, 1.0, 1.0);
    glVertex3f(center_x-0.15f, center_y-0.15f, center_z + 0.15f);
    glVertex3f(center_x + 0.15f, center_y-0.15f, center_z + 0.15f);
    glVertex3f(center_x + 0.15f, center_y + 0.15f, center_z + 0.15f);
    glVertex3f(center_x-0.15f, center_y + 0.15f, center_z + 0.15f);
    glEnd();
 
    glBegin(GL_POLYGON);
    glColor3f(1.0, 1.0, 1.0);
    glVertex3f(center_x-0.15f, center_y + 0.15f, center_z-0.15f);
    glVertex3f(center_x-0.15f, center_y + 0.15f, center_z + 0.15f);
    glVertex3f(center_x + 0.15f, center_y + 0.15f, center_z + 0.15f);
    glVertex3f(center_x + 0.15f, center_y + 0.15f, center_z-0.15f);
    glEnd();
 
    glBegin(GL_POLYGON);
    glColor3f(1.0, 1.0, 0.0);
    glVertex3f(center_x + 0.15f, center_y-0.15f, center_z-0.15f);
    glVertex3f(center_x + 0.15f, center_y + 0.15f, center_z-0.15f);
    glVertex3f(center_x + 0.15f, center_y + 0.15f, center_z + 0.15f);
    glVertex3f(center_x + 0.15f, center_y-0.15f, center_z + 0.15f);
    glEnd();
 
    glFlush();
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if(action == GLFW_PRESS && key == GLFW_KEY_R)
        alpha += 15;
    if(action == GLFW_PRESS && key == GLFW_KEY_T)
        alpha -= 15;
    if(action == GLFW_PRESS && key == GLFW_KEY_L){
        scale[0] *= 1.1f;
        scale[1] *= 1.1f;
        scale[2] *= 1.1f;
    }
    if(action == GLFW_PRESS && key == GLFW_KEY_M){
        scale[0] *= 0.9f;
        scale[1] *= 0.9f;
        scale[2] *= 0.9f;
    }
    if(action == GLFW_PRESS && key == GLFW_KEY_C){
        polygon_mode = !polygon_mode;
    } 
}

GLFWwindow* init_window(){
    GLFWwindow* window;
    if (!glfwInit()){
        exit(EXIT_FAILURE);
    }
    window = glfwCreateWindow(1920, 1080, "Simple example", NULL, NULL);
    if (!window){
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    return window;
}

void set_viewport(GLFWwindow* window){
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
}

void set_projection(){
    ratio = width / (float) height;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
    glRotatef(-15, 0.0f, 1.f, 1.f);
}

void draw(GLFWwindow *window){
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPushMatrix(); 
    glRotatef(alpha,rotation_axis[0],rotation_axis[1], rotation_axis[2]);
    glTranslatef(translation[0], translation[1], translation[2]);
    glScalef(scale[0], scale[1], scale[2]);
    drawCube(0, 0, 0);
    glPopMatrix();
    drawCube(-1,-0.5f,0);
    glfwSwapBuffers(window);
    glfwPollEvents();
}

void stop(GLFWwindow *window){
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

int main(void){
    
    GLFWwindow* window = init_window();
    glfwSetKeyCallback(window, key_callback);
   while (!glfwWindowShouldClose(window)){
        set_viewport(window);
        set_projection();
        draw(window);
    }
    stop(window);
    return 0;
}


