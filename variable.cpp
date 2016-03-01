#include "variable.h"

variable::variable(){
    rangeFrom = -1;
    rangeTo = 1;
}
variable::variable(double from, double to, int prec) : rangeFrom(from), rangeTo(to){
    precision = prec;
}

variable::~variable(){
    if(values.size() != 0){
        values.clear();
    }
}
void variable::setPrecision(int prec){
    precision = prec;
}

int variable::getPrecision(){
    return precision;
}
