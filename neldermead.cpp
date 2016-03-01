#include "neldermead.h"

NelderMead::NelderMead(RowVectorXd (*TargetFunction)(MatrixXd), double (*StopCondFunc)(MatrixXd), Vector6d params) : minValue(0)
{
    this->TargetFunction = TargetFunction;
    this->StopCondFunc = StopCondFunc;
    this->params = params;
    this->minPoint = RowVectorXd(this->params[5]);
}

NelderMead::~NelderMead()
{
    while(!this->simplexStates.empty())
        this->simplexStates.pop_back();
}

void NelderMead::Run()
{
    //nie wykonujemy algorytmu, jeśli historia ruchów nie jest pusta, bo to oznacza,
    //że algorytm został już dla danego obiektu wykonany i nie ma sensu robić tego drugi raz,
    //bo wynik będzie ten sam.
    if(!this->simplexStates.empty())
        throw "Simplex states history not empty";

    //Stop awaryjny.
    int safetyStop = 0;
    //Główny warunek stopu
    double stop = INFINITY;
    //Istotne punkty, które warto trzymać w postaci zmiennych.
    RowVectorXd centerPoint(static_cast< int> (this->params[5]));
    RowVectorXd reflectionPoint(static_cast< int > (this->params[5]));
    RowVectorXd contractionPoint(static_cast< int > (this->params[5]));
    RowVectorXd expansionPoint(static_cast< int > (this->params[5]));
    //Simpleks z braku lepszego powodu jest macierzą jednostkową. (Zresztą radzi sobie nieźle)
    MatrixXd simplex = MatrixXd::Identity(this->params[5] +1, this->params[5]);

    //Główna pętla
    while(stop > this->params[4] && safetyStop < 1000)
    {
        //Wrzucamy simpleks na stos i zwiekszamy stop awaryjny.
        this->simplexStates.push_back(simplex);
        safetyStop++;

        //sortowanie, punkt centralny, odbicie i zapisanie wykonania odbicia.
        simplex = this->Sort(simplex);
        centerPoint = this->GravityCenter(simplex);
        reflectionPoint = this->Reflection(centerPoint, simplex.row(static_cast< int > (this->params[5])), this->params[0]);
        this->moves.append("Reflection...\n");

        //ekspansja
        if(this->Compare(reflectionPoint, simplex.row(1)))
        {
            expansionPoint = this->Expansion(centerPoint, reflectionPoint, this->params[1]);
            this->moves.append("Expansion...\n");

            if(this->Compare(expansionPoint, reflectionPoint))
            {
                simplex.row(static_cast< int > (this->params[5])) = expansionPoint;
            }
            else
            {
                simplex.row(static_cast< int > (this->params[5])) = reflectionPoint;
            }

            //obliczamy warunek stopu i przchodzimy do nastepnej pętli.
            stop = this->StopCondFunc(simplex);
            continue;
        }
        else
        {
            //Potworek, ale musialem tu jakos zrobic operacje <=, a funkcja Compare dziala jak <
            if((this->Compare(simplex.row(this->params[5] - 1), reflectionPoint) || \
                this->TargetFunction(simplex.row(static_cast< int > (this->params[5] - 1))) == \
                this->TargetFunction(reflectionPoint)) && \
                this->Compare(reflectionPoint, simplex.row(static_cast< int > (this->params[5]))))
            {
                contractionPoint = this->ContractionOut(centerPoint, reflectionPoint, this->params[2]);
                this->moves.append("ContractionOut...\n");

                if(this->Compare(contractionPoint, reflectionPoint))
                {
                    simplex.row(static_cast< int > (this->params[5])) = contractionPoint;
                    stop = this->StopCondFunc(simplex);
                    continue;
                }
            }
            else
            {
                contractionPoint = this->ContractionIns(centerPoint, simplex.row(static_cast< int > (this->params[5])), this->params[2]) ;
                this->moves.append("ContractionIns...\n");

                if(this->Compare(contractionPoint, simplex.row(static_cast< int > (this->params[5]))))
                {
                    simplex.row(static_cast< int > (this->params[5])) = contractionPoint;
                    stop = this->StopCondFunc(simplex);
                    continue;
                }
            }

            //redukcja, jesli nie znaleziono lepszego punktu
            simplex = this->Shrink(simplex, this->params[3]);
            this->moves.append("Shrink...\n");
            stop = this->StopCondFunc(simplex);
        }
    }

    //Zwracamy środek ciężkości simpleksu (nasz szukany punkt)
    this->minPoint = this->GravityCenter(simplex);
    this->minValue = this->TargetFunction(this->minPoint).coeff(0, 0);
    this->moves.append("FIN.");
}

RowVectorXd NelderMead::Reflection(RowVectorXd centerPoint, RowVectorXd point, double alfa)
{
    return centerPoint + alfa * (centerPoint - point);
}

RowVectorXd NelderMead::Expansion(RowVectorXd centerPoint, RowVectorXd point, double beta)
{
    return centerPoint + beta * (point - centerPoint);
}

RowVectorXd NelderMead::ContractionOut(RowVectorXd centerPoint, RowVectorXd point, double gamma)
{
   return centerPoint + gamma * (point - centerPoint);
}

RowVectorXd NelderMead::ContractionIns(RowVectorXd centerPoint, RowVectorXd point, double gamma)
{
    return centerPoint - gamma * (point - centerPoint);
}

MatrixXd NelderMead::Shrink(MatrixXd inputSimplex, double sigma)
{
    MatrixXd mat(inputSimplex.rows(), inputSimplex.cols());
    mat = ((inputSimplex.rowwise() - inputSimplex.row(0)) * sigma).rowwise() + inputSimplex.row(0);
    mat.row(0) = inputSimplex.row(0);

    return mat;
}

RowVectorXd NelderMead::GravityCenter(MatrixXd inputSimplex)
{
    return (inputSimplex.colwise().sum() - inputSimplex.row(inputSimplex.cols())) / inputSimplex.cols();
}

//target function przyjmuje macierz, wiec mozemy oba porownywane punkty polaczyc i przeslac.
bool NelderMead::Compare(RowVectorXd first, RowVectorXd second)
{
    RowVector2d val;
    MatrixXd temp(2, first.cols());
    temp.row(0) = first;
    temp.row(1) = second;
    val = this->TargetFunction(temp);

    return val[0] < val[1];
}

MatrixXd NelderMead::Sort(MatrixXd inputSimplex)
{
    RowVectorXd temp(inputSimplex.cols());

    for(int i = 0; i < inputSimplex.rows(); i++)
    {
        for(int j = 0; j < inputSimplex.rows(); j++)
        {
            if(this->Compare(inputSimplex.row(i), inputSimplex.row(j)))
            {
                temp = inputSimplex.row(i);
                inputSimplex.row(i) = inputSimplex.row(j);
                inputSimplex.row(j) = temp;
            }
        }
    }

    return inputSimplex;
}

RowVectorXd NelderMead::MinPoint()
{
    return this->minPoint;
}

double NelderMead::MinValue()
{
    return this->minValue;
}

QString NelderMead::Moves()
{
    return this->moves;
}

std::vector< MatrixXd > NelderMead::States()
{
    return this->simplexStates;
}
