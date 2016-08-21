#include <GL/glew.h> 
#include <GLFW/glfw3.h> 
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <linmath.h>
#include "parametrs.h"

float ratio;
int prev_partition;

GLuint vao, vao_c;
GLuint vbo, vbo_c;
GLuint shader_prog;
GLuint vs, fs;

struct point {
	float x;
    float y;
    float z;
};


struct point* vertices;
struct point* normals;
struct point* drawable;
struct point* cube;
int count = 0;

struct scene scene;



static float cube_vertices[] = {-1.15f, -0.65f, -0.15f,
                                -1.15f, -0.35f, -0.15f,
                                -0.85f, -0.35f, -0.15f,
                                -0.85f, -0.65f, -0.15f, 
                                -1.15f, -0.65f, -0.15f,
                                -0.85f, -0.65f, -0.15f,
                                -0.85f, -0.65f, 0.15f,
                                -1.15f, -0.65f, 0.15f,
                                -1.15f, -0.65f, -0.15f,
                                -1.15f, -0.65f, 0.15f,
                                -1.15f, -0.35f, 0.15f,
                                -1.15f, -0.35f, -0.15f,
                                -1.15f, -0.65f, 0.15f,
                                -0.85f, -0.65f, 0.15f,
                                -0.85f, -0.35f, 0.15f,
                                -1.15f, -0.35f, 0.15f,
                                -1.15f, -0.35f, -0.15f,
                                -1.15f, -0.35f, 0.15f, 
                                -0.85f, -0.35f, 0.15f,
                                -0.85f, -0.35f, -0.15f,
                                -0.85f, -0.65f, -0.15f,
                                -0.85f, -0.35f, -0.15f,
                                -0.85f, -0.35f, 0.15f,
                                -0.85f, -0.65f, 0.15f 
};

char* file_to_buf(const char *file){
    FILE *fptr;
    long length;
    char *buf;
    fptr = fopen(file, "rb"); 
    if (!fptr){ 
        printf("File not found\n");
        return NULL;
    }
    fseek(fptr, 0, SEEK_END);
    length = ftell(fptr); 
    buf = malloc(length+1);
    fseek(fptr, 0, SEEK_SET);
    fread(buf, length, 1, fptr);
    fclose(fptr);
    buf[length] = 0;
    return buf;
}

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

int create_torus_shaders () {
	const char* vertex_shader = file_to_buf("vert_shader_torus.glsl");
	const char* fragment_shader = file_to_buf("frag_shader_torus.glsl");
    char buffer[512];	
    
	GLint status;

	vs = glCreateShader (GL_VERTEX_SHADER);
	glShaderSource (vs, 1, &vertex_shader, NULL);
	glCompileShader (vs);
    glGetShaderiv(vs, GL_COMPILE_STATUS, &status);    
    glGetShaderInfoLog(vs, 512, NULL, buffer);

	fs = glCreateShader (GL_FRAGMENT_SHADER);
	glShaderSource (fs, 1, &fragment_shader, NULL);
	glCompileShader (fs);
    glGetShaderiv(fs, GL_COMPILE_STATUS, &status);    
    glGetShaderInfoLog(fs, 512, NULL, buffer);
	
    shader_prog = glCreateProgram ();
	glAttachShader (shader_prog, fs);
	glAttachShader (shader_prog, vs);
	glLinkProgram (shader_prog);
    return 0;
}


void create_vbo(){
	glGenBuffers (1, &vbo);
	glBindBuffer (GL_ARRAY_BUFFER, vbo);
	glBufferData (GL_ARRAY_BUFFER, (scene.torus_param.partition + 1) * (scene.torus_param.partition + 1) * 6 * sizeof (GLfloat), drawable, GL_STATIC_DRAW);

	glGenBuffers (1, &vbo_c);
	glBindBuffer (GL_ARRAY_BUFFER, vbo_c);
	glBufferData (GL_ARRAY_BUFFER, 24 * 3 * sizeof (GLfloat), cube_vertices, GL_STATIC_DRAW);
}

