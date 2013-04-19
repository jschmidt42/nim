///
/// Copyright (C) 2013 - All Rights Reserved
/// All rights reserved. http://www.equals-forty-two.com
///
/// @brief Various QT helpers
///

#pragma once

struct ProcessInfo
{
	quint32 pid;
	QString name;
	QString workingDir;
	QString commandLine;
	QStringList args;
};

QList<ProcessInfo> GetProcessInfoList(const QString& query);

bool GetProcessInfo(quint32 processId, ProcessInfo& out_Info);
QList<ProcessInfo> GetProcessInfos(const QString& processName);
QList<ProcessInfo> GetAllProcessInfo();