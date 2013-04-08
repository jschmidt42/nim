///
/// Copyright (C) 2013 - All Rights Reserved
/// All rights reserved. http://www.equals-forty-two.com
///
/// @brief Single node instance configuration widget implementation
///

#include "stdafx.h"
#include "nodeinstancewidget.h"

#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>

NodeInstanceWidget::NodeInstanceWidget(NodeInstance* instance, QWidget *parent)
	: QWidget(parent)
	, mInstance(instance)
{
	Q_ASSERT( instance );

	QHBoxLayout* mainLayout = new QHBoxLayout();
	mainLayout->setSpacing( 2 );
	mainLayout->setMargin(0);

	//
	// Script path
	//
	mScriptPathEdit = new QLineEdit( instance->GetScriptPath() );

	mScriptBrowse = new QPushButton("...", mScriptPathEdit);
	mScriptBrowse->setFixedSize( 20, 20 );
	mScriptBrowse->move(100, 0);

	//
	// Port
	//
	mPortEdit = new QLineEdit();
	mPortEdit->setFixedWidth( 45 );

	//
	// Running state
	//
	mStateButton = new QPushButton(">");
	mStateButton->setFixedWidth( 20 );

	//
	// Configuration dropdown
	//
	mConfigButton = new QPushButton("C");
	mConfigButton->setFixedWidth( 20 );
	
	//
	// Layout
	//
	mainLayout->addWidget( mScriptPathEdit );
	mainLayout->addWidget( mPortEdit );
	mainLayout->addWidget( mStateButton );
	mainLayout->addWidget( mConfigButton );

	// Create connections
	connect( mScriptPathEdit, SIGNAL(editingFinished()), this, SLOT(OnScriptPathEdited()) );

	setLayout( mainLayout );
}

NodeInstanceWidget::~NodeInstanceWidget()
{
}

void NodeInstanceWidget::resizeEvent(QResizeEvent* event)
{
	QWidget::resizeEvent( event );

	mScriptBrowse->move( mScriptPathEdit->width() - mScriptBrowse->width(), 0 );

	mStateButton->setFixedHeight( mScriptPathEdit->height() );
	mConfigButton->setFixedHeight( mScriptPathEdit->height() );
}

void NodeInstanceWidget::OnScriptPathEdited()
{
	mInstance->SetScriptPath( mScriptPathEdit->text() );
}
