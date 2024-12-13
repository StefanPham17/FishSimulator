#ifndef CGL_UTIL_FISHDRAWING_H
#define CGL_UTIL_FISHDRAWING_H

#include <vector>

#include <nanogui/nanogui.h>

#include "CGL/CGL.h"

using namespace nanogui;

namespace CGL {
namespace Misc {
  
class FishMesh {
public:
  // Supply the desired number of vertices
  FishMesh();
  
  
  /**
   * Draws a fish with the given position and radius in opengl, using the
   * current modelview/projection matrices and color/material settings.
   */
  void draw_fish(GLShader &shader, const Vector3D &p, const Vector3D v, double s);
private:  
  void build_data();
  
  MatrixXf positions;
  MatrixXf normals;
  MatrixXf uvs;
  MatrixXf tangents;
};


} // namespace Misc
} // namespace CGL

#endif // CGL_UTIL_FISHDRAWING_H
