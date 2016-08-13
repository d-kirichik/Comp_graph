#include <GLFW/glfw3.h>
#include <linmath.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define TORUS_MAJOR 0.15
#define TORUS_MINOR 0.05
#define ANIMATION_SPEED 20.0f

float ratio;
float alpha = 75;
int partition = 32;
int prev_partition = 32;
int polygon_mode = GL_FALSE;
int animated = 0;
int down = 1;
int textured = 0;
vec3 rotation_axis = {0.f, 1.f, 0.f};
vec3 translation = {0.f, 0.f, 0.f};
vec3 scale = {1.f, 1.f, 1.f};
vec3 fall = {0.f, -0.1f, 0.f};

float lightcolor0[] = {0.5f, 0.2f, 0.2f, 1.f};
float lightpos0[] = {0.f, -1.f, 1.f, 0.f};
float lightcolor1[] = {0.2f, 0.2f, 1.f, 1.f};
float lightpos1[] = {0.f, 1.f, 1.f, 0.f};

const char *img_path = "ball.bmp";

struct point {
	float x;
    float y;
    float z;
};

struct img{
    unsigned char *data;
    unsigned int width;
    unsigned int height;
};

struct img img;
GLuint t_id;

struct point** vertices;
struct point** normals;
int  min_ind_f = 0, min_ind_s = 0;

int light0 = 0;
int light1 = 0;

float start_time = 0.f; 
float dt = 0.f;

float cur_scale = 1.f;
float cur_translation = 0.f;

void load_BMP(){
    unsigned char header[54];
    unsigned int data_pos;
    unsigned int width, height;
    unsigned int img_size;
    unsigned char *data;
    FILE *file = fopen(img_path, "rb");
    if(!file){
        printf("File not found\n");
        return;
    }
    if(fread(header, sizeof(char), 54, file) != 54){
        printf("Invalid file: too small\n");
        return;
    }
    if(header[0] != 'B' || header[1] != 'M'){
        printf("Invalid file: wrong header\n");
        return;
    }
    data_pos = *(int*) &(header[0x0A]);
    img_size = *(int*) &(header[0x22]);
    height = *(int*) &(header[0x12]);
    img.height = height;
    width = *(int*) &(header[0x16]);
    img.width = width;
    if(!img_size)
        img_size = width * height * 3;
    if(!data_pos)
        data_pos = 54;
    data = (unsigned char*) malloc (img_size * sizeof(unsigned char));
    fread(data, sizeof(unsigned char), img_size, file); 
    img.data = data;
    fclose(file);
}

GLuint load_texture(){
    GLuint t_id;
    glGenTextures(1, &t_id);
    glBindTexture(GL_TEXTURE_2D, t_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img.width, img.height, 0, GL_BGR, GL_UNSIGNED_BYTE, img.data);
    return t_id;
}

void calcTorus() {
    int i,j;
    double phi = 2*M_PI/partition, theta = 2*M_PI/partition;
	for (i = 0; i <= partition; i++) {
		for (j = 0; j <= partition; j++) {
            vertices[i][j].x = (TORUS_MAJOR + TORUS_MINOR * cos(theta*j)) * cos(phi*i);
            vertices[i][j].y = (TORUS_MAJOR + TORUS_MINOR * cos(theta*j)) * sin(phi*i);
            vertices[i][j].z = TORUS_MINOR * sin(theta*j);		
            normals[i][j].x = -(cos(theta * j) * cos(phi * i));
            normals[i][j].y = -(cos(theta * j) * sin(phi * i));
            normals[i][j].z = -(sin(theta * j));
            if(vertices[i][j].y < vertices[min_ind_f][min_ind_s].y){
                min_ind_f = i;
                min_ind_s = j;
            }
        }
	}
    return;
}

void drawTorus() {
	struct point first_vertex, second_vertex, first_normal, second_normal;
	int i,j;
	for (i = 0; i <= partition; i++) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBegin(GL_QUAD_STRIP);
		for (j = 0; j <= partition; j++) {
			first_vertex = vertices[i][j];
            first_normal = normals[i][j];
			second_vertex = vertices[(i + 1) % partition][j];
            second_normal = normals[(i + 1) % partition][j];
			glNormal3f(first_normal.x, first_normal.y, first_normal.z);
            if(textured){
                if((j % 2) == 0){
                    glTexCoord2f(0.f, 0.f);
                }
                else{
                    glTexCoord2f(1.f, 1.f);
                }
            }
            glVertex3f(first_vertex.x, first_vertex.y, first_vertex.z);
            glNormal3f(second_normal.x, second_normal.y, second_normal.z);
            if(textured){
                if((j % 2) == 0){
			        glTexCoord2f(1.0f, 0.f);
                }
                else{
                    glTexCoord2f(0.f, 1.f);
                }
            }
            glVertex3f(second_vertex.x, second_vertex.y, second_vertex.z);
		}
		glEnd();
	}
}

