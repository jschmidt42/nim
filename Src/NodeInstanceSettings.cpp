///
/// Copyright (C) 2013 - All Rights Reserved
/// All rights reserved. http://www.equals-forty-two.com
///
/// @brief Node instance settings
///

#include "StdAfx.h"
#include "NodeInstanceSettings.h"

#include <QDataStream>

QDataStream &operator<<(QDataStream &out, const NodeInstanceSettings &obj)
{
	out << obj.scriptPath << obj.port;
	return out;
}

QDataStream &operator>>(QDataStream &in, NodeInstanceSettings &obj)
{
	in >> obj.scriptPath >> obj.port;
	return in;
}
