#include "GlobalVariables.h"



bool GlobalVariables::globalDebugMode = true;
long GlobalVariables::globalDebugCounter = 0;


QString GlobalVariables::mainFolderPath = QString(QDir::homePath() + "/.config/emotions");
QString GlobalVariables::filePathPasswords = "";
QString GlobalVariables::fileLogPath = "";
QString GlobalVariables::fileConfig = "";
QByteArray GlobalVariables::passwordAdminHash;


GlobalVariables::GlobalVariables(QObject *parent) : QObject(parent)
{
}

void GlobalVariables::initializeAllStaticVars()
{
    //qDebug()<<" Global Initialized ";
    GlobalVariables::filePathPasswords = QString(mainFolderPath + "/passwordsAuthentications");
    GlobalVariables::fileLogPath  = QString(mainFolderPath + "/emotions2022");
    GlobalVariables::fileConfig = QString(mainFolderPath + "/Configuration");

    GlobalVariables::passwordAdminHash = "3439B05E148FDA79AF58696EDB5C099432D476437BE90FABC4411BD578A3EA119550C44DBAEB7A305130C3AD1FDD7D5C67D3A07315381DF6201936AECDE6DE7E";
    //GlobalVariables::passwordAdminHash = "3C9909AFEC25354D551DAE21590BB26E38D53F2173B8D3DC3EEE4C047E7AB1C1EB8B85103E3BE7BA613B31BB5C9C36214DC9F14A42FD7A2FDB84856BCA5C44C2";

}
