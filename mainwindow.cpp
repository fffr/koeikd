    #include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    canHandle = 0;
    canHandleSecond = 0;

    pModule = NULL;

    qRegisterMetaType<VSCAN_MSG>("VSCAN_MSG"); // qt cross threading access
    connect(ui->exitMenu, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(&cameraView, SIGNAL(closed()), this, SLOT(hideCameraAction()));
    ui->mainLayout->setStretch(0, 5);
    ui->mainLayout->setStretch(1, 1);
    //-----------start some interface settings----------//

    connect(ui->mainCanAction, SIGNAL(triggered(bool)), this, SLOT(mainCanAction(bool)));
    connect(ui->secondCanAction, SIGNAL(triggered(bool)), this, SLOT(secondCanAction(bool)));
    connect(ui->actionLcard, SIGNAL(triggered(bool)), this, SLOT(lcardAction(bool)));

    ui->recieveTable->horizontalHeader()->setStretchLastSection(true);
    ui->recieveTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->recieveTable->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
 
    ui->recieveTable->verticalHeader()->setDefaultSectionSize(20);
    ui->recieveTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->recieveTable->setShowGrid(false);
    connect(ui->newRecieve, SIGNAL(triggered()), this, SLOT(clearRecieveTable()));

    ui->sendTable->horizontalHeader()->setStretchLastSection(true);
    ui->sendTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->sendTable->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);

    ui->sendTable->verticalHeader()->setDefaultSectionSize(20);
    ui->sendTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->sendTable->setShowGrid(false);
    connect(ui->newSend, SIGNAL(triggered()), this, SLOT(clearSendTable()));

    connect(ui->idEdit, SIGNAL(focussed(bool)), this, SLOT(lineEmptyFill(bool)));
    connect(ui->dlcEdit, SIGNAL(focussed(bool)), this, SLOT(lineEmptyFill(bool)));
    connect(ui->data1Edit, SIGNAL(focussed(bool)), this, SLOT(lineEmptyFill(bool)));
    connect(ui->data2Edit, SIGNAL(focussed(bool)), this, SLOT(lineEmptyFill(bool)));
    connect(ui->data3Edit, SIGNAL(focussed(bool)), this, SLOT(lineEmptyFill(bool)));
    connect(ui->data4Edit, SIGNAL(focussed(bool)), this, SLOT(lineEmptyFill(bool)));
    connect(ui->data5Edit, SIGNAL(focussed(bool)), this, SLOT(lineEmptyFill(bool)));
    connect(ui->data6Edit, SIGNAL(focussed(bool)), this, SLOT(lineEmptyFill(bool)));
    connect(ui->data7Edit, SIGNAL(focussed(bool)), this, SLOT(lineEmptyFill(bool)));
    connect(ui->data8Edit, SIGNAL(focussed(bool)), this, SLOT(lineEmptyFill(bool)));

    connect(ui->extCheckBox, SIGNAL(clicked()), this, SLOT(setCanFormat()));
    connect(ui->idEdit, SIGNAL(textChanged(QString)), this, SLOT(lineToUpper()));
    connect(ui->dlcEdit, SIGNAL(textChanged(QString)), this, SLOT(lineToUpper()));
    connect(ui->data1Edit, SIGNAL(textChanged(QString)), this, SLOT(lineToUpper()));
    connect(ui->data2Edit, SIGNAL(textChanged(QString)), this, SLOT(lineToUpper()));
    connect(ui->data3Edit, SIGNAL(textChanged(QString)), this, SLOT(lineToUpper()));
    connect(ui->data4Edit, SIGNAL(textChanged(QString)), this, SLOT(lineToUpper()));
    connect(ui->data5Edit, SIGNAL(textChanged(QString)), this, SLOT(lineToUpper()));
    connect(ui->data6Edit, SIGNAL(textChanged(QString)), this, SLOT(lineToUpper()));
    connect(ui->data7Edit, SIGNAL(textChanged(QString)), this, SLOT(lineToUpper()));
    connect(ui->data8Edit, SIGNAL(textChanged(QString)), this, SLOT(lineToUpper()));

    QRegExp num2Hex("[0-9A-Fa-f]{1,2}");
    QRegExpValidator *hex2Validator = new QRegExpValidator(num2Hex);
    ui->data1Edit->setValidator(hex2Validator);
    ui->data2Edit->setValidator(hex2Validator);
    ui->data3Edit->setValidator(hex2Validator);
    ui->data4Edit->setValidator(hex2Validator);
    ui->data5Edit->setValidator(hex2Validator);
    ui->data6Edit->setValidator(hex2Validator);
    ui->data7Edit->setValidator(hex2Validator);
    ui->data8Edit->setValidator(hex2Validator);

    QRegExp num3Hex("[0-9A-Fa-f]{1,3}");
    QRegExpValidator *hex3Validator = new QRegExpValidator(num3Hex);
    ui->idEdit->setValidator(hex3Validator);

    QRegExp num1Dec("[0-8]{1}");
    QRegExpValidator *dec1Validator = new QRegExpValidator(num1Dec);
    ui->dlcEdit->setValidator(dec1Validator);

    connect(ui->digitalOut1, SIGNAL(clicked()), this, SLOT(getDigitalOut()));
    connect(ui->digitalOut2, SIGNAL(clicked()), this, SLOT(getDigitalOut()));
    connect(ui->digitalOut3, SIGNAL(clicked()), this, SLOT(getDigitalOut()));
    connect(ui->digitalOut4, SIGNAL(clicked()), this, SLOT(getDigitalOut()));
    connect(ui->digitalOut5, SIGNAL(clicked()), this, SLOT(getDigitalOut()));
    connect(ui->digitalOut6, SIGNAL(clicked()), this, SLOT(getDigitalOut()));
    connect(ui->digitalOut7, SIGNAL(clicked()), this, SLOT(getDigitalOut()));
    connect(ui->digitalOut8, SIGNAL(clicked()), this, SLOT(getDigitalOut()));
    connect(ui->digitalOut9, SIGNAL(clicked()), this, SLOT(getDigitalOut()));

    //-----------end some interface settings----------//


    //-----------start candialog settings------------//
    canDialog = new CanDialog(this);
    connect(ui->setCanAction, SIGNAL(triggered()), canDialog, SLOT(open()));
    connect(canDialog, SIGNAL(getHandle(VSCAN_HANDLE)), this, SLOT(getHandle(VSCAN_HANDLE)));
    //-----------end candialog settings------------//

    //-------------reserve-------------------------//
    canDialogSecond = new CanDialog(this);
    connect(ui->setCanActionSecond, SIGNAL(triggered()), canDialogSecond, SLOT(open()));
    connect(canDialogSecond, SIGNAL(getHandle(VSCAN_HANDLE)), this, SLOT(getHandleSecond(VSCAN_HANDLE)));
    //-------------reserve-------------------------//

    //-----------start little connection-----------//
    connect(ui->closeCanAction, SIGNAL(triggered()), this, SLOT(closeCan()));
    connect(ui->addButton, SIGNAL(clicked()), this, SLOT(addCanMessage()));
    connect(ui->shotButton, SIGNAL(clicked()), this, SLOT(sendSingleCan()));
    connect(ui->copyButton, SIGNAL(clicked()), this, SLOT(copyCanMessage()));
    connect(ui->deleteButton, SIGNAL(clicked()), this, SLOT(deleteCanMessage()));
    connect(ui->sendAllButton, SIGNAL(clicked()), this, SLOT(sendAllCan()));

    connect(ui->saveSend, SIGNAL(triggered()), this, SLOT(saveSendCSV()));
    connect(ui->saveRecieve, SIGNAL(triggered()), this, SLOT(saveRecieveCSV()));
    connect(ui->openSend, SIGNAL(triggered()), this, SLOT(loadSendCSV()));

    connect(ui->showMessageBox, SIGNAL(currentTextChanged(QString)), this, SLOT(hideChannel()));

    connect(ui->setDigitalOut, SIGNAL(clicked()), this, SLOT(setDigitalOut()));
    connect(ui->closeLcardAction, SIGNAL(triggered()), this, SLOT(closeLcard()));
    //-----------end little connection-----------//

    //-------------reserve-------------------------//
    connect(ui->closeCanActionSecond, SIGNAL(triggered()), this, SLOT(closeCanSecond()));
    //-------------reserve-------------------------//

    //-----------start rxCanHandler settings-----------//
    rxWorker = new canRxWorker;
    canRxThread = new QThread;
    connect(canRxThread, SIGNAL(started()), rxWorker, SLOT(startRxCan()));
    //connect(canRxThread, SIGNAL(finished()), canRxThread, SLOT(deleteLater()));
    connect(rxWorker, SIGNAL(recieveMessage(VSCAN_MSG)), this, SLOT(recieveCanMessage(VSCAN_MSG)));
    rxWorker->moveToThread(canRxThread);
    //-----------end rxCanHandler settings-----------//

    //-------------reserve-------------------------//
    rxWorkerSecond = new canRxWorker;
    canRxThreadSecond = new QThread;
    connect(canRxThreadSecond, SIGNAL(started()), rxWorkerSecond, SLOT(startRxCan()));
    connect(rxWorkerSecond, SIGNAL(recieveMessage(VSCAN_MSG)), this, SLOT(recieveCanMessageSecond(VSCAN_MSG)));
    rxWorkerSecond->moveToThread(canRxThreadSecond);
    //-------------reserve-------------------------//


    //-----------start lcardDialog settings------------//
    lcardDialog = new LcardDialog(this);
    connect(ui->setLcardAction, SIGNAL(triggered()), lcardDialog, SLOT(open()));
    connect(lcardDialog, SIGNAL(accepted()), this, SLOT(getLcardModule()));
    //-----------end lcardDialog settings------------//

    //-----------start lcardHandler settings-----------//
    lcardWorker = new LcardWorker;
    lcardThread = new QThread;

    connect(lcardThread, SIGNAL(started()), lcardWorker, SLOT(startLcard()));
    connect(lcardWorker, SIGNAL(adcMeasure(float,float)), this, SLOT(adcMesure(float,float)));
    connect(lcardWorker, SIGNAL(digitalMeasure(int)), this, SLOT(digitalMeasure(int)));
    connect(lcardThread, SIGNAL(finished()), this, SLOT(adcClear()));
    lcardWorker->moveToThread(lcardThread);
    //-----------end lcardHandler settings-----------//

    connect(lcardDialog, SIGNAL(rejected()), this, SLOT(rejectLcard()));
    connect(canDialog, SIGNAL(rejected()), this, SLOT(rejectCanMain()));
    connect(canDialogSecond, SIGNAL(rejected()), this, SLOT(rejectCanSecond()));

    connect(ui->cameraAction, SIGNAL(triggered(bool)), this, SLOT(cameraViewerAction(bool)));
    connect(ui->showCameraAction, SIGNAL(triggered()), this, SLOT(showCameraAction()));
    connect(ui->hideCameraAction, SIGNAL(triggered()), this, SLOT(hideCameraAction()));

    //initialStart();
    this->getLcardModule();
    this->getHandle(VSCAN_Open(VSCAN_FIRST_FOUND, VSCAN_MODE_NORMAL));
    if (canHandle > 0) {
        VSCAN_Ioctl(canHandle, VSCAN_IOCTL_SET_SPEED, VSCAN_SPEED_1M);
    } else {
        QMessageBox::warning(this, "Vscan", "Не найден адаптер USB-CAN основного канала.");
    }
    this->getHandleSecond(VSCAN_Open(VSCAN_FIRST_FOUND, VSCAN_MODE_NORMAL));
    if (canHandleSecond > 0) {
        VSCAN_Ioctl(canHandleSecond, VSCAN_IOCTL_SET_SPEED, VSCAN_SPEED_1M);
    } else {
        QMessageBox::warning(this, "Vscan", "Не найден адаптер USB-CAN резервного канала.");
    }

    cameraView.setFixedSize(400, 300);
    cameraView.setWindowTitle("Камера");
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    cameraView.close();
    event->accept();
}

