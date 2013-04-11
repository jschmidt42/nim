///
/// Copyright (C) 2013 - All Rights Reserved
/// All rights reserved. http://www.equals-forty-two.com
///
/// @brief Various QT helpers
///

#pragma once

#include <QObject>

#include <functional>

namespace QTUtils {

	typedef std::function<void()> TimeoutCallback;

	void SetTimeout(TimeoutCallback callback, int timeout);

	QString ReadStyleSheet(const QString& url);	

	namespace Internal {

		class STimeout : public QObject
		{
			Q_OBJECT;

		public:

			STimeout(TimeoutCallback callback)
				: mCallback(callback)
			{
			}

		public Q_SLOTS:

			void OnTimeout();

		private:

			TimeoutCallback mCallback;

		};

	}
}