void resizeTorus(){
    int i;
    min_ind_f = min_ind_s = 0;
    if(vertices != NULL){
        for(i = 0; i < prev_partition+1; i++)
            free(vertices[i]);
        free(vertices);
    }
    vertices = (struct point**)malloc((partition+1)*sizeof(struct point*));
	for (i = 0; i < partition + 1; i++) 
		vertices[i] = (struct point*)malloc((partition+1)*sizeof(struct point));
    if(normals != NULL){
        for(i = 0; i < prev_partition + 1; i++)
            free(normals[i]);
        free(normals);
    }
    normals = (struct point**)malloc((partition + 1) * sizeof(struct point*));
    for(i = 0; i < partition + 1; i++)
        normals[i] = (struct point*) malloc((partition + 1) * sizeof(struct point));
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
        cur_scale *= 1.1f;
    }
    if(action == GLFW_PRESS && key == GLFW_KEY_M){
        scale[0] *= 0.9f;
        scale[1] *= 0.9f;
        scale[2] *= 0.9f;
        cur_scale *= 0.9f;
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
        translation[0] += 0.15f;
    }
    if(action == GLFW_PRESS && key == GLFW_KEY_LEFT){
        translation[0] -= 0.15f;
    }
    if(action == GLFW_PRESS && key == GLFW_KEY_UP){
        translation[1] += 0.15f;
        cur_translation += 0.15f;
    }
    if(action == GLFW_PRESS && key == GLFW_KEY_DOWN){
        translation[1] -= 0.15f;
        cur_translation -= 0.15f;
    }
    if(action == GLFW_PRESS && key == GLFW_KEY_1){
        if(light0 == 0){
            glLightfv(GL_LIGHT0, GL_DIFFUSE, lightcolor0);
            glLightfv(GL_LIGHT0, GL_POSITION, lightpos0);
            glEnable(GL_LIGHT0);
            light0 = 1;
        }
        else{
            glDisable(GL_LIGHT0);
            light0 = 0;
        }
    }
    if(action == GLFW_PRESS && key == GLFW_KEY_2){
        if(light1 == 0){
            glLightfv(GL_LIGHT1, GL_DIFFUSE, lightcolor1);
            glLightfv(GL_LIGHT1, GL_POSITION, lightpos1);
            glEnable(GL_LIGHT1);
            light1 = 1;
        }
        else{
            glDisable(GL_LIGHT1);
            light1 = 0;
        }
    }
    if(action == GLFW_PRESS && key == GLFW_KEY_A){
        start_time = (float) glfwGetTime();
        animated = 1;
    }
    if(action == GLFW_PRESS && key == GLFW_KEY_X){
        load_BMP();
        t_id = load_texture();
        if(textured == 0){
            textured = 1;
            glEnable(GL_TEXTURE_2D);
        }
        else{
            glDisable(GL_TEXTURE_2D);
            textured = 0;
        }
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
    window = glfwCreateWindow(1920, 1080, "lab6", NULL, NULL);
    if (!window){
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE); 
    float amb_color[] = {0.2f, 0.2f, 0.2f, 1.f};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb_color);
    glfwSwapInterval(1);
    return window;
}

int detect_collision(float bias){
    if(vertices[min_ind_f][min_ind_s].y * cur_scale + bias + cur_translation < -1.0f){
        return 1;
    }
    else return 0;
}

void draw(GLFWwindow *window){
    if (polygon_mode == GL_TRUE){
        glEnable(GL_CULL_FACE); 
	    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
	else{
        glDisable(GL_CULL_FACE);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);   
    }
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPushMatrix(); 
    if(animated == 1){
        if(down){
            if(dt < 0)
                dt = 0;
            dt += ((float) glfwGetTime() - start_time)/ANIMATION_SPEED;
        }
        if(!down){
            dt -= ((float) glfwGetTime() - start_time)/ANIMATION_SPEED;
        }
        if(detect_collision(dt * fall[1])){
            down = 0;
        }
        if(dt < 0){
            start_time = glfwGetTime();
            down = 1;
        }
        glTranslatef(fall[0], dt * fall[1], fall[2]);
    }
    if(textured == 1){
        glBindTexture(GL_TEXTURE_2D, t_id);
    }
    glTranslatef(translation[0], translation[1], translation[2]);
    glRotatef(alpha,rotation_axis[0],rotation_axis[1], rotation_axis[2]);
    glScalef(scale[0], scale[1], scale[2]);
    drawTorus();
    glPopMatrix();
    glfwSwapBuffers(window);
    glfwPollEvents();
}

void stop(GLFWwindow *window){
    glfwDestroyWindow(window);
    glfwTerminate();
    if(vertices)
        free(vertices);
    if(normals)
        free(normals);
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
    glOrtho(-ratio, ratio, -1.f, 1.f, 5.f, -5.f);
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
