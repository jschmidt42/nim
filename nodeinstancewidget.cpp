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

NodeInstanceWidget::NodeInstanceWidget(NodeInstance* nodeInstance, QWidget *parent)
	: QWidget(parent)
	, mInstance(nodeInstance)
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
	QValidator* portValidator = new QIntValidator(0, 66000, this);
	mPortEdit = new QLineEdit();
	mPortEdit->setFixedWidth( 45 );
	mPortEdit->setValidator( portValidator );
	mPortEdit->setText( nodeInstance->GetPort() == 0 ? "" : QString("%1").arg(nodeInstance->GetPort()) );

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
	connect( mScriptPathEdit, SIGNAL(textChanged(const QString&)), this, SLOT(OnValidateScriptPath(const QString&)) );
	connect( mPortEdit, SIGNAL(editingFinished()), this, SLOT(OnPortEdited()) );
	connect( mScriptBrowseButton, SIGNAL(clicked()), this, SLOT(OnBrowseScript()) );
	connect( mInstance, SIGNAL(ScriptPathChanged(const QString&)), mScriptPathEdit, SLOT(setText(const QString&)) );

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
	mInstance->SetScriptPath( mScriptPathEdit->text() );
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
			mInstance->SetScriptPath( scriptFile );
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
	mInstance->SetPort( port );

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
