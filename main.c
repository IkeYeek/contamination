#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <raylib.h>

#include "gameobject.h"
#include "quadtree.h"
#include "vector.h"

#define WIN_WIDTH 500
#define WIN_HEIGHT 500
#define CONTAMINATION_RADIUS 6 
#define QUADTREE_CAP 32 

void gen_rand_move(Vector2* vec) {
  int dir_x = 0;
  int dir_y = 0;
  do {
    dir_x = (random() % 3) - 1;
    dir_y = (random() % 3) - 1;
  } while (dir_x == 0 && dir_y == 0);
  vec->x = dir_x;
  vec->y = dir_y;
}

GameObject* init_go(int x, int y, bool contaminated) {
  GameObject* go_to_create = calloc(1, sizeof(GameObject));
  if (go_to_create == NULL) exit(1);
  go_to_create->position = calloc(1, sizeof(Vector2));
  if (go_to_create->position == NULL) exit(1);
  go_to_create->movement = calloc(1, sizeof(Vector2));
  if (go_to_create->movement == NULL) exit(1);

  go_to_create->position->x = x;
  go_to_create->position->y = y;
  go_to_create->potency = 3;

  Vector2 dir;
  gen_rand_move(&dir);

  go_to_create->movement->x = dir.x;
  go_to_create->movement->y = dir.y;

  go_to_create->contaminated = contaminated; 
  return go_to_create;
}

void compute_next_step(Vector* game_objects_vector) {
  Vector2* screen_center = create_point(WIN_WIDTH / 2, WIN_HEIGHT / 2);
  Partition* screen_boundary = create_partition(screen_center, WIN_WIDTH, WIN_HEIGHT);
  QuadTree* game_objects_quadtree = create_quadtree(screen_boundary, QUADTREE_CAP);

  for (int i = 0; i < game_objects_vector->members; i += 1) {
    GameObject* current_go = vector_get(game_objects_vector, i);
    if (random() % 1000 == 3) {
      Vector2 rand_move;
      gen_rand_move(&rand_move);
      current_go->movement->x = rand_move.x;
      current_go->movement->y = rand_move.y;
    }
    current_go->position->x += current_go->movement->x;
    current_go->position->y += current_go->movement->y;
    if (current_go->position->x <= 0 || current_go->position->x >= WIN_WIDTH) current_go->movement->x = -current_go->movement->x;
    if (current_go->position->y <= 0 || current_go->position->y >= WIN_HEIGHT) current_go->movement->y = -current_go->movement->y;

    insert_quadtree(game_objects_quadtree, current_go);
  }

  for (int i = 0; i < game_objects_vector->members; i += 1) {
    GameObject* current_go = vector_get(game_objects_vector, i);
    if (current_go->contaminated && (random() % current_go->potency) == 0) {
      Vector2* reach_center = create_point(current_go->position->x, current_go->position->y);
      Partition* reach_partition = create_partition(reach_center, CONTAMINATION_RADIUS, CONTAMINATION_RADIUS);
      Vector* points_in_reach = query_quadtree(game_objects_quadtree, reach_partition);
      for (int j = 0; j < points_in_reach->members && current_go->potency > 0; j += 1) {
        GameObject* to_contaminate = vector_get(points_in_reach, j);
        if (to_contaminate == current_go || to_contaminate->contaminated) continue;
        to_contaminate->contaminated = true;
        printf("%p contaminated %p \n", current_go, to_contaminate);
/*         current_go->potency -= 1; */
      }
      vector_free(points_in_reach);
      partition_free(reach_partition);
      free(reach_center);
    }
  }

  free_quadtree(game_objects_quadtree);
  partition_free(screen_boundary);
  free(screen_center);
}

int main() {
  Vector* game_objects_vector = create_vector();

  Vector2 screen_center = { .x = (int)(WIN_WIDTH / 2), .y = (int)(WIN_HEIGHT / 2)};
  Partition screen_boundary = { .center = &screen_center, .width = WIN_WIDTH, .height = WIN_HEIGHT};
  QuadTree* init_qt = create_quadtree(&screen_boundary, QUADTREE_CAP);

  for (int i = 0; i < 60000; i += 1) {
    int x, y;
    x = random() % WIN_WIDTH;
    y = random() % WIN_HEIGHT;
    GameObject* go = init_go(x, y, false);
    insert_quadtree(init_qt, go);
    vector_add(game_objects_vector, go);
  }

  free_quadtree(init_qt);

  InitWindow(WIN_WIDTH, WIN_HEIGHT, "contamination"); 
  SetTargetFPS(150);
  while(!WindowShouldClose()) {
    int mouse_button_pressed;
    if ((IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && ((mouse_button_pressed = MOUSE_BUTTON_LEFT) || true)) // they don't allow bitmasks so I kind of made my own..... the || true avoids evaluating the value of 0
      || (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) && (mouse_button_pressed = MOUSE_RIGHT_BUTTON))) {
      int mouse_x, mouse_y;
      mouse_x = GetMouseX();
      mouse_y = GetMouseY();
      GameObject* go_to_create = init_go(mouse_x, mouse_y, (mouse_button_pressed == MOUSE_BUTTON_RIGHT));
      vector_add(game_objects_vector, go_to_create);
    }
    BeginDrawing();
    DrawFPS(0, 0);
    ClearBackground(BLACK);

    for (int i = 0; i < game_objects_vector->members; i += 1) {
      GameObject* current_go = vector_get(game_objects_vector, i);
      DrawCircle(current_go->position->x, current_go->position->y, 2, current_go->contaminated ? RED : GREEN);
    }

    EndDrawing();
    compute_next_step(game_objects_vector); 
  }

cleanup:
  CloseWindow();
  for (int i = 0; i < game_objects_vector->members; i += 1) {
    GameObject* curr = vector_get(game_objects_vector, i);
    free(curr->position);
    free(curr->movement);
    free(curr);
  }
  vector_free(game_objects_vector);
  return EXIT_SUCCESS;

alloc_error: 
  fprintf(stderr, "allocation error\n");
  goto cleanup;
}

