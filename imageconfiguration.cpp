#include "imageconfiguration.h"

imageConfiguration::imageConfiguration(QObject *parent)
    : QThread(parent), kernel(false)
{
    text = ct.toString("hh:mm");
}

imageConfiguration::~imageConfiguration()
{
    wait();
}

void imageConfiguration::framePassed(Mat imgToConfigurated)
{
    ModImage = imgToConfigurated;
}

void imageConfiguration::doUndo()
{
    ct = QTime::currentTime();
    text = ct.toString("hh:mm");
    seek.convertToGreyScale(ModImage, ModImageU8);
    QImage qimgConf((uchar*)ModImageU8.data, ModImageU8.cols, ModImageU8.rows, ModImageU8.step, QImage::Format_Indexed8);
    emit processImageConf(qimgConf);
    QString undo = text + " Changes have been removed";
    emit sendText(undo);
}

void imageConfiguration::doImageFilter()
{
    if (kernel)
    {
        switch(f)
        {
            case 1: GaussianFilter();
            break;
            case 2: HomoFilter();
            break;
            case 3: MedianFilter();
            break;
            case 4: BilateralFilter();
            break;
        }
    }
}

void imageConfiguration::imageFilter(int arg)
{
    f = arg;
    doImageFilter();
}

void imageConfiguration::kernelSize(int k)
{
    n = k;
    kernel = true;
    doImageFilter();
}

void imageConfiguration::GaussianFilter()
{
    ct = QTime::currentTime();
    text = ct.toString("hh:mm");

    if (n % 2 == 1)
    {
        GaussianBlur(ModImage, imageConf, Size(n,n), 0);
        seek.convertToGreyScale(imageConf, ModImageU8);
        QImage qimgGauss((uchar*)ModImageU8.data, ModImageU8.cols, ModImageU8.rows, ModImageU8.step, QImage::Format_Indexed8);
        emit processImageConf(qimgGauss);
        QString gauss = text + " Gaussian Filter has done";
        emit sendText(gauss);
    }

    else
    {
        QString errG = text + " Invalid Kernel size for this filter";
        emit sendText(errG);
    }
}

void imageConfiguration::BilateralFilter()
{
    ct = QTime::currentTime();
    text = ct.toString("hh:mm");
    bilateralFilter(ModImage, imageConf, n, n*2, n/2);
    seek.convertToGreyScale(imageConf, ModImageU8);
    QImage qimgBilateral((uchar*)ModImageU8.data, ModImageU8.cols, ModImageU8.rows, ModImageU8.step, QImage::Format_Indexed8);
    emit processImageConf(qimgBilateral);
    QString bilateral = text + " Bilateral Filter has done";
    emit sendText(bilateral);
}

void imageConfiguration::HomoFilter()
{
    ct = QTime::currentTime();
    text = ct.toString("hh:mm");
    blur(ModImage, imageConf, Size(n,n), Point(-1,-1));
    seek.convertToGreyScale(imageConf, ModImageU8);
    QImage qimgHomo((uchar*)ModImageU8.data, ModImageU8.cols, ModImageU8.rows, ModImageU8.step, QImage::Format_Indexed8);
    emit processImageConf(qimgHomo);
    QString homo = text + " Homogeneous Filter has done";
    emit sendText(homo);
}

void imageConfiguration::MedianFilter()
{
    ct = QTime::currentTime();
    text = ct.toString("hh:mm");

    if (n % 2 == 1)
    {
        medianBlur(ModImage, imageConf, n);
        seek.convertToGreyScale(imageConf, ModImageU8);
        QImage qimgMedian((uchar*)ModImageU8.data, ModImageU8.cols, ModImageU8.rows, ModImageU8.step, QImage::Format_Indexed8);
        emit processImageConf(qimgMedian);
        QString median = text + " Median Filter has done";
        emit sendText(median);
    }
    else
    {
        QString errM = text + " Invalid Kernel size for this filter";
        emit sendText(errM);
    }
}

void imageConfiguration::saveImageConf()
{
    emit processImageToSave(imageConf);
}
