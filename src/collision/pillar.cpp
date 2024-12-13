#include <nanogui/nanogui.h>

#include "../clothMesh.h"
#include "../misc/pillar_drawing.h"
#include "pillar.h"

using namespace nanogui;
using namespace CGL;

Vector3D Pillar::collide(Fish &f) {
  // TODO (Part 3): Handle collisions with spheres.
  Vector3D p = Vector3D(f.position.x, 0, f.position.z);
  Vector3D o = Vector3D(base.x, 0, base.z);
  Vector3D v = Vector3D(f.velocity.x, 0, f.velocity.z);
  float dist = (p - o).norm();
  if (dist < radius) {
    f.position += (radius - dist) * (p - o);
    return (p - o) / dist;
  }
  if (dist < radius + 0.5) {
    float a = dot(v, v);
    float b = 2.0f*dot(p - o, v);
    float c = dot(p - o, (p - o)) - radius * radius;

    double discriminant = b*b - 4.0f*a*c;

    if (discriminant <= 0) {
        return Vector3D();
    } else {
        float t1 = (-b - sqrt(discriminant)) / (2.0f * a);
        float t2 = (-b + sqrt(discriminant)) / (2.0f * a);
        float t;
        if (t1 < 0 && t2 < 0) {
          return Vector3D();
        }
        if (t1 < 0) {
          t = t2;
        } else {
          t = t1;
        }
        Vector3D normal = (t * v + p) - o;
        normal /= normal.norm();
        Vector3D result = v - 2 * normal * dot(normal, v);
        result -= result * dot(result, v / v.norm());
        return result;
    }
  }

  return Vector3D();
}

Vector3D Pillar::collide(Predator &f) {
  // TODO (Part 3): Handle collisions with spheres.
  Vector3D p = Vector3D(f.position.x, 0, f.position.z);
  Vector3D o = Vector3D(base.x, 0, base.z);
  Vector3D v = Vector3D(f.velocity.x, 0, f.velocity.z);
  float dist = (p - o).norm();
  if (dist < radius) {
    f.position += (radius - dist) * (p - o);
    return (p - o) / dist;
  }
  if (dist < radius + 0.5) {
    float a = dot(v, v);
    float b = 2.0f*dot(p - o, v);
    float c = dot(p - o, (p - o)) - radius * radius;

    double discriminant = b*b - 4.0f*a*c;

    if (discriminant <= 0) {
        return Vector3D();
    } else {
        float t1 = (-b - sqrt(discriminant)) / (2.0f * a);
        float t2 = (-b + sqrt(discriminant)) / (2.0f * a);
        float t;
        if (t1 < 0 && t2 < 0) {
          return Vector3D();
        }
        if (t1 < 0) {
          t = t2;
        } else {
          t = t1;
        }
        Vector3D normal = (t * v + p) - o;
        normal /= normal.norm();
        Vector3D result = v - 2 * normal * dot(normal, v);
        result -= result * dot(result, v / v.norm());
        return result;
    }
  }

  return Vector3D();
}

void Pillar::render(GLShader &shader) {
  // We decrease the radius here so flat triangles don't behave strangely
  // and intersect with the sphere when rendered
  m_pillar_mesh.draw_pillar(shader, base, radius * 0.92, height);
}
