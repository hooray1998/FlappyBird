#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtMath>

#define show_x 0
#define show_y 0

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("纸飞机");
    resize(1400,900);
    initStyle();
    move(show_x,show_y);


    playing = false;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initStyle()
{
    //加载样式表
    QFile file(":/psblack.css");
    if (file.open(QFile::ReadOnly)) {
        QString qss = QLatin1String(file.readAll());
        QString paletteColor = qss.mid(20, 7);
        qApp->setPalette(QPalette(QColor(paletteColor)));
        qApp->setStyleSheet(qss);
        file.close();
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    //设置某个tube必须特定的字母
    ui->label->Jump(event->key());

}
