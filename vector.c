#include <stdlib.h>
#include <stdio.h>
#include "string.h" 
#include "vector.h"

struct Vector* create_vector() {
  struct Vector* v = calloc(1, sizeof(struct Vector));
  if (v == NULL) {
    fprintf(stderr, "allocation error in create_vector\n");
    exit(EXIT_FAILURE);
  }
  v->cap = DEF_CAP;
  v->members = 0;
  v->elems = calloc(DEF_CAP, sizeof(void*));
  if (v->elems == NULL) {
    fprintf(stderr, "allocation error in create_vector (2)\n");
    exit(EXIT_FAILURE);
  }
  return v;
}

void vector_add(struct Vector* v, void* e) {
  if (v->members + 1 >= v->cap) {
    v->cap += DEF_CAP;
    v->elems = realloc(v->elems, sizeof(void*) * v->cap);
  }
  v->elems[v->members] = e;
  v->members += 1;
}

void* vector_get(struct Vector* v, int idx) {
  if (idx >= 0 && idx < v->members) {
    return v->elems[idx];
  } else {
    fprintf(stderr, "trying to access illegal index in vector_get (idx: %d, size: %d)\n", idx, v->members);
    exit(1);
  }
}

void vector_delete(struct Vector* v, int idx) {
  if (idx > 0 && idx < v->members) {
   /*  free(v->elems[idx]); */
    for (int i = idx; i < v->members - 1; i += 1) {
      v->elems[i] = v->elems[i+1];
    }
    v->members -= 1;
  }
}

void vector_merge(struct Vector* to, struct Vector* from) {
  if (from == NULL || to == NULL) {
    return;
  }
  if (to->cap < to->members + from->members) {
     to->cap = to->members + from->members;
    to->elems = realloc(to->elems, sizeof(void*) * to->cap);
    if (to->elems == NULL) {
      fprintf(stderr, "allocation error in vector_merge\n");
      exit(EXIT_FAILURE);
    }
  }
  memcpy(to->elems+to->members, from->elems, sizeof(void*) * from->members); 
  to->members += from->members;
}

void vector_free(struct Vector* v) {
  free(v->elems);
  free(v);
}
