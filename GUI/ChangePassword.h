#ifndef CHANGEPASSWORD_H
#define CHANGEPASSWORD_H

enum PassChangeState {
    PassVerifyOLD,
    PassEnterNew,
    PassConfirmNew
};

#include <QDialog>
#include "../GlobalVariables.h"

namespace Ui {
class ChangePassword;
}

class ChangePassword : public QDialog
{
    Q_OBJECT

public:
    explicit ChangePassword(QWidget *parent = 0);
    ~ChangePassword();


signals:
    void txClosingChangePasswordWindow();
    void txShowMessageBox(GuiSubModules parentWindow, int parm, QString title, QString msg, QString yesBtnText, bool yesVisible, QString noBtnText, bool noVisible);
    void txShowMessageBox(GuiSubModules parentWindow, int parm, QString title, QString msg, QString yesBtnText, QString yesBtnStyle, bool yesVisible, QString noBtnText, QString noBtnStyle, bool noVisible);



public slots:

    //Timer Slots
    void rxTimerSingleShotElapsed();
    void rxMessageBoxResult(int parm, bool yesNoButton);


private slots:
    void on_passwordPushButtonEnter_clicked();
    void on_passwordPushButtonClear_clicked();
    void on_passwordWindowCancel_clicked();



    void on_lblPassWordInput_returnPressed();

private:
    Ui::ChangePassword *ui;
    QTimer *timerSingleShot;
    int singleShotTimerOperation = 0;

    QString styleLabelNormal = "font: 800 32pt 'Times New Roman'; background-color: rgb(255, 255, 255); padding-top:8px; ";
    QString styleLabelGreen = "font: 800 32pt 'Times New Roman'; background-color: green; padding-top:8px; ";
    QString styleLabelYellow = "font: 800 32pt 'Times New Roman'; background-color: yellow; padding-top:8px; ";
    QString styleLabelRed = "font: 800 32pt 'Times New Roman'; background-color: red; padding-top:8px; ";

    bool userPasswordChangeRequest = true;
    QByteArray enteredPasswordByteArray;
    QByteArray enteredNewPasswordByteArray;


    void saveNewHashesAndCloseThisWindow();
    void changeGUIAccordingToStates();

    PassChangeState passwordState =  PassVerifyOLD;

};

#endif // CHANGEPASSWORD_H
