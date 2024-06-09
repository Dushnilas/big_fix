#include "moviewidget.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPixmap>

MovieWidget::MovieWidget(const QString &title, const QString &imagePath, QWidget *parent)
    : QWidget(parent) {
    layout = new QVBoxLayout(this);

    imageLabel = new QLabel(this);
    QPixmap pixmap(100, 150);
    pixmap.fill(Qt::gray);
    imageLabel->setPixmap(pixmap);
    layout->addWidget(imageLabel);

    titleLabel = new QLabel(title, this);
    titleLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(titleLabel);

    setLayout(layout);
}

MovieWidget::~MovieWidget() {

}
