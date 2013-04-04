
#include "stdafx.h"
#include "nim.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	NIM w;
	w.show();
	return a.exec();
}
