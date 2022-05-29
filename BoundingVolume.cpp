#include "BoundingVolume.h"

bool BoundingBox::Colision(BoundingBox &b) {

  return (min.x <= b.max.x && max.x >= b.min.x) &&
         (min.y <= b.max.y && max.y >= b.min.y) &&
         (min.z <= b.max.z && max.z >= b.min.z);
}

void BoundingBox::calcular(Esfera &esf) {
  min = esf.centro - vec3(esf.radius);
  max = esf.centro + vec3(esf.radius);
}