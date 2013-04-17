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
#include "NoticeMessageBox.h"
#include "NodeLogWindow.h"

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
	mEnableDebuggingAction = AddActionDebug();
	mEnableDebuggingAction->setCheckable( true );
	mEnableDebuggingAction->setChecked( mNodeInstance->IsDebugEnabled() );

	AddActionOpenBrowser();
	AddActionOpenExplorer();
	AddActionEditEnvVars();
	AddActionLog();
	AddActionDelete();

	UpdateStateButton();
	
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
	connect( mPortEdit, SIGNAL(textChanged(const QString&)), this, SLOT(OnPortTextChanged(const QString&)) );
	connect( mScriptBrowseButton, SIGNAL(clicked()), this, SLOT(OnBrowseScript()) );
	connect( mStateButton, SIGNAL(clicked()), this, SLOT(OnNodeStateToggled()) );

	connect( mNodeInstance, SIGNAL(ScriptPathChanged(const QString&)), mScriptPathEdit, SLOT(setText(const QString&)) );
	connect( mNodeInstance, SIGNAL(NodeStateChanged(bool)), this, SLOT(OnNodeStateChanged(bool)) );
	connect( mNodeInstance, SIGNAL(DebugStateChanged(bool)), this, SLOT(OnNodeDebugStateChanged(bool)) );

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
	mPortEdit->setStyleSheet( "background-color:rgb(244,247,153)" );
	mPortEdit->setPalette( bgPal );
}

void NodeInstanceWidget::ClearPortWarning()
{
	mPortEdit->setPalette( QPalette() );
	mPortEdit->setStyleSheet( "" );
}

void NodeInstanceWidget::OnNodeStateToggled()
{
	if ( mNodeInstance->IsRunning() )
	{
		mNodeInstance->Stop();
	}
	else
	{
		if ( mNodeInstance->GetPort() == 0 )
		{
			NoticeMessageBox runningWarning( "noport", this );
			runningWarning.SetTitle( tr("No Port") );
			runningWarning.SetMessage( tr(
				"You have specified no process PORT.\n") );
			if ( runningWarning.exec() == QDialog::Rejected )
				return;
		}
		mNodeInstance->Start();
	}
}

void NodeInstanceWidget::OnNodeStateChanged(bool)
{
	UpdateStateButton();

}

QAction* NodeInstanceWidget::AddAction(const QString& actionName, std::function<void()> actionCallback)
{
	QAction* newAction = new QAction( actionName, mConfigButton );
	mConfigButton->addAction( newAction );
	connect( newAction, SIGNAL(triggered()), _Q, _Q->Call(actionCallback) );
	return newAction;
}

QAction* NodeInstanceWidget::AddActionOpenBrowser()
{
	return AddAction( tr("Open Browser"), [this](){
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
		QString root = mNodeInstance->GetEnvValue( "HTTPROOT" );
		QDesktopServices::openUrl( tr("http://localhost:%1/%2").arg(port).arg(root) );
	});
}

QAction* NodeInstanceWidget::AddActionOpenExplorer()
{
	return AddAction( tr("Open Explorer"), [this](){
		if ( !mNodeInstance->IsValid() )
			return;

		QFileInfo fi( mNodeInstance->GetScriptPath() );
		QDesktopServices::openUrl(QUrl(QString("file:///%1").arg(fi.absolutePath()), QUrl::TolerantMode));
	});
}

QAction* NodeInstanceWidget::AddActionEditEnvVars()
{
	return AddAction( tr("Edit Env. Vars."), [this](){
		EnvVarEditor* envVarEditor =  new EnvVarEditor( mNodeInstance->GetVars(), this );
		envVarEditor->setWindowTitle( tr("Variables: %1").arg( mNodeInstance->GetScriptPath() ) );
		envVarEditor->show();
	});
}

QAction* NodeInstanceWidget::AddActionDebug()
{
	return AddAction( tr("Enable Debugging"), [this](){
		if ( mNodeInstance->IsRunning() )
		{
			NoticeMessageBox runningWarning( "debug", this );
			runningWarning.SetTitle( tr("Node running...") );
			runningWarning.SetMessage( tr(
				"The node process is already running.\n"
				"Restart process process for this change to take effect.") );
			runningWarning.exec();
		}

		mNodeInstance->EnableDebugging( mEnableDebuggingAction->isChecked() );
	});
}

QAction* NodeInstanceWidget::AddActionDelete()
{
	return AddAction( tr("Delete"), [this](){
		deleteLater();
		emit DeleteNodeInstance(mNodeInstance);
	});
}

QAction* NodeInstanceWidget::AddActionLog()
{
	return AddAction( tr("Log"), [this]() {
		NodeLogWindow* logWindow =  new NodeLogWindow( mNodeInstance, this );
		logWindow->setWindowTitle( tr("Log: %1").arg( mNodeInstance->GetScriptPath() ) );
		logWindow->show();
	});
}

void NodeInstanceWidget::OnNodeDebugStateChanged(bool debug)
{
	mEnableDebuggingAction->setChecked( debug );
}

NodeInstance* NodeInstanceWidget::Instance() const
{
	return mNodeInstance;
}

void NodeInstanceWidget::OnPortTextChanged(const QString& text)
{
	// NOTE: Real validation is done in OnPortEdited
	//       Here we just set the port to 0 if the text if empty
	if ( text.isEmpty() )
	{
		mNodeInstance->SetPort( 0 );
	}
}

void NodeInstanceWidget::UpdateStateButton()
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
