#ifndef PROGRESS_H
#define PROGRESS_H
#include <QMouseEvent>
#include <QProgressBar>
#include <QtWidgets/qframe.h>

class Progress : public QProgressBar
{
public:
    Progress() : QProgressBar(){}
    Progress(QWidget *parent = Q_NULLPTR) : QProgressBar(parent){}
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
};

#endif // PROGRESS_H
