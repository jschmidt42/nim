///
/// Copyright (C) 2013 - All Rights Reserved
/// All rights reserved. http://www.equals-forty-two.com
///
/// @brief Single node instance implementation
///

#include "Precompiled.h"
#include "NodeInstance.h"

#include "NodeInstanceSettings.h"

NodeInstance::NodeInstance()
	: mScriptPath()
	, mPort(0)
	, mProcess( this )
{
}

NodeInstance::NodeInstance(const NodeInstanceSettings& settings)
	: mScriptPath(settings.scriptPath)
	, mPort(settings.port)
{
	connect( &mProcess, SIGNAL(stateChanged(QProcess::ProcessState)), this, SLOT(OnProcessStateChanged(QProcess::ProcessState)) );
}

NodeInstance::~NodeInstance()
{
	mProcess.disconnect( this );
	Stop();
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
	// TODO: read from config the node.exe path

	if ( !IsValid() )
		return;

	if ( IsRunning() )
	{
		Q_ASSERT( false );
		Stop();
	}

	// Get the directory of the script
	QFileInfo scriptFileInfo( mScriptPath );

	QStringList  arguments;
	arguments << scriptFileInfo.fileName();
	mProcess.setWorkingDirectory( scriptFileInfo.absolutePath() );
	
	// Set env. vars.
	QStringList env = QProcess::systemEnvironment();
	env << QString("PORT=%1").arg(mPort);
	mProcess.setEnvironment(env);

	// Start the process
	mProcess.start("node.exe", arguments);
}

void NodeInstance::Stop()
{
	mProcess.kill();
}

bool NodeInstance::IsRunning() const
{
	return mProcess.state() != QProcess::NotRunning;
}

void NodeInstance::OnProcessStateChanged(QProcess::ProcessState state)
{
	emit NodeStateChanged( IsRunning() );
}
