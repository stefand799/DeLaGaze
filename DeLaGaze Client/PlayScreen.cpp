#include "PlayScreen.h"
#include "qformlayout.h"

PlayScreen::PlayScreen(QWidget* parent)
	: QWidget(parent),

	/*TODO: stop hardcoding strings. Create a separate file for them, akin to the android strings.xml ( gotta be the .qrc here i think)*/
	ffaGameDescription("An all out feline mayhem, where everyone must fend for themselves!\n\n * 2-4 players"),
	teamGameDescription("A collaborative feline showdown,where everyone must fend for themselves, and their teammate!\n\n *2 teams of 2 players each \n* This gamemode does NOT offer any points\n "),
	ffaGameTextEdit(new QTextEdit( this)),
	teamGameTextEdit(new QTextEdit(this)),
	ffaGameButton(new QPushButton("Free For All", this)),
	teamGameButton(new QPushButton("Team", this))
{
	/*TODO: scrollbarPolicies, padding, propersizing*/
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
	
	setLayout(groupLayout);

}

PlayScreen::~PlayScreen()
{}

void PlayScreen::buttonClicked()
{
}
