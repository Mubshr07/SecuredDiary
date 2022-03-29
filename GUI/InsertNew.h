#ifndef INSERTNEW_H
#define INSERTNEW_H

#include <QMainWindow>
#include "GlobalVariables.h"
#include "ChangePassword.h"

QT_BEGIN_NAMESPACE
namespace Ui { class InsertNew; }
QT_END_NAMESPACE

class InsertNew : public QMainWindow
{
    Q_OBJECT

public:
    InsertNew(QWidget *parent = nullptr);
    ~InsertNew();

signals:
    void txCloseInsertNew();
    void txGenerateGUIModule(GuiSubModules module);



public slots: 
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
    void on_ZoomInPushButton_clicked();
    void on_ZoomOutPushButton_clicked();
    void on_NewLogPushButton_clicked();



    void on_deleteThisLogPushButton_clicked();

private:
    Ui::InsertNew *ui;

    ChangePassword *changePass;
    bool changePassIsClosed = true;

    QTimer *timerSingleShot;
    QByteArray seperatorDateBytes = "::$%@#::\n";
    QByteArray seperatorLogBytes = "::!!||!!::\n\n";
    QFile *logFile;
    void saveCurrentLogIntoFile();

    QByteArray wholeFile = "";
    int logCounter = 0;
    QVector<QString> dateVector;
    QVector<QString> logStrVector;


    QFont localFont;
    bool localBoldBoolean = false;


};
#endif // INSERTNEW_H
