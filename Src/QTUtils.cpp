///
/// Copyright (C) 2013 - All Rights Reserved
/// All rights reserved. http://www.equals-forty-two.com
///

#include "Precompiled.h"
#include "QTUtils.h"

#include <QDebug>
#include <QtGlobal>

#include <sstream>

QTUtils::Internal::GlobalQTReceiver* _Q = nullptr;

namespace QTUtils {

	void Init()
	{
		_Q = new Internal::GlobalQTReceiver();
		atexit( Uninit );
	}

	void Uninit()
	{
		delete _Q;
		_Q = nullptr;
	}

	void SetTimeout(std::function<void()> callback, int timeout)
	{
		Internal::STimeout* timeoutWrapper = new Internal::STimeout(callback);
		QTimer::singleShot( timeout, timeoutWrapper, SLOT(OnTimeout()) );
	}

	BlockSignalGuard::BlockSignalGuard( QObject* obj1, QObject* obj2, QObject* obj3, QObject* obj4, QObject* obj5,
		QObject* obj6, QObject* obj7, QObject* obj8, QObject* obj9, QObject* obj10 )
	{    
		if ( obj1 )
		{
			mObjects.push_back(std::make_pair( obj1, obj1->signalsBlocked() ) );
		}    
		if (obj2)
		{
			mObjects.push_back(std::make_pair( obj2, obj2->signalsBlocked() ));
		}
		if (obj3)
		{
			mObjects.push_back(std::make_pair( obj3, obj3->signalsBlocked() ));
		}
		if (obj4)
		{
			mObjects.push_back(std::make_pair( obj4, obj4->signalsBlocked() ));
		}
		if (obj5)
		{
			mObjects.push_back(std::make_pair( obj5, obj5->signalsBlocked() ));
		}
		if (obj6)
		{
			mObjects.push_back(std::make_pair( obj6, obj6->signalsBlocked() ));
		}
		if (obj7)
		{
			mObjects.push_back(std::make_pair( obj7, obj7->signalsBlocked() ));
		}
		if (obj8)
		{
			mObjects.push_back(std::make_pair( obj8, obj8->signalsBlocked() ));
		}
		if (obj9)
		{
			mObjects.push_back(std::make_pair( obj9, obj9->signalsBlocked() ));
		}
		if (obj10)
		{
			mObjects.push_back(std::make_pair( obj10, obj10->signalsBlocked() ));
		}


		for ( size_t i = 0, count = mObjects.size(); i < count; ++i )
		{
			mObjects[i].second = mObjects[i].first->blockSignals(true);
		}
	}

	BlockSignalGuard::~BlockSignalGuard()
	{
		for ( size_t i = 0, count = mObjects.size(); i < count; ++i )
		{
			mObjects[i].first->blockSignals(mObjects[i].second);
		}
	}

	namespace Internal {
		
		void STimeout::OnTimeout()
		{
			mCallback();
			delete this;
		}

		GlobalQTReceiver::GlobalQTReceiver()
			: mNextId(0)
		{
			FillMetaStructs();
		}
		
		const char* GlobalQTReceiver::Call( std::function<void()> slotCallback )
		{
			std::stringstream ss;
			ss << "func" << mNextId++ << "()";

			FuncInfo fi;
			fi.name = ss.str();
			fi.slotName = "1"+fi.name;
			fi.callback = slotCallback;

			mFuncs.push_back( fi );
			FillMetaStructs();
						
			return mFuncs.back().slotName.c_str();
		}

		int GlobalQTReceiver::qt_metacall(QMetaObject::Call call, int id, void** args)
		{
			Q_ASSERT( call == QMetaObject::InvokeMetaMethod );

			id = QObject::qt_metacall(call, id, args);
			if (id < 0)
				return id;
			
			mFuncs[id].callback();
			id -= mFuncs.size();

			return id;
		}

		void* GlobalQTReceiver::qt_metacast( const char *_clname )
		{
			Q_ASSERT( !"not supported" );
			return nullptr;
		}

		const QMetaObject* GlobalQTReceiver::metaObject() const 
		{
			return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &mMetaObject;
		}

		void GlobalQTReceiver::FillMetaStructs()
		{
			const char objectName[] = "QTUtils::Internal::GlobalQTReceiver";
			mMetaString.clear();
			mMetaString.insert(mMetaString.begin(), objectName, objectName+sizeof(objectName));
			mMetaString.push_back('\0');

			  /* content:
			  6,       // revision
				0,       // classname
				0,    0, // classinfo
				7,   14, // methods
				0,    0, // properties
				0,    0, // enums/sets
				0,    0, // constructors
				0,       // flags
				0,       // signalCount
				*/
			mMetaData.clear();
			mMetaData.push_back( 6 ); // revision
			mMetaData.push_back( 0 ); // classname
			mMetaData.push_back( 0 ); mMetaData.push_back( 0 ); // classinfo
			mMetaData.push_back( mFuncs.size() ); mMetaData.push_back( 14 ); // methods
			mMetaData.push_back( 0 ); mMetaData.push_back( 0 ); // properties
			mMetaData.push_back( 0 ); mMetaData.push_back( 0 ); // enum/sets
			mMetaData.push_back( 0 ); mMetaData.push_back( 0 ); // constructors
			mMetaData.push_back( 0 ); // flags
			mMetaData.push_back( 0 ); // signalCount

			// slots: signature, parameters, type, tag, flags
			int offset = sizeof(objectName)+1;
			for (auto it = mFuncs.begin(), end = mFuncs.end(); it != end; ++it)
			{
				size_t funcNameLength = it->name.size();
				const char* funcName = it->name.c_str();
				mMetaString.insert(mMetaString.end(), funcName, funcName+funcNameLength);
				mMetaString.push_back('\0');

				mMetaData.push_back( offset ); // signature
				mMetaData.push_back( 36 ); // parameters
				mMetaData.push_back( 36 ); // type
				mMetaData.push_back( 36 ); // tag
				mMetaData.push_back( 0x09 ); // flags
				offset += it->name.size()+1;
			}

			// eod
			mMetaData.push_back( 0 ); 

			mMetaObject.d.superdata  = &QObject::staticMetaObject;
			mMetaObject.d.stringdata = &mMetaString[0];
			mMetaObject.d.data       = &mMetaData[0];
			mMetaObject.d.extradata  = nullptr;
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
