#include "SolutionContent.h"
#include <cstdio>

SolutionContent::SolutionContent(SolutionContent* source){
    destCount = source->destCount;
    carCount = source->carCount;
    
    g = new int*[carCount];
    routeSize = new int[carCount];
    for (int i = 0; i < carCount; i++){
        g[i] = new int[destCount + 3];
        routeSize[i] = source->routeSize[i];
    }
    for (int i = 0; i < carCount; i++)
        for (int j = 0; j < destCount + 3; j++)
            g[i][j] = source->g[i][j], g[i][0] = 0;
}
void SolutionContent::init(){
    g = new int*[carCount];
    routeSize = new int[carCount];
    for (int i = 0; i < carCount; i++){
        g[i] = new int[destCount + 3];
        routeSize[i] = 1;
    }
    for (int i = 0; i < carCount; i++)
        for (int j = 0; j < destCount + 3; j++)
            g[i][j] = -1, g[i][0] = 0;
}
void SolutionContent::addDestToRoute(int car,int dest){
    g[car][routeSize[car]] = dest;
    routeSize[car]++;
}
void SolutionContent::removeLastDestToRoute(int car){
    g[car][routeSize[car]] = -1;
    routeSize[car]--;
}
void SolutionContent::toString(FILE *f){
    for (int i = 0; i < carCount; i++)if (routeSize[i]>1){
        fprintf(f,"\nThe route of car %d:", i + 1);
        for (int j = 0; j < routeSize[i]; j++)
            fprintf(f," %d", g[i][j]);
    }
    fprintf(f,"\n");
}
bool SolutionContent::checkContent(){
    for (int i = 0; i < carCount; i++)if (routeSize[i]>1)
        for (int j = 0; j < routeSize[i]; j++)
                if (g[i][j]==-1) return false;
    return true;
}