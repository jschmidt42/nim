///
/// Copyright (C) 2013 - All Rights Reserved
/// All rights reserved. http://www.equals-forty-two.com
///
/// @brief Single node instance widget
///

#pragma once

#include "NodeInstance.h"

#include <QWidget>

#include <functional>

class QLineEdit;
class QPushButton;
class QToolButton;

class NodeInstanceWidget : public QWidget
{
	Q_OBJECT;

public:
	
	NodeInstanceWidget(NodeInstance* instance, QWidget *parent = 0);
	~NodeInstanceWidget();

	NodeInstance* Instance() const;

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
	void OnNodeDebugStateChanged(bool);
	void OnPortTextChanged(const QString&);

Q_SIGNALS:

	void PortEdited(int);
	void DeleteNodeInstance(NodeInstance*);

private: // Details

	QAction* AddAction(const QString& actionName, std::function<void()> actionCallback);
	QAction* AddActionOpenBrowser();
	QAction* AddActionOpenExplorer();
	QAction* AddActionEditEnvVars();
	QAction* AddActionLog();
	QAction* AddActionDebug();
	QAction* AddActionDelete();

	void UpdateStateButton();

private: // Controls
	
	QLineEdit*   mScriptPathEdit;
	QPushButton* mScriptBrowseButton;
	QLineEdit*   mPortEdit;
	QPushButton* mStateButton;
	QToolButton* mConfigButton;
	QAction*     mEnableDebuggingAction;

private: // Data

	NodeInstance* mNodeInstance;

};
