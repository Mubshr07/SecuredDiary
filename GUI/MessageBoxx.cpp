#include "MessageBoxx.h"
#include "ui_MessageBoxx.h"

MessageBoxx::MessageBoxx(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MessageBoxx)
{
    ui->setupUi(this);
    this->setFixedSize(400, 260);

    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setWindowFlags(Qt::WindowStaysOnTopHint);
    //this->setWindowState(Qt::WindowFullScreen);
    //this->setAttribute(Qt::WA_DeleteOnClose, true); //so that it will be deleted when closed

    //this->activateWindow();
    this->raise();
    ui->yesPushButton->setFocus();

}

MessageBoxx::~MessageBoxx()
{
    delete ui;
}



void MessageBoxx::on_yesPushButton_clicked()
{
    emit txMessageBoxResult(local_parm, true);
    this->deleteLater();
    this->close();
}

void MessageBoxx::on_noPushButton_clicked()
{
    emit txMessageBoxResult(local_parm, false);
    this->deleteLater();
    this->close();
}




void MessageBoxx::rxSetMessageBoxParams(int parm, QString title, QString msg)
{
    local_parm = parm;
    ui->lblMessageBoxTitle->setText(title);
    ui->lblMessageBoxText->setText(msg);
}
void MessageBoxx::rxSetMessageBoxParams(int parm, QString title, QString msg, QString yesBtnText, bool yesVisible, QString noBtnText, bool noVisible)
{
    local_parm = parm;
    ui->lblMessageBoxTitle->setText(title);
    ui->lblMessageBoxText->setText(msg);

    ui->yesPushButton->setText(yesBtnText);
    ui->yesPushButton->setVisible(yesVisible);

    ui->noPushButton->setText(noBtnText);
    ui->noPushButton->setVisible(noVisible);
}
void MessageBoxx::rxSetMessageBoxParams(int parm, QString title, QString msg, QString yesBtnText, QString yesBtnStyle, bool yesVisible, QString noBtnText, QString noBtnStyle, bool noVisible)
{
    local_parm = parm;
    ui->lblMessageBoxTitle->setText(title);
    ui->lblMessageBoxText->setText(msg);

    ui->yesPushButton->setText(yesBtnText);
    ui->yesPushButton->setVisible(yesVisible);
    ui->yesPushButton->setStyleSheet(yesBtnStyle);

    ui->noPushButton->setText(noBtnText);
    ui->noPushButton->setVisible(noVisible);
    ui->noPushButton->setStyleSheet(noBtnStyle);
}


