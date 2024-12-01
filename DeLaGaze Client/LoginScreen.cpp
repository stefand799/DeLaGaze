#include "LoginScreen.h"
#include "qboxlayout.h"

LoginScreen::LoginScreen(QWidget *parent)
	: QWidget(parent),
	usernameLineEdit(new QLineEdit(this)),
	loginButton(new QPushButton("LOGIN",this)),
	m_validUsernamePattern(R"(^\S{3,}$)") {
	
	
	auto groupLayout = new QVBoxLayout();
	groupLayout->addWidget(usernameLineEdit, 0, Qt::AlignHCenter);
	usernameLineEdit->setPlaceholderText("Insert your username!");	usernameLineEdit->setFixedSize(200, 40);
	groupLayout->addWidget(loginButton,0,Qt::AlignHCenter);
	loginButton->setFixedSize(100, 70);
	QFont loginButtonFont = loginButton->font();
	loginButtonFont.setPointSize(20);
	loginButton->setFont(loginButtonFont);

	auto layout = new QVBoxLayout(this);
	layout->addLayout(groupLayout);

	connect(loginButton, &QPushButton::clicked, this, &LoginScreen::manageLogin);

}

LoginScreen::~LoginScreen()
{}

void LoginScreen::manageLogin() {

	if (std::regex_match(usernameLineEdit->text().toStdString(), m_validUsernamePattern))
		emit loginSuccessful();
	else {
		usernameLineEdit->clear();
		usernameLineEdit->setPlaceholderText("Too short or illegal! Try again!");
	}
}