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
