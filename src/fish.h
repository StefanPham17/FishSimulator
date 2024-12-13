#ifndef FISH_H
#define FISH_H

#include "CGL/CGL.h"
#include "CGL/misc.h"
#include "CGL/vector3D.h"
#include <vector>

#include "clothMesh.h"
#include "misc/fish_drawing.h"

using namespace CGL;

// Forward declarations
class Halfedge;

struct Fish {
  public:
    Fish(Vector3D position, Vector3D velocity, float size)
        : start_position(position), position(position),
          start_velocity(velocity), velocity(velocity),
          prev_velocity(velocity), fish_size(size), personalTime(0),
          hermitePoints({{(double) rand() / (RAND_MAX) - 0.5, (double) rand() / (RAND_MAX) - 0.5, (double) rand() / (RAND_MAX) - 0.5},
                         {(double) rand() / (RAND_MAX) - 0.5, (double) rand() / (RAND_MAX) - 0.5, (double) rand() / (RAND_MAX) - 0.5},
                         {(double) rand() / (RAND_MAX) - 0.5, (double) rand() / (RAND_MAX) - 0.5, (double) rand() / (RAND_MAX) - 0.5},
                         {(double) rand() / (RAND_MAX) - 0.5, (double) rand() / (RAND_MAX) - 0.5, (double) rand() / (RAND_MAX) - 0.5}}),
          alive(true), deadTime(0)
          {}

    Vector3D normal();

    // static values
    Vector3D start_position;
    Vector3D start_velocity;

    // dynamic values
    Vector3D position;
    Vector3D velocity;
    Vector3D prev_velocity;
    bool alive;

    // wander values
    std::vector<Vector3D> hermitePoints;
    int personalTime;
    int deadTime;

    float fish_size;

    void render(GLShader &shader);
    void replenishHermitePoints();

    // mesh reference
    Halfedge *halfedge;

  private:
    
    Misc::FishMesh m_fish_mesh;
};

#endif /* FISH_H */
