#include <GLFW/glfw3.h>
#include <linmath.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define TORUS_MAJOR 0.45
#define TORUS_MINOR 0.15

float ratio;
float alpha = 75;
int partition = 32;
int prev_partition = 32;
int polygon_mode = GL_FALSE;
vec3 rotation_axis = {1.f, 1.f, 1.f};
vec3 translation = {0.f, 0.f, 0.f};
vec3 scale = {1.f, 1.f, 1.f};

struct point {
	float x;
    float y;
    float z;
};

struct point** vertices;

void calcTorus() {
    int i,j;
    double phi = 2*M_PI/partition, theta = 2*M_PI/partition;
	for (i = 0; i <= partition; i++) {
		for (j = 0; j <= partition; j++) {
            vertices[i][j].x = (TORUS_MAJOR + TORUS_MINOR * cos(theta*j)) * cos(phi*i);
            vertices[i][j].y = (TORUS_MAJOR + TORUS_MINOR * cos(theta*j)) * sin(phi*i);
            vertices[i][j].z = TORUS_MINOR * sin(theta*j);		
        }
	}
    return;
}

void drawTorus() {
	struct point first_vertex, second_vertex;
	int i,j;
	for (i = 0; i <= partition; i++) {
		glBegin(GL_QUAD_STRIP);
		for (j = 0; j <= partition; j++) {
			first_vertex = vertices[i][j];
			second_vertex = vertices[(i + 1) % partition][j];
			glVertex3f(first_vertex.x, first_vertex.y, first_vertex.z);
			glVertex3f(second_vertex.x, second_vertex.y, second_vertex.z);
		}
		first_vertex = vertices[i][partition-1];
		second_vertex = vertices[(i + 1) % partition][partition-1];
		glVertex3f(first_vertex.x, first_vertex.y, first_vertex.z);
		glVertex3f(second_vertex.x, second_vertex.y, second_vertex.z);
		glEnd();
	}
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

void resizeTorus(){
    int i;
    if(vertices != NULL){
        for(i = 0; i < prev_partition+1; i++)
            free(vertices[i]);
        free(vertices);
    }
    vertices = (struct point**)malloc((partition+1)*sizeof(struct point*));
	for (i = 0; i < partition + 1; i++) 
		vertices[i] = (struct point*)malloc((partition+1)*sizeof(struct point));
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
        prev_partition = partition;
        partition /= 2;
        resizeTorus();
        calcTorus();
    }
    if(action == GLFW_PRESS && key == GLFW_KEY_O){
        prev_partition = partition;
        partition *= 2;
        resizeTorus();
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
    drawTorus();
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
    glOrtho(-ratio, ratio, -1.f, 1.f, 10.f, -10.f);
    glRotatef(-20, -1.f, 0.f, 0.f);
    glRotatef(-20, 0.f, 1.f, 0.f);
}

int main(void){
    GLFWwindow* window = init_window();
    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    set_viewport(window, 1920, 1080);
    set_projection(1920, 1080);
    resizeTorus();
    calcTorus(); 
    while (!glfwWindowShouldClose(window)){
        draw(window);
    }
    stop(window);
    return 0;
}
