#include "calculator.h"
#include "ui_calculator.h"
#include <iostream>

double calcVal = 0.0;
bool divTrigger = false;
bool multTrigger = false;
bool addTrigger = false;
bool subTrigger = false;

Calculator::Calculator(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Calculator)
{
    ui->setupUi(this);
    // init everything
    ui->Display->setText(QString::number(calcVal));
    QPushButton* numButtons[10];
    for(int i = 0; i < 10; ++i) {
        QString butName = "Button" + QString::number(i);
        // add them to array
        numButtons[i] = Calculator::findChild<QPushButton *>(butName);
        connect(numButtons[i], SIGNAL(released()), this,
                SLOT(NumPressed()));
    }
    // connect to the buttons to their function slots
    connect(ui->Add, SIGNAL(released()), this,
            SLOT(MathButtonPressed()));
    connect(ui->Subtract, SIGNAL(released()), this,
            SLOT(MathButtonPressed()));
    connect(ui->Multiply, SIGNAL(released()), this,
            SLOT(MathButtonPressed()));
    connect(ui->Divide, SIGNAL(released()), this,
            SLOT(MathButtonPressed()));

    connect(ui->Equals, SIGNAL(released()), this,
            SLOT(EqualButtonPressed()));

    connect(ui->ChangeSign, SIGNAL(released()), this,
            SLOT(ChangeNumberSign()));

    connect(ui->MemAdd, SIGNAL(released()), this,
            SLOT(MemButtonPressed()));
    connect(ui->MemSub, SIGNAL(released()), this,
            SLOT(MemButtonPressed()));
    connect(ui->MemStore, SIGNAL(released()), this,
            SLOT(MemButtonPressed()));

    connect(ui->Clear, SIGNAL(released()), this,
            SLOT(ClearButtonPressed()));

    storedVal = 0.0;
}

Calculator::~Calculator()
{
    delete ui;
}

void Calculator::NumPressed() {
    QPushButton *button = (QPushButton *)sender();
    QString butVal = button->text();
    QString displayVal = ui->Display->text();
    if((displayVal.toDouble() == 0) || (displayVal.toDouble() == 0.0)) {
        ui->Display->setText(butVal);
    }
    else {
        QString newVal = displayVal + butVal;
        double dblNewVal = newVal.toDouble();
        ui->Display->setText(QString::number(dblNewVal, 'g', 16));

    }
}

void Calculator::MathButtonPressed() {
    // Reset all to false
    divTrigger = false;
    multTrigger = false;
    addTrigger = false;
    subTrigger = false;
    // get the text out of display
    QString displayVal = ui->Display->text();
    calcVal = displayVal.toDouble();
    // Check sender to find which math button was clicked
    QPushButton *button = (QPushButton *)sender();
    QString butVal = button->text();
    if(QString::compare(butVal, "/", Qt::CaseInsensitive) == 0) {
        divTrigger = true;
    }
    else if(QString::compare(butVal, "*", Qt::CaseInsensitive) == 0) {
        multTrigger = true;
    }
    else if(QString::compare(butVal, "+", Qt::CaseInsensitive) == 0) {
        addTrigger = true;
    }
    else { // Subtract
        subTrigger = true;
    }
    ui->Display->setText("");
}

void Calculator::EqualButtonPressed() {
    double solution = 0.0;
    QString displayVal = ui->Display->text();
    double dblDisplayVal = displayVal.toDouble();
    if(addTrigger || subTrigger || multTrigger || divTrigger) {
        if(addTrigger) {
            solution = calcVal + dblDisplayVal;
        }
        else if(subTrigger) {
            solution = calcVal - dblDisplayVal;
        }
        else if(multTrigger) {
            solution = calcVal * dblDisplayVal;
        }
        else { // divTrigger
            solution = calcVal / dblDisplayVal;
        }
    }
    ui->Display->setText(QString::number(solution));
}

void Calculator::ChangeNumberSign() {
    QString displayVal = ui->Display->text();
    QRegExp reg("[-]?[0-9.]*");
    if(reg.exactMatch(displayVal)) {
        double dblDisplayVal = displayVal.toDouble();
        double dblDisplayValSign = -1 * dblDisplayVal;
        ui->Display->setText(QString::number(dblDisplayValSign));
    }
}

void Calculator::MemButtonPressed() {
    // get the text out of display
    QString displayVal = ui->Display->text();
    calcVal = displayVal.toDouble();

    QPushButton *button = (QPushButton *)sender();
    QString butVal = button->text();
    if(QString::compare(butVal, "M+", Qt::CaseInsensitive) == 0) {
        calcVal = calcVal + storedVal;
        std::cout << calcVal << std::endl;
    }
    else if(QString::compare(butVal, "M-", Qt::CaseInsensitive) == 0) {
        calcVal = calcVal - storedVal;
    }
    else { // store the value
        storedVal = calcVal;
        std::cout << storedVal << std::endl;
    }

    ui->Display->setText(QString::number(calcVal));
}

void Calculator::ClearButtonPressed() {
    ui->Display->setText(QString::number(0));
    storedVal = 0;
}
