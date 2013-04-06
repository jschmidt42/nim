#include "stdafx.h"
#include "nim.h"
#include "nodeinstancewidget.h"

NIM::NIM(QWidget *parent, Qt::WFlags flags)
	: QDialog(parent, flags)
{
	ui.setupUi(this);

	QVBoxLayout* instanceLayout = new QVBoxLayout();
	instanceLayout->setSpacing(2);
	instanceLayout->setMargin(0);

	instanceLayout->addWidget( new NodeInstanceWidget() );
	instanceLayout->addWidget( new NodeInstanceWidget() );
	instanceLayout->addWidget( new NodeInstanceWidget() );
	instanceLayout->addWidget( new NodeInstanceWidget() );
	instanceLayout->addWidget( new NodeInstanceWidget() );
	instanceLayout->addWidget( new NodeInstanceWidget() );
	instanceLayout->addWidget( new NodeInstanceWidget() );
	instanceLayout->addWidget( new NodeInstanceWidget() );
	instanceLayout->addWidget( new NodeInstanceWidget() );
	instanceLayout->addWidget( new NodeInstanceWidget() );
	instanceLayout->addWidget( new NodeInstanceWidget() );
	instanceLayout->addWidget( new NodeInstanceWidget() );

	instanceLayout->addWidget( new QPushButton("Add"), 0, Qt::AlignRight );
	instanceLayout->addStretch(1);

	ui.Instances->setLayout( instanceLayout );
}

NIM::~NIM()
{

}
