#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <linmath.h>


float ratio;
int width, height;
int polygon = 0;
int clipper_count = 0, number_of_clipper_vertices = 10;
int polygon_count = 0, number_of_polygon_vertices = 10;
int clipped_polygon_count = 0, number_of_clipped_polygon_vertices = 40;
int clip = 0;

struct point {
	double x;
    double y;
};

struct point* clipper_vertices;
struct point* polygon_vertices;
struct point* clipped_polygon_vertices;

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

int is_inside_polygon(struct point* pl, struct point p);

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
   if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if(key == GLFW_KEY_P && action == GLFW_PRESS)
        polygon = 1;
    if(key == GLFW_KEY_C && action == GLFW_PRESS){
        number_of_clipped_polygon_vertices = 4 * number_of_polygon_vertices;
        clipped_polygon_vertices = (struct point*) calloc(number_of_clipped_polygon_vertices, sizeof(struct point));
        clip = 1;
        int i;
        for(i = 0; i < clipper_count; i++){
            if(is_inside_polygon(polygon_vertices, clipper_vertices[i])){
                clipped_polygon_vertices[clipped_polygon_count] = clipper_vertices[i];
                clipped_polygon_count++;
            }
        }
    }
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

int sign(double x){
    return x > 0 ? 1 : x < 0 ? -1 : 0;
}

struct point* intersection(struct point first_start, struct point first_end, struct point second_start, struct point second_end){
    vec3 first_vector, second_vector;
    first_vector[0] = first_end.x - first_start.x;
    first_vector[1] = first_end.y - first_start.y;
    first_vector[2] = second_vector[2] = 0;
    second_vector[0] = second_end.x - second_start.x;
    second_vector[1] = second_end.y - second_start.y;
    float vec_prod_first = first_vector[0] * (second_start.y - first_start.y) - first_vector[1] * (second_start.x - first_start.x);
    float vec_prod_second = first_vector[0] * (second_end.y - first_start.y) - first_vector[1] * (second_end.x - first_start.x);
    //printf("vec_proc1 = %f %f\n", vec_prod_first, vec_prod_second);
    if(sign(vec_prod_first) == sign(vec_prod_second) || (vec_prod_first == 0) || vec_prod_second == 0)
        return NULL;
    vec_prod_first = second_vector[0] * (first_start.y - second_start.y) - second_vector[1] * (first_start.x - second_start.x);
    vec_prod_second = second_vector[0] * (first_end.y - second_start.y) - second_vector[1] * (first_end.x - second_start.x);
    //printf("vec_proc2 = %f %f\n", vec_prod_first, vec_prod_second);
    if(sign(vec_prod_first) == sign(vec_prod_second) || vec_prod_first == 0 || vec_prod_second == 0)
        return NULL;
    struct point* intersect = (struct point*) malloc(sizeof(struct point));
    intersect->x = first_start.x + first_vector[0] * fabs(vec_prod_first)/fabs(vec_prod_second-vec_prod_first);
    intersect->y = first_start.y + first_vector[1] * fabs(vec_prod_first)/fabs(vec_prod_second-vec_prod_first);
    return intersect;         
}

int is_inside_polygon(struct point* pl, struct point p){
    int c = 0;
    int i, j;    
    for (i = 0, j = polygon_count-1; i < polygon_count; j = i++) {
        if ((((pl[i].y<=p.y) && (p.y<pl[j].y)) || ((pl[j].y<=p.y) && (p.y<pl[i].y))) && (p.x > (pl[j].x - pl[i].x) * (p.y - pl[i].y) / (pl[j].y - pl[i].y) + pl[i].x))
            c = !c;
    }
    return c;
}

void Satherlend_Hodgman(struct point* polygon_vertices, struct point* clipper_vertices){
    int i, j;
    for(i = 0; i < polygon_count; i++){ 
        for(j = 0; j < clipper_count; j++){
            struct point* intersect = intersection(polygon_vertices[i], polygon_vertices[(i+1) % polygon_count], clipper_vertices[j], clipper_vertices[(j+1) % clipper_count]); 
            if(intersect != NULL){
                //printf("%d %d\n", (int)intersect->x, (int)intersect->y);
                free(intersect);
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

void draw_clipped_polygon(){
    Satherlend_Hodgman(polygon_vertices, clipper_vertices);
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
    if(clip)
        draw_clipped_polygon();
    glfwSwapBuffers(window);
    glfwPollEvents();
}

void stop(GLFWwindow *window, GLFWcursor* cursor){
    glfwDestroyWindow(window);
    glfwDestroyCursor(cursor);
    glfwTerminate();
    free(clipper_vertices);
    free(polygon_vertices);
    free(clipped_polygon_vertices);
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
    
