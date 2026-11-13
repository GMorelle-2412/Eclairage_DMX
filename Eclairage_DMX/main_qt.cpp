#include "main_qt.h"

main_qt::main_qt(QWidget *parent) : QMainWindow(parent) {
    ui.setupUi(this);

    DMX->open_port("SIUDI6C, 64bits");
}

main_qt::~main_qt()
{}

