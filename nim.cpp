///
/// Copyright (C) 2013 - All Rights Reserved
/// All rights reserved. http://www.equals-forty-two.com
///
/// @brief Main window implementation
///

#include "stdafx.h"
#include "nim.h"
#include "nodeinstancewidget.h"
#include "instancesettings.h"

#include <QSettings>

///////////////////////////////////////////////////////////////////////
NIM::NIM(QWidget *parent, Qt::WFlags flags)
	: QDialog(parent, flags)
	, mInstanceLayout(nullptr)
	, mInstanceSlotCount(0)
{
	CreateUI();
	PopulateUI();
}

///////////////////////////////////////////////////////////////////////
NIM::~NIM()
{
	SaveInstances();
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
	QSettings settings;

	int instanceIdx = 0;

	// Load instance settings from registry
	while (true)
	{
		QString instanceKey = QString("instances%1").arg(instanceIdx);
		QVariant instance = settings.value( instanceKey );
		if ( instance.isNull() )
			break;

		if ( !instance.canConvert<InstanceSettings>() )
			break;

		// Create node instance widget

		instanceIdx++;
	}

	mInstanceSlotCount = instanceIdx;

	// Add at least one more node instance slot
	mInstanceLayout->insertWidget( mInstanceSlotCount++, new NodeInstanceWidget() );
}

///////////////////////////////////////////////////////////////////////
void NIM::SaveInstances()
{

}
