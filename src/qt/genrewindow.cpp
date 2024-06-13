#include "genrewindow.h"
#include <QVBoxLayout>
#include <QFont>

GenreWindow::GenreWindow(const QString &genre, QWidget *parent) : QWidget(parent), genre(genre) {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QLabel *titleLabel = new QLabel(QString("Films with genre: %1").arg(genre), this);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(18);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);

    mainLayout->addWidget(titleLabel);

    backButton = new QPushButton("Back", this);
    connect(backButton, &QPushButton::clicked, this, &GenreWindow::onBackButtonClicked);
    mainLayout->addWidget(backButton);

    userProfileButton = new QPushButton("User Profile", this);
    connect(userProfileButton, &QPushButton::clicked, this, &GenreWindow::onUserProfileButtonClicked);
    mainLayout->addWidget(userProfileButton);

    setLayout(mainLayout);
    setWindowTitle(QString("Films with genre: %1").arg(genre));
    setFixedSize(800, 600);

    setStyleSheet(
        "GenreWindow {"
        "    background: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgb(0, 0, 0), stop:1 rgb(19, 21, 59));"
        "    color: white;"
        "}"
        "QLabel {"
        "    color: rgb(229, 217, 190);"
        "}"
        "QPushButton {"
        "    background-color: rgba(255, 255, 255, 0);"
        "    color: rgb(229, 217, 190);"
        "    border: 5px solid rgb(229, 217, 190);"
        "    border-radius: 5px;"
        "    padding: 5px;"
        "}"
        );
}

void GenreWindow::onBackButtonClicked() {
    emit backToMoviesWindow();
    this->hide();
}

void GenreWindow::onUserProfileButtonClicked() {
    // Here you would typically show the user profile window.
    // For this example, we'll just print a message.
    qDebug() << "User Profile button clicked.";
}
