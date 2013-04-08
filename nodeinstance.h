///
/// Copyright (C) 2013 - All Rights Reserved
/// All rights reserved. http://www.equals-forty-two.com
///
/// @brief Single node instance
///

#pragma once

class InstanceSettings;

class NodeInstance : public QObject
{
	Q_OBJECT;

public:

	NodeInstance();
	explicit NodeInstance(const InstanceSettings& settings);
	~NodeInstance();

public: // Interface

	QString GetScriptPath() const;
	void SetScriptPath(const QString& path);
	bool IsValid() const;

Q_SIGNALS:

	void ScriptPathChanged(const QString& path);

private:

	QString mScriptPath;
};

