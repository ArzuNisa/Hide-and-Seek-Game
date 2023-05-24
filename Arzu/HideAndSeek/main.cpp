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
    int whiteSqureSize = height * 4;
    int whiteSlide = whiteSqureSize/2.0;
};

struct Ghost {
    int x, y; // Hayaletin konumu
    int width, height; // Hayaletin boyutu
    bool isHidden; // Hayaletin görünürlüğü
    bool isCatch;
};

class GameWidget : public QWidget {
    int scWidth = 1920;
    int scHeight = 1080;
public:
    GameWidget(QWidget *parent = nullptr)
        : QWidget(parent), player({ 0, 50, 20, 20, 38, 0}), player2({ scWidth - 100, 50, 20, 20, 38, 0}), ghosts(), timer()
    {

        setFixedSize(scWidth, scHeight);

        // Rastgele hayaletler oluşturma
        for (int i = 0; i < numGhosts; ++i) {
            int x = QRandomGenerator::global()->bounded(width() - player.width);
            int y = QRandomGenerator::global()->bounded(height() - player.height);
            ghosts.push_back({ x, y, 20, 20, false, false});
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

        // Beyaz kareyi çizme p1
        painter.fillRect(player.x - player.whiteSlide, player.y - player.whiteSlide, player.width + player.whiteSqureSize, player.height + player.whiteSqureSize, Qt::gray);
        // Oyuncuyu çizme p1
        painter.fillRect(player.x, player.y, player.width, player.height, Qt::blue);

        // Beyaz kareyi çizme p2
        painter.fillRect(player2.x - player2.whiteSlide, player2.y - player2.whiteSlide, player2.width + player2.whiteSqureSize, player2.height + player2.whiteSqureSize, Qt::gray);
        // Oyuncuyu çizme p2
        painter.fillRect(player2.x, player2.y, player2.width, player2.height, Qt::green);


        // Hayaletleri çizme
        for (const auto& ghost : ghosts) {
            if (!ghost.isHidden) {
                painter.fillRect(ghost.x, ghost.y,ghost.width, ghost.height, Qt::red);
            }
            if (ghost.isCatch) {
                painter.setFont(QFont("Arial", 15));
                painter.drawText(ghost.x + 4, ghost.y + ghost.height - 2, "X");
            }
        }


        // Skoru yazdırma p1
        painter.setPen(Qt::blue);
        painter.setFont(QFont("Arial", 28));
        painter.drawText(10, 50, "Score: " + QString::number(player.score));

        // Skoru yazdırma p2
        painter.setPen(Qt::green);
        painter.setFont(QFont("Arial", 28));
        painter.drawText(scWidth - 200, 50, "Score: " + QString::number(player2.score));
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
            // Kareyle çarpışma kontrolü
            if (ghost.isHidden && player.x - player.whiteSlide < ghost.x + ghost.width &&
                player.x - player.whiteSlide + player.whiteSqureSize > ghost.x &&
                player.y - player.whiteSlide < ghost.y + ghost.height &&
                player.y - player.height  + player.whiteSqureSize> ghost.y)
            {
                ghost.isHidden = false;
            }
            // Oyuncuyla çarpışma kontrolü
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
            // Kareyle çarpışma kontrolü
            if (ghost.isHidden && player2.x - player2.whiteSlide < ghost.x + ghost.width &&
                player2.x - player2.whiteSlide + player2.whiteSqureSize > ghost.x &&
                player2.y - player2.whiteSlide < ghost.y + ghost.height &&
                player2.y - player2.height  + player2.whiteSqureSize> ghost.y)
            {
                ghost.isHidden = false;
            }
            // Oyuncuyla çarpışma kontrolü
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
            // Oyun bittiğinde yapılacak işlemler
        }

        // Ekrana tekrar çizim talebi gönderme
        update();
    }

private:
    static constexpr int numGhosts = 20; // Hayalet sayısı
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
