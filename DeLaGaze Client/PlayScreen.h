#pragma once

#include <QWidget>
#include "qpushbutton.h"
#include "qtextedit.h"

class PlayScreen  : public QWidget
{
	Q_OBJECT
public:
	PlayScreen(QWidget *parent=nullptr);
	~PlayScreen();
private slots:
	void buttonClicked();
private:
	QPushButton* ffaGameButton;
	QPushButton* teamGameButton;
	QTextEdit* ffaGameTextEdit;
	QTextEdit* teamGameTextEdit;
	QString ffaGameDescription;
	QString teamGameDescription;
};
