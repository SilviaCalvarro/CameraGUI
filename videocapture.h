#ifndef VIDEOCAPTURE_H
#define VIDEOCAPTURE_H

#include <QObject>
#include <QImage>
#include <QThread>
#include <QImage>
#include <QTime>
#include <QWaitCondition>
#include <QMetaType>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <core.hpp>

#include <time.h>
#include "seek.h"

#include <iostream>
#include <fstream>

#define IMNUMBER 20     /*!< Number of frames that will acquire the camera */
#define G 0.0225292653
#define Offf 321.0588

#define HEIGHT 240      /*!< Input frame's height */
#define WIDHT 320       /*!< Input frame's width */
#define TYPE CV_32FC1   /*!< Input frame's type */

using namespace std; 		/** @namespace std */
using namespace cv;			/** @namespace cv */
using namespace LibSeek;	/** @namespace LibSeek */

/**
 * @brief The videoCapture class
 *
 *      Working Thread en el que se realiza la adquisición de imágenes con
 *      la cámara térmica. Se les aplica el NUC y Calibrado y se muestra por
 *      pantalla con estas mejoras.
 */

class videoCapture : public QThread
{
    Q_OBJECT

public:

    /**
     * @brief videoCapture
     * @param parent
     */

    videoCapture(QObject *parent = 0);
    ~videoCapture();

    /** @fn 	Stop()
     * 	@brief 	Se pone el bool stop a true
     * 	@return void
     */

    void Stop();

    /** @fn 	binaryFiles(Mat file1, Mat file2)
     * 	@brief 	Se leen los ficheros binarios para realizar el NUC
     *  @param  file1: fichero A.bin
     *  @param  file2: fichero B.bin
     * 	@return void
     *  \sa emit sendText(erroF)
     */

    void binaryFiles(Mat file1, Mat file2);

    /** @fn 	passFrameBetweenThreads()
     * 	@brief 	Se pasa el frame leído en el WT videoCapture al WT
     *          imageConfiguration
     * 	@return void
     *  \sa emit frameToPass(ModImage)
     */

    void passFrameBetweenThreads();

    /** @fn 	isStopped() const
     * 	@brief 	Retorna el valor del bool stop
     * 	@return Puntero a stop
     */

    bool isStopped() const;

public slots:

    /** @fn 	run()
     * 	@brief 	Slot principal del WT. Se activa en mainwindow al pulsar el
     *          QPushButton START en la GUI, y se encarga de la adquisición
     *          de imágenes con la cámara; del NUC y calibrado; de guardar
     *          IMNUMBER imágenes en el computador y de la captación de frames
     *          de la cámara, que serán pasados al WT imageConfiguration para
     *          que sean modificados.
     * 	@return void
     *  \sa emit processImage(qimgFrame)
     *  \sa emit processACQImage(qimgACQ)
     *  \sa emit frameToPass(ModImage)
     *  \sa emit sendText(ACQ)
     *  \sa emit sendText(save)
     */

    void run();

    /** @fn 	acqImage()
     * 	@brief 	Slot que se activa cuando se pulsa el botón de adquisición de
     *          imagen. Pone a true el bool acq para que, en el slot run(),
     *          se copie el frame y se envíe como señal a MainWindow en formato
     *          QImage.
     * 	@return void
     */

    void acqImage();

    /** @fn 	saveImage()
     * 	@brief 	Slot que se activa cuando se pulsa el botón de Save to File.
     *          Pone a true el bool save para que, en el slot run(), se guarden
     *          IMNUMBER frames adquiridos por la cámara.
     * 	@return void
     */

    void saveImage();

    /** @fn 	cameraType(int c)
     * 	@brief 	Slot que se activa cuando se elige un tipo de cámara en la QComboBox.
     *          Recibe la cámara elegida y la inicializa.
     *  @param  c: Índice de la QComboBox elegido.
     * 	@return void
     *  \sa emit sendText(open)
     *  \sa emit startEnabled(true)
     *  \sa emit sendText(close)
     *  \sa emit startEnabled(false)
     */

    void cameraType(int c);

private:

    SeekThermalPro seek;                /**< @brief Instanciación del objeto seek, para la cámara Seek
                                            Thermal Compact Pro */
    QString text;                       /**< @brief String para mostrar la hora en la QTextBrowser */
    QTime ct = QTime::currentTime();    /**< @brief Atributo que almacena la hora */

    Mat frame;                          /**< @brief Frame adquirido con la cámara térmica */
    Mat frameU8;                        /**< @brief Frame en formato de 8 bits */
    Mat ModImage;                       /**< @brief Copia de un frame que se quiere modificar */
    Mat ModImageU8;                     /**< @brief Copia del frame que se quiere modificar en
                                                formato de 8 bits */

    bool stop;                          /**< @brief Bool para parar la adquisición de imágenes */
    bool acq;                           /**< @brief Bool para capturar un frame de la adquisición */
    bool save;                          /**< @brief Bool para guardar IMNUMBER copias en formato png */
    bool cFolder;

signals:

    /** @fn 	startEnabled(bool start)
     * 	@brief 	Signal que se emite cuando se elige alguna opción en la
     *          QComboBox de la GUI (pestaña de adquisición de imágenes).
     *          Sirve para habilitar o inhabilitar el botón de START.
     *  @param  start: bool que se pone a true si se elige SeekThermalPro.
     * 	@return void
     */

    void startEnabled(bool start);

    /** @fn 	processImage(const QImage &image)
     * 	@brief 	Signal que envía la imagen adquirida por la cámara y mejorada
     *          (NUC + Calibrado) a mainwindow para que sea mostrada.
     *          Está conectado con el SLOT showCapture y el objeto lbl_image
     *          (QLabel de la pestaña de adquisición de imágenes).
     *  @param  image: imagen en formato QImage que se pasa a mainwindow.
     * 	@return void
     */

    void processImage(const QImage &image);

    /** @fn 	processACQImage(const QImage &image)
     * 	@brief 	Signal que envía la imagen captada al pulsar el QPushButton
     *          ACQButton, y que será modificada. Está conectado con el SLOT
     *          showACQImage y con el objeto lbl_image2 (QLabel de la pestaña
     *          de configuración de imágenes).
     *  @param  const QImage &image: imagen que se capta al pulsar el botón ACQ
     *          y se pasa en formato QImage.
     * 	@return void
     */

    void processACQImage(const QImage &image);

    /** @fn 	sendText(const QString &string)
     * 	@brief 	Signal que se usa cada vez que se quiere imprimir un mensaje por pantalla.
     *          Se envía a MainWindow.
     *  @param  const QString &string: Mensaje que se quiere imprimir.
     * 	@return void
     */

    void sendText(const QString &string);

    /** @fn 	frameToPass(const Mat &mat)
     * 	@brief 	Signal que envía el frame capturado de la cámara (al pulsar ACQ) al WT
     *          imageConfiguration en formato mat de OpenCV.
     *  @param  const Mat &mat: Imagen en formato mat que se quiere modificar.
     * 	@return void
     */

    void frameToPass(const Mat &mat);
};

#endif // VIDEOCAPTURE_H
