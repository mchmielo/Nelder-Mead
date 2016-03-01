#include "parserLib.h"
string expression;
bool flag;

void ListVar(const mu::ParserBase &parser)
{
    // Query the used variables (must be done after calc)
    mu::varmap_type variables = parser.GetVar();
    if (!variables.size())
        return;

    cout << "\nParser variables:\n";
    cout <<   "-----------------\n";
    cout << "Number: " << (int)variables.size() << "\n";
    varmap_type::const_iterator item = variables.begin();
    for (; item!=variables.end(); ++item)
        mu::console() << _T("Name: ") << item->first << _T("   Address: [0x") << item->second << _T("]\n");
}

value_type* AddVariable(const char_type *a_szName, void *a_pUserData)
{
    static value_type afValBuf[50];
    static int iVal = -1;
    if(flag){
        iVal = -1;
        flag = 0;
    }

    ++iVal;

//    mu::console() << _T("Generating new variable \"")
//                  << a_szName << std::dec << _T("\" (slots left: ")
//                  << 49-iVal << _T(")")
//                  << _T(" User data pointer is:")
//                  << std::hex << a_pUserData <<endl;
    afValBuf[iVal] = 0;

    if (iVal>=49)
        throw mu::ParserError( _T("Variable buffer overflow.") );
    else
        return &afValBuf[iVal];
}
void readExpression(vector<variable> &variables){
    mu::Parser  parser;
    flag = 1;
    // Define the variable factory
    parser.SetVarFactory(AddVariable, &parser);
    parser.SetExpr(expression);

    // Get the number of variables
    varmap_type varMap = parser.GetUsedVar();
    // Get the number of variables
    varmap_type::const_iterator item = varMap.begin();
    // Query the variables
    for (; item!=varMap.end(); ++item)
    {
        variable V;
        V.name = item->first;
        variables.push_back(V);
    }
}

double calcOneVal(vector<string> &variableNames, vector<double> &variables){
    mu::Parser  parser;
    flag = 1;
    parser.SetVarFactory(AddVariable, &parser);

    for(int i = 0; i < static_cast<int>(variables.size()); ++i){
        string tmp1,tmp2;
        char dToStr[24];
        stringstream s;
        sprintf (dToStr, "%0.2f", variables[i]);
        s << dToStr;
        s >> tmp2;
        tmp1 = variableNames[i] + "=" + tmp2;
        parser.SetExpr(tmp1);
        parser.Eval();
    }
    parser.SetExpr(expression);
    return parser.Eval();

}

void rCalc(mu::Parser &parser, int n, vector<variable> &variables){
    if( n < (int)variables.size()-1 ){
        for(int i = 0; i <= variables[n].getPrecision(); ++i){
            string tmp1;
            char dToStr[24];
            stringstream s;
            string tmp2;
            sprintf (dToStr, "%0.2f", variables[n].values[i]);
            s << dToStr;
            s >> tmp2;
            tmp1 += variables[n].name + "=" + tmp2;
            parser.SetExpr(tmp1);
            parser.Eval();
//            cout << "Tmp1 = " << tmp1 << ", parser response:" << parser.Eval() << endl;
            rCalc(parser,n+1, variables);
            if(n == (int)(variables.size()-2) ){
                parser.SetExpr(expression);
                double ans = parser.Eval();
//                cout << "n = " << n << ", wynik: " << ans << endl;
                variables[variables.size()-1].values.push_back(ans);
//                if(n==0)
//                    cout << "i = " << i;
//                else
//                    cout << "j = " << i << endl;
            }
        }

    }
}

RowVectorXd rCalc(mu::Parser &parser, MatrixXd &m, vector<variable> &variables){
    string tmp1, tmp2;
    char dToStr[24];
    RowVectorXd v( static_cast<int>(m.rows()) );
    for(int i = 0; i < m.rows(); ++i){
        for(int j = 0; j < m.cols(); ++j){
            tmp1 = ""; tmp2 = "";
            stringstream s;
            sprintf (dToStr, "%0.2f", m(i,j));
            s << dToStr;
            s >> tmp2;
            tmp1 = variables[j].name + "=" + tmp2;
//            cout << "Tmp1 = " << tmp1 << endl;
            parser.SetExpr(tmp1);
            parser.Eval();
//            cout << "parser response to " << expression << " is: "  << parser.Eval() << endl;
        }
        parser.SetExpr(expression);
        v(i) = parser.Eval();
        }
    return v;
}

