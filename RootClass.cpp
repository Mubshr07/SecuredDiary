#include "RootClass.h"
#include "QApplication"

RootClass::RootClass(QObject *parent) : QObject(parent)
{
    GlobalVariables::initializeAllStaticVars();
    initializeFoldersAndReadPasswordHashes();
    timerSingleShot = new QTimer(this);
    connect(timerSingleShot, SIGNAL(timeout()), this, SLOT(onTimerSingleShotElapsed()));
    timerSingleShot->setSingleShot(true);
    timerSingleShot->start(200);


    timerGUIGenerator = new QTimer(this);
    connect(timerGUIGenerator, SIGNAL(timeout()), this, SLOT(onTimerGUIGeneratorElapsed()));
    timerGUIGenerator->setSingleShot(true);


}

void RootClass::rxGenerateGUIModule(GuiSubModules module)
{
    //qDebug()<<"rxGenerateGUIModule::  "<<module;
    needToGenerateThisGUI = module;
    timerGUIGenerator->start(200);
}

void RootClass::rxClosingPasswordWindow(bool close)
{
    qDebug()<<"rxClosingPasswordWindow:: "<<close;
    if(close) QApplication::quit();
}

void RootClass::onTimerSingleShotElapsed()
{
    generatePasswordAuthenticationWindow();
}
void RootClass::onTimerGUIGeneratorElapsed()
{
    switch (needToGenerateThisGUI) {
    case  guiNULL: break;
    case guiChangePassword: generateChangePasswordWindow(); break;
    case guiMainWindow: generateMainWindowGUI(); break;
    case guiInsertNew: generateInsertNewWindow(); break;
    case guiMessageBoxx: break;
    case guiPasswordWindow: generatePasswordAuthenticationWindow(); break;

    }
}




void RootClass::generatePasswordAuthenticationWindow()
{
    passwordAuthentication = new PasswordWindow();
    connect(passwordAuthentication, SIGNAL(txGenerateGUIModule(GuiSubModules)), this, SLOT(rxGenerateGUIModule(GuiSubModules)));
    passwordAuthentication->show();
}
void RootClass::generateInsertNewWindow()
{
    insertNewWindow = new InsertNew();
    connect(insertNewWindow, SIGNAL(txGenerateGUIModule(GuiSubModules)), this, SLOT(rxGenerateGUIModule(GuiSubModules)));
    connect(insertNewWindow, SIGNAL(txCloseInsertNew()), this, SLOT(rxCloseInsertNew()));

    insertNewWindow->showFullScreen();

}
void RootClass::generateChangePasswordWindow()
{

}
void RootClass::generateMainWindowGUI()
{
    //qDebug()<<" generateMainWindowGUI() " ;
    mainGUI = new MainWindow();
    mainGUI->showMaximized();
}

void RootClass::initializeFoldersAndReadPasswordHashes()
{
    QDir mainDir(GlobalVariables::mainFolderPath);
    if (!mainDir.exists())
    {
        mainDir.mkdir(GlobalVariables::mainFolderPath);
    }

    //qDebug()<<" Password FilePath:  "<<GlobalVariables::filePathPasswords;
    // ------- Password Hash File
    QByteArray lineByte[4];
    if(QFileInfo::exists(GlobalVariables::filePathPasswords))
    {
        passwordHashFile = new QFile(GlobalVariables::filePathPasswords);
        if(passwordHashFile->open(QIODevice::ReadOnly)) {
            int counter = 0;
            while(!passwordHashFile->atEnd()) {
                lineByte[counter] = passwordHashFile->readLine();
                lineByte[counter].chop(1);
                counter++;
            }
            passwordHashFile->close();
            GlobalVariables::passwordAdminHash=lineByte[0].toUpper();
            //qDebug()<<"Admin PassHash:"<<GlobalVariables::passwordAdminHash;
        } else {
            qDebug()<<" Password File did not opened";
        }
    }


    //qDebug()<<" Configuration FilePath:  "<<GlobalVariables::fileConfig;
    // -------  Configuration File
    if(QFileInfo::exists(GlobalVariables::fileConfig))
    {
        passwordHashFile = new QFile(GlobalVariables::fileConfig);
        if(passwordHashFile->open(QIODevice::ReadOnly)) {
            int counter = 0;
            while(!passwordHashFile->atEnd()) {
                lineByte[counter] = passwordHashFile->readLine();
                //qDebug()<<" lineByte of Config::: "<<lineByte[counter];
                lineByte[counter].chop(1);
                counter++;
            }
            passwordHashFile->close();
            GlobalVariables::fileLogPath = lineByte[0];
            //qDebug()<<"fileLogPath Path:"<<GlobalVariables::fileLogPath;
        } else {
            qDebug()<<" Password File did not opened";
        }
    }
}




void RootClass::rxCloseInsertNew()
{
    QApplication::quit();
}
