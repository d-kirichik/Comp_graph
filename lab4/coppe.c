#include <GLFW/glfw3.h>
#include <linmath.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define INTENSITY 256 

float ratio;
int count = 0;
int number_of_vertices = 10;
int drawable = 0;
int width, height;
int delimiter_pos = 0;

struct point {
	double x;
    double y;
    int inverse;
};

struct pixel{
    float red, green, blue;
};

struct point* vertices;
struct pixel* pixels;

void set_viewport(GLFWwindow* window){
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
}

void set_projection(){
    ratio = width / (float) height;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, height, 0, 0.1f, -0.1f);
}

void set_delimiter(){
    delimiter_pos = vertices[0].x;
    int i;
    for(i = 0; i < count; i++){
        if(vertices[i].x > delimiter_pos){
            delimiter_pos = vertices[i].x;   
        }       
    }
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
   if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if(key == GLFW_KEY_D && action == GLFW_PRESS){
        drawable++;
    }
    if(key == GLFW_KEY_R && action == GLFW_PRESS){
        count--;
    }
    if(key == GLFW_KEY_C && action == GLFW_PRESS){
        count = drawable = 0;
        vertices = (struct point*) calloc(number_of_vertices, sizeof(struct point));
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    set_viewport(window);
    set_projection();
    if (pixels != NULL) {
        free(pixels);
    }
    pixels = calloc(width * height, sizeof(struct pixel));
    set_delimiter();
}

void mouse_button_callback(GLFWwindow* window, int button ,int action, int mode){
    if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
        glfwGetCursorPos(window, &vertices[count].x, &vertices[count].y);
        vertices[count].inverse = 0;
        printf("%f, %f\n", vertices[count].x, vertices[count].y);
        count++;
        set_delimiter();
        if(count == number_of_vertices){
            number_of_vertices *= 2;
            vertices = (struct point*) realloc(vertices, number_of_vertices * sizeof(struct point)); 
        } 
    }
}