RowVectorXd TargetFunc(MatrixXd m){
    mu::Parser  parser;
    flag = 1;
    vector<variable> variables;
    readExpression(variables);

    // Define the variable factory
    parser.SetVarFactory(AddVariable, &parser);
    parser.SetExpr(expression);
    parser.Eval();
    return rCalc(parser, m, variables);
}

void setVariables(vector<variable> &variables){
    vector<variable>::iterator iter = variables.begin();
    int count = 1;
    for(iter = variables.begin() ;iter != variables.end(); ++iter){

        if(count < static_cast<int>(variables.size())){   //przedzialy
            for(int j = 0; j <= (*iter).getPrecision(); ++j){
                (*iter).values.push_back( (((*iter).rangeTo-(*iter).rangeFrom)/static_cast<double>((*iter).getPrecision()))*(static_cast<double>(j)) + (*iter).rangeFrom );
            }
        }
        count++;
    }
}

void Calc(vector<variable> &variables)
{
    mu::Parser  parser;
    flag = 1;
    // Define the variable factory
    parser.SetVarFactory(AddVariable, &parser);
    vector<variable>::iterator iter = variables.begin();
    for(;iter != variables.end(); ++iter){
        parser.SetExpr((*iter).name);
    }
    setVariables(variables);
    rCalc(parser, 0, variables);

//    int temp = variables[variables.size()-1].values.size();
//    RowVectorXd Vect(temp);
//    for(int i = 0; i < static_cast<int>(variables[variables.size()-1].values.size()); ++i){
//        Vect[i]= variables[variables.size()-1].values[i];
//    }
//    return Vect;
}


double Stop(MatrixXd input)
{
    return (input.rowwise() - input.colwise().sum() / input.cols()).rowwise().norm().maxCoeff();
}

void setVarRanges(vector<variable> &variables, vector<MatrixXd> simplexStates){
    vector<MatrixXd>::iterator iter = simplexStates.begin();
    for(;iter != simplexStates.end(); ++iter){
        VectorXd tmpMin((*iter).cols()), tmpMax((*iter).cols());
        tmpMin = (*iter).colwise().minCoeff();
        tmpMax = (*iter).colwise().maxCoeff();
        for(int i = 0; i < tmpMin.cols(); ++i){
            if(i == 0 && iter == simplexStates.begin()){
                variables[i].rangeFrom = tmpMin[i];
                variables[i].rangeTo = tmpMax[i];
            }
            if(tmpMin[i] < variables[i].rangeFrom )
                variables[i].rangeFrom = tmpMin[i];
            if(tmpMax[i] < variables[i].rangeFrom )
                variables[i].rangeTo = tmpMax[i];
        }
    }
    vector<variable>::iterator iter2 = variables.begin();
    for(; iter2 != variables.end(); ++iter2){
        (*iter2).rangeFrom -= ((*iter2).rangeTo - (*iter2).rangeFrom)*0.1;
        (*iter2).rangeTo += ((*iter2).rangeTo - (*iter2).rangeFrom)*0.1;
    }
}

double minSearch(vector<variable> &variables, int index){
    double min = variables[index].values[0];
    for(int i = 0; i < static_cast<int>(variables[index].values.size()); ++i){
        if(min > variables[index].values[i])
            min =  variables[index].values[i];
    }
    return min;
}

double maxSearch(vector<variable> &variables, int index){
    double max = variables[index].values[0];
    for(int i = 0; i < static_cast<int>(variables[index].values.size()); ++i){
        if(max < variables[index].values[i])
            max =  variables[index].values[i];
    }
    return max;
}
