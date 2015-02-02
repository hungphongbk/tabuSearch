#ifndef _SOLUTION_
#define _SOLUTION_
#include <cmath>
#include "SolutionContent.h"
#include "TabuList.h"
#include <cstdio>

struct Point{
    int x, y;
};
class Solution{
public:
    void readFile(char* file);
    void readFromExcel(char* file);
    void createInitialSolution();
    SolutionContent* nextStep(FILE *f);
    void iterate(int count,char* file = NULL);
    void writeFile(char* file);

    SolutionContent* content;
    TabuList* tabu;
private:
    int carCount, carWeight, carDist;
    int destCount;
    int* destDemands;
    Point* p;
    Point* start;
    float** g;

    bool fill(int car, int dest);
    SolutionContent* generateNeighbor(int u, int v, int method, bool copy = true);
    float calculateCarRoute(SolutionContent* _content);
    bool isValidSolution(SolutionContent* cont);
};
#endif