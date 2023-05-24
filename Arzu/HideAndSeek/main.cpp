#include <QApplication>
#include <QWidget>
#include <QPainter>
#include <QKeyEvent>
#include <QTimer>
#include <QRandomGenerator>
#include <QDebug>


struct Player {
    int x, y; // Oyuncunun konumu
    int width, height; // Oyuncunun boyutu
    int speed; // Oyuncunun hızı
    int score; // Oyuncunun skoru
};

struct Ghost {
    int x, y; // Hayaletin konumu
    int width, height; // Hayaletin boyutu
    bool isHidden; // Hayaletin görünürlüğü
};

class GameWidget : public QWidget {
public:
    GameWidget(QWidget *parent = nullptr)
        : QWidget(parent), player({ 0, 50, 50, 50, 20, 0 }), player2({ 700, 50, 50, 50, 20, 0 }), ghosts(), timer()
    {
        setFixedSize(800, 600);

        // Rastgele hayaletler oluşturma
        for (int i = 0; i < numGhosts; ++i) {
            int x = QRandomGenerator::global()->bounded(width() - player.width);
            int y = QRandomGenerator::global()->bounded(height() - player.height);
            ghosts.push_back({ x, y, 30, 30, true });
        }

        // Timer'ı başlatma
        connect(&timer, &QTimer::timeout, this, [this]() {
            timerEvent(nullptr);
        });
        timer.start(16); // 60 FPS (16 ms)
    }

protected:
    void paintEvent(QPaintEvent *event) override {
        QPainter painter(this);

        // Yolu çizme
        painter.setBrush(Qt::black);
        painter.drawRect(0, 0, width(), height());


        // Hayaletleri çizme
        for (const auto& ghost : ghosts) {
            if (!ghost.isHidden) {
                painter.fillRect(ghost.x, ghost.y, ghost.width, ghost.height, Qt::red);
            }
        }

        // Oyuncuyu çizme p1
        painter.fillRect(player.x, player.y, player.width, player.height, Qt::blue);
        // Oyuncuyu çizme p2
        painter.fillRect(player2.x, player2.y, player2.width, player2.height, Qt::green);

        // Skoru yazdırma p1
        painter.setPen(Qt::blue);
        painter.setFont(QFont("Arial", 12));
        painter.drawText(10, 20, "Score: " + QString::number(player.score));

        // Skoru yazdırma p2
        painter.setPen(Qt::green);
        painter.setFont(QFont("Arial", 12));
        painter.drawText(700, 20, "Score: " + QString::number(player2.score));
    }

    void keyPressEvent(QKeyEvent *event) override {
        // Klavye tuşlarına göre oyuncuyu hareket ettirme
        switch (event->key()) {
        case Qt::Key_Up:
            player.y -= player.speed;
            break;
        case Qt::Key_Down:
            player.y += player.speed;
            break;
        case Qt::Key_Left:
            player.x -= player.speed;
            break;
        case Qt::Key_Right:
            player.x += player.speed;
            break;

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

        // Ekrana tekrar çizim talebi gönderme
        update();
    }

    void timerEvent(QTimerEvent *event) {
        // Timer her tetiklendiğinde hayaletleri kontrol etme
        for (auto& ghost : ghosts) {
            // Oyuncuyla çarpışma kontrolü
            if (ghost.isHidden && player.x < ghost.x + ghost.width &&
                player.x + player.width > ghost.x &&
                player.y < ghost.y + ghost.height &&
                player.y + player.height > ghost.y)
            {
                ghost.isHidden = false;
                player.score++;
                qDebug() << "Player 1 scored! Score: " << player.score;
            }

            if (ghost.isHidden && player2.x < ghost.x + ghost.width &&
                player2.x + player2.width > ghost.x &&
                player2.y < ghost.y + ghost.height &&
                player2.y + player2.height > ghost.y)
            {
                ghost.isHidden = false;
                player2.score++;
                qDebug() << "Player 2 scored! Score: " << player2.score;
            }
        }

        // Oyunun bitip bitmediğini kontrol etme
        bool gameFinished = true;
        for (const auto& ghost : ghosts) {
            if (ghost.isHidden) {
                gameFinished = false;
                break;
            }
        }

        if (gameFinished) {
            qDebug() << "Game finished! Final score: " << player.score;
            timer.stop();
            // Oyun bittiğinde yapılacak işlemler
        }

        // Ekrana tekrar çizim talebi gönderme
        update();
    }

private:
    static constexpr int numGhosts = 10; // Hayalet sayısı
    Player player; // Oyuncu
    Player player2;
    std::vector<Ghost> ghosts; // Hayaletler
    QTimer timer; // Timer
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    GameWidget gameWidget;
    gameWidget.show();

    return app.exec();
}
