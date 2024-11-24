#include "LoginScreen.h"

LoginScreen::LoginScreen(QWidget *parent)
	: QWidget(parent)
{
	//ui.setupUi(this);

    usernameInput = new QLineEdit(this);
    usernameInput->setGeometry(20, 60, 200, 30);
    usernameInput->setPlaceholderText("Username");

   
    loginButton = new QPushButton("Login", this);
    loginButton->setGeometry(20, 140, 90, 30);


    //connect(loginButton, &QPushButton::clicked, this, &LoginScreen::loginClicked);
    //connect(registerButton, &QPushButton::clicked, this, &LoginScreen::registerClicked);
}

LoginScreen::~LoginScreen()
{}