MainWindow::~MainWindow()
{
    this->closeCan();
    this->closeCanSecond();
    this->closeLcard();
    delete rxWorker;
    delete rxWorkerSecond;
    delete ui;
}

void MainWindow::showCameraAction()
{
    ui->hideCameraAction->setEnabled(true);
    cameraView.show();
    ui->showCameraAction->setEnabled(false);
    ui->cameraAction->setChecked(true);
}

void MainWindow::hideCameraAction()
{
    ui->hideCameraAction->setEnabled(false);
    cameraView.hide();
    ui->showCameraAction->setEnabled(true);
    ui->cameraAction->setChecked(false);
}

void MainWindow::cameraViewerAction(bool checked)
{
    if (checked) {
        cameraView.show();
        ui->showCameraAction->setEnabled(false);
        ui->hideCameraAction->setEnabled(true);
    }
    else {
        cameraView.hide();
        ui->showCameraAction->setEnabled(true);
        ui->hideCameraAction->setEnabled(false);
    }
}

//---------------can functions---------//
void MainWindow::mainCanAction(bool checked)
{
    if (checked)
        canDialog->open();
    else this->closeCan();
}

void MainWindow::secondCanAction(bool checked)
{
    if (checked)
        canDialogSecond->open();
    else this->closeCanSecond();
}

