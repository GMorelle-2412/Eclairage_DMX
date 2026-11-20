#include "main_qt.h"
#include "FenetreTest.h"

main_qt::main_qt(QWidget *parent) : QMainWindow(parent) {
    ui.setupUi(this);
    setCentralWidget(new FenetreTest(this));
}

main_qt::~main_qt()
{}

