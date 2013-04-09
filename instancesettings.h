///
/// Copyright (C) 2013 - All Rights Reserved
/// All rights reserved. http://www.equals-forty-two.com
///
/// @brief Declares node instance settings
///

#pragma once

#include <QMetaType>

struct InstanceSettings
{
	QString scriptPath;
	int     port;

	friend QDataStream& operator<<(QDataStream &out, const InstanceSettings &obj);
	friend QDataStream& operator>>(QDataStream &in, InstanceSettings &obj);
};

Q_DECLARE_METATYPE(InstanceSettings);
