#include "collectiondialog.h"
#include <QMessageBox>
#include <QPixmap>
#include "backend.h"

std::string replaceAllOccurrences(const std::string &str, const std::string &toReplace) {
    std::string result = str;
    size_t pos = 0;

    while ((pos = result.find(toReplace, pos)) != std::string::npos) {
        result.replace(pos, toReplace.length(), "");
    }

    return result;
}

CollectionDialog::CollectionDialog(QWidget *parent)
    : QDialog(parent), selectedImagePath(""), selectedButton(nullptr)
{

    setWindowTitle("Add Collection");
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QLabel *nameLabel = new QLabel("Enter collection name:", this);
    nameEdit = new QLineEdit(this);
    mainLayout->addWidget(nameLabel);
    mainLayout->addWidget(nameEdit);

    QLabel *imageLabel = new QLabel("Select an image:", this);
    mainLayout->addWidget(imageLabel);

    buttonGroup = new QButtonGroup(this);
    QHBoxLayout *imagesLayout = new QHBoxLayout();

    for (int i = 1; i <= 6; ++i) {
        QString imagePath = QString(qFilePath("/pictures/collection_image_%1.jpg")).arg(i);
        QPixmap pixmap(imagePath);

        QPushButton *imageButton = new QPushButton(this);
        imageButton->setIcon(QIcon(pixmap));
        imageButton->setIconSize(QSize(100, 100));
        imageButton->setFixedSize(100, 100);
        imageButton->setCheckable(true);


        buttonGroup->addButton(imageButton, i);
        imagesLayout->addWidget(imageButton);

        connect(buttonGroup, QOverload<int>::of(&QButtonGroup::idClicked), this, &CollectionDialog::onImageButtonClicked);
    }

    mainLayout->addLayout(imagesLayout);

    QPushButton *addButton = new QPushButton("Add", this);
    connect(addButton, &QPushButton::clicked, this, &QDialog::accept);
    mainLayout->addWidget(addButton);

    setLayout(mainLayout);

    setStyleSheet(
        "CollectionDialog {"
        "    background: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgb(0, 0, 0), stop:1 rgb(19, 21, 59));"
        "    color: white;"
        "}"
        "QLabel {"
        "    color: rgb(229, 217, 190);"
        "}"
        "QPushButton {"
        "    background-color: rgba(255, 255, 255, 0);"
        "    color: rgb(229, 217, 190);"
        "    border: 1px solid rgb(229, 217, 190);"
        "    border-radius: 5px;"
        "    padding: 5px;"
        "}"

        );
}

QString CollectionDialog::getCollectionName() const
{
    return nameEdit->text();
}

QString CollectionDialog::getSelectedImagePath() const
{
    return selectedImagePath;
}

std::string CollectionDialog::getDatabaseText() const {
    std::string path = selectedImagePath.toStdString();
    return replaceAllOccurrences(path, MY_PATH);
}

void CollectionDialog::onImageButtonClicked(int id)
{
    selectedImagePath = QString(qFilePath("/pictures/collection_image_%1.jpg")).arg(id);
    selectedButton = qobject_cast<QPushButton*>(buttonGroup->button(id));

    updateButtonStyles();
}

void CollectionDialog::updateButtonStyles()
{
    QList<QAbstractButton*> buttons = buttonGroup->buttons();
    for (QAbstractButton *button : buttons) {
        QPushButton *pushButton = qobject_cast<QPushButton*>(button);
        if (pushButton) {
            if (pushButton == selectedButton) {
                pushButton->setStyleSheet("border: 3px solid red;");
            } else {
                pushButton->setStyleSheet("border: 1px solid rgb(229, 217, 190);");
            }
        }
    }
}