void MainWindow::lcardAction(bool checked)
{
    if (checked)
        lcardDialog->open();
    else this->closeLcard();
}

void MainWindow::rejectLcard()
{
    ui->actionLcard->setChecked(false);
}

void MainWindow::rejectCanMain()
{
    ui->mainCanAction->setChecked(false);
}

void MainWindow::rejectCanSecond()
{
    ui->secondCanAction->setChecked(false);
}

void MainWindow::setCanFormat()
{
    if (ui->extCheckBox->isChecked()) {
        QRegExp num8Hex("[0-9A-Fa-f]{1,7}");
        QRegExpValidator *hex8Validator = new QRegExpValidator(num8Hex);
        ui->idEdit->setValidator(hex8Validator);
        ui->idEdit->setText("0000000"); //validator, save text
    }
    else {
        QRegExp num3Hex("[0-9A-Fa-f]{1,3}");
        QRegExpValidator *hex3Validator = new QRegExpValidator(num3Hex);
        ui->idEdit->setValidator(hex3Validator);
        ui->idEdit->setText("000"); //validator, last 3
    }
}

void MainWindow::lineToUpper()
{
    ui->idEdit->setText(ui->idEdit->text().toUpper());
    ui->data1Edit->setText(ui->data1Edit->text().toUpper());
    ui->data2Edit->setText(ui->data2Edit->text().toUpper());
    ui->data3Edit->setText(ui->data3Edit->text().toUpper());
    ui->data4Edit->setText(ui->data4Edit->text().toUpper());
    ui->data5Edit->setText(ui->data5Edit->text().toUpper());
    ui->data6Edit->setText(ui->data6Edit->text().toUpper());
    ui->data7Edit->setText(ui->data7Edit->text().toUpper());
    ui->data8Edit->setText(ui->data8Edit->text().toUpper());
}

void MainWindow::lineEmptyFill(bool focussed)
{
    if (!focussed) {

        if (ui->idEdit->text().isEmpty()) {
            if (ui->extCheckBox->isChecked())
                ui->idEdit->setText("00000000");
            else ui->idEdit->setText("000");
        }

        if (ui->dlcEdit->text().isEmpty())
            ui->dlcEdit->setText("8");

        if (ui->data1Edit->text().isEmpty())
            ui->data1Edit->setText("00");

        if (ui->data2Edit->text().isEmpty())
            ui->data2Edit->setText("00");

        if (ui->data3Edit->text().isEmpty())
            ui->data3Edit->setText("00");

        if (ui->data4Edit->text().isEmpty())
            ui->data4Edit->setText("00");

        if (ui->data5Edit->text().isEmpty())
            ui->data5Edit->setText("00");

        if (ui->data6Edit->text().isEmpty())
            ui->data6Edit->setText("00");

        if (ui->data7Edit->text().isEmpty())
            ui->data7Edit->setText("00");

        if (ui->data8Edit->text().isEmpty())
            ui->data8Edit->setText("00");
    }
}

void MainWindow::getHandle(VSCAN_HANDLE handle)
{
    canHandle = handle;

    if (canHandle > 0) {
        ui->closeCanAction->setEnabled(true);
        ui->setCanAction->setEnabled(false);
        ui->recieveTable->setEnabled(true);
        ui->recieveLabel->setEnabled(true);
        ui->sendAllButton->setEnabled(true);
        ui->shotButton->setEnabled(true);
        ui->recieveTable->clearContents();

        ui->sendTable->setEnabled(true);
        ui->sendLabel->setEnabled(true);

        ui->mainCanAction->setChecked(true);

        rxWorker->setHandle(canHandle);
        rxWorker->setRunning(true);
        canRxThread->start();
    }
}

void MainWindow::closeCan()
{
    VSCAN_Close(canHandle);

    canHandle = 0;
    rxWorker->setRunning(false);
    canRxThread->quit();

    ui->closeCanAction->setEnabled(false);
    ui->setCanAction->setEnabled(true);
    ui->mainCanAction->setChecked(false);
    if (canHandleSecond <= 0) {
        ui->recieveTable->setEnabled(false);
        ui->recieveLabel->setEnabled(false);
        ui->sendAllButton->setEnabled(false);
        ui->shotButton->setEnabled(false);

        ui->sendTable->setEnabled(false);
        ui->sendLabel->setEnabled(false);

        this->clearRecieveTable();
    }
}

