//
// Created by hgallegos on 23/05/2022.
//

#ifndef PROYECTO2_BOUNDINGVOLUME_H
#define PROYECTO2_BOUNDINGVOLUME_H

#include <glm/glm.hpp>
#include "Objeto.h"
using namespace glm;

class BoundingVolume {
public:
    virtual bool Colision(BoundingVolume &bv)=0;
    virtual void calcular(Esfera &esf)=0;
};

class BoundingBox : public BoundingVolume {
public:
    vec3 min, max;
    bool Colision(BoundingVolume &bv) { return false; }
    bool Colision(BoundingBox &bv);
    void calcular(Esfera &esf);
};


#endif //PROYECTO2_BOUNDINGVOLUME_H
