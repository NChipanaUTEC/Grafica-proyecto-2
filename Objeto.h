#ifndef LEARNOPENGL_OBJETO_H
#define LEARNOPENGL_OBJETO_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include "shader_m.h"

using namespace std;
using namespace glm;

struct BoundingBox {
    vec3 min, max;
    bool Colision(BoundingBox &bv) {
        return (min.x <= bv.max.x && max.x >= bv.min.x) &&
               (min.y <= bv.max.y && max.y >= bv.min.y) &&
               (min.z <= bv.max.z && max.z >= bv.min.z);
    }
};

class Objeto {
public:
    vector<vec3> positions;
    vector<vec3> normals;
    vector<vec2> textureCoords;
    vector<GLuint> indices;
    GLuint indices_size;
    GLuint vao;
    GLuint vbo;
    mat4 model;
    bool visible=true;
    bool afectaGravedad=true;
    BoundingBox *bb;
    GLint POSITION_ATTRIBUTE=0, NORMAL_ATTRIBUTE=1, TEXCOORD0_ATTRIBUTE=8;

    vec3 vel_ini, pos_ini;
    float ang_ini;

    virtual GLuint setup()=0;
    virtual void display(Shader &sh)=0;
    virtual void actualizarDatos(float t)=0;
    virtual void calcularColision(vector<Objeto*> pObjetos)=0;
    virtual void calcularBoundingBox() = 0;
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
    GLuint setup() {
        using namespace glm;
        using namespace std;
        const float pi = 3.1415926535897932384626433832795f;
        const float _2pi = 2.0f * pi;
        vector<vec3> positions;
        vector<vec3> normals;
        vector<vec2> textureCoords;
        for( int i = 0; i <= stacks; ++i )
        {
            // V texture coordinate.
            float V = i / (float)stacks;
            float phi = V * pi;
            for ( int j = 0; j <= slices; ++j )
            {
                // U texture coordinate.
                float U = j / (float)slices;
                float theta = U * _2pi;
                float X = cos(theta) * sin(phi);
                float Y = cos(phi);
                float Z = sin(theta) * sin(phi);
                positions.push_back( vec3( X, Y, Z) * radius );
                normals.push_back( vec3(X, Y, Z) );
                textureCoords.push_back( vec2(U, V) );
            }
        }
        // Now generate the index buffer
        //vector<GLuint> indicies;
        for( int i = 0; i < slices * stacks + slices; ++i ) {
            indices.push_back( i );
            indices.push_back( i + slices + 1  );
            indices.push_back( i + slices );
            indices.push_back( i + slices + 1  );
            indices.push_back( i );
            indices.push_back( i + 1 );
        }

        GLuint vao;
        glGenVertexArrays( 1, &vao );
        glBindVertexArray( vao );

        GLuint vbos[4];
        glGenBuffers( 4, vbos );

        glBindBuffer( GL_ARRAY_BUFFER, vbos[0] );
        glBufferData( GL_ARRAY_BUFFER, positions.size() * sizeof(vec3), positions.data(), GL_STATIC_DRAW );
        glVertexAttribPointer( POSITION_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glEnableVertexAttribArray( POSITION_ATTRIBUTE );

        glBindBuffer( GL_ARRAY_BUFFER, vbos[1] );
        glBufferData( GL_ARRAY_BUFFER, normals.size() * sizeof(vec3), normals.data(), GL_STATIC_DRAW );
        glVertexAttribPointer( NORMAL_ATTRIBUTE, 3, GL_FLOAT, GL_TRUE, 0, (void*)0 );
        glEnableVertexAttribArray( NORMAL_ATTRIBUTE );

        glBindBuffer( GL_ARRAY_BUFFER, vbos[2] );
        glBufferData( GL_ARRAY_BUFFER, textureCoords.size() * sizeof(vec2), textureCoords.data(), GL_STATIC_DRAW );
        glVertexAttribPointer( TEXCOORD0_ATTRIBUTE, 2, GL_FLOAT, GL_FALSE, 0, (void*)0 );
        glEnableVertexAttribArray( TEXCOORD0_ATTRIBUTE );

        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, vbos[3] );
        glBufferData( GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW );
        glBindVertexArray( 0 );
        glBindBuffer( GL_ARRAY_BUFFER, 0 );
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
        indices_size = indices.size();
        return vao;
    }
    void display(Shader &sh) {
        model = mat4(1.0);
        model = scale(model, vec3(0.5));
        model = translate(model, centro);
        sh.setMat4("model", model);
        if (visible) {
            glBindVertexArray(vao);
            glDrawElements(GL_TRIANGLES, indices_size, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        }
    }
    void actualizarDatos(float t) {
        if(afectaGravedad){
            float g = 9.8;
            centro.x = pos_ini.x + vel_ini.x * cos(radians(ang_ini)) * t;
            centro.y = pos_ini.y + vel_ini.y * sin(radians(ang_ini)) * t - 0.5 * g * t * t;
        }
        //cout<< t << "\t" << to_string(pos_ini) << "\t" << to_string(centro) << endl;
    }
    void calcularColision(vector<Objeto*> pObjetos) {
        for (auto &obj : pObjetos) {
            if (obj != this && bb->Colision( *obj->bb)) {
                // reacciónar a la colision
                cout << "Colisiono\n";
            }

        }
    }
    void calcularBoundingBox() {
        bb = new BoundingBox();
        bb->min = centro - vec3(radius);
        bb->max = centro + vec3(radius);
    }
};

class Caja : public Objeto {
public:
    vec3 posmin, posmax,centro;
    Caja() {
        posmin = vec3(0.0);
        posmax = vec3(1.0);
    }
    Caja(vec3 centro){
        posmin = centro - vec3(0.5);
        posmax = centro + vec3(0.5);
        this->centro = centro;
    }
    GLuint setup(){
        int vertices[24] = {
            -1, -1, -1,
            1, -1, -1,
            1, 1, -1,
            -1, 1, -1,
            -1, -1, 1,
            1, -1, 1,
            1, 1, 1,
            -1, 1, 1
        };

        vec2 texCoords[4] ={
            vec2(0, 0),
            vec2(1, 0),
            vec2(1, 1),
            vec2(0, 1)
        };

        vec3 normals[6] = {
            vec3(0, 0, 1),
            vec3(1, 0, 0),
            vec3(0, 0, -1),
            vec3(-1, 0, 0),
            vec3(0, 1, 0),
            vec3(0, -1, 0)
        };

        int indices[6 * 6] = {
            0, 1, 3, 3, 1, 2,
            1, 5, 2, 2, 5, 6,
            5, 4, 6, 6, 4, 7,
            4, 0, 7, 7, 0, 3,
            3, 2, 7, 7, 2, 6,
            4, 5, 0, 0, 5, 1
        };

        int texInds[6] = { 0, 1, 3, 3, 1, 2 };

        float vertexBuffer[18 * 6];
        for (int i = 0; i < 36; i++) {
            vertexBuffer[i * 3 + 0] = vertices[indices[i]];
            vertexBuffer[i * 3 + 1] = vertices[indices[i]+1];
            vertexBuffer[i * 3 + 2] = vertices[indices[i]+2];
        }

        float textureBuffer[12 * 6];
        for (int i = 0; i < 36; i++) {
            textureBuffer[i * 2 + 0] = texCoords[texInds[i % 4]].x;
            textureBuffer[i * 2 + 1] = texCoords[texInds[i % 4]].y;
        }

        float normalBuffer[18 * 6];
        for (int i = 0; i < 36; i++) {
            normalBuffer[i * 3 + 0] = normals[indices[i / 6]].x;
            normalBuffer[i * 3 + 1] = normals[indices[i / 6]].y;
            normalBuffer[i * 3 + 2] = normals[indices[i / 6]].z;
        }

        GLuint vao;
        glGenVertexArrays( 1, &vao );
        glBindVertexArray( vao );

        GLuint vbo;
        glGenBuffers( 1, &vbo );
        glBindBuffer( GL_ARRAY_BUFFER, vbo );
        glBufferData( GL_ARRAY_BUFFER, sizeof(vertexBuffer)*24, vertexBuffer, GL_STATIC_DRAW );
        glEnableVertexAttribArray( POSITION_ATTRIBUTE );
        glVertexAttribPointer( POSITION_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, 0, (void*)0 );


        this->vao = vao;
        this->vbo = vbo;
        return vao;
    }
    void display(Shader &sh){
        model = mat4(1.0);
        model = scale(model, vec3(0.5));
        model = translate(model, centro);
        sh.setMat4("model", model);
        if (visible) {
            glBindBuffer(GL_ARRAY_BUFFER, vbo); 
            glEnableVertexAttribArray( POSITION_ATTRIBUTE );
            glVertexAttribPointer( POSITION_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, 0, (void*)0 );

            glDrawArrays(GL_TRIANGLES, 0, 24); 

            glDisableVertexAttribArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }
    }

    void calcularColision(vector<Objeto*> pObjetos) {
        for (auto &obj : pObjetos) {
            if (obj != this && bb->Colision( *obj->bb)) {
                // reacciónar a la colision
                cout << "Colisionó caja\n";
            }

        }
    }
    void calcularBoundingBox() {
        bb = new BoundingBox();
        bb->min = centro - posmin;
        bb->max = centro + posmax;
    }
    void actualizarDatos(float t){
        if(afectaGravedad){
            float g = 9.8;
            centro.x = pos_ini.x + vel_ini.x * cos(radians(ang_ini)) * t;
            centro.y = pos_ini.y + vel_ini.y * sin(radians(ang_ini)) * t - 0.5 * g * t * t;
        }

    }
};

#endif //LEARNOPENGL_OBJETO_H