void MainWindow::recieveCanMessage(VSCAN_MSG msg)
{
    VSCAN_MSG tempMsg = msg;

    ui->recieveTable->setRowCount(ui->recieveTable->rowCount() + 1);

    QTableWidgetItem *time = new QTableWidgetItem;
    time->setText(QTime::currentTime().toString());
    time->setTextAlignment(Qt::AlignCenter);
    time->setFlags(time->flags() ^ Qt::ItemIsEditable);
    ui->recieveTable->setItem(ui->recieveTable->rowCount() - 1, 0, time);

    QTableWidgetItem *chItem = new QTableWidgetItem;
    chItem->setText("Основной");
    chItem->setTextAlignment(Qt::AlignCenter);
    chItem->setFlags(chItem->flags() ^ Qt::ItemIsEditable);
    ui->recieveTable->setItem(ui->recieveTable->rowCount() - 1, 1, chItem);

    QTableWidgetItem *idItem = new QTableWidgetItem;
    idItem->setText(QString::number(tempMsg.Id, 16).toUpper());
    idItem->setTextAlignment(Qt::AlignCenter);
    idItem->setFlags(idItem->flags() ^ Qt::ItemIsEditable);
    ui->recieveTable->setItem(ui->recieveTable->rowCount() - 1, 2, idItem);

    QTableWidgetItem *dlcItem = new QTableWidgetItem;
    dlcItem->setText(QString::number(tempMsg.Size, 16));
    dlcItem->setTextAlignment(Qt::AlignCenter);
    dlcItem->setFlags(dlcItem->flags() ^ Qt::ItemIsEditable);
    ui->recieveTable->setItem(ui->recieveTable->rowCount() - 1, 3, dlcItem);

    QTableWidgetItem *dataItem = new QTableWidgetItem;
    QString tempData;

    for (int i = 0; i != 8; ++i) {
        if (tempMsg.Data[i] == 0)
            tempData += "00";
        else tempData += QString::number(tempMsg.Data[i], 16).toUpper();

        tempData += " ";
    }

    tempData.simplified();

    dataItem->setText(tempData);
    dataItem->setTextAlignment(Qt::AlignCenter);
    dataItem->setFlags(dataItem->flags() ^ Qt::ItemIsEditable);
    ui->recieveTable->setItem(ui->recieveTable->rowCount() - 1, 4, dataItem);

    if (ui->showMessageBox->currentText() == "Резервный канал")
        ui->recieveTable->hideRow(ui->recieveTable->rowCount() - 1);

}

void MainWindow::addCanMessage()
{
    ui->sendTable->setRowCount(ui->sendTable->rowCount() +1);

    QTableWidgetItem *chItem = new QTableWidgetItem;
    QTableWidgetItem *idItem = new QTableWidgetItem;
    QTableWidgetItem *dlcItem = new QTableWidgetItem;
    QTableWidgetItem *dataItem = new QTableWidgetItem;

    if (ui->sendMessageBox->currentText() == "Основной канал")
        chItem->setText("Основной");
    else {
        chItem->setText("Резервный");
        chItem->setBackgroundColor(Qt::lightGray);
        idItem->setBackgroundColor(Qt::lightGray);
        dlcItem->setBackgroundColor(Qt::lightGray);
        dataItem->setBackgroundColor(Qt::lightGray);
    }
    chItem->setTextAlignment(Qt::AlignCenter);
    chItem->setFlags(chItem->flags() ^ Qt::ItemIsEditable);
    ui->sendTable->setItem(ui->sendTable->rowCount() - 1, 0, chItem);

    idItem->setText(ui->idEdit->text());
    idItem->setTextAlignment(Qt::AlignCenter);
    idItem->setFlags(idItem->flags() ^ Qt::ItemIsEditable);
    ui->sendTable->setItem(ui->sendTable->rowCount() - 1, 1, idItem);

    dlcItem->setText(ui->dlcEdit->text());
    dlcItem->setTextAlignment(Qt::AlignCenter);
    dlcItem->setFlags(dlcItem->flags() ^ Qt::ItemIsEditable);
    ui->sendTable->setItem(ui->sendTable->rowCount() - 1, 2, dlcItem);

    QString tempData = ui->data1Edit->text();
    tempData += " " + ui->data2Edit->text();
    tempData += " " + ui->data3Edit->text();
    tempData += " " + ui->data4Edit->text();
    tempData += " " + ui->data5Edit->text();
    tempData += " " + ui->data6Edit->text();
    tempData += " " + ui->data7Edit->text();
    tempData += " " + ui->data8Edit->text();
    dataItem->setText(tempData);
    dataItem->setTextAlignment(Qt::AlignCenter);
    dataItem->setFlags(dataItem->flags() ^ Qt::ItemIsEditable);
    ui->sendTable->setItem(ui->sendTable->rowCount() - 1, 3, dataItem);
}

void MainWindow::sendSingleCan()
{
    QList<QTableWidgetItem*> selectionRange = ui->sendTable->selectedItems();
    if (!selectionRange.isEmpty()) {
        int rowIndex;
        QList<QTableWidgetItem*> selectionRangeTrimmed;
        int countRows = 0;
        foreach (QTableWidgetItem* itemPointer, selectionRange) {
            if (countRows % (ui->sendTable->columnCount()) == 0)
                selectionRangeTrimmed.append(itemPointer);
            countRows++;
        }

        QListIterator<QTableWidgetItem*> selectionRangeIter(selectionRangeTrimmed);
        while (selectionRangeIter.hasNext()) {
            rowIndex = ((int)((QTableWidgetItem*)selectionRangeIter.next())->row());

            VSCAN_MSG msg;
            DWORD wr;

            QTableWidgetItem * item = ui->sendTable->item(rowIndex, 0);
            if (item) {
                if (ui->sendTable->item(rowIndex, 1)->text().length() > 3)
                    msg.Flags = VSCAN_FLAGS_EXTENDED;
                else msg.Flags = VSCAN_FLAGS_STANDARD;

                msg.Id = ui->sendTable->item(rowIndex, 1)->text().toInt(NULL, 16);
                msg.Size = ui->sendTable->item(rowIndex, 2)->text().toInt(NULL, 10);

                QStringList tempData = ui->sendTable->item(rowIndex, 3)->text().split(" ");
                for (int i = 0; i < tempData.size(); ++i)
                    msg.Data[i] = tempData.at(i).toInt(NULL, 16);

                if (ui->sendTable->item(rowIndex, 0)->text() == "Основной")
                    VSCAN_Write(canHandle, &msg, 1, &wr);
                else VSCAN_Write(canHandleSecond, &msg, 1, &wr);
            }
        }
    } else {
        VSCAN_MSG msg;
        DWORD wr;

        QTableWidgetItem *item = ui->sendTable->item(0, 0);
        if (item) {
            if (ui->sendTable->item(0, 1)->text().length() > 3)
                msg.Flags = VSCAN_FLAGS_EXTENDED;
            else msg.Flags = VSCAN_FLAGS_STANDARD;

            msg.Id = ui->sendTable->item(0, 1)->text().toInt(NULL, 16);
            msg.Size = ui->sendTable->item(0, 2)->text().toInt(NULL, 10);

            QStringList tempData = ui->sendTable->item(0, 3)->text().split(" ");
            for (int i = 0; i < tempData.size(); ++i)
                msg.Data[i] = tempData.at(i).toInt(NULL, 16);

            if (ui->sendTable->item(0, 0)->text() == "Основной")
                VSCAN_Write(canHandle, &msg, 1, &wr);
            else VSCAN_Write(canHandleSecond, &msg, 1, &wr);
        }
    }
}

