#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setWindowFlags(Qt::WindowStaysOnTopHint);
    this->setWindowState(Qt::WindowFullScreen);

    //this->setCentralWidget(ui->plainTextEdit);

    ui->plainTextEdit->setEnabled(true);
    ui->plainTextEdit->setReadOnly(false);
    ui->plainTextEdit->setFontFamily("Ubuntu");
    ui->plainTextEdit->setUndoRedoEnabled(true);

    timerSingleShot = new QTimer(this);
    connect(timerSingleShot, SIGNAL(timeout()), this, SLOT(onTimerSingleShotElapsed()));
    timerSingleShot->setSingleShot(true);
    timerSingleShot->start(200);





}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::rxClosingChangePasswordWindow()
{
    qDebug()<<" ChangePassword is Closed ";
    changePassIsClosed = true;
}


void MainWindow::onTimerSingleShotElapsed()
{
    //qDebug()<<"Opening Log File : "<<GlobalVariables::fileLogPath;
    if(QFile::exists(GlobalVariables::fileLogPath)) {
        QFile abc(GlobalVariables::fileLogPath);
        if(abc.open(QIODevice::ReadOnly)) {
            QByteArray ba = QByteArray::fromBase64(abc.readAll());
            ui->plainTextEdit->setHtml(ba);
            ui->plainTextEdit->setFocus();

            abc.close();
        }
        ui->statusbar->showMessage(QString("loaded Previous file: "+GlobalVariables::fileLogPath));
    } else {
        ui->statusbar->showMessage(QString("Previous file Not Found, Path: "+GlobalVariables::fileLogPath));
    }
}

void MainWindow::on_SavePushButton_clicked()
{
    saveCurrentLogIntoFile();
}
void MainWindow::on_SaveClosePushButton_clicked()
{
    saveCurrentLogIntoFile();
    this->hide();
    this->close();
}
void MainWindow::on_loadPushButton_clicked()
{
    //qDebug()<<" FileDialog :: "<<GlobalVariables::mainFolderPath;
    GlobalVariables::fileLogPath = QFileDialog::getOpenFileName(this, "Select File", GlobalVariables::mainFolderPath);
    //qDebug()<<"Opening Report File : "<<GlobalVariables::fileLogPath;
    QFile abc(GlobalVariables::fileLogPath);
    if(abc.open(QIODevice::ReadOnly))
    {
        QByteArray ba = QByteArray::fromBase64(abc.readAll());
        ui->plainTextEdit->setHtml(ba);
        abc.close();
    }
    ui->statusbar->showMessage(QString("loaded file: "+GlobalVariables::fileLogPath));
}
void MainWindow::on_boldPushButton_clicked()
{
    localBoldBoolean = !localBoldBoolean;
    ui->boldPushButton->setText(QString(localBoldBoolean? "Bold On" : "Bold Off"));

    ui->plainTextEdit->setFontWeight((localBoldBoolean? QFont::ExtraBold : QFont::Normal));
ui->plainTextEdit->setFocus();
}
void MainWindow::on_boldTextIconPushButton_clicked()
{
    ui->plainTextEdit->setFontWeight(QFont::ExtraBold);
    ui->plainTextEdit->setFocus();
}
void MainWindow::on_onlyClosePushButton_clicked()
{
    this->hide();
    this->close();
}
void MainWindow::on_cmbColorSelector_currentIndexChanged(int index)
{
    switch (index) {
    case 0: ui->plainTextEdit->setTextColor(Qt::black); break;
    case 1: ui->plainTextEdit->setTextColor(Qt::red); break;
    case 2: ui->plainTextEdit->setTextColor(Qt::green); break;
    case 3: ui->plainTextEdit->setTextColor(Qt::yellow); break;
    case 4: ui->plainTextEdit->setTextColor(Qt::blue); break;
    default: ui->plainTextEdit->setTextColor(Qt::black);
    }
    ui->plainTextEdit->setFocus();
}
void MainWindow::on_cmbFontSize_currentIndexChanged(int index)
{
    switch (index) {
    case 0: { ui->plainTextEdit->setFontPointSize(8); break; }
    case 1: { ui->plainTextEdit->setFontPointSize(12); break; }
    case 2: { ui->plainTextEdit->setFontPointSize(14); break; }
    case 3: { ui->plainTextEdit->setFontPointSize(16); break; }
    case 4: { ui->plainTextEdit->setFontPointSize(18); break; }
    default: { ui->plainTextEdit->setFontPointSize(22); }
    }
    ui->plainTextEdit->setFocus();
    //qDebug()<<" Font set to ::"<<ui->cmbFontSize->currentText()<<" Size: "<<ui->plainTextEdit->fontPointSize();
}
void MainWindow::on_endOfLinePushButton_clicked()
{
    ui->plainTextEdit->append("--------------------------------------------\n");
ui->plainTextEdit->setFocus();
}
void MainWindow::on_changePasswordPushButton_clicked()
{
    if(changePassIsClosed){
        changePassIsClosed = false;
        changePass = new ChangePassword(this);
        connect(changePass, SIGNAL(txClosingChangePasswordWindow()), this, SLOT(rxClosingChangePasswordWindow()));
        changePass->setModal(true);
        changePass->show();
    }

}
void MainWindow::on_newFilePushButton_clicked()
{
    //qDebug()<<" FileDialog :: "<<GlobalVariables::mainFolderPath;
    //QString filePath = QFileDialog::getOpenFileName(this, "Select Report File", GlobalVariables::mainFolderPath);
    GlobalVariables::fileLogPath = QFileDialog::getSaveFileName(this, "New File Name", GlobalVariables::mainFolderPath);
    ui->statusbar->showMessage(QString("New File name set: "+ GlobalVariables::fileLogPath ));
    ui->plainTextEdit->clear();
    ui->plainTextEdit->setFocus();
}
void MainWindow::on_ZoomInPushButton_clicked()
{
    qDebug()<<" Zoom In";
    ui->plainTextEdit->zoomIn(2);
}
void MainWindow::on_ZoomOutPushButton_clicked()
{
    qDebug()<<" Zoom Out";
    ui->plainTextEdit->zoomOut(3);

}



void MainWindow::saveCurrentLogIntoFile()
{

    logFile = new QFile(GlobalVariables::fileLogPath);
    if(logFile->open(QIODevice::WriteOnly)) {
        QByteArray ba = ui->plainTextEdit->toHtml().toUtf8();
        logFile->write(ba.toBase64());
        logFile->close();
        ui->statusbar->showMessage(QString("Data saved in LogFile.: "+GlobalVariables::fileLogPath));
    } else {
        qDebug()<<" Error in opening report file :: "<<GlobalVariables::fileLogPath;
    }


    // ----------- Configurations --------------------
    QString str = GlobalVariables::fileLogPath+"\n";
    logFile = new QFile(GlobalVariables::fileConfig);
    if(logFile->open(QIODevice::WriteOnly)) {
        logFile->write(str.toUtf8());
        logFile->close();
    } else {
        qDebug()<<" Error in opening report file :: "<<GlobalVariables::fileConfig;
    }


}





