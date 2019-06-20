#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), cFolder (false),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    text = ct.toString("hh:mm");
    date = dt.toString("yyyy.MM.dd  -  ");
    QString login = date + text + " LOGIN";
    ui->StatusBrowser->append(login);

    myVideoCapture = new videoCapture(this);
    myImageConfiguration = new imageConfiguration(this);

    qRegisterMetaType<cv::Mat>("cv::Mat");

    connect (myVideoCapture, SIGNAL(startEnabled(bool)), this, SLOT(setEnableStartButton(bool)));
    connect (myVideoCapture, SIGNAL(processImage(QImage)), this, SLOT(showCapture(QImage)));
    connect (myVideoCapture, SIGNAL(processACQImage(QImage)), this, SLOT(showACQImage(QImage)));
    connect (myVideoCapture, SIGNAL(sendText(QString)), this, SLOT(getText(QString)));
    connect (myVideoCapture, SIGNAL(frameToPass(Mat)), myImageConfiguration, SLOT(framePassed(Mat)), Qt::QueuedConnection);

    connect (myImageConfiguration, SIGNAL(processImageConf(QImage)), this, SLOT(showConfImage(QImage)));
    connect (myImageConfiguration, SIGNAL(processImageToSave(Mat)), this, SLOT(saveImageConfDialog(Mat)));
    connect (myImageConfiguration, SIGNAL(sendText(QString)), this, SLOT(getText(QString)));

    connect (ui->filterBox, SIGNAL(currentIndexChanged(int)), myImageConfiguration, SLOT(imageFilter(int)));
    connect (ui->kernelBox, SIGNAL(currentIndexChanged(int)), myImageConfiguration, SLOT(kernelSize(int)));
    connect (ui->cameraBox, SIGNAL(currentIndexChanged(int)), myVideoCapture, SLOT(cameraType(int)));
    connect (this, SIGNAL(frameLoadToPass(Mat)), myImageConfiguration, SLOT(framePassed(Mat)));

    cameraBoxptr = ui->cameraBox;
    cameraBoxptr->setStyleSheet("QComboBox{border: 1px solid white}; {font-color: solid white}; { background: lightgray }");

    this->setStyleSheet("QGroupBox::title {"
                        "color: white;}");

    filterBoxptr = ui->filterBox;
    filterBoxptr->setStyleSheet("QComboBox{border: 1px solid white}; {font-color: solid white}; {background: lightgray}");

    kernelBoxptr = ui->kernelBox;
    kernelBoxptr->setStyleSheet("QComboBox{border: 1px solid white}; {font-color: solid white}; {background: lightgray}");

    startptr = ui->startButton;
    startptr->setEnabled(false);

    loadptr = ui->LoadButton;

    ACQptr = ui->ACQButton;
    ACQptr->setEnabled(false);

    saveptr = ui->saveButton;
    saveptr->setEnabled(false);

    deleteptr = ui->deleteButton;
    deleteptr->setEnabled(false);

    undoptr = ui->undoButton;
    undoptr->setEnabled(false);

    saveptr2 = ui->save2Button;
    saveptr2->setEnabled(false);

    txtptr = ui->txtButton;

    mainTabptr = ui->mainTab;

    if(myVideoCapture->isStopped())
    {
        QImage log("/home/pi/Documents/GUI/Multimedia/logotipo.png");
        ui->lbl_image->setAlignment(Qt::AlignCenter);
        ui->lbl_image->setPixmap(QPixmap::fromImage(log));
    }

    QPixmap imageIconStart("/home/pi/Documents/GUI/Multimedia/play-icon.png");
    QPixmap imageIconStop("/home/pi/Documents/GUI/Multimedia/stop.png");
    QIcon *iconStartStop = new QIcon();
    iconStartStop->addPixmap(imageIconStart, QIcon::Normal, QIcon::Off);
    iconStartStop->addPixmap(imageIconStop, QIcon::Normal, QIcon::On);
    startptr->setIcon(*iconStartStop);
    startptr->setCheckable(true);
    startptr->setIconSize(QSize(65, 65));

    QPixmap imageIconLoad("/home/pi/Documents/GUI/Multimedia/load.png");
    QIcon iconLoad(imageIconLoad);
    loadptr->setIcon(iconLoad);
    loadptr->setIconSize(QSize(65, 65));

    QPixmap imageIconACQ("/home/pi/Documents/GUI/Multimedia/acq.png");
    QIcon iconACQ(imageIconACQ);
    ACQptr->setIcon(iconACQ);
    ACQptr->setIconSize(QSize(65, 65));

    QPixmap imageIconSave("/home/pi/Documents/GUI/Multimedia/save.png");
    QIcon iconSave(imageIconSave);
    saveptr->setIcon(iconSave);
    saveptr->setIconSize(QSize(65, 65));

    QPixmap imageIconDelete("/home/pi/Documents/GUI/Multimedia/trash.png");
    QIcon iconDelete(imageIconDelete);
    deleteptr->setIcon(iconDelete);
    deleteptr->setIconSize(QSize(65, 65));

    QPixmap imageIconUndo("/home/pi/Documents/GUI/Multimedia/undo.png");
    QIcon iconUndo(imageIconUndo);
    undoptr->setIcon(iconUndo);
    undoptr->setIconSize(QSize(65, 65));

    QPixmap imageIconSave2("/home/pi/Documents/GUI/Multimedia/save.png");
    QIcon iconSave2(imageIconSave2);
    saveptr2->setIcon(iconSave2);
    saveptr2->setIconSize(QSize(65, 65));

    QPixmap imageIconTxt("/home/pi/Documents/GUI/Multimedia/txt-icon.png");
    QIcon iconTxt(imageIconTxt);
    txtptr->setIcon(iconTxt);
    txtptr->setIconSize(QSize(65, 65));

    QPixmap backImg("/home/pi/Documents/GUI/Multimedia/background.jpg");
    backImg = backImg.scaledToWidth(ui->imgBack->width(), Qt::SmoothTransformation);
    ui->imgBack->setPixmap(backImg);
    ui->imgBack_2->setPixmap(backImg);

}