void MainWindow::copyCanMessage()
{
    QList<QTableWidgetItem*> selectionRange = ui->sendTable->selectedItems();
    QListIterator<QTableWidgetItem*> selectionRangeIter(selectionRange);

    int row = 0;
    int column = 0;
    while (selectionRangeIter.hasNext()) {
        if (row == 0) {
             ui->sendTable->setRowCount(ui->sendTable->rowCount() + 1);
             row = 1;
        }

        if (row == 1) {
            QTableWidgetItem *chItem = new QTableWidgetItem;
            if (column == 0) {
                chItem->setText(((QString)((QTableWidgetItem*)selectionRangeIter.next())->text()));
                if (chItem->text() == "Резервный")
                    chItem->setBackgroundColor(Qt::lightGray);
                chItem->setTextAlignment(Qt::AlignCenter);
                chItem->setFlags(chItem->flags() ^ Qt::ItemIsEditable);
                ui->sendTable->setItem(ui->sendTable->rowCount() - 1, column, chItem);
                column++;
            }
            if (column == 1) {
                QTableWidgetItem *idItem = new QTableWidgetItem;
                idItem->setText(((QString)((QTableWidgetItem*)selectionRangeIter.next())->text()));
                if (chItem->text() == "Резервный")
                    idItem->setBackgroundColor(Qt::lightGray);
                idItem->setTextAlignment(Qt::AlignCenter);
                idItem->setFlags(idItem->flags() ^ Qt::ItemIsEditable);
                ui->sendTable->setItem(ui->sendTable->rowCount() - 1, column, idItem);
                column++;
            }

            if (column == 2) {
                QTableWidgetItem *dlcItem = new QTableWidgetItem;
                dlcItem->setText(((QString)((QTableWidgetItem*)selectionRangeIter.next())->text()));
                if (chItem->text() == "Резервный")
                    dlcItem->setBackgroundColor(Qt::lightGray);
                dlcItem->setTextAlignment(Qt::AlignCenter);
                dlcItem->setFlags(dlcItem->flags() ^ Qt::ItemIsEditable);
                ui->sendTable->setItem(ui->sendTable->rowCount() - 1, column, dlcItem);
                column++;
            }

            if (column == 3) {
                QTableWidgetItem *dataItem = new QTableWidgetItem;
                dataItem->setText(((QString)((QTableWidgetItem*)selectionRangeIter.next())->text()));
                if (chItem->text() == "Резервный")
                    dataItem->setBackgroundColor(Qt::lightGray);
                dataItem->setTextAlignment(Qt::AlignCenter);
                dataItem->setFlags(dataItem->flags() ^ Qt::ItemIsEditable);
                ui->sendTable->setItem(ui->sendTable->rowCount() - 1, column, dataItem);
                column = 0;
                row = 0;
            }
        }
    }
}

void MainWindow::deleteCanMessage()
{
    QList<QTableWidgetItem*> selectionRange = ui->sendTable->selectedItems();
    int rowIndex;
    QListIterator<QTableWidgetItem*> selectionRangeIter(selectionRange);

    while (selectionRangeIter.hasNext()) {
        rowIndex = ((int)((QTableWidgetItem*)selectionRangeIter.next())->row());
        ui->sendTable->removeRow(rowIndex);
    }
}

void MainWindow::sendAllCan()
{
    for (int i = 0; i <= ui->sendTable->rowCount() - 1; ++i) {
        VSCAN_MSG msg;
        DWORD wr;
        if (/*ui->extCheckBox->isChecked()*/ui->sendTable->item(i, 1)->text().length() > 3)
            msg.Flags = VSCAN_FLAGS_EXTENDED;
        else msg.Flags = VSCAN_FLAGS_STANDARD;

        msg.Id = ui->sendTable->item(i, 1)->text().toInt(NULL, 16);
        msg.Size = ui->sendTable->item(i, 2)->text().toInt(NULL, 10);

        QStringList tempData = ui->sendTable->item(i, 3)->text().split(" ");
        for (int j = 0; j < tempData.size(); ++j)
            msg.Data[j] = tempData.at(j).toInt(NULL, 16);

        if (ui->sendTable->item(i, 0)->text() == "Основной")
            VSCAN_Write(canHandle, &msg, 1, &wr);
        else VSCAN_Write(canHandleSecond, &msg, 1, &wr);
    }
}

void MainWindow::clearSendTable()
{
    ui->sendTable->setRowCount(0);
}

void MainWindow::clearRecieveTable()
{
    ui->recieveTable->setRowCount(0);
}

void MainWindow::saveSendCSV()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Сохранить", "", "CSV (*.csv);;All Files (*)");

    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QFile::WriteOnly | QFile::Truncate)) {
            QTextStream data(&file);
            QStringList strList;
            strList.clear();
            for (int i = 0; i < ui->sendTable->columnCount(); ++i)
                strList << "\" " + ui->sendTable->horizontalHeaderItem(i)->data(Qt::DisplayRole).toString() + "\" ";

            data << strList.join(";") + "\n";

            for (int i = 0; i < ui->sendTable->rowCount(); ++i) {
                strList.clear();
                for (int j = 0; j < ui->sendTable->columnCount(); ++j)
                    strList << "\" " + ui->sendTable->item(i, j)->text() + "\" ";

                data << strList.join(";") + "\n";
            }
            file.close();
        } else QMessageBox::warning(this, "Сохранить", "Невозможно записать файл.");
    } else QMessageBox::warning(this, "Сохранить", "Пустое имя файла.");
}

