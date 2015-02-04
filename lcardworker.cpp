#include "lcardworker.h"

LcardWorker::LcardWorker(QObject *parent) :
    QObject(parent)
{
    isRunning = false;
}

void LcardWorker::setPointer(ILE140 *uModule)
{
    pModule = uModule;
}

void LcardWorker::setRunning(bool run)
{
    isRunning = run;
}

void LcardWorker::startLcard()
{
    SHORT adcBuffer[0x2];
    WORD ttlIn;

    while (isRunning) {
        pModule->ADC_KADR(adcBuffer);
        emit adcMeasure((float) adcBuffer[0], (float) adcBuffer[1]);
        pModule->TTL_IN(&ttlIn);
        emit digitalMeasure((int) ttlIn);
    }
}
