#include "ChangePassword.h"
#include "ui_ChangePassword.h"

ChangePassword::ChangePassword(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChangePassword)
{
    ui->setupUi(this);

    this->setBaseSize(400 , 280);
    this->setWindowFlags(Qt::WindowStaysOnTopHint);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_DeleteOnClose, true); //so that it will be deleted when closed

    singleShotTimerOperation = 0;
    timerSingleShot = new QTimer(this);
    timerSingleShot->setSingleShot(true);
    connect(timerSingleShot, SIGNAL(timeout()), SLOT(rxTimerSingleShotElapsed()));
    timerSingleShot->start(200);


    ui->lblPassWordInput->setValidator(new QIntValidator(0, 9999, this));
    ui->lblPassWordInput->setFocus();
}
ChangePassword::~ChangePassword()
{
    delete ui;
}


void ChangePassword::on_passwordPushButtonClear_clicked()
{
    ui->lblPassWordInput->clear();
    ui->lblPassWordInput->setStyleSheet(styleLabelNormal);
    ui->lblPassWordInput->setFocus();
}
void ChangePassword::on_passwordPushButtonEnter_clicked()
{
    if(ui->lblPassWordInput->text().length() != 4 ){
        qDebug()<<" input string length is less than 4 ";
        if(messageWindowIsClose) {
            messageWindowIsClose = false;
            messageBoxWindow = new MessageBoxx(this);
            messageBoxWindow->rxSetMessageBoxParams(ErrorNumber, "Change Password", "You need to enter only number with length 4.", "OK", true, "", false);
            connect(messageBoxWindow, SIGNAL(txMessageBoxResult(int,bool)), this, SLOT(rxMessageBoxResult(int,bool)));
            messageBoxWindow->setGeometry(0, 0, this->width(), messageBoxWindow->height());
            messageBoxWindow->show();
        }
        ui->lblPassWordInput->setFocus();
        return;
    }

    enteredPasswordByteArray.clear();
    enteredPasswordByteArray =  QCryptographicHash::hash(ui->lblPassWordInput->text().toUtf8(), QCryptographicHash::Sha512).toHex().toUpper();

    qDebug()<<" text:"<<ui->lblPassWordInput->text()<<" hash:"<<enteredPasswordByteArray;

    changeGUIAccordingToStates();
}
void ChangePassword::on_passwordWindowCancel_clicked()
{
    this->deleteLater();
    this->close();
}



void ChangePassword::rxTimerSingleShotElapsed()
{
    switch (singleShotTimerOperation) {
    case 0: { // first time to connect button click signals to slot
        break;
    }
    case 1: { // correct Password change GUI according to user or admin
        ui->lblPassWordInput->setStyleSheet(styleLabelNormal);
        ui->lblPassWordInput->clear();
        if(userPasswordChangeRequest)
        {
            ui->lblWindowHeading->setText("Change User Password");
        }
        else
        {
            ui->lblWindowHeading->setText("Change Admin Password");
        }
        ui->lblPasswordHeading->setText("Enter New Password");

        break;
    }
    case 2: { // Change GUI to get again password to confirm
        ui->lblPassWordInput->setStyleSheet(styleLabelNormal);
        ui->lblPassWordInput->clear();
        ui->lblPasswordHeading->setText("Confirm New Password");
        break;
    }
    case 4: { // Error in Parsing
        qDebug()<<" Error in Parsing.";
        break;
    }

    case 10: { // Wrong PassWord
        ui->lblPassWordInput->setStyleSheet(styleLabelNormal);
        ui->lblPassWordInput->setText("");
        ui->lblPasswordHeading->setText("Enter Old Password");
        ui->lblWindowHeading->setText("Change Password");
        break;
    }
    case 11: { // Wrong PassWord
        ui->lblPassWordInput->setStyleSheet(styleLabelNormal);
        ui->lblPassWordInput->setText("");
        ui->lblPasswordHeading->setText("Enter New Password");
        ui->lblWindowHeading->setText("Change Password");
        break;
    }
    } // end switch statements

    ui->passwordPushButtonEnter->setDisabled(false);
    ui->lblPassWordInput->setFocus();
}

