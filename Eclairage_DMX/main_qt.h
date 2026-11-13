#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_main_qt.h"
#include "PilotageLumiere.h"
#include <QString>

using namespace std;

class main_qt : public QMainWindow
{
    Q_OBJECT

public:
    main_qt(QWidget *parent = nullptr);
    ~main_qt();

private:
    Ui::main_qtClass ui;
    PilotageLumiere *DMX;

    QString port;
};

