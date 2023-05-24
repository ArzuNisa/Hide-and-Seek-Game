#include <QApplication>
#include <QWidget>
#include <QPainter>
#include <QKeyEvent>
#include <QTimer>
#include <QRandomGenerator>

struct Player {
    int x, y; // Oyuncunun konumu
    int width, height; // Oyuncunun boyutu
    int speed; // Oyuncunun hızı
};

struct Ghost {
    int x, y; // Hayaletin konumu
    int width, height; // Hayaletin boyutu
    bool isHidden; // Hayaletin görünürlüğü
};

class GameWidget : public QWidget {
public:
    GameWidget(QWidget *parent = nullptr)
        : QWidget(parent), player({ 0, 0, 50, 50, 5 }), ghosts(), timer()
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

        // Oyuncuyu çizme
        painter.fillRect(player.x, player.y, player.width, player.height, Qt::blue);
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
            }
        }

        // Ekrana tekrar çizim talebi gönderme
        update();
    }

private:
    static constexpr int numGhosts = 10; // Hayalet sayısı
    Player player; // Oyuncu
    std::vector<Ghost> ghosts; // Hayaletler
    QTimer timer; // Timer
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    GameWidget gameWidget;
    gameWidget.show();

    return app.exec();
}
