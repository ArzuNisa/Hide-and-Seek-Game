#include <QApplication>
#include <QWidget>
#include <QPainter>
#include <QKeyEvent>
#include <QTimer>
#include <QRandomGenerator>
#include <QDebug>
#include <QMediaPlayer>
#include <QAudioOutput>


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
        : QWidget(parent), player({ 60, 50, 20, 20, 38, 0}), player2({ scWidth - 100, 50, 20, 20, 38, 0}), ghosts(), timer(), isGameFinished(false)
    {

        setFixedSize(scWidth, scHeight);

        // Rastgele hayaletler oluşturma
        for (int i = 0; i < numGhosts; ++i) {
            int x = QRandomGenerator::global()->bounded(width() - player.width);
            int y = QRandomGenerator::global()->bounded(height() - player.height);
            ghosts.push_back({ x, y, 20, 20, true, false});
        }

        // Timer'ı başlatma
        connect(&timer, &QTimer::timeout, this, [this]() {
            timerEvent(nullptr);
        });
        timer.start(16); // 60 FPS (16 ms)

        QMediaPlayer *mediaPlayer = new QMediaPlayer();
        QAudioOutput *audioOutput = new QAudioOutput();

        mediaPlayer->setAudioOutput(audioOutput);
        connect(mediaPlayer, SIGNAL(positionChanged(qint64)), this, SLOT(positionChanged(qint64)));
        // Set the media source for the media player

        mediaPlayer->setSource(QUrl("qrc:/music/kurba.mp3"));
        audioOutput->setVolume(10);

        connect(mediaPlayer,&QMediaPlayer::mediaStatusChanged,mediaPlayer,&QMediaPlayer::play);


        // Start playing the music
        mediaPlayer->play();
    }
    //Destructor
    ~GameWidget() {
        qDebug() << "\nDestructor executed\n";
        timer.stop();
    }


