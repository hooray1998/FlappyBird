#include "mylabel.h"
#define PI  3.1415926
#define Tube_Width 100
#define Tube_Distance 350
#define Tube_Max_Height_Diff 200

#define MaxHeight 900
#define MinTubeHeight 200


#define SpeedX 90 //速度的倒数
#define SpeedYChange_once 3
#define Zhongli_G 0.25
#define JumpPower 40

myLabel::myLabel(QWidget *parent) :
    QLabel(parent)
{
    srand(time(NULL));
    moveTimer = new QTimer();
    QObject::connect(moveTimer,SIGNAL(timeout()),this,SLOT(moveFluently()));
    moveTimer->start(timerLength);
    initGame();
}

void myLabel::initGame()
{
    //effect.setSource(QUrl::fromLocalFile("1.wav"));
    //effect.setVolume(1.0f);
    tubeQueue.clear();
    letterQueue.clear();

    bird=MaxHeight/2;
    nokey = false;
    SpeedY = 0;

    keyCrash = false;
    over = true;
    x_move = 0;
    move_time = -500;
    QPoint prePos(1400,200);
    for(int i=0;i<20;i++)
    {
        QPoint curPos = prePos+QPoint(Tube_Distance, rand()%(Tube_Max_Height_Diff*2)-Tube_Max_Height_Diff);
        if((curPos.y()+Tube_Height+MinTubeHeight)>=MaxHeight)
        {
            i--;
        }
        else if(curPos.y()<MinTubeHeight)
        {
            i--;
        }
        else
        {
            prePos = curPos;
            tubeQueue.enqueue(curPos);
        }
    }
    for(int i=0;i<20;i++)
    {
        letterQueue.enqueue(rand()%26+'A');
    }


    grade = 0;
    readGrade();



}


void myLabel::paintEvent(QPaintEvent *)
{
        painter.begin(this);          //进行重绘;

        int s[5] = {8,17,20,23,24};
        painter.setPen(QPen(Qt::gray, s[0], Qt::SolidLine, Qt::RoundCap));    //设置画笔;


        int curX;
        bool Break=false;
        for(int i=0;i<tubeQueue.length();i++)
        {
            curX = tubeQueue.at(i).x()-x_move;
            painter.drawRect(curX,-5,Tube_Width, tubeQueue.at(i).y());
            painter.drawRect(curX,tubeQueue.at(i).y()+Tube_Height,Tube_Width, MaxHeight-Tube_Height-tubeQueue.at(i).y());
            if(curX<=600&&curX>=(600-Tube_Width))
            {
                centerX = curX;
                centerY = tubeQueue.at(i).y();
                if(bird<=centerY||bird>=(centerY+Tube_Height))
                {
                    Break = true;
                }
            }
            if(curX+Tube_Width<=600&&600<(curX+Tube_Width+((Tube_Distance+Tube_Width)/SpeedX)))
            {
                grade ++;
                //effect.play();
                if(grade%5==0)
                    timerLength--;
            }
        }

        if(!nokey)
        {
            painter.setPen(QPen(Qt::green, 4, Qt::SolidLine, Qt::RoundCap));    //设置画笔;
            painter.setFont(QFont(NULL,3*fontUnit));
            for(int i=0;i<tubeQueue.length();i++)
            {
                curX = tubeQueue.at(i).x()-x_move;
                painter.drawText(QRect(curX,tubeQueue.at(i).y(),Tube_Width,Tube_Height), Qt::AlignCenter, QString("%1").arg(letterQueue.at(i)));
            }
        }
        //record grade
        painter.setPen(QPen(Qt::gray, s[2], Qt::SolidLine, Qt::RoundCap));    //设置画笔;
        painter.setFont(QFont(NULL,2*fontUnit));
        painter.drawText(QRect(0,0,180,30), Qt::AlignCenter, QString("History:\t%1").arg(oldgrade));
        painter.setFont(QFont(NULL,3*fontUnit));
        painter.drawText(QRect(0,30,180,50), Qt::AlignCenter, QString("Grade:\t%1").arg(grade));
        painter.setFont(QFont(NULL,4*fontUnit));
        painter.drawText(QRect(0,70,180,70), Qt::AlignCenter, QString("%1").arg((timerLength<19)?((timerLength<16)?((timerLength<13)?"极速":"高速"):("中等")):"低速"));

        //painter.setPen(QPen(Qt::gray, s[2], Qt::SolidLine, Qt::RoundCap));    //设置画笔;
        //painter.drawPoint(600,bird);
        painter.save();

        painter.translate(600,bird);
        painter.rotate(45*qAtan(SpeedY*SpeedX/(Tube_Distance+Tube_Width))/PI);
        QPixmap pix;
        pix.load("planenew.png");//434*272
        painter.drawPixmap(0,0,72,45,pix);


        painter.restore();

        //keyCrash
        if(keyCrash)
        {
            painter.setPen(QPen(Qt::red, 4, Qt::SolidLine, Qt::RoundCap));    //设置画笔;
            painter.setFont(QFont(NULL,5*fontUnit));
            char curk = curKey;
            painter.drawText(QRect(centerX,MaxHeight-Tube_Height,Tube_Width,Tube_Height), Qt::AlignCenter, QString("%1").arg(curk));
        }

        painter.end();  //重绘结束;

        if(Break||keyCrash||bird>(MaxHeight-30))
            GameOver();
}

