#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "unit.h"
#include "electronic.h"
#include <QPair>
#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , unity(new unit)
    , elecObject(new electronic)
{
    ui->setupUi(this);

    // Connect signals to slots
    setupConnections();

    // Initialize UI elements
    initializeUI();
}

#pragma UI and connection{
    // Set up connections for buttons and combo boxes
    void MainWindow::setupConnections() {
        connect(ui->calc_ohmLaw, SIGNAL(clicked()), this, SLOT(calcOhmLaw()));
        connect(ui->resetValue_ohmLaw, SIGNAL(clicked()), this, SLOT(resetValueOhmLaw()));
        connect(ui->velocity_choiceDtCovered, SIGNAL(currentTextChanged(QString)), this, SLOT(witchTermVelocity(QString)));
        connect(ui->velocity_calc, SIGNAL(clicked()), this, SLOT(calcVelocity()));
        connect(ui->velocity_reset, SIGNAL(clicked()), this, SLOT(resetValueVelocity()));
        connect(ui->velocity_calcAverageVelocity, SIGNAL(clicked()), this, SLOT(calcAverageVelocity()));
        connect(ui->velocity_choiceAverageVelocity, SIGNAL(currentTextChanged(QString)), this, SLOT(witchTermAverageVelocity(QString)));
        connect(ui->velocity_resetAverageVelocity, SIGNAL(clicked()), this, SLOT(resetValueAverageVelocity()));

        connect(ui->kineticEnergy_calcLinear, SIGNAL(clicked()), this, SLOT(calcKineticEnergyLinear()));
        connect(ui->kineticEnergy_choiceLinear, SIGNAL(currentTextChanged(QString)), this, SLOT(witchTermKineticEnergyLinear(QString)));
        connect(ui->kineticEnergy_resetLinear, SIGNAL(clicked()), this, SLOT(resetValueKineticEnergyLinear()));
        connect(ui->kineticEnergy_calcRotational, SIGNAL(clicked()), this, SLOT(calcKineticEnergyRotational()));
        connect(ui->kineticEnergy_choiceRotational, SIGNAL(currentTextChanged(QString)), this, SLOT(witchTermKineticEnergyRotational(QString)));
        connect(ui->kineticEnergy_resetRotational, SIGNAL(clicked()), this, SLOT(resetValueKineticEnergyRotational()));

        connect(ui->electricity_appliance, SIGNAL(currentTextChanged(QString)), this, SLOT(setWattByApplicance(QString)));
        connect(ui->electricityCost_calc, SIGNAL(clicked()), this, SLOT(calcElecCost()));
        connect(ui->electricityCost_reset, SIGNAL(clicked()), this, SLOT(resetElecCost()));

        //connect(ui->radioButton_simpleElectricCOst, SIGNAL(toggled(bool)), this, SLOT(simpleElectricCostUI()));
        //connect(ui->radioButton_complexElectricCOst, SIGNAL(toggled(bool)), this, SLOT(complexElectricCostUI()));

        connect(ui->acceleration_choiceSpeedDifference, SIGNAL(currentTextChanged(QString)), this, SLOT(witchTermSpeedDifference(QString)));
        connect(ui->acceleration_choiceConstantAcceleration, SIGNAL(currentTextChanged(QString)), this, SLOT(knownParameters(QString)));
        connect(ui->acceleration_calc, SIGNAL(clicked(bool)), this, SLOT(calcSpeedDiffTerm()));
        connect(ui->acceleration_averageAcceleration_calc, SIGNAL(clicked(bool)), this, SLOT(calcAverageAcceleration()));
        connect(ui->acceleration_distanceTraveled_calc, SIGNAL(clicked(bool)), this, SLOT(calcDistanceTraveled()));
        connect(ui->acceleration_forceAndMass_calc, SIGNAL(clicked(bool)), this, SLOT(calcForceAndMass()));
        connect(ui->acceleration_constantAcceleration_calc, SIGNAL(clicked(bool)), this, SLOT(calcConstantAcceleration()));

        connect(ui->acceleration_reset, SIGNAL(clicked(bool)), this, SLOT(resetSpeedDiff()));
        connect(ui->acceleration_averageAcceleration_reset, SIGNAL(clicked(bool)), this, SLOT(resetAverageAcceleration()));
        connect(ui->acceleration_distanceTraveled_reset, SIGNAL(clicked(bool)), this, SLOT(resetDistanceTraveled()));
        connect(ui->acceleration_constantAcceleration_reset, SIGNAL(clicked(bool)), this, SLOT(resetConstantAcceleration()));
        connect(ui->acceleration_forceAndMass_reset, SIGNAL(clicked(bool)), this, SLOT(resetForceAndMass()));
    }

    // Initialize UI elements, populate combo boxes, and set default values
    void MainWindow::initializeUI() {
        populateBasicUnitComboBoxes();
        setDefaultValues();
        witchTermVelocity("Velocity");
        witchTermAverageVelocity("Average Velocity");
        witchTermKineticEnergyLinear("Kinetic Energy");
        witchTermKineticEnergyRotational("Kinetic Energy");
        witchTermSpeedDifference("Acceleration");
        setupElecCostUI();
        setupUiAverageAcceleration();
        setupUiDistanceTraveled();
        setupUiForceAndMass();
        setupUiConstantAcceleration();
        knownParameters("Time, Inital Velocity, Final Velocity");
    }

    // Set default values for static combo boxes
    void MainWindow::setDefaultValues() {
        ui->ohmLaw_unitA->setCurrentText("A");
        ui->ohmLaw_unitW->setCurrentText("W");
        ui->electricCost_unitPowerConsuption->setCurrentText("W");
        ui->ohmLaw_unitV->setCurrentText("V");
        ui->ohmLaw_unitohm->setCurrentText("Ώ");

        for (auto key : elecObject->electronics.keys()) {
            ui->electricity_appliance->addItem(elecObject->electronics.value(key));
        }
    }

#pragma}

