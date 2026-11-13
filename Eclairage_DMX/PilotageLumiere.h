#pragma once

#include <QSerialPort>
#include <QSerialPortInfo>
#include <qdebug.h>
#include <Qstring.h>

class PilotageLumiere{
	
public:
	void open_port(const QString &portName);
};

