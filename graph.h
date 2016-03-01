#ifndef GRAPH_H
#define GRAPH_H

#include <QDialog>
#include <vector>
#include "variable.h"
#include "Eigen/Dense"
#include "parserLib.h"
using namespace Eigen;
using namespace std;

namespace Ui {
class graph;
}

class graph : public QDialog
{
    Q_OBJECT

public:
    explicit graph(QWidget *parent = 0);
    ~graph();
    void setupPlot2D(vector<variable> &variables, vector<MatrixXd> simplexStates);
    void setupColorMap(vector<variable> &variables, RowVector2d minPoint);

private:
    Ui::graph *ui;
};

#endif // GRAPH_H
