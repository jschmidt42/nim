///
/// Copyright (C) 2013 - All Rights Reserved
/// All rights reserved. http://www.equals-forty-two.com
///

#include "stdafx.h"
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
} // end namespace QTUtils
