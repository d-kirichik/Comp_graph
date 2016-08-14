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

struct scene parse_params_file(const char *name){
    struct scene scene;
    FILE* file = fopen(name, "rb");
    if(!file){
        printf("File not found\n");
    }
    fseek(file, sizeof("torus_params:"), SEEK_SET);    
    fscanf(file, "%d\n", &scene.torus_param.partition);
    fseek(file, sizeof("//partition"), SEEK_CUR);
    fscanf(file, "%f\n", &scene.torus_param.torus_major);
    fseek(file, sizeof("//torus_major"), SEEK_CUR);
    fscanf(file, "%f\n", &scene.torus_param.torus_minor);
    fseek(file, sizeof("//torus_minor"), SEEK_CUR);
    fscanf(file, "%d\n", &scene.torus_param.polygon_mode);
    fseek(file, sizeof("//polygon_mode\nlight_param:"), SEEK_CUR);    
    fscanf(file, "%f %f %f %f\n", &scene.light_param.lightcolor0[0], &scene.light_param.lightcolor0[1], &scene.light_param.lightcolor0[2], &scene.light_param.lightcolor0[3]);
    fseek(file, sizeof("//lightcolor0"), SEEK_CUR);
    fscanf(file, "%f %f %f %f\n", &scene.light_param.lightpos0[0], &scene.light_param.lightpos0[1], &scene.light_param.lightpos0[2], &scene.light_param.lightpos0[3]);
    fseek(file, sizeof("//lightpos0"), SEEK_CUR);
    fscanf(file, "%f %f %f %f\n", &scene.light_param.lightcolor1[0], &scene.light_param.lightcolor1[1], &scene.light_param.lightcolor1[2], &scene.light_param.lightcolor1[3]);
    fseek(file, sizeof("//lightcolor1"), SEEK_CUR);
    fscanf(file, "%f %f %f %f\n", &scene.light_param.lightpos1[0], &scene.light_param.lightpos1[1], &scene.light_param.lightpos1[2], &scene.light_param.lightpos1[3]);
    fseek(file, sizeof("//lightpos1"), SEEK_CUR);
    fscanf(file, "%d\n", &scene.light_param.light0);
    fseek(file, sizeof("//light0"), SEEK_CUR);
    fscanf(file, "%d\n", &scene.light_param.light1);
    fseek(file, sizeof("//light1\ntex_param:\n"), SEEK_CUR);    
    fscanf(file, "%s\n", &scene.tex_param.img_path[0]);
    fseek(file, sizeof("//path to image"), SEEK_CUR);
    fscanf(file, "%d\n", &scene.tex_param.textured);
    fseek(file, sizeof("//textured\nanima_param:"), SEEK_CUR);    
    fscanf(file, "%f %f %f %f\n", &scene.anima_param.fall[0], &scene.anima_param.fall[1], &scene.anima_param.fall[2], &scene.anima_param.fall[3]);
    fseek(file, sizeof("//fall"), SEEK_CUR);
    fscanf(file, "%d\n", &scene.anima_param.animated);
    fseek(file, sizeof("//animated"), SEEK_CUR);
    fscanf(file, "%f\n", &scene.anima_param.dt);
    fseek(file, sizeof("//dt"), SEEK_CUR);
    fscanf(file, "%f\n", &scene.anima_param.animation_speed);
    fseek(file, sizeof("//animation_speed\ntransform_param:"), SEEK_CUR);    
    fscanf(file, "%f %f %f\n", &scene.transform_param.rotation_axis[0], &scene.transform_param.rotation_axis[1], &scene.transform_param.rotation_axis[2]);
    fseek(file, sizeof("//rotation_axis"), SEEK_CUR);
    fscanf(file, "%f %f %f\n", &scene.transform_param.translation[0], &scene.transform_param.translation[1], &scene.transform_param.translation[2]);
    fseek(file, sizeof("//translation"), SEEK_CUR);
    fscanf(file, "%f %f %f\n", &scene.transform_param.scale[0], &scene.transform_param.scale[1], &scene.transform_param.scale[2]);
    fseek(file, sizeof("//scale"), SEEK_CUR);
    fscanf(file, "%f\n", &scene.transform_param.alpha);
    return scene;
}
