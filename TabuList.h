#pragma once
class TabuList
{
public:
    TabuList(int,int);
    ~TabuList();
    void tabuMove(int x, int y, int method);
    bool isTabu(int x, int y, int method);
    void tabuDecrement();
private:
    int tabuSize, solSize;
    int*** tabu;
};