#pragma ohmLaw{
    //Réinitialise les valeurs des champs de saisie de la loi d'Ohm à zéro.
    void MainWindow::resetValueOhmLaw() {
        //reset all values in calc ohm law to 0
        ui->ohmLaw_currentInput->setValue(0);
        ui->ohmLaw_powInput->setValue(0);
        ui->ohmLaw_resInput->setValue(0);
        ui->ohmLaw_voltInput->setValue(0);
    }

    //Calcule les valeurs manquantes (tension, courant, résistance ou puissance) en fonction des valeurs saisies par l'utilisateur.
    void MainWindow::calcOhmLaw()
    {
        //get the value with the unit
        voltage = ui->ohmLaw_voltInput->value() *  pow(10,unity->basicUnit.key(ui->ohmLaw_unitV->currentText().remove("V")));
        current = ui->ohmLaw_currentInput->value() * pow(10,unity->basicUnit.key(ui->ohmLaw_unitA->currentText().remove("A")));
        resistance = ui->ohmLaw_resInput->value() * pow(10,unity->basicUnit.key(ui->ohmLaw_unitohm->currentText().remove("Ώ")));
        power = ui->ohmLaw_powInput->value() * pow(10,unity->basicUnit.key(ui->ohmLaw_unitW->currentText().remove("W")));

        //do current calc with the known value
        if (resistance > 0 && power > 0) {
            current = sqrt(power / resistance);
            voltage = current * resistance;
        } else if (current > 0 && power > 0) {
            voltage = power / current;
            resistance = voltage * current;
        } else if (current > 0 && resistance > 0) {
            voltage = current * resistance;
            power = voltage * current;
        } else if (voltage > 0 && power > 0) {
            current = power / voltage;
            resistance = voltage / current;
        } else if (voltage > 0 && resistance > 0) {
            current = voltage / resistance;
            power = voltage * current;
        } else if (voltage > 0 && current > 0) {
            resistance = voltage / current;
            power = voltage * current;
        } else {
            // Afficher un message d'erreur
            // Pas assez de valeurs
        }

        //check value and change item depends on their exponent
        numberWithUnit = calcUnit(voltage);
        voltage = numberWithUnit.first;
        ui->ohmLaw_unitV->setCurrentText(unity->basicUnit.value(numberWithUnit.second)+ "V");


        numberWithUnit = calcUnit(current);
        current = numberWithUnit.first;
        ui->ohmLaw_unitA->setCurrentText(unity->basicUnit.value(numberWithUnit.second)+ "A");


        numberWithUnit = calcUnit(power);
        power = numberWithUnit.first;
        ui->ohmLaw_unitW->setCurrentText(unity->basicUnit.value(numberWithUnit.second)+ "W");



        numberWithUnit = calcUnit(resistance);
        resistance = numberWithUnit.first;
        ui->ohmLaw_unitohm->setCurrentText(unity->basicUnit.value(numberWithUnit.second)+ "Ώ");

        //display current new numbers
        ui->ohmLaw_currentInput->setValue(current);
        ui->ohmLaw_powInput->setValue(power);
        ui->ohmLaw_resInput->setValue(resistance);
        ui->ohmLaw_voltInput->setValue(voltage);

    }

#pragma}

#pragma Distance covered{
    //Configure les champs de saisie de vitesse en fonction de l'option choisie par l'utilisateur (vitesse, distance ou temps).
    void MainWindow::witchTermVelocity(QString Term) {
        // Clear combo boxes
        ui->velocity_unitInput1->clear();
        ui->velocity_unitInput2->clear();

        if (Term == "Velocity") {
            setupTermVelocityUI("Distance", "m", "Time", "s");
        } else if (Term == "Distance") {
            setupTermVelocityUI("Velocity", "m/s", "Time", "s");
        } else if (Term == "Time") {
            setupTermVelocityUI("Distance", "m", "Velocity", "m/s");
        }
    }

    // Initialise les champs de saisie de vitesse en fonction de l'option choisie par l'utilisateur.
    void MainWindow::setupTermVelocityUI(const QString &input1Label, const QString &input1Unit, const QString &input2Label, const QString &input2Unit) {
        ui->velocity_input1Label->setText(input1Label);
        ui->velocity_input2Label->setText(input2Label);
        populateUnits(ui->velocity_unitInput1, findUnitMap(input1Unit), input1Unit, input1Unit);
        populateUnits(ui->velocity_unitInput2, findUnitMap(input2Unit), input2Unit, input2Unit);
    }

    //Calcule la vitesse, la distance ou le temps en fonction des valeurs saisies et de l'unité de mesure choisie.
    void MainWindow::calcVelocity() {
        // Convert inputs to SI units
        float input1_SI = getConvertedInput(ui->velocity_input1->text().toFloat(), ui->velocity_unitInput1->currentText());
        float input2_SI = getConvertedInput(ui->veloity_input2->text().toFloat(), ui->velocity_unitInput2->currentText());

        float result;
        QString resultText;
        QString finalUnit;

        // Calculate and format the result based on user's choice
        if (ui->velocity_choiceDtCovered->currentText() == "Velocity") {
            result = input1_SI / input2_SI;
            finalUnit = "m/s";
            resultText = "Velocity : ";
        } else if (ui->velocity_choiceDtCovered->currentText() == "Distance") {
            result = input1_SI * input2_SI;
            finalUnit = "m";
            resultText = "Distance : ";
        } else if (ui->velocity_choiceDtCovered->currentText() == "Time") {
            result = input1_SI / input2_SI;
            finalUnit = "s";
            resultText = "Time : ";
        }

        // Display the result
        msgBox.setText(resultText + QString::number(result) + " " + finalUnit);
        msgBox.exec();
    }

    //Réinitialise les valeurs des champs de saisie de vitesse à zéro.
    void MainWindow::resetValueVelocity() {
        ui->velocity_input1->setText("0");
        ui->veloity_input2->setText("0");
    }
#pragma}