void MainWindow::saveRecieveCSV()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Сохранить", "", "CSV (*.csv);;All Files (*)");

    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QFile::WriteOnly | QFile::Truncate)) {
            QTextStream data(&file);
            QStringList strList;
            strList.clear();
            for (int i = 0; i < ui->recieveTable->columnCount(); ++i)
                strList << "\" " + ui->recieveTable->horizontalHeaderItem(i)->data(Qt::DisplayRole).toString() + "\" ";

            data << strList.join(";") + "\n";

            for (int i = 0; i < ui->recieveTable->rowCount(); ++i) {
                strList.clear();
                if (ui->showMessageBox->currentText() == "Основной канал") {
                    if (ui->recieveTable->item(i, 1)->text() == "Основной") {
                        for (int j = 0; j < ui->recieveTable->columnCount(); ++j)
                            strList << "\" " + ui->recieveTable->item(i, j)->text() + "\" ";
                        data << strList.join(";") + "\n";
                    }
                } else if (ui->showMessageBox->currentText() == "Резервный канал") {
                    if (ui->recieveTable->item(i, 1)->text() == "Резервный") {
                        for (int j = 0; j < ui->recieveTable->columnCount(); ++j)
                            strList << "\" " + ui->recieveTable->item(i, j)->text() + "\" ";
                        data << strList.join(";") + "\n";
                    }
                } else {
                    for (int j = 0; j < ui->recieveTable->columnCount(); ++j)
                        strList << "\" " + ui->recieveTable->item(i, j)->text() + "\" ";
                    data << strList.join(";") + "\n";
                }
            }
            file.close();
        } else QMessageBox::warning(this, "Сохранить", "Невозможно записать файл.");
    } else QMessageBox::warning(this, "Сохранить", "Пустое имя файла.");
}

void MainWindow::loadSendCSV()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Открыть", "", "CSV (*.csv);;All Files (*)");

    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QFile::ReadOnly)) {
            QTextStream load(&file);
            QString text = load.readAll();
            QStringList tempList = text.split("\n");
            QStringList parsedtempList;
            QString trimmedString;

            //maaaagic
            //shame on me

            foreach (QString temp, tempList) {
                temp.remove(";");
                temp.remove("\"");
                trimmedString = temp.trimmed();
                if (!trimmedString.isEmpty())
                    parsedtempList.append(trimmedString);
            }

            if (parsedtempList.startsWith("Канал  ID  DLC  Data")) {
                parsedtempList.removeFirst();

                this->clearSendTable();

                foreach (QString temp, parsedtempList) {
                    QStringList canMessageFormat = temp.split("  ");
                    ui->sendTable->setRowCount(ui->sendTable->rowCount() + 1);

                    QTableWidgetItem *chItem = new QTableWidgetItem;
                    chItem->setText(canMessageFormat.at(0));
                    chItem->setTextAlignment(Qt::AlignCenter);
                    chItem->setFlags(chItem->flags() ^ Qt::ItemIsEditable);
                    ui->sendTable->setItem(ui->sendTable->rowCount() - 1, 0, chItem);

                    QTableWidgetItem *idItem = new QTableWidgetItem;
                    idItem->setText(canMessageFormat.at(1));
                    idItem->setTextAlignment(Qt::AlignCenter);
                    idItem->setFlags(idItem->flags() ^ Qt::ItemIsEditable);
                    ui->sendTable->setItem(ui->sendTable->rowCount() - 1, 1, idItem);

                    QTableWidgetItem *dlcItem = new QTableWidgetItem;
                    dlcItem->setText(canMessageFormat.at(2));
                    dlcItem->setTextAlignment(Qt::AlignCenter);
                    dlcItem->setFlags(dlcItem->flags() ^ Qt::ItemIsEditable);
                    ui->sendTable->setItem(ui->sendTable->rowCount() - 1, 2, dlcItem);

                    QTableWidgetItem *dataItem = new QTableWidgetItem;
                    dataItem->setText(canMessageFormat.at(3));
                    dataItem->setTextAlignment(Qt::AlignCenter);
                    dataItem->setFlags(dataItem->flags() ^ Qt::ItemIsEditable);
                    ui->sendTable->setItem(ui->sendTable->rowCount() - 1, 3, dataItem);
                }
            } else QMessageBox::warning(this, "Открыть", "Неправильный формат сообщения.");
        } else QMessageBox::warning(this, "Открыть", "Невозможно открыть файл.");
    }
}

void MainWindow::hideChannel()
{
    for (int i = 0; i <= ui->recieveTable->rowCount() - 1; ++i) {
        if (ui->showMessageBox->currentText() == "Основной канал") {
            if (ui->recieveTable->item(i, 1)->text() == "Основной")
                ui->recieveTable->showRow(i);
            else ui->recieveTable->hideRow(i);
        } else if (ui->showMessageBox->currentText() == "Резервный канал") {
            if (ui->recieveTable->item(i, 1)->text() == "Резервный")
                ui->recieveTable->showRow(i);
            else ui->recieveTable->hideRow(i);
        } else ui->recieveTable->showRow(i);
    }
}

//-------end can functions----------------//

//-------start can reserve functions------//

void MainWindow::getHandleSecond(VSCAN_HANDLE handle)
{
    canHandleSecond = handle;

    if (canHandleSecond > 0) {
        ui->closeCanActionSecond->setEnabled(true);
        ui->setCanActionSecond->setEnabled(false);

        ui->sendTable->setEnabled(true);
        ui->sendLabel->setEnabled(true);
        ui->recieveTable->setEnabled(true);
        ui->recieveLabel->setEnabled(true);
        ui->sendAllButton->setEnabled(true);
        ui->shotButton->setEnabled(true);
        ui->recieveTable->clearContents();

        ui->secondCanAction->setChecked(true);

        rxWorkerSecond->setHandle(canHandleSecond);
        rxWorkerSecond->setRunning(true);
        canRxThreadSecond->start();
    }
}

void MainWindow::closeCanSecond()
{
    VSCAN_Close(canHandleSecond);

    canHandleSecond = 0;
    rxWorkerSecond->setRunning(false);
    canRxThreadSecond->quit();

    ui->closeCanActionSecond->setEnabled(false);
    ui->setCanActionSecond->setEnabled(true);

    ui->secondCanAction->setChecked(false);

    if (canHandle <= 0) {
        ui->recieveTable->setEnabled(false);
        ui->recieveLabel->setEnabled(false);
        ui->sendAllButton->setEnabled(false);
        ui->shotButton->setEnabled(false);
        ui->sendTable->setEnabled(false);
        ui->sendLabel->setEnabled(false);

        this->clearRecieveTable();
    }

}

