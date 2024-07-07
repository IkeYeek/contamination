#ifndef VECTOR_H
#define VECTOR_H
#define DEF_CAP 16
typedef struct Vector {
  int cap;
  int members;
  void** elems;
} Vector;

struct Vector* create_vector();
void vector_add(struct Vector* v, void* e);
void* vector_get(struct Vector* v, int idx);
void vector_delete(struct Vector* v, int idx);
void vector_merge(struct Vector* to, struct Vector* from);
void vector_free(struct Vector* v);
#endif
