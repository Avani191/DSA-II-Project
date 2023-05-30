
typedef struct individual{                 //Strucure of an individual
    int solution[10];   //Solution vector, the cities are traversed in order
    double fitness;             //Quantifies the goodness of the above solution
} individual;

typedef struct graph{
    int **a;
    int n;

}graph;

typedef struct vector {
    int *items;
    int capacity;
    int total;
} vector;
 

#ifndef VECTOR_H
#define VECTOR_H
 
#define VECTOR_INIT_CAPACITY 4
 
#define VECTOR_INIT(vec) vector vec; vector_init(&vec)
#define VECTOR_ADD(vec, item) vector_add(&vec, (void *) item)
#define VECTOR_SET(vec, id, item) vector_set(&vec, id, (void *) item)
#define VECTOR_GET(vec, type, id) (type) vector_get(&vec, id)
#define VECTOR_DELETE(vec, id) vector_delete(&vec, id)
#define VECTOR_TOTAL(vec) vector_total(&vec)
#define VECTOR_FREE(vec) vector_free(&vec)

void vector_init(vector *);
int vector_total(vector *);
static void vector_resize(vector *, int size);
void vector_add(vector *, int n);
void vector_set(vector *, int, void *);
int vector_get(vector *v, int index);
void vector_delete(vector *, int);
void vector_free(vector *);
#endif

 

void init_G(graph *g,vector v,int size,char *filename);
void display_G(graph g);
void traverse(graph g,int vertex);
int degree(graph g,int vertex);
void degofall(graph g);
int isConnected(graph g);
int isadjacent(graph g,int vertex,int adjacent);
void components(graph g);


void init(individual *v,int num);
void debug(char *);
//double min(individual *); 
void generate_initial_population(individual *population,vector v, int CITY_COUNT);                 // Generates casually the initial population, creating random solutions
//(as we are giving dist between two cities, thus no need to count it)double get_distance(city *, int, int);                          // Returns the distance between two cities, given a cities_map
double * evaluate_fitness(graph g, individual * population,int size);                // Computes the various fitness values for every individual in the input, given a cities_map
int * selection(double *);                                      // 
individual * crossover(individual * population, int * candidates,int CITY_COUNT);                    // 
void mutation(individual *,int CITY_COUNT);                                    // Randomically changes some solutions (swapping two elements within a solution) based on the likelihood
void cycle_crossover(individual * population, int id_1, int id_2, int * child_1, int * child_2,int CITY_COUNT);     // Computes the cycle crossover alghoritm generating two new solutions (int *), given two parent IDs (int, index of individual *)
void swap(int *, int *);                                        // Swaps two cities within a solution
int highest_fitness(individual *);   
int factorial(int n);                           // Returns the index of the individual with the best fitness value

