#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "GlobalVariables.h"
#include "ChangePassword.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void txCloseMainWindow();



public slots:
    void rxClosingChangePasswordWindow();

    void onTimerSingleShotElapsed();


private slots:
    void on_SavePushButton_clicked();
    void on_SaveClosePushButton_clicked();
    void on_loadPushButton_clicked();
    void on_newFilePushButton_clicked();
    void on_boldPushButton_clicked();
    void on_cmbColorSelector_currentIndexChanged(int index);
    void on_cmbFontSize_currentIndexChanged(int index);
    void on_boldTextIconPushButton_clicked();
    void on_onlyClosePushButton_clicked();
    void on_endOfLinePushButton_clicked();
    void on_changePasswordPushButton_clicked();



private:
    Ui::MainWindow *ui;


    ChangePassword *changePass;
    bool changePassIsClosed = true;

    QTimer *timerSingleShot;

    QFile *logFile;
    void saveCurrentLogIntoFile();


    QFont localFont;
    bool localBoldBoolean = false;


};
#endif // MAINWINDOW_H
