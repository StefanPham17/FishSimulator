#ifndef CGL_UTIL_PILLARDRAWING_H
#define CGL_UTIL_PILLARDRAWING_H

#include <vector>

#include <nanogui/nanogui.h>

#include "CGL/CGL.h"

using namespace nanogui;

namespace CGL {
namespace Misc {
  
class PillarMesh {
public:
  // Supply the desired number of vertices
  PillarMesh(int num_lat = 40, int num_lon = 40);
  
  
  /**
   * Draws a sphere with the given position and radius in opengl, using the
   * current modelview/projection matrices and color/material settings.
   */
  void draw_pillar(GLShader &shader, const Vector3D &p, double r, double h);
private:
  std::vector<unsigned int> Indices;
  std::vector<double> Vertices;
  
  void build_data();
  int p_index(int x, int y);
  
  MatrixXf positions;
  MatrixXf normals;
  MatrixXf uvs;
  MatrixXf tangents;
};


} // namespace Misc
} // namespace CGL

#endif // CGL_UTIL_SPHEREDRAWING_H
