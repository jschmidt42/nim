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
#include <QSystemTrayIcon>

class NIM : public QDialog
{
	Q_OBJECT;

public:
	NIM(QWidget *parent = 0, Qt::WFlags flags = 0);
	~NIM();

protected Q_SLOTS:
	
	void OnAddInstance();
	void OnValidatePorts();
	void OnTrayActivated(QSystemTrayIcon::ActivationReason reason);
	void OnCountActiveNodes();

protected:

	virtual void closeEvent(QCloseEvent* event);

private: // Implementation

	void CreateUI();
	void PopulateUI();
	void SetConnections();
	void AddNewNodeInstanceWidget();
	void InsertNodeInstanceWidget(int idx, NodeInstance* nodeInstance);
	void ValidatePorts();
	void CreateTrayIcon();

private: // Controls

	Ui::NIMClass        mUI;
	QSystemTrayIcon     mTrayIcon;
	QVBoxLayout*        mInstanceLayout;
	QPushButton*        mAddInstanceButton;

private: // Data

	NodeInstanceManager mInstanceManager;

};
