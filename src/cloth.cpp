#include <cstdlib>
#include <iostream>
#include <math.h>
#include <random>
#include <vector>

#include "cloth.h"
#include "collision/plane.h"
#include "collision/sphere.h"
#include "collision/pillar.h"

using namespace std;

Cloth::Cloth(double width, double height, int num_fish,
             int num_predators, float thickness) {
  this->width = width;
  this->height = height;
  this->num_fish = num_fish;
  this->num_predators = num_predators;
  this->thickness = thickness;

  buildGrid();
}

Cloth::~Cloth() {
  predators.clear();
  fishes.clear();

  if (clothMesh) {
    delete clothMesh;
  }
}

void Cloth::buildGrid() {
  // TODO (Part 1): Build a grid of masses and springs.
  for (int i = 0; i < num_fish; i++) {
    Vector3D position = Vector3D(((double) rand() - RAND_MAX / 2) * width / (RAND_MAX), \
                                 ((double) rand() - RAND_MAX / 2) * height / (RAND_MAX),\
                                 ((double) rand() - RAND_MAX / 2) * width / (RAND_MAX));
    Vector3D velocity = Vector3D((double) rand() / (RAND_MAX) - 0.5, 
                                 (double) rand() / (RAND_MAX) - 0.5,
                                 (double) rand() / (RAND_MAX) - 0.5);
    fishes.emplace_back(position, velocity / 2, 0.1);
  }
}

