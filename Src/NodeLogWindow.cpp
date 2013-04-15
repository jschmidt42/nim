///
/// Copyright (C) 2013 - All Rights Reserved
/// All rights reserved. http://www.equals-forty-two.com
///

#include "Precompiled.h"
#include "NodeLogWindow.h"

#include "QTUtils.h"

#include <QVBoxLayout>
#include <QTextEdit>

namespace {

}

NodeLogWindow::NodeLogWindow(NodeInstance* nodeInstance, QWidget *parent)
	: QDialog(parent, Qt::Dialog)
	, mNodeInstnace(nodeInstance)
{
	QVBoxLayout* mainLayout = new QVBoxLayout(this);
	
	mLog = new QTextEdit("test...", this);

	QPushButton* closeButton = new QPushButton( tr("Close"), this );

	mainLayout->addWidget( mLog, 1 );
	mainLayout->addWidget( closeButton, 0, Qt::AlignRight );

	setLayout( mainLayout );

	connect( closeButton, SIGNAL(clicked()), this, SLOT(deleteLater()) );
}

NodeLogWindow::~NodeLogWindow()
{
}
