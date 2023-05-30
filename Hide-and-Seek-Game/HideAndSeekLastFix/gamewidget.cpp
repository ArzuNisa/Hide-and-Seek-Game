#include "gamewidget.h"

extern std::vector<int> counter;

GameWidget::GameWidget(QWidget *parent)
    : QWidget(parent), player({ 60, 50, 35, 35, 38, 0}), player2({ scWidth - 100, 50, 35, 35, 38, 0}), ghosts(), timer(), isGameFinished(false)
{

    setFixedSize(scWidth, scHeight);


     // Creating random ghosts
    try {
        for (int i = 0; i < numGhosts; ++i) {
            int x = QRandomGenerator::global()->bounded((width()-300) - player.width);
            int y = QRandomGenerator::global()->bounded((height()-300) - player.height);
            ghosts.push_back({ x, y, 35, 35, true, false});
        }
    } catch (const std::bad_alloc& e) {
        qDebug() << "Failed to allocate memory for ghosts: " << e.what();

    }

    // Starting the timer
    connect(&timer, &QTimer::timeout, this, [this]() {
        try {
            timerEvent(nullptr);
        } catch (const std::exception& e) {
            qDebug() << "Exception occurred in timer event: " << e.what();
            // Handle the error accordingly (e.g., show an error message)
        }
    });
    timer.start(16); // 60 FPS (16 ms)

}
//Destructor
GameWidget::~GameWidget() {
    qDebug() << "\nDestructor executed\n";
    timer.stop();
}


