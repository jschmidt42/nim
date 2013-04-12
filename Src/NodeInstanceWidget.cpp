///
/// Copyright (C) 2013 - All Rights Reserved
/// All rights reserved. http://www.equals-forty-two.com
///
/// @brief Single node instance configuration widget implementation
///

#include "Precompiled.h"
#include "NodeInstanceWidget.h"

#include "QTUtils.h"
#include "EnvVarEditor.h"

#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QToolButton>

NodeInstanceWidget::NodeInstanceWidget(NodeInstance* nodeInstance, QWidget *parent)
	: QWidget(parent)
	, mNodeInstance(nodeInstance)
{
	Q_ASSERT( nodeInstance );

	QHBoxLayout* mainLayout = new QHBoxLayout();
	mainLayout->setSpacing( 2 );
	mainLayout->setMargin(0);

	//
	// Script path
	//
	mScriptPathEdit = new QLineEdit( nodeInstance->GetScriptPath() );
	mScriptPathEdit->setPlaceholderText( "enter script path..." );
	mScriptPathEdit->setTextMargins(0, 0, 20, 0);

	mScriptBrowseButton = new QPushButton("...", mScriptPathEdit);
	mScriptBrowseButton->setCursor( Qt::ArrowCursor );
	mScriptBrowseButton->setFixedSize( 20, 20 );
	mScriptBrowseButton->move(100, 0);

	//
	// Port
	//
	QValidator* portValidator = new QIntValidator(0, 65535, this);
	mPortEdit = new QLineEdit();
	mPortEdit->setFixedWidth( 45 );
	mPortEdit->setValidator( portValidator );
	mPortEdit->setText( nodeInstance->GetPort() == 0 ? "" : QString("%1").arg(nodeInstance->GetPort()) );
	mPortEdit->setPlaceholderText( "port" );

	//
	// Running state
	//
	mStateButton = new QPushButton( QIcon(":/NIM/Resources/start-node.png"), "" );
	mStateButton->setFlat(true);
	mStateButton->setFixedWidth( 20 );

	//
	// Configuration dropdown
	//
	mConfigButton = new QToolButton();
	mConfigButton->setIcon( QIcon(":/NIM/Resources/more-options.png") );
	mConfigButton->setFixedWidth( 20 );
	mConfigButton->setAutoRaise( true );
	mConfigButton->setPopupMode( QToolButton::InstantPopup );
	mConfigButton->setToolButtonStyle( Qt::ToolButtonIconOnly );

	//
	// Add custom node actions
	//
	AddActionOpenBrowser();
	AddActionOpenExplorer();
	AddActionEditEnvVars();
	AddActionLog();
	AddActionDebug();
	AddActionDelete();
	
	//
	// Layout
	//
	mainLayout->addWidget( mStateButton );
	mainLayout->addWidget( mScriptPathEdit );
	mainLayout->addWidget( mPortEdit );
	
	mainLayout->addWidget( mConfigButton );

	// Create connections
	connect( mScriptPathEdit, SIGNAL(editingFinished()), this, SLOT(OnScriptPathEdited()) );
	connect( mScriptPathEdit, SIGNAL(textChanged(const QString&)), this, SLOT(OnValidateScriptPath(const QString&)) );
	connect( mPortEdit, SIGNAL(editingFinished()), this, SLOT(OnPortEdited()) );
	connect( mScriptBrowseButton, SIGNAL(clicked()), this, SLOT(OnBrowseScript()) );
	connect( mStateButton, SIGNAL(clicked()), this, SLOT(OnNodeStateToggled()) );

	connect( mNodeInstance, SIGNAL(ScriptPathChanged(const QString&)), mScriptPathEdit, SLOT(setText(const QString&)) );
	connect( mNodeInstance, SIGNAL(NodeStateChanged(bool)), this, SLOT(OnNodeStateChanged(bool)) );

	setLayout( mainLayout );
}

NodeInstanceWidget::~NodeInstanceWidget()
{
}

void NodeInstanceWidget::resizeEvent(QResizeEvent* event)
{
	QWidget::resizeEvent( event );

	mScriptBrowseButton->move( mScriptPathEdit->width() - mScriptBrowseButton->width(), 0 );

	mStateButton->setFixedHeight( mScriptPathEdit->height() );
	mConfigButton->setFixedHeight( mScriptPathEdit->height() );
}