void myLabel::moveFluently()
{
    if(over)
        return;


    bird += Zhongli_G*SpeedY;
    SpeedY+=SpeedYChange_once;

    x_move+=(Tube_Distance+Tube_Width)/SpeedX;
    move_time ++;
    if(move_time==SpeedX)
    {
        move_time = 0;
        tubeQueue.dequeue();
        letterQueue.dequeue();
        QPoint prePos = tubeQueue.last();
        for(int i=0;i<1;i++)
        {
            QPoint curPos = prePos+QPoint(Tube_Distance, rand()%(Tube_Max_Height_Diff*2)-Tube_Max_Height_Diff);
            if((curPos.y()+Tube_Height+MinTubeHeight)>=MaxHeight)
            {
                i--;
            }
            else if(curPos.y()<MinTubeHeight)
            {
                i--;
            }
            else
            {
                tubeQueue.enqueue(curPos);
                letterQueue.enqueue(rand()%26+'A');
            }
        }

    }
    update();
    moveTimer->stop();
    moveTimer->start(timerLength);

}


void myLabel::readGrade()
{
    QFile *file = new QFile("flappybird.config");
    bool ok =file->open(QIODevice::ReadOnly|QIODevice::Text);
    if(ok)
    {
        QTextStream in(file);
        in>>Tube_Height>>timerLength>>fontUnit;
        file->close();
        delete file;
    }

    file = new QFile("old.grade");
    ok =file->open(QIODevice::ReadOnly|QIODevice::Text);

    if(ok)
    {
        QTextStream in(file);
        in>>oldgrade;
        file->close();
        delete file;
    }
}
void myLabel::writeGrade()
{
    if(!nokey&&grade>oldgrade)
    {
        QFile *file = new QFile("old.grade");
        bool ok = file->open(QIODevice::WriteOnly|QIODevice::Text);

        if(ok)
        {
            QTextStream out(file);
            out<<grade;
            file->close();
            delete file;
        }
    }
}

void myLabel::Jump(int key)
{
    if(over&&key==Qt::Key_Space)
    {
        over = false;
        nokey = false;
    }
    else if(over&&key==Qt::Key_Return)
    {
        over = false;
        nokey = true;
    }
    SpeedY = -JumpPower;
    if(nokey)
        return ;
    curKey = key;
    int curX;
    for(int i=0;i<tubeQueue.length();i++)
    {
        curX = tubeQueue.at(i).x()-x_move;
        if(curX<=600&&curX>=(600-Tube_Width))
        {
            if(key!=letterQueue.at(i))
            {
                keyCrash = true;
            }
        }
    }
}

void myLabel::GameOver()
{
    painter.begin(this);          //进行重绘;
    //record grade
    if(grade>oldgrade)
        painter.setPen(QPen(Qt::yellow, 8, Qt::SolidLine, Qt::RoundCap));    //设置画笔;
    else
        painter.setPen(QPen(Qt::red, 8, Qt::SolidLine, Qt::RoundCap));    //设置画笔;
    painter.drawRect(QRect(550,300,300,200));
    painter.setFont(QFont(NULL,3*fontUnit));
    painter.drawText(QRect(550,300,300,100), Qt::AlignCenter, QString("History:\t%1").arg(oldgrade));
    painter.drawText(QRect(550,400,300,100), Qt::AlignCenter, QString("Grade:\t%1").arg(grade));
    painter.save();
    painter.rotate(345);
    painter.translate(520,640);
    if(grade>oldgrade)
    {
        painter.setPen(QPen(Qt::red, 8, Qt::SolidLine, Qt::RoundCap));    //设置画笔;
        painter.setFont(QFont(NULL,3.5*fontUnit));
        painter.drawText(QRect(0,0,300,100), Qt::AlignCenter, QString("新纪录").arg(grade));
    }
    painter.restore();
    painter.end();  //重绘结束;


    over = true;
    writeGrade();
    initGame();
}

void myLabel::StartGame()
{
    over = false;
}
