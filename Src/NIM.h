///
/// Copyright (C) 2013 - All Rights Reserved
/// All rights reserved. http://www.equals-forty-two.com
///
/// @brief Main window
///

#pragma once

#include "NodeInstanceManager.h"

#include "ui_NIM.h"

#include <QtGui/QDialog>
#include <QSystemTrayIcon>

class NodeInstanceWidget;

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
	void OnGenerateTrayIcon();
	void OnDeleteNodeInstance(NodeInstance*);

protected:

	virtual void closeEvent(QCloseEvent* event) override;
	virtual void changeEvent(QEvent* event) override;
	virtual void resizeEvent(QResizeEvent* event) override;

private: // Implementation

	void                CreateUI();
	void                PopulateUI();
	void                SetConnections();
	void                AddNewNodeInstanceWidget();
	NodeInstanceWidget* InsertNodeInstanceWidget(int idx, NodeInstance* nodeInstance);
	void                ValidatePorts();
	void                CreateTrayIcon();
	void                RestoreWindowSettings();
	void                SaveWindowSettings();
	void                UpdateTrayIconState();

private: // Controls

	Ui::NIMClass        mUI;
	QSystemTrayIcon     mTrayIcon;
	QVBoxLayout*        mInstanceLayout;
	QPushButton*        mAddInstanceButton;

private: // Data

	NodeInstanceManager mNodeInstanceManager;

};
