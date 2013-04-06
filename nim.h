///
/// Copyright (C) 2013 - All Rights Reserved
/// All rights reserved. http://www.equals-forty-two.com
///
/// @brief Main window
///

#ifndef NIM_H
#define NIM_H

#include <QtGui/QDialog>
#include "ui_nim.h"

class NIM : public QDialog
{
	Q_OBJECT;

public:
	NIM(QWidget *parent = 0, Qt::WFlags flags = 0);
	~NIM();

private: // Implementation

	void CreateUI();
	void PopulateUI();
	void SaveInstances();

private:

	Ui::NIMClass mUI;
	QVBoxLayout* mInstanceLayout;

	int          mInstanceSlotCount;
};

#endif // NIM_H
