#ifndef QUADTREE_H
#define QUADTREE_H
#include <stdbool.h>
#include <raylib.h>

typedef struct Partition {
  Vector2* center;
  int width;
  int height;
} Partition;

typedef struct QuadTree {
  struct Partition* boundary;
  int capacity;
  struct GameObject** points;
  int members;
  struct QuadTree* ne;
  struct QuadTree* nw;
  struct QuadTree* se;
  struct QuadTree* sw;
} QuadTree;

// creates a point from (x,y) couple
Vector2* create_point(int x, int y);
// creates a partition from a center point, a width and a length
struct Partition* create_partition(Vector2* p, int width, int height);
// check if a point coords is contained inside a partition
bool contains_partition(struct Partition* part, Vector2* pt);
// creates a quadtree from a partition with a defined capacity
struct QuadTree* create_quadtree(struct Partition* boundary, int capacity);
// inserts a point inside a quadtree
void insert_quadtree(struct QuadTree* q, GameObject* go);
// removes a point from a quadtree if it exists
void remove_quadtree(struct QuadTree* q, Vector2* p);
// subdivides a quadtree into 4 sub partitions
void subdivide_quadtree(struct QuadTree* q);
// checks if a quadtree contains a point
bool contains_quadtree(struct QuadTree* q, Vector2* p);
// queries points from a partition inside a quadtree
struct Vector* query_quadtree(struct QuadTree* q, struct Partition* p);
// checks if two partitions intersects eachother
bool intersects_partition(struct Partition* a, struct Partition *b);
void partition_free(struct Partition *p);
void free_quadtree(struct QuadTree* q);
#endif
