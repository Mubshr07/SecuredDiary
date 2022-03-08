#include "MainWindow.h"
#include "ui_mainwindow.h"

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

    timerSingleShot = new QTimer(this);
    connect(timerSingleShot, SIGNAL(timeout()), this, SLOT(onTimerSingleShotElapsed()));
    timerSingleShot->setSingleShot(true);
    timerSingleShot->start(200);




}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::onTimerSingleShotElapsed()
{
    qDebug()<<"Opening Report File : "<<GlobalVariables::fileLogPath;
    if(QFile::exists(GlobalVariables::fileLogPath)) {
        QFile abc(GlobalVariables::fileLogPath);
        if(abc.open(QIODevice::ReadOnly)) {
            QByteArray ba = QByteArray::fromBase64(abc.readAll());
            ui->plainTextEdit->setHtml(ba);

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
}
void MainWindow::on_loadPushButton_clicked()
{
    qDebug()<<" FileDialog :: "<<GlobalVariables::mainFolderPath;
    QString filePath = QFileDialog::getOpenFileName(this, "Select File", GlobalVariables::mainFolderPath);
    qDebug()<<"Opening Report File : "<<filePath;
    QFile abc(filePath);
    if(abc.open(QIODevice::ReadOnly))
    {
        QByteArray ba = QByteArray::fromBase64(abc.readAll());
        ui->plainTextEdit->setPlainText(ba);
        abc.close();
    }
    ui->statusbar->showMessage(QString("loaded file: "+filePath));
}
void MainWindow::on_boldPushButton_clicked()
{
    localBoldBoolean = !localBoldBoolean;
    ui->boldPushButton->setText(QString(localBoldBoolean? "Bold On" : "Bold Off"));

    ui->plainTextEdit->setFontWeight((localBoldBoolean? QFont::ExtraBold : QFont::Normal));
    //ui->plainTextEdit->setTextColor(new QColor())

}



void MainWindow::saveCurrentLogIntoFile()
{

    logFile = new QFile(GlobalVariables::fileLogPath);
    if(logFile->open(QIODevice::WriteOnly))
    {
        QByteArray ba = ui->plainTextEdit->toPlainText().toUtf8();
        logFile->write(ba.toBase64());

        logFile->close();
        ui->statusbar->showMessage("Data saved in LogFile.");
    }
    else
    {
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
void MainWindow::on_newFilePushButton_clicked()
{
    qDebug()<<" FileDialog :: "<<GlobalVariables::mainFolderPath;
    //QString filePath = QFileDialog::getOpenFileName(this, "Select Report File", GlobalVariables::mainFolderPath);
    GlobalVariables::fileLogPath = QFileDialog::getSaveFileName(this, "New File Name", GlobalVariables::mainFolderPath);
    ui->statusbar->showMessage(QString("New File name set: "+ GlobalVariables::fileLogPath ));
}




void MainWindow::on_cmbColorSelector_currentIndexChanged(int index)
{
    switch (index) {
    case 0: ui->plainTextEdit->setTextColor(Qt::black); break;
    case 1: ui->plainTextEdit->setTextColor(Qt::red); break;
    case 2: ui->plainTextEdit->setTextColor(Qt::green); break;
    case 3: ui->plainTextEdit->setTextColor(Qt::yellow); break;
    case 4: ui->plainTextEdit->setTextColor(Qt::black); break;
    default: ui->plainTextEdit->setTextColor(Qt::black);
    }
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
    qDebug()<<" Font set to ::"<<ui->cmbFontSize->currentText()<<" Size: "<<ui->plainTextEdit->fontPointSize();
}
