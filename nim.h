#ifndef NIM_H
#define NIM_H

#include <QtGui/QDialog>
#include "ui_nim.h"

class NIM : public QDialog
{
	Q_OBJECT

public:
	NIM(QWidget *parent = 0, Qt::WFlags flags = 0);
	~NIM();

private:
	Ui::NIMClass ui;
};

#endif // NIM_H
