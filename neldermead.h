#ifndef NM_H
#define NM_H
#include <vector>
#include <QString>
#include <Eigen/Dense>
using namespace Eigen;

/*!
 \brief
 Wektor 6 elementowy. Wykorzystuję go do przekazania obiektowi parametrów, których jest 6 właśnie.
 Wektor bazuje na macierzy z biblioteki Eigen i można na niego bez problemu castować
 macierz z tej biblioteki o rozmiarach 1:6 i typie danych double.
 Typedef użyłem, żeby zapis był ładniejszy.
 \typedef Vector6d
*/
typedef Matrix< double, 1, 6 > Vector6d;
/*!
 \brief
 Klasa, jak klasa, żadnych cudów. Korzystam tu z biblioteki eigen do obsługi macierzy
 i prawdopodobnie GUI również będzie jej potrzebowało.
 \class NelderMead
*/
class NelderMead
{
private:
    std::vector< MatrixXd > simplexStates; /*!< TODO */
    RowVectorXd minPoint; /*!< TODO */
    //Parametry przekazywane do algorytmu trzymane są w zmiennej params.
    //Parametry te to: alfa, beta, gamma, sigma, dokładność, liczba zmiennych
    //Przy tak skonstruowanej klasie, jaką mamy, nie ma innej możliwości,
    //by dowiedzieć się jaki jest rozmiar problemu, niż po prostu podanie liczby
    //zmiennych w parametrach. Odpowiedzialność za to spada na GUI.
    Vector6d params; /*!< TODO */
    double minValue; /*!< TODO */
    QString moves; /*!< TODO */

    /*!
     \brief
     Wskaźnik do funkcji celu. Musi ona przyjmować na wejście macierz o dynamicznie określanym rozmiarze,
     z elementami typu double, taką jaka została zdefiniowana w bibliotece Eigen. (Do funkcji wrzucamy cały simpleks).
     Funkcja musi zwracać wektor wierszowy, który także zdefiniowano w bibliotece Eigen.
     Wektor ten ma elementy typu double i są to wartości funkcji celu dla każdego wierzchołka simpleksu.
     \fn TargetFunction
     \param (*TargetFunction)(MatrixXd)
     \return RowVectorXd
    */
    RowVectorXd (*TargetFunction)(MatrixXd);
    /*!
     \brief
     Wskaźnik do funkcji obliczającej warunek stopu. Ta równierz musi przyjąć na wejście
     cały simpleks w postaci macierzy, ale zwraca pojedynczą liczbę typu double, czyli obliczoną wartość stop.
     \fn StopCondFunc
     \param StopCondFunc)(MatrixXd
     \return double
    */
    double (*StopCondFunc)(MatrixXd);

    /*!
     \brief
     \fn Reflection
     \param RowVectorXd
     \param RowVectorXd
     \param double
     \return RowVectorXd
    */
    RowVectorXd Reflection(RowVectorXd, RowVectorXd, double);
    /*!
     \brief
     \fn Expansion
     \param RowVectorXd
     \param RowVectorXd
     \param double
     \return RowVectorXd
    */
    RowVectorXd Expansion(RowVectorXd, RowVectorXd, double);
    /*!
     \brief
     \fn ContractionOut
     \param RowVectorXd
     \param RowVectorXd
     \param double
     \return RowVectorXd
    */
    RowVectorXd ContractionOut(RowVectorXd, RowVectorXd, double);
    /*!
     \brief
     \fn ContractionIns
     \param RowVectorXd
     \param RowVectorXd
     \param double
     \return RowVectorXd
    */
    RowVectorXd ContractionIns(RowVectorXd, RowVectorXd, double);
    /*!
     \brief
     \fn Shrink
     \param MatrixXd
     \param double
     \return MatrixXd
    */
    MatrixXd Shrink(MatrixXd, double);
    /*!
     \brief
     \fn GravityCenter
     \param MatrixXd
     \return RowVectorXd
    */
    RowVectorXd GravityCenter(MatrixXd);
    /*!
     \brief
     \fn Compare
     \param RowVectorXd
     \param RowVectorXd
     \return bool
    */
    bool Compare(RowVectorXd, RowVectorXd);
    /*!
     \brief
     \fn Sort
     \param inputSimplex
     \return MatrixXd
     */
    MatrixXd Sort(MatrixXd inputSimplex);
public:
    /*!
     \brief
     Konstruktor klasy potrzebuje wkaźników do funkcji celu i stopu, oraz wektora parametrów.
     \fn NelderMead
     \param (TargetFunction)(RowVectorXd)
     \param (StopCondFunc)(MatrixXd)
     \param params
    */
    NelderMead(RowVectorXd (*TargetFunction)(MatrixXd), double (*StopCondFunc)(MatrixXd), Vector6d params);
    /*!
     \brief
     \fn ~NelderMead
    */
    ~NelderMead();
    /*!
     \brief
     Uruchamia algorytm. Nic nie przyjmuje, nic nie zwraca, wszystko jest zapisane w zmiennych prywatnych klasy.
     \fn Run
    */
    void Run();
    /*!
     \brief
     Zwraca współrzędne szukanego, najlepszego punktu.
     \fn MinPoint
     \return
     */
    RowVectorXd MinPoint();
    /*!
     \brief
     Zwraca wartość funkcji celu w najlepszym punkcie.
     \fn MinValue
     \return
     */
    double MinValue();
    /*!
     \brief
     Zwraca string z listą wszystkich wykonanych czynności.
    \fn Moves
     \return QString
     */
    QString Moves();
    /*!
     \brief
     Zwraca wektor (zwykły, ze standardowej biblioteki c++) zawierający wszystkie stany, w jakich znajdował się
     simplex (macierze typu MatrixXd) od momentu rozpoczecia algorytmu do momentu jego zakończenia.
     Będzie to potrzebne do rysowania po kolei simpleksu na wykresie, a std::vector ma ładnie zdefiniowane iteratory.
     \fn States
     \return std::vector< MatrixXd>
     */
    std::vector< MatrixXd> States();
};

#endif // NM_H
