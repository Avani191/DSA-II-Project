#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
//#include "vector.h"
#include "genetic.h"
#include <limits.h>

 
void vector_init(vector *v) {                            //initialize the vector
    v->capacity = VECTOR_INIT_CAPACITY;
    v->total = 0;
    v->items = (int*)malloc(sizeof(int)*v->capacity);
}
 
int vector_total(vector *v) {
    return v->total;
}
 
static void vector_resize(vector *v, int capacity) {
#ifdef DEBUG_ON
    printf("vector_resize: %d to %d\n", v->capacity, capacity);
#endif
 
    int *items = realloc(v->items, sizeof(int) * capacity);
    if (items) {
        v->items = items;
        v->capacity = capacity;
    }
}
 
void vector_add(vector *v, int data) {
    if (v->capacity == v->total)
        vector_resize(v, v->capacity * 2);
    v->items[v->total++] = data;
}
 

 
int vector_get(vector *v, int index) {
    if (index >= 0 && index < v->total)
        return v->items[index];
    return INT_MIN;
}
 
