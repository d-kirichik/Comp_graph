/******************************************************************************\
| OpenGL 4 Example Code.                                                       |
| Accompanies written series "Anton's OpenGL 4 Tutorials"                      |
| Email: anton at antongerdelan dot net                                        |
| First version 27 Jan 2014                                                    |
| Copyright Dr Anton Gerdelan, Trinity College Dublin, Ireland.                |
| See individual libraries for separate legal notices                          |
|******************************************************************************|
| "Hello Triangle". Just the basics.                                           |
| If you're on Mac un-comment the version number code at the beginning. It     |
| will give you the latest, even if you say 3.2!                               |
| This uses the libraries GLEW and GLFW3 to start GL. Download and compile     |
| these first. Linking them might be a pain, but you'll need to master this.   |
|                                                                              |
| I wrote this so that it compiles in pedantic ISO C90, to show that it's      |
| easily done. I usually use minimalist C++ though, for tidier-looking maths   |
| functions.                                                                   |
\******************************************************************************/
#include <GL/glew.h> /* include GLEW and new version of GL on Windows */
#include <GLFW/glfw3.h> /* GLFW helper library */
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
	/* geometry to use. these are 3 xyz points (9 floats total) to make a
     triangle */

	/* these are the strings of code for the shaders
	the vertex shader positions each vertex point */
	const char* vertex_shader =
	"#version 130\n"
	"in vec3 vp;"
	"void main () {"
	"	gl_Position = vec4 (vp, 1.0);"
	"}";
	/* the fragment shader colours each fragment (pixel-sized area of the
	triangle) */
	const char* fragment_shader =
	"#version 130\n"
	"out vec4 frag_colour;"
	"void main () {"
	"	frag_colour = vec4 (0.5, 0.0, 0.5, 1.0);"
	"}";
	/* GL shader objects for vertex and fragment shader [components] */
	GLuint vs, fs;
	/* GL shader programme object [combined, to link] */
	
	/* start GL context and O/S window using the GLFW helper library */
	if (!glfwInit ()) {
		fprintf (stderr, "ERROR: could not start GLFW3\n");
		return 1;
	} 

	/* tell GL to only draw onto a pixel if the shape is closer to the viewer
     */
	glEnable (GL_DEPTH_TEST); /* enable depth-testing */
    /* with LESS depth-testing interprets a smaller value as "closer" */
	glDepthFunc (GL_LESS);
	/* a vertex buffer object (VBO) is created here. this stores an array of
     data on the graphics adapter's memory. in our case - the vertex points */
	glGenBuffers (1, &vbo);
	glBindBuffer (GL_ARRAY_BUFFER, vbo);
	glBufferData (GL_ARRAY_BUFFER, (scene.torus_param.partition + 1) * (scene.torus_param.partition + 1) * 6 * sizeof (GLfloat), drawable,
                  GL_STATIC_DRAW);
	
	/* the vertex array object (VAO) is a little descriptor that defines which
	data from vertex buffer objects should be used as input variables to vertex
	shaders. in our case - use our only VBO, and say 'every three floats is a 
	variable' */
	glGenVertexArrays (1, &vao);
	glBindVertexArray (vao);
	glEnableVertexAttribArray (0);
	glBindBuffer (GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	
	/* here we copy the shader strings into GL shaders, and compile them. we
     then create an executable shader 'program' and attach both of the compiled
     shaders. we link this, which matches the outputs of the vertex shader to
     the inputs of the fragment shader, etc. and it is then ready to use */
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
		/* draw points 0-3 from the currently bound VAO with current in-use
         shader */
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 2 * (scene.torus_param.partition + 1) * (scene.torus_param.partition + 1));
		/* update other events like input handling */
    glfwPollEvents ();
		/* put the stuff we've been drawing onto the display */
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

