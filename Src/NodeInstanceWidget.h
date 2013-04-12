///
/// Copyright (C) 2013 - All Rights Reserved
/// All rights reserved. http://www.equals-forty-two.com
///
/// @brief Single node instance widget
///

#pragma once

#include "nodeinstance.h"

#include <QWidget>

#include <functional>

class NodeInstanceWidget : public QWidget
{
	Q_OBJECT;

public:
	
	NodeInstanceWidget(NodeInstance* instance, QWidget *parent = 0);
	~NodeInstanceWidget();

	void SetPortWarning();
	void ClearPortWarning();

protected:

	virtual void resizeEvent(QResizeEvent* event) override;

protected Q_SLOTS:

	void OnScriptPathEdited();
	void OnBrowseScript();
	void OnValidateScriptPath(const QString& path);
	void OnPortEdited();
	void OnNodeStateToggled();
	void OnNodeStateChanged(bool);

Q_SIGNALS:

	void PortEdited(int);

private: // Details

	void AddAction(const QString& actionName, std::function<void()> actionCallback);
	void AddActionOpenBrowser();
	void AddActionOpenExplorer();
	void AddActionEditEnvVars();
	void AddActionLog();
	void AddActionDebug();
	void AddActionDelete();

private: // Controls
	
	QLineEdit*   mScriptPathEdit;
	QPushButton* mScriptBrowseButton;
	QLineEdit*   mPortEdit;
	QPushButton* mStateButton;
	QToolButton* mConfigButton;

private: // Data

	NodeInstance* mNodeInstance;

};
