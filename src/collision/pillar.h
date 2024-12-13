#ifndef COLLISIONOBJECT_PILLAR_H
#define COLLISIONOBJECT_PILLAR_H

#include "../clothMesh.h"
#include "../misc/pillar_drawing.h"
#include "collisionObject.h"

using namespace CGL;
using namespace std;

struct Pillar : public CollisionObject {
public:
  Pillar(const Vector3D &base, double radius, double height)
      : base(base), radius(radius), height(height),
        m_pillar_mesh(Misc::PillarMesh()) {}

  void render(GLShader &shader);
  Vector3D collide(Fish &f);
  Vector3D collide(Predator &f);

private:
  Vector3D base;
  double radius;
  double height;
  
  Misc::PillarMesh m_pillar_mesh;
};

#endif /* COLLISIONOBJECT_PILLAR_H */
