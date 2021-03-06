#include <GLFW/glfw3.h>
#include <linmath.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define INTENSITY 255.f 

float ratio;
int count = 0;
int number_of_vertices = 10;
int drawable = 0;
int width, height;
int delimiter_pos = 0;
int lines = 1;
int smooth = 0;

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

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
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
        lines = 0;
    }
    if(key == GLFW_KEY_R && action == GLFW_PRESS){
        count--;
        smooth = 0;
    }
    if(key == GLFW_KEY_S && action == GLFW_PRESS){
        smooth = 1;
    }
    if(key == GLFW_KEY_C && action == GLFW_PRESS){
        count = drawable = 0;
        lines = 1;
        smooth = 0;
        free(vertices);
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
        if(vertices[i].y == vertices[(i+1)%count].y)
            vertices[i].inverse = vertices[(i+1)%count].inverse = 1;
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
    while (y != end_y) {
        if(y == start_y && (a.inverse == 1)){
            y += delta_y;
            continue;
        }
        int x_inv = floor(start_x +floor((end_x - start_x) * (double)(y - start_y) / (end_y - start_y))) + 1 ;
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
    return;
} 

void to_zero(int *x, int *y, int direction) {
    int new_x = *x, new_y = *y;
    switch (direction) {
        case 0: {
            *x = new_x;
            *y = new_y;
        }
        break;
        case 1: {
            *x = new_y;
            *y = new_x;
        }
        break;
        case 2: {
            *x = new_y;
            *y = -new_x;
        }
        break;
        case 3: {
            *x = -new_x;
            *y = new_y;
        }
        break;
        case 4: {
            *x = -new_x;
            *y = -new_y;
        }
        break;
        case 5: {
            *x = -new_y;
            *y = -new_x;
        }
        break;
        case 6: {
            *x = -new_y;
            *y = new_x;
        }
        break;
        case 7: {
            *x = new_x;
            *y = -new_y;
        }
        break;
    }
}

void from_zero(int *x, int *y, int direction) {
    int new_x = *x, new_y = *y;
    switch (direction) {
        case 0: {
            *x = new_x;
            *y = new_y;
        }
        break;
        case 1: {
            *x = new_y;
            *y = new_x;
        }
        break;
        case 2: {
            *x = -new_y;
            *y = new_x;
        }
        break;
        case 3: {
            *x = -new_x;
            *y = new_y;
        }
        break;
        case 4: {
            *x = -new_x;
            *y = -new_y;
        }
        break;
        case 5: {
            *x = -new_y;
            *y = -new_x;
        }
        break;
        case 6: {
            *x = new_y;
            *y = -new_x;
        }
        break;
        case 7: {
            *x = new_x;
            *y = -new_y;
        }
        break;
    }
}

void mark_pixel(struct pixel* pixel, int intensity){
	pixel->red = pixel->green = pixel->blue = (1.f * intensity)/255;
}

void set_pixel(int x, int y, int dir, int intensity) {
    from_zero(&x, &y, dir);
    mark_pixel(&pixels[(height-y) * width + x], intensity);
}

int get_dir(struct point a, struct point b){
    int dx = b.x - a.x;
    int dy = b.y - a.y;
    if(dx > 0 && dy > 0 && dx >= dy)
        return 0;
    if(dx > 0 && dy > 0 && dx < dy)
        return 1;
    if(dx < 0 && dy > 0 && abs(dx) <= dy)
        return 2;
    if(dx < 0 && dy > 0 && abs(dx) > dy)
        return 3;
    if(dx < 0 && dy < 0 && dx <= dy)
        return 4;
    if(dx < 0 && dy < 0 && dx > dy)
        return 5;
    if(dx > 0 && dy < 0 && dx < abs(dy))
        return 6;
    if(dx > 0 && dy < 0 && dx >= abs(dy))
        return 7;
    return -1;
}
    
void algorithm_Bresenham(int x0, int y0, int x1, int y1, int oct) {
	int dx = x1 - x0;
    int dy = y1 - y0;
    float m = INTENSITY * dy / dx;
    float err = INTENSITY / 2;
    float w = INTENSITY - m;
    set_pixel(x0, y0, oct, INTENSITY / 2);
    int y = y0;
    int x = x0;
    for (x = x0; x <= x1; x++) {
        float intensity;
        if (oct % 2)
            intensity = INTENSITY - err;
        else
            intensity = err;
        set_pixel(x, y, oct, intensity);
        if (err < w) {
            err += m;
        } else {
            y++;
            err -= w;
        }
    }
}

void draw_line(struct point a, struct point b) {
    int x0 = a.x;
    int y0 = a.y;
    int x1 = b.x;
    int y1 = b.y;
	int dir = get_dir(a, b);
    printf("%d\n", dir);
    to_zero(&x0, &y0, dir);
    to_zero(&x1, &y1, dir);
    algorithm_Bresenham(x0, y0, x1, y1, dir);
}

void draw_polygon(){
    memset(pixels, 0, width * height * sizeof(struct pixel));
    int i;
    if(count > 1){
        if(count == 2 && lines){
            draw_line(vertices[0], vertices[1]);
            return;
        }
        for(i = 0; i < count; i++){
                draw_line(vertices[i], vertices[(i+1) % count]);
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
        if(smooth){
            int dir = get_dir(vertices[i], vertices[(i+1)%count]);
            printf("%d\n",get_dir(vertices[i], vertices[(i+1)%count]));
            if(dir == 5 || dir == 4)
                draw_line(vertices[(i+1) % count], vertices[i]);
            else 
                draw_line(vertices[i], vertices[(i+1) % count]);
        }
    }
}

void draw(GLFWwindow *window){
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    draw_polygon();
    if(drawable){
        inverse_polygon();
    }
	glDrawPixels(width, height, GL_RGB, GL_FLOAT, pixels);
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
    
