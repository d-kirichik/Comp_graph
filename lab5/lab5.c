#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <linmath.h>
#include <string.h>

float ratio;
int width, height;
int polygon = 0;
int clipper_count = 0, number_of_clipper_vertices = 10;
int polygon_count = 0, number_of_polygon_vertices = 10;
int clipped_polygon_count = 0, number_of_clipped_polygon_vertices = 40;
int clip = 0;
int buf_count = 0;

struct point {
    double x;
    double y;
};

struct point* clipper_vertices;
struct point* polygon_vertices;
struct point* clipped_polygon_vertices;
struct point* buffer;

void set_viewport(GLFWwindow* window){
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
}

void set_projection(GLFWwindow* window){
    glfwGetFramebufferSize(window, &width, &height);
    ratio = width / (float) height;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, height, 0, 0.1f, -0.1f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void Satherlend_Hodgman(const struct point*, struct point*);

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
   if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if(key == GLFW_KEY_P && action == GLFW_PRESS)
        polygon = 1;
    if(key == GLFW_KEY_C && action == GLFW_PRESS){
        number_of_clipped_polygon_vertices = 4 * number_of_polygon_vertices;
        clipped_polygon_vertices = (struct point*) calloc(number_of_clipped_polygon_vertices, sizeof(struct point));
        clip = 1;
        Satherlend_Hodgman(polygon_vertices, clipper_vertices);
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    set_projection(window);
    set_viewport(window);
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

struct point* intersect(struct point p1, struct point p2, struct point p3, struct point p4){
    double denom = (p4.y - p3.y) * (p2.x - p1.x) - (p4.x - p3.x) * (p2.y - p1.y);
    if(!denom)
        return NULL;
    double f_inter_param = ((p4.x - p3.x) * (p1.y - p3.y) - (p4.y - p3.y) * (p1.x - p3.x)) / denom;
    double s_inter_param = ((p2.x - p1.x) * (p1.y - p3.y) - (p2.y - p1.y) * (p1.x - p3.x)) / denom;
    if(0 < f_inter_param && f_inter_param < 1){
        struct point *intersection = (struct point *)malloc(sizeof(struct point));
        intersection->x = p1.x + f_inter_param * (p2.x - p1.x);
        intersection->y = p1.y + f_inter_param * (p2.y - p1.y);
        printf("params %f %f\n", f_inter_param, s_inter_param);
        printf("point =  %p\n", intersection);
        return intersection;
    }
    else return NULL;
}

int is_visible(struct point *vertices, struct point p, int begin, int end, size_t count){
    if(begin >= count || end >= count){
        printf("Wrong params\n");
        return -1;
    }
    int rand = (end + 1) % count;
    if(rand == begin){
      printf("Smth goes wrong...\n");
      return -1;
    }
    double prod_p, prod_rand;
    struct point d_vec, p_vec, rand_vec;
    d_vec.x = vertices[end].x - vertices[begin].x;
    d_vec.y = vertices[end].y - vertices[begin].y;
    p_vec.x = p.x - vertices[begin].x;
    p_vec.y = p.y - vertices[begin].y;
    rand_vec.x = vertices[rand].x - vertices[begin].x;
    rand_vec.y = vertices[rand].y - vertices[begin].y;
    prod_p = d_vec.x * p_vec.y - p_vec.x * d_vec.y;
    prod_rand = d_vec.x * rand_vec.y - rand_vec.x * d_vec.y;
    if(prod_p * prod_rand > 0)
        return 1;
    else return 0; 
}

void Satherlend_Hodgman(const struct point* polygon_vertices, struct point* clipper_vertices){
    int i, j;
    buf_count = polygon_count;
    buffer = (struct point *)calloc(number_of_clipped_polygon_vertices, sizeof(struct point));
    memcpy(buffer, polygon_vertices, polygon_count * sizeof(struct point));
    for(i = 0; i < clipper_count; i++){
        for(j = 0; j < buf_count; j++){
            int vis_f = is_visible(clipper_vertices, buffer[j], i, (i + 1) % clipper_count, clipper_count);
            int vis_s = is_visible(clipper_vertices, buffer[(j + 1) % buf_count], i, (i + 1) % clipper_count, clipper_count);
            if(vis_f == -1 || vis_s == -1){
                printf("Error in is_visible!\n");
                return;
            }
            if(!vis_f && !vis_s)
                continue;
            else if(vis_f && vis_s){
                clipped_polygon_vertices[clipped_polygon_count] = buffer[j];

                clipped_polygon_count++;
                printf("added %d\n", j);
                continue;
            }
            struct point* intersection = intersect(buffer[j], buffer[(j + 1) % buf_count], clipper_vertices[i], clipper_vertices[(i + 1) % clipper_count]); 
            printf("point =  %p at  %d %d with %d %d\n", intersection, j, (j+1) % buf_count, i, (i+1) % clipper_count);
            printf("visibility = %d %d\n", vis_f, vis_s);
            if(!intersection){
                printf("Erorr in intersection!\n");
                return;
            }
            else if(vis_f && !vis_s){
                clipped_polygon_vertices[clipped_polygon_count] = buffer[j];
                printf("added %d\n", j);
                clipped_polygon_count++;
                clipped_polygon_vertices[clipped_polygon_count] = *intersection; 
                printf("added intersection %d %d with %d %d\n", j, (j+1) % buf_count, i, (i+1) % clipper_count); 
                clipped_polygon_count++;
            }
            else if(!vis_f && vis_s){
                clipped_polygon_vertices[clipped_polygon_count] = *intersection;
                printf("added intersection %d %d with %d %d\n", j, (j+1) % buf_count, i, (i+1) % clipper_count); 
                clipped_polygon_count++;
            }
            free(intersection);
        }
        buf_count = clipped_polygon_count;
        memcpy(buffer, clipped_polygon_vertices, clipped_polygon_count * sizeof(struct point));
        clipped_polygon_count = 0;
        memset(clipped_polygon_vertices, 0, sizeof(struct point));

    }
    return;
}

int belongs_to_segment(struct point s_seg, struct point e_seg, struct point unknown){
    double s_prod, ps_prod;
    struct point vec_s, vec_e, vec_dir;
    vec_s.x = s_seg.x - unknown.x;
    vec_s.y = s_seg.y - unknown.y;
    vec_e.x = e_seg.x - unknown.x;
    vec_e.y = e_seg.y - unknown.y;
    vec_dir.x = e_seg.x - s_seg.x;
    vec_dir.y = e_seg.y - s_seg.y;
    s_prod = vec_s.x * vec_e.x + vec_s.y * vec_e.y;
    ps_prod = vec_dir.x * (-vec_s.y) + vec_s.x * vec_dir.y;
    printf("prods = %f %f\n", ps_prod, s_prod);
    if((int)ps_prod == 0 && s_prod <= 0 )
        return 1;
    else return 0;
}

void draw_clipper(){
    glBegin(GL_LINE_LOOP);
    int i;
    for(i = 0; i < clipper_count; i++){
        glColor3f(1.f,1.f,1.f);
        glVertex2f(clipper_vertices[i].x, clipper_vertices[i].y);
    } 
    glEnd();
}

void draw_polygon(){
    glBegin(GL_LINE_LOOP);
    int i;
    for(i = 0; i < polygon_count; i++){
        glColor3f(0.f,0.f,1.f);
        glVertex2f(polygon_vertices[i].x, polygon_vertices[i].y);
    }
    glEnd();
}

void draw_clipped_polygon(){
    glBegin(GL_LINE_LOOP);
    int i;
    for(i = 0; i < polygon_count; i++){
        glColor3f(1.f,0.f,0.f);
        glVertex2f(polygon_vertices[i].x, polygon_vertices[i].y);
    }
    glEnd();
    glBegin(GL_LINE_LOOP);
    for(i = 0; i < buf_count; i++){
        glColor3f(0.f,0.f,1.f);
        glVertex2f(buffer[i].x , buffer[i].y);
    }
    glEnd();
    int j;
    for(i = 0; i < buf_count; i++){
        for(j = 0; j < clipper_count; j++){
            int belong_f = belongs_to_segment(clipper_vertices[j], clipper_vertices[(j+1) % clipper_count], buffer[i]);
            int belong_s = belongs_to_segment(clipper_vertices[j], clipper_vertices[(j+1) % clipper_count], buffer[(i+1) % buf_count]);
            printf("%d %d\n", belong_f, belong_s);
            if(belong_f && belong_s){
                glBegin(GL_LINES);
                glColor3f(1.f, 0.f, 0.f);
                glVertex2f(buffer[i].x, buffer[i].y);
                glVertex2f(buffer[(i+1) % buf_count].x, buffer[(i+1) % buf_count].y);
                glEnd();
            }
        }
    }
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
    free(buffer);
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
    
