#ifndef MOVIEWIDGET_H
#define MOVIEWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>

class MovieWidget : public QWidget {
    Q_OBJECT

public:
    MovieWidget(const QString &title, const QString &imagePath, QWidget *parent = nullptr);

private:
    QLabel *titleLabel;
    QLabel *imageLabel;
    QVBoxLayout *layout;
};

#endif // MOVIEWIDGET_H