protected:
    void paintEvent(QPaintEvent *event) override {
        QPainter painter(this);

        if (!isGameFinished) {
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
        else {
            // Arka planı siyah olarak ayarlama
            painter.fillRect(0, 0, width(), height(), Qt::black);

            // Oyun bittiğinde "OYUN BİTTİ" mesajını kırmızı renkte ve büyük yazıyla ekrana çizme
            painter.setPen(Qt::red);
            painter.setFont(QFont("Arial", 60, QFont::Bold));

            int textWidth = 600;
            int textHeight = 100;
            int textX = width() / 2 - textWidth / 2;
            int textY = height() / 2 - textHeight / 2 - 30; // Yukarı taşımak için -30 ekleyin

            painter.drawText(textX, textY, textWidth, textHeight, Qt::AlignCenter, "GAME OVER");


            // Kazananı yazdırma
            QString winnerText = "";
            if (player.score > player2.score) {
                winnerText += "Winner: Player 1 \n Score: " + std::to_string(player.score);
            } else if (player2.score > player.score) {
                winnerText += "Winner: Player 2 \n Score: " +  std::to_string(player2.score);
            } else {
                winnerText += "It's a tie";
            }

            // Kazanan metninin boyutlarını ve konumunu hesaplama
            int winnerWidth = 300;
            int winnerHeight = 100;
            int winnerX = width() / 2 - winnerWidth / 2;
            int winnerY = height() / 2 - winnerHeight / 2 + 50;

            painter.setPen(Qt::red);
            painter.setFont(QFont("Arial", 25, QFont::Bold));
            painter.drawText(winnerX, winnerY, winnerWidth, winnerHeight, Qt::AlignCenter, winnerText);

            // Yeniden başlatma düğmesini çizme
            int buttonWidth = 250;
            int buttonHeight = 60;
            int buttonX = width() / 2 - buttonWidth / 2;
            int buttonY = winnerY + winnerHeight + 30;

            painter.setPen(Qt::red);
            painter.setBrush(Qt::gray);
            painter.drawRect(buttonX, buttonY, buttonWidth, buttonHeight);
            painter.setFont(QFont("Arial", 20, QFont::Bold));
            painter.drawText(buttonX, buttonY, buttonWidth, buttonHeight, Qt::AlignCenter, "Restart");


        }
    }

    void keyPressEvent(QKeyEvent* event) override {
        // Klavye tuşlarına göre oyuncuyu hareket ettirme
        switch (event->key()) {
        case Qt::Key_Up:
            if (player.y - player.speed >= -10) {
                player.y -= player.speed;
            }
            break;
        case Qt::Key_Down:
            if (player.y + player.height + player.speed + 55 <= height()) {
                player.y += player.speed;
            }
            break;
        case Qt::Key_Left:
            if (player.x - player.speed >= -20) {
                player.x -= player.speed;
            }
            break;
        case Qt::Key_Right:
            if (player.x + player.width + player.speed -25 <= width()) {
                player.x += player.speed;
            }
            break;

        case Qt::Key_W:
            if (player2.y - player2.speed >= -10) {
                player2.y -= player2.speed;
            }
            break;
        case Qt::Key_S:
            if (player2.y + player2.height + player2.speed +55 <= height()) {
                player2.y += player2.speed;
            }
            break;
        case Qt::Key_A:
            if (player2.x - player2.speed >= -10) {
                player2.x -= player2.speed;
            }
            break;
        case Qt::Key_D:
            if (player2.x + player2.width + player2.speed -25<= width()) {
                player2.x += player2.speed;
            }
            break;
        }

        // Ekrana tekrar çizim talebi gönderme
        update();
    }


    void mousePressEvent(QMouseEvent* event) override {
        // Oyun bittiğinde yeniden başlatma düğmesine tıklama kontrolü
        if (isGameFinished && event->button() == Qt::LeftButton) {
            // Restart butonunun konumunu ve boyutunu hesaplama
            int buttonWidth = 250;
            int buttonHeight = 60;
            int buttonX = width() / 2 - buttonWidth / 2;
            int buttonY = height() / 2 - buttonHeight / 2 + 150;

            // Tıklanan koordinatların restart düğmesinin içinde olup olmadığını kontrol etme
            if (event->x() >= buttonX && event->x() <= buttonX + buttonWidth &&
                event->y() >= buttonY && event->y() <= buttonY + buttonHeight) {
                // Oyunu yeniden başlatma
                resetGame();
            }
        }

    }

    void resetGame() {
        // Oyuncuları başlangıç konumuna getirme
        player.x = 75;
        player.y = 50;
        player2.x = scWidth - 100;
        player2.y = 50;

        ghosts.clear();

        for (int i = 0; i < numGhosts; ++i) {
            int x = QRandomGenerator::global()->bounded(width() - player.width);
            int y = QRandomGenerator::global()->bounded(height() - player.height);
            ghosts.push_back({ x, y, 20, 20, false, false});
        }

        // Hayaletleri tekrar gizleme ve yakalanmadı olarak işaretleme
        for (auto& ghost : ghosts) {
            ghost.isHidden = true;
            ghost.isCatch = false;
        }

        // Skorları sıfırlama
        player.score = 0;
        player2.score = 0;

        // Oyunun başladığını ve bitmediğini işaretleme
        isGameFinished = false;

        // Timer'ı başlatma
        timer.start(16); // 60 FPS (16 ms)

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
                player.y - player.height + player.whiteSqureSize > ghost.y)
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
                player2.y - player2.height + player2.whiteSqureSize > ghost.y)
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
        if (player.score + player2.score == numGhosts) {
            isGameFinished = true;
            timer.stop();

        }

        // Ekrana tekrar çizim talebi gönderme
        update();
    }

private:
    Player player;
    Player player2;
    std::vector<Ghost> ghosts;
    QTimer timer;
    bool isGameFinished;
    int numGhosts = 2;
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    GameWidget gameWidget;
    gameWidget.show();

    return app.exec();
}
