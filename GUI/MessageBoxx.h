#ifndef MSGWIN_H
#define MSGWIN_H

#include <QDialog>

namespace Ui {
class MessageBoxx;
}

class MessageBoxx : public QDialog
{
    Q_OBJECT

public:
    explicit MessageBoxx(QWidget *parent = nullptr);
    ~MessageBoxx();


signals:
    void txMessageBoxResult(int parm, bool yesNoButton);


public slots:
    void rxSetMessageBoxParams(int parm, QString title, QString msg);
    void rxSetMessageBoxParams(int parm, QString title, QString msg, QString yesBtnText, bool yesVisible, QString noBtnText, bool noVisible);
    void rxSetMessageBoxParams(int parm, QString title, QString msg, QString yesBtnText, QString yesBtnStyle, bool yesVisible, QString noBtnText, QString noBtnStyle, bool noVisible);


private slots:
    void on_yesPushButton_clicked();
    void on_noPushButton_clicked();



private:
    Ui::MessageBoxx *ui;
    int local_parm = 0;
};

#endif // MSGWIN_H