#pragma Average Velocity{
    void MainWindow::resetValueAverageVelocity(){
        ui->velocity_input2_1->setText(0);
        ui->velocity_input2_2->setText(0);
    }

    void MainWindow::witchTermAverageVelocity(QString Term){
        // Clear combo boxes
        ui->velocity_input2_2Unit->clear();
        ui->velocity_input2_2Unit_2->clear();

        if (Term == "Average Velocity") {
            setupTermAverageVelocityUI("Initial Velocity", "m/s", "Final Velocity", "m/s");
        } else if (Term == "Initial Velocity") {
            setupTermAverageVelocityUI("Average Velocity", "m/s", "Final Velocity", "m/s");
        } else if (Term == "Final Velocity") {
            setupTermAverageVelocityUI("Average Velocity", "m/s", "Initial Velocity", "m/s");
        }
    }

    void MainWindow::setupTermAverageVelocityUI(const QString &input1Label, const QString &input1Unit, const QString &input2Label, const QString &input2Unit) {
        ui->velocity_input2_1Label->setText(input1Label);
        ui->velocity_input2_2Label->setText(input2Label);
        populateUnits(ui->velocity_input2_2Unit, findUnitMap(input1Unit), input1Unit, input1Unit);
        populateUnits(ui->velocity_input2_2Unit_2, findUnitMap(input2Unit), input2Unit, input2Unit);
    }

    void MainWindow::calcAverageVelocity(){
        float input1_SI = getConvertedInput(ui->velocity_input2_1->text().toFloat(), ui->velocity_input2_2Unit->currentText());
        float input2_SI = getConvertedInput(ui->velocity_input2_2->text().toFloat(), ui->velocity_input2_2Unit_2->currentText());

        float result;
        QString resultText;
        QString finalUnit;

        /*
         *
         *Vf = Vi + 2 * Av
         *
         *où :
         *
         *  Vf est la vitesse finale
         *  Vi est la vitesse initiale
         *  Av est la vitesse moyenne
         *
         *
         */


        // Calculate and format the result based on user's choice
        if (ui->velocity_choiceAverageVelocity->currentText() == "Average Velocity") {
            result = (input1_SI - input2_SI) / -2;
            resultText = "Average Velocity : ";
        } else if (ui->velocity_choiceAverageVelocity->currentText() == "Initial Velocity") {
            result = input2_SI - 2 * input1_SI;
            resultText = "Initial Velocity : ";
        } else if (ui->velocity_choiceAverageVelocity->currentText() == "Final Velocity") {
            result = input2_SI + 2 * input1_SI;
            resultText = "Final Velocity : ";
        }

        finalUnit = "m/s";
        // Display the result
        msgBox.setText(resultText + QString::number(result) + " " + finalUnit);
        msgBox.exec();
    }
#pragma}

#pragma Kinetic Energy{


    // Resets input fields for linear kinetic energy calculations
    void MainWindow::resetValueKineticEnergyLinear(){
        ui->kineticEnergy_input1_1->setText(0);
        ui->kineticEnergy_input1_2->setText(0);
    }

    // Determines which term to calculate for linear kinetic energy and sets up the UI accordingly
    void MainWindow::witchTermKineticEnergyLinear(QString Term){
        // Clear combo boxes
        ui->kineticEnergy_unit1_1->clear();
        ui->kineticEnergy_unit1_1->clear();

        if (Term == "Kinetic Energy") {
            setupTermKineticEnergyLinear("Mass", "g", "Velocity", "m/s");
        } else if (Term == "Mass") {
            setupTermKineticEnergyLinear("Kinetic Energy", "J", "Velocity", "m/s");
        } else if (Term == "Velocity") {
            setupTermKineticEnergyLinear("Kinetic Energy", "J", "Mass", "g");
        }
    }

    // Sets up input labels and units for linear kinetic energy calculations
    void MainWindow::setupTermKineticEnergyLinear(const QString &input1Label, const QString &input1Unit, const QString &input2Label, const QString &input2Unit){
        ui->kineticEnergy_label1_1->setText(input1Label);
        ui->kineticEnergy_label1_2->setText(input2Label);
        populateUnits(ui->kineticEnergy_unit1_1, findUnitMap(input1Unit), input1Unit, input1Unit);
        populateUnits(ui->kineticEnergy_unit1_2, findUnitMap(input2Unit), input2Unit, input2Unit);
    }

    // Calculates and displays the result for linear kinetic energy based on user's choice
    void MainWindow::calcKineticEnergyLinear(){
        // Convert inputs to SI units
        float input1_SI = getConvertedInput(ui->kineticEnergy_input1_1->text().toFloat(), ui->kineticEnergy_unit1_1->currentText());
        float input2_SI = getConvertedInput(ui->kineticEnergy_input1_2->text().toFloat(), ui->kineticEnergy_unit1_2->currentText());
        float result;
        QString resultText;
        QString finalUnit;

        // Calculate and format the result based on user's choice
        if (ui->kineticEnergy_choiceLinear->currentText() == "Kinetic Energy") {
            result = 0.5 * input1_SI * input2_SI * input2_SI;
            finalUnit = "J";
            resultText = "Kinetic Energy : ";
        } else if (ui->kineticEnergy_choiceLinear->currentText() == "Mass") {
            result = input1_SI / (0.5 * input2_SI * input2_SI);
            finalUnit = "kg";
            resultText = "Mass : ";
        } else if (ui->kineticEnergy_choiceLinear->currentText() == "Velocity") {
            result = sqrt(input1_SI/(0.5*input2_SI));
            finalUnit = "m/s";
            resultText = "Velocity : ";
        }

        // Display the result
        msgBox.setText(resultText + QString::number(result) + " " + finalUnit);
        msgBox.exec();
    }


    void MainWindow::resetValueKineticEnergyRotational(){
        ui->kineticEnergy_input2_1->setText("0");
        ui->kineticEnergy_input2_2->setText("0");
    }

    // Determines which term to calculate for rotational kinetic energy and sets up the UI accordingly
    void MainWindow::witchTermKineticEnergyRotational(QString Term){
        // Clear combo boxes
        ui->kineticEnergy_unit2_1->clear();
        ui->kineticEnergy_unit2_1->clear();

        if (Term == "Kinetic Energy") {
            setupTermKineticEnergyRotational("Moment of Inertia", "kg*m^2", "Angular Velocity", "none");
        } else if (Term == "Moment of Inertia") {
            setupTermKineticEnergyRotational("Kinetic Energy", "J", "Angular Velocity", "none");
        } else if (Term == "Angular Velocity") {
            setupTermKineticEnergyRotational("Kinetic Energy", "J", "Moment of Inertia", "kg*m^2");
        }
    }

    // Sets up input labels and units for rotational kinetic energy calculations
    void MainWindow::setupTermKineticEnergyRotational(const QString &input1Label, const QString &input1Unit, const QString &input2Label, const QString &input2Unit){
        ui->kineticEnergy_label2_1->setText(input1Label);
        ui->kineticEnergy_label2_2->setText(input2Label);
        populateUnits(ui->kineticEnergy_unit2_1, findUnitMap(input1Unit), input1Unit, input1Unit);
        populateUnits(ui->kineticEnergy_unit2_2, findUnitMap(input2Unit), input2Unit, input2Unit);
    }

    // Calculates and displays the result for rotational kinetic energy based on user's choice
    void MainWindow::calcKineticEnergyRotational(){
        // Convert inputs to SI units
        float input1_SI = getConvertedInput(ui->kineticEnergy_input2_1->text().toFloat(), ui->kineticEnergy_unit2_1->currentText());
        float input2_SI = getConvertedInput(ui->kineticEnergy_input2_2->text().toFloat(), ui->kineticEnergy_unit2_2->currentText());

        float result;
        QString resultText;
        QString finalUnit;

        // Calculate and format the result based on user's choice
        if (ui->kineticEnergy_choiceRotational->currentText() == "Kinetic Energy") {
            result = 0.5 * input1_SI * pow(input2_SI,2);
            finalUnit = "J";
            resultText = "Kinetic Energy : ";
        } else if (ui->kineticEnergy_choiceRotational->currentText() == "Moment of Inertia") {
            result = input1_SI / (0.5 * pow(input2_SI,2));
            finalUnit = "kg*m^2";
            resultText = "Moment of Inertia : ";
        } else if (ui->kineticEnergy_choiceRotational->currentText() == "Angular Velocity") {
            result = sqrt(input1_SI/(0.5*input2_SI));
            finalUnit = "";
            resultText = "Angular Velocity : ";
        }

        // Display the result
        msgBox.setText(resultText + QString::number(result) + " " + finalUnit);
        msgBox.exec();
    }

    #pragma}

