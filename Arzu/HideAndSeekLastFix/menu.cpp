#include <QApplication>
#include <QVBoxLayout>
#include <QWidget>
#include <QPushButton>
#include <QMessageBox>
#include "menu.h"
#include "gamewidget.h"

Menu::Menu(QWidget *parent)
    : QMainWindow(parent)
{
    layout = new QVBoxLayout();
    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

    playButton = new QPushButton("Play", this);
    connect(playButton, SIGNAL(clicked()), this, SLOT(onPlayButtonClicked()));
    layout->addWidget(playButton, 0, Qt::AlignHCenter);

    quitButton = new QPushButton("Quit", this);
    connect(quitButton, SIGNAL(clicked()), this, SLOT(onQuitButtonClicked()));
    layout->addWidget(quitButton, 0, Qt::AlignHCenter);

    aboutButton = new QPushButton("About", this);
    connect(aboutButton, SIGNAL(clicked()), this, SLOT(onAboutButtonClicked()));
    layout->addWidget(aboutButton, 0, Qt::AlignHCenter);

    adjustButtonSize();

    setStyleSheet("QPushButton { background-color: rgba(76, 175, 80, 0.5); color: white; border-radius: 5px; font-size: 18px; }"
                  "QPushButton:hover { background-color: #45a049; }"
                  "QWidget { background-color: #333333; }");

}

Menu::~Menu()
{
    delete playButton;
    delete quitButton;
    delete aboutButton;
    delete layout;
}

void Menu::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    adjustButtonSize();
}

void Menu::adjustButtonSize()
{
    int buttonWidth = width() * 0.4;
    int buttonHeight = height() * 0.1;

    playButton->setFixedSize(buttonWidth, buttonHeight);
    quitButton->setFixedSize(buttonWidth, buttonHeight);
    aboutButton->setFixedSize(buttonWidth, buttonHeight);
}

void Menu::onPlayButtonClicked()
{
    hide();

    GameWidget* gameWidget = new GameWidget();
    gameWidget->show();


}

void Menu::onQuitButtonClicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Quit", "Oyundan çıkmak istediğinize emin misiniz?",
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        qApp->quit();
    }
}

void Menu::onAboutButtonClicked()
{
    QMessageBox::information(this, "About", "Bu oyun hakkında bilgi veren bir metin.");

}
