#include "PilotageLumiere.h"

void PilotageLumiere::open_port(const QString &portName) {
	QSerialPort* port = new QSerialPort();

    port->open(QIODevice::ReadWrite);
    port->setPortName(portName);
    port->setBaudRate(QSerialPort::Baud9600);
    port->setDataBits(QSerialPort::Data8);
    port->setParity(QSerialPort::NoParity);
    port->setStopBits(QSerialPort::OneStop);
    port->setFlowControl(QSerialPort::NoFlowControl);

    QByteArray data = port->readAll();
    qDebug() << "message :" << data;
};