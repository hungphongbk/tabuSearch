#include "Solution.h"
#include <cstdio>
#include <iostream>
#include <vector>
#include "libxl.h"
using namespace libxl;

float dist(Point* p1, Point* p2){
    float x = p1->x - p2->x;
    float y = p1->y - p2->y;
    return sqrt(x*x + y*y);
}
float calculateCarRouteLength(SolutionContent* content, float** g, int car){
    int* route = content->getRoute(car);
    int i = 1;
    float rs = 0;
    while (route[i] >= 0){
        rs += g[route[i - 1]][route[i]];
        i++;
    }
    rs += g[route[i - 1]][route[0]];
    if (rs < 0){
        int x = 0;
    }
    return rs;
}
int calculateCarWeight(SolutionContent* content, int* demands, int car){
    int* route = content->getRoute(car);
    int i = 1, rs = 0;
    while (route[i] >= 0)
        rs += demands[route[i++]];
    return rs;
}
void Solution::readFile(char* file){
    FILE *f; f = fopen(file, "rt");
    fscanf(f, "%d%d%d%d", &carCount, &carWeight, &carDist, &destCount);
    destDemands = new int[destCount];
    for (int i = 0; i < destCount; i++)
        fscanf(f, "%d", destDemands + i);
    p = new Point[destCount + 1];
    for (int i = 0; i <= destCount; i++)
        fscanf(f, "%d%d", &p[i].x, &p[i].y);
    start = p;//Tro toi phan tu dau tien - diem xuat phat
    g = new float*[destCount + 1];
    for (int i = 0; i <= destCount; i++)
        g[i] = new float[destCount + 1];
    for (int i = 0; i < destCount; i++)
        for (int j = i + 1; j <= destCount; j++)
            g[i][j] = g[j][i] = dist(p + i, p + j),
            g[i][i] = 0;

    content = new SolutionContent(destCount, carCount);
    tabu = new TabuList(10, destCount + 3);
}
std::vector<int>* getClusterDemands(Sheet* cluster){
    int col=3,row=-1,value=0;
    //find row
    Format** format=new Format*();
    do{
        cluster->readNum(++row,col,format);
    } while (*format==0);
    std::vector<int>* vector=new std::vector<int>();
    do{
        value=cluster->readNum(row++,col,format);
        if (*format==0) break;
        vector->push_back(value);
    } while (true);
    return vector;
}
float** getDistanceMatrix(Sheet* matrix,int clusterCount){
    int padding=2;
    float** rs=new float*[clusterCount+1];
    for(int i=0;i<=clusterCount;i++){
        rs[i]=new float[clusterCount+1];
        for(int j=0;j<=clusterCount;j++)
            rs[i][j]=matrix->readNum(i+padding,j+padding);
    }
    return rs;
}
void Solution::readFromExcel(char* file){
    Book* book=xlCreateBook();
    if (book){
        printf("[OUTPUT] prepare to load %s file\n",file);
        if (book->load(file)){
            //TYPE OTHER DATA BY HAND
            printf("Nhap so luong xe, tai trong toi da va khoang cach toi da cua moi xe: ");
            scanf("%d%d%d",&carCount,&carWeight,&carDist);

            Sheet* sheet=book->getSheet(0);
            //CLUSTER ANALYSIS SHEET
            if (sheet){
                std::vector<int>* cluster=getClusterDemands(sheet);
                destCount=cluster->size();
                destDemands=cluster->data();
            }
            sheet=book->getSheet(1);
            //DISTANCE MATRIX SHEET
            if (sheet)
                g=getDistanceMatrix(sheet,destCount);

            content = new SolutionContent(destCount, carCount);
            tabu = new TabuList(10, destCount + 3);

            printf("[OUTPUT] Load %s completed\n",file);
            printf("[OUTPUT] Destination count: %d\n",destCount);
        }
    }
}

bool* visited;
//g[x][0]=0
//car=so luot xe da tham gia van chuyen
//dest=so diem da di qua hien tai
bool Solution::fill(int car, int dest){
    bool found = false;
    for (int D = 1; D <= destCount; D++) if (!visited[D
    ]){
        //neu diem D chua di qua
        content->addDestToRoute(car, D);
        //printf("%d %d\n", D,car);
        visited[D] = true;
        //tinh quang duong di cua xe 'car' hien tai
        float d = calculateCarRouteLength(content, g, car);
        int w = calculateCarWeight(content, destDemands, car);
        if (d < carDist && w < carWeight){
            //quang duong 'car' di duoc nam trong khoang cho phep
            //xet xem co the di qua them 1 diem nua khong
            //truong hop dest = destCount => da di den diem cuoi cung
            if (dest == destCount)
                return true;
            else found |= fill(car, dest + 1);

        }
        else found |= fill(car + 1, dest + 1);
        visited[D] = false;
        if (found) break;
        content->removeLastDestToRoute(car);
    }
    return found;
}

