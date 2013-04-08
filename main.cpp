///
/// Copyright (C) 2013 - All Rights Reserved
/// All rights reserved. http://www.equals-forty-two.com
///
/// @brief Main entry point.
///

#include "stdafx.h"
#include "nim.h"
#include "instancesettings.h"

#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	QCoreApplication::setOrganizationName("Equals42");
	QCoreApplication::setOrganizationDomain("equals-forty-two.com");
	QCoreApplication::setApplicationName("NIM");

	qRegisterMetaType<InstanceSettings>("InstanceSettings");
	qRegisterMetaTypeStreamOperators<InstanceSettings>("InstanceSettings");

	NIM w;
	w.show();
	return a.exec();
}
