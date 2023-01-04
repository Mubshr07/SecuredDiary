#include "PasswordWindow.h"
#include "ui_PasswordWindow.h"

PasswordWindow::PasswordWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PasswordWindow)
{
    ui->setupUi(this);
    //this->setFixedSize(360 , 500);
    this->setBaseSize(460 , 230);

    this->setWindowFlags(Qt::WindowStaysOnTopHint);
    this->setWindowFlags(Qt::FramelessWindowHint);
    //this->setWindowState(Qt::WindowFullScreen);
    this->setAttribute(Qt::WA_DeleteOnClose, true); //so that it will be deleted when closed

    singleShotTimerOperation = 0;
    timerSingleShot = new QTimer(this);
    timerSingleShot->setSingleShot(true);
    connect(timerSingleShot, SIGNAL(timeout()), SLOT(rxTimerSingleShotElapsed()));
    timerSingleShot->start(300);

    //ui->lblPassWord->setValidator(new QIntValidator(0, 9999, this));
    ui->lblPassWord->setFocus();
}

PasswordWindow::~PasswordWindow()
{
    delete ui;
}



void PasswordWindow::on_passwordpushButtonEnter_clicked()
{
    QString passTxt = ui->lblPassWord->text();
    //qDebug()<<"\t KeyBoard Enter Button Pressed::"<<passTxt;
    QByteArray enteredPassword = QCryptographicHash::hash(passTxt.toUtf8(), QCryptographicHash::Sha512).toHex().toUpper();

    /*
    qDebug()<<" \n\n";
    qDebug()<<" 1. Enter Password Text ::"<<passTxt;
    qDebug()<<" 2. Enter Password Hash is ::"<<enteredPassword;
    qDebug()<<" 3. Compared Hash is ::"<<GlobalVariables::passwordAdminHash;
    qDebug()<<" \n\n";
    */

    if(enteredPassword == GlobalVariables::passwordAdminHash) {
        ui->lblPassWord->setStyleSheet(styleLabelGreen);
        singleShotTimerOperation = 1;
    } else {
        ui->lblPassWord->setStyleSheet(styleLabelRed);
        singleShotTimerOperation = 2;
    }
    timerSingleShot->start(300);
}


//Timer Slots
void PasswordWindow::rxTimerSingleShotElapsed()
{
    switch (singleShotTimerOperation) {
    case 0: { // first time to connect button click signals to slot
        break;
    }
    case 1: { // correct Password
        //emit txGenerateGUIModule(guiMainWindow);
        emit txGenerateGUIModule(guiInsertNew);
        emit txClosingPasswordWindow(false);
        this->hide();
        singleShotTimerOperation = 15179;
        timerSingleShot->start(300);
    }
    case 2: { // Wrong PassWord
        ui->lblPassWord->clear();
        ui->lblPassWord->setStyleSheet(styleLabelNormal);
        break;
    }
    case 3: { // Error in Parsing
        qDebug()<<" Error in Parsing.";
        break;
    }
    case 15179: { // Closing event
        this->close();
        break;
    }

    } // end switch statements
    ui->lblPassWord->setFocus();
}


void PasswordWindow::on_CancelWindowpushButton_clicked()
{
    emit txClosingPasswordWindow(true);
    this->hide();
    this->close();
}

void PasswordWindow::on_lblPassWord_returnPressed()
{
    on_passwordpushButtonEnter_clicked();
}
