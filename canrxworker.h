#ifndef CANRXWORKER_H
#define CANRXWORKER_H

#include <QObject>
#include <QDebug>
#include <windows.h>

#include "vs_can_api.h"

class canRxWorker : public QObject
{
    Q_OBJECT
public:
    explicit canRxWorker(QObject *parent = 0);
    void setHandle(VSCAN_HANDLE uhandle);
    ~canRxWorker();

signals:
    void recieveMessage(VSCAN_MSG msg);

public slots:
    void startRxCan();
    void setRunning(bool run);

private:
    VSCAN_HANDLE handle;
    bool isRunning;

    HANDLE rxEvent;
    DWORD rxCode;

};

#endif // CANRXWORKER_H
