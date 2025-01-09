#include "MiauScreen.h"
#include "qpixmap.h"
#include "qpainter.h"

MiauScreen::MiauScreen(const QString& imagePath, QWidget* parent)
	:QWidget(parent),m_backgroundImagePath(imagePath)
{}

MiauScreen::~MiauScreen()
{}

void MiauScreen::paintEvent(QPaintEvent* event)
{
	QWidget::paintEvent(event);
	QPainter painter(this);
	QPixmap background(m_backgroundImagePath);
	background = background.scaled(size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	painter.drawPixmap(0, 0, background);
}
