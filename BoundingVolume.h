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
    bool Colision(BoundingBox &bv) {
        return (min.x <= bv.max.x && max.x >= bv.min.x) &&
               (min.y <= bv.max.y && max.y >= bv.min.y) &&
               (min.z <= bv.max.z && max.z >= bv.min.z);
    }
    void calcular(Esfera &esf) {
        min = esf.centro - vec3(esf.radius);
        max = esf.centro + vec3(esf.radius);
    }
};


#endif //PROYECTO2_BOUNDINGVOLUME_H
