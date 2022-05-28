//
// Created by hgallegos on 20/05/2022.
//

#ifndef LEARNOPENGL_OBJETO_H
#define LEARNOPENGL_OBJETO_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include "shader_m.h"
class BoundingVolume;

using namespace std;
using namespace glm;

class Objeto {
public:
    vector<vec3> positions;
    vector<vec3> normals;
    vector<vec2> textureCoords;
    vector<GLuint> indices;
    GLuint indices_size;
    GLuint vao;
    mat4 model;
    bool visible=true;
    BoundingVolume *bv;
    GLint POSITION_ATTRIBUTE=0, NORMAL_ATTRIBUTE=1, TEXCOORD0_ATTRIBUTE=8;

    vec3 vel_ini, pos_ini;
    float ang_ini;

    virtual GLuint setup()=0;
    virtual void display(Shader &sh)=0;
    virtual void actualizarDatos(float t)=0;
    virtual void calcularColision(vector<Objeto*> pObjetos)=0;
};

class Esfera:public Objeto{
public:
    vec3 centro;
    float radius;
    int slices, stacks;
    Esfera() {
        centro = vec3(0.0);
    }
    Esfera(vec3 _centro) {
        centro = _centro;
    }
    Esfera(vec3 _centro, float _radius, int _slices, int _stacks) {
        centro = _centro;
        radius = _radius;
        slices = _slices;
        stacks = _stacks;
    }
    GLuint setup();
    void display(Shader &sh);
    void actualizarDatos(float t);
    void calcularColision(vector<Objeto*> pObjetos);
};

class Caja : public Objeto {
public:
    vec3 posmin, posmax;
    Caja() {
        posmin = vec3(0.0);
        posmax = vec3(1.0);
    }
    GLuint setup();
    void display(Shader &sh);
    void actualizarDatos(float t);
};

#endif //LEARNOPENGL_OBJETO_H
