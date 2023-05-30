#include "mediaplayer.h"

MediaPlayer::MediaPlayer(QObject *parent) : QObject(parent)
{
    mediaPlayer = new QMediaPlayer();
    audioOutput = new QAudioOutput();

    mediaPlayer->setAudioOutput(audioOutput);
    connect(mediaPlayer, SIGNAL(positionChanged(qint64)), this, SLOT(positionChanged(qint64)));

    // Set the media source for the media player
    mediaPlayer->setSource(QUrl("qrc:/music/kurba.mp3"));
    audioOutput->setVolume(10);

    connect(mediaPlayer, &QMediaPlayer::mediaStatusChanged, mediaPlayer, &QMediaPlayer::play);

    // Start playing the music
    mediaPlayer->play();
}

void MediaPlayer::positionChanged(qint64 position)
{
    // Implementation for position change handling
}


