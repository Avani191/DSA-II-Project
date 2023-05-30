#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <limits.h>
//#include "vector.h"
//#include "graph.h"
#include "genetic.h"
 
#define DEBUG 0              
#define POPULATION_COUNT 10         //Number of individuals   
#define MUTATION_LIKELIHOOD 0.05    //Random mutation probability
int main(){
    printf(" 0.Shaniwar Wada\n 1.Aga Khan Palace\n 2.Lal Mahal\n 3.Dagadusheth\n 4.Sinhagad Fort\n 5.khadakwasla\n 6.Rajgad Fort\n 7.Parvati hill\n 8..Ramdara Temple\n 9.Rajiv Gandhi Zoological Park\n");
    vector v;  
    vector_init(&v); 

    int num=0; 
    int final=5000;
    int idx=0;

    vector v1;
    vector_init(&v1);

    int CITY_COUNT=0;               
    int ans[CITY_COUNT];

    printf("Enter no of places you want to visit \n");
    scanf("%d",&CITY_COUNT);                              //input from user

    for(int i=0;i<CITY_COUNT;i++){
        printf("Enter the place no you want to visit:\n");
        scanf("%d",&num);
        vector_add(&v, num);
    }

    int ITERATION_COUNT = 10; 
    for (int i = 0; i < vector_total(&v); i++)           //printing vector
        printf("%d ",vector_get(&v, i));
    printf("\n");

    graph g;
    init_G(&g,v,CITY_COUNT,"try.txt");
    printf("Graph formed : \n");
    display_G(g);


    srand(time(NULL));           //Random seed initialization

    individual * population, * new_gen;

    population = (individual*)malloc(sizeof(individual) * POPULATION_COUNT); //Allocates the population, there are "POPULATION_COUNT" individuals
    if(!population) exit(0);
    
    generate_initial_population(population,v,CITY_COUNT);    //Generates casually the initial population, creating random solutions
   
    int best;
   
    int * candidates;
   
    double length;
  
    double * cdf;
    
    for (int i = 0; i < ITERATION_COUNT; i++){
      cdf = evaluate_fitness(g,population,CITY_COUNT);    //We obtain the cumulative distribution function associated to the current population
      int k=0;
      //to print all fitness
      for(int j=0;j<POPULATION_COUNT;j++){
          printf("Path : ");
          for(k=0;k<CITY_COUNT;k++){
              if(k<CITY_COUNT-1){
            printf("%d -> ",population[j].solution[k]);
              }
              else{
                    printf("%d ",population[j].solution[k]);
              }
          }
          length = 1/(population[j].fitness);
          printf(" | Total Distance : %f\n",length);
          printf("\n");
      }
      printf("\n");
      best = highest_fitness(population);     //Returns the index of the individual with the best fitness value          
      length = 1/population[best].fitness;    //The lenght of the solution is the inverse of the fitness (the fitness is defined as 1/lenght
      if(length<final){
        final=length;
        for(int k=0;k<CITY_COUNT;k++){
          int num1 = population[best].solution[k];
          ans[k]=num1;
          vector_add(&v1, population[best].solution[k]);
        }
      }
     // printf("FINAL 2 : %d\n",final);
     // printf("idx 2 : %d\n",idx);
     //   printf("Best Path : ");
      for(int j=0;j<CITY_COUNT;j++){
            if(j<CITY_COUNT-1){
                printf("%d -> ",population[best].solution[j]);
              }
            else{
                printf("%d ",population[best].solution[j]);
              }
          }
      printf("| Current generation: %d | Best Distance: %f\n", i, length);        
 
       candidates = selection(cdf);        //candidates is an integer vector containing the indexes of those that are going to reproduce
     //  printf("Candidate : %d\n",i);
       new_gen = crossover(population, candidates, CITY_COUNT); //We crossover the population to obtain a new one
     //   printf("Candidate : %d\n",i);
       free(population);
       population = new_gen;
       mutation(population,CITY_COUNT);  //We apply the eventual mutation to the population
     
       
     }
     printf("\n");
     printf("\n");
     printf("*************************************CONCLUSION *************************************\n");
     printf("\n");
     printf("\n");
     printf("Best path : ");
    
      for(int j=0;j<CITY_COUNT;j++){
       
        if(j<CITY_COUNT-1){
              printf("%d -> ",ans[j]);
                }
        else{
              printf("%d ",ans[j]);
            }
      }
     
    printf(" | Best Length : %d\n",final);
    printf("\n");
    printf("\n");
    printf("\n");
    return 0;
}
 
