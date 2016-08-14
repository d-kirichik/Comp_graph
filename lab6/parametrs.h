#include <stdio.h>
#include <malloc.h>
#include <ctype.h>
#include <string.h>

struct light_param{
    float lightcolor0[4]; // {0.5f, 0.2f, 0.2f, 1.f};
    float lightpos0[4]; //{0.f, -1.f, 1.f, 0.f};
    float lightcolor1[4]; //{0.2f, 0.2f, 1.f, 1.f};
    float lightpos1[4]; //{0.f, 1.f, 1.f, 0.f};
    int light0; //0;
    int light1; //0;
};

struct torus_param{
    int partition; 
    float torus_major;
    float torus_minor;
    int polygon_mode;
};

struct tex_param{
    char img_path[100];
    int textured;//0
};

struct anima_param{
    vec3 fall;// {0.f, -0.1f, 0.f};
    int animated;//0
    float dt;// 0.f;
    float animation_speed;//20.0f;
};

struct transform_param{
    vec3 rotation_axis;// {0.f, 1.f, 0.f};
    vec3 translation;// {0.f, 0.f, 0.f};
    vec3 scale;// {1.f, 1.f, 1.f};
    float alpha;// 75;
};

struct scene{
    struct light_param light_param;
    struct torus_param torus_param;
    struct tex_param tex_param;
    struct anima_param anima_param;
    struct transform_param transform_param;
};

size_t get_next_int(char* data, const char* offset, int* field){
    char *temp = data;
    for(; *data != '\t'; data++);
    data += strlen(offset);
    *field = atoi(data);
    return data - temp;
} 

size_t get_next_float(char* data, const char* offset, float* field){
    char *temp = data;
    for(; *data != '\t'; data++);
    data += strlen(offset);
    *field = atof(data);
    return data - temp;
}

size_t get_array(char *data, float* array, size_t size){
    char *temp = data;
    int i;  
    printf("\n%s\n", data);
    array[0] = atof(data);
    for(i = 0; i < size; i++){
        for(; *data != ','; data++);
        array[i+1] = atof(data += 2);
    }
    return data - temp;
}

size_t get_str(char *data, char* field){
    char *temp = data;
    int i;
    for(; *data != '"'; data++);
    data++;
    for(i = 0; *data != '"'; data++, i++){
        field[i] = *data;
    }
    field[i++] = '\0';
    return data - temp;
}

struct scene parse_params_file(const char *name){
    struct scene scene;
    FILE* file = fopen(name, "rb");
    long file_size;
    char* data, *temp;
    int bias;
    if(!file){
        printf("File not found\n");
    }
    fseek(file, 0L, SEEK_END);
    file_size = ftell(file);
    rewind(file);
    data = (char*) malloc((file_size + 1) * sizeof(char));
    temp = data;
    fread(data, sizeof(char), file_size, file);
    bias = get_next_int(data, "partition = ", &scene.torus_param.partition);
    data += bias;
    bias = get_next_float(data, "torus_major = ", &scene.torus_param.torus_major); 
    data += bias;
    bias = get_next_float(data, "torus_minor = ", &scene.torus_param.torus_minor); 
    data += bias;
    bias = get_next_int(data, "polygon_mode =", &scene.torus_param.polygon_mode); 
    data += bias;
    for(; *data != '\n'; data++);
    data += sizeof("\nlight_param:\n\tlightcolor0 =");
    get_array(data, scene.light_param.lightcolor0, 4);
    for(; *data != '\t'; data++);
    data += sizeof("lightpos0 = ");
    get_array(data, scene.light_param.lightpos0, 4);
    for(; *data != '\t'; data++);
    data += sizeof("lightcolor1 = ");
    get_array(data, scene.light_param.lightcolor1, 4);
    data += sizeof("lightpos1 = ");
    get_array(data, scene.light_param.lightpos1, 4);
    bias = get_next_int(data, "light0 = ", &scene.light_param.light0);
    data += bias;
    bias = get_next_int(data, "light1 = ", &scene.light_param.light1);
    data += bias;
    for(; *data != '\n'; data++);
    data += sizeof("\ntex_param:\n\timg_path =");
    bias = get_str(data, &scene.tex_param.img_path[0]);
    data += bias;
    bias = get_next_int(data, "textured = ", &scene.tex_param.textured);
    data += bias;
    data += sizeof("\nanima_param:\n\tfall =");
    bias = get_array(data, scene.anima_param.fall, 3);
    bias = get_next_int(data,"animated = ", &scene.anima_param.animated);
    data += bias;
    bias = get_next_float(data, "dt = ", &scene.anima_param.dt);
    data += bias;
    bias = get_next_float(data, "annimattion_speed = ", &scene.anima_param.animation_speed);
    for(; *data != '\n'; data++);
    data += sizeof("\ntransform_param:\n\trotation_axis =");
    bias = get_array(data, scene.transform_param.rotation_axis, 3);
    bias = get_array(data, scene.transform_param.translation, 3);
    data += bias;
    data += 14;
    bias = get_array(data, scene.transform_param.scale, 3);
    printf("t_param = %f\n", scene.transform_param.translation[0]);
    get_next_float(data, "alpha = ",  &scene.transform_param.alpha);
    free(temp);
    fclose(file);
    return scene;
}











