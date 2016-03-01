#include "graph.h"
#include "ui_graph.h"

graph::graph(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::graph)
{
    ui->setupUi(this);
}

graph::~graph()
{
    delete ui;
}

void graph::setupPlot2D(vector<variable> &variables, vector<MatrixXd> simplexStates){
    ui->customPlot->clearGraphs();
    ui->customPlot->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom);
    int k = 0;
   // ui->customPlot->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom);
    QVector<double> x(variables[0].getPrecision()+1), y(variables[0].getPrecision()+1); // initialize with entries 0..100
    for (int i=0; i <= variables[0].getPrecision(); ++i)
    {
      x[i] = variables[0].values[i]; // x goes from -1 to 1
      y[i] = variables[1].values[i]; // let's plot a quadratic function
    }
    QVector<double> x2(2), y2(2);
    // create graph and assign data to it:
    ui->customPlot->addGraph();
    ui->customPlot->graph(0)->setData(x, y);

    vector<string> names;
    names.push_back(variables[0].name);
    k = 1;
    for(int i = 0; i < static_cast<int>(simplexStates.size()); ++i){
        x2[0] = simplexStates[i](0,0);
        vector<double> vars;
        vars.push_back(x2[0]);
        y2[0] = calcOneVal(names, vars);
        vars.clear();
        x2[1] = simplexStates[i](1,0);
        vars.push_back(x2[1]);
        y2[1] = calcOneVal(names, vars);
        ui->customPlot->addGraph();
        ui->customPlot->graph(k)->addData(x2, y2);
        ui->customPlot->graph(k)->setPen(QPen(Qt::black));
        ui->customPlot->graph(k++)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, Qt::black, Qt::white, 3));
    }
    ui->customPlot->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, Qt::green, Qt::white, 5));
    ui->customPlot->graph(k-1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, Qt::red, Qt::white, 5));
    // give the axes some labels:
    ui->customPlot->xAxis->setLabel(variables[0].name.c_str());
    ui->customPlot->yAxis->setLabel(variables[1].name.c_str());
    // set axes ranges, so we see all data:
    ui->customPlot->xAxis->setRange(variables[0].rangeFrom,variables[0].rangeTo);
    ui->customPlot->yAxis->setRange(variables[1].rangeFrom,variables[1].rangeTo);


    ui->customPlot->replot();
}

void graph::setupColorMap(vector<variable> &variables, RowVector2d minPoint)
{
    // configure axis rect:
    ui->customPlot->clearGraphs();
    ui->customPlot->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom); // this will also allow rescaling the color scale by dragging/zooming
    ui->customPlot->axisRect()->setupFullAxesBox(true);
    ui->customPlot->xAxis->setLabel(variables[0].name.c_str());
    ui->customPlot->yAxis->setLabel(variables[1].name.c_str());

    // set up the QCPColorMap:
    QCPColorMap *colorMap = new QCPColorMap(ui->customPlot->xAxis, ui->customPlot->yAxis);
    ui->customPlot->addPlottable(colorMap);
    int nx = variables[0].getPrecision();
    int ny = variables[0].getPrecision();
    colorMap->data()->setSize(nx, ny); // we want the color map to have nx * ny data points
    colorMap->data()->setRange(QCPRange(variables[0].rangeFrom, variables[0].rangeTo), QCPRange(variables[1].rangeFrom, variables[1].rangeTo)); // and span the coordinate range -4..4 in both key (x) and value (y) dimensions
    // now we assign some data, by accessing the QCPColorMapData instance of the color map:
    double z;
    int k = 0;
    for(int i = 0; i <= variables[0].getPrecision(); ++i){
        for(int j = 0; j <= variables[0].getPrecision(); ++j){
            z = variables[2].values[k++];
//                z = variables[0].values[i]*variables[1].values[j];
                colorMap->data()->setCell(i, j, z);
        }
    }
    // add a color scale:
    QCPColorScale *colorScale = new QCPColorScale(ui->customPlot);
    ui->customPlot->plotLayout()->addElement(0, 1, colorScale); // add it to the right of the main axis rect
    colorScale->setType(QCPAxis::atRight); // scale shall be vertical bar with tick/axis labels right (actually atRight is already the default)
    colorMap->setColorScale(colorScale); // associate the color map with the color scale
    colorScale->axis()->setLabel("Funkcja celu");

    // set the color gradient of the color map to one of the presets:
    colorMap->setGradient(QCPColorGradient::gpHues);
    // we could have also created a QCPColorGradient instance and added own colors to
    // the gradient, see the documentation of QCPColorGradient for what's possible.

    // rescale the data dimension (color) such that all data points lie in the span visualized by the color gradient:
    colorMap->rescaleDataRange();

    // make sure the axis rect and color scale synchronize their bottom and top margins (so they line up):
    QCPMarginGroup *marginGroup = new QCPMarginGroup(ui->customPlot);
    ui->customPlot->axisRect()->setMarginGroup(QCP::msBottom|QCP::msTop, marginGroup);
    colorScale->setMarginGroup(QCP::msBottom|QCP::msTop, marginGroup);

    // rescale the key (x) and value (y) axes so the whole color map is visible:
    ui->customPlot->rescaleAxes();

    QVector<double> x1(1), y1(1);
    x1[0] = minPoint(0);
    y1[0] = minPoint(1);
    ui->customPlot->addGraph();
    ui->customPlot->graph(0)->setData(x1, y1);
    ui->customPlot->graph(0)->setLineStyle(QCPGraph::lsNone);
    ui->customPlot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, Qt::blue, Qt::white, 5));

    ui->customPlot->replot();
}
