///
/// Copyright (C) 2013 - All Rights Reserved
/// All rights reserved. http://www.equals-forty-two.com
///
/// @brief Various QT helpers
///

#pragma once

#include <QObject>

#include <functional>
#include <string>
#include <vector>

namespace QTUtils {

	typedef std::function<void()> TimeoutCallback;

	void Init();
	void Uninit();

	void SetTimeout(TimeoutCallback callback, int timeout);

	QString ReadStyleSheet(const QString& url);	

	class BlockSignalGuard
	{
	public:
		BlockSignalGuard( QObject* obj1, QObject* obj2 = 0, QObject* obj3 = 0, QObject* obj4 = 0, QObject* obj5 = 0,
			QObject* obj6 = 0, QObject* obj7 = 0, QObject* obj8 = 0, QObject* obj9 = 0, QObject* obj10 = 0 );
		~BlockSignalGuard();
	private:
		std::vector<std::pair<QObject*, bool> > mObjects;
	};


	namespace Internal {

		class GlobalQTReceiver : public QObject
		{
			Q_GADGET;
		public:

			typedef std::function<void()> Func;

			GlobalQTReceiver();

			const char* Call(Func slotCallback);

		protected:

			virtual const QMetaObject *metaObject() const override;
			virtual int qt_metacall(QMetaObject::Call, int, void **) override;
			virtual void* qt_metacast(const char *_clname) override;
			
		protected Q_SLOTS:

			void func1();
		/*	void test2();
			void test3();
			void test66();
			void test3(int, QString);
			void test4();
			void test4(int, int, int);*/

		private:

			void FillMetaStructs();

			int mNextId;
			std::map<std::string, Func> mFuncs;
			std::vector<uint> mMetaData;
			std::vector<char> mMetaString;
			QMetaObject mMetaObject;

		};

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

extern QTUtils::Internal::GlobalQTReceiver* _Q;
