#include <stdbool.h>
#include <raylib.h>

typedef struct GameObject {
  Vector2* position;
  Vector2* movement;
  bool contaminated;
  int potency;
} GameObject;

