#include "canrxworker.h"

canRxWorker::canRxWorker(QObject *parent) :
    QObject(parent)
{
    handle = 0;
    isRunning = false;
    rxEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
}

void canRxWorker::startRxCan()
{
    VSCAN_MSG msg;
    DWORD dwTemp;

    while (isRunning) {
        rxCode = WaitForSingleObject(rxEvent, INFINITE);
        switch (rxCode) {
            case WAIT_OBJECT_0:
                VSCAN_Read(handle, &msg, 1, &dwTemp);
                if (dwTemp == 1)
                    emit recieveMessage(msg);
                break;
            default:
                qDebug() << "canThread error";
        }
    }
}

canRxWorker::~canRxWorker()
{
    this->setRunning(false);
    CloseHandle(rxEvent);
}

void canRxWorker::setHandle(VSCAN_HANDLE uhandle)
{
    handle = uhandle;
    VSCAN_SetRcvEvent(handle, rxEvent);
}

void canRxWorker::setRunning(bool run)
{
    isRunning = run;
}
