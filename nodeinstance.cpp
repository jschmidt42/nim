///
/// Copyright (C) 2013 - All Rights Reserved
/// All rights reserved. http://www.equals-forty-two.com
///
/// @brief Single node instance implementation
///

#include "StdAfx.h"
#include "nodeinstance.h"
#include "instancesettings.h"

NodeInstance::NodeInstance()
{
}

NodeInstance::NodeInstance(const InstanceSettings& settings)
	: mScriptPath(settings.scriptPath)
{
}

NodeInstance::~NodeInstance()
{
}

void NodeInstance::SetScriptPath(const QString& path)
{
	mScriptPath = path;
}

QString NodeInstance::GetScriptPath() const
{
	return mScriptPath;
}

bool NodeInstance::IsValid() const
{
	if ( mScriptPath.isEmpty() )
		return false;

	return true;
}
