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
	: mScriptPath()
	, mPort(0)
{
}

NodeInstance::NodeInstance(const InstanceSettings& settings)
	: mScriptPath(settings.scriptPath)
	, mPort(settings.port)
{
}

NodeInstance::~NodeInstance()
{
}

void NodeInstance::SetScriptPath(const QString& path)
{
	mScriptPath = path;
	emit ScriptPathChanged(path);
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

int NodeInstance::GetPort() const
{
	return mPort;
}

void NodeInstance::SetPort(int port)
{
	mPort = port;
	emit PortChanged(port);
}

void NodeInstance::Start()
{

}

void NodeInstance::Stop()
{

}
