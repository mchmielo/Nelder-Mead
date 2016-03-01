#ifndef PARSERLIB_H
#define PARSERLIB_H

#include <sstream>
#include <vector>
#include <string>
#include "muParserTest.h"
#include "Eigen/Dense"
#include "neldermead.h"
#include "muParser.h"
#include "variable.h"
#include <cmath>

#if defined(_WIN32) && defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define CREATE_LEAKAGE_REPORT
#endif

#if defined( USINGDLL ) && defined( _WIN32 )
#error This sample can be used only with STATIC builds of muParser (on win32)
#endif

/** \brief This macro will enable mathematical constants like M_PI. */
#define _USE_MATH_DEFINES

using namespace Eigen;
using namespace mu;
using namespace std;

extern string expression;

value_type* AddVariable(const char_type *a_szName, void *a_pUserData);
void ListVar(const mu::ParserBase &parser);

double calcOneVal(vector<string> &variableNames, vector<double> &variables);
void Calc(vector<variable> &variables);
void readExpression(vector<variable> &variables);
void rCalc(mu::Parser &parser, int n, vector<variable> &variables);
RowVectorXd rCalc(mu::Parser &parser, MatrixXd &m, vector<variable> &variables);
RowVectorXd TargetFunc(MatrixXd m);
MatrixXd makeMatrix(vector<variable> &variables);
void setVariables(vector<variable> &variables);
double Stop(MatrixXd input);
void setVarRanges(vector<variable> &variables, vector< MatrixXd> simplexStates);
double minSearch(vector<variable> &variables, int index);
double maxSearch(vector<variable> &variables, int index);
#endif // PARSERLIB_H
