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

class QTableWidget;
class QTableWidgetItem;

class EnvVarEditor : public QDialog
{
	Q_OBJECT;

public:
	
	EnvVarEditor(NodeInstance::Vars& vars, QWidget *parent = 0);
	~EnvVarEditor();
	
protected: // Events

protected Q_SLOTS:

	void OnCellChanged(QTableWidgetItem* item);

Q_SIGNALS:

private: // Details

	void LoadVars();
	void AddNewEntry();

private: // Controls

	QTableWidget* mTable;
	
private: // Data

	NodeInstance::Vars& mVars;

};
