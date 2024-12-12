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
	void showServerError(const std::string& errorMessage);
signals:
	void loginRequest(const std::string& username);
private slots:
	void manageLogin();
private:
	std::regex m_validUsernamePattern;
	QLineEdit* usernameLineEdit;
	QPushButton* loginButton;
	
};

