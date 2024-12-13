#include "iostream"
#include <nanogui/nanogui.h>

#include "../clothMesh.h"
#include "../fishSimulator.h"
#include "plane.h"

using namespace std;
using namespace CGL;

#define SURFACE_OFFSET 0.0001

Vector3D Plane::collide(Fish &f) {
  // TODO (Part 3): Handle collisions with planes.
  if (f.position.y - point.y < 0.001) {
    f.position.y = 0.001;
    if (!f.alive) f.velocity = Vector3D(0.000001, -0.001, 0.000001);
  }
  if (f.position.y - point.y < 0.1) {
    float weight = dot(f.velocity / f.velocity.norm(), Vector3D(0, -1, 0));
    if (weight < 0) weight = 0;
    return Vector3D(0, 1 - (f.position.y - point.y) / 0.1, 0) * weight;
  }
  return Vector3D();
}

Vector3D Plane::collide(Predator &f) {
  // TODO (Part 3): Handle collisions with planes.
  if (f.position.y - point.y < 0.001) f.position.y = 0.001;
  if (f.position.y - point.y < 0.1) {
    float weight = dot(f.velocity / f.velocity.norm(), Vector3D(0, -1, 0));
    if (weight < 0) weight = 0;
    return Vector3D(0, 1 - (f.position.y - point.y) / 0.1, 0) * weight;
  }
  return Vector3D();
}

void Plane::render(GLShader &shader) {
  nanogui::Color color(255.0f / 256, 233.0f / 256, 198.0f / 256, 1.0f);

  Vector3f sPoint(point.x, point.y, point.z);
  Vector3f sNormal(normal.x, normal.y, normal.z);
  Vector3f sParallel(normal.y - normal.z, normal.z - normal.x,
                     normal.x - normal.y);
  sParallel.normalize();
  Vector3f sCross = sNormal.cross(sParallel);

  MatrixXf positions(3, 4);
  MatrixXf normals(3, 4);
  MatrixXf uvs(2, 4);

  positions.col(0) << (sPoint + 2 * (sCross + sParallel) * 8);
  positions.col(1) << (sPoint + 2 * (sCross - sParallel) * 8);
  positions.col(2) << (sPoint + 2 * (-sCross + sParallel) * 8);
  positions.col(3) << (sPoint + 2 * (-sCross - sParallel) * 8);

  normals.col(0) << sNormal;
  normals.col(1) << sNormal;
  normals.col(2) << sNormal;
  normals.col(3) << sNormal;

  uvs.col(0) << 0, 0;
  uvs.col(1) << 0, 1;
  uvs.col(2) << 1, 0;
  uvs.col(3) << 1, 1;

  shader.uploadAttrib("in_position", positions);
  if (shader.attrib("in_normal", false) != -1) {
    shader.uploadAttrib("in_normal", normals);
  }
  if (shader.attrib("in_uv", false) != -1) {
    shader.uploadAttrib("in_uv", uvs);
  }

  shader.drawArray(GL_TRIANGLE_STRIP, 0, 4);
}
