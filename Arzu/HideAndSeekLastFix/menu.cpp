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
    layout->setSpacing(100);
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

    setStyleSheet("QPushButton { background-color: rgba(76, 175, 80, 0.5); color: white; border-radius: 5px; font-size: 28px; font-family: Arial; font-weight: bold; }"
                  "QPushButton:hover { background-color: #02B7E5; }"
                  "QWidget { background-color: #080834; }");

    layout->setAlignment(Qt::AlignVCenter);

    showFullScreen();
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
    int buttonWidth = width() * 0.35;
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
    QMessageBox msgBox;
    msgBox.setStyleSheet("QPushButton { background-color: rgba(76, 175, 80, 0.5); color: white; border-radius: 5px; font-size: 10px; font-family: Arial; font-weight: bold; }"
                         "QPushButton:hover { background-color: #02B7E5; }"
                         "QWidget { background-color: #080834; }");

    reply = msgBox.question(this, "Quit", "<span style='color: white; font-size: 18px;'>Do you want to exit the game</span>",
                            QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        qApp->quit();
    }
}


void Menu::onAboutButtonClicked()
{
    QMessageBox::information(this, "About", "<span style='color: white; font-size: 18px;'>Our game is based on quickly finding and catching hidden ghosts. It's a two-player game where the first player moves with the W, A, S, D keys and the second player moves with the arrow keys to find the hidden ghosts. Once they find a ghost, they need to pass over it to catch it. The player who catches the most ghosts in the shortest amount of time wins. Speed is key in this game!");

}
