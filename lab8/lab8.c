#include <GL/glew.h> 
#include <GLFW/glfw3.h> 
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <linmath.h>
#include "parametrs.h"

float ratio;
int prev_partition;
int  min_ind_f = 0,  min_ind_s = 0;
int down = 1;
float cur_scale = 1.f;
float cur_translation = 0.f;
float start_time = 0.f;

GLuint vao;
GLuint vbo;
GLuint shader_programme;

struct point {
	float x;
    float y;
    float z;
};


struct point* vertices;
struct point* normals;
struct point* drawable;
int count = 0;

struct scene scene;

void calcTorus() {
    int i,j;
    double phi = 2*M_PI/scene.torus_param.partition, theta = 2*M_PI/scene.torus_param.partition;
	for (i = 0; i <= scene.torus_param.partition; i++) {
		for (j = 0; j <= scene.torus_param.partition; j++) {
            vertices[i * scene.torus_param.partition + j].x = (scene.torus_param.torus_major + scene.torus_param.torus_minor * cos(theta*j)) * cos(phi*i);
            vertices[i * scene.torus_param.partition + j].y = (scene.torus_param.torus_major + scene.torus_param.torus_minor * cos(theta*j)) * sin(phi*i);
            vertices[i * scene.torus_param.partition + j].z = scene.torus_param.torus_minor * sin(theta*j);		
            normals[i * scene.torus_param.partition + j].x = -(cos(theta * j) * cos(phi * i));
            normals[i * scene.torus_param.partition + j].y = -(cos(theta * j) * sin(phi * i));
            normals[i * scene.torus_param.partition + j].z = -(sin(theta * j));
            count++;
            if(vertices[i * scene.torus_param.partition + j].y < vertices[min_ind_f * scene.torus_param.partition + min_ind_s].y){
                min_ind_f = i;
                min_ind_s = j;
            }
        }
	}
    return;
}

void form_array_for_drawing(){
    drawable = calloc(2 * count, sizeof(struct point));
    int i, j;
    int rep = 0;
    for(i = 0, j = 0;; i+= 2){
        if(j!= 1 && (j-1) % scene.torus_param.partition == 0){
            drawable[i] = vertices[j-1];
            j++;
            rep = 1;
            continue;
        }
        if(rep){
            rep = 0;
            j--;
        }
        drawable[i] = vertices[j];
        if(j == scene.torus_param.partition * (scene.torus_param.partition + 1)){
            break;
        }
        j++;
    }
    rep = 0;
    for(i = 1, j = scene.torus_param.partition; i < 2 * count; i += 2){
        if(j!= 1 && i != 3 && (j-1) % scene.torus_param.partition == 0 && j != (scene.torus_param.partition * scene.torus_param.partition + 1)){
            drawable[i] = vertices[j-1];
            j++;
            rep = 1; 
            continue;
        }
        if(rep){
            rep = 0;
            j--;
        }
        if(j == (scene.torus_param.partition * scene.torus_param.partition + 1)){
            j = 0;
        }
        drawable[i] = vertices[j];
        j++;
    }
}
	
int create_shaders () {
	const char* vertex_shader =
	"#version 130\n"
	"in vec3 vp;"
	"void main () {"
	"	gl_Position = vec4 (vp, 1.0);"
	"}";
	const char* fragment_shader =
	"#version 130\n"
	"out vec4 frag_colour;"
	"void main () {"
	"	frag_colour = vec4 (0.5, 0.0, 0.5, 1.0);"
	"}";
	GLuint vs, fs;
	
	if (!glfwInit ()) {
		fprintf (stderr, "ERROR: could not start GLFW3\n");
		return 1;
	} 

	glEnable (GL_DEPTH_TEST); 
	glDepthFunc (GL_LESS);
	glGenBuffers (1, &vbo);
	glBindBuffer (GL_ARRAY_BUFFER, vbo);
	glBufferData (GL_ARRAY_BUFFER, (scene.torus_param.partition + 1) * (scene.torus_param.partition + 1) * 6 * sizeof (GLfloat), drawable,
                  GL_STATIC_DRAW);
	
	glGenVertexArrays (1, &vao);
	glBindVertexArray (vao);
	glEnableVertexAttribArray (0);
	glBindBuffer (GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	
	vs = glCreateShader (GL_VERTEX_SHADER);
	glShaderSource (vs, 1, &vertex_shader, NULL);
	glCompileShader (vs);
	fs = glCreateShader (GL_FRAGMENT_SHADER);
	glShaderSource (fs, 1, &fragment_shader, NULL);
	glCompileShader (fs);
	shader_programme = glCreateProgram ();
	glAttachShader (shader_programme, fs);
	glAttachShader (shader_programme, vs);
	glLinkProgram (shader_programme);
    return 0;
}

void draw(GLFWwindow* window){
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram (shader_programme);
	glBindVertexArray (vao);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 2 * (scene.torus_param.partition + 1) * (scene.torus_param.partition + 1));
    glfwPollEvents ();
	glfwSwapBuffers (window);
}

void resizeTorus(){
    min_ind_f = min_ind_s = 0;
    if(vertices != NULL){
        free(vertices);
    }
    vertices = (struct point*)malloc((scene.torus_param.partition+1) * (scene.torus_param.partition+1)*sizeof(struct point));
    if(normals != NULL){
        free(normals);
    }
    normals = (struct point*)malloc((scene.torus_param.partition + 1) * (scene.torus_param.partition + 1) * sizeof(struct point));
}

void stop(GLFWwindow *window){
    glfwDestroyWindow(window);
    glfwTerminate();
    if(vertices)
        free(vertices);
    if(normals)
        free(normals);
    if(drawable)
        free(drawable);
}

GLFWwindow* init_window(){
    GLFWwindow* window;
    if (!glfwInit()){
        exit(EXIT_FAILURE);
    }
    window = glfwCreateWindow(1920, 1080, "lab8", NULL, NULL);
    if (!window){
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
	glewExperimental = GL_TRUE;
	glewInit ();
    return window;
}

void set_viewport(GLFWwindow* window, int width, int height){
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
}

void set_projection(int width, int height){
    ratio = width / (float) height;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-ratio, ratio, -1.f, 1.f, 5.f, -5.f);
    glRotatef(10, 1.0f, 0.f, 0.f);
    glRotatef(10, 0.f, 1.f, 0.f);
}

int main(void){
    scene = parse_params_file("params.txt");
    GLFWwindow* window = init_window();
    set_viewport(window, 1920, 1080);
    set_projection(1920, 1080);
    resizeTorus();
    calcTorus(); 
    form_array_for_drawing();
    create_shaders();
    while (!glfwWindowShouldClose(window)){
        draw(window);
    }
	glfwTerminate();
    stop(window);
    return 0;
}