void Solution::createInitialSolution(){
    visited = new bool[destCount + 1];
    for (int i = 0; i <= destCount; i++) visited[i] = false;
    fill(0, 1);
    printf("[OUTPUT] Completed initial solution\n");
}
void insert(int* arr,int length,int pos,int value){
    for(int i=length-1;i>pos;i--)
        arr[i]=arr[i-1];
    arr[pos]=value;
}
void remove(int* arr,int length,int pos){
    for(int i=pos;i<length-1;i++)
        arr[i]=arr[i+1];
}
SolutionContent* Solution::generateNeighbor(int u, int v, int method, bool copy){
    SolutionContent* rs;
    if (copy)
        rs = new SolutionContent(content);
    else
        rs = content;

    int car_u=32767, car_v=32767, pos_u, pos_v;
    for (int i = 0; i < rs->getCarCount(); i++){
        for (int j = 0; j < rs->getDestCount() + 3; j++){
            if (rs->g[i][j] == u)
                car_u = i, pos_u = j;
            if (rs->g[i][j] == v)
                car_v = i, pos_v = j;
        }
    }
    if (car_u==32767 || car_v==32767){
        //printf("FAIL with u=%d v=%d\n",u,v);
        //rs->toString(stdout);
        //std::cin.get();
        return rs;
    }
    int tmp;
    int length=rs->getDestCount()+3;
    //printf("%d (%d %d) (%d %d)\n", method, car_u,pos_u,car_v,pos_v);
    switch (method){
    case 0:
        //Don gian la doi vi tri
        tmp = rs->g[car_u][pos_u];
        rs->g[car_u][pos_u] = rs->g[car_v][pos_v];
        rs->g[car_v][pos_v] = tmp;
        break;
    case 1:
        //Car_u ==> Car_v
        if (car_u==car_v) break;
        tmp=rs->g[car_u][pos_u];
        insert(rs->g[car_v],length,pos_v,tmp);
        rs->routeSize[car_v]++;
        remove(rs->g[car_u],length,pos_u);
        rs->routeSize[car_u]--;
        break;
    case 2:
        if (car_u==car_v) break;
        tmp=rs->g[car_v][pos_v];
        insert(rs->g[car_u],length,pos_u,tmp);
        rs->routeSize[car_u]++;
        remove(rs->g[car_v],length,pos_v);
        rs->routeSize[car_v]--;
        break;
    }
    return rs;
}
float Solution::calculateCarRoute(SolutionContent* cont){
    float rs = 0;
    for (int i = 0; i < carCount; i++)
        rs += calculateCarRouteLength(cont, g, i);
    return rs;
}
bool Solution::isValidSolution(SolutionContent* cont){
    float rs;
    int w;
    for (int i = 0; i < carCount; i++){
        rs = calculateCarRouteLength(cont, g, i);
        if (rs > carDist) return false;
        w = calculateCarWeight(cont, destDemands, i);
        if (w > carWeight) return false;
    }
    return true;
}
SolutionContent* Solution::nextStep(FILE *f){
    //get new solution from current as a copy
    fprintf(f,"Thuc hien chon lang gieng tot nhat\n");
    SolutionContent* best = new SolutionContent(content);
    float bestRoute = calculateCarRoute(best);
    float newBestRoute;
    int c1, c2, sol;
    bool first = true;

    SolutionContent* newBest = NULL;
    for (int i = 1; i <= destCount; i++)
        for (int j = 1; j <= destCount; j++) if (i != j){
        for (int k = 0; k < 3; k++){
            if (best != newBest)
                delete newBest;
            newBest = generateNeighbor(i, j, k);
            if (!newBest->checkContent()){
                newBest->toString(stdout);std::cin.get();
            }
            newBestRoute = calculateCarRoute(newBest);
            if ((newBestRoute < bestRoute || first) && (!tabu->isTabu(i, j, k))){
                first = false;
                c1 = i;
                c2 = j;
                sol = k;
                delete best;
                best = newBest;
            }
        }
        }
    if (c1 != 0){
        tabu->tabuDecrement();
        tabu->tabuMove(c1, c2, sol);
    }
    return best;
}
void Solution::iterate(int count, char* file){
    FILE *f;
    if (file == NULL) f = stdout;
    else f = fopen(file, "wt");
    SolutionContent* best = NULL;
    float bestRoute;
    float currentRoute = calculateCarRoute(content);
    content->toString(f);
    for (int i = 0; i < count; i++){
        fprintf(f, "Tong do dai hien tai: %0.5f\n", currentRoute);
        if (content != best)
            delete best;
        best = nextStep(f);
        bestRoute = calculateCarRoute(best);
        if (i%5==0) fprintf(f, "Step %d\n", i);
        if (bestRoute < currentRoute){
            best->toString(f);
            fprintf(f, "Tong do dai cua lang gieng tot nhat: %0.5f\n", bestRoute);
            fprintf(f, "Lang gieng tim duoc tot hon\n");
            currentRoute = bestRoute;
            content = best;
        }
    }
    fclose(f);
}