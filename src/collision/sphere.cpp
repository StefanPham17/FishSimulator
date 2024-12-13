#include <nanogui/nanogui.h>

#include "../clothMesh.h"
#include "../misc/sphere_drawing.h"
#include "sphere.h"

using namespace nanogui;
using namespace CGL;

Vector3D Sphere::collide(Fish &f) {
  // TODO (Part 3): Handle collisions with spheres.
  Vector3D p = f.position;
  Vector3D o = origin;
  Vector3D v = f.velocity;
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

Vector3D Sphere::collide(Predator &f) {
  // TODO (Part 3): Handle collisions with spheres.
  Vector3D p = f.position;
  Vector3D o = origin;
  Vector3D v = f.velocity;
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

void Sphere::render(GLShader &shader) {
  // We decrease the radius here so flat triangles don't behave strangely
  // and intersect with the sphere when rendered
  m_sphere_mesh.draw_sphere(shader, origin, radius * 0.92);
}
