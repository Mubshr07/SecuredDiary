#include "InsertNew.h"
#include "ui_InsertNew.h"

InsertNew::InsertNew(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::InsertNew)
{
    ui->setupUi(this);


    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setWindowFlags(Qt::WindowStaysOnTopHint);
    this->setWindowState(Qt::WindowFullScreen);


    ui->plainTextEdit->setEnabled(true);
    ui->plainTextEdit->setReadOnly(false);
    ui->plainTextEdit->setFontFamily("Ubuntu");
    ui->plainTextEdit->setUndoRedoEnabled(true);

    timerSingleShot = new QTimer(this);
    connect(timerSingleShot, SIGNAL(timeout()), this, SLOT(onTimerSingleShotElapsed()));
    timerSingleShot->setSingleShot(true);
    timerSingleShot->start(200);


    ui->lblTodayDateDay->setText(QString(QDate::currentDate().toString("dd:MMMM:yy")+" "+QDate::currentDate().toString("dddd")));
    ui->lblLoadedDate->setText(QString(QDate::currentDate().toString("dd:MMMM:yy")+" "+QDate::currentDate().toString("dddd")));

    dateVector.clear();
    logStrVector.clear();

}

InsertNew::~InsertNew()
{
    delete ui;
}


void InsertNew::onTimerSingleShotElapsed()
{
    //qDebug()<<"Opening Log File : "<<GlobalVariables::fileLogPath;
    if(QFile::exists(GlobalVariables::fileLogPath)) {
        QFile abc(GlobalVariables::fileLogPath);
        if(abc.open(QIODevice::ReadOnly)) {

            QByteArray ba = QByteArray::fromBase64(abc.readAll());

            /*
            qDebug()<<" 8*********************** ";
            qDebug()<<" Whole FIle: "<<abc.readAll();
            qDebug()<<"\n 8*********************** ";
            qDebug()<<" Whole 2 FIle Base64: "<<ba;
            qDebug()<<"\n 8*********************** ";
            qDebug()<<" seperatorLogBytes"<<seperatorLogBytes<<" Length: "<<seperatorLogBytes.length();
            qDebug()<<"\n 8*********************** ";
            qDebug()<<" seperatorDateBytes"<<seperatorDateBytes<<" Length: "<<seperatorDateBytes.length();
            qDebug()<<"\n 8*********************** ";
            */

            wholeFile = ba;
            int endIndex = 0;
            endIndex = ba.indexOf(seperatorLogBytes);
            //qDebug()<<"\n\n indx:: "<<endIndex;
            if(endIndex < 0) {
                // not found
                ui->plainTextEdit->setText("Wrong File, This file is not in correct format: "+GlobalVariables::fileLogPath);
                ui->plainTextEdit->setFocus();
                dateVector.clear();
                logStrVector.clear();
                return;
            }

            QString singleDateStr = "";
            QString singleLogBytes = "";
            QString logBytes = ""; // ba.left(endIndex);

            while(true){
                endIndex = ba.indexOf(seperatorLogBytes);
                if(endIndex < 0 ) break;

                logBytes = ba.left(endIndex);
                ba.remove(0, (endIndex+seperatorLogBytes.length()));
                //qDebug()<<" LogBytes :: "<<logBytes<<" Index ::"<<endIndex<<"\n\n";
                endIndex = logBytes.indexOf(seperatorDateBytes);
                singleDateStr = logBytes.left(endIndex);
                //qDebug()<<" Date Bytes :: "<<singleDateStr<<" Index ::"<<endIndex<<"\n\n";
                logBytes.remove(0, (endIndex+seperatorDateBytes.length()));

                /*
                qDebug()<<" removed date then remain :: "<<logBytes<<" Index ::"<<endIndex<<"\n\n";
                qDebug()<<"**************************************";
                qDebug()<<" Log Bytes :: "<<logBytes<<"\n\n logSeperator ::"<<seperatorLogBytes<<"\n\n";
                qDebug()<<" Log Length :: "<<logBytes.length()<<" logSeperatorLength ::"<<seperatorLogBytes.length()<<"\n\n";
                qDebug()<<" -*-*-**-*-*-*-*-*-*-*-*-*-*-*-*-*-*--*-* \n\n\n";
                */


                singleLogBytes = logBytes;
                dateVector.append(singleDateStr);
                logStrVector.append(singleLogBytes);
            }

            //qDebug()<<"\n\n\n *********** \n Length of Vector: "<<dateVector.length();


            logCounter = dateVector.length()-1;

            ui->lblLoadedDate->setText(QString(dateVector.at(logCounter)));
            ui->plainTextEdit->setHtml(logStrVector.at(logCounter));
            ui->plainTextEdit->setFocus();
            if(ui->lblTodayDateDay->text() == ui->lblLoadedDate->text()){
                ui->NewLogPushButton->hide();
            }

            ui->lblCurrentIndexWithTotalRecords->setText(QString::number(logCounter)+"/"+QString::number(logStrVector.length()-1));
            abc.close();
        }
        ui->statusbar->showMessage(QString("loaded Previous file: "+GlobalVariables::fileLogPath));
    } else {
        ui->statusbar->showMessage(QString("Previous file Not Found, Path: "+GlobalVariables::fileLogPath));
    }
}

