#include "candialog.h"
#include "ui_candialog.h"

CanDialog::CanDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CanDialog)
{
    handle = 0;
    ui->setupUi(this);

    ui->comBox->addItem(" ");
    ui->comBox->addItems(this->findVSCAN());

    connect(ui->comBox, SIGNAL(currentTextChanged(QString)), this, SLOT(getInfVSCAN()));
    connect(ui->refreshButton, SIGNAL(clicked()), this, SLOT(updateVSCAN()));
    connect(ui->okButton, SIGNAL(clicked()), this, SLOT(setVSCAN()));
    connect(ui->filterBox, SIGNAL(clicked()), this, SLOT(enableFilter()));

    //some interface set//
    QRegExp num2Hex("[0-9A-Fa-f]{1,2}");
    QRegExpValidator *hex2Validator = new QRegExpValidator(num2Hex);
    ui->btr0Edit->setValidator(hex2Validator);
    ui->btr1Edit->setValidator(hex2Validator);

    QRegExp num8Hex("[0-9A-Fa-f]{1,7}");
    QRegExpValidator *hex8Validator = new QRegExpValidator(num8Hex);
    ui->codeFilterLine->setValidator(hex8Validator);
    ui->maskFilterLine->setValidator(hex8Validator);

    connect(ui->btr0Edit, SIGNAL(textChanged(QString)), this, SLOT(lineToUpper()));
    connect(ui->btr1Edit, SIGNAL(textChanged(QString)), this, SLOT(lineToUpper()));
    connect(ui->codeFilterLine, SIGNAL(textChanged(QString)), this, SLOT(lineToUpper()));
    connect(ui->maskFilterLine, SIGNAL(textChanged(QString)), this, SLOT(lineToUpper()));

    connect(ui->btr0Edit, SIGNAL(focussed(bool)), this, SLOT(lineEmptyFill(bool)));
    connect(ui->btr1Edit, SIGNAL(focussed(bool)), this, SLOT(lineEmptyFill(bool)));
    connect(ui->codeFilterLine, SIGNAL(focussed(bool)), this, SLOT(lineEmptyFill(bool)));
    connect(ui->maskFilterLine, SIGNAL(focussed(bool)), this, SLOT(lineEmptyFill(bool)));
}

CanDialog::~CanDialog()
{
    VSCAN_Close(handle);
    delete ui;
}

QStringList CanDialog::findVSCAN()
{
    QStringList canList;
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        canList.append(info.portName());
    }

    return canList;
}

void CanDialog::getInfVSCAN()
{
    char *tmpCom;
    QByteArray tmpStrToByte;
    tmpStrToByte = ui->comBox->currentText().toLocal8Bit();
    tmpCom = tmpStrToByte.data();
    handle = VSCAN_Open(tmpCom, VSCAN_MODE_NORMAL);

    if (handle > 0) {
        VSCAN_HWPARAM hw;
        status = VSCAN_Ioctl(handle, VSCAN_IOCTL_GET_HWPARAM, &hw);
        if (hw.HwType == VSCAN_HWTYPE_UNKNOWN)
            ui->hwtypeLabel->setText("Тип устройства: Неизвестно");
        if (hw.HwType == VSCAN_HWTYPE_BUS)
            ui->hwtypeLabel->setText("Тип устройства: PCI-CAN");
        if (hw.HwType == VSCAN_HWTYPE_NET)
            ui->hwtypeLabel->setText("Тип устройства: NET-CAN");
        if (hw.HwType == VSCAN_HWTYPE_SERIAL)
            ui->hwtypeLabel->setText("Тип устройства: Serial-CAN");
        if (hw.HwType == VSCAN_HWTYPE_USB)
            ui->hwtypeLabel->setText("Тип устройства: USB-CAN");

        ui->snLabel->setText("Серийный номер: " + QString::number(hw.SerialNr));
        ui->swlabel->setText("Версия ПО: " + QString::number(hw.SwVersion >> 4) + "." + QString::number(hw.SwVersion & 0xF));
        ui->hwLabel->setText("Версия оборудования: " + QString::number(hw.HwVersion >> 4) + "." + QString::number(hw.HwVersion & 0xF));
        ui->okButton->setEnabled(true);
        VSCAN_Close(handle);
    } else {
        ui->hwtypeLabel->setText("Тип устройства: Неизвестно");
        ui->snLabel->setText("Серийный номер: Неизвестно");
        ui->swlabel->setText("Версия ПО: Неизвестно");
        ui->hwLabel->setText("Версия оборудования: Неизвестно");
        ui->okButton->setEnabled(false);
    }
}

void CanDialog::updateVSCAN()
{
    ui->comBox->clear();
    ui->comBox->addItem(" ");
    ui->comBox->addItems(this->findVSCAN());
}

