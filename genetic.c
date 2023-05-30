#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "genetic.h"
#include "time.h"
#include <string.h>
//#include "vector.h"
// #include "graph.h"

#define POPULATION_COUNT 10
#define MUTATION_LIKELIHOOD 0.05
#define DEBUG 0


int factorial(int n)  
{  
    if(n>5){
        return 200;
    } 
    else{
        if (n == 0)  
    return 1;  
  else  
    return(n * factorial(n-1));
    }
}  
void debug(char *message)
{
    if (DEBUG)
        printf("[D] %s\n", message);
}

void swap(int *a, int *b)
{
    int t = *a;
    *a = *b;
    *b = t;
}



int highest_fitness(individual *population)
{

    int index = 0;
    double initial = population[0].fitness;
    for (int j = 1; j < POPULATION_COUNT; j++)
    {
        if (population[j].fitness > initial)  // Finds the highest fitness
        { 
            initial = population[j].fitness;
            index = j;
        }
    }
    return index;
}

double get_distance(graph g, int id_1, int id_2)
{
    int i,j=0;
     i=id_1;
     j=id_2;
    int skip=0;
    FILE *fp=NULL;
    rewind(fp);
    fp=fopen("try.txt","r");
    int num;
    if(fp==NULL){
        printf("File cannot open\n");
        exit(0);
    }
    else{
        if(i==0){
                fseek(fp,j*3,SEEK_SET); 
                fscanf(fp,"%d",&num); ;
                rewind(fp);
        }
        else{
              skip=i*32 + j*3;
             fseek(fp,skip,SEEK_SET);
            fscanf(fp,"%d",&num);  //between double digits 3bits ka gap 
        //  printf("num  %d\n",num);
            rewind(fp);
        }  
       
    }
    fclose(fp);
    return num;
}

double *evaluate_fitness(graph g, individual *population, int size)
{

    int id_1, id_2;

    double total, fitness_sum = 0;
    double *cdf = (double *)malloc(sizeof(double) * POPULATION_COUNT); // Allocate space for the cumulative distribution function of a probability distribution
    //printf("CREATED\n");
    for (int i = 0; i < POPULATION_COUNT; i++)
    {

       // printf("INSIDE FOR LOOP\n");
        total = 0; // Contains the total distances of one solution

        for (int j = 0; j < size - 1; j++)
        {
          //  printf("Inner for loop\n");
            id_1 = population[i].solution[j];
           // printf("id_1 : %d\n", id_1);
            id_2 = population[i].solution[j + 1];
           // printf("id_2 : %d\n", id_2);
            total += get_distance(g, id_1,id_2); // Caomputes the distance between cities and the total sum
           // printf("TOTAL : \n");
        }

        population[i].fitness = 1 / total;    // The fitness is 1/total
        fitness_sum += population[i].fitness; // We use the sum of all the fitness to normalize the probability distribution
    }

    cdf[0] = (population[0].fitness) / fitness_sum;

    for (int i = 1; i < POPULATION_COUNT; i++)
    {
        cdf[i] = cdf[i - 1] + population[i].fitness / fitness_sum; // Assign to cumulative distribution the value of the previous element + the current normalized fitness divided by the sum of all the fitnesses
    }

    debug("Computed fitness");

    return cdf;
}

int *selection(double *cdf)
{

    double extraction;
    int pivot, first, last;
    int *candidates = (int *)malloc(sizeof(int) * POPULATION_COUNT);

    for (int i = 0; i < POPULATION_COUNT; i++)
    {

        first = 0;
        last = POPULATION_COUNT;
        extraction = (double)rand() / RAND_MAX; // rand() generates a number in [0, RAND_MAX] therefore dividing by RAND_MAX we obtain a value in [0, 1]

        while (first != last)
        { // Binary search of the first element of the CDF that has bigger probability than the value randomly generated
            pivot = (first + last) / 2;
            if (cdf[pivot] == extraction)
            {
                break;
            }
            if (cdf[pivot] > extraction)
            {
                last = pivot;
            }
            else
            {
                first = pivot + 1;
            }
        }
        candidates[i] = first; // candidates[i] contains the index of the extracted individual, the bigger the probability, the more the index appears in candidates (so it reproduces often because it's a good solution)
    }
    free(cdf); // It does not serve any purpose anymore

    debug("Extracted candidates");

    return candidates;
}

