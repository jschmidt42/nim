///
/// Copyright (C) 2013 - All Rights Reserved
/// All rights reserved. http://www.equals-forty-two.com
///
/// @brief Main window implementation
///

#include "stdafx.h"
#include "nim.h"
#include "nodeinstancewidget.h"

#include <QSettings>

///////////////////////////////////////////////////////////////////////
NIM::NIM(QWidget *parent, Qt::WFlags flags)
	: QDialog(parent, flags)
	, mInstanceLayout(nullptr)
{
	CreateUI();
	PopulateUI();
	SetConnections();
}

///////////////////////////////////////////////////////////////////////
NIM::~NIM()
{
}

///////////////////////////////////////////////////////////////////////
void NIM::CreateUI()
{
	mUI.setupUi(this);

	mInstanceLayout = new QVBoxLayout();
	mInstanceLayout->setSpacing(2);
	mInstanceLayout->setMargin(0);

	mInstanceLayout->addWidget( new QPushButton("Add"), 0, Qt::AlignRight );
	mInstanceLayout->addStretch(1);

	mUI.Instances->setLayout( mInstanceLayout );
}

///////////////////////////////////////////////////////////////////////
void NIM::PopulateUI()
{
	// Create node instance widgets and populate the UI
	for (int i = 0; i < mInstanceManager.GetInstanceCount(); ++i)
	{
		NodeInstance* newInstance = mInstanceManager.GetInstance(i);
		mInstanceLayout->insertWidget( i, new NodeInstanceWidget( newInstance ) );
	}

	// Add at least one more node instance slot for data entry
	NodeInstance* newInstance = mInstanceManager.CreateInstance();
	mInstanceLayout->insertWidget( mInstanceManager.GetInstanceCount()-1, new NodeInstanceWidget( newInstance ) );
}

void NIM::SetConnections()
{
	connect( this, SIGNAL(finished(int)), this, SLOT(OnExit(int)) );
	connect( mUI.ExitButton, SIGNAL(clicked()), this, SLOT(close()) );
}

void NIM::OnExit(int code)
{
}

void NIM::closeEvent(QCloseEvent* event)
{
	// TODO: Check if any node are running, if yes, pop an warning
	if(true)
	{
		event->accept();
	}
	else
	{
		event->ignore();
	}
}
