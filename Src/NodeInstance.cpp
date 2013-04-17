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
	, mDebug(false)
{
	Init();
}

NodeInstance::NodeInstance(const NodeInstanceSettings& settings)
	: mScriptPath(settings.scriptPath)
	, mPort(settings.port)
	, mVars(settings.vars)
	, mDebug(settings.debug)
{
	Init();

	bool isValid = false;
	int initStart = mVars["INITSTART"].toInt(&isValid);
	if ( isValid && initStart == 1 )
	{
		Start();
	}
}

NodeInstance::~NodeInstance()
{
	mProcess.disconnect( this );
	Stop();
}

void NodeInstance::Init()
{
	connect( &mProcess, SIGNAL(stateChanged(QProcess::ProcessState)), this, SLOT(OnProcessStateChanged(QProcess::ProcessState)) );
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
	if ( mDebug )
	{
		bool isPortValid = false;
		int debugPort = mVars["DEBUGPORT"].toInt(&isPortValid);
		if ( isPortValid )
			arguments << tr("--debug=%1").arg(debugPort);
		else
			arguments << tr("debug");
	}
	arguments << scriptFileInfo.fileName();
	mProcess.setWorkingDirectory( scriptFileInfo.absolutePath() );
	
	// Set env. vars.
	QStringList env = QProcess::systemEnvironment();
	env << QString("PORT=%1").arg(mPort);

	for (auto it = mVars.begin(), end = mVars.end(); it != end; ++it)
	{
		env << QString("%1=%2").arg(it.key()).arg(it.value());
	}

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

void NodeInstance::EnableDebugging(bool value /*= true*/)
{
	if ( mDebug != value )
	{
		mDebug = value;
		emit DebugStateChanged( mDebug );
	}
}

bool NodeInstance::IsDebugEnabled() const
{
	return mDebug;
}

QString NodeInstance::ReadLog()
{
	char buf[1024];
	qint64 lineLength = mProcess.readLine(buf, sizeof(buf));
	if (lineLength != -1) 
	{

		mLog += buf;
		if ( mLog.size() >= 20000 )
		{
			// Clean log a bit
			mLog.remove(0, 2000);
		}

		// the line is available in buf
		return QString(buf);
	}
	return "";
}

QString NodeInstance::GetLog() const
{
	return mLog;
}
