#include "RootClass.h"

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
    qDebug()<<"rxGenerateGUIModule::  "<<module;
    needToGenerateThisGUI = module;
    timerGUIGenerator->start(200);
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
void RootClass::generateChangePasswordWindow()
{

}
void RootClass::generateMainWindowGUI()
{
    qDebug()<<" generateMainWindowGUI() " ;
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

    //qDebug()<<" All Local Dir created ";


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
            if(counter > 0) GlobalVariables::passwordAdminHash=lineByte[0].toUpper();
            //qDebug()<<"Admin PassHash:"<<GlobalVariables::password_AdminHash;
        } else {
            qDebug()<<" Password File did not opened";
        }
    }


    // -------  Configuration File
    if(QFileInfo::exists(GlobalVariables::fileConfig))
    {
        passwordHashFile = new QFile(GlobalVariables::fileConfig);
        if(passwordHashFile->open(QIODevice::ReadOnly)) {
            int counter = 0;
            while(!passwordHashFile->atEnd()) {
                lineByte[counter] = passwordHashFile->readLine();
                lineByte[counter].chop(1);
                counter++;
            }
            passwordHashFile->close();
            if(counter > 0)
                GlobalVariables::fileLogPath = lineByte[0];
            //qDebug()<<"Admin PassHash:"<<GlobalVariables::password_AdminHash;
        } else {
            qDebug()<<" Password File did not opened";
        }
    }
}



