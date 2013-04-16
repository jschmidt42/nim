///
/// Copyright (C) 2013 - All Rights Reserved
/// All rights reserved. http://www.equals-forty-two.com
///
/// @brief Single node instance
///

#pragma once

#include <QProcess>
#include <QMap>

class NodeInstanceSettings;

class NodeInstance : public QObject
{
	Q_OBJECT;

public:

	typedef QMap<QString, QString>  Vars;

	NodeInstance();
	explicit NodeInstance(const NodeInstanceSettings& settings);
	~NodeInstance();

public: // Interface

	bool IsValid() const;

	QString GetScriptPath() const;
	void SetScriptPath(const QString& path);

	int GetPort() const;
	void SetPort(int port);

	void Init();
	void Start();
	void Stop();
	bool IsRunning() const;
	QString ReadLog();
	QString GetLog() const;

	void EnableDebugging(bool value = true);
	bool IsDebugEnabled() const;

	Vars& GetVars() { return mVars; }
	const Vars& GetVars() const { return mVars; }

protected Q_SLOTS:

	void OnProcessStateChanged(QProcess::ProcessState state);

Q_SIGNALS:

	void ScriptPathChanged(const QString& path);
	void PortChanged(int port);
	void NodeStateChanged(bool);
	void DebugStateChanged(bool);
	
private:

	QString  mScriptPath;
	int      mPort;
	Vars     mVars;
	bool     mDebug;
	QString  mLog;

	QProcess mProcess;
};

