#ifndef LCARDDIALOG_H
#define LCARDDIALOG_H

#include <QDialog>

#include "Lusbapi.h"
#include <QDebug>

namespace Ui {
class LcardDialog;
}

class LcardDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LcardDialog(QWidget *parent = 0);
    ~LcardDialog();

    ILE140* getModulePointer();
    float getCurrentOffset();
    float getCurrentScale();
    float getVoltageOffset();
    float getVoltageScale();

signals:
    void lcardVirtualSlot(ILE140 *uModule);

private slots:
    void getInfLcard();
    void refreshLcardList();

    void lineEmptyFill(bool focussed);

private:
    Ui::LcardDialog *ui;

    ILE140 *pModule;

    QString findLcard();

};

#endif // LCARDDIALOG_H
