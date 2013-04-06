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

NodeInstanceWidget::NodeInstanceWidget(QWidget *parent)
	: QWidget(parent)
{
	QHBoxLayout* mainLayout = new QHBoxLayout();
	mainLayout->setSpacing( 2 );
	mainLayout->setMargin(0);

	mScriptPathEdit = new QLineEdit();
	//mScriptPathEdit->setFixedWidth( 120 );

	mScriptBrowse = new QPushButton("...", mScriptPathEdit);
	mScriptBrowse->setFixedSize( 20, 20 );
	mScriptBrowse->move(100, 0);

	mPortEdit = new QLineEdit();
	mPortEdit->setFixedWidth( 45 );

	mStateButton = new QPushButton(">");
	mStateButton->setFixedWidth( 20 );

	mConfigButton = new QPushButton("C");
	mConfigButton->setFixedWidth( 20 );
	
	mainLayout->addWidget( mScriptPathEdit );
	mainLayout->addWidget( mPortEdit );
	mainLayout->addWidget( mStateButton );
	mainLayout->addWidget( mConfigButton );

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
