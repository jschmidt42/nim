///
/// Copyright (C) 2013 - All Rights Reserved
/// All rights reserved. http://www.equals-forty-two.com
///
/// @brief Simple environement variables editor
///

#pragma once

#include "NodeInstance.h"

#include <QDialog>

#include <functional>

class QTextEdit;

class NodeLogWindow : public QDialog
{
	Q_OBJECT;

public:
	
	NodeLogWindow(NodeInstance* nodeInstance, QWidget *parent = 0);
	~NodeLogWindow();
	
protected: // Events

protected Q_SLOTS:

Q_SIGNALS:

private: // Details

private: // Controls

	QTextEdit* mLog;
	
private: // Data

	NodeInstance* mNodeInstnace;

};