#pragma Acceleration Calculator{
    #pragma Speed Difference{

    void MainWindow::witchTermSpeedDifference(QString Term){
        // Clear combo boxes
        ui->acceleration_inputUnit1->clear();
        ui->acceleration_inputUnit2->clear();
        ui->acceleration_inputUnit3->clear();

        if (Term == "Acceleration") {
            setupTermSpeedDifference("Inital Speed", "m/s", "Final Speed", "m/s", "Time", "s");
        } else if (Term == "Initial Speed") {
            setupTermSpeedDifference("Final Speed", "m/s", "Time", "s", "Acceleration", "m/s^2");
        } else if (Term == "Final Speed") {
            setupTermSpeedDifference("Inital Speed", "m/s", "Time", "s", "Acceleration", "m/s^2");
        } else if (Term == "Time") {
            setupTermSpeedDifference("Inital Speed", "m/s", "Final Speed", "m/s", "Acceleration", "m/s^2");
        }
    }

    void MainWindow::setupTermSpeedDifference(const QString &input1Label, const QString &input1Unit, const QString &input2Label, const QString &input2Unit, const QString &input3Label, const QString &input3Unit) {
        ui->acceleration_inputLabel1->setText(input1Label);
        ui->acceleration_inputLabel2->setText(input2Label);
        ui->acceleration_inputLabel3->setText(input3Label);

        populateUnits(ui->acceleration_inputUnit1, findUnitMap(input1Unit), input1Unit, input1Unit);
        populateUnits(ui->acceleration_inputUnit2, findUnitMap(input2Unit), input2Unit, input2Unit);
        populateUnits(ui->acceleration_inputUnit3, findUnitMap(input3Unit), input3Unit, input3Unit);
    }

    void MainWindow::calcSpeedDiffTerm(){
        float input1_SI = getConvertedInput(ui->acceleration_input1->text().toFloat(), ui->acceleration_inputUnit1->currentText());
        float input2_SI = getConvertedInput(ui->acceleration_input1_2->text().toFloat(), ui->acceleration_inputUnit2->currentText());
        float input3_SI = getConvertedInput(ui->acceleration_input1_3->text().toFloat(), ui->acceleration_inputUnit3->currentText());

        float result;
        QString resultText;
        QString finalUnit = "m/s";

        // Calculate and format the result based on user's choice
        if (ui->acceleration_choiceSpeedDifference->currentText() == "Acceleration") {
            result = (input2_SI - input1_SI) / input3_SI;
            resultText = "Acceleration : ";
            finalUnit = "m/s^2";
        } else if (ui->acceleration_choiceSpeedDifference->currentText() == "Initial Speed") {
            result = -(input3_SI * input2_SI) + input1_SI;
            resultText = "Initial Speed : ";
        } else if (ui->acceleration_choiceSpeedDifference->currentText() == "Final Speed") {
            result = input1_SI + input2_SI * input3_SI;
            resultText = "Final Speed : ";
        } else if (ui->acceleration_choiceSpeedDifference->currentText() == "Time") {
            result = (input2_SI - input1_SI) / input3_SI;
            resultText = "Time : ";
            finalUnit = "s";
        }


        // Display the result
        msgBox.setText(resultText + QString::number(result) + " " + finalUnit);
        msgBox.exec();
    }

    void MainWindow::resetSpeedDiff(){
        ui->acceleration_input1->setText("");
        ui->acceleration_input1_2->setText("");
        ui->acceleration_input1_3->setText("");
    }

    #pragma}

    #pragma Average Acceleration{

    void MainWindow::setupUiAverageAcceleration(){

        populateUnits(ui->acceleration_unitInitalVelocity_averageAcceleration, findUnitMap("m/s"), "m/s", "m/s");
        populateUnits(ui->acceleration_unitFinalVelocity_averageAcceleration, findUnitMap("m/s"), "m/s", "m/s");
        populateUnits(ui->acceleration_unitInitialTime_averageAcceleration, findUnitMap("s"), "s", "s");
        populateUnits(ui->acceleration_unitFinalTime_averageAcceleration, findUnitMap("s"), "s", "s");
        populateUnits(ui->acceleration_unitAverageAccelerationy_averageAcceleration_2, findUnitMap("m/s^2"), "m/s^2", "m/s^2");


    }

    void MainWindow::calcAverageAcceleration(){
        //get the value with the unit
        float initialVelocity = getConvertedInput(ui->acceleration_inputInitalVelocity_averageAcceleration->text().toFloat(), ui->acceleration_unitInitalVelocity_averageAcceleration->currentText());
        float finalVelocity = getConvertedInput(ui->acceleration_inputFinalVelocity_averageAcceleration->text().toFloat(), ui->acceleration_unitFinalVelocity_averageAcceleration->currentText());
        float initialTime = getConvertedInput(ui->acceleration_inputInitialTime_averageAcceleration->text().toFloat(), ui->acceleration_unitInitialTime_averageAcceleration->currentText());
        float finalTime = getConvertedInput(ui->acceleration_inputFinalTime_averageAcceleration->text().toFloat(), ui->acceleration_unitFinalTime_averageAcceleration->currentText());
        float averageAcceleration = getConvertedInput(ui->acceleration_inputAverageAccelerationy_averageAcceleration->text().toFloat(), ui->acceleration_unitAverageAccelerationy_averageAcceleration_2->currentText());
        //do current calc with 4 value at least to find the fifth

        QString resultText;

        // Calculate missing values
        if (initialVelocity > 0 && finalVelocity > 0 && averageAcceleration > 0) {
            float timeDifference = (finalVelocity - initialVelocity) / averageAcceleration;
            if (initialTime == 0 && finalTime == 0) {
                initialTime = 0;
                finalTime = timeDifference;
            } else if (finalTime == 0) {
                finalTime = initialTime + timeDifference;
            } else if (initialTime == 0) {
                initialTime = finalTime - timeDifference;
            }

            resultText = "Initial Time: " + QString::number(initialTime) + " s\n" +
                         "Final Time: " + QString::number(finalTime) + " s\n";
        } else if (initialVelocity > 0 && finalVelocity > 0 && initialTime >= 0 && finalTime > 0) {

            float timeDifference = finalTime - initialTime;
            averageAcceleration = (finalVelocity - initialVelocity) / timeDifference;
            resultText = "Average Acceleration: " + QString::number(averageAcceleration) + " m/s^2";

        } else if (initialVelocity > 0 && initialTime >= 0 && finalTime > 0 && averageAcceleration > 0) {
            finalVelocity = initialVelocity + averageAcceleration * (finalTime - initialTime);
            resultText = "Final Velocity: " + QString::number(finalVelocity) + " m/s";
        } else if (finalVelocity > 0 && initialTime >= 0 && finalTime > 0 && averageAcceleration > 0) {
            initialVelocity = finalVelocity - averageAcceleration * (finalTime - initialTime);
            resultText = "Initial Velocity: " + QString::number(initialVelocity) + " m/s";
        } else {
            // Display an error message
            resultText = "Not enough values provided. Please input at least 4 values.";
        }

        // Display the result
        msgBox.setText(resultText);
        msgBox.exec();
    }

    void MainWindow::resetAverageAcceleration(){
        ui->acceleration_inputInitalVelocity_averageAcceleration->setText("");
        ui->acceleration_inputFinalVelocity_averageAcceleration->setText("");
        ui->acceleration_inputInitialTime_averageAcceleration->setText("");
        ui->acceleration_inputFinalTime_averageAcceleration->setText("");
        ui->acceleration_inputAverageAccelerationy_averageAcceleration->setText("");
    }

    #pragma}

    #pragma Distance Travelled{

    void MainWindow::setupUiDistanceTraveled(){
        populateUnits(ui->acceleration_distanceTraveled_unitInitalSpeed, findUnitMap("m/s"), "m/s", "m/s");
        populateUnits(ui->acceleration_distanceTraveled_unitDistance, findUnitMap("m"), "m", "m");
        populateUnits(ui->acceleration_distanceTraveled_unitTime, findUnitMap("s"), "s", "s");
        populateUnits(ui->acceleration_distanceTraveled_unitAcceleration, findUnitMap("m/s^2"), "m/s^2", "m/s^2");
    }

    void MainWindow::calcDistanceTraveled(){
        //get the value with the unit
        float initialSpeed = getConvertedInput(ui->acceleration_distanceTraveled_inputInitalSpeed->text().toFloat(), ui->acceleration_distanceTraveled_unitInitalSpeed->currentText());
        float distance = getConvertedInput(ui->acceleration_distanceTraveled_inputDistance->text().toFloat(), ui->acceleration_distanceTraveled_unitDistance->currentText());
        float time = getConvertedInput(ui->acceleration_distanceTraveled_inputTime->text().toFloat(), ui->acceleration_distanceTraveled_unitTime->currentText());
        float acceleration = getConvertedInput(ui->acceleration_distanceTraveled_inputAcceleration->text().toFloat(), ui->acceleration_distanceTraveled_unitAcceleration->currentText());

        QString resultText;

        // Calculate missing values when 3 of the 4 values are provided
        if (initialSpeed > 0 && distance > 0 && time > 0) {
            acceleration = 2 * (distance - initialSpeed * time) / (time * time);
            resultText = "Acceleration: " + QString::number(acceleration) + " m/s^2";
        } else if (initialSpeed > 0 && distance > 0 && acceleration > 0) {
            time = 2 * distance / (initialSpeed + sqrt(initialSpeed * initialSpeed + 2 * acceleration * distance));
            resultText = "Time: " + QString::number(time) + " s";
        } else if (initialSpeed > 0 && time > 0 && acceleration > 0) {
            distance = initialSpeed * time + 0.5 * acceleration * time * time;
            resultText = "Distance: " + QString::number(distance) + " m";
        } else if (distance > 0 && time > 0 && acceleration > 0) {
            initialSpeed = (distance - 0.5 * acceleration * time * time) / time;
            resultText = "Initial Speed: " + QString::number(initialSpeed) + " m/s";
        } else {
            resultText = "Not enough input values provided. Please provide at least 3 of the 4 values.";
        }

        // Display the result
        msgBox.setText(resultText);
        msgBox.exec();
    }

    void MainWindow::resetDistanceTraveled(){
        ui->acceleration_distanceTraveled_inputInitalSpeed->setText("");
        ui->acceleration_distanceTraveled_inputDistance->setText("");
        ui->acceleration_distanceTraveled_inputTime->setText("");
        ui->acceleration_distanceTraveled_inputAcceleration->setText("");
    }

    #pragma}

    #pragma Force and Mass{

    void MainWindow::setupUiForceAndMass(){
        populateUnits(ui->acceleration_forceAndMass_unitMass, findUnitMap("g"), "g", "g");
        populateUnits(ui->acceleration_forceAndMass_unitNetForce, findUnitMap("N"), "N", "N");
        populateUnits(ui->acceleration_forceAndMass_unitAcceleration, findUnitMap("m/s^2"), "m/s^2", "m/s^2");

    }

    void MainWindow::calcForceAndMass(){
        //get the value with the unit
        float mass = getConvertedInput(ui->acceleration_forceAndMass_inputMass->text().toFloat(), ui->acceleration_forceAndMass_unitMass->currentText()) * 0.001; // *0.001 because kg by default for calc
        float netForce = getConvertedInput(ui->acceleration_forceAndMass_inputNetForce->text().toFloat(), ui->acceleration_forceAndMass_unitNetForce->currentText());
        float acceleration = getConvertedInput(ui->acceleration_forceAndMass_inputAcceleration->text().toFloat(), ui->acceleration_forceAndMass_unitAcceleration->currentText());

        // Calculate missing values when 2 of the 3 values are provided
        QString resultText;

        if (mass > 0 && netForce > 0) {
            acceleration = netForce / mass;
            resultText = "Acceleration: " + QString::number(acceleration) + " m/s^2";
        } else if (mass > 0 && acceleration > 0) {
            netForce = mass * acceleration;
            resultText = "Net Force: " + QString::number(netForce) + " N";
        } else if (netForce > 0 && acceleration > 0) {
            mass = netForce / acceleration;
            resultText = "Mass: " + QString::number(mass) + " kg";
        } else {
            resultText = "Please provide at least two values.";
        }

        // Display the result
        msgBox.setText(resultText);
        msgBox.exec();
    }

    void MainWindow::resetForceAndMass(){
        ui->acceleration_forceAndMass_inputMass->setText("");
        ui->acceleration_forceAndMass_inputNetForce->setText("");
        ui->acceleration_forceAndMass_inputAcceleration->setText("");
    }

    #pragma}

    #pragma Constant Aceceleration{
    void MainWindow::setupUiConstantAcceleration(){
        populateUnits(ui->acceleration_constantAcceleration_unitDisplacement, findUnitMap("m"), "m", "m");
        populateUnits(ui->acceleration_constantAcceleration_unitInitialVelocity, findUnitMap("m/s"), "m/s", "m/s");
        populateUnits(ui->acceleration_constantAcceleration_unitFinalVelocity, findUnitMap("m/s"), "m/s", "m/s");
        populateUnits(ui->acceleration_constantAcceleration_unitElapsedTime, findUnitMap("s"), "s", "s");
        populateUnits(ui->acceleration_constantAcceleration_unitConstantAcceleration, findUnitMap("m/s^2"), "m/s^2", "m/s^2");

    }

    void MainWindow::knownParameters(QString knownParam){

        ui->acceleration_constantAcceleration_inputDisplacement->setEnabled(true);
        ui->acceleration_constantAcceleration_inputInitialVelocity->setEnabled(true);
        ui->acceleration_constantAcceleration_inputFinalVelocity->setEnabled(true);
        ui->acceleration_constantAcceleration_inputConstantAcceleration->setEnabled(true);
        ui->acceleration_constantAcceleration_inputElapsedTime->setEnabled(true);

        if(knownParam == "Time, Inital Velocity, Final Velocity"){
            ui->acceleration_constantAcceleration_inputDisplacement->setEnabled(false);
            ui->acceleration_constantAcceleration_inputConstantAcceleration->setEnabled(false);
        }else if(knownParam == "Acceleration, Initial Veloctiy, Final Velocity"){
            ui->acceleration_constantAcceleration_inputDisplacement->setEnabled(false);
            ui->acceleration_constantAcceleration_inputElapsedTime->setEnabled(false);
        }else if(knownParam == "Time, Acceleration, Final Velocity"){
            ui->acceleration_constantAcceleration_inputDisplacement->setEnabled(false);
            ui->acceleration_constantAcceleration_inputInitialVelocity->setEnabled(false);
        }else if(knownParam == "Time, Acceleration, Initial Velocity"){
            ui->acceleration_constantAcceleration_inputDisplacement->setEnabled(false);
            ui->acceleration_constantAcceleration_inputFinalVelocity->setEnabled(false);
        }else if(knownParam == "Time, Initial Velocity, Displacement"){
            ui->acceleration_constantAcceleration_inputFinalVelocity->setEnabled(false);
            ui->acceleration_constantAcceleration_inputConstantAcceleration->setEnabled(false);
        }else if(knownParam == "Time, Final Velocity, Displacement"){
            ui->acceleration_constantAcceleration_inputInitialVelocity->setEnabled(false);
            ui->acceleration_constantAcceleration_inputConstantAcceleration->setEnabled(false);
        }else if(knownParam == "Acceleration, Initial Velocity, Displacement"){
            ui->acceleration_constantAcceleration_inputFinalVelocity->setEnabled(false);
            ui->acceleration_constantAcceleration_inputElapsedTime->setEnabled(false);
        }else if(knownParam == "Acceleration, Final Velocity, Displacement"){
            ui->acceleration_constantAcceleration_inputInitialVelocity->setEnabled(false);
            ui->acceleration_constantAcceleration_inputElapsedTime->setEnabled(false);
        }
    }

    void MainWindow::calcConstantAcceleration(){
        float displacement = getConvertedInput(ui->acceleration_constantAcceleration_inputDisplacement->text().toFloat(), ui->acceleration_constantAcceleration_unitDisplacement->currentText());
        float initialVelocity = getConvertedInput(ui->acceleration_constantAcceleration_inputInitialVelocity->text().toFloat(), ui->acceleration_constantAcceleration_unitInitialVelocity->currentText());
        float finalVelocity = getConvertedInput(ui->acceleration_constantAcceleration_inputFinalVelocity->text().toFloat(), ui->acceleration_constantAcceleration_unitFinalVelocity->currentText());
        float elapsedTime = getConvertedInput(ui->acceleration_constantAcceleration_inputElapsedTime->text().toFloat(), ui->acceleration_constantAcceleration_unitElapsedTime->currentText());
        float acceleration = getConvertedInput(ui->acceleration_constantAcceleration_inputConstantAcceleration->text().toFloat(), ui->acceleration_constantAcceleration_unitConstantAcceleration->currentText());

        QString resultText;

        QString knownParam = ui->acceleration_choiceConstantAcceleration->currentText();

        if (knownParam == "Time, Inital Velocity, Final Velocity") {
            acceleration = (finalVelocity - initialVelocity) / elapsedTime;
            displacement = (initialVelocity + finalVelocity) * elapsedTime / 2;
            resultText = "Displacement: " + QString::number(displacement) + " m\n";
            resultText += "Acceleration: " + QString::number(acceleration) + " m/s^2";
        } else if (knownParam == "Acceleration, Initial Veloctiy, Final Velocity") {
            elapsedTime = (finalVelocity - initialVelocity) / acceleration;
            displacement = (initialVelocity + finalVelocity) * elapsedTime / 2;
            resultText = "Displacement: " + QString::number(displacement) + " m\n";
            resultText += "Time: " + QString::number(elapsedTime) + " s";
        } else if (knownParam == "Time, Acceleration, Final Velocity") {
            initialVelocity = finalVelocity - acceleration * elapsedTime;
            displacement = (initialVelocity + finalVelocity) * elapsedTime / 2;
            resultText = "Displacement: " + QString::number(displacement) + " m\n";
            resultText += "Initial Velocity: " + QString::number(initialVelocity) + " m/s";
        } else if (knownParam == "Time, Acceleration, Initial Velocity") {
            finalVelocity = initialVelocity + acceleration * elapsedTime;
            displacement = (initialVelocity + finalVelocity) * elapsedTime / 2;
            resultText = "Displacement: " + QString::number(displacement) + " m\n";
            resultText += "Final Velocity: " + QString::number(finalVelocity) + " m/s";
        } else if (knownParam == "Time, Initial Velocity, Displacement") {
            acceleration = (2 * (displacement - initialVelocity * elapsedTime)) / (elapsedTime * elapsedTime);
            finalVelocity = initialVelocity + acceleration * elapsedTime;
            resultText = "Final Velocity: " + QString::number(finalVelocity) + " m/s\n";
            resultText += "Acceleration: " + QString::number(acceleration) + " m/s^2";
        } else if (knownParam == "Time, Final Velocity, Displacement") {
            initialVelocity = 2 * displacement / elapsedTime - finalVelocity;
            acceleration = (finalVelocity - initialVelocity) / elapsedTime;
            resultText = "Initial Velocity: " + QString::number(initialVelocity) + " m/s\n";
            resultText += "Acceleration: " + QString::number(acceleration) + " m/s^2";
        }else if (knownParam == "Acceleration, Initial Velocity, Displacement") {
            initialVelocity = finalVelocity - 2 * acceleration * displacement;
            acceleration = (finalVelocity * finalVelocity - initialVelocity * initialVelocity) / (2 * displacement);
            resultText = "Initial Velocity: " + QString::number(initialVelocity) + " m/s\n";
            resultText += "Acceleration: " + QString::number(acceleration) + " m/s^2";
        } else if (knownParam == "Acceleration, Final Velocity, Displacement") {
            elapsedTime = 2 * displacement / (finalVelocity + sqrt(finalVelocity * finalVelocity - 2 * acceleration * displacement));
            initialVelocity = finalVelocity - acceleration * elapsedTime;
            resultText = "Initial Velocity: " + QString::number(initialVelocity) + " m/s\n";
            resultText += "Time: " + QString::number(elapsedTime) + " s";
        }

        // Display the result
        msgBox.setText(resultText);
        msgBox.exec();
    }

    void MainWindow::resetConstantAcceleration(){
        ui->acceleration_constantAcceleration_inputDisplacement->setText("");
        ui->acceleration_constantAcceleration_inputInitialVelocity->setText("");
        ui->acceleration_constantAcceleration_inputFinalVelocity->setText("");
        ui->acceleration_constantAcceleration_inputElapsedTime->setText("");
        ui->acceleration_constantAcceleration_inputConstantAcceleration->setText("");
    }

    #pragma}

