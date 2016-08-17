#include <GLFW/glfw3.h>
#include <linmath.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "parametrs.h"

float ratio;
int prev_partition;
int  min_ind_f = 0,  min_ind_s = 0;
int down = 1;
float cur_scale = 1.f;
float cur_translation = 0.f;
float start_time = 0.f;
int intensity = 0;

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

struct scene scene;

void load_BMP(){
    unsigned char header[54];
    unsigned int data_pos;
    unsigned int width, height;
    unsigned int img_size;
    unsigned char *data;
    FILE *file = fopen(scene.tex_param.img_path, "rb");
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
    double phi = 2*M_PI/scene.torus_param.partition, theta = 2*M_PI/scene.torus_param.partition;
	for (i = 0; i <= scene.torus_param.partition; i++) {
		for (j = 0; j <= scene.torus_param.partition; j++) {
            vertices[i][j].x = (scene.torus_param.torus_major + scene.torus_param.torus_minor * cos(theta*j)) * cos(phi*i);
            vertices[i][j].y = (scene.torus_param.torus_major + scene.torus_param.torus_minor * cos(theta*j)) * sin(phi*i);
            vertices[i][j].z = scene.torus_param.torus_minor * sin(theta*j);		
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
	for (i = 0; i <= scene.torus_param.partition; i++) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBegin(GL_QUAD_STRIP);
		for (j = 0; j <= scene.torus_param.partition; j++) {
			first_vertex = vertices[i][j];
            first_normal = normals[i][j];
			second_vertex = vertices[(i + 1) % scene.torus_param.partition][j];
            second_normal = normals[(i + 1) % scene.torus_param.partition][j];
			glNormal3f(first_normal.x, first_normal.y, first_normal.z);
            if(scene.tex_param.textured){
                if((j % 2) == 0){
                    glTexCoord2f(0.f, 0.f);
                }
                else{
                    glTexCoord2f(1.f, 1.f);
                }
            }
            glVertex3f(first_vertex.x, first_vertex.y, first_vertex.z);
            glNormal3f(second_normal.x, second_normal.y, second_normal.z);
            if(scene.tex_param.textured){
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

void drawCube(float center_x, float center_y, float center_z){
    
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
    glVertex3f(center_x + 0.15f, center_y-0.15f, center_z-0.15f);
    glVertex3f(center_x + 0.15f, center_y + 0.15f, center_z-0.15f);
    glVertex3f(center_x + 0.15f, center_y + 0.15f, center_z + 0.15f);
    glVertex3f(center_x + 0.15f, center_y-0.15f, center_z + 0.15f);
    glEnd();
 
    glFlush();
}

void resizeTorus(){
    int i;
    min_ind_f = min_ind_s = 0;
    if(vertices != NULL){
        for(i = 0; i < prev_partition+1; i++)
            free(vertices[i]);
        free(vertices);
    }
    vertices = (struct point**)malloc((scene.torus_param.partition+1)*sizeof(struct point*));
	for (i = 0; i < scene.torus_param.partition + 1; i++) 
		vertices[i] = (struct point*)malloc((scene.torus_param.partition+1)*sizeof(struct point));
    if(normals != NULL){
        for(i = 0; i < prev_partition + 1; i++)
            free(normals[i]);
        free(normals);
    }
    normals = (struct point**)malloc((scene.torus_param.partition + 1) * sizeof(struct point*));
    for(i = 0; i < scene.torus_param.partition + 1; i++)
        normals[i] = (struct point*) malloc((scene.torus_param.partition + 1) * sizeof(struct point));
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
        cur_scale *= 1.1f;
    }
    if(action == GLFW_PRESS && key == GLFW_KEY_M){
        scene.transform_param.scale[0] *= 0.9f;
        scene.transform_param.scale[1] *= 0.9f;
        scene.transform_param.scale[2] *= 0.9f;
        cur_scale *= 0.9f;
    }
    if(action == GLFW_PRESS && key == GLFW_KEY_C){
        scene.torus_param.polygon_mode = !scene.torus_param.polygon_mode;
    } 
    if(action == GLFW_PRESS && key == GLFW_KEY_P){
        prev_partition = scene.torus_param.partition;
        scene.torus_param.partition /= 2;
        resizeTorus();
        calcTorus();
    }
    if(action == GLFW_PRESS && key == GLFW_KEY_O){
        prev_partition = scene.torus_param.partition;
        scene.torus_param.partition *= 2;
        resizeTorus();
        calcTorus();
    }
    if(action == GLFW_PRESS && key == GLFW_KEY_RIGHT){
        scene.transform_param.translation[0] += 0.15f;
    }
    if(action == GLFW_PRESS && key == GLFW_KEY_LEFT){
        scene.transform_param.translation[0] -= 0.15f;
    }
    if(action == GLFW_PRESS && key == GLFW_KEY_UP){
        scene.transform_param.translation[1] += 0.15f;
        cur_translation += 0.15f;
    }
    if(action == GLFW_PRESS && key == GLFW_KEY_DOWN){
        scene.transform_param.translation[1] -= 0.15f;
        cur_translation -= 0.15f;
    }
    if(action == GLFW_PRESS && key == GLFW_KEY_1){
        if(scene.light_param.light0 == 0){
            glLightfv(GL_LIGHT0, GL_DIFFUSE, scene.light_param.lightcolor0);
            glLightfv(GL_LIGHT0, GL_POSITION, scene.light_param.lightpos0);
            glEnable(GL_LIGHT0);
            scene.light_param.light0 = 1;
        }
        else{
            glDisable(GL_LIGHT0);
            scene.light_param.light0 = 0;
        }
    }
    if(action == GLFW_PRESS && key == GLFW_KEY_2){
        if(scene.light_param.light1 == 0){
            glLightfv(GL_LIGHT1, GL_DIFFUSE, scene.light_param.lightcolor1);
            glLightfv(GL_LIGHT1, GL_POSITION, scene.light_param.lightpos1);
            glEnable(GL_LIGHT1);
            scene.light_param.light1 = 1;
        }
        else{
            glDisable(GL_LIGHT1);
            scene.light_param.light1 = 0;
        }
    }
    if(action == GLFW_PRESS && key == GLFW_KEY_A){
        if(!scene.anima_param.animated){
            start_time = (float) glfwGetTime();
            scene.anima_param.animated = 1;
        }
        else
            scene.anima_param.animated = 0;
    }
    if(action == GLFW_PRESS && key == GLFW_KEY_X){
        load_BMP();
        t_id = load_texture();
        if(scene.tex_param.textured == 0){
            scene.tex_param.textured = 1;
        }
        else{
            scene.tex_param.textured = 0;
        }
    }
    if(action == GLFW_PRESS && key == GLFW_KEY_I){
        if(scene.tex_param.textured){
            intensity = !intensity;
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
    if (scene.torus_param.polygon_mode == GL_TRUE){
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
    if(scene.anima_param.animated == 1){
        if(down){
            if(scene.anima_param.dt < 0)
                scene.anima_param.dt = 0;
            scene.anima_param.dt += ((float) glfwGetTime() - start_time)/scene.anima_param.animation_speed;
        }
        if(!down){
            scene.anima_param.dt -= ((float) glfwGetTime() - start_time)/scene.anima_param.animation_speed;
        }
        if(detect_collision(scene.anima_param.dt * scene.anima_param.fall[1])){
            down = 0;
        }
        if(scene.anima_param.dt < 0){
            start_time = glfwGetTime();
            down = 1;
        }
        glTranslatef(scene.anima_param.fall[0], scene.anima_param.dt * scene.anima_param.fall[1], scene.anima_param.fall[2]);
    }
    if(scene.tex_param.textured == 1){
        glEnable(GL_TEXTURE_2D);
        if(intensity){
            float emission[4] = {0.25f, 0.25, 0.25f, 1.f};
            glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emission);
        }
    }
    glTranslatef(scene.transform_param.translation[0], scene.transform_param.translation[1], scene.transform_param.translation[2]);
    glRotatef(scene.transform_param.alpha,scene.transform_param.rotation_axis[0],scene.transform_param.rotation_axis[1], scene.transform_param.rotation_axis[2]);
    glScalef(scene.transform_param.scale[0], scene.transform_param.scale[1], scene.transform_param.scale[2]);
    drawTorus();
    glPopMatrix();
    if(scene.tex_param.textured){
        glDisable(GL_TEXTURE_2D);
    }
    if(intensity){
        float emission[4] = {0.f, 0.f, 0.f, 1.f};
        glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emission);
    }
    drawCube(-1,-0.5f,0);
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
    glRotatef(10, 1.0f, 0.f, 0.f);
    glRotatef(10, 0.f, 1.f, 0.f);
}


int main(void){
    scene = parse_params_file("params.txt");
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
