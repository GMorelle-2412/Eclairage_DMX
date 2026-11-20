#pragma once

#include <QSerialPort>
#include <QSerialPortInfo>
#include <qdebug.h>
#include <Qstring.h>
#include <windows.h>      // nécessaire : le .h utilise WORD
#include <QLibrary>
#include <QDebug>
#include "Include/_DasHard.h"  

class PilotageLumiere
{
public:
    PilotageLumiere();
    ~PilotageLumiere();

    bool charger(const QString& chemin = "DasHard2006");   // sans extension
    bool ouvrir();
    void fermer();

    // canal : 1 à 512, valeur : 0 à 255 (modifie le tampon, n'envoie pas)
    void setCanal(int canal, unsigned char valeur);

    // envoie les 512 canaux à l'interface
    int envoyer();

    bool estOuvert() const { return m_ouvert; }

private:
    typedef int (*DasUsbCommandFn)(int, int, unsigned char*);

    QLibrary m_lib;
    DasUsbCommandFn m_cmd = nullptr;
    bool m_ouvert = false;
    unsigned char m_canaux[512] = { 0 };
};

