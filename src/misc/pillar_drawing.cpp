#include <cmath>
#include <nanogui/nanogui.h>

#include "pillar_drawing.h"

#include "CGL/color.h"
#include "CGL/vector3D.h"

#define TCOORD_OFFSET 0
#define NORMAL_OFFSET 2
#define VERTEX_OFFSET 5
#define TANGEN_OFFSET 8
#define VERTEX_SIZE 11

using namespace nanogui;

namespace CGL {
namespace Misc {

PillarMesh::PillarMesh(int num_lat, int num_lon)
{
  
  Vertices.resize(VERTEX_SIZE * 328);
  
  
  for (int i = 0; i <= 40; i++) {
    for (int j = 0; j < 8; j++) {
      float offset = i % 2 == 1 ? 0.5 : 0;
      double angle = (j + offset) * PI / 4;
      double *vptr = &Vertices[VERTEX_SIZE * p_index(i, j)];
      vptr[NORMAL_OFFSET + 0] = vptr[VERTEX_OFFSET + 0] = cos(angle);
      vptr[VERTEX_OFFSET + 1] = ((double) i) / 40;
      vptr[NORMAL_OFFSET + 1] = 0;
      vptr[NORMAL_OFFSET + 2] = vptr[VERTEX_OFFSET + 2] = sin(angle);
      
      vptr[TANGEN_OFFSET + 0] = -sin(angle);
      vptr[TANGEN_OFFSET + 1] = 0;
      vptr[TANGEN_OFFSET + 2] = cos(angle);
    }
  }
  
  build_data();
}

int PillarMesh::p_index(int x, int y) {
  return (x * 8 + y);
}


void PillarMesh::build_data() {
  
  positions = MatrixXf(4, 656 * 3);
  normals = MatrixXf(4, 656 * 3);
  uvs = MatrixXf(2, 656 * 3);
  tangents = MatrixXf(4, 656 * 3);

  for (int i = 0; i < 41; i++) {
    for (int j = 0; j < 8; j++) {
      int a = i % 2 == 1 ? 1 : 0;
      double *vPtr1 = &Vertices[VERTEX_SIZE * p_index(i, j)];
      double *vPtr2 = &Vertices[VERTEX_SIZE * p_index(i, (j + 1) % 8)];
      double *vPtr3 = &Vertices[VERTEX_SIZE * p_index(i + 1, (j + a) % 8)];
      double *vPtr4 = &Vertices[VERTEX_SIZE * p_index(i + 1, (j + a + 1) % 8)];

      Vector3D p1(vPtr1[VERTEX_OFFSET], vPtr1[VERTEX_OFFSET + 1],
                  vPtr1[VERTEX_OFFSET + 2]);
      Vector3D p2(vPtr2[VERTEX_OFFSET], vPtr2[VERTEX_OFFSET + 1],
                  vPtr2[VERTEX_OFFSET + 2]);
      Vector3D p3(vPtr3[VERTEX_OFFSET], vPtr3[VERTEX_OFFSET + 1],
                  vPtr3[VERTEX_OFFSET + 2]);
      Vector3D p4(vPtr4[VERTEX_OFFSET], vPtr4[VERTEX_OFFSET + 1],
                  vPtr4[VERTEX_OFFSET + 2]);

      Vector3D n1(vPtr1[NORMAL_OFFSET], vPtr1[NORMAL_OFFSET + 1],
                  vPtr1[NORMAL_OFFSET + 2]);
      Vector3D n2(vPtr2[NORMAL_OFFSET], vPtr2[NORMAL_OFFSET + 1],
                  vPtr2[NORMAL_OFFSET + 2]);
      Vector3D n3(vPtr3[NORMAL_OFFSET], vPtr3[NORMAL_OFFSET + 1],
                  vPtr3[NORMAL_OFFSET + 2]);
      Vector3D n4(vPtr4[NORMAL_OFFSET], vPtr4[NORMAL_OFFSET + 1],
                  vPtr4[NORMAL_OFFSET + 2]);
      
      Vector3D uv1(i, j, 0);
      Vector3D uv2(i + 1, j, 0);
      Vector3D uv3(i, j + 1, 0);
      Vector3D uv4(i + 1, j + 1, 0);
      
      Vector3D t1(vPtr1[TANGEN_OFFSET], vPtr1[TANGEN_OFFSET + 1],
                  vPtr1[TANGEN_OFFSET + 2]);
      Vector3D t2(vPtr2[TANGEN_OFFSET], vPtr2[TANGEN_OFFSET + 1],
                  vPtr2[TANGEN_OFFSET + 2]);
      Vector3D t3(vPtr3[TANGEN_OFFSET], vPtr3[TANGEN_OFFSET + 1],
                  vPtr3[TANGEN_OFFSET + 2]);
      Vector3D t4(vPtr4[TANGEN_OFFSET], vPtr4[TANGEN_OFFSET + 1],
                  vPtr4[TANGEN_OFFSET + 2]);

      int t = (i * 16 + 2 * j) * 3;

      positions.col(t    ) << p1.x, p1.y, p1.z, 1.0;
      positions.col(t + 1) << p2.x, p2.y, p2.z, 1.0;
      positions.col(t + 2) << p3.x, p3.y, p3.z, 1.0;

      normals.col(t    ) << n1.x, n1.y, n1.z, 0.0;
      normals.col(t + 1) << n2.x, n2.y, n2.z, 0.0;
      normals.col(t + 2) << n3.x, n3.y, n3.z, 0.0;
      
      uvs.col(t    ) << uv1.x, uv1.y;
      uvs.col(t + 1) << uv2.x, uv2.y;
      uvs.col(t + 2) << uv3.x, uv3.y;
      
      tangents.col(t    ) << t1.x, t1.y, t1.z, 0.0;
      tangents.col(t + 1) << t2.x, t2.y, t2.z, 0.0;
      tangents.col(t + 2) << t3.x, t3.y, t3.z, 0.0;

      t = (i * 16 + 2 * j + 1) * 3;

      positions.col(t    ) << p4.x, p4.y, p4.z, 1.0;
      positions.col(t + 1) << p3.x, p3.y, p3.z, 1.0;
      positions.col(t + 2) << p2.x, p2.y, p2.z, 1.0;

      normals.col(t    ) << n4.x, n4.y, n4.z, 0.0;
      normals.col(t + 1) << n3.x, n3.y, n3.z, 0.0;
      normals.col(t + 2) << n2.x, n2.y, n2.z, 0.0;

      
      uvs.col(t    ) << uv4.x, uv4.y;
      uvs.col(t + 1) << uv3.x, uv3.y;
      uvs.col(t + 2) << uv2.x, uv2.y;
      
      tangents.col(t    ) << t4.x, t4.y, t4.z, 0.0;
      tangents.col(t + 1) << t3.x, t3.y, t3.z, 0.0;
      tangents.col(t + 2) << t2.x, t2.y, t2.z, 0.0;
    }
  }
}

void PillarMesh::draw_pillar(GLShader &shader, const Vector3D &p, double r, double h) {

  Matrix4f model;
  model << r, 0, 0, p.x, 
           0, h, 0, p.y,
           0, 0, r, p.z, 
           0, 0, 0, 1;

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

  shader.drawArray(GL_TRIANGLES, 0, 640 * 3);
}

} // namespace Misc
} // namespace CGL
