///
/// Copyright (C) 2013 - All Rights Reserved
/// All rights reserved. http://www.equals-forty-two.com
///

#include "Precompiled.h"
#include "QTUtils.h"

namespace QTUtils {

	void QTUtils::SetTimeout(std::function<void()> callback, int timeout)
	{
		Internal::STimeout* timeoutWrapper = new Internal::STimeout(callback);
		QTimer::singleShot( timeout, timeoutWrapper, SLOT(OnTimeout()) );
	}

	namespace Internal {
		
		void STimeout::OnTimeout()
		{
			mCallback();
			delete this;
		}
	} // end namespace Internal

	QString ReadStyleSheet( const QString& url )
	{
		// Let's use QFile and point to a resource...
		QFile data(url);
		QString style;
		// ...to open the file
		if(data.open(QFile::ReadOnly)) {
			QTextStream styleIn(&data);
			// Read file to a string.
			style = styleIn.readAll();
			data.close();

			return style;
		}

		return "";
	}

} // end namespace QTUtils
