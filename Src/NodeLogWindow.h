///
/// Copyright (C) 2013 - All Rights Reserved
/// All rights reserved. http://www.equals-forty-two.com
///
/// @brief Simple environement variables editor
///

#pragma once

#include "NodeInstance.h"

#include <QDialog>
#include <QTimer>

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

	QString GetColorTag(const QString& code) const;
	QString ToHTML(const QString& log) const;

private: // Controls

	QTextEdit* mLog;
	QTimer     mQueryLogTimer;
	
private: // Data

	QString       mWindowId;
	NodeInstance* mNodeInstnace;

};
