#include "PilotageLumiere.h"
#include <QDebug>

PilotageLumiere::PilotageLumiere() {}

PilotageLumiere::~PilotageLumiere()
{
    fermer();
}

bool PilotageLumiere::charger(const QString& chemin)
{
    m_lib.setFileName(chemin);
    if (!m_lib.load()) {
        qDebug() << "Chargement DLL impossible :" << m_lib.errorString();
        return false;
    }

    m_cmd = (DasUsbCommandFn)m_lib.resolve("DasUsbCommand");
    if (!m_cmd) {
        qDebug() << "Fonction DasUsbCommand introuvable";
        return false;
    }
    return true;
}

bool PilotageLumiere::ouvrir()
{
    if (!m_cmd) return false;

    m_cmd(DHC_INIT, 0, nullptr);
    int r = m_cmd(DHC_OPEN, 0, nullptr);
    qDebug() << "DHC_OPEN ->" << r;

    m_ouvert = (r == DHE_OK || r == DHE_ERROR_ALREADYOPEN);
    return m_ouvert;
}

void PilotageLumiere::fermer()
{
    if (m_cmd && m_ouvert) {
        m_cmd(DHC_DMXOUTOFF, 0, nullptr);
        m_cmd(DHC_CLOSE, 0, nullptr);
        m_cmd(DHC_EXIT, 0, nullptr);
        m_ouvert = false;
    }
    if (m_lib.isLoaded())
        m_lib.unload();
}

void PilotageLumiere::setCanal(int canal, unsigned char valeur)
{
    if (canal >= 1 && canal <= 512)
        m_canaux[canal - 1] = valeur;
}

int PilotageLumiere::envoyer()
{
    if (!m_cmd || !m_ouvert) return DHE_ERROR_NOTOPEN;
    return m_cmd(DHC_DMXOUT, 512, m_canaux);
}