#pragma}

#pragma Electricity Cost{

    // Sets the wattage input field value based on the selected appliance
    void MainWindow::setWattByApplicance(QString appliance){
        int wattage = elecObject->electronics.key(appliance);
        ui->electricityCost_wattageInput->setText(QString::number(wattage));
    }

    // Resets the electricity cost input fields and selected appliance
    void MainWindow::resetElecCost(){
        ui->electricityCost_dailyUsageInput->setText(0);
        ui->electricityCost_costInput->setText(0);
        setWattByApplicance("Other");
        ui->electricity_appliance->setCurrentText("Other");
    }

    // Sets up the electricity cost UI components
    void MainWindow::setupElecCostUI(){
        populateUnits(ui->electricCost_unitCostPerTime_2, findUnitMap("month"), "month", "month");
        ui->electricCost_unitCostPerTime_2->hide();
    }

    // Calculates the electricity cost based on the selected radio button
    void MainWindow::calcElecCost() {
        // Check if the simple cost radio button is selected
        if (ui->radioButton_simpleElectricCOst->isChecked()) {
            calculateSimpleElectricityCost();
        } else {
            calculateComplexElectricityCost();
        }
    }

    // Calculates the simple electricity cost based on user inputs
    void MainWindow::calculateSimpleElectricityCost() {
        // Get power consumption, usage time, and energy price from user inputs
        float powerConsumption = getConvertedInput(ui->electricityCost_wattageInput->text().toFloat(), ui->electricCost_unitPowerConsuption->currentText()); // in Watt
        float usageTime = getConvertedInput(ui->electricityCost_dailyUsageInput->text().toFloat(), ui->electricCost_unitCostPerTime_2->currentText()); // in month
        float energyPrice = ui->electricityCost_costInput->text().toFloat(); // in kWh

        // Calculate power consumed and total cost
        float powerConsumed = round(((usageTime * powerConsumption / 1000) * 100)) / 100;
        float totalCost = round(powerConsumed * energyPrice * 100) / 100;

        // Format the result text and display it in a message box
        QString resultText = QString("Power consumed = %1 (kWh per month)\nCost = %2 ($ per month)")
                                 .arg(powerConsumed)
                                 .arg(totalCost);

        msgBox.setText(resultText);
        msgBox.exec();
    }

    // Calculates the complex electricity cost based on user inputs
    void MainWindow::calculateComplexElectricityCost() {
        // Get power consumption, usage time, and energy price from user inputs
        float powerConsumption = ui->electricityCost_wattageInput->text().toFloat(); // in W
        float usageTime = ui->electricityCost_dailyUsageInput->text().toFloat(); // hours per day
        float energyPrice = ui->electricityCost_costInput->text().toFloat();

        // Calculate power consumed and total cost
        const float hoursPerMonth = 30.44;
        float powerConsumed = round(((usageTime * hoursPerMonth) * powerConsumption / 1000) * 100) / 100;
        float totalCost = round(powerConsumed * energyPrice * 100) / 100;

        // Format the result text and display it in a message box
        QString resultText = QString("Power consumed = %1 (kWh per month)\nCost = %2 ($ per month)")
                                 .arg(powerConsumed)
                                 .arg(totalCost);

        msgBox.setText(resultText);
        msgBox.exec();
    }
