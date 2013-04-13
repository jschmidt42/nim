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

namespace {

	const int kRowHeight = 20;

	const int kOriginalValueRole = Qt::UserRole+1;

}

EnvVarEditor::EnvVarEditor(NodeInstance::Vars& vars, QWidget *parent)
	: QDialog(parent, Qt::Dialog)
	, mVars(vars)
{
	QVBoxLayout* mainLayout = new QVBoxLayout(this);
	mTable = new QTableWidget(vars.count(), 2, this);
	mTable->setHorizontalHeaderLabels( QStringList() << "Name" << "Value" );
	mTable->horizontalHeader()->setResizeMode( 0, QHeaderView::Interactive );
	mTable->horizontalHeader()->setResizeMode( 1, QHeaderView::Stretch );
	mTable->verticalHeader()->hide();

	LoadVars();
	AddNewEntry();

	QPushButton* closeButton = new QPushButton( tr("Close"), this );

	mainLayout->addWidget( mTable, 1 );
	mainLayout->addWidget( closeButton, 0, Qt::AlignRight );

	setLayout( mainLayout );

	connect( closeButton, SIGNAL(clicked()), this, SLOT(deleteLater()) );
	connect( mTable, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(OnCellChanged(QTableWidgetItem*)) );
}

EnvVarEditor::~EnvVarEditor()
{
}

void EnvVarEditor::LoadVars()
{
	int row = 0;
	NodeInstance::Vars::const_iterator it = mVars.begin();
	while (it != mVars.end()) 
	{
		QTableWidgetItem* nameItem = new QTableWidgetItem( it.key() );
		QTableWidgetItem* valueItem = new QTableWidgetItem( it.value() );
		
		nameItem->setData( kOriginalValueRole, it.key() );
		valueItem->setData( kOriginalValueRole, it.value() );

		mTable->setItem(row, 0, nameItem);
		mTable->setItem(row, 1, valueItem);

		mTable->setRowHeight( row, kRowHeight );

		row++;
		it++;
	}

	mTable->resizeColumnsToContents();
}

void EnvVarEditor::AddNewEntry()
{
	mTable->insertRow( mTable->rowCount() );
	mTable->setRowHeight( mTable->rowCount()-1, kRowHeight );
}

void EnvVarEditor::OnCellChanged(QTableWidgetItem* item)
{
	QTUtils::BlockSignalGuard blockSignal( mTable );

	if ( item->column() == 0 )
	{
		QString newKey = item->text();
		QString oldKey = item->data(kOriginalValueRole).toString();
		QString oldValue;

		// get old value key
		auto fItr = mVars.find( oldKey );
		
		if ( fItr != mVars.end() )
		{
			oldValue = fItr.value();
			// delete old key
			mVars.erase( fItr );
		}

		if ( !newKey.isEmpty() )
		{
			// set new key
			mVars[ newKey ] = oldValue;
			item->setData( kOriginalValueRole, newKey );

			if ( item->row() == mTable->rowCount() - 1 )
			{
				AddNewEntry();
			}
		}
	}
	else if ( item->column() == 1 )
	{
		QString key = mTable->item( item->row(), 0 )->text();
		mVars[ key ] = item->text();
	}
}