void GameWidget::paintEvent(QPaintEvent *event){
    QPainter painter(this);

    try {
        if (!isGameFinished) {
            // Drawing the path
            painter.setBrush(Qt::black);
            painter.drawRect(0, 0, width(), height());
            painter.fillRect(0, 0, width(), height(), QColor("#080834"));

            // Drawing the white square for player 1
            painter.fillRect(player.x - player.whiteSlide, player.y - player.whiteSlide, player.width + player.whiteSqureSize, player.height + player.whiteSqureSize, Qt::gray);

            // Drawing player 1
            if (direction == 0)
                painter.drawPixmap(QRect(player.x, player.y, player.width, player.height), QPixmap(":/images/playerBlueRight.png"))  ;
            else if(direction == 6) {
                painter.drawImage(QRect(player.x, player.y, player.width, player.height), QImage(":/images/playerBlueRight.png"))  ;
            }
            else if(direction == 4) {
                painter.drawImage(QRect(player.x, player.y, player.width, player.height), QImage(":/images/playerBlueLeft.png"))  ;
            }
            else if(direction == 2) {
                painter.drawImage(QRect(player.x, player.y, player.width, player.height), QImage(":/images/playerBlueDown.png"))  ;
            }
            else if(direction == 8) {
                painter.drawImage(QRect(player.x, player.y, player.width, player.height), QImage(":/images/playerBlueUp.png"))  ;
            }

            // Drawing the white square for player 2
            painter.fillRect(player2.x - player2.whiteSlide, player2.y - player2.whiteSlide, player2.width + player2.whiteSqureSize, player2.height + player2.whiteSqureSize, Qt::gray);
            // Drawing player 2

            if (direction2 == 0)
                painter.drawImage(QRect(player2.x, player2.y, player2.width, player2.height), QImage(":/images/playerGreenRight.png"))  ;
            else if(direction2 == 6) {
                painter.drawImage(QRect(player2.x, player2.y, player2.width, player2.height), QImage(":/images/playerGreenRight.png"))  ;
            }
            else if(direction2 == 4) {
                painter.drawImage(QRect(player2.x, player2.y, player2.width, player2.height), QImage(":/images/playerGreenLeft.png"))  ;
            }
            else if(direction2 == 2) {
                painter.drawImage(QRect(player2.x, player2.y, player2.width, player2.height), QImage(":/images/playerGreenDown.png"))  ;
            }
            else if(direction2 == 8) {
                painter.drawImage(QRect(player2.x, player2.y, player2.width, player2.height), QImage(":/images/playerGreenUp.png"))  ;
            }


            // Drawing the ghosts
            for (const auto& ghost : ghosts ) {
                if (!ghost.isHidden) {

                    painter.drawImage(QRect(ghost.x, ghost.y,ghost.width, ghost.height), QImage(":/images/ghost.png"))  ;
                    // painter.fillRect(ghost.x, ghost.y,ghost.width, ghost.height, Qt::red);
                }
                if (ghost.isCatch) {
                    painter.setFont(QFont("Arial", 20));
                    painter.drawText(ghost.x+12, ghost.y-7 + ghost.height , "X");
                }
            }

            // Drawing the score for player 1
            painter.setPen(Qt::blue);
            painter.setFont(QFont("Arial", 28 , QFont::Bold));
            painter.drawText(10, 50, "Score: " + QString::number(player.score));

            // Drawing the score for player 2
            painter.setPen(Qt::green);
            painter.setFont(QFont("Arial", 28 , QFont::Bold));
            painter.drawText(scWidth - 200, 50, "Score: " + QString::number(player2.score));
        }
        else {
            painter.fillRect(0, 0, width(), height(), QColor("#080834"));
            painter.setPen(Qt::white);
            painter.setFont(QFont("Arial", 65, QFont::Bold));

            int textWidth = 800;
            int textHeight = 100;
            int textX = width() / 2 - textWidth / 2;
            int textY = height() / 2 - textHeight / 2 - 120; // Yukarı taşımak için -30 ekleyin

            painter.drawText(textX, textY, textWidth, textHeight, Qt::AlignCenter, "GAME OVER");


            // Drawing the winner text
            QString winnerText = "";
            if (player.score > player2.score) {

                winnerText += "Winner: Blue Player \n Score: " + std::to_string(player.score ) ;

            } else if (player2.score > player.score) {

                winnerText += "Winner: Green Player \n Score: " +  std::to_string(player2.score) ;
            } else if (player2.score == player.score){

                winnerText += "It's a tie.";
            }

            // Calculating the dimensions and position of the winner text
            int winnerWidth = 500;
            int winnerHeight = 100;
            int winnerX = width() / 2 - winnerWidth / 2;
            int winnerY = height() / 2 - winnerHeight / 2;

            painter.setPen(QColor("#02B7E5"));
            painter.setFont(QFont("Arial", 25, QFont::Bold));
            painter.drawText(winnerX, winnerY, winnerWidth, winnerHeight, Qt::AlignCenter, winnerText);

            // Drawing the restart button
            int buttonWidth = 250;
            int buttonHeight = 60;
            int buttonX = width() / 2 - buttonWidth / 2;
            int buttonY = winnerY + winnerHeight + 30;

            painter.setPen(Qt::white);
            painter.setBrush(QColor("#02B7E5"));
            painter.drawRect(buttonX, buttonY, buttonWidth, buttonHeight);
            painter.setFont(QFont("Arial", 20, QFont::Bold));
            painter.drawText(buttonX, buttonY, buttonWidth, buttonHeight, Qt::AlignCenter, "Restart");


        }

    } catch (const std::exception& e) {
        // Handle any exceptions that occur during painting
        qDebug() << "Exception caught: " << e.what();
    }
}

void GameWidget::keyPressEvent(QKeyEvent* event) {
    // Move the player based on the keyboard keys
    switch (event->key()) {
    case Qt::Key_Up:
        if (player.y - player.speed >= -10) {
            player.y -= player.speed;
            direction = 8;
        }
        break;
    case Qt::Key_Down:
        if (player.y + player.height + player.speed + 55 <= height()) {
            player.y += player.speed;
            direction = 2;
        }
        break;
    case Qt::Key_Left:
        if (player.x - player.speed >= -20) {
            player.x -= player.speed;
            direction = 4;
        }
        break;
    case Qt::Key_Right:
        if (player.x + player.width + player.speed -25 <= width()) {
            player.x += player.speed;
            direction = 6;
        }
        break;

    case Qt::Key_W:
        if (player2.y - player2.speed >= -10) {
            player2.y -= player2.speed;
            direction2 = 8;
        }
        break;
    case Qt::Key_S:
        if (player2.y + player2.height + player2.speed +55 <= height()) {
            player2.y += player2.speed;
            direction2 = 2;
        }
        break;
    case Qt::Key_A:
        if (player2.x - player2.speed >= -10) {
            player2.x -= player2.speed;
            direction2 = 4;
        }
        break;
    case Qt::Key_D:
        if (player2.x + player2.width + player2.speed -25<= width()) {
            player2.x += player2.speed;
            direction2 = 6;
        }
        break;
    }

    // Request a redraw to the screen
    update();
}