void InsertNew::on_SavePushButton_clicked()
{
    saveCurrentLogIntoFile();
}
void InsertNew::on_SaveClosePushButton_clicked()
{
    saveCurrentLogIntoFile();
    this->close();
    emit txCloseInsertNew();

}
void InsertNew::on_loadPushButton_clicked()
{
    //qDebug()<<" FileDialog :: "<<GlobalVariables::mainFolderPath;
    QString file = QFileDialog::getOpenFileName(this, "Select File", GlobalVariables::mainFolderPath);
    //qDebug()<<"Opening Report File : "<<GlobalVariables::fileLogPath;
    QByteArray ba;
    ba.clear();
    QFile abc(file);
    if(abc.open(QIODevice::ReadOnly)) {
        ba = QByteArray::fromBase64(abc.readAll());
        ba.replace(seperatorDateBytes, "<br>");
        ba.replace(seperatorLogBytes, "<br> ************************* <br><br>");
        qDebug()<<" \n\n\n Loaded File Text:: \n"<<QString(ba)<<"\n\n";
        ui->plainTextEdit->setHtml(ba);
        abc.close();
    }
    ui->statusbar->showMessage(QString("loaded file: "+file));
    ui->frameNewLogControl->setHidden(true);
}
void InsertNew::on_boldPushButton_clicked()
{
    localBoldBoolean = !localBoldBoolean;
    ui->boldPushButton->setText(QString(localBoldBoolean? "Bold On" : "Bold Off"));

    ui->plainTextEdit->setFontWeight((localBoldBoolean? QFont::ExtraBold : QFont::Normal));
    ui->plainTextEdit->setFocus();
}
void InsertNew::on_boldTextIconPushButton_clicked()
{
    ui->plainTextEdit->setFontWeight(QFont::ExtraBold);
    ui->plainTextEdit->setFocus();
}
void InsertNew::on_onlyClosePushButton_clicked()
{
    this->hide();
    this->close();
}
void InsertNew::on_cmbColorSelector_currentIndexChanged(int index)
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
void InsertNew::on_cmbFontSize_currentIndexChanged(int index)
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
void InsertNew::on_endOfLinePushButton_clicked()
{
    ui->plainTextEdit->append("--------------------------------------------\n");
    ui->plainTextEdit->setFocus();
}
void InsertNew::on_changePasswordPushButton_clicked()
{
    if(changePassIsClosed){
        changePassIsClosed = false;
        changePass = new ChangePassword(this);
        connect(changePass, SIGNAL(txClosingChangePasswordWindow()), this, SLOT(rxClosingChangePasswordWindow()));
        changePass->setModal(true);
        changePass->show();
    }

}
void InsertNew::on_newFilePushButton_clicked()
{
    //qDebug()<<" FileDialog :: "<<GlobalVariables::mainFolderPath;
    //QString filePath = QFileDialog::getOpenFileName(this, "Select Report File", GlobalVariables::mainFolderPath);
    GlobalVariables::fileLogPath = QFileDialog::getSaveFileName(this, "New File Name", GlobalVariables::mainFolderPath);
    ui->statusbar->showMessage(QString("New File name set: "+ GlobalVariables::fileLogPath ));

    dateVector.clear();
    logStrVector.clear();
    ui->NewLogPushButton->hide();
    ui->deleteThisLogPushButton->hide();
    ui->lblLoadedDate->setText(ui->lblTodayDateDay->text());
    ui->plainTextEdit->clear();
    ui->plainTextEdit->setFocus();
}
void InsertNew::on_ZoomInPushButton_clicked()
{
    //qDebug()<<" Zoom In";
    logCounter++;
    if(logCounter >= (dateVector.length())) {
        logCounter = dateVector.length()-1;
        ui->statusbar->showMessage("End of Log File.. ");
    }

    ui->lblLoadedDate->setText(QString(dateVector.at(logCounter)));
    ui->plainTextEdit->setHtml(logStrVector.at(logCounter));
    ui->plainTextEdit->setFocus();

    ui->lblCurrentIndexWithTotalRecords->setText(QString::number(logCounter)+"/"+QString::number(logStrVector.length()-1));
}
void InsertNew::on_ZoomOutPushButton_clicked()
{
    //qDebug()<<" Zoom Out";
    logCounter--;
    if(logCounter < 0) {
        logCounter = 0;
        ui->statusbar->showMessage("Start of Log File.. ");
    }

    ui->lblLoadedDate->setText(QString(dateVector.at(logCounter)));
    ui->plainTextEdit->setHtml(logStrVector.at(logCounter));
    ui->plainTextEdit->setFocus();
    ui->lblCurrentIndexWithTotalRecords->setText(QString::number(logCounter)+"/"+QString::number(logStrVector.length()-1));

}
void InsertNew::on_NewLogPushButton_clicked()
{
    if(ui->lblTodayDateDay->text() != ui->lblLoadedDate->text()){
        ui->lblLoadedDate->setText(ui->lblTodayDateDay->text());
        ui->plainTextEdit->clear();
        ui->plainTextEdit->append("\t\t");
        ui->NewLogPushButton->hide();
        ui->deleteThisLogPushButton->hide();
        ui->plainTextEdit->setFocus();
    }
}
void InsertNew::on_deleteThisLogPushButton_clicked()
{
    dateVector.remove(logCounter);
    logStrVector.remove(logCounter);
    logFile = new QFile(GlobalVariables::fileLogPath);
    QByteArray ba = "";
    if(logFile->open(QIODevice::WriteOnly)) {
        ba.clear();
        for(int i=0; i<dateVector.length(); i++) {
            ba += dateVector.at(i);
            ba += seperatorDateBytes;
            ba += logStrVector.at(i);
            ba += seperatorLogBytes;
        }
        logFile->write(ba.toBase64());
        logFile->close();
        ui->statusbar->showMessage(QString("Data saved in LogFile.: "+GlobalVariables::fileLogPath));
    } else {
        qDebug()<<" Error in opening report file :: "<<GlobalVariables::fileLogPath;
    }
    ui->lblLoadedDate->setText("");
    dateVector.clear();
    logStrVector.clear();

    timerSingleShot->start(200);

}




