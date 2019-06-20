#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QImage>
#include <QTime>
#include <QDate>
#include <QComboBox>
#include <QGroupBox>
#include <QMetaType>
#include <QTextStream>
#include <QMessageBox>
#include "videocapture.h"
#include "imageconfiguration.h"

#include <opencv2/highgui/highgui.hpp>

#include "seek.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

Q_DECLARE_METATYPE(Mat)

using namespace std;        /** @namespace std */
using namespace cv;         /** @namespace cv */
using namespace LibSeek;    /** @namespace LibSeek */

namespace Ui {              /** @namespace Ui */
class MainWindow;
}

/**
 * @brief The MainWindow class
 *
 *      Main Thread que se encarga de los aspectos relacionados con la GUI.
 *      Aquí se definen los slots de los QPushButton, QComboBox, QLabel, etc.
 *      También se añade la parte decorativa y visual de la GUI, con imágenes
 *      e iconos; y los mensajes de la QTextBrowser que servirán para comunicarse
 *      con el usuario.
 *
 */

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    /**
     * @brief MainWindow
     * @param parent
     */

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    videoCapture* myVideoCapture;
    imageConfiguration *myImageConfiguration;

    QString text;
    QTime ct = QTime::currentTime();
    QString date;
    QDate dt = QDate::currentDate();
    bool cFolder;
    int idx = 0;

    QPushButton *startptr;
    QPushButton *loadptr;
    QPushButton *ACQptr;
    QPushButton *saveptr;
    QPushButton *deleteptr;
    QPushButton *undoptr;
    QPushButton *saveptr2;
    QPushButton *txtptr;

    QTabWidget *mainTabptr;
    QComboBox *cameraBoxptr;
    QComboBox *filterBoxptr;
    QComboBox *kernelBoxptr;

public slots:

    /** @fn 	setEnableStartButton(bool activate)
     * 	@brief 	Slot que se activa cuando se elige un tipo de filtro en la QComboBox.
     *  @param  bool activate:
     * 	@return void
     */

    void setEnableStartButton(bool activate);

    /** @fn 	showCapture(QImage qimgFrame)
     * 	@brief 	Slot que se activa cuando se elige un tipo de filtro en la QComboBox.
     *          Recibe el filtro de ruido elegido y llama a la función doImageFilter()
     *          para que lo aplique.
     *  @param  int arg: Índice de la QComboBox elegido.
     * 	@return void
     */

    void showCapture(QImage qimgFrame);

    /** @fn 	showACQImage(QImage qimgACQ)
     * 	@brief 	Slot que se activa cuando se elige un tipo de filtro en la QComboBox.
     *          Recibe el filtro de ruido elegido y llama a la función doImageFilter()
     *          para que lo aplique.
     *  @param  int arg: Índice de la QComboBox elegido.
     * 	@return void
     */

    void showACQImage(QImage qimgACQ);

    /** @fn 	showConfImage(QImage qimgConf)
     * 	@brief 	Slot que se activa cuando se elige un tipo de filtro en la QComboBox.
     *          Recibe el filtro de ruido elegido y llama a la función doImageFilter()
     *          para que lo aplique.
     *  @param  int arg: Índice de la QComboBox elegido.
     * 	@return void
     */

    void showConfImage(QImage qimgConf);

    /** @fn 	getText(QString message)
     * 	@brief 	Slot que se activa cuando se elige un tipo de filtro en la QComboBox.
     *          Recibe el filtro de ruido elegido y llama a la función doImageFilter()
     *          para que lo aplique.
     *  @param  int arg: Índice de la QComboBox elegido.
     * 	@return void
     */

    void getText(QString message);

private slots:

    /** @fn 	on_LoadButton_clicked()
     * 	@brief 	Slot que se activa cuando se pulsa el botón Load Image, en la
     *          pestaña Camera Options de la GUI. Carga una imagen.
     * 	@return void
     */

    void on_LoadButton_clicked();

    /** @fn 	on_startButton_clicked()
     * 	@brief 	Slot que se activa cuando se pulsa el botón Start, en la pestaña
     *          Camera Options de la GUI. Inicia la cámara (videoCapture::run).
     * 	@return void
     *  \sa myVideoCapture->start()
     *  \sa myVideoCapture->Stop()
     */

    void on_startButton_clicked();

    /** @fn 	on_ACQButton_clicked()
     * 	@brief 	Slot que se activa cuando se pulsa el botón Acquisition, en la
     *          pestaña Camera Options de la GUI. Captura un frame.
     * 	@return void
     *  \sa myVideoCapture->acqImage()
     */

    void on_ACQButton_clicked();

    /** @fn 	on_saveButton_clicked()
     * 	@brief 	Slot que se activa cuando se pulsa el botón Save to file, en la
     *          pestaña Camera Options de la GUI. Guarda IMNUMBER copias en
     *          formato png.
     * 	@return void
     *  \sa myVideoCapture->saveImage()
     */

    void on_saveButton_clicked();

    /** @fn 	on_undoButton_clicked()
     * 	@brief 	Slot que se activa cuando se pulsa el botón Undo, en la pestaña
     *          Configurations de la GUI. Deshace el filtrado de ruido a la imagen.
     * 	@return void
     *  \sa myImageConfiguration->doUndo()
     */

    void on_undoButton_clicked();

    /** @fn 	on_deleteButton_clicked()
     * 	@brief 	Slot que se activa cuando se pulsa el botón Delete, en la pestaña
     *          Configurations de la GUI. Elimina la imagen capturada.
     * 	@return void
     */

    void on_deleteButton_clicked();

    /** @fn 	on_save2Button_clicked()
     * 	@brief 	Slot que se activa cuando se pulsa el botón Save to File, en la
     *          pestaña Confirations de la GUI. Guarda la imagen modificada en
     *          formato png.
     * 	@return void
     *  \sa myImageConfiguration->saveImageConf()
     */

    void on_save2Button_clicked();

    /** @fn 	on_txtButton_clicked()
     * 	@brief 	Slot que se activa cuando se pulsa el botón TXT Documentation, en
     *          la pestaña Configurations de la GUI. Guarda un seguimiento de la
     *          sesión realizada.
     * 	@return void
     */

    void on_txtButton_clicked();

    /** @fn 	saveImageConfDialog(Mat imageConfToSave)
     * 	@brief 	Slot que se activa cuando se realiza cualquiera de los filtrados
     *          de ruido a la imagen.
     *  @param  Mat imageConfToSave: Imagen en formato mat que se quiere guardar
     * 	@return void
     */

    void saveImageConfDialog(Mat imageConfToSave);

signals:

    /** @fn 	frameToPass(const Mat &mat)
     * 	@brief 	Signal que envía el frame capturado de la cámara (al pulsar ACQ) al WT
     *          imageConfiguration en formato mat de OpenCV.
     *  @param  const Mat &mat: Imagen en formato mat que se quiere modificar.
     * 	@return void
     */

    void frameLoadToPass(const Mat &mat);

private:

    SeekThermalPro seek;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
