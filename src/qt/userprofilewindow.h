#ifndef USERPROFILEWINDOW_H
#define USERPROFILEWINDOW_H

#include <QWidget>

class UserProfileWindow : public QWidget {
    Q_OBJECT

public:
    UserProfileWindow(QWidget *parent = nullptr);
    ~UserProfileWindow();
};

#endif // USERPROFILEWINDOW_H
