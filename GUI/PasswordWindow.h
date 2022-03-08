#ifndef PASSWORDWIN_H
#define PASSWORDWIN_H

#include <QMainWindow>

#include "../GlobalVariables.h"

namespace Ui {
class PasswordWindow;
}

class PasswordWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PasswordWindow(QWidget *parent = nullptr);
    ~PasswordWindow();

signals:
    void txClosingPasswordWindow();
    void txGenerateGUIModule(GuiSubModules module);


public slots:

    //Timer Slots
    void rxTimerSingleShotElapsed();


private slots:
    void on_passwordpushButtonEnter_clicked();
    void on_CancelWindowpushButton_clicked();
    void on_lblPassWord_returnPressed();

private:
    Ui::PasswordWindow *ui;

    QTimer *timerSingleShot;
    int singleShotTimerOperation = 0;

    QString styleLabelNormal = "font: 800 32pt 'Times New Roman'; background-color: rgb(255, 255, 255); padding-top:8px; color:black; ";
    QString styleLabelGreen = "font: 800 32pt 'Times New Roman'; background-color: green; padding-top:8px;  color:black;";
    QString styleLabelYellow = "font: 800 32pt 'Times New Roman'; background-color: yellow; padding-top:8px;  color:black;";
    QString styleLabelRed = "font: 800 32pt 'Times New Roman'; background-color: red; padding-top:8px;  color:black;";


};

#endif // PASSWORDWIN_H