void MainWindow::recieveCanMessageSecond(VSCAN_MSG msg)
{
    VSCAN_MSG tempMsg = msg;

    ui->recieveTable->setRowCount(ui->recieveTable->rowCount() + 1);

    QTableWidgetItem *time = new QTableWidgetItem;
    time->setText(QTime::currentTime().toString());
    time->setBackgroundColor(Qt::lightGray);
    time->setTextAlignment(Qt::AlignCenter);
    time->setFlags(time->flags() ^ Qt::ItemIsEditable);
    ui->recieveTable->setItem(ui->recieveTable->rowCount() - 1, 0, time);

    QTableWidgetItem *chItem = new QTableWidgetItem;
    chItem->setText("Резервный");
    chItem->setBackgroundColor(Qt::lightGray);
    chItem->setTextAlignment(Qt::AlignCenter);
    chItem->setFlags(chItem->flags() ^ Qt::ItemIsEditable);
    ui->recieveTable->setItem(ui->recieveTable->rowCount() - 1, 1, chItem);

    QTableWidgetItem *idItem = new QTableWidgetItem;
    idItem->setText(QString::number(tempMsg.Id, 16).toUpper());
    idItem->setBackgroundColor(Qt::lightGray);
    idItem->setTextAlignment(Qt::AlignCenter);
    idItem->setFlags(idItem->flags() ^ Qt::ItemIsEditable);
    ui->recieveTable->setItem(ui->recieveTable->rowCount() - 1, 2, idItem);

    QTableWidgetItem *dlcItem = new QTableWidgetItem;
    dlcItem->setText(QString::number(tempMsg.Size, 16));
    dlcItem->setBackgroundColor(Qt::lightGray);
    dlcItem->setTextAlignment(Qt::AlignCenter);
    dlcItem->setFlags(dlcItem->flags() ^ Qt::ItemIsEditable);
    ui->recieveTable->setItem(ui->recieveTable->rowCount() - 1, 3, dlcItem);

    QTableWidgetItem *dataItem = new QTableWidgetItem;
    QString tempData;

    for (int i = 0; i != 8; ++i) {
        if (tempMsg.Data[i] == 0)
            tempData += "00";
        else tempData += QString::number(tempMsg.Data[i], 16).toUpper();

        tempData += " ";
    }

    tempData.simplified();

    dataItem->setText(tempData);
    dataItem->setBackgroundColor(Qt::lightGray);
    dataItem->setTextAlignment(Qt::AlignCenter);
    dataItem->setFlags(dataItem->flags() ^ Qt::ItemIsEditable);
    ui->recieveTable->setItem(ui->recieveTable->rowCount() - 1, 4, dataItem);

    if (ui->showMessageBox->currentText() == "Основной канал")
        ui->recieveTable->hideRow(ui->recieveTable->rowCount() - 1);
}

//-------end can reserve functions------//

//-------start lcard functions----------------//

void MainWindow::getLcardModule()
{

    pModule = lcardDialog->getModulePointer();
    if (pModule->GET_MODULE_DESCRIPTION(&md)) {

        currentOffset = lcardDialog->getCurrentOffset();
        currentScale = lcardDialog->getCurrentScale();
        voltageOffset = lcardDialog->getVoltageOffset();
        voltageScale = lcardDialog->getVoltageScale();

        adcRound = 0;
        adcMeanCh1 = 0;
        adcMeanCh2 = 0;
        adcSample = 0.00125; // look at usermanual e140

        ui->setLcardAction->setEnabled(false);
        ui->closeLcardAction->setEnabled(true);
        ui->actionLcard->setChecked(true);

        pModule->GET_ADC_PARS(&adcPar);
        adcPar.ClkSource = INT_ADC_CLOCK_E140;
        adcPar.EnableClkOutput = ADC_CLOCK_TRANS_DISABLED_E140;
        adcPar.InputMode = NO_SYNC_E140;
        adcPar.ChannelsQuantity = 0x2;

        const WORD InputRangeIndex = ADC_INPUT_RANGE_10000mV_E140;
        for (WORD i = 0x0; i < adcPar.ChannelsQuantity; i++)
            adcPar.ControlTable[i] = (WORD) (i | (InputRangeIndex << 0x6)); // look at e140 usermanual

        adcPar.AdcRate = 100.0;
        adcPar.InterKadrDelay = 0.0;

        pModule->SET_ADC_PARS(&adcPar);

        ui->digitalIn1->setCheckable(true);
        ui->digitalIn2->setCheckable(true);
        ui->digitalIn3->setCheckable(true);
        ui->digitalIn4->setCheckable(true);
        ui->digitalIn5->setCheckable(true);
        ui->digitalIn6->setCheckable(true);
        ui->digitalIn7->setCheckable(true);
        ui->digitalIn8->setCheckable(true);
        ui->digitalIn9->setCheckable(true);
        ui->digitalIn10->setCheckable(true);

        ui->digitalOut1->setCheckable(true);
        ui->digitalOut2->setCheckable(true);
        ui->digitalOut3->setCheckable(true);
        ui->digitalOut4->setCheckable(true);
        ui->digitalOut5->setCheckable(true);
        ui->digitalOut6->setCheckable(true);
        ui->digitalOut7->setCheckable(true);
        ui->digitalOut8->setCheckable(true);
        ui->digitalOut9->setCheckable(true);

        ui->setDigitalOut->setEnabled(true);

        lcardWorker->setPointer(pModule);
        lcardWorker->setRunning(true);
        lcardThread->start();
    } else {
        QMessageBox::warning(this, "Lcard", "Не найден адаптер E14-140-M.");
    }
}

