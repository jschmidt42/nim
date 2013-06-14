///
/// Copyright (C) 2013 - All Rights Reserved
/// All rights reserved. http://www.equals-forty-two.com
///

#include "Precompiled.h"
#include "NodeLogWindow.h"

#include "QTUtils.h"

#include <QVBoxLayout>
#include <QTextEdit>
#include <QSettings>
#include <QCryptographicHash>

namespace {

}

NodeLogWindow::NodeLogWindow(NodeInstance* nodeInstance, QWidget *parent)
	: QDialog(parent, Qt::Dialog)
	, mNodeInstnace(nodeInstance)
{
	QVBoxLayout* mainLayout = new QVBoxLayout(this);
	
	mLog = new QTextEdit(this);
	mLog->setReadOnly( true );
	mLog->insertHtml( ToHTML(mNodeInstnace->GetLog()) );

    QHBoxLayout* buttonLayout = new QHBoxLayout(this);

	QPushButton* closeButton = new QPushButton( tr("Close"), this );
    QPushButton* clearButton = new QPushButton( tr("Clear"), this );

	mainLayout->addWidget( mLog, 1 );
    mainLayout->addLayout(buttonLayout);
    buttonLayout->addWidget( clearButton, 0, Qt::AlignLeft );
	buttonLayout->addWidget( closeButton, 0, Qt::AlignRight );

	setLayout( mainLayout );

	connect( closeButton, SIGNAL(clicked()), this, SLOT(deleteLater()) );
    connect( clearButton, SIGNAL(clicked()), this, SLOT(Clear()) );

	mQueryLogTimer.setInterval( 500 );
	mQueryLogTimer.start();
	connect( &mQueryLogTimer, SIGNAL(timeout()), _Q, _Q->Call([this](){
		QString log = mNodeInstnace->ReadLog();
		if  ( !log.isEmpty() )
		{
			mLog->insertHtml( ToHTML(log) );
			mLog->ensureCursorVisible();
		}
	}) );


	// Restore pos settings
	QSettings settings;
	mWindowId = QCryptographicHash::hash(mNodeInstnace->GetScriptPath().toAscii(), QCryptographicHash::Md5);
	int x = settings.value( tr("_log_pos_x%1").arg(mWindowId), -1 ).toInt();
	int y = settings.value( tr("_log_pos_y%1").arg(mWindowId), -1 ).toInt();
	int w = settings.value( tr("_log_pos_w%1").arg(mWindowId), 300 ).toInt();
	int h = settings.value( tr("_log_pos_h%1").arg(mWindowId), 300 ).toInt();
	if ( x != -1 && y != -1 )
	{
		move( x, y );
		resize( w, h );
	}
}

NodeLogWindow::~NodeLogWindow()
{
	QSettings settings;
	settings.setValue( tr("_log_pos_x%1").arg(mWindowId), this->pos().x() );
	settings.setValue( tr("_log_pos_y%1").arg(mWindowId), this->pos().y() );
	settings.setValue( tr("_log_pos_w%1").arg(mWindowId), this->width() );
	settings.setValue( tr("_log_pos_h%1").arg(mWindowId), this->height() );
}

QString NodeLogWindow::ToHTML(const QString& log) const
{
	QString html;
	QString escStr;
	bool escaping = false;
	
	html = "<html>";

	int pos = 0;
	while( pos < log.size() )
	{
		if ( log[pos] == 27 )
		{
			pos++;
			// Detected an escape string
			escStr.clear();
			while ( true )
			{
				escStr += log[pos];
				if ( log[pos++] == 'm' )
				{
					// Done
					break;
				}
			}

			if ( escaping )
			{
				html += "</span>";
			}

			QString tag = GetColorTag( escStr );
			if ( !tag.isEmpty() )
			{
				html += tag;
				escaping = true;
			}
			else
			{
				// Reset
				escaping = false;
			}
		}
		else if ( log[pos] == '\n' )
		{
			html += "<br/>";
			pos++;
		}
		else
		{
			html += log[pos++];
		}
	}

    html.replace("error", "<span style='color:red'>error</span>");
    html.replace("Error", "<span style='color:red'>Error</span>");

    html.replace("warning", "<span style='color:yellow'>warning</span>");
    html.replace("Warning", "<span style='color:yellow'>Warning</span>");

	html += "</html>";

	return html;
}

QString NodeLogWindow::GetColorTag(const QString& code) const
{
	// NOTE: http://www.tux-planet.fr/les-codes-de-couleurs-en-bash/
	if ( code == "[30m" )
	{
		return "<span style='color:black'>";
	}
	else if ( code == "[31m" )
	{
		return "<span style='color:red'>";
	}
	else if ( code == "[32m" )
	{
		return "<span style='color:green'>";
	}
	else if ( code == "[33m" )
	{
		return "<span style='color:yellow'>";
	}
	else if ( code == "[34m" )
	{
		return "<span style='color:blue'>";
	}
	else if ( code == "[35m" )
	{
		return "<span style='color:magenta'>";
	}
	else if ( code == "[36m" )
	{
		return "<span style='color:cyan'>";
	}
	else if ( code == "[37m" )
	{
		return "<span style='color:white'>";
	}

	return "";
}

void NodeLogWindow::Clear()
{
    mLog->clear();
}
