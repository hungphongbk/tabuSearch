#ifndef _SOL_CONTENT_
#define _SOL_CONTENT_
#include<cstdio>

class SolutionContent{
public:
    SolutionContent(int _destCount,int _carCount){
        destCount = _destCount;
        carCount = _carCount;
        init();
    }
    SolutionContent(SolutionContent* source);
    int getDestCount(){ return destCount; }
    int getCarCount(){ return carCount; }
    void addDestToRoute(int car,int dest);
    void removeLastDestToRoute(int car);
    int* getRoute(int car){ return g[car]; }
    void toString(FILE *f);
    bool checkContent();

    int** g;
    int* routeSize;
protected:
    int destCount;
    int carCount;

    void init();
};
#endif