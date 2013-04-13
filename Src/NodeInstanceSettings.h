///
/// Copyright (C) 2013 - All Rights Reserved
/// All rights reserved. http://www.equals-forty-two.com
///
/// @brief Declares node instance settings
///

#pragma once

#include <QMetaType>

struct NodeInstanceSettings
{
	QString scriptPath;
	int     port;
	QMap<QString, QString> vars;

	friend QDataStream& operator<<(QDataStream &out, const NodeInstanceSettings &obj);
	friend QDataStream& operator>>(QDataStream &in, NodeInstanceSettings &obj);
};

Q_DECLARE_METATYPE(NodeInstanceSettings);
