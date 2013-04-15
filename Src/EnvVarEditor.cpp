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
#include <QTableWidget>
#include <QTableWidgetItem>

namespace {

	const int kNameColumnIdx = 0;
	const int kValueColumnIdx = 1;

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
	mTable->horizontalHeader()->setResizeMode( kNameColumnIdx, QHeaderView::Interactive );
	mTable->horizontalHeader()->setResizeMode( kValueColumnIdx, QHeaderView::Stretch );
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

		mTable->setItem(row, kNameColumnIdx, nameItem);
		mTable->setItem(row, kValueColumnIdx, valueItem);

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

	if ( item->column() == kNameColumnIdx )
	{
		QString newKey = item->text();
		QString oldKey = item->data(kOriginalValueRole).toString();

		QTableWidgetItem* valueItem = mTable->item( item->row(), kValueColumnIdx );
		QString value = valueItem ? valueItem->text() : "";

		// Delete old key if found
		auto fItr = mVars.find( oldKey );
		if ( fItr != mVars.end() )
		{
			mVars.erase( fItr );
		}

		// Set new key and value
		if ( !newKey.isEmpty() )
		{
			// set new key
			mVars[ newKey ] = value;
			item->setData( kOriginalValueRole, newKey );

			if ( item->row() == mTable->rowCount() - 1 )
			{
				AddNewEntry();
			}
		}
	}
	else if ( item->column() == kValueColumnIdx )
	{
		QTableWidgetItem* keyItem = mTable->item( item->row(), kNameColumnIdx );
		if ( keyItem )
		{
			QString key = keyItem->text();
			if ( !key.isEmpty() )
			{
				mVars[ key ] = item->text();
			}
		}
	}
}
