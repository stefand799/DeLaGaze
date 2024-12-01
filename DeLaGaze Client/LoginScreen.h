#pragma once

#include <QWidget>
#include "qlineedit.h"
#include "qpushbutton.h"
#include <regex>

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
	std::regex m_validUsernamePattern;
	QLineEdit* usernameLineEdit;
	QPushButton* loginButton;
	
};

