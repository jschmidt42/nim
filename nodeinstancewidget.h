///
/// Copyright (C) 2013 - All Rights Reserved
/// All rights reserved. http://www.equals-forty-two.com
///
/// @brief Single node instance widget
///

#ifndef NODEINSTANCEWIDGET_H
#define NODEINSTANCEWIDGET_H

#pragma once

#include <QWidget>

class NodeInstanceWidget : public QWidget
{
	Q_OBJECT;

public:
	NodeInstanceWidget(QWidget *parent = 0);
	~NodeInstanceWidget();

protected:

	virtual void resizeEvent(QResizeEvent* event) override;

private:
	
	QLineEdit*   mScriptPathEdit;
	QPushButton* mScriptBrowse;
	QLineEdit*   mPortEdit;
	QPushButton* mStateButton;
	QPushButton* mConfigButton;

};

#endif // NODEINSTANCEWIDGET_H