GLFWwindow* init_window(){
    GLFWwindow* window;
    if (!glfwInit()){
        exit(EXIT_FAILURE);
    }
    window = glfwCreateWindow(1920, 1080, "lab4", NULL, NULL);
    if (!window){
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    vertices = (struct point*) calloc(number_of_vertices, sizeof(struct point));
    if(vertices == NULL)
        exit(EXIT_FAILURE);
    return window;
}

int sign(int x) {
    return x > 0 ? 1 : x < 0 ? -1 : 0;
}

void check_edges(){
    int i;
    for(i = 0; i < count; i++){
        if(sign(vertices[(i+1) % count].y - vertices[i].y) == sign(vertices[(i+1) % count].y - vertices[(i+2) % count].y))
            vertices[(i+1) % count].inverse = 1;
    }
}

void inverse_pixel(struct pixel* pixel){
    pixel->red = 1.f - pixel->red;
    pixel->green = 1.f - pixel->green;
    pixel->blue = 1.f - pixel->blue;
}

void inverse_right(struct point a, struct point b){
    int start_x = (int) a.x,  start_y = (int) a.y,  end_x = (int) b.x,  end_y = (int) b.y;
    int y = start_y;
    int delta_y = sign(end_y - start_y);
    if(start_y == end_y)
        return;
    while (y != end_y) {
        if(y == start_y && (a.inverse == 1)){
            y += delta_y;
            continue;
        }
        int x_inv = start_x + (end_x - start_x) * (y - start_y) / (end_y - start_y);
        while(x_inv < delimiter_pos){
            inverse_pixel(&pixels[(height-y) * width + x_inv]);
            x_inv++;
        } 
        while(x_inv > delimiter_pos){
            inverse_pixel(&pixels[(height-y) * width + x_inv]);
            x_inv--;
        }
        y += delta_y;
    }
    glDrawPixels(width, height, GL_RGB, GL_FLOAT, pixels);
    return;
} 

void set_pixel(struct pixel* pixel, int c){
    pixel->red = pixel->green = pixel->blue = (1.f * c)/255;
}

int get_direction(struct point a, struct point b){
    int dx = b.x - a.x;
    int dy = b.y - a.y;
    float der = dx/dy;
    printf("dx & dy %d %d %f\n", dx, dy, der);
    if(dx < 0 && dy > 0 && der <= -1){
        return 0; 
    }
    if(dx > 0 && dy > 0 && der >= 1){
        return 1;
    }
    if(dx > 0 && dy < 0 && der <= -1 ){
        return 2;
    }
    if(dx < 0 && dy < 0 && der >= 1 ){
        return 3;
    }
    return -1;
}

void algorithm_Bresenheim(struct point a, struct point b, int direction){
    int x_start = (int)a.x, x_end = (int) b.x, y_start = (int)a.y, y_end = (int)b.y; 
    const int delta_x = x_end - x_start;
    const int delta_y = y_end - y_start;
    printf("direction %d\n", direction); 
    int m = abs((255*delta_y)/delta_x);
    int delta_err = 255/2;
    int w = 255 - m;
    set_pixel(&pixels[(height-y_start) * width + x_start], m/2);
    switch (direction){
        case 0:
            while(x_start > x_end) {
    	        if(delta_err < w){
                    x_start--;
                    delta_err += m;
                }
                else{
                    x_start--;
                    y_start++;
                    delta_err -= w;
                }
            set_pixel(&pixels[(height-y_start) * width + x_start], delta_err);
            }
        glDrawPixels(width, height, GL_RGB, GL_FLOAT, pixels);
        break;
        case 1:
            while(x_start < x_end) {
    	        if(delta_err < w){
                    x_start++;
                    delta_err += m;
                }
                else{
                    x_start++;
                    y_start++;
                    delta_err -= w;
                }
            set_pixel(&pixels[(height-y_start) * width + x_start], delta_err);
            }
        glDrawPixels(width, height, GL_RGB, GL_FLOAT, pixels);
        break;
        case 2:
            while(x_start < x_end) {
    	        if(delta_err < w){
                    x_start++;
                    delta_err += m;
                }
                else{
                    x_start++;
                    y_start--;
                    delta_err -= w;
                }
            set_pixel(&pixels[(height-y_start) * width + x_start], delta_err);
            }
        glDrawPixels(width, height, GL_RGB, GL_FLOAT, pixels);
        break;
        case 3:
            while(x_start < x_end) {
    	        if(delta_err < w){
                    x_start--;
                    delta_err += m;
                }
                else{
                    x_start--;
                    y_start--;
                    delta_err -= w;
                }
            set_pixel(&pixels[(height-y_start) * width + x_start], delta_err);
            }
        glDrawPixels(width, height, GL_RGB, GL_FLOAT, pixels);
        break;
    }
}


void draw_polygon(){
    memset(pixels, 0, width * height * sizeof(struct pixel));
    int i;
    if(count > 1){
        for(i = 0; i < count; i++){
            int direction = get_direction(vertices[i], vertices[(i+1)%count]);
            algorithm_Bresenheim(vertices[i], vertices[(i+1) % count], direction);
        }
    }
}

void inverse_polygon(){
    int i;
    for(i = 0; i < count ; i++)
        vertices[i].inverse = 0;
    check_edges();
    for(i = 0; i < count; i++){
        inverse_right(vertices[i], vertices[(i+1) % count]);
    }
}

void draw(GLFWwindow *window){
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    draw_polygon();
    if(drawable)
        inverse_polygon();
    glfwSwapBuffers(window);
    glfwPollEvents();
}

void stop(GLFWwindow *window, GLFWcursor* cursor){
    glfwDestroyWindow(window);
    glfwDestroyCursor(cursor);
    glfwTerminate();
    free(vertices);
    free(pixels);
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
    
