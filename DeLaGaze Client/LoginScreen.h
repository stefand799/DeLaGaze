#pragma once

#include <QWidget>
#include "qlineedit.h"
#include "qpushbutton.h"
#include "ui_LoginScreen.h"

class LoginScreen :public QWidget {
	Q_OBJECT
public:
	LoginScreen(QWidget* parent = nullptr);
	~LoginScreen();
private:
	Ui::LoginScreen ui;
	QLineEdit* usernameInput;
	QPushButton* loginButton;
	
};