void CanDialog::setVSCAN()
{
    char *tmpCom;
    QByteArray tmpStrToByte;
    tmpStrToByte = ui->comBox->currentText().toLatin1();
    tmpCom = tmpStrToByte.data();
    if (ui->normalRadioButton->isChecked())
        handle = VSCAN_Open(tmpCom, VSCAN_MODE_NORMAL);
    else if (ui->listenRadioButton->isChecked())
        handle = VSCAN_Open(tmpCom, VSCAN_MODE_LISTEN_ONLY);
    else if (ui->selfRadioButton->isChecked())
        handle = VSCAN_Open(tmpCom, VSCAN_MODE_SELF_RECEPTION);

    if (ui->hwTimeBox->isChecked())
        VSCAN_Ioctl(handle, VSCAN_IOCTL_SET_TIMESTAMP, VSCAN_TIMESTAMP_ON);
    else
        VSCAN_Ioctl(handle, VSCAN_IOCTL_SET_TIMESTAMP, VSCAN_TIMESTAMP_OFF);

    if (ui->debugBox->isChecked()) {
        VSCAN_Ioctl(handle, VSCAN_IOCTL_SET_DEBUG, VSCAN_DEBUG_HIGH);
        VSCAN_Ioctl(handle, VSCAN_IOCTL_SET_DEBUG_MODE, VSCAN_DEBUG_MODE_FILE);
    } else
        VSCAN_Ioctl(handle, VSCAN_IOCTL_SET_DEBUG, VSCAN_DEBUG_NONE);

    if (ui->speedBox->currentText() == "BTR регистры") {
        VSCAN_BTR btr;
        btr.Btr0 = ui->btr0Edit->text().toInt(NULL, 16);
        btr.Btr1 = ui->btr1Edit->text().toInt(NULL, 16);
        VSCAN_Ioctl(handle, VSCAN_IOCTL_SET_BTR, &btr);
    } else if (ui->speedBox->currentText() == "20 кбит")
        VSCAN_Ioctl(handle, VSCAN_IOCTL_SET_SPEED, VSCAN_SPEED_20K);
    else if (ui->speedBox->currentText() == "50 кбит")
        VSCAN_Ioctl(handle, VSCAN_IOCTL_SET_SPEED, VSCAN_SPEED_50K);
    else if (ui->speedBox->currentText() == "100 кбит")
        VSCAN_Ioctl(handle, VSCAN_IOCTL_SET_SPEED, VSCAN_SPEED_100K);
    else if (ui->speedBox->currentText() == "125 кбит")
        VSCAN_Ioctl(handle, VSCAN_IOCTL_SET_SPEED, VSCAN_SPEED_125K);
    else if (ui->speedBox->currentText() == "250 кбит")
        VSCAN_Ioctl(handle, VSCAN_IOCTL_SET_SPEED, VSCAN_SPEED_250K);
    else if (ui->speedBox->currentText() == "500 кбит")
        VSCAN_Ioctl(handle, VSCAN_IOCTL_SET_SPEED, VSCAN_SPEED_500K);
    else if (ui->speedBox->currentText() == "800 кбит")
        VSCAN_Ioctl(handle, VSCAN_IOCTL_SET_SPEED, VSCAN_SPEED_800K);
    else if (ui->speedBox->currentText() == "1 мбит")
        VSCAN_Ioctl(handle, VSCAN_IOCTL_SET_SPEED, VSCAN_SPEED_1M);

    if (ui->filterBox->isChecked()) {
        VSCAN_CODE_MASK codeMask;
        codeMask.Code = ui->codeFilterLine->text().toInt(NULL, 16);
        codeMask.Mask = ui->maskFilterLine->text().toInt(NULL, 16);
        VSCAN_Ioctl(handle, VSCAN_IOCTL_SET_ACC_CODE_MASK, &codeMask);
    } else {
        VSCAN_CODE_MASK codeMask;
        codeMask.Code = VSCAN_IOCTL_ACC_CODE_ALL;
        codeMask.Mask = VSCAN_IOCTL_ACC_MASK_ALL;
        VSCAN_Ioctl(handle, VSCAN_IOCTL_SET_ACC_CODE_MASK, &codeMask);
    }

    this->accept();
    emit getHandle(handle);
}

void CanDialog::enableFilter()
{
    if (ui->filterBox->isChecked()) {
        ui->codeFilterLine->setEnabled(true);
        ui->maskFilterLine->setEnabled(true);
        ui->code0xLabel->setEnabled(true);
        ui->codeLabel->setEnabled(true);
        ui->mask0xLabel->setEnabled(true);
        ui->maskLabel->setEnabled(true);
    } else {
        ui->codeFilterLine->setEnabled(false);
        ui->maskFilterLine->setEnabled(false);
        ui->code0xLabel->setEnabled(false);
        ui->codeLabel->setEnabled(false);
        ui->mask0xLabel->setEnabled(false);
        ui->maskLabel->setEnabled(false);
    }
}

void CanDialog::lineToUpper()
{
    ui->btr0Edit->setText(ui->btr0Edit->text().toUpper());
    ui->btr1Edit->setText(ui->btr1Edit->text().toUpper());
    ui->codeFilterLine->setText(ui->codeFilterLine->text().toUpper());
    ui->maskFilterLine->setText(ui->maskFilterLine->text().toUpper());
}

void CanDialog::lineEmptyFill(bool focussed)
{
    if (!focussed) {
        if (ui->btr0Edit->text().isEmpty())
            ui->btr0Edit->setText("00");

        if (ui->btr1Edit->text().isEmpty())
            ui->btr1Edit->setText("00");

        if (ui->codeFilterLine->text().isEmpty())
            ui->codeFilterLine->setText("0000000");

        if (ui->maskFilterLine->text().isEmpty())
            ui->maskFilterLine->setText("FFFFFFF");
    }
}
