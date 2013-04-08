///
/// Copyright (C) 2013 - All Rights Reserved
/// All rights reserved. http://www.equals-forty-two.com
///
/// @brief Single node instance
///

#pragma once

class InstanceSettings;

class NodeInstance
{
public:
	NodeInstance();
	explicit NodeInstance(const InstanceSettings& settings);
	~NodeInstance();

	QString GetScriptPath() const;
	void SetScriptPath(const QString& path);
	
	bool IsValid() const;

private:

	QString mScriptPath;
};