void NodeInstanceWidget::OnScriptPathEdited()
{
	mNodeInstance->SetScriptPath( mScriptPathEdit->text() );
}

void NodeInstanceWidget::OnBrowseScript()
{
	QFileDialog dialog(this);
	dialog.setFileMode(QFileDialog::AnyFile);
	dialog.setFilters( QStringList() << "JavaScript (*.js)" << "All files (*.*)" );
	if ( dialog.exec() )
	{
		const QStringList files = dialog.selectedFiles();
		if ( files.count() == 1 )
		{
			const QString scriptFile = files[0];
			mNodeInstance->SetScriptPath( scriptFile );
		}
	}
}

void NodeInstanceWidget::OnValidateScriptPath(const QString& path)
{
	QFileInfo fi( path );

	QPalette bgPal;
	if ( !fi.exists() )
	{
		bgPal.setColor( mScriptPathEdit->backgroundRole(), QColor(0xFF, 0x5A, 0x5A) );
	}
	mScriptPathEdit->setPalette( bgPal );
}

void NodeInstanceWidget::OnPortEdited()
{
	int port = mPortEdit->text().toInt();
	mNodeInstance->SetPort( port );

	emit PortEdited(port);
}

void NodeInstanceWidget::SetPortWarning()
{
	QPalette bgPal = mPortEdit->palette();
	bgPal.setColor( mPortEdit->backgroundRole(), QColor(244, 247, 153) );
	mPortEdit->setPalette( bgPal );
}

void NodeInstanceWidget::ClearPortWarning()
{
	mPortEdit->setPalette( QPalette() );
}

void NodeInstanceWidget::OnNodeStateToggled()
{
	if ( mNodeInstance->IsRunning() )
	{
		mNodeInstance->Stop();
	}
	else
	{
		mNodeInstance->Start();
	}
}

void NodeInstanceWidget::OnNodeStateChanged(bool)
{
	// Update UI state
	if ( mNodeInstance->IsRunning() )
	{
		mStateButton->setIcon( QIcon(":/NIM/Resources/stop-node.png") );
	}
	else
	{
		mStateButton->setIcon( QIcon(":/NIM/Resources/start-node.png") );
	}
}

void NodeInstanceWidget::AddAction(const QString& actionName, std::function<void()> actionCallback)
{
	QAction* newAction = new QAction( actionName, mConfigButton );
	mConfigButton->addAction( newAction );
	connect( newAction, SIGNAL(triggered()), _Q, _Q->Call(actionCallback) );
}

void NodeInstanceWidget::AddActionOpenBrowser()
{
	AddAction( tr("Open Browser"), [this](){
		if ( !mNodeInstance->IsRunning() ) 
		{
			int result = QMessageBox::warning( this, "Node not running", "Node instance is not started.\n\nWould you like to start it?", 
				QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, QMessageBox::Yes );

			if ( result == QMessageBox::Cancel )
				return;

			if ( result == QMessageBox::Yes )
				mNodeInstance->Start();
		}

		int port = mNodeInstance->GetPort();
		QDesktopServices::openUrl( tr("http://localhost:%1").arg(port) );
	});
}

void NodeInstanceWidget::AddActionOpenExplorer()
{
	AddAction( tr("Open Explorer"), [this](){
		if ( !mNodeInstance->IsValid() )
			return;

		QFileInfo fi( mNodeInstance->GetScriptPath() );
		QDesktopServices::openUrl(QUrl(QString("file:///%1").arg(fi.absolutePath()), QUrl::TolerantMode));
	});
}

void NodeInstanceWidget::AddActionEditEnvVars()
{
	AddAction( tr("Edit Env. Vars."), [this](){
		EnvVarEditor* envVarEditor =  new EnvVarEditor( mNodeInstance->GetVars(), this );
		envVarEditor->setWindowTitle( tr("Variables: %1").arg( mNodeInstance->GetScriptPath() ) );
		envVarEditor->show();
	});
}

void NodeInstanceWidget::AddActionDebug()
{
	// TODO:
}

void NodeInstanceWidget::AddActionDelete()
{
	// TODO:
}

void NodeInstanceWidget::AddActionLog()
{
	// TODO:
}
