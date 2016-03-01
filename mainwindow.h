#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qcustomplot.h"
#include <vector>
#include <string>
#include "variable.h"
#include "graph.h"
using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setupColorMap(vector<variable> &variables/*QCustomPlot *customPlot*/);
    void setupPlot2D(vector<variable> &variables);


private slots:
    void on_expressionIn_textChanged(const QString &arg1);

    void on_StartButton_clicked();

    void on_expressionIn_returnPressed();

    void on_sliderPrecision_valueChanged(int value);

private:
    graph *g_pointer;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
