#pragma once

#include <QWidget>

class MiauScreen  : public QWidget
{
	Q_OBJECT

public:
	MiauScreen (const QString& imagePath,QWidget* parent = nullptr);
	virtual ~MiauScreen();
protected:
	void paintEvent(QPaintEvent* event) override;
private:
	QString m_backgroundImagePath;
};
