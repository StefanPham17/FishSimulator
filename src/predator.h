#ifndef PREDATOR_H
#define PREDATOR_H

#include "CGL/CGL.h"
#include "CGL/misc.h"
#include "CGL/vector3D.h"
#include "fish.h"

using namespace CGL;

// Forward declarations
class Halfedge;

struct Predator {
  public:
    Predator(Vector3D position, Vector3D velocity)
        : start_position(position), position(position),
          start_velocity(velocity), velocity(velocity),
          prev_velocity(velocity), personalTime(0),
          hermitePoints({{(double) rand() / (RAND_MAX) - 0.5, (double) rand() / (RAND_MAX) - 0.5, (double) rand() / (RAND_MAX) - 0.5},
                         {(double) rand() / (RAND_MAX) - 0.5, (double) rand() / (RAND_MAX) - 0.5, (double) rand() / (RAND_MAX) - 0.5},
                         {(double) rand() / (RAND_MAX) - 0.5, (double) rand() / (RAND_MAX) - 0.5, (double) rand() / (RAND_MAX) - 0.5},
                         {(double) rand() / (RAND_MAX) - 0.5, (double) rand() / (RAND_MAX) - 0.5, (double) rand() / (RAND_MAX) - 0.5}}),
          restTime(0), lockedInTime(0), lockedFish(nullptr)
          {}

    Vector3D normal();

    // static values
    Vector3D start_position;
    Vector3D start_velocity;

    // dynamic values
    Vector3D position;
    Vector3D velocity;
    Vector3D prev_velocity;

    // wander values
    std::vector<Vector3D> hermitePoints;
    int personalTime;
    int restTime;
    int lockedInTime;
    Fish* lockedFish;

    void render(GLShader &shader);
    void replenishHermitePoints();

    // mesh reference
    Halfedge *halfedge;

  private:
    
    Misc::FishMesh m_fish_mesh;
};

#endif /* PREDATOR_H */
