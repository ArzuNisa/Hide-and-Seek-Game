#include <QApplication>
#include <QWidget>
#include <QPainter>
#include <QKeyEvent>
#include <QTimer>
#include <QRandomGenerator>
#include <QDebug>


class GameObject {
public:
    int x, y; // Object position
    int width, height; // Object size

    GameObject(int x, int y, int width, int height)
        : x(x), y(y), width(width), height(height)
    {
    }
};

class Player : public GameObject {
public:
    int speed; // Player speed
    int score; // Player score
    int whiteSquareSize;
    int whiteSlide;

    Player(int x, int y, int width, int height, int speed)
        : GameObject(x, y, width, height), speed(speed), score(0), whiteSquareSize(height * 4), whiteSlide(whiteSquareSize / 2.0)
    {
    }
};

class Ghost : public GameObject {
public:
    bool isHidden; // Ghost visibility
    bool isCaught;

    Ghost(int x, int y, int width, int height)
        : GameObject(x, y, width, height), isHidden(false), isCaught(false)
    {
    }
};

class GameWidget : public QWidget {
    int scWidth = 1920;
    int scHeight = 1080;
    static constexpr int numGhosts = 20; // Number of ghosts
    Player player1;
    Player player2;
    std::vector<Ghost> ghosts;
    QTimer timer;

public:
    GameWidget(QWidget *parent = nullptr)
        : QWidget(parent), player1(0, 50, 20, 20, 38), player2(scWidth - 100, 50, 20, 20, 38), ghosts(), timer()
    {
        setFixedSize(scWidth, scHeight);

        // Create random ghosts
        for (int i = 0; i < numGhosts; ++i) {
            int x = QRandomGenerator::global()->bounded(width() - player1.width);
            int y = QRandomGenerator::global()->bounded(height() - player1.height);
            ghosts.push_back(Ghost(x, y, 20, 20));
        }

        // Start the timer
        connect(&timer, &QTimer::timeout, this, [this]() {
            try {
                timerEvent(nullptr);
            } catch (const std::exception& e) {
                qDebug() << "Exception occurred: " << e.what();
            }
        });
        timer.start(16); // 60 FPS (16 ms)
    }

protected:
    void paintEvent(QPaintEvent *event) override {
        QPainter painter(this);

        // Draw the path
        painter.setBrush(Qt::black);
        painter.drawRect(0, 0, width(), height());

        // Draw the white square for player 1
        painter.fillRect(player1.x - player1.whiteSlide, player1.y - player1.whiteSlide, player1.width + player1.whiteSquareSize, player1.height + player1.whiteSquareSize, Qt::gray);
        // Draw player 1
        painter.fillRect(player1.x, player1.y, player1.width, player1.height, Qt::blue);

        // Draw the white square for player 2
        painter.fillRect(player2.x - player2.whiteSlide, player2.y - player2.whiteSlide, player2.width + player2.whiteSquareSize, player2.height + player2.whiteSquareSize, Qt::gray);
        // Draw player 2
        painter.fillRect(player2.x, player2.y, player2.width, player2.height, Qt::green);

        // Draw the ghosts
        for (const auto& ghost : ghosts) {
            try {
                if (!ghost.isHidden) {
                    painter.fillRect(ghost.x, ghost.y, ghost.width, ghost.height, Qt::red);
                }
                if (ghost.isCaught) {
                    painter.setFont(QFont("Arial", 15));
                    painter.drawText(ghost.x + 4, ghost.y + ghost.height - 2, "X");
                }
            } catch (const std::exception& e) {
                qDebug() << "Exception occurred: " << e.what();
            }
        }

        // Draw player 1's score
        painter.setPen(Qt::blue);
        painter.setFont(QFont("Arial", 28));
        painter.drawText(10, 50, "Score: " + QString::number(player1.score));

        // Draw player 2's score
        painter.setPen(Qt::green);
        painter.setFont(QFont("Arial", 28));
        painter.drawText(scWidth - 200, 50, "Score: " + QString::number(player2.score));
    }

    void keyPressEvent(QKeyEvent *event) override {
        try {
            // Move player 1 based on keyboard input
            switch (event->key()) {
            case Qt::Key_Up:
                player1.y -= player1.speed;
                break;
            case Qt::Key_Down:
                player1.y += player1.speed;
                break;
            case Qt::Key_Left:
                player1.x -= player1.speed;
                break;
            case Qt::Key_Right:
                player1.x += player1.speed;
                break;
            }

            // Move player 2 based on keyboard input
            switch (event->key()) {
            case Qt::Key_W:
                player2.y -= player2.speed;
                break;
            case Qt::Key_S:
                player2.y += player2.speed;
                break;
            case Qt::Key_A:
                player2.x -= player2.speed;
                break;
            case Qt::Key_D:
                player2.x += player2.speed;
                break;
            }

            // Request a redraw of the screen
            update();
        } catch (const std::exception& e) {
            qDebug() << "Exception occurred: " << e.what();
        }
    }

    void timerEvent(QTimerEvent *event) {
        try {
            // Check ghosts on each timer event
            for (auto& ghost : ghosts) {
                // Check collision with the white square for player 1
                if (ghost.isHidden && player1.x - player1.whiteSlide < ghost.x + ghost.width &&
                    player1.x - player1.whiteSlide + player1.whiteSquareSize > ghost.x &&
                    player1.y - player1.whiteSlide < ghost.y + ghost.height &&
                    player1.y - player1.height + player1.whiteSquareSize > ghost.y)
                {
                    ghost.isHidden = false;
                }
                // Check collision with player 1
                if (!ghost.isCaught && player1.x < ghost.x + ghost.width &&
                    player1.x + player1.width > ghost.x &&
                    player1.y < ghost.y + ghost.height &&
                    player1.y + player1.height > ghost.y)
                {
                    player1.score++;
                    ghost.isCaught = true;
                    qDebug() << "Player 1 scored! Score: " << player1.score;
                }

                // Check collision with the white square for player 2
                if (ghost.isHidden && player2.x - player2.whiteSlide < ghost.x + ghost.width &&
                    player2.x - player2.whiteSlide + player2.whiteSquareSize > ghost.x &&
                    player2.y - player2.whiteSlide < ghost.y + ghost.height &&
                    player2.y - player2.height + player2.whiteSquareSize > ghost.y)
                {
                    ghost.isHidden = false;
                }
                // Check collision with player 2
                if (!ghost.isCaught && player2.x < ghost.x + ghost.width &&
                    player2.x + player2.width > ghost.x &&
                    player2.y < ghost.y + ghost.height &&
                    player2.y + player2.height > ghost.y)
                {
                    player2.score++;
                    ghost.isCaught = true;
                    qDebug() << "Player 2 scored! Score: " << player2.score;
                }
            }

            // Request a redraw of the screen
            update();
        } catch (const std::exception& e) {
            qDebug() << "Exception occurred: " << e.what();
        }
    }
};


int main(int argc, char *argv[]) {
    try {
        QApplication app(argc, argv);
        GameWidget gameWidget;
        gameWidget.show();
        return app.exec();
    } catch (const std::exception& e) {
        qDebug() << "Exception occurred: " << e.what();
        return 1;
    }
}
