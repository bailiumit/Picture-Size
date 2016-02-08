//编写者：刘柏
//编写时间：2015.11.10
//功能：主窗体显示

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "manipulate.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowIcon(QIcon("://icon/mainIcon.png"));
    this->setWindowTitle("Numerical Analysis Project");
    this->srcImg = new QImage();
    this->destImg = srcImg;
    this->originPicture = new QGraphicsView(this);

    //初始化主窗口界面
    QString srcRoute = "://icon/welcomeText.jpg";
    srcImg->load(srcRoute);
    this->ImageShow(srcImg);

    //初始化菜单栏
    QAction *openAction = new QAction(tr("打开"), this);
    QObject::connect(openAction, SIGNAL(triggered()), this, SLOT(OpenPicture()));
    QAction *saveOption = new QAction(tr("保存"), this);
    QObject::connect(saveOption, SIGNAL(triggered()), this, SLOT(SavePicture()));
    QMenu *openMenu = menuBar()->addMenu(tr("文件"));
    openMenu->addAction(openAction);
    openMenu->addAction(saveOption);

    QAction *manipOption = new QAction(tr("操作图片"), this);
    QObject::connect(manipOption, SIGNAL(triggered()), this, SLOT(ManipShow()));
    QMenu *manipMenu = menuBar()->addMenu(tr("操作"));
    manipMenu->addAction(manipOption);

    QAction *guideOption = new QAction(tr("使用方法"), this);
    QObject::connect(guideOption, SIGNAL(triggered()), this, SLOT(GuideShow()));
    QAction *aboutOption = new QAction(tr("关于"), this);
    QObject::connect(aboutOption, SIGNAL(triggered()), this, SLOT(AboutShow()));
    QMenu *helpMenu = menuBar()->addMenu(tr("帮助"));
    helpMenu->addAction(guideOption);
    helpMenu->addAction(aboutOption);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::OpenPicture()
{
    QString fileName = QFileDialog::getOpenFileName(
                this, "open image file",
                ".",
                "image files (*.bmp *.jpg *.jpeg *.pbm *.pgm *.png *.ppm *.xbm *.xpm);;All files (*.*)");
    if(fileName != "")
    {
        if(srcImg->load(fileName))
        {
            this->ImageShow(srcImg);
        }

    }
}

void MainWindow::SavePicture()
{
    QString fileName = QFileDialog::getSaveFileName(
                this, tr("Save File"), "./default.png", tr("Images (*.png)"));
    if(fileName != "")
    {
        destImg->save(fileName);
    }
}

void MainWindow::ImageShow(QImage *image)
{
    QGraphicsScene *scene = new QGraphicsScene;
    scene->addPixmap(QPixmap::fromImage(*image));
    originPicture->setScene(scene);
    originPicture->setGeometry(0, 56, image->width()+2, image->height()+2);
    originPicture->show();
    this->resize(image->width(), image->height() + 56);
}

void MainWindow::WarningShow()
{
    QDialog *warningDialog = new QDialog;
    QLabel *warningLabel = new QLabel;
    QString titleString = "Warning";
    warningDialog->setWindowTitle(titleString);
    warningDialog->setWindowIcon(QIcon("://icon/infoIcon.png"));
    warningLabel = new QLabel(warningDialog);
    QPixmap contentPix("://icon/warningText.png");
    warningLabel->setPixmap(contentPix);
    warningLabel->show();
    warningDialog->exec();
}

