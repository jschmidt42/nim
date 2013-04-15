///
/// Copyright (C) 2013 - All Rights Reserved
/// All rights reserved. http://www.equals-forty-two.com
///

#include "Precompiled.h"
#include "NoticeMessageBox.h"

NoticeMessageBox::NoticeMessageBox(const QString& noticeId, QWidget* parent /*= nullptr*/)
	: QDialog( parent, Qt::Dialog )
	, mNoticeId( "_notice_" + noticeId )
{
	Q_ASSERT( !noticeId.isEmpty() );

	setModal( true );
	setWindowFlags( windowFlags() & ~Qt::WindowContextHelpButtonHint );

	CreateUI();
}

NoticeMessageBox::~NoticeMessageBox()
{
}

void NoticeMessageBox::SetTitle( const QString& title )
{
	setWindowTitle( title );
}

void NoticeMessageBox::SetMessage( const QString& message )
{
	mMessage = message;
	mMessageLabel->setText(message);
}

void NoticeMessageBox::CreateUI()
{
	QVBoxLayout* mainLayout = new QVBoxLayout();
	mMessageLabel = new QLabel();
	mMessageLabel->setTextInteractionFlags( Qt::TextBrowserInteraction );
	mMessageLabel->setFrameShape( QFrame::Panel );
	mMessageLabel->setFrameShadow( QFrame::Sunken );

	mShowAgainCheckbox = new QCheckBox("Show message again?");
	mShowAgainCheckbox->setChecked( IsShowAgain() );
	
	QHBoxLayout* buttonLayout = new QHBoxLayout();
	QPushButton* okButton = new QPushButton("OK");
	okButton->setFixedWidth( 50 );
	okButton->setDefault( true );

	QPushButton* cancelButton = new QPushButton("Cancel");
	cancelButton->setFixedWidth( 50 );

	buttonLayout->addWidget( okButton );
	buttonLayout->addWidget( cancelButton );

	mainLayout->addWidget( mMessageLabel, 1 );
	mainLayout->addWidget( mShowAgainCheckbox, 0, Qt::AlignCenter );
	mainLayout->addLayout( buttonLayout, 0 );

	mainLayout->setSizeConstraint( QLayout::SetFixedSize );

	connect( okButton, SIGNAL(clicked()), this, SLOT(accept()) );
	connect( cancelButton, SIGNAL(clicked()), this, SLOT(reject()) );

	setLayout( mainLayout );
}

void NoticeMessageBox::done( int r )
{
	QSettings().setValue( mNoticeId, mShowAgainCheckbox->isChecked() );
	QDialog::done(r);
}

int NoticeMessageBox::exec()
{
	if ( IsShowAgain() )
		return QDialog::exec();

	return QDialog::Accepted;
}

bool NoticeMessageBox::IsShowAgain() const
{
	if ( QApplication::keyboardModifiers() & Qt::CTRL )
		return true;

	return QSettings().value(mNoticeId, true).toBool();
}
