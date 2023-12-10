#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPair>
#include <QMap>
#include <QComboBox>
#include "unit.h"
#include "electronic.h"
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void calcOhmLaw();
    void resetValueOhmLaw();
    void resetValueVelocity();
    void witchTermVelocity(QString);
    void calcVelocity();

    void resetValueAverageVelocity();
    void witchTermAverageVelocity(QString);
    void calcAverageVelocity();

    void resetValueKineticEnergyLinear();
    void witchTermKineticEnergyLinear(QString);
    void calcKineticEnergyLinear();
    void resetValueKineticEnergyRotational();
    void witchTermKineticEnergyRotational(QString);
    void calcKineticEnergyRotational();

    void setWattByApplicance(QString);
    void resetElecCost();
    void calcElecCost();

    void witchTermSpeedDifference(QString);
    void knownParameters(QString);

    void calcSpeedDiffTerm();
    void calcAverageAcceleration();
    void calcDistanceTraveled();
    void calcConstantAcceleration();
    void calcForceAndMass();

    void resetSpeedDiff();
    void resetAverageAcceleration();
    void resetDistanceTraveled();
    void resetConstantAcceleration();
    void resetForceAndMass();

    private:
    // Déclaration des variables membres
    QMessageBox msgBox;
    QPair<double, int> numberWithUnit;
    double voltage;
    double current;
    double resistance;
    double power;
    Ui::MainWindow *ui;
    unit *unity;
    electronic *elecObject;

    // Déclaration des fonctions membres
        //unit
    QPair<double, int> calcUnit(double nb);
    void populateUnits(QComboBox *comboBox, const QMap<double, QString> *unitMap, const QString &defaultUnit, QString suffixe);
    QMap<double, QString> *selectUnitMap(const QString &inputUnit);
    float getConvertedInput(float inputValue, const QString &inputUnit);
    QMap<double, QString>* findUnitMap(const QString &inputUnit);
    void populateBasicUnitComboBoxes();
        //velocity
    void setupTermVelocityUI(const QString &input1Label, const QString &input1Unit, const QString &input2Label, const QString &input2Unit);
    void setupTermAverageVelocityUI(const QString &input1Label, const QString &input1Unit, const QString &input2Label, const QString &input2Unit);
    void displayVelocityResult(float result, const QString &resultText, const QString &finalUnit);
        //kinetic energy
    void setupTermKineticEnergyLinear(const QString &input1Label, const QString &input1Unit, const QString &input2Label, const QString &input2Unit);
     void setupTermKineticEnergyRotational(const QString &input1Label, const QString &input1Unit, const QString &input2Label, const QString &input2Unit);
        //ui and connect
    void setupConnections();
    void initializeUI();
    void setDefaultValues();
        //electricity cost
    void setupElecCostUI();
    void calculateSimpleElectricityCost();
    void calculateComplexElectricityCost();

        //Acceleration Calculator
    //Speed Difference
    void setupTermSpeedDifference(const QString &input1Label, const QString &input1Unit, const QString &input2Label, const QString &input2Unit, const QString &input3Label, const QString &input3Unit);

    //Average Acceleration
    void setupUiAverageAcceleration();

    //Distance Traveled
    void setupUiDistanceTraveled();

    //Force and mass
    void setupUiForceAndMass();

    //Constant Acceleratino
    void setupUiConstantAcceleration();

};

#endif // MAINWINDOW_H
