#ifndef CLOTH_MESH_H
#define CLOTH_MESH_H

#include <vector>

#include "CGL/CGL.h"
#include "pointMass.h"
#include "fish.h"
#include "predator.h"
#include "food.h"

using namespace CGL;
using namespace std;

class Triangle {
public:
  Triangle(PointMass *pm1, PointMass *pm2, PointMass *pm3, Vector3D uv1, Vector3D uv2, Vector3D uv3)
      : pm1(pm1), pm2(pm2), pm3(pm3), uv1(uv1), uv2(uv2), uv3(uv3) {}

  // Static references to constituent mesh objects
  PointMass *pm1;
  PointMass *pm2;
  PointMass *pm3;
  
  // UV values for each of the points.
  // Uses Vector3D for convenience. This means that the z dimension
  // is not used, and xy corresponds to uv.
  Vector3D uv1;
  Vector3D uv2;
  Vector3D uv3;

  Halfedge *halfedge;
}; // struct Triangle

class Edge {
public:
  Halfedge *halfedge;
}; // struct Edge

class Halfedge {
public:
  Edge *edge;
  Halfedge *next;
  Halfedge *twin;
  Triangle *triangle;
  PointMass *pm;
}; // struct Halfedge

class ClothMesh {
public:
  ~ClothMesh() {}

  vector<Triangle *> triangles;
}; // struct ClothMesh

#endif // CLOTH_MESH_H
