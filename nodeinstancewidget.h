///
/// Copyright (C) 2013 - All Rights Reserved
/// All rights reserved. http://www.equals-forty-two.com
///
/// @brief Single node instance widget
///

#pragma once

#include "nodeinstance.h"

#include <QWidget>

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

Q_SIGNALS:

	void PortEdited(int);

private: // Controls
	
	QLineEdit*   mScriptPathEdit;
	QPushButton* mScriptBrowseButton;
	QLineEdit*   mPortEdit;
	QPushButton* mStateButton;
	QPushButton* mConfigButton;

private: // Data

	NodeInstance* mInstance;

};