void InsertNew::saveCurrentLogIntoFile()
{
    if(dateVector.contains(ui->lblLoadedDate->text())){
        int idx = dateVector.indexOf(ui->lblLoadedDate->text());
        logStrVector.replace(idx, ui->plainTextEdit->toHtml());
    } else {
        dateVector.append(ui->lblLoadedDate->text());
        logStrVector.append(ui->plainTextEdit->toHtml());
    }


    logFile = new QFile(GlobalVariables::fileLogPath);
    if(logFile->open(QIODevice::WriteOnly)) {
        QByteArray ba = "\n\n";
        //qDebug()<<" Parsing Today Date for New Log is : "<<ba;
            ba.clear();


        for(int i=0; i<dateVector.length(); i++) {
            ba += dateVector.at(i);
            ba += seperatorDateBytes;
            ba += logStrVector.at(i);
            ba += seperatorLogBytes;
        }

        /*
        qDebug()<<"String :: "<<ba;
        qDebug()<<"\n\n\n";
        */

        logFile->write(ba.toBase64());

        logFile->close();
        ui->statusbar->showMessage(QString("Data saved in LogFile.: "+GlobalVariables::fileLogPath));
    } else {
        qDebug()<<" Error in opening report file :: "<<GlobalVariables::fileLogPath;
        ui->statusbar->showMessage(QString("Error in opening LogFile.: "+GlobalVariables::fileLogPath));
    }
    logFile->close();


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


void InsertNew::on_pbChangePassword_clicked()
{
    changePass = new ChangePassword(this);
    changePass->exec();
}
void InsertNew::on_plainTextEdit_textChanged()
{
    QString strr = ui->plainTextEdit->toPlainText();
    QStringList totalWords = strr.split(" ");
    QStringList totalSentences = strr.split(".");
    int totalChars = strr.remove(" ").size();

    QString output = QString("\t Chars:"+QString::number(totalChars, 10));
    output += QString("     Words:"+QString::number(totalWords.size()-1, 10));
    output += QString("     Sentences:"+QString::number(totalSentences.size()-1, 10));

    ui->statusbar->showMessage(output, 5000);
}