#pragma}

#pragma unit{
    //Détermine l'exposant approprié pour une valeur donnée et ajuste la valeur en conséquence.
    QPair<double, int> MainWindow::calcUnit(double nb){
        // Initialise l'exposant
        int exp = 0;

        // Si nb >= 100, réduire nb et augmenter exp
           if(nb >= 100){
                while(nb >= 100){
                   nb /= 10;
                   exp++;
               };

               while(!unity->basicUnit.contains(exp)){
                   nb /= 10;
                   exp++;
               }
           } else if(nb < 1){ // Si nb < 1, augmenter nb et réduire exp
               while(nb < 1){
                   nb *= 10;
                   exp--;
               }
               while(!unity->basicUnit.contains(exp)){
                   nb *= 10;
                   exp--;
               }
           }

           // Retourner un QPair avec nb et exp ajustés
           return qMakePair(nb, exp);
    }

    //Trouve la QMap appropriée en fonction de l'unité de mesure saisie.
    QMap<double, QString>* MainWindow::findUnitMap(const QString &inputUnit) {
        if(unity->timeUnit.values().contains(inputUnit)) {
            return &unity->timeUnit;
        }else if (unity->speedUnit.values().contains(inputUnit)) {
            return &unity->speedUnit;
        }else if(unity->momentOfInertiaUnit.values().contains(inputUnit)){
            return &unity->momentOfInertiaUnit;
        }else if(unity->longTimeUnit.values().contains(inputUnit)){
            return &unity->longTimeUnit;
        }else if(unity->accelerationUnit.values().contains(inputUnit)){
            return &unity->accelerationUnit;
        }
        // Ajoutez d'autres conditions pour d'autres QMap ici
        return &unity->basicUnit;
    }

    //Peuple les comboboxes avec les unités appropriées en fonction de la QMap trouvée précédemment.
    void MainWindow::populateUnits(QComboBox *comboBox, const QMap<double, QString> *unitMap, const QString &defaultUnit, QString suffixe) {
        comboBox->clear();
        bool isBasicUnit = false;

        //check if it's a basic unit to add sufixwe in cbbox
        if (unitMap->size() != unity->basicUnit.size()) {
            isBasicUnit = false;
        } else {
            isBasicUnit = true;
            auto it1 = unitMap->constBegin();
            auto it2 = unity->basicUnit.constBegin();
            while (it1 != unitMap->constEnd() && it2 != unity->basicUnit.constEnd()) {
                if (it1.key() != it2.key() || it1.value() != it2.value()) {
                    isBasicUnit = false;
                    break;
                }
                ++it1;
                ++it2;
            }
        }

        // If defaultUnit is none, hide the comboBox
        if (defaultUnit == "none") {
            comboBox->hide();
        } else {
            for (const auto &unit : unitMap->values()) {
                if (isBasicUnit) {
                    comboBox->addItem(unit + suffixe);
                } else {
                    comboBox->addItem(unit);
                }
            }
            comboBox->setCurrentText(defaultUnit);
            comboBox->show();
        }
    }

    //Convertit la valeur saisie dans l'unité de mesure choisie en unité de mesure SI.
    float MainWindow::getConvertedInput(float inputValue, const QString &inputUnit) {
        float input_SI;

        if (unity->timeUnit.values().contains(inputUnit)
                || unity->speedUnit.values().contains(inputUnit)
                || unity->momentOfInertiaUnit.values().contains(inputUnit)
                || unity->longTimeUnit.values().contains(inputUnit)
                || unity->accelerationUnit.values().contains(inputUnit))
        {
            input_SI = unity->convertUnits(inputValue, inputUnit);
        } else {
            //only distance is basic here
            QString inputUnitCopy = inputUnit;
            inputUnitCopy.chop(1);
            input_SI = inputValue * pow(10, unity->basicUnit.key(inputUnitCopy));
        }
        return input_SI;
    }

    // Populate combo boxes with basic units
    void MainWindow::populateBasicUnitComboBoxes() {
        for (auto key : unity->basicUnit.keys()) {
            ui->ohmLaw_unitA->addItem(unity->basicUnit.value(key) + "A");
            ui->ohmLaw_unitV->addItem(unity->basicUnit.value(key) + "V");
            ui->ohmLaw_unitW->addItem(unity->basicUnit.value(key) + "W");
            ui->ohmLaw_unitohm->addItem(unity->basicUnit.value(key) + "Ώ");
            ui->electricCost_unitPowerConsuption->addItem(unity->basicUnit.value(key) + "W");
        }
    }
#pragma}

MainWindow::~MainWindow() {
    delete ui;
    delete unity;
    delete elecObject;
}

