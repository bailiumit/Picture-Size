//编写者：刘柏
//编写时间：2015.11.10
//功能：主窗体显示

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QFileDialog>
#include <QAction>
#include <QMenuBar>
#include <QObject>
#include <QDialog>
#include <QLabel>
#include <QTime>
#include <QProcess>
#include <QGraphicsItem>

#include <QPushButton>
#include <QGridLayout>
#include <QLineEdit>
#include <QString>
#include <QComboBox>
#include <QRadioButton>

#include <QIcon>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

//private:
    Ui::MainWindow *ui;

    QImage *srcImg, *destImg;
    QGraphicsView *originPicture;
    QLineEdit *heightEdit, *widthEdit, *rotateEdit;
    QComboBox *modeSelect, *methodSelect;

    float heightTimes, widthTimes, rotateDegree;  //分别代表纵向放大倍数、横向放大倍数、顺时针旋转度数
    int mode, method;

    void ImageShow(QImage* img);
    void WarningShow();

private slots:
    void ManipShow();
    void GuideShow();
    void AboutShow();
    void OpenPicture(); //信号槽
    void SavePicture();
    void StartManipulate();
};

#endif // MAINWINDOW_H
