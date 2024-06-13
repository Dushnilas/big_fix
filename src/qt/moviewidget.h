#ifndef MOVIEWIDGET_H
#define MOVIEWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>

class MovieWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MovieWidget(const QString &title, const QString &imagePath, QWidget *parent = nullptr);
    ~MovieWidget() override;

private:
    QVBoxLayout *layout;
    QLabel *imageLabel;
    QLabel *titleLabel;
};

#endif // MOVIEWIDGET_H
