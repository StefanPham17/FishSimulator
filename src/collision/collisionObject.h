#ifndef COLLISIONOBJECT
#define COLLISIONOBJECT

#include <nanogui/nanogui.h>

#include "../clothMesh.h"

using namespace CGL;
using namespace std;
using namespace nanogui;

class CollisionObject {
public:
  virtual void render(GLShader &shader) = 0;
  virtual Vector3D collide(Fish &f) = 0;
  virtual Vector3D collide(Predator &f) = 0;

private:
  double friction;
};

#endif /* COLLISIONOBJECT */
