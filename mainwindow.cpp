
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "neldermead.h"
#include <string>
#include <iostream>
#include "parserLib.h"
#include "graph.h"



//eigen
#include <Eigen/Dense>
using namespace Eigen;

#include <cstdlib>
#include <cstring>
#include <cmath>

#include <locale>
#include <limits>
#include <ios>
#include <iomanip>
#include <numeric>

using namespace std;

extern string expression;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    g_pointer = NULL;
    ui->setupUi(this);
    stringstream s;
    s << variable::getPrecision();
    string tmp;
    s >> tmp;
    ui->lDokladnosc->setText("Dokładność: " + QString::fromStdString(tmp) );
}

MainWindow::~MainWindow()
{
    if(g_pointer != NULL)
        delete g_pointer;
    delete ui;
}

void MainWindow::on_expressionIn_textChanged(const QString &arg1)
{
    expression = arg1.toLocal8Bit().constData();
}

void MainWindow::on_StartButton_clicked()
{
    if(g_pointer != NULL){
        delete g_pointer;
    }
    vector<variable> variables;
    readExpression(variables);


    Vector6d v6d(6);
    v6d(0) = 1; v6d(1) = 2; v6d(2) = 0.5; v6d(3) = 0.5; v6d(4) = 0.1; v6d(5) = static_cast<double>(variables.size()-1);
    NelderMead nm(&TargetFunc,&Stop,v6d);
    nm.Run();

    g_pointer = new graph();
    setVarRanges(variables, nm.States());
    Calc(variables);
    variables[variables.size()-1].rangeFrom = minSearch(variables, variables.size()-1);
    variables[variables.size()-1].rangeTo = maxSearch(variables, variables.size()-1);
    variables[variables.size()-1].rangeFrom -= 0.1*(variables[variables.size()-1].rangeTo-variables[variables.size()-1].rangeFrom);
    variables[variables.size()-1].rangeTo += 0.1*(variables[variables.size()-1].rangeTo-variables[variables.size()-1].rangeFrom);
    RowVectorXd rV(variables.size()-1);
    rV = nm.MinPoint();
    if((int)variables.size() == 2){
        g_pointer->setupPlot2D(variables, nm.States());

    }
    else if((int)variables.size() == 3){
        g_pointer->setupColorMap(variables, rV);
    }
    g_pointer->show();



}

void MainWindow::on_expressionIn_returnPressed()
{
    on_StartButton_clicked();
}

void MainWindow::on_sliderPrecision_valueChanged(int value)
{
    variable::setPrecision(value);
    stringstream s;
    s << variable::getPrecision();
    string tmp;
    s >> tmp;
    ui->lDokladnosc->setText("Dokładność: " + QString::fromStdString(tmp) );
}