void MainWindow::ManipShow()
{
    QDialog *settings;
    QLabel *heightLabel, *widthLabel, *rotateLabel, *modeLabel, *methodLabel;
    QPushButton *confirmButton;
    QGridLayout *layout;

    settings = new QDialog;
    settings->setWindowTitle(tr("设置"));
    settings->setWindowIcon(QIcon("://icon/settingsIcon.png"));

    heightLabel = new QLabel(settings);
    QPixmap heightPix("://icon/heightIcon.png");
    heightLabel->setPixmap(heightPix);

    widthLabel = new QLabel(settings);
    QPixmap widthPix("://icon/widthIcon.png");
    widthLabel->setPixmap(widthPix);

    rotateLabel = new QLabel(settings);
    QPixmap rotatePix("://icon/rotateIcon.png");
    rotateLabel->setPixmap(rotatePix);

    modeLabel = new QLabel(settings);
    QPixmap modelPix("://icon/cropIcon.png");
    modeLabel->setPixmap(modelPix);

    methodLabel = new QLabel(settings);
    QPixmap methodPix("://icon/interIcon.png");
    methodLabel->setPixmap(methodPix);

    heightEdit = new QLineEdit(settings);
    heightEdit->setText("1");

    widthEdit = new QLineEdit(settings);
    widthEdit->setText("1");

    rotateEdit = new QLineEdit(settings);
    rotateEdit->setText("0");

    modeSelect = new QComboBox(settings);
    modeSelect->insertItem(0, tr("Crop"));
    modeSelect->insertItem(1, tr("Loose"));

    methodSelect = new QComboBox(settings);
    methodSelect->insertItem(0, tr("NearestNeighbor"));
    methodSelect->insertItem(1, tr("Bilinear"));
    methodSelect->insertItem(2, tr("Bicubic"));

    confirmButton = new QPushButton(settings);
    confirmButton->setText(tr("确定"));
    QObject::connect(confirmButton, SIGNAL(clicked()), this, SLOT(StartManipulate()));

    layout = new QGridLayout;
    layout->addWidget(heightLabel, 0, 0);
    layout->addWidget(widthLabel, 1, 0);
    layout->addWidget(rotateLabel, 2, 0);
    layout->addWidget(modeLabel, 3, 0);
    layout->addWidget(methodLabel, 4, 0);
    layout->addWidget(heightEdit, 0, 1);
    layout->addWidget(widthEdit, 1, 1);
    layout->addWidget(rotateEdit, 2, 1);
    layout->addWidget(modeSelect, 3, 1);
    layout->addWidget(methodSelect, 4, 1);
    layout->addWidget(confirmButton, 5, 0, 1, 2);

    settings->setLayout(layout);

    settings->exec();
}

void MainWindow::GuideShow()
{
    QDialog *guideDialog = new QDialog;
    QLabel *guideLabel = new QLabel;
    QString titleString = "使用说明";
    guideDialog->setWindowTitle(titleString);
    guideDialog->setWindowIcon(QIcon("://icon/helpIcon.png"));
    guideLabel = new QLabel(guideDialog);
    QPixmap contentPix("://icon/welcomeText.jpg");
    guideLabel->setPixmap(contentPix);
    guideLabel->show();
    guideDialog->exec();
}

void MainWindow::AboutShow()
{
    QDialog *aboutDialog = new QDialog;
    QLabel *aboutLabel = new QLabel;
    QString titleString = "关于";
    aboutDialog->setWindowTitle(titleString);
    aboutDialog->setWindowIcon(QIcon("://icon/infoIcon.png"));
    aboutLabel = new QLabel(aboutDialog);
    QPixmap contentPix("://icon/aboutText.png");
    aboutLabel->setPixmap(contentPix);
    aboutLabel->show();
    aboutDialog->exec();
}

void MainWindow::StartManipulate()
{
    heightTimes = heightEdit->text().toFloat();
    widthTimes = widthEdit->text().toFloat();
    rotateDegree = rotateEdit->text().toFloat();
    mode = modeSelect->currentIndex();
    method = methodSelect->currentIndex();

    if (heightTimes <= 0 || widthTimes <= 0 || srcImg->width()*widthTimes >= 1450 || srcImg->height()*heightTimes >= 1450)
    {
        this->WarningShow();
    }
    else
    {
        destImg = Manipulate(srcImg, heightTimes, widthTimes, rotateDegree, mode, method);
        this->ImageShow(destImg);
    }
}
