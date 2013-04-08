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

protected:

	virtual void resizeEvent(QResizeEvent* event) override;

protected Q_SLOTS:

	void OnScriptPathEdited();

private: // Controls
	
	QLineEdit*   mScriptPathEdit;
	QPushButton* mScriptBrowse;
	QLineEdit*   mPortEdit;
	QPushButton* mStateButton;
	QPushButton* mConfigButton;

private: // Data

	NodeInstance* mInstance;

};
