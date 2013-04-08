///
/// Copyright (C) 2013 - All Rights Reserved
/// All rights reserved. http://www.equals-forty-two.com
///
/// @brief Main window
///

#pragma once

#include "ui_nim.h"
#include "instancemanager.h"

#include <QtGui/QDialog>

class NIM : public QDialog
{
	Q_OBJECT;

public:
	NIM(QWidget *parent = 0, Qt::WFlags flags = 0);
	~NIM();

protected Q_SLOTS:

	void OnExit(int code);

protected:

	virtual void closeEvent(QCloseEvent* event);

private: // Implementation

	void CreateUI();
	void PopulateUI();
	void SetConnections();

private: // Controls

	Ui::NIMClass        mUI;
	QVBoxLayout*        mInstanceLayout;

private: // Data

	NodeInstanceManager mInstanceManager;

};