MainWindow::~MainWindow()
{
    delete myVideoCapture;
    delete ui;
}

void MainWindow::showCapture(QImage qimgFrame)
{
    if (!qimgFrame.isNull())
    {
        if (!myVideoCapture->isStopped())
        {
            ui->lbl_image->setAlignment(Qt::AlignCenter);
            ui->lbl_image->setPixmap(QPixmap::fromImage(qimgFrame));
        }
    }
}

void MainWindow::showACQImage(QImage qimgACQ)
{
    if (!qimgACQ.isNull())
    {
        qimgACQ = qimgACQ.scaledToWidth(ui->lbl_image2->width(), Qt::SmoothTransformation);
        //qimgLoad = qimgLoad.scaledToHeight(ui->lbl_image2->height(), Qt::SmoothTransformation);
        ui->lbl_image2->setAlignment(Qt::AlignCenter);
        ui->lbl_image2->setPixmap(QPixmap::fromImage(qimgACQ));
        deleteptr->setEnabled(true);
    }
}

void MainWindow::showConfImage(QImage qimgConf)
{
    if (!qimgConf.isNull())
    {
        ui->lbl_image2->setPixmap(QPixmap::fromImage(qimgConf));
        saveptr2->setEnabled(true);
        undoptr->setEnabled(true);
    }
}

void MainWindow::on_LoadButton_clicked()
{
    ct = QTime::currentTime();
    text = ct.toString("hh:mm");
    QString filename = QFileDialog::getOpenFileName(this, tr("Choose Image"), "", tr("Images (*.png *.jpg *.jpeg *.bmp *.gif)"));

    if (QString::compare(filename, QString()) != 0)
    {
       Mat imgLoad;
       Mat imgLoadU8;

       imgLoad = imread(filename.toStdString(), CV_LOAD_IMAGE_GRAYSCALE | CV_LOAD_IMAGE_ANYDEPTH);
       imgLoad.convertTo(imgLoad, CV_32F);

       emit frameLoadToPass(imgLoad);

       seek.convertToGreyScale(imgLoad, imgLoadU8);
       QImage qimgLoad((uchar*)imgLoadU8.data, imgLoadU8.cols, imgLoadU8.rows, imgLoadU8.step, QImage::Format_Indexed8);

       if (!qimgLoad.isNull())
       {
           mainTabptr->setCurrentIndex(1);
           qimgLoad = qimgLoad.scaledToWidth(ui->lbl_image2->width(), Qt::SmoothTransformation);
           //qimgLoad = qimgLoad.scaledToHeight(ui->lbl_image2->height(), Qt::SmoothTransformation);
           ui->lbl_image2->setAlignment(Qt::AlignCenter);
           ui->lbl_image2->setPixmap(QPixmap::fromImage(qimgLoad));
           QString load = text + " Image " + filename + " has been loaded";
           ui->StatusBrowser->append(load);
           deleteptr->setEnabled(true);
           undoptr->setEnabled(false);
           saveptr2->setEnabled(false);
       }
    }
}

