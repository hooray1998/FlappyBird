#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtDebug>
#include <QKeyEvent>



namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void initStyle();

protected:
    void keyPressEvent(QKeyEvent *event);
private:
    Ui::MainWindow *ui;
    bool playing;

signals:

};

#endif // MAINWINDOW_H
