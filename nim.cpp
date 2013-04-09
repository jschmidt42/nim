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

	resize( QSettings().value( "width", 250 ).toInt(), QSettings().value( "height", 300 ).toInt() );
}

///////////////////////////////////////////////////////////////////////
NIM::~NIM()
{
	QSettings().setValue( "width", width() );
	QSettings().setValue( "height", height() );
}

///////////////////////////////////////////////////////////////////////
void NIM::CreateUI()
{
	mUI.setupUi(this);

	mInstanceLayout = new QVBoxLayout();
	mInstanceLayout->setSpacing(2);
	mInstanceLayout->setMargin(0);

	mAddInstanceButton = new QPushButton( QIcon(":/NIM/Resources/add-node.png"), "Add");
	mInstanceLayout->addWidget( mAddInstanceButton, 0, Qt::AlignRight );
	mInstanceLayout->addStretch(1);

	mUI.Instances->setLayout( mInstanceLayout );
}

///////////////////////////////////////////////////////////////////////
void NIM::PopulateUI()
{
	// Create node instance widgets and populate the UI
	for (int i = 0; i < mInstanceManager.GetInstanceCount(); ++i)
	{
		NodeInstance* nodeInstance = mInstanceManager.GetInstance(i);
		InsertNodeInstanceWidget( i, nodeInstance );
	}

	// Add at least one more node instance slot for data entry
	AddNewNodeInstanceWidget();

	ValidatePorts();
}

void NIM::SetConnections()
{
	// Add some global actions
	QAction* insertNodeAction = new QAction( tr("add new node"), this );
	insertNodeAction->setShortcut( QKeySequence(Qt::Key_Insert) );
	addAction( insertNodeAction );

	// Do UI connections
	connect( mUI.ExitButton, SIGNAL(clicked()), this, SLOT(close()) );
	connect( mAddInstanceButton, SIGNAL(clicked()), this, SLOT(OnAddInstance()) );
	connect( insertNodeAction, SIGNAL(triggered()), this, SLOT(OnAddInstance()) );
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

void NIM::OnAddInstance()
{
	AddNewNodeInstanceWidget();
}

void NIM::AddNewNodeInstanceWidget()
{
	NodeInstance* newInstance = mInstanceManager.CreateInstance();
	InsertNodeInstanceWidget( mInstanceManager.GetInstanceCount()-1, newInstance );
}


void NIM::InsertNodeInstanceWidget(int idx, NodeInstance* nodeInstance)
{
	NodeInstanceWidget* nodeInstanceWidget = new NodeInstanceWidget( nodeInstance );
	mInstanceLayout->insertWidget( idx, nodeInstanceWidget );
	connect( nodeInstanceWidget, SIGNAL(PortEdited(int)), this, SLOT(OnValidatePorts()) );
}


void NIM::OnValidatePorts()
{
	ValidatePorts();

}

void NIM::ValidatePorts()
{
	// Clear all warning first
	for (int i = 0; i < mInstanceManager.GetInstanceCount(); ++i)
	{
		NodeInstanceWidget* nodeInstWidget = qobject_cast<NodeInstanceWidget*>( mInstanceLayout->itemAt(i)->widget() );
		nodeInstWidget->ClearPortWarning();
	}

	// Validate all node instance port with each other
	for (int i = 0; i < mInstanceManager.GetInstanceCount(); ++i)
	{
		NodeInstance* nodeInstance = mInstanceManager.GetInstance(i);

		for (int j = i; j < mInstanceManager.GetInstanceCount(); ++j)
		{
			if ( i != j )
			{
				NodeInstance* otherNodeInstance = mInstanceManager.GetInstance(j);

				if ( nodeInstance->GetPort() == otherNodeInstance->GetPort() )
				{
					NodeInstanceWidget* w1 = qobject_cast<NodeInstanceWidget*>( mInstanceLayout->itemAt(i)->widget() );
					NodeInstanceWidget* w2 = qobject_cast<NodeInstanceWidget*>( mInstanceLayout->itemAt(j)->widget() );

					w1->SetPortWarning();
					w2->SetPortWarning();
				}
			}
		}
	}
}
