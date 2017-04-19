#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include <QMainWindow>
#include "progress.h"
#include <QFileDialog>
#include <QTimer>
#include <QTextCodec>
#include <QMessageBox>
#include <QTime>
#include <QString>
#include <QMouseEvent>
#include "bass.h"

namespace Ui {
class AudioPlayer;
}

class AudioPlayer : public QMainWindow
{
    Q_OBJECT

public:
    explicit AudioPlayer(QWidget *parent = 0);
    ~AudioPlayer();

private:
    Ui::AudioPlayer *ui;
    QTextCodec *codec;
    enum{
        playing,
        paused,
        stopped,
        waiting
    }state;
    int i;
    QStringList list;
    QTimer *timer;
    HSTREAM track;

    void player();

private slots:
    void play_pause();
    void stop();
    void next();
    void prev();
    void open_files();
    void changeTime();
    void playlist_doubleClicked(const QModelIndex&);
    void create_playlist();
    void delete_playlist();
};

#endif // AUDIOPLAYER_H
