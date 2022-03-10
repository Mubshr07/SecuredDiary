#ifndef ROOTCLASS_H
#define ROOTCLASS_H

#include <QObject>





#include "GlobalVariables.h"

#include "GUI/ChangePassword.h"
#include "GUI/MainWindow.h"
#include "GUI/MessageBoxx.h"
#include "GUI/PasswordWindow.h"

class RootClass : public QObject
{
    Q_OBJECT
public:
    explicit RootClass(QObject *parent = nullptr);

signals:


public slots:
    void rxGenerateGUIModule(GuiSubModules module);
    void rxClosingPasswordWindow(bool close);



    void onTimerSingleShotElapsed();
    void onTimerGUIGeneratorElapsed();


private:
    ChangePassword *changePasswordWindow;
    MainWindow *mainGUI;
    MessageBoxx *messageBoxWindow;
    PasswordWindow *passwordAuthentication;

    QTimer *timerSingleShot;
    QTimer *timerGUIGenerator;
    GuiSubModules needToGenerateThisGUI = guiNULL;



    void generatePasswordAuthenticationWindow();
    void generateChangePasswordWindow();
    void generateMainWindowGUI();

    QFile *passwordHashFile;
    void initializeFoldersAndReadPasswordHashes();

};

#endif // ROOTCLASS_H
