#include <cmath>
#include <nanogui/nanogui.h>

#include "fish_drawing.h"

#include "CGL/color.h"
#include "CGL/vector2D.h"
#include "CGL/vector3D.h"

#define TCOORD_OFFSET 0
#define NORMAL_OFFSET 2
#define VERTEX_OFFSET 5
#define TANGEN_OFFSET 8
#define VERTEX_SIZE 11

using namespace nanogui;

namespace CGL {
namespace Misc {

FishMesh::FishMesh() {
  build_data();
}
// Helper function that does linear interpolation.
// Borrowed from : https://gamedev.stackexchange.com/questions/23743/whats-the-most-efficient-way-to-find-barycentric-coordinates
Vector2D compute_uv(Vector3D p, Vector3D a, Vector3D b, Vector3D c)
{
  Vector3D normal = cross(a, b);
  Vector3D u = Vector3D();
  if (normal.x == 0) {
    u.y = -1 * normal.z;
    u.z = normal.y;
  } else if (normal.y == 0) {
    u.x = -1 * normal.z;
    u.z = normal.x;
  } else {
    u.x = -1 * normal.y;
    u.y = normal.x;
  }
  Vector3D v = cross(normal, u);
  return Vector2D(dot(u, p), dot(v, p));
}

void FishMesh::build_data() {
  
  positions = MatrixXf(4, 72);
  normals = MatrixXf(4, 72);
  uvs = MatrixXf(2, 72);
  tangents = MatrixXf(4, 72);

  Vector3D head(0.5, 0, 0);
  Vector3D end(-0.25, 0, 0);
  Vector3D tail(-0.5, 0, 0);

  Vector3D headNorm(1, 0, 0);
  Vector3D endNorm(-1, 0, 0);
  Vector3D tailNorm(-1, 0, 0);

  Vector3D headTang(0, 1, 0);
  Vector3D endTang(0, -1, 0);
  Vector3D tailTang(0, -1, 0);

  std::vector<Vector3D> body = {};
  std::vector<Vector3D> bodyNorm = {};
  std::vector<Vector3D> bodyTang = {};

  std::vector<Vector3D> tailBody = {};
  std::vector<Vector3D> tailBodyNorm = {};
  std::vector<Vector3D> tailBodyTang = {};

  for (int i = 0; i < 8; i++) {
    body.emplace_back(0.25, sin(PI * i / 4) / 4, cos(PI * i / 4) / 4);
    bodyNorm.emplace_back(0, sin(PI * i / 4), cos(PI * i / 4));
    bodyTang.emplace_back(0, cos(PI * i / 4), -sin(PI * i / 4));
  }

  tailBody.emplace_back(-0.5, 0.25, 0);
  tailBody.emplace_back(-0.5, 0, 0.125);
  tailBody.emplace_back(-0.5, -0.25, 0);
  tailBody.emplace_back(-0.5, 0, -0.125);

  for (int i = 0; i < 4; i++) {
    tailBodyNorm.emplace_back(0, cos(PI * i / 2), sin(PI * i / 2));
    tailBodyTang.emplace_back(0, -sin(PI * i / 2), cos(PI * i / 2));
  }

  for (int i = 0; i < 8; i++) {
    positions.col(3 * i    ) << body[i % 8].x, body[i % 8].y, body[i % 8].z, 1.0;
    positions.col(3 * i + 1) << body[(i + 1) % 8].x, body[(i + 1) % 8].y, body[(i + 1) % 8].z, 1.0;
    positions.col(3 * i + 2) << head.x, head.y, head.z, 1.0;

    normals.col(3 * i    ) << bodyNorm[i % 8].x, bodyNorm[i % 8].y, bodyNorm[i % 8].z, 0.0;
    normals.col(3 * i + 1) << bodyNorm[(i + 1) % 8].x, bodyNorm[(i + 1) % 8].y, bodyNorm[(i + 1) % 8].z, 0.0;
    normals.col(3 * i + 2) << headNorm.x, headNorm.y, headNorm.z, 0.0;

    tangents.col(3 * i    ) << bodyTang[i % 8].x, bodyTang[i % 8].y, bodyTang[i % 8].z, 0.0;
    tangents.col(3 * i + 1) << bodyTang[(i + 1) % 8].x, bodyTang[(i + 1) % 8].y, bodyTang[(i + 1) % 8].z, 0.0;
    tangents.col(3 * i + 2) << headTang.x, headTang.y, headTang.z, 0.0;

    uvs.col(3 * i) << 0, 0;
    uvs.col(3 * i + 1) << 1, 0;
    uvs.col(3 * i + 2) << 0, 1;
  }

  for (int i = 8; i < 16; i++) {
    positions.col(3 * i    ) << body[i % 8].x, body[i % 8].y, body[i % 8].z, 1.0;
    positions.col(3 * i + 1) << body[(i + 1) % 8].x, body[(i + 1) % 8].y, body[(i + 1) % 8].z, 1.0;
    positions.col(3 * i + 2) << end.x, end.y, end.z, 1.0;

    normals.col(3 * i    ) << bodyNorm[i % 8].x, bodyNorm[i % 8].y, bodyNorm[i % 8].z, 0.0;
    normals.col(3 * i + 1) << bodyNorm[(i + 1) % 8].x, bodyNorm[(i + 1) % 8].y, bodyNorm[(i + 1) % 8].z, 0.0;
    normals.col(3 * i + 2) << endNorm.x, endNorm.y, endNorm.z, 0.0;

    tangents.col(3 * i    ) << bodyTang[i % 8].x, bodyTang[i % 8].y, bodyTang[i % 8].z, 0.0;
    tangents.col(3 * i + 1) << bodyTang[(i + 1) % 8].x, bodyTang[(i + 1) % 8].y, bodyTang[(i + 1) % 8].z, 0.0;
    tangents.col(3 * i + 2) << endTang.x, endTang.y, endTang.z, 0.0;

    uvs.col(3 * i) << 0, 0;
    uvs.col(3 * i + 1) << 1, 0;
    uvs.col(3 * i + 2) << 0, 1;
  }

  for (int i = 16; i < 20; i++) {
    positions.col(3 * i    ) << tailBody[i % 4].x, tailBody[i % 4].y, tailBody[i % 4].z, 1.0;
    positions.col(3 * i + 1) << tailBody[(i + 1) % 4].x, tailBody[(i + 1) % 4].y, tailBody[(i + 1) % 4].z, 1.0;
    positions.col(3 * i + 2) << end.x, end.y, end.z, 1.0;

    normals.col(3 * i    ) << tailBodyNorm[i % 4].x, tailBodyNorm[i % 4].y, tailBodyNorm[i % 4].z, 0.0;
    normals.col(3 * i + 1) << tailBodyNorm[(i + 1) % 4].x, tailBodyNorm[(i + 1) % 4].y, tailBodyNorm[(i + 1) % 4].z, 0.0;
    normals.col(3 * i + 2) << endNorm.x, endNorm.y, endNorm.z, 0.0;

    tangents.col(3 * i    ) << bodyTang[i % 4].x, bodyTang[i % 4].y, bodyTang[i % 4].z, 0.0;
    tangents.col(3 * i + 1) << bodyTang[(i + 1) % 4].x, bodyTang[(i + 1) % 4].y, bodyTang[(i + 1) % 4].z, 0.0;
    tangents.col(3 * i + 2) << endTang.x, endTang.y, endTang.z, 0.0;

    uvs.col(3 * i) << 0, 0;
    uvs.col(3 * i + 1) << 1, 0;
    uvs.col(3 * i + 2) << 0, 1;
  }

  for (int i = 20; i < 24; i++) {
    positions.col(3 * i    ) << tailBody[i % 4].x, tailBody[i % 4].y, tailBody[i % 4].z, 1.0;
    positions.col(3 * i + 1) << tailBody[(i + 1) % 4].x, tailBody[(i + 1) % 4].y, tailBody[(i + 1) % 4].z, 1.0;
    positions.col(3 * i + 2) << tail.x, tail.y, tail.z, 1.0;

    normals.col(3 * i    ) << tailBodyNorm[i % 4].x, tailBodyNorm[i % 4].y, tailBodyNorm[i % 4].z, 0.0;
    normals.col(3 * i + 1) << tailBodyNorm[(i + 1) % 4].x, tailBodyNorm[(i + 1) % 4].y, tailBodyNorm[(i + 1) % 4].z, 0.0;
    normals.col(3 * i + 2) << tailNorm.x, tailNorm.y, tailNorm.z, 0.0;

    tangents.col(3 * i    ) << bodyTang[i % 4].x, bodyTang[i % 4].y, bodyTang[i % 4].z, 0.0;
    tangents.col(3 * i + 1) << bodyTang[(i + 1) % 4].x, bodyTang[(i + 1) % 4].y, bodyTang[(i + 1) % 4].z, 0.0;
    tangents.col(3 * i + 2) << tailTang.x, tailTang.y, tailTang.z, 0.0;

    uvs.col(3 * i) << 0, 0;
    uvs.col(3 * i + 1) << 1, 0;
    uvs.col(3 * i + 2) << 0, 1;
  }
}

void FishMesh::draw_fish(GLShader &shader, const Vector3D &p, const Vector3D v, double s) {

  Matrix4f model;
  Matrix4f scale;

  Vector3D d = v / v.norm();
  double x = asin(d.y);
  double y = acos(d.x/cos(x));

  scale << s, 0, 0, 0, 
           0, s, 0, 0, 
           0, 0, s / 2, 0, 
           0, 0, 0, 1;

  model << d.x, -sin(x)*cos(y), sin(y), p.x, 
           d.y, cos(x), 0, p.y, 
           d.z, sin(x)*sin(y), cos(y), p.z, 
           0, 0, 0, 1;
  
  model = model * scale;

  shader.setUniform("u_model", model);


  shader.uploadAttrib("in_position", positions);
  if (shader.attrib("in_normal", false) != -1) {
    shader.uploadAttrib("in_normal", normals);
  } 
  if (shader.attrib("in_uv", false) != -1) {
    shader.uploadAttrib("in_uv", uvs);
  }
  if (shader.attrib("in_tangent", false) != -1) {
    shader.uploadAttrib("in_tangent", tangents, false);
  }

  shader.drawArray(GL_TRIANGLES, 0, 72);
}

} // namespace Misc
} // namespace CGL
