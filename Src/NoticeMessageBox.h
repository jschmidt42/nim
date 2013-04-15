///
/// Copyright (C) 2013 - All Rights Reserved
/// All rights reserved. http://www.equals-forty-two.com
///
/// @brief Dialog box that presents a message and lets the user ignore further notices.
///

#pragma once

#include <QDialog>

class QLabel;
class QCheckBox;

class NoticeMessageBox : public QDialog
{
	Q_OBJECT;

public:

	NoticeMessageBox(const QString& noticeId, QWidget* parent = nullptr);
	~NoticeMessageBox();

public: // Interface

	bool IsShowAgain() const;
	void SetTitle(const QString& title);
	void SetMessage(const QString& message);

	int exec();

protected:

	virtual void done(int r) override;
	
private: // Details

	void CreateUI();

private:

	QString mNoticeId;
	QString mMessage;

private:

	QLabel* mMessageLabel;
	QCheckBox* mShowAgainCheckbox;
};