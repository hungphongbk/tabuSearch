#pragma once
#include<cstdio>
#include<cmath>
#include<cstdlib>
#include<ctime>

#include "Solution.h"
using namespace std;

#define inputfile "input.txt"

Solution* s;

int printMenu();
void createTest();
void initFirstSol();
void calculateSol();
int main(){
    int cn = 0;
    do{
        cn = printMenu();
        switch (cn){
        case 1:
            createTest();
            break;
        case 2:
            initFirstSol();
            break;
        case 3:
            calculateSol();
            break;
        default:
            break;
        }
        printf("\n");
    } while (cn != 0);
    return 0;
}

/*IMPLEMENTS*/
int printMenu(){
    printf("1: Sinh test ngau nhien [DEPRECATED]\n");
    printf("2: Doc du lieu\n");
    printf("3: Tinh toan\n");
    printf("0: Thoat\n");
    printf("Nhap so de chon chuc nang: ");
    int n = 0; scanf("%d", &n);
    return n;
}
void createTest(){
    FILE *f;
    f = fopen(inputfile, "wt");
    int* arr = new int[4];
    printf("Nhap so luong xe, tai trong toi da va khoang cach toi da: ");
    scanf("%d%d%d", arr, arr + 1, arr + 2);
    printf("Nhap so luong diem den: ");
    scanf("%d", arr + 3);
    fprintf(f, "%d %d %d %d\n", arr[0], arr[1], arr[2], arr[3]);
    srand(time(NULL));
    for (int i = 0; i < arr[3]; i++)
        fprintf(f, "%d ", rand() % 100);
    int n = arr[3];
    //Xu ly cac diem sinh ra trung nhau
    for (int i = 0; i <= n; i++){
        arr[0] = rand() % 50;
        arr[1] = rand() % 50;
        fprintf(f,"\n%d %d", arr[0], arr[1]);
    }
    fclose(f);
}
void initFirstSol(){
    if (s!=NULL) delete s;
    s = new Solution();
    s->readFromExcel("cluster.xls");
}
void calculateSol(){
    if (s==NULL)
    {
        printf("Chua khoi tao input!");
        return;
    }
    s->createInitialSolution();
    s->iterate(100,"output.txt");
}