void ChangePassword::rxMessageBoxResult(int parm, bool yesNoButton)
{
    messageWindowIsClose = true;
}

void ChangePassword::changeGUIAccordingToStates()
{
    ui->passwordPushButtonEnter->setDisabled(true);
    switch (passwordState) {
    case PassVerifyOLD: {
        if(enteredPasswordByteArray == GlobalVariables::passwordAdminHash)
        {
            ui->lblPassWordInput->setStyleSheet(styleLabelGreen);
            singleShotTimerOperation = 1;
            ui->passwordWindowCancel->hide();
            userPasswordChangeRequest = false;
            passwordState = PassEnterNew;
            timerSingleShot->start(300);
        } else {
            ui->lblPassWordInput->setStyleSheet(styleLabelRed);
            ui->lblPassWordInput->clear();
            singleShotTimerOperation = 10;
            timerSingleShot->start(300);
        }
        break;
    }
    case PassEnterNew: {
        enteredNewPasswordByteArray = enteredPasswordByteArray;
        ui->lblPassWordInput->setStyleSheet(styleLabelGreen);
        singleShotTimerOperation = 2;
        passwordState = PassConfirmNew;
        timerSingleShot->start(300);

        break;
    }
    case PassConfirmNew: {
        if(enteredPasswordByteArray == enteredNewPasswordByteArray)
        {
            GlobalVariables::passwordAdminHash = enteredNewPasswordByteArray;
            saveNewHashesAndCloseThisWindow();
        }
        else
        {
            //passwordState = PassVerifyOLD;
            if(messageWindowIsClose) {
                messageWindowIsClose = false;
                messageBoxWindow = new MessageBoxx(this);
                messageBoxWindow->rxSetMessageBoxParams(ErrorNumber, "Change Password Failure", "Confirm new password did not match.", "OK", "background-color:red;color:white;", true, "", "", false);
                connect(messageBoxWindow, SIGNAL(txMessageBoxResult(int,bool)), this, SLOT(rxMessageBoxResult(int,bool)));
                messageBoxWindow->setGeometry(0, 0, this->width(), messageBoxWindow->height());
                messageBoxWindow->show();
                //emit txShowMessageBox(guiChangePasswordWindow, ErrorNumber, "Change Password Failure", "Confirm new password did not match.", "OK", "background-color:red;color:white;", true, "", "", false);
            }
            passwordState = PassEnterNew;
            ui->lblPassWordInput->setStyleSheet(styleLabelRed);
            ui->lblPassWordInput->clear();
            ui->passwordWindowCancel->show();
            singleShotTimerOperation = 11;
            timerSingleShot->start(300);
        }
        break;
    }
    default:
        break;
    } // end of Switch statements

    ui->lblPassWordInput->setFocus();

}
void ChangePassword::saveNewHashesAndCloseThisWindow()
{

    QString str = GlobalVariables::passwordAdminHash+"\n";

    QFile fileSettings(GlobalVariables::filePathPasswords);
    if(fileSettings.open(QIODevice::WriteOnly))
    {
        //qDebug()<<" Writing in PassWord Hashes:"<<str;
        fileSettings.write(str.toUtf8());
        fileSettings.close();
    }
    else
    {
        qDebug()<<" PassWord File did not opened";
    }

    if(messageWindowIsClose){
        messageWindowIsClose = false;
        messageBoxWindow = new MessageBoxx(this);
        connect(messageBoxWindow, SIGNAL(txMessageBoxResult(int,bool)), this, SLOT(rxMessageBoxResult(int,bool)));
        messageBoxWindow->rxSetMessageBoxParams(ErrorNumber, "Change Password Success", "Password change successfully.", "OK", true, "", false);
        messageBoxWindow->setGeometry(0, 0, this->width(), messageBoxWindow->height());
        messageBoxWindow->show();
    }

    //emit txShowMessageBox(guiChangePasswordWindow, ErrorNumber, "Change Password Success", "Password change successfully.", "OK", true, "", false);
    emit txClosingChangePasswordWindow();
    this->deleteLater();
    //this->close();
}




void ChangePassword::on_lblPassWordInput_returnPressed()
{
    on_passwordPushButtonEnter_clicked();
}
