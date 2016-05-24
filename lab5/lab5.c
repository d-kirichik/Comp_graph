#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>


float ratio;
int width, height;
int polygon = 0;
int clipper_count = 0, number_of_clipper_vertices = 10;
int polygon_count = 0, number_of_polygon_vertices = 10;

struct point {
	double x;
    double y;
};

struct point* clipper_vertices;
struct point* polygon_vertices;

void set_viewport(GLFWwindow* window){
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
}

void set_projection(){
    ratio = width / (float) height;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, height, 0, 0.1f, -0.1f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
   if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if(key == GLFW_KEY_P && action == GLFW_PRESS)
        polygon = 1;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    set_viewport(window);
    set_projection();
}

void mouse_button_callback(GLFWwindow* window, int button ,int action, int mode){
    if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
        if(polygon == 0){
            glfwGetCursorPos(window, &clipper_vertices[clipper_count].x, &clipper_vertices[clipper_count].y);
            clipper_count++;
            if(clipper_count == number_of_clipper_vertices){
                number_of_clipper_vertices *= 2;
                clipper_vertices = (struct point*) realloc(clipper_vertices, number_of_clipper_vertices * sizeof(struct point)); 
            }
        }
        else if(polygon == 1){
            glfwGetCursorPos(window, &polygon_vertices[polygon_count].x, &polygon_vertices[polygon_count].y);
            polygon_count++;
            if(polygon_count == number_of_polygon_vertices){
                number_of_polygon_vertices *= 2;
                polygon_vertices = (struct point*) realloc(polygon_vertices, number_of_polygon_vertices * sizeof(struct point)); 
            }
        } 
    }
}

void draw_clipper(){
    glBegin(GL_LINE_LOOP);
    int i;
    for(i = 0; i < clipper_count; i++){
        glVertex2f(clipper_vertices[i].x, clipper_vertices[i].y);
    } 
    glEnd();
}

void draw_polygon(){
    glBegin(GL_POLYGON);
    int i;
    for(i = 0; i < polygon_count; i++){
        glVertex2f(polygon_vertices[i].x, polygon_vertices[i].y);
    }
    glEnd();
}

GLFWwindow* init_window(){
    GLFWwindow* window;
    if (!glfwInit()){
        exit(EXIT_FAILURE);
    }
    window = glfwCreateWindow(1920, 1080, "lab5", NULL, NULL);
    if (!window){
        glfwTerminate();
        exit(EXIT_FAILURE);
    } 
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    clipper_vertices = (struct point*) calloc(number_of_clipper_vertices, sizeof(struct point));
    if(clipper_vertices == NULL)
        exit(EXIT_FAILURE);
    polygon_vertices = (struct point*) calloc(number_of_polygon_vertices, sizeof(struct point));
    if(polygon_vertices == NULL)
        exit(EXIT_FAILURE);
    return window;
}

void draw(GLFWwindow *window){
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    draw_clipper();
    if(polygon)
        draw_polygon();
    glfwSwapBuffers(window);
    glfwPollEvents();
}

void stop(GLFWwindow *window, GLFWcursor* cursor){
    glfwDestroyWindow(window);
    glfwDestroyCursor(cursor);
    glfwTerminate();
    free(clipper_vertices);
    free(polygon_vertices);
}

int main(void){
    GLFWwindow* window = init_window();
    GLFWcursor* cursor = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);     
    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    framebuffer_size_callback(window, 1920, 1080);
    while (!glfwWindowShouldClose(window)){
        draw(window);
    }
    stop(window, cursor);
    return 0;
}
    