void mutation(individual *population, int CITY_COUNT)
{
    srand(time(NULL)); // Seed the random number generator with the current time

    double mutation_probability = MUTATION_LIKELIHOOD;
    int x = 0;
    int y = 0;

    for (int i = 0; i < POPULATION_COUNT; i++)
    {
        if ((double)rand() / RAND_MAX < mutation_probability)
        {
            do
            {
                x = rand() % CITY_COUNT;
                y = rand() % CITY_COUNT;
            } while (x == y || population[i].solution[x] == population[i].solution[y]);

            swap(&population[i].solution[x], &population[i].solution[y]);
        }
    }

    debug("Mutation executed");
}


individual *crossover(individual *population, int *candidates, int CITY_COUNT)
{
    individual *new_gen = (individual *)malloc(sizeof(individual) * POPULATION_COUNT);
    memset(new_gen, 0, sizeof(individual) * POPULATION_COUNT);

    //printf("NEW GENE CREATED in crossover\n");

    for (int i = 0; i < POPULATION_COUNT; i = i + 2)

    {
       // printf("candidate : %d\n",candidates[i]);
        cycle_crossover(population, candidates[i], candidates[i + 1], new_gen[i].solution, new_gen[i + 1].solution, CITY_COUNT); // We perform the crossover two by two, choosing as parents two candidates chosen by index, then we save the offspring into new_gen
    }

   // printf("DONE FOR\n");
    free(candidates); // We don't need them anymore
    debug("Reproduction performed");

    return new_gen;
}



void generate_initial_population(individual *population, vector v, int CITY_COUNT)
{
    for (int i = 0; i < POPULATION_COUNT; i++)
    {
        for (int j = 0; j < CITY_COUNT; j++)
        {
            population[i].solution[j] = vector_get(&v, j);// Assign entries from the arr array to the solution array
        }

        // Shuffle the solution array randomly
        for (int j = CITY_COUNT - 1; j > 0; j--)
        {
            int k = rand() % (j + 1);
            int temp = population[i].solution[j];
            population[i].solution[j] = population[i].solution[k];
            population[i].solution[k] = temp;
        }
    }

    printf("Random population generation completed\n");
}


void cycle_crossover(individual *population, int id_1, int id_2, int *child_1, int *child_2, int CITY_COUNT)
{
    int parent_1 = id_1, parent_2 = id_2;           // Copy the parent indexes
    int j = 0;                                      // Current column
    int lookup[CITY_COUNT];                         // Addressing vector
    int flags[CITY_COUNT];                           // Visited columns vector
    memset(flags, 0, sizeof(int) * CITY_COUNT);     // Initialize flags array with zeros
    int visited = 0;                                // Number of visited columns
    int first = population[id_1].solution[0];
    int last;                                       // First and last element of the cycle

    for (int i = 0; i < CITY_COUNT; i++) {
        lookup[population[id_1].solution[i]] = i;   // This vector is used to obtain the position of an element of id_2 in id_1.
    }

    while (visited < CITY_COUNT) {
        child_1[j] = population[parent_1].solution[j];  // Copy the cities to the child
        child_2[j] = population[parent_2].solution[j];
        last = population[id_2].solution[j];

        flags[j] = 1;       // J becomes visited
        visited++;
        j = lookup[last];   // Search for last in id_1

        if (flags[j] == 0) {
            // If the next column has not been visited, find the next unvisited column
            while (flags[j] != 0) {
                j = lookup[population[id_2].solution[j]];
            }
        }

        if (first == last) {
            swap(&parent_1, &parent_2);          // Swap parents to copy the cities in the child in reverse order
            while (flags[j] == 1)
                j++;                   // Search for a free column
            first = population[id_1].solution[j];   // Reinitialize the first element of the cycle
        }
    }
}

