#include "videocapture.h"

videoCapture::videoCapture(QObject *parent)
    : QThread(parent), stop(true), acq(false), save(false)
{
    text = ct.toString("hh:mm");
}

videoCapture::~videoCapture()
{
    stop = true;
    wait();
}

void videoCapture::cameraType(int c)
{
    ct = QTime::currentTime();
    text = ct.toString("hh:mm");
    if (c == 1)
    {
        if (!seek.isOpened())
        {
             QString open = text + " Camera open";
             seek.open();
             emit sendText(open);
             emit startEnabled(true);
        }
    }
    if (c == 2 || c == 0)
    {
        QString close = text + " Camera close";
        seek.close();
        emit sendText(close);
        emit startEnabled(false);
        exit();
    }

}

void videoCapture::run()
{
    if (seek.isOpened())
    {
        stop = false;

        Mat fileA (HEIGHT, WIDHT, TYPE);
        Mat fileB (HEIGHT, WIDHT, TYPE);

        binaryFiles(fileA, fileB);
        int idx = 0;

        char filename[50];
        string filename2;

        struct tm *timeString;
        time_t now = time(NULL);

        Mat frameToSave;
        Mat framePNG;
        vector<int> compression_params;

        compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
        compression_params.push_back(9);

        while(!stop)
        {
            if (seek.grab())
            {
                seek.retrieve(frame);
                frame.convertTo(frame, TYPE);

                if(!frame.empty())
                {
                    //NUC
                    multiply(frame, fileA, frame);
                    frame = frame - fileB;

                    //Calibration
                    multiply(frame, G, frame);
                    frame = frame - Offf;

                    seek.convertToGreyScale(frame, frameU8);
                    QImage qimgFrame((uchar*)frameU8.data, frameU8.cols, frameU8.rows, frameU8.step, QImage::Format_Indexed8);

                    emit processImage(qimgFrame);
                }
            }
            if (acq)
            {
                ct = QTime::currentTime();
                text = ct.toString("hh:mm");
                ModImage = frame.clone();
                seek.convertToGreyScale(ModImage, ModImageU8);
                QImage qimgACQ((uchar*)ModImageU8.data, ModImageU8.cols, ModImageU8.rows, ModImageU8.step, QImage::Format_Indexed8);

                emit processACQImage(qimgACQ);
                emit frameToPass(ModImage);

                QString ACQ = text + " Acquisition has done";
                emit sendText(ACQ);

                acq = false;
            }
            if (save)
            {
                ct = QTime::currentTime();
                text = ct.toString("hh:mm");
                frameToSave = frame.clone();
                frameToSave = frameToSave*1000;
                frameToSave.convertTo(framePNG, CV_16U);

                timeString = gmtime(&now);
                strftime(filename, sizeof(filename), "%Y-%m-%d_%H%M%S_dataSeekCompactPro", timeString);
                filename2 = string(filename) + "_" + to_string(idx+1) + ".png" ;

                imwrite(filename2, framePNG, compression_params);

                idx = idx + 1;

                if (idx == IMNUMBER)
                {
                    idx = 0;
                    save = false;
                    QString save = text + " images have been saved";
                    emit sendText(save);
                }
            }
        }
    }
}

void videoCapture::binaryFiles(Mat file1, Mat file2)
{
    FILE* A;
    FILE* B;
    float buff [76802];
    float aux;

    A = fopen("A.bin", "rb");
    B = fopen("B.bin", "rb");

    if (A != NULL && B != NULL)
    {
        fread (buff,sizeof(float_t),2,A);

        memcpy (&aux, buff, sizeof(float_t));
        file1.rows = aux;

        memcpy (&aux, buff+1, sizeof(float_t));
        file1.cols = aux;

        fread (file1.data,sizeof(float_t),76800,A);

        fread (buff,sizeof(float_t),2,B);

        memcpy (&aux, buff, sizeof(float_t));
        file2.rows = aux;

        memcpy (&aux, buff+1, sizeof(float_t));
        file2.cols = aux;

        fread (file2.data,sizeof(float_t),76800,B);

        fclose(A);
        fclose(B);
    }
    else
    {
        QString erroF = " Error reading files";
        emit sendText(erroF);
    }
}

void videoCapture::passFrameBetweenThreads()
{
    emit frameToPass(ModImage);
}

void videoCapture::acqImage()
{
    acq = true;    
}

void videoCapture::saveImage()
{
    save = true;
}

bool videoCapture::isStopped() const
{
    return this->stop;
}

void videoCapture::Stop()
{
    stop = true;
}
