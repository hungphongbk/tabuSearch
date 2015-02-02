#include "TabuList.h"


TabuList::TabuList(int _tabuSize,int _solutionSize)
{
    tabuSize = _tabuSize;
    solSize = _solutionSize;
    tabu = new int**[solSize];
    int i,j;
    for (i = 0; i < solSize; i++){
        tabu[i] = new int*[solSize];
        for (j = 0; j < solSize; j++){
            tabu[i][j] = new int[3];
            tabu[i][j][0] = 0;
            tabu[i][j][1] = 0;
            tabu[i][j][2] = 0;
        }
    }
}


TabuList::~TabuList()
{
    for (int i = 0; i < solSize; i++){
        for (int j = 0; j < solSize; j++)
            delete tabu[i][j];
        delete tabu[i];
    }
    delete tabu;
}

void TabuList::tabuMove(int x, int y, int method){
    tabu[x][y][method] += tabuSize;
    tabu[y][x][method] += tabuSize;
}
bool TabuList::isTabu(int x, int y, int method){
    return tabu[x][y][method] > 0;
}
void TabuList::tabuDecrement(){
    int i, j, k;
    for (i = 0; i < solSize; i++)
        for (j = 0; j < solSize; j++)
            for (k = 0; k < 3; k++)
                tabu[i][j][k] -= (tabu[i][j][k] > 0) ? 1 : 0;
}