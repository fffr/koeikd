#ifndef LCARDWORKER_H
#define LCARDWORKER_H

#include <QObject>
#include "Lusbapi.h"

class LcardWorker : public QObject
{
    Q_OBJECT
public:
    explicit LcardWorker(QObject *parent = 0);
    void setPointer(ILE140* uModule);

signals:
    void adcMeasure(float ch1, float ch2);
    void digitalMeasure(int ttlIn);

public slots:
    void startLcard();
    void setRunning(bool run);

private:
    bool isRunning;
    ILE140 *pModule;
};

#endif // LCARDWORKER_H
