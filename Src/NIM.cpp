///
/// Copyright (C) 2013 - All Rights Reserved
/// All rights reserved. http://www.equals-forty-two.com
///
/// @brief Main window implementation
///

#include "Precompiled.h"
#include "NIM.h"

#include "NodeInstanceWidget.h"
#include "NoticeMessageBox.h"
#include "QTUtils.h"

#include <QSettings>

///////////////////////////////////////////////////////////////////////
NIM::NIM(QWidget *parent, Qt::WFlags flags)
	: QDialog(parent, flags | Qt::WindowMinimizeButtonHint)
	, mTrayIcon( QIcon(":/NIM/main-icon.png"), this )
	, mInstanceLayout(nullptr)
{
	QTUtils::Init();

	setStyleSheet( QTUtils::ReadStyleSheet(":/NIM/theme.css") );

	CreateTrayIcon();
	CreateUI();
	PopulateUI();
	SetConnections();
	UpdateTrayIconState();

	RestoreWindowSettings();
}

///////////////////////////////////////////////////////////////////////
NIM::~NIM()
{
	SaveWindowSettings();
}

///////////////////////////////////////////////////////////////////////
void NIM::CreateUI()
{
	mUI.setupUi(this);

	mUI.ConfigButton->setVisible( false ); // TODO

	mUI.Title->setText( tr(
		"<html>"
			"<div style='color:#FFF;font-size:200%'>N</font><font color='#AFD06C'>o</font>de <font color='#77982E'>js</font> Instance Manager</div>"
		"</html>") );

	mInstanceLayout = new QVBoxLayout();
	mInstanceLayout->setSpacing(2);
	mInstanceLayout->setMargin(0);

	mAddInstanceButton = new QPushButton( QIcon(":/NIM/add-node.png"), "Add");
	mInstanceLayout->addWidget( mAddInstanceButton, 0, Qt::AlignRight );
	mInstanceLayout->addStretch(1);

	mUI.Instances->setLayout( mInstanceLayout );
}

///////////////////////////////////////////////////////////////////////
void NIM::PopulateUI()
{
	// Create node instance widgets and populate the UI
	for (int i = 0; i < mNodeInstanceManager.GetInstanceCount(); ++i)
	{
		NodeInstance* nodeInstance = mNodeInstanceManager.GetInstance(i);
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
	NodeInstance* newInstance = mNodeInstanceManager.CreateInstance();
	InsertNodeInstanceWidget( mNodeInstanceManager.GetInstanceCount()-1, newInstance );
}

NodeInstanceWidget* NIM::InsertNodeInstanceWidget(int idx, NodeInstance* nodeInstance)
{
	NodeInstanceWidget* nodeInstanceWidget = new NodeInstanceWidget( nodeInstance );
	mInstanceLayout->insertWidget( idx, nodeInstanceWidget );

	connect( nodeInstanceWidget, SIGNAL(PortEdited(int)), this, SLOT(OnValidatePorts()) );
	connect( nodeInstanceWidget, SIGNAL(DeleteNodeInstance(NodeInstance*)), this, SLOT(OnDeleteNodeInstance(NodeInstance*)) );
	connect( nodeInstance, SIGNAL(NodeStateChanged(bool)), this, SLOT(OnGenerateTrayIcon()) );
	
	return nodeInstanceWidget;
}

void NIM::OnValidatePorts()
{
	ValidatePorts();
}

void NIM::ValidatePorts()
{
	// Clear all warning first
	for (int i = 0; i < mNodeInstanceManager.GetInstanceCount(); ++i)
	{
		NodeInstanceWidget* nodeInstWidget = qobject_cast<NodeInstanceWidget*>( mInstanceLayout->itemAt(i)->widget() );
		nodeInstWidget->ClearPortWarning();
	}

	// Validate all node instance port with each other
	for (int i = 0; i < mNodeInstanceManager.GetInstanceCount(); ++i)
	{
		NodeInstance* nodeInstance = mNodeInstanceManager.GetInstance(i);

		if ( nodeInstance->GetPort() == 0)
			continue;

		for (int j = i; j < mNodeInstanceManager.GetInstanceCount(); ++j)
		{
			if ( i != j )
			{
				NodeInstance* otherNodeInstance = mNodeInstanceManager.GetInstance(j);

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
	QMenu* menu = new QMenu();
	menu->addAction( QIcon(":/NIM/shutdown-icon.png"), "Open", _Q, _Q->Call( [this](){
		showNormal();
	} ) );
	menu->addSeparator();
	menu->addAction( QIcon(":/NIM/shutdown-icon.png"), "Exit", this, SLOT(accept()) );
	mTrayIcon.setContextMenu( menu );
	mTrayIcon.show();
}

void NIM::OnTrayActivated(QSystemTrayIcon::ActivationReason reason)
{
	if ( reason == QSystemTrayIcon::Trigger )
	{
		if ( isVisible() )
		{
			setWindowState( Qt::WindowMinimized );
		}
		else
		{
			RestoreWindowSettings();
			setVisible( true );
			setWindowState( Qt::WindowActive );
		}
	}
}

void NIM::OnGenerateTrayIcon()
{
	UpdateTrayIconState();
}

void NIM::changeEvent( QEvent* event )
{
	QDialog::changeEvent( event );

	if( event->type() == QEvent::WindowStateChange )
	{
		if( isMinimized() )
		{
			SaveWindowSettings();

			QTUtils::SetTimeout( [this]() {
				setVisible( false );
			}, 300 );
		}
	}
}

void NIM::OnDeleteNodeInstance(NodeInstance* nodeInstance)
{
	mNodeInstanceManager.DeleteInstance( nodeInstance );	
}

void NIM::resizeEvent(QResizeEvent* event)
{
	QDialog::resizeEvent( event );
}

void NIM::RestoreWindowSettings()
{
	QSettings settings;

	bool ok;
	int x = settings.value("posx").toInt(&ok);
	if ( !ok )
		return;
	int y = settings.value("posy").toInt(&ok);
	if ( !ok )
		return;

	setGeometry( 
		x, y,
		settings.value("width", width() ).toInt(), 
		settings.value( "height", height() ).toInt() );
}

void NIM::SaveWindowSettings()
{
	QRect fr = geometry();
	QSettings().setValue( "width", fr.width() );
	QSettings().setValue( "height", fr.height() );
	QSettings().setValue( "posx", fr.x() );
	QSettings().setValue( "posy", fr.y() );
}

void NIM::UpdateTrayIconState()
{
	bool hasRunningNodes = false;
	for (int i = 0; i < mNodeInstanceManager.GetInstanceCount(); ++i)
	{
		NodeInstance* nodeInstance = mNodeInstanceManager.GetInstance(i);
		if ( nodeInstance->IsRunning() )
		{
			hasRunningNodes = true;
			break;
		}
	}

	if ( hasRunningNodes )
	{
		QPixmap trayIconPixmap(16, 16);
		trayIconPixmap.fill(Qt::transparent);

		QPainter trayIconPainter( &trayIconPixmap );
		QIcon(":/NIM/main-icon.png").paint(&trayIconPainter, 0, 0, 16, 16);

		trayIconPainter.setPen( QPen(Qt::green, 1.5f) );
		trayIconPainter.setBrush( Qt::NoBrush );
		trayIconPainter.drawRoundedRect( QRect(0,0,15,15), 3, 3 );
		trayIconPainter.end();

		mTrayIcon.setIcon( trayIconPixmap );
	}
	else
	{
		mTrayIcon.setIcon( QIcon(":/NIM/main-icon.png") );
	}
}
