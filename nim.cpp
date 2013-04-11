///
/// Copyright (C) 2013 - All Rights Reserved
/// All rights reserved. http://www.equals-forty-two.com
///
/// @brief Main window implementation
///

#include "Precompiled.h"
#include "nim.h"

#include "nodeinstancewidget.h"
#include "NoticeMessageBox.h"
#include "QTUtils.h"

#include <QSettings>

///////////////////////////////////////////////////////////////////////
NIM::NIM(QWidget *parent, Qt::WFlags flags)
	: QDialog(parent, flags | Qt::WindowMinimizeButtonHint)
	, mTrayIcon( QIcon(":/NIM/Resources/main-icon.png"), this )
	, mInstanceLayout(nullptr)
{
	CreateTrayIcon();
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
	connect( mUI.ExitButton, SIGNAL(clicked()), this, SLOT(accept()) );
	connect( mAddInstanceButton, SIGNAL(clicked()), this, SLOT(OnAddInstance()) );
	connect( insertNodeAction, SIGNAL(triggered()), this, SLOT(OnAddInstance()) );
	connect( &mTrayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(OnTrayActivated(QSystemTrayIcon::ActivationReason)) );
}

void NIM::closeEvent(QCloseEvent* event)
{
	NoticeMessageBox minimizeWarning( "minimize", this );
	minimizeWarning.SetTitle( tr("Minimize Warning") );
	minimizeWarning.SetMessage( tr(
		"This window will be minimized to the system tray.\n"
		"Click on the system tray icon to open again.\n"
		"Use the Exit button to fully close the application.") );
	if ( minimizeWarning.exec() == QDialog::Accepted )
	{
		setWindowState( Qt::WindowMinimized );
	}
	event->ignore();
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
	
	connect( nodeInstance, SIGNAL(NodeStateChanged(bool)), this, SLOT(OnCountActiveNodes()) );
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

void NIM::CreateTrayIcon()
{
	mTrayIcon.show();
}

void NIM::OnTrayActivated(QSystemTrayIcon::ActivationReason reason)
{
	if ( reason == QSystemTrayIcon::Trigger )
	{
		showNormal();
	}
}

void NIM::OnCountActiveNodes()
{
#if 0
	int runningNodes = 0;
	for (int i = 0; i < mInstanceManager.GetInstanceCount(); ++i)
	{
		NodeInstance* nodeInstance = mInstanceManager.GetInstance(i);
		if ( nodeInstance->IsRunning() )
			runningNodes++;
	}

	QPixmap trayIconPixmap( ":/NIM/Resources/main-icon.png" );
	QPainter trayIconPainter( &trayIconPixmap );
	QFont ft = trayIconPainter.font();
	ft.setPointSize( 16 );
	ft.setBold(true);
	trayIconPainter.setPen( Qt::white );
	trayIconPainter.setFont( ft );
	trayIconPainter.drawText( trayIconPixmap.rect(), Qt::AlignBottom | Qt::AlignRight, QString("%1").arg(runningNodes) );
	trayIconPainter.end();
	mTrayIcon.setIcon( QIcon(trayIconPixmap) );
#endif
}

void NIM::changeEvent( QEvent* event )
{
	if( event->type() == QEvent::WindowStateChange )
	{
		if( isMinimized() )
		{
			QTUtils::SetTimeout( [this]() {
				this->hide();
			}, 300 );
		}
	}
}