void GameWidget::mousePressEvent(QMouseEvent* event) {
    // Check for click on the restart button when the game is finished
    if (isGameFinished && event->button() == Qt::LeftButton) {
        int buttonWidth = 250;
        int buttonHeight = 60;
        int buttonX = width() / 2 - buttonWidth / 2;
        int buttonY = height() / 2 - buttonHeight / 2 + 100;

        // Check if the clicked coordinates are within the restart button
        if (event->x() >= buttonX && event->x() <= buttonX + buttonWidth &&
            event->y() >= buttonY && event->y() <= buttonY + buttonHeight) {
            resetGame();
        }
    }

}

void GameWidget::resetGame() {
    // Reset the players to their initial positions
    player.x = 75;
    player.y = 50;
    player2.x = scWidth - 100;
    player2.y = 50;

    ghosts.clear();

    for (int i = 0; i < numGhosts; ++i) {
        int x = QRandomGenerator::global()->bounded(width() - player.width);
        int y = QRandomGenerator::global()->bounded(height() - player.height);
        ghosts.push_back({ x, y, 35, 35, false, false});
    }

    for (auto& ghost : ghosts) {
        ghost.isHidden = true;
        ghost.isCatch = false;
    }

    player.score = 0;
    player2.score = 0;

    isGameFinished = false;

    timer.start(16); // 60 FPS (16 ms)

    update();


}
void GameWidget::timerEvent(QTimerEvent *event) {
    // Check the ghosts on each timer tick
    for (auto& ghost : ghosts) {
        // Check for collision with the square
        if (ghost.isHidden && player.x - player.whiteSlide < ghost.x + ghost.width &&
            player.x - player.whiteSlide + player.whiteSqureSize > ghost.x &&
            player.y - player.whiteSlide < ghost.y + ghost.height &&
            player.y - player.height + player.whiteSqureSize > ghost.y)
        {
            ghost.isHidden = false;
        }
        // Check for collision with the player
        if (!ghost.isCatch && player.x < ghost.x + ghost.width &&
            player.x + player.width > ghost.x &&
            player.y < ghost.y + ghost.height &&
            player.y + player.height > ghost.y)
        {
            player.score++;
            ghost.isCatch = true;
            qDebug() << "Player 1 scored! Score: " << player.score;
        }

        // PLAYER 2
        if (ghost.isHidden && player2.x - player2.whiteSlide < ghost.x + ghost.width &&
            player2.x - player2.whiteSlide + player2.whiteSqureSize > ghost.x &&
            player2.y - player2.whiteSlide < ghost.y + ghost.height &&
            player2.y - player2.height + player2.whiteSqureSize > ghost.y)
        {
            ghost.isHidden = false;
        }
        if (!ghost.isCatch && player2.x < ghost.x + ghost.width &&
            player2.x + player2.width > ghost.x &&
            player2.y < ghost.y + ghost.height &&
            player2.y + player2.height > ghost.y)
        {
            player2.score++;
            ghost.isCatch = true;
            qDebug() << "Player 2 scored! Score: " << player2.score;
        }
    }

    // Check if the game is over
    if (player.score> numGhosts/2 || player2.score >numGhosts/2 || player.score + player2.score == numGhosts) {
        //counter.push_back(1);
        isGameFinished = true;
        timer.stop();

    }

    // Request a redraw to the screen
    update();
}
