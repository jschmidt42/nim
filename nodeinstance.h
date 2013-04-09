///
/// Copyright (C) 2013 - All Rights Reserved
/// All rights reserved. http://www.equals-forty-two.com
///
/// @brief Single node instance
///

#pragma once

#include <QProcess>

class InstanceSettings;

class NodeInstance : public QObject
{
	Q_OBJECT;

public:

	NodeInstance();
	explicit NodeInstance(const InstanceSettings& settings);
	~NodeInstance();

public: // Interface

	bool IsValid() const;

	QString GetScriptPath() const;
	void SetScriptPath(const QString& path);

	int GetPort() const;
	void SetPort(int port);

	void Start();
	void Stop();
	bool IsRunning() const;

protected Q_SLOTS:

	void OnProcessStateChanged(QProcess::ProcessState state);

Q_SIGNALS:

	void ScriptPathChanged(const QString& path);
	void PortChanged(int port);
	void NodeStateChanged(bool);

private:

	QString  mScriptPath;
	int      mPort;

	QProcess mProcess;
};

