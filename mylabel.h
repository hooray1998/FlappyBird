#ifndef MYLABEL_H
#define MYLABEL_H

#include <QtMath>
#include <QQueue>
#include <QTimer>
#include <QLabel>
#include <QPainter>
#include <QPoint>
#include <QtDebug>
#include <QMouseEvent>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QPainter>
#include <QtMath>
#include <QFile>
#include <QTextStream>


class myLabel : public QLabel
{
    Q_OBJECT
public:
    explicit myLabel(QWidget*parent = 0);

    void initGame();
    void readGrade();
    void writeGrade();
    void StartGame();
    void GameOver();
private:
    QPainter painter;
    QTimer *timer;
    QTimer *moveTimer;
    int x_move;
    int move_time;
    float SpeedY;
    float bird;

    int centerX;
    int centerY;

    QQueue<QPoint> tubeQueue;
    QQueue<char> letterQueue;

    bool over;
    bool nokey;
    int grade;
    int oldgrade;
    QFile file;
    int curKey;
    bool keyCrash;
    bool playing;

    int Tube_Height;//250
    int timerLength;//20
    int fontUnit;//10


    //QSoundEffect effect;
public slots:
    void paintEvent(QPaintEvent *);
    void moveFluently();
    void Jump(int key);
};

#endif // MYLABEL_H
