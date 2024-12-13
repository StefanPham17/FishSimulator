#include <nanogui/nanogui.h>

#include <cstdlib>
#include "clothMesh.h"
#include "misc/fish_drawing.h"
#include "fish.h"

using namespace nanogui;
using namespace CGL;

void Fish::render(GLShader &shader) {
  // We decrease the radius here so flat triangles don't behave strangely
  // and intersect with the sphere when rendered
  m_fish_mesh.draw_fish(shader, position, velocity, fish_size);
}

void Fish::replenishHermitePoints() {
  // We decrease the radius here so flat triangles don't behave strangely
  // and intersect with the sphere when rendered
    if (hermitePoints.size() != 4) {
        return;
    }
    Vector3D front = hermitePoints[3];
    hermitePoints.clear();
    hermitePoints.push_back(front);
    for (int i = 0; i < 3; i++) {
        hermitePoints.emplace_back((double) rand() / (RAND_MAX) - 0.5, 
                                    (double) rand() / (RAND_MAX) - 0.5,
                                    (double) rand() / (RAND_MAX) - 0.5);
    }
}       
