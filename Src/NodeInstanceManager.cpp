///
/// Copyright (C) 2013 - All Rights Reserved
/// All rights reserved. http://www.equals-forty-two.com
///

#include "Precompiled.h"
#include "NodeInstanceManager.h"

#include "NodeInstanceSettings.h"

#include <QSettings>

NodeInstanceManager::NodeInstanceManager()
{
	Load( QSettings() );
}


NodeInstanceManager::~NodeInstanceManager()
{
	QSettings settings;
	Save( settings );

	// Cleanup node memory
	for (int i = 0; i < mInstances.count(); ++i)
	{
		delete mInstances[i];
	}
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

NodeInstance* NodeInstanceManager::CreateInstance(const NodeInstanceSettings& settings)
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

		NodeInstanceSettings instanceSettings;
		instanceSettings.scriptPath = nodeInstance->GetScriptPath();
		instanceSettings.port = nodeInstance->GetPort();
		instanceSettings.vars = nodeInstance->GetVars();

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

		if ( !instanceSettings.canConvert<NodeInstanceSettings>() )
			break;

		// Create node instance widget
		CreateInstance( instanceSettings.value<NodeInstanceSettings>() );
	}
}

NodeInstance* NodeInstanceManager::GetInstance(int idx) const
{
	Q_ASSERT( idx >= 0 && idx < mInstances.count() );
	return mInstances.at( idx );
}
