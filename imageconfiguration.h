#ifndef IMAGECONFIGURATION_H
#define IMAGECONFIGURATION_H

#include <QObject>
#include <QImage>
#include <QThread>
#include <QTime>
#include <QWaitCondition>

#include <opencv2/highgui/highgui.hpp>
#include <core.hpp>

#include "seek.h"

#include <iostream>
#include <fstream>

#define HEIGHT 240      /*!< Input frame's height */
#define WIDHT 320       /*!< Input frame's width */
#define TYPE CV_32FC1   /*!< Input frame's type */

using namespace std; 		/** @namespace std */
using namespace cv;			/** @namespace cv */
using namespace LibSeek;	/** @namespace LibSeek */
/**
 * @brief The imageConfiguration class
 *
 *      Working Thread en el que se lleva a cabo las tareas relacionadas con la
 *      configuración de las imágenes capturadas con la cámara térmica.
 *      En este caso se realiza el filtrado de ruido como procesamiento, y se
 *      elige entre cuatro tipos diferentes, además del tamaño de kernel
 *      correspondiente.
 *
 */

class imageConfiguration : public QThread
{
    Q_OBJECT

public:

    /**
     * @brief imageConfiguration
     * @param parent
     */

    imageConfiguration(QObject *parent = 0);
    ~imageConfiguration();

    /** @fn 	doImageFilter()
     * 	@brief 	Se realiza el filtrado elegido a la imagen
     * 	@return void
     */

    void doImageFilter();

    /** @fn 	GaussianFilter()
     * 	@brief 	Se realiza el filtrado Gaussiano a la imagen
     * 	@return void
     *  \sa emit processImageConf(qimgGauss)
     *  \sa emit sendText(gauss)
     *  \sa emit sendText(errG)
     */

    void GaussianFilter();

    /** @fn 	BilateralFilter()
     * 	@brief 	Se realiza el filtrado Bilateral a la imagen
     * 	@return void
     *  \sa emit processImageConf(qimgBilateral)
     *  \sa emit sendText(bilateral)
     */

    void BilateralFilter();

    /** @fn 	HomoFilter()
     * 	@brief 	Se realiza el filtrado Homogéneo a la imagen
     * 	@return void
     *  \sa emit processImageConf(qimgHomo)
     *  \sa emit sendText(homo)
     */

    void HomoFilter();

    /** @fn 	MedianFilter()
     * 	@brief 	Se realiza el filtrado Mediano a la imagen
     * 	@return void
     *  \sa emit processImageConf(qimgConf)
     *  \sa emit sendText(undo)
     */

    void MedianFilter();

    /** @fn 	doUndo()
     * 	@brief 	Se eliminan los cambios realizados en la
     *          imagen referentes al filtrado de ruido
     * 	@return void
     * \sa emit processImageConf(qimgMedian)
     * \sa emit sendText(median)
     * \sa emit sendText(errM)
     */

    void doUndo();

public slots:

    /** @fn 	saveImageConf()
     * 	@brief 	Slot que se activa cuando se pulsa el botón de Save to File 2.
     *          Emite una señal a MainWindow con la imagen configurada (después
     *          de aplicarle filtrado de ruido) en formato QImage.
     * 	@return void
     *  \sa emit processImageToSave(imageConf)
     */

    void saveImageConf();

    /** @fn 	imageFilter(int arg)
     * 	@brief 	Slot que se activa cuando se elige un tipo de filtro en la QComboBox.
     *          Recibe el filtro de ruido elegido y llama a la función doImageFilter()
     *          para que lo aplique.
     *  @param  int arg: Índice de la QComboBox elegido.
     * 	@return void
     */

    void imageFilter(int arg);

    /** @fn 	kernelSize(int k)
     * 	@brief 	Slot que se activa cuando se elige el tamaño de kernel en la QComboBox.
     *          Recibe el tamaño de Kernel elegido y llama a la función doImageFilter()
     *          para que aplique el filtro con ese kernel.
     *  @param  int k: Índice de la QComboBox elegido.
     * 	@return void
     */

    void kernelSize(int k);

    /** @fn 	framePassed(Mat imgToConfigurated)
     * 	@brief 	Slot que recibe el frame a modificar enviado por el QThread
     *          videoCapture.
     *  @param  Mat ImgToConfigurate: Frame adquirido por la cámara con NUC
     *          y Calibrado.
     * 	@return void
     */

    void framePassed(Mat imgToConfigurated);

private:
    SeekThermalPro seek;                /**< @brief Instancia del objeto seek, para la cámara Seek
                                            Thermal Compact Pro */
    QString text;                       /**< @brief String para mostrar la hora en la QTextBrowser */
    QTime ct = QTime::currentTime();    /**< @brief Atributo que almacena la hora */

    Mat ModImage;                       /**< @brief Copia de imagen pasada desde el QThread videoCapture
                                            hasta el Qthread imageConfiguration para ser modificada
                                            por el filtro de ruido */
    Mat imageConf;                      /**< @brief Imagen con filtrado de ruido aplicado y que se pasará a
                                            MainWindow para que la guarde */
    Mat ModImageU8;                     /**< @brief Imagen de 8 bits del Mat imageConf (usada para imprimir
                                            por pantalla su QImage correspondiente) */

    bool kernel;                        /**< @brief Variable usada para que solo se aplique filtro de ruido
                                            si se ha elegido tamaño de Kernel previamente */

    int n;                              /**< @brief Variable global a la que se le asigna el valor del
                                            índice en la QComboBox del tamaño del Kernel */
    int f;                              /**< @brief Variable global a la que se le asigna el valor del
                                            índice en la QComboBox del filtro de ruido elegido */

signals:

    /** @fn 	processImageConf(const QImage &image)
     * 	@brief 	Signal que envía la QImage correspondiente a la imagen Mat con
     *          filtrado de ruido aplicado. Se activa cada vez que se elige filtro
     *          de ruido o tamaño de Kernel.
     *  @param  const QImage &image: Imagen de 8 bits.
     * 	@return void
     */

    void processImageConf(const QImage &image);

    /** @fn 	processImageToSave(const Mat &mat)
     * 	@brief 	Signal que envía la imagen Mat, con filtrado de ruido aplicado,
     *          a la clase MainWindow para que se encargue de guardarla en formato
     *          .png. Se activa cuando se pulsa Save to File 2 (Save File de la pestaña
     *          de configuración de la GUI).
     *  @param  const Mat &mat: Imagen Mat con filtrado de ruido.
     * 	@return void
     */

    void processImageToSave(const Mat &mat);

    /** @fn 	sendText(const QString &string)
     * 	@brief 	Signal que se usa cada vez que se quiere imprimir un mensaje por pantalla.
     *          Se envía a MainWindow.
     *  @param  const QString &string: Mensaje que se quiere imprimir.
     * 	@return void
     */

    void sendText(const QString &string);
};

#endif // IMAGECONFIGURATION_H
