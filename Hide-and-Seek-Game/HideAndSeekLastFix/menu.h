#ifndef MENU_H
#define MENU_H

#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMessageBox>


class Menu : public QMainWindow
{
    Q_OBJECT

public:
    Menu(QWidget *parent = nullptr);
    ~Menu();

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    QVBoxLayout *layout;
    QPushButton *playButton;
    QPushButton *quitButton;
    QPushButton *aboutButton;

    void adjustButtonSize();

private slots:
    void onPlayButtonClicked();
    void onQuitButtonClicked();
    void onAboutButtonClicked();
};

#endif // MENU_H