void MainWindow::saveImageConfDialog(Mat imageConfToSave)
{
    cFolder = true;
    ct = QTime::currentTime();
    text = ct.toString("hh:mm");
    QString dateFolder;
    string filename2;
    QDir dir = QDir::root();
    QString name;
    vector<int> compression_params;
    compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
    compression_params.push_back(9);

    if(cFolder)
    {
        dateFolder = dt.toString("yyyy.MM.dd");
        dateFolder = dateFolder + "_Denoise_Images/";
        name = QDir("/home/pi/Documents/GUI/").filePath(dateFolder);
        dir.mkdir(name);
        ui->StatusBrowser->append(name);
        cFolder = false;
    }

    imageConfToSave = imageConfToSave*1000;
    imageConfToSave.convertTo(imageConfToSave, CV_16U);

    filename2 = name.toStdString() + text.toStdString() + "_dataSeekCompactPro_" + to_string(idx) + ".png";
    imwrite(filename2, imageConfToSave, compression_params);

    QString save2 = text + " Configured image has been saved with location: " + QString::fromStdString(filename2);
    ui->StatusBrowser->append(save2);

    idx++;
}

void MainWindow::on_startButton_clicked()
{
    ct = QTime::currentTime();
    text = ct.toString("hh:mm");
    if (!myVideoCapture->isRunning())
    {
        myVideoCapture->start();
        ACQptr->setEnabled(true);
        saveptr->setEnabled(true);
        QString start = text + " Running video camera";
        ui->StatusBrowser->append(start);
    }
    else
    {
        myVideoCapture->Stop();
        ACQptr->setEnabled(false);
        saveptr->setEnabled(false);
        QString stop = text + " Acquisition has stopped";
        ui->StatusBrowser->append(stop);

        QImage log("/home/pi/Documents/GUI/Multimedia/logotipo.png");
        ui->lbl_image->setAlignment(Qt::AlignCenter);
        ui->lbl_image->setPixmap(QPixmap::fromImage(log));
    }
}

void MainWindow::on_ACQButton_clicked()
{
    myVideoCapture->acqImage();
    mainTabptr->setCurrentIndex(1);
}

void MainWindow::on_saveButton_clicked()
{
    myVideoCapture->saveImage();
}

void MainWindow::on_undoButton_clicked()
{
    myImageConfiguration->doUndo();
    saveptr2->setEnabled(false);
    undoptr->setEnabled(false);
}

void MainWindow::on_deleteButton_clicked()
{
    mainTabptr->setCurrentIndex(0);
    ui->lbl_image2->clear();
}

void MainWindow::on_save2Button_clicked()
{
    myImageConfiguration->saveImageConf();
}

void MainWindow::setEnableStartButton(bool activate)
{
    if (activate)
    {
        startptr->setEnabled(true);
    }
    else startptr->setEnabled(false);
}

void MainWindow::getText(QString message)
{
    ui->StatusBrowser->append(message);
}

void MainWindow::on_txtButton_clicked()
{
    cFolder = true;
    ct = QTime::currentTime();
    text = ct.toString("hh:mm");
    QString dateFolder;
    string filename2;
    QDir dir = QDir::root();
    QString name;

    if(cFolder)
    {
        dateFolder = dt.toString("yyyy.MM.dd");
        dateFolder = dateFolder + "_Documentation_txt/";
        name = QDir("/home/pi/Documents/GUI/").filePath(dateFolder);
        dir.mkdir(name);
        cFolder = false;
    }

    filename2 = name.toStdString() + text.toStdString() + "_txtSeekCompactPro.txt";

    QFile file(QString::fromStdString(filename2));

    if(file.open(QIODevice::ReadWrite))
    {
        QTextStream stream (&file);
        stream << ui->StatusBrowser->toPlainText();
        file.flush();
        file.close();
    }

    QString txt = text + "File has been saved with location: " + QString::fromStdString(filename2);
    ui->StatusBrowser->append(txt);

}
