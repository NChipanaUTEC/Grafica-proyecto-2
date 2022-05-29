#include "Objeto.h"
#include "BoundingVolume.h"

GLuint Esfera::setup() {
  using namespace glm;
  using namespace std;
  const float pi = 3.1415926535897932384626433832795f;
  const float _2pi = 2.0f * pi;
  vector<vec3> positions;
  vector<vec3> normals;
  vector<vec2> textureCoords;
  for (int i = 0; i <= stacks; ++i) {
    // V texture coordinate.
    float V = i / (float)stacks;
    float phi = V * pi;
    for (int j = 0; j <= slices; ++j) {
      // U texture coordinate.
      float U = j / (float)slices;
      float theta = U * _2pi;
      float X = cos(theta) * sin(phi);
      float Y = cos(phi);
      float Z = sin(theta) * sin(phi);
      positions.push_back(vec3(X, Y, Z) * radius);
      normals.push_back(vec3(X, Y, Z));
      textureCoords.push_back(vec2(U, V));
    }
  }
  // Now generate the index buffer
  // vector<GLuint> indicies;
  for (int i = 0; i < slices * stacks + slices; ++i) {
    indices.push_back(i);
    indices.push_back(i + slices + 1);
    indices.push_back(i + slices);
    indices.push_back(i + slices + 1);
    indices.push_back(i);
    indices.push_back(i + 1);
  }

  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  GLuint vbos[4];
  glGenBuffers(4, vbos);

  glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
  glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(vec3),
               positions.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(POSITION_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, 0,
                        (void *)0);
  glEnableVertexAttribArray(POSITION_ATTRIBUTE);

  glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
  glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(vec3), normals.data(),
               GL_STATIC_DRAW);
  glVertexAttribPointer(NORMAL_ATTRIBUTE, 3, GL_FLOAT, GL_TRUE, 0, (void *)0);
  glEnableVertexAttribArray(NORMAL_ATTRIBUTE);

  glBindBuffer(GL_ARRAY_BUFFER, vbos[2]);
  glBufferData(GL_ARRAY_BUFFER, textureCoords.size() * sizeof(vec2),
               textureCoords.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(TEXCOORD0_ATTRIBUTE, 2, GL_FLOAT, GL_FALSE, 0,
                        (void *)0);
  glEnableVertexAttribArray(TEXCOORD0_ATTRIBUTE);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos[3]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint),
               indices.data(), GL_STATIC_DRAW);
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  indices_size = indices.size();
  return vao;
}

void Esfera::display(Shader &sh) {
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

void Esfera::actualizarDatos(float t) {
  float g = 9.8;
  centro.x = pos_ini.x + vel_ini.x * cos(radians(ang_ini)) * t;
  centro.y =
      pos_ini.y + vel_ini.y * sin(radians(ang_ini)) * t - 0.5 * g * t * t;
  cout << t << "\t" << to_string(pos_ini) << "\t" << to_string(centro) << endl;
}

void Esfera::calcularColision(vector<Objeto *> pObjetos) {
  for (auto &obj : pObjetos) {
    if (bv->Colision(*obj->bv)) {
      // reacciónar a la colision
    }
  }
}
