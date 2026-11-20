#pragma once

#include <QWidget>
#include <QTimer>
#include "PilotageLumiere.h"

class QLineEdit;
class QPushButton;
class QLabel;
class QSpinBox;
class QSlider;

class FenetreTest : public QWidget
{
    Q_OBJECT

public:
    explicit FenetreTest(QWidget* parent = nullptr);

private slots:
    void basculerConnexion();
    void appliquerCanaux();
    void blackout();
    void pleinFeu();

private:
    static constexpr int NB_CANAUX = 8;

    PilotageLumiere m_lumiere;
    QTimer m_timer;

    QLineEdit* m_editDll;
    QPushButton* m_btnConnexion;
    QLabel* m_statut;
    QSpinBox* m_spinAdresse;
    QSlider* m_sliders[NB_CANAUX];
    QLabel* m_valeurs[NB_CANAUX];
};