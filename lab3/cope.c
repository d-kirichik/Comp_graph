#include <GLFW/glfw3.h>
#include <linmath.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define TORUS_MAJOR 0.45
#define TORUS_MINOR 0.15

float vertices[300000] = {0};
float ratio;
float alpha = 75;
int partition = 4;
int polygon_mode = GL_FALSE;
vec3 rotation_axis = {1.f, 1.f, 1.f};
vec3 translation = {0.f, 0.f, 0.f};
vec3 scale = {1.f, 1.f, 1.f};

int calcTorus(){
    int i, j;
    float theta, phi;
    for(phi = 0, i = 0; phi < 2*M_PI; phi += 2.0*M_PI/partition, i++){
        glBegin(GL_POINTS);
        for(theta = -M_PI, j = 0; theta < M_PI; theta += 2.0*M_PI/partition, j++){
            vertices[3*i +j] = (TORUS_MAJOR + TORUS_MINOR * cos(phi)) * cos(theta);
            vertices[3*i+1 + j] = (TORUS_MAJOR + TORUS_MINOR * cos(phi)) * sin(theta);
            vertices[3*i+2 + j] = TORUS_MINOR * sin(phi);
     //       glVertex3f(vertices[3*i+j], vertices[3*i+1+j], vertices[3*i+2+j]);
        }
        glEnd();
    }
    return i*j;
}

void drawTorus(){
    int count = calcTorus(vertices);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glDrawArrays(GL_POINTS, 0, 30000);
    glEnd();
    glDisableClientState(GL_VERTEX_ARRAY);  
}

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
    if(action == GLFW_PRESS && key == GLFW_KEY_P){
        partition /= 2;
	calcTorus();
    }
    if(action == GLFW_PRESS && key == GLFW_KEY_O){
        partition *= 2;
	calcTorus();
    }
    if(action == GLFW_PRESS && key == GLFW_KEY_RIGHT){
        translation[2] += 0.15f;
        translation[0] += 0.10f;
    }
    if(action == GLFW_PRESS && key == GLFW_KEY_LEFT){
        translation[2] -= 0.15f;
        translation[0] -= 0.10f;
    }
    if(action == GLFW_PRESS && key == GLFW_KEY_UP){
        translation[1] += 0.15f;
        translation[0] += 0.10f;
    }
    if(action == GLFW_PRESS && key == GLFW_KEY_DOWN){
        translation[1] -= 0.15f;
        translation[0] -= 0.10f;
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}

GLFWwindow* init_window(){
    GLFWwindow* window;
    if (!glfwInit()){
        exit(EXIT_FAILURE);
    }
    window = glfwCreateWindow(1920, 1080, "lab3", NULL, NULL);
    if (!window){
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    return window;
}

void draw(GLFWwindow *window){
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPushMatrix(); 
    glRotatef(alpha,rotation_axis[0],rotation_axis[1], rotation_axis[2]);
    glTranslatef(translation[0], translation[1], translation[2]);
    glScalef(scale[0], scale[1], scale[2]);
    drawTorus(vertices);
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

void set_viewport(GLFWwindow* window, int width, int height){
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
}

void set_projection(int width, int height){
    ratio = width / (float) height;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
    glRotatef(-15, -0.5f, 0.5f, 0.f);
}

int main(void){
    GLFWwindow* window = init_window();
    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    set_viewport(window, 1920, 1080);
    set_projection(1920, 1080); 
    while (!glfwWindowShouldClose(window)){
        draw(window);
    }
    stop(window);
    return 0;
}


