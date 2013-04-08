///
/// Copyright (C) 2013 - All Rights Reserved
/// All rights reserved. http://www.equals-forty-two.com
///
/// @brief Node instance settings
///

#include "StdAfx.h"
#include "instancesettings.h"

#include <QDataStream>

QDataStream &operator<<(QDataStream &out, const InstanceSettings &obj)
{
	out << obj.scriptPath;
	return out;
}

QDataStream &operator>>(QDataStream &in, InstanceSettings &obj)
{
	in >> obj.scriptPath;
	return in;
}
