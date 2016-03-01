#ifndef VARIABLE_H
#define VARIABLE_H
#include <vector>
#include <string>
using namespace std;

class variable{

public:
    string name;
    std::vector<double> values;
    double rangeFrom;
    double rangeTo;
    static int precision;


    variable();
    variable(double from, double to, int prec);
    ~variable();
    static int getPrecision();
    static void setPrecision(int prec);
};

#endif // VARIABLE_H
