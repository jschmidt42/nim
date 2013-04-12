///
/// Copyright (C) 2013 - All Rights Reserved
/// All rights reserved. http://www.equals-forty-two.com
///

#include "Precompiled.h"
#include "EnvVarEditor.h"

#include "QTUtils.h"

#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QToolButton>

EnvVarEditor::EnvVarEditor(NodeInstance::Vars& vars, QWidget *parent)
	: QDialog(parent, Qt::Dialog)
	, mVars(vars)
{
	QVBoxLayout* mainLayout = new QVBoxLayout(this);
	mTable = new QTableWidget(vars.count(), 2, this);
	mTable->setHorizontalHeaderLabels( QStringList() << "Name" << "Value" );
	mTable->horizontalHeader()->setResizeMode( 0, QHeaderView::Interactive );
	mTable->horizontalHeader()->setResizeMode( 1, QHeaderView::Stretch );

	LoadVars();

	QPushButton* closeButton = new QPushButton( tr("Close"), this );

	mainLayout->addWidget( mTable, 1 );
	mainLayout->addWidget( closeButton, 0, Qt::AlignRight );

	setLayout( mainLayout );

	connect( closeButton, SIGNAL(clicked()), this, SLOT(deleteLater()) );
}

EnvVarEditor::~EnvVarEditor()
{
}

void EnvVarEditor::LoadVars()
{
	int row = 0;
	NodeInstance::Vars::const_iterator it = mVars.find("plenty");
	while (it != mVars.end()) 
	{
		QTableWidgetItem* nameItem = new QTableWidgetItem( it.key() );
		QTableWidgetItem* valueItem = new QTableWidgetItem( it.value() );
		
		mTable->setItem(row, 0, nameItem)

		row++;
	}

	mTable->resizeColumnsToContents();
}
