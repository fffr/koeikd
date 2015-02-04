#include "lcarddialog.h"
#include "ui_lcarddialog.h"

LcardDialog::LcardDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LcardDialog)
{
    ui->setupUi(this);
    pModule = static_cast<ILE140*>(CreateLInstance("e140"));
    if (!this->findLcard().isEmpty()) {
        ui->virtualSlotBox->addItem(" ");
        ui->virtualSlotBox->addItem(this->findLcard());
    }

    connect(ui->virtualSlotBox, SIGNAL(currentTextChanged(QString)), this, SLOT(getInfLcard()));
    connect(ui->okButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(ui->refreshButton, SIGNAL(clicked()), this, SLOT(refreshLcardList()));

    QRegExp numDec("\\d*\\.\\d+");
    QRegExpValidator *decValidator = new QRegExpValidator(numDec);

    ui->currentOffsetEdit->setValidator(decValidator);
    ui->currentScaleEdit->setValidator(decValidator);
    ui->voltageOffsetEdit->setValidator(decValidator);
    ui->currentScaleEdit->setValidator(decValidator);

    connect(ui->currentOffsetEdit, SIGNAL(focussed(bool)), this, SLOT(lineEmptyFill(bool)));
    connect(ui->voltageOffsetEdit, SIGNAL(focussed(bool)), this, SLOT(lineEmptyFill(bool)));
    connect(ui->currentScaleEdit, SIGNAL(focussed(bool)), this, SLOT(lineEmptyFill(bool)));
    connect(ui->voltageScaleEdit, SIGNAL(focussed(bool)), this, SLOT(lineEmptyFill(bool)));

}

LcardDialog::~LcardDialog()
{
    if (pModule)
        pModule->ReleaseLInstance();
    delete ui;
}

void LcardDialog::lineEmptyFill(bool focussed)
{
    if (!focussed) {
        if (ui->currentOffsetEdit->text().isEmpty())
            ui->currentOffsetEdit->setText("0");

        if (ui->voltageOffsetEdit->text().isEmpty())
            ui->voltageOffsetEdit->setText("0");

        if (ui->currentScaleEdit->text().isEmpty())
            ui->currentScaleEdit->setText("1");

        if (ui->voltageScaleEdit->text().isEmpty())
            ui->voltageScaleEdit->setText("1");
    }
}

QString LcardDialog::findLcard()
{
    QString lcardString;
    if (pModule) {
        for (int i = 0x0; i < MAX_VIRTUAL_SLOTS_QUANTITY_LUSBAPI; i++) {
            if (pModule->OpenLDevice(i)) {
                lcardString.append(QString::number(i));
                break;
            }
        }
    }
    return lcardString;
}

float LcardDialog::getCurrentOffset()
{
    return ui->currentOffsetEdit->text().toFloat(NULL);
}

float LcardDialog::getCurrentScale()
{
    return ui->currentScaleEdit->text().toFloat(NULL);
}

float LcardDialog::getVoltageOffset()
{
    return ui->voltageOffsetEdit->text().toFloat(NULL);
}

float LcardDialog::getVoltageScale()
{
    return ui->voltageScaleEdit->text().toFloat(NULL);
}

ILE140* LcardDialog::getModulePointer()
{
    return pModule;
}

void LcardDialog::getInfLcard()
{
    if (ui->virtualSlotBox->currentText() == "0") {
        ui->okButton->setEnabled(true);
        MODULE_DESCRIPTION_E140 moduleDescription;
        pModule->GET_MODULE_DESCRIPTION(&moduleDescription);
        QString temp = QByteArray((const char*) moduleDescription.Module.DeviceName);
        ui->typeLabel->setText("Тип устройства: " + temp);
        temp = QString::number(moduleDescription.Module.Modification, 16).toUpper();
        ui->modLabel->setText("Модификация: " + temp);
        temp = QString::number(moduleDescription.Module.Revision, 16).toUpper();
        ui->revLabel->setText("Ревизия: " + temp);
        temp = QByteArray((const char*) moduleDescription.Module.SerialNumber);
        ui->snLabel->setText("Серийный номер: " + temp);
    } else {
        ui->okButton->setEnabled(false);
        ui->typeLabel->setText("Тип устройства: Неизвестно");
        ui->modLabel->setText("Модификация: Неизвестно");
        ui->revLabel->setText("Ревизия: Неизвестно");
        ui->snLabel->setText("Серийный номер: Неизвестно");
    }
}

void LcardDialog::refreshLcardList()
{
    ui->virtualSlotBox->clear();
    if (!this->findLcard().isEmpty())
        ui->virtualSlotBox->addItem(this->findLcard());
}
