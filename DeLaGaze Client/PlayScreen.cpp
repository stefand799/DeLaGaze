#include "PlayScreen.h"
#include "qformlayout.h"

PlayScreen::PlayScreen(QWidget* parent)
	:MiauScreen(":/DeLaGazeClient/images/miau_main.png", parent),
	/*TODO: stop hardcoding strings. Create a separate file for them, akin to the android strings.xml ( gotta be the .qrc here i think)*/
	ffaGameDescription("An all out feline mayhem, where everyone must fend for themselves!\n\n * 2-4 players"),
	teamGameDescription("A collaborative feline showdown,where everyone must fend for themselves, and their teammate!\n\n *2 teams of 2 players each \n* This gamemode does NOT offer any points\n "),
	ffaGameTextEdit(new QTextEdit( this)),
	teamGameTextEdit(new QTextEdit(this)),
	ffaGameButton(new QPushButton("Free For All", this)),
	teamGameButton(new QPushButton("Team", this)),
	backButton(new QPushButton("Back",this))
{
	/*TODO: scrollbarPolicies, padding, proper sizing*/
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

std::vector<std::vector<std::string>> PlayScreen::deserializeMapToGrid(const std::string& jsonString) {
	json jsonData = json::parse(jsonString);

	int mapHeight = jsonData["height"];
	int mapWidth = jsonData["width"];

	std::vector<std::vector<std::string>> grid(mapHeight, std::vector<std::string>(mapWidth, ""));

	for (const auto& obj : jsonData["map"]) {
		int x = obj["x"];
		int y = obj["y"];
		std::string type = obj["type"];

		if (x >= 0 && x < mapHeight && y >= 0 && y < mapWidth) {
			grid[x][y] = type;
		}
	}

	return grid;
}

void PlayScreen::buttonClicked()
{
	QPushButton* clickedButton = qobject_cast<QPushButton*>(sender());
	if (clickedButton==nullptr){
		qWarning() << "Sender does not belong to the known buttons!";
	} else if (clickedButton==backButton){
		qDebug() << "Back button clicked!";
		emit selectedScreen(Screen::MainScreen);
	} else if (clickedButton==ffaGameButton){
		emit selectedScreen(Screen::FFAGameScreen);
	}else if (clickedButton==teamGameButton){
		emit selectedScreen(Screen::TeamGameScreen);
	}
}
