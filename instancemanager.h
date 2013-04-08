///
/// Copyright (C) 2013 - All Rights Reserved
/// All rights reserved. http://www.equals-forty-two.com
///
/// @brief Node instances manager
///

#pragma once

#include "nodeinstance.h"

class NodeInstanceManager
{
public:
	NodeInstanceManager();
	~NodeInstanceManager();

public: // Interface

	/// Creates a new instance and return it
	NodeInstance* CreateInstance();
	NodeInstance* CreateInstance(const InstanceSettings& settings);

	/// Returns the number of instances being managed
	int GetInstanceCount() const;

	/// Returns a given node instance
	NodeInstance* GetInstance(int idx) const;

public: // Serialization

	/// Saves node instances into application settings
	void Save(QSettings& settings);

	/// Loads node settings and create node instances
	void Load(const QSettings& settings);

private:

	QList<NodeInstance*> mInstances;
};