void MainWindow::closeLcard()
{
    lcardWorker->setRunning(false);
    lcardThread->quit();

    ui->closeLcardAction->setEnabled(false);
    ui->setLcardAction->setEnabled(true);
    ui->actionLcard->setChecked(false);

    ui->digitalIn1->setChecked(false);
    ui->digitalIn2->setChecked(false);
    ui->digitalIn3->setChecked(false);
    ui->digitalIn4->setChecked(false);
    ui->digitalIn5->setChecked(false);
    ui->digitalIn6->setChecked(false);
    ui->digitalIn7->setChecked(false);
    ui->digitalIn8->setChecked(false);
    ui->digitalIn9->setChecked(false);
    ui->digitalIn10->setChecked(false);

    ui->digitalIn1->setCheckable(false);
    ui->digitalIn2->setCheckable(false);
    ui->digitalIn3->setCheckable(false);
    ui->digitalIn4->setCheckable(false);
    ui->digitalIn5->setCheckable(false);
    ui->digitalIn6->setCheckable(false);
    ui->digitalIn7->setCheckable(false);
    ui->digitalIn8->setCheckable(false);
    ui->digitalIn9->setCheckable(false);
    ui->digitalIn10->setCheckable(false);

    ui->setDigitalOut->setEnabled(false);

    ui->digitalOut1->setChecked(false);
    ui->digitalOut2->setChecked(false);
    ui->digitalOut3->setChecked(false);
    ui->digitalOut4->setChecked(false);
    ui->digitalOut5->setChecked(false);
    ui->digitalOut6->setChecked(false);
    ui->digitalOut7->setChecked(false);
    ui->digitalOut8->setChecked(false);
    ui->digitalOut9->setChecked(false);

    ui->digitalOut1->setCheckable(false);
    ui->digitalOut2->setCheckable(false);
    ui->digitalOut3->setCheckable(false);
    ui->digitalOut4->setCheckable(false);
    ui->digitalOut5->setCheckable(false);
    ui->digitalOut6->setCheckable(false);
    ui->digitalOut7->setCheckable(false);
    ui->digitalOut8->setCheckable(false);
    ui->digitalOut9->setCheckable(false);
}

void MainWindow::adcMesure(float ch1, float ch2)
{
    if (adcRound == 999) {
        ui->currentEdit->setText(QString::number((((adcMeanCh1/1000)*adcSample)+currentOffset)*currentScale));
        ui->voltageEdit->setText(QString::number((((adcMeanCh2/1000)*adcSample)+voltageOffset)*voltageScale));
        adcRound = 0;
        adcMeanCh1 = 0;
        adcMeanCh2 = 0;
    } else {
        adcMeanCh1 += ch1;
        adcMeanCh2 += ch2;
        adcRound++;
    }
}

void MainWindow::adcClear()
{
    ui->currentEdit->setText("0");
    ui->voltageEdit->setText("0");

    ui->digitalInEdit->setText("0");
    ui->digitalOutEdit->setText("0");
}

void MainWindow::digitalMeasure(int ttlIn)
{
    QString binaryIn = QString::number(ttlIn, 2);
    if (binaryIn.size() < 10) {
        int sizeBin = binaryIn.size();
        for (int i = 0; i < (10 - sizeBin); ++i)
            binaryIn.insert(0, "0");
    }

    QString binaryInLast10;
    binaryInLast10.append(binaryIn.at(binaryIn.size() - 10));
    binaryInLast10.append(binaryIn.at(binaryIn.size() - 9));
    binaryInLast10.append(binaryIn.at(binaryIn.size() - 8));
    binaryInLast10.append(binaryIn.at(binaryIn.size() - 7));
    binaryInLast10.append(binaryIn.at(binaryIn.size() - 6));
    binaryInLast10.append(binaryIn.at(binaryIn.size() - 5));
    binaryInLast10.append(binaryIn.at(binaryIn.size() - 4));
    binaryInLast10.append(binaryIn.at(binaryIn.size() - 3));
    binaryInLast10.append(binaryIn.at(binaryIn.size() - 2));
    binaryInLast10.append(binaryIn.at(binaryIn.size() - 1));

    ui->digitalInEdit->setText(QString::number(binaryInLast10.toInt(NULL,2), 16).toUpper());

    if (binaryInLast10.at(0) == '1')
        ui->digitalIn1->setChecked(true);
    else ui->digitalIn1->setChecked(false);

    if (binaryInLast10.at(1) == '1')
        ui->digitalIn2->setChecked(true);
    else ui->digitalIn2->setChecked(false);

    if (binaryInLast10.at(2) == '1')
        ui->digitalIn3->setChecked(true);
    else ui->digitalIn3->setChecked(false);

    if (binaryInLast10.at(3) == '1')
        ui->digitalIn4->setChecked(true);
    else ui->digitalIn4->setChecked(false);

    if (binaryInLast10.at(4) == '1')
        ui->digitalIn5->setChecked(true);
    else ui->digitalIn5->setChecked(false);

    if (binaryInLast10.at(5) == '1')
        ui->digitalIn6->setChecked(true);
    else ui->digitalIn6->setChecked(false);

    if (binaryInLast10.at(6) == '1')
        ui->digitalIn7->setChecked(true);
    else ui->digitalIn7->setChecked(false);

    if (binaryInLast10.at(7) == '1')
        ui->digitalIn8->setChecked(true);
    else ui->digitalIn8->setChecked(false);

    if (binaryInLast10.at(8) == '1')
        ui->digitalIn9->setChecked(true);
    else ui->digitalIn9->setChecked(false);

    if (binaryInLast10.at(9) == '1')
        ui->digitalIn10->setChecked(true);
    else ui->digitalIn10->setChecked(false);
}

void MainWindow::setDigitalOut()
{
    WORD ttlOut = (WORD) ui->digitalOutEdit->text().toInt(NULL, 16);
    pModule->TTL_OUT(ttlOut);
}

void MainWindow::getDigitalOut()
{
    QString digitalOut;

    if (ui->digitalOut1->isChecked())
        digitalOut.insert(0, '1');
    else digitalOut.insert(0, '0');

    if (ui->digitalOut2->isChecked())
        digitalOut.insert(0, '1');
    else digitalOut.insert(0, '0');

    if (ui->digitalOut3->isChecked())
        digitalOut.insert(0, '1');
    else digitalOut.insert(0, '0');

    if (ui->digitalOut4->isChecked())
        digitalOut.insert(0, '1');
    else digitalOut.insert(0, '0');

    if (ui->digitalOut5->isChecked())
        digitalOut.insert(0, '1');
    else digitalOut.insert(0, '0');

    if (ui->digitalOut6->isChecked())
        digitalOut.insert(0, '1');
    else digitalOut.insert(0, '0');

    if (ui->digitalOut7->isChecked())
        digitalOut.insert(0, '1');
    else digitalOut.insert(0, '0');

    if (ui->digitalOut8->isChecked())
        digitalOut.insert(0, '1');
    else digitalOut.insert(0, '0');

    if (ui->digitalOut9->isChecked())
        digitalOut.insert(0, '1');
    else digitalOut.insert(0, '0');

    ui->digitalOutEdit->setText(QString::number(digitalOut.toInt(NULL, 2), 16).toUpper());
}

//-------end lcard functions----------------//


void MainWindow::initialStart()
{

}
