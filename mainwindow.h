#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>

#include "candialog.h"
#include "canmessageedit.h"
#include "canrxworker.h"

#include "lcarddialog.h"
#include "lcardworker.h"

#include "video/view.h"

#include <QDebug>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void getHandle(VSCAN_HANDLE handle);
    void getHandleSecond(VSCAN_HANDLE handle);
    void getLcardModule();

private slots:
    void setCanFormat();
    void lineToUpper();
    void lineEmptyFill(bool focussed);

    void closeCan();

    void recieveCanMessage(VSCAN_MSG msg);

    void addCanMessage();
    void sendSingleCan();
    void copyCanMessage();
    void deleteCanMessage();
    void sendAllCan();

    void clearSendTable();
    void clearRecieveTable();
    void saveSendCSV();
    void saveRecieveCSV();
    void loadSendCSV();

    void hideChannel();

    //----reserve---//
    void closeCanSecond();
    void recieveCanMessageSecond(VSCAN_MSG msg);
    //-------------//

    void adcMesure(float ch1, float ch2);
    void digitalMeasure(int ttlIn);
    void setDigitalOut();
    void getDigitalOut();
    void closeLcard();
    void adcClear();

    void mainCanAction(bool checked);
    void secondCanAction(bool checked);
    void lcardAction(bool checked);

    void rejectLcard();
    void rejectCanMain();
    void rejectCanSecond();

    void cameraViewerAction(bool checked);
    void showCameraAction();
    void hideCameraAction();

protected:
    void closeEvent(QCloseEvent *event);

private:
    Ui::MainWindow *ui;

    CanDialog *canDialog;
    canRxWorker *rxWorker;
    QThread *canRxThread;
    VSCAN_HANDLE canHandle;

    VSCAN_HANDLE canHandleSecond;
    CanDialog *canDialogSecond;
    canRxWorker *rxWorkerSecond;
    QThread *canRxThreadSecond;

    LcardDialog *lcardDialog;
    ILE140 *pModule;
    ADC_PARS_E140 adcPar;
    MODULE_DESCRIPTION_E140 md;
    QThread *lcardThread;
    LcardWorker *lcardWorker;
    float adcSample;
    int adcRound;
    float adcMeanCh1;
    float adcMeanCh2;
    float currentOffset;
    float currentScale;
    float voltageOffset;
    float voltageScale;

    void initialStart();

    view cameraView;

};

#endif // MAINWINDOW_H