void Cloth::simulate(double frames_per_sec, double simulation_steps, ClothParameters *cp,
                     vector<Vector3D> external_accelerations,
                     vector<CollisionObject *> *collision_objects,
                     Vector3D alignment, Vector3D separation, Vector3D cohesion, Vector3D entities,
                     Plane* ground, float wander, float boundary, float collision, float hunt, float flee) {
  double delta_t = 1.0f / frames_per_sec / simulation_steps;

  if (fishes.size() > entities.x) {
    int difference = fishes.size() - entities.x;
    for (int i = 0; i < difference; i++) {
      fishes.pop_back();
    }
  } else if (fishes.size() < entities.x) {
    int difference = entities.x - fishes.size();
    for (int i = 0; i < difference; i++) {
      Vector3D position = Vector3D(((double) rand() - RAND_MAX / 2) * width / (RAND_MAX), \
                                  ((double) rand() - RAND_MAX / 2) * height / (RAND_MAX),\
                                  ((double) rand() - RAND_MAX / 2) * width / (RAND_MAX));
      Vector3D velocity = Vector3D((double) rand() / (RAND_MAX) - 0.5, 
                                  (double) rand() / (RAND_MAX) - 0.5,
                                  (double) rand() / (RAND_MAX) - 0.5);
      fishes.emplace_back(position, velocity / 2, entities.z);
    }
  }

  if (predators.size() > entities.y) {
    int difference = predators.size() - entities.y;
    for (int i = 0; i < difference; i++) {
      predators.pop_back();
    }
  } else if (predators.size() < entities.y) {
    int difference = entities.y - predators.size();
    for (int i = 0; i < difference; i++) {
      Vector3D position = Vector3D(((double) rand() - RAND_MAX / 2) * width / (RAND_MAX), \
                                  ((double) rand() - RAND_MAX / 2) * height / (RAND_MAX),\
                                  ((double) rand() - RAND_MAX / 2) * width / (RAND_MAX));
      Vector3D velocity = Vector3D((double) rand() / (RAND_MAX) - 0.5, 
                                  (double) rand() / (RAND_MAX) - 0.5,
                                  (double) rand() / (RAND_MAX) - 0.5);
      predators.emplace_back(position, velocity / 2);
    }
  }

  for (Fish &f : fishes) {
    f.position += delta_t * f.velocity;
    f.prev_velocity = f.velocity;
    f.velocity += delta_t * ground->collide(f) * collision;
    f.fish_size = entities.z;
    for (CollisionObject* object : *collision_objects) {
      f.velocity += delta_t * object->collide(f) * collision;
    }
  }
  
  /* Fish Movement */
  

  for (Fish &f : fishes) {
    if (!f.alive) {
      f.velocity -= f.velocity / f.velocity.norm() * delta_t * 4;
      f.velocity -= Vector3D((double) rand() / (RAND_MAX) / 100, 1, (double) rand() / (RAND_MAX) / 100) * delta_t * 4;
      if (f.deadTime > 0) {
        f.deadTime--;
      } else {
        f.alive = true;
      }
      continue;
    } 

    /* Alignment, Separation, Cohesion */
    Vector3D alighnmentAcceleration = Vector3D();
    Vector3D separationAcceleration = Vector3D();
    Vector3D cohesionAcceleration = Vector3D();
    int ac(0), sc(0), cc(0);

    for (Fish &n : fishes) {
      if (!n.alive) continue;
      if (n.position == f.position) continue;
      Vector3D difference = n.position - f.position;
      float dist = difference.norm();
      if (dist < alignment.x) {
        ac++;
        if (n.prev_velocity.norm() != 0) alighnmentAcceleration += n.prev_velocity / n.prev_velocity.norm() * (1 - dist / alignment.x);
      }
      if (dist < separation.x) {
        sc++;
        if (dist != 0) separationAcceleration -= difference / dist * (1 - dist / separation.x);
      }
      if (dist < cohesion.x) {
        cc++;
        if (dist != 0) cohesionAcceleration += difference / dist * (1 - dist / cohesion.x);
      }
    }

    if (ac) alighnmentAcceleration /= ac;
    if (sc) separationAcceleration /= sc;
    if (cc) cohesionAcceleration   /= cc;

    f.velocity += alighnmentAcceleration * delta_t * alignment.y \
                + separationAcceleration * delta_t * separation.y \
                + cohesionAcceleration   * delta_t * cohesion.y;

    /* Boundary */
    Vector3D boundaryAcceleration = Vector3D();
    if (f.position.x < -width / 2) {
      boundaryAcceleration.x += -width / 2 - f.position.x;
    }
    if (f.position.y < -height / 2) {
      boundaryAcceleration.y += -height / 2 - f.position.y;
    }
    if (f.position.z < -width / 2) {
      boundaryAcceleration.z += -width / 2 - f.position.z;
    }
    if (f.position.x > width / 2) {
      boundaryAcceleration.x +=  width / 2 - f.position.x;
    }
    if (f.position.y > height / 2) {
      boundaryAcceleration.y +=  height / 2 - f.position.y;
    }
    if (f.position.z > width / 2) {
      boundaryAcceleration.z +=  width / 2 - f.position.z;
    }

    f.velocity += boundaryAcceleration * delta_t * boundary;

    /* Wander */
    if (f.personalTime >= 3000) {
      f.replenishHermitePoints();
      f.personalTime = 0;
    } else {
      f.personalTime++;
    }

    Vector3D A((2 * f.hermitePoints[0] - 2 * f.hermitePoints[1] + f.hermitePoints[2] + f.hermitePoints[3]));
    Vector3D B((-3 * f.hermitePoints[0] + 3 * f.hermitePoints[1] - 2 * f.hermitePoints[2] - f.hermitePoints[3]));
    Vector3D C(f.hermitePoints[2]);
    Vector3D D(f.hermitePoints[0]);

    float t = f.personalTime / 3000;
    Vector3D wanderAcceleration = A * t * t * t + B * t * t + C * t + D;
    wanderAcceleration.y *= 0.2;

    f.velocity += wanderAcceleration * delta_t * wander;

    /* Predator Avoidance */
    Vector3D avoidanceAcceleration = Vector3D();
    int pc(0);
    for (Predator &p : predators) {
      Vector3D difference = p.position - f.position;
      float dist = difference.norm();
      if (dist < 0.8) {
        pc++;
        if (dist != 0) avoidanceAcceleration -= difference / (dist) * (1 - dist / 0.8);
      }
    }
    if (pc) avoidanceAcceleration /= pc;
    f.velocity += avoidanceAcceleration * delta_t * flee; 


    /* Velocity Normalized */

    if (f.velocity.norm() > 0.9) {
      f.velocity = f.velocity / f.velocity.norm() * 9 / 10;
    }

    if (f.velocity.norm() < 0.1) {
      f.velocity = f.velocity / f.velocity.norm() / 10;
    }
  }


  // TODO (Part 3): Handle collisions with other primitives.

  for (Predator &p : predators) {
    p.position += delta_t * p.velocity;
    p.prev_velocity = p.velocity;
    p.velocity += delta_t * ground->collide(p) * collision;
    for (CollisionObject* object : *collision_objects) {
      p.velocity += delta_t * object->collide(p) * collision;
    }
  }


  for (Predator &p : predators) {

    /* Hunt */
    if (p.restTime > 0) {
      p.velocity -= p.velocity / p.velocity.norm() * delta_t * 5;
      p.restTime--;
    } else {
      int pc = 0;
      Vector3D huntAcceleration = Vector3D();
      if (p.lockedInTime > 0 && p.lockedFish != nullptr) {
        Vector3D difference = p.lockedFish->position - p.position;
        huntAcceleration = difference / difference.norm();
        p.lockedInTime--;
        if (!p.lockedFish->alive) {
          p.lockedFish = nullptr;
          p.lockedInTime = 0;
        }
      } else {
        Fish* closestFish = nullptr;
        float minDist = 1;
        for (Fish &f : fishes) {
          if (!f.alive) continue;
          Vector3D difference = f.position - p.position;
          float dist = difference.norm();
          if (dist < minDist) {
            closestFish = &f;
            minDist = dist;
            huntAcceleration = difference / difference.norm();
            p.lockedInTime = 3000;
          }
        }
        p.lockedFish = closestFish;
      }
      if (p.lockedFish != nullptr && (p.lockedFish->position - p.position).norm() < 0.01) {
        p.lockedFish->alive = false;
        p.lockedFish->deadTime = 24000;
        p.lockedFish = nullptr;
        p.lockedInTime = 0;
        p.restTime = 12000;
      }
      p.velocity += huntAcceleration * delta_t * hunt;
    }

    /* Wander */
    if (p.personalTime >= 3000) {
      p.replenishHermitePoints();
      p.personalTime = 0;
    } else {
      p.personalTime++;
    }

    Vector3D A((2 * p.hermitePoints[0] - 2 * p.hermitePoints[1] + p.hermitePoints[2] + p.hermitePoints[3]));
    Vector3D B((-3 * p.hermitePoints[0] + 3 * p.hermitePoints[1] - 2 * p.hermitePoints[2] - p.hermitePoints[3]));
    Vector3D C(p.hermitePoints[2]);
    Vector3D D(p.hermitePoints[0]);

    float t = p.personalTime / 3000;
    Vector3D wanderAcceleration = A * t * t * t + B * t * t + C * t + D;
    wanderAcceleration.y *= 0.2;

    p.velocity += wanderAcceleration * delta_t * 5;

    /* Boundary */
    Vector3D boundaryAcceleration = Vector3D();
    if (p.position.x < -width / 2) {
      boundaryAcceleration.x += -width / 2 - p.position.x;
    }
    if (p.position.y < -height / 2) {
      boundaryAcceleration.y += -height / 2 - p.position.y;
    }
    if (p.position.z < -width / 2) {
      boundaryAcceleration.z += -width / 2 - p.position.z;
    }
    if (p.position.x > width / 2) {
      boundaryAcceleration.x +=  width / 2 - p.position.x;
    }
    if (p.position.y > height / 2) {
      boundaryAcceleration.y +=  height / 2 - p.position.y;
    }
    if (p.position.z > width / 2) {
      boundaryAcceleration.z +=  width / 2 - p.position.z;
    }

    p.velocity += boundaryAcceleration * delta_t;


    /* Velocity Normalized */

    if (p.velocity.norm() > 1.2) {
      p.velocity = p.velocity / p.velocity.norm() * 1.2;
    }

    if (p.velocity.norm() < 0.1) {
      p.velocity = p.velocity / p.velocity.norm() * 0.1;
    }
  }

  // TODO (Part 2): Constrain the changes to be such that the spring does not change
  // in length more than 10% per timestep [Provot 1995].

}

void Cloth::build_spatial_map() {
  for (const auto &entry : map) {
    delete(entry.second);
  }
  map.clear();

  // TODO (Part 4): Build a spatial map out of all of the point masses.

}

void Cloth::self_collide(PointMass &pm, double simulation_steps) {
  // TODO (Part 4): Handle self-collision for a given point mass.

}

float Cloth::hash_position(Vector3D pos) {
  // TODO (Part 4): Hash a 3D position into a unique float identifier that represents membership in some 3D box volume.

  return 0.f; 
}

///////////////////////////////////////////////////////
/// YOU DO NOT NEED TO REFER TO ANY CODE BELOW THIS ///
///////////////////////////////////////////////////////

void Cloth::reset() {
  Fish *f = &fishes[0];
  for (int i = 0; i < fishes.size(); i++) {
    f->position = f->start_position;
    f->velocity = f->start_velocity;
    f++;
  }
  Predator *p = &predators[0];
  for (int i = 0; i < predators.size(); i++) {
    p->position = p->start_position;
    p->velocity = p->start_velocity;
    p++;
  }
}