void create_vao(){

	glGenVertexArrays (1, &vao);
	glBindVertexArray (vao);
	glEnableVertexAttribArray (0);
	glBindBuffer (GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	
	glGenVertexArrays (1, &vao_c);
	glBindVertexArray (vao_c);
	glEnableVertexAttribArray (0);
	glBindBuffer (GL_ARRAY_BUFFER, vbo_c);
	glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	
    glBindVertexArray(0);
}

void draw(GLFWwindow* window){
    if (scene.torus_param.polygon_mode == GL_TRUE){
	    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
	else{
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);   
    }
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPushMatrix(); 
    glTranslatef(scene.transform_param.translation[0], scene.transform_param.translation[1], scene.transform_param.translation[2]);
    glRotatef(scene.transform_param.alpha,scene.transform_param.rotation_axis[0],scene.transform_param.rotation_axis[1], scene.transform_param.rotation_axis[2]);
    glScalef(scene.transform_param.scale[0], scene.transform_param.scale[1], scene.transform_param.scale[2]);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram (shader_prog);
	glBindVertexArray (vao);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 2 * (scene.torus_param.partition + 1) * (scene.torus_param.partition + 1));
    glPopMatrix();    
	glBindVertexArray(0);
    glBindVertexArray(vao_c);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 24);
    glBindVertexArray(0);
    glUseProgram(0);
	glfwPollEvents ();
	glfwSwapBuffers (window);
}

void resizeTorus(){
    if(vertices != NULL){
        free(vertices);
    }
    vertices = (struct point*)malloc((scene.torus_param.partition+1) * (scene.torus_param.partition+1)*sizeof(struct point));
    if(normals != NULL){
        free(normals);
    }
    normals = (struct point*)malloc((scene.torus_param.partition + 1) * (scene.torus_param.partition + 1) * sizeof(struct point));
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
   if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if(action == GLFW_PRESS && key == GLFW_KEY_R)
        scene.transform_param.alpha += 15;
    if(action == GLFW_PRESS && key == GLFW_KEY_T)
        scene.transform_param.alpha -= 15;
    if(action == GLFW_PRESS && key == GLFW_KEY_L){
        scene.transform_param.scale[0] *= 1.1f;
        scene.transform_param.scale[1] *= 1.1f;
        scene.transform_param.scale[2] *= 1.1f;
    }
    if(action == GLFW_PRESS && key == GLFW_KEY_M){
        scene.transform_param.scale[0] *= 0.9f;
        scene.transform_param.scale[1] *= 0.9f;
        scene.transform_param.scale[2] *= 0.9f;
    }
    if(action == GLFW_PRESS && key == GLFW_KEY_C){
        scene.torus_param.polygon_mode = !scene.torus_param.polygon_mode;
    } 
    if(action == GLFW_PRESS && key == GLFW_KEY_P){
        prev_partition = scene.torus_param.partition;
        scene.torus_param.partition /= 2;
        resizeTorus();
        calcTorus();
        form_array_for_drawing();
        create_vbo();
        create_vao();
    }
    if(action == GLFW_PRESS && key == GLFW_KEY_O){
        prev_partition = scene.torus_param.partition;
        scene.torus_param.partition *= 2;
        resizeTorus();
        calcTorus();
        form_array_for_drawing();
        create_vbo();
        create_vao();
    }
    if(action == GLFW_PRESS && key == GLFW_KEY_RIGHT){
        scene.transform_param.translation[0] += 0.15f;
    }
    if(action == GLFW_PRESS && key == GLFW_KEY_LEFT){
        scene.transform_param.translation[0] -= 0.15f;
    }
    if(action == GLFW_PRESS && key == GLFW_KEY_UP){
        scene.transform_param.translation[1] += 0.15f;
    }
    if(action == GLFW_PRESS && key == GLFW_KEY_DOWN){
        scene.transform_param.translation[1] -= 0.15f;
    }
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
    glDeleteProgram(shader_prog);
    glDeleteShader(vs);
    glDeleteShader(fs);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
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
    glfwSetKeyCallback(window, key_callback);
    set_viewport(window, 1920, 1080);
    set_projection(1920, 1080);
    resizeTorus();
    calcTorus(); 
    form_array_for_drawing();
    create_vbo();
    create_vao();
    create_torus_shaders();
    while (!glfwWindowShouldClose(window)){
        draw(window);
    }
	glfwTerminate();
    stop(window);
    return 0;
}

