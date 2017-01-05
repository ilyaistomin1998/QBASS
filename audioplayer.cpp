#include "audioplayer.h"
#include "ui_audioplayer.h"

AudioPlayer::AudioPlayer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AudioPlayer)
{
    ui->setupUi(this);
    ui->statusBar->setVisible(false);
    ui->menuBar->setVisible(false);
    ui->mainToolBar->setVisible(false);
    codec = QTextCodec::codecForName("utf-8");
    timer = new QTimer();
    state = waiting;
    ui->btn_play_pause->setText("play");
    ui->progress->setValue(0);
    i = 0;
    connect(ui->btn_play_pause, SIGNAL(clicked()), this, SLOT(play_pause()));
    connect(ui->btn_next, SIGNAL(clicked()), this, SLOT(next()));
    connect(ui->btn_prev, SIGNAL(clicked()), this, SLOT(prev()));
    connect(ui->btn_stop, SIGNAL(clicked()), this, SLOT(stop()));
    connect(ui->btn_open, SIGNAL(clicked()), this, SLOT(open_files()));
    connect(ui->playlist, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(playlist_doubleClicked(QModelIndex)));
    BASS_Init(-1, 44100, BASS_DEVICE_3D, 0, NULL);

    timer->setInterval(30);
    connect(timer, SIGNAL(timeout()), this, SLOT(changeTime()));

    timer->start();
}

AudioPlayer::~AudioPlayer()
{
    delete ui;
}

void AudioPlayer::play_pause(){
    if (state == playing)
    {
        ui->btn_play_pause->setText("Play");
        BASS_ChannelPause(track);
        state = paused;
    }
    else
    {
        if (!list.size())
        {
            open_files();
            if (!list.size()) return;
            timer->start();
        }
        player();
    }
}

void AudioPlayer::stop(){
    BASS_ChannelStop(track);
    state = stopped;
    ui->btn_play_pause->setText("play");
    ui->position->setText("0:00:00");
    ui->progress->setValue(0);
}

void AudioPlayer::next(){
    if (i == list.size() - 1)
        i = 0;
    else i++;
    BASS_ChannelStop(track);
    ui->btn_play_pause->setText("play");
    player();
}

void AudioPlayer::prev(){
    if (i == 0)
        i = list.size() - 1;
    else i--;
    BASS_ChannelStop(track);
    ui->btn_play_pause->setText("play");
    player();
}

void AudioPlayer::open_files(){
    QTextCodec::setCodecForLocale(codec);
    QStringList files = QFileDialog::getOpenFileNames(
        this,
        tr("Open file"),
        "/home",
        "Music file (*.mp3)");
    if (files.size())
    for (int j = 0; j < files.size(); j++)
    {
        list += files[j];
        QFileInfo fn(files.at(j));
        ui->playlist->addItem(fn.fileName());
    }
}

void AudioPlayer::player(){
    if (state != paused)
        {
            track = BASS_StreamCreateFile(false, list[i].toStdString().c_str(), 0, 0, 0);
            if (track == 0)
            {
                QMessageBox::warning(this, codec->toUnicode("Загрузка файла"), codec->toUnicode("Ошибка загрузки файла"));
                return;
            }
        }

        ui->progress->setMinimum(0);
        ui->progress->setMaximum(BASS_ChannelGetLength(track, 0));
        if (!BASS_ChannelPlay(track, false))
        {
            QMessageBox::warning(this, codec->toUnicode("Воспроизведение файла"), codec->toUnicode("Ошибка воспроизведения файла"));
            return;
        }

        ui->playlist->setCurrentRow(i);
        ui->btn_play_pause->setText("pause");
        state = playing;
}

void AudioPlayer::changeTime(){
    if (state != playing) return;

    if (BASS_ChannelGetPosition(track, 0) == BASS_ChannelGetLength(track, 0))
    {
        if (i == list.size()-1)
            i = 0;
        else i++;
        state = stopped;
        ui->btn_play_pause->setText("play");
        player();
    }

    ui->progress->setValue(BASS_ChannelGetPosition(track, 0));
    int trackPos = BASS_ChannelBytes2Seconds(track, BASS_ChannelGetPosition(track, 0));
    int trackLen = BASS_ChannelBytes2Seconds(track, BASS_ChannelGetLength(track, 0));
    int hh = (int)(trackPos / 3600);
    int mm = (int)((trackPos - hh*3600) / 60);
    trackPos = trackPos - hh*3600 - mm*60;
    QTime timePos (hh, mm, trackPos);
    ui->position->setText(timePos.toString("h:mm:ss"));
    hh = (int)(trackLen / 3600);
    mm = (int)((trackLen - hh*3600)/60);
    trackLen = trackLen - hh*3600 - mm*60;
    QTime timeLen (hh, mm, trackLen);
    ui->length->setText(timeLen.toString("h:mm:ss"));
}

void AudioPlayer::playlist_doubleClicked(const QModelIndex &index){
    BASS_ChannelStop(track);
    i = index.row();
    state = stopped;
    player();
}
