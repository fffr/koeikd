#ifndef CANDIALOG_H
#define CANDIALOG_H

#include <QDialog>
#include <QtSerialPort/QSerialPortInfo>

#include "vs_can_api.h"
#include <QDebug>

namespace Ui {
class CanDialog;
}

class CanDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CanDialog(QWidget *parent = 0);
    ~CanDialog();

signals:
    void getHandle(VSCAN_HANDLE handle);
private slots:
    void getInfVSCAN();
    void updateVSCAN();
    void setVSCAN();
    void enableFilter();

    void lineToUpper();
    void lineEmptyFill(bool focussed);

private:
    Ui::CanDialog *ui;
    VSCAN_HANDLE handle;
    VSCAN_STATUS status;

    QStringList findVSCAN();
};

#endif // CANDIALOG_H
