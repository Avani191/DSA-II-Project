#include <stdio.h>
#include <stdlib.h>
//#include "graph.h"
#include <string.h>
#include "genetic.h"
//#include "vector.h"

void init_G(graph *g,vector v,int n,char *filename){
     g->a=(int**)malloc(sizeof(int*)*(n));
     g->n=n; 
     int i,j;
      FILE *fp=NULL;
            fp=fopen("try.txt","r");
    for(int k=0;k<n;k++){
         g->a[k]=(int*)malloc(sizeof(int)*n);
         i=vector_get(&v,k);
    
        // printf("i : %d\n",i);
        for(int l=0;l<n;l++){
           j=vector_get(&v,l);
          // printf("j:%d\n",j);
           
            int num;
            if(fp==NULL){
                printf("File cannot open\n");
                exit(0);
            }
            else{
                if(i==0){
                fseek(fp,j*3,SEEK_SET); 
                fscanf(fp,"%d ",&g->a[k][l]);
                rewind(fp);
                 
                }
                else{
                    int  skip=i*32 + j*3;;
            fseek(fp,skip,SEEK_SET);
                
               // fscanf(fp,"%d",&num);  //between double digits 3bits ka gap 
               fscanf(fp,"%d ",&g->a[k][l]);
               rewind(fp);
                
                }  
                
            }
              
           
        }
    }
    
   
    
}

void display_G(graph g){
    for(int i=0;i<g.n;i++){
        for(int j=0;j<g.n;j++){
            printf("%d ",g.a[i][j]);
        }
        printf("\n");
    }
}

