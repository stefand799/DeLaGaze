#pragma once

#include <QWidget>
#include "qlineedit.h"
#include "qpushbutton.h"

class LoginScreen :public QWidget {
	Q_OBJECT
public:
	LoginScreen(QWidget* parent = nullptr);
	~LoginScreen();
signals:
	void loginSuccessful();
private slots:
	void manageLogin();
private:
	QLineEdit* usernameLineEdit;
	QPushButton* loginButton;
	
};

