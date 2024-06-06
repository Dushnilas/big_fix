#include "moviewidget.h"

MovieWidget::MovieWidget(const QString &title, const QString &imagePath, QWidget *parent)
    : QWidget(parent) {
    layout = new QVBoxLayout(this);

    imageLabel = new QLabel(this);
    QPixmap pixmap(100, 150); // Создаем пустой квадрат вместо изображения
    pixmap.fill(Qt::gray); // Заливаем его серым цветом
    imageLabel->setPixmap(pixmap);
    layout->addWidget(imageLabel);

    titleLabel = new QLabel(title, this);
    titleLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(titleLabel);

    setLayout(layout);
}
