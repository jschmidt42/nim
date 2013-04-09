///
/// Copyright (C) 2013 - All Rights Reserved
/// All rights reserved. http://www.equals-forty-two.com
///

#include "stdafx.h"
#include "instancemanager.h"
#include "instancesettings.h"

#include <QSettings>

NodeInstanceManager::NodeInstanceManager()
{
	Load( QSettings() );
}


NodeInstanceManager::~NodeInstanceManager()
{
	Save( QSettings() );
}

int NodeInstanceManager::GetInstanceCount() const
{
	return mInstances.count();
}

NodeInstance* NodeInstanceManager::CreateInstance()
{
	NodeInstance* newInstance = new NodeInstance();
	mInstances.push_back( newInstance );
	return newInstance;
}

NodeInstance* NodeInstanceManager::CreateInstance(const InstanceSettings& settings)
{
	NodeInstance* newInstance = new NodeInstance( settings );
	mInstances.push_back( newInstance );
	return newInstance;
}

void NodeInstanceManager::Save(QSettings& settings)
{
	int regIdx = 0;
	for (int i = 0; i < mInstances.count(); ++i)
	{
		NodeInstance* nodeInstance = mInstances[i];

		// Check that the instance is valid
		if ( !nodeInstance->IsValid() )
			continue;

		InstanceSettings instanceSettings;
		instanceSettings.scriptPath = nodeInstance->GetScriptPath();
		instanceSettings.port = nodeInstance->GetPort();

		QString instanceKey = QString("instance%1").arg(regIdx);
		QVariant varInstanceSettings = QVariant::fromValue(instanceSettings);
		settings.setValue( instanceKey, varInstanceSettings );

		regIdx++;
	}

	// Clear the next instance registry setting to make sure not to load invalid data.
	QString instanceKeyToRemove = QString("instance%1").arg(regIdx);
	settings.remove( instanceKeyToRemove );

}

void NodeInstanceManager::Load(const QSettings& settings)
{
	// Load instance settings from registry
	while (true)
	{
		QString instanceKey = QString("instance%1").arg(GetInstanceCount());
		QVariant instanceSettings = settings.value( instanceKey );
		if ( instanceSettings.isNull() )
			break;

		if ( !instanceSettings.canConvert<InstanceSettings>() )
			break;

		// Create node instance widget
		CreateInstance( instanceSettings.value<InstanceSettings>() );
	}
}

NodeInstance* NodeInstanceManager::GetInstance(int idx) const
{
	Q_ASSERT( idx >= 0 && idx < mInstances.count() );
	return mInstances.at( idx );
}
