#include "FenetreTest.h"

#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QSpinBox>
#include <QSlider>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>

FenetreTest::FenetreTest(QWidget* parent)
    : QWidget(parent)
{
    setWindowTitle("Test pilotage DMX");

    // --- Connexion ---
    m_editDll = new QLineEdit("DasHard2006");
    m_btnConnexion = new QPushButton("Connecter");
    m_statut = new QLabel("Non connecté");

    auto* layoutConnexion = new QHBoxLayout;
    layoutConnexion->addWidget(new QLabel("DLL :"));
    layoutConnexion->addWidget(m_editDll);
    layoutConnexion->addWidget(m_btnConnexion);

    // --- Adresse DMX ---
    m_spinAdresse = new QSpinBox;
    m_spinAdresse->setRange(1, 512);
    m_spinAdresse->setValue(1);

    auto* layoutAdresse = new QHBoxLayout;
    layoutAdresse->addWidget(new QLabel("Adresse DMX de départ :"));
    layoutAdresse->addWidget(m_spinAdresse);
    layoutAdresse->addStretch();

    // --- Curseurs ---
    const char* noms[NB_CANAUX] = {
        "Canal 1 (Rouge)", "Canal 2 (Vert)", "Canal 3 (Bleu)", "Canal 4 (Blanc)",
        "Canal 5", "Canal 6", "Canal 7", "Canal 8"
    };

    auto* groupe = new QGroupBox("Canaux");
    auto* grille = new QGridLayout(groupe);

    for (int i = 0; i < NB_CANAUX; ++i) {
        m_sliders[i] = new QSlider(Qt::Horizontal);
        m_sliders[i]->setRange(0, 255);
        m_valeurs[i] = new QLabel("0");
        m_valeurs[i]->setMinimumWidth(30);

        grille->addWidget(new QLabel(noms[i]), i, 0);
        grille->addWidget(m_sliders[i], i, 1);
        grille->addWidget(m_valeurs[i], i, 2);

        connect(m_sliders[i], &QSlider::valueChanged, this, [this, i](int v) {
            m_valeurs[i]->setText(QString::number(v));
            appliquerCanaux();
            });
    }

    // --- Boutons rapides ---
    auto* btnBlackout = new QPushButton("Blackout");
    auto* btnPleinFeu = new QPushButton("Plein feu (canaux 1-4)");

    auto* layoutBoutons = new QHBoxLayout;
    layoutBoutons->addWidget(btnBlackout);
    layoutBoutons->addWidget(btnPleinFeu);

    // --- Mise en page générale ---
    auto* principal = new QVBoxLayout(this);
    principal->addLayout(layoutConnexion);
    principal->addLayout(layoutAdresse);
    principal->addWidget(groupe);
    principal->addLayout(layoutBoutons);
    principal->addWidget(m_statut);

    // --- Connexions ---
    connect(m_btnConnexion, &QPushButton::clicked, this, &FenetreTest::basculerConnexion);
    connect(btnBlackout, &QPushButton::clicked, this, &FenetreTest::blackout);
    connect(btnPleinFeu, &QPushButton::clicked, this, &FenetreTest::pleinFeu);
    connect(m_spinAdresse, &QSpinBox::valueChanged, this, &FenetreTest::appliquerCanaux);

    // Envoi régulier de la trame DMX (25 images/s)
    connect(&m_timer, &QTimer::timeout, this, [this]() {
        int r = m_lumiere.envoyer();
        if (r < 0)
            m_statut->setText(QString("Erreur d'envoi DMX (code %1)").arg(r));
        });
}

void FenetreTest::basculerConnexion()
{
    if (!m_lumiere.estOuvert()) {
        if (!m_lumiere.charger(m_editDll->text())) {
            m_statut->setText("Échec : DLL ou fonction introuvable");
            return;
        }
        if (!m_lumiere.ouvrir()) {
            m_statut->setText("Échec : interface DMX non détectée");
            return;
        }
        appliquerCanaux();
        m_timer.start(40);
        m_btnConnexion->setText("Déconnecter");
        m_statut->setText("Connecté, envoi DMX en cours");
    }
    else {
        m_timer.stop();
        m_lumiere.fermer();
        m_btnConnexion->setText("Connecter");
        m_statut->setText("Non connecté");
    }
}

void FenetreTest::appliquerCanaux()
{
    // On remet tout à zéro (au cas où l'adresse a changé), puis on réécrit les 8 canaux
    for (int c = 1; c <= 512; ++c)
        m_lumiere.setCanal(c, 0);

    const int debut = m_spinAdresse->value();
    for (int i = 0; i < NB_CANAUX; ++i) {
        const int canal = debut + i;
        if (canal <= 512)
            m_lumiere.setCanal(canal, static_cast<unsigned char>(m_sliders[i]->value()));
    }
}

void FenetreTest::blackout()
{
    for (int i = 0; i < NB_CANAUX; ++i)
        m_sliders[i]->setValue(0);
}

void FenetreTest::pleinFeu()
{
    for (int i = 0; i < 4; ++i)
        m_sliders[i]->setValue(255);
}
