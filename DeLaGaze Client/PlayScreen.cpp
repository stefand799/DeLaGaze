#include "PlayScreen.h"
#include "qformlayout.h"

PlayScreen::PlayScreen(QWidget* parent)
	:MiauScreen(":/DeLaGazeClient/images/miau_main.png", parent),
	ffaGameDescription("An all out feline mayhem, where everyone must fend for themselves!\n\n * 2-4 players"),
	teamGameDescription("A collaborative feline showdown,where everyone must fend for themselves, and their teammate!\n\n *2 teams of 2 players each (Left vs Right) \n* This gamemode does NOT offer any points\n "),
	ffaGameTextEdit(new QTextEdit( this)),
	teamGameTextEdit(new QTextEdit(this)),
	ffaGameButton(new QPushButton("Free For All", this)),
	teamGameButton(new QPushButton("Team", this)),
	backButton(new QPushButton("Back",this))
{
	ffaGameTextEdit->setText(ffaGameDescription);
	ffaGameTextEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	teamGameTextEdit->setText(teamGameDescription);
	teamGameTextEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);


	
	auto centerText = [](QTextEdit* textEdit) {
		QTextCursor cursor = textEdit->textCursor();
		cursor.select(QTextCursor::Document);
		QTextBlockFormat format;
		format.setAlignment(Qt::AlignCenter);
		cursor.mergeBlockFormat(format);
		cursor.clearSelection();
		textEdit->setTextCursor(cursor);
		textEdit->setFocusPolicy(Qt::NoFocus);
		};

	auto groupLayout = new QFormLayout();
	QList<QPair<QPushButton*, QTextEdit*>> items = { { ffaGameButton,ffaGameTextEdit }, { teamGameButton,teamGameTextEdit } };
	QFont buttonFont;
	QFont textEditFont;
	buttonFont.setPointSize(30);
	textEditFont.setPointSize(20);
	for (auto pair:items){
		QPushButton* button = pair.first;
		QTextEdit* textEdit = pair.second;
		button->setFixedSize(400,200);
		button->setFont(buttonFont);

		textEdit->setFont(textEditFont);
		textEdit->setReadOnly(true);
		centerText(textEdit);

		connect(button, &QPushButton::clicked, this, &PlayScreen::buttonClicked);
		groupLayout->addRow(button, textEdit);
	}

	QFont backButtonFont;
	backButtonFont.setPointSize(15);
	backButton->setFixedSize(200, 50);
	backButton->setFont(backButtonFont);
	connect(backButton, &QPushButton::clicked, this, &PlayScreen::buttonClicked);

	auto topLayout = new QHBoxLayout;
	topLayout->addWidget(backButton);
	topLayout->addStretch();

	auto mainLayout = new QVBoxLayout;
	mainLayout->addLayout(topLayout);
	mainLayout->addLayout(groupLayout);

	setLayout(mainLayout);
	


}

PlayScreen::~PlayScreen()
{}


void PlayScreen::buttonClicked()
{
	QPushButton* clickedButton = qobject_cast<QPushButton*>(sender());
	if (clickedButton==nullptr){
		qWarning() << "Sender does not belong to the known buttons!";
	} else if (clickedButton==backButton){
		qDebug() << "Back button clicked!";
		emit backButtonClicked();
	} else if (clickedButton==ffaGameButton){
		emit joinLobbyRequest("ffa");
	}else if (clickedButton==teamGameButton){
		emit joinLobbyRequest("teams");
	}
}
