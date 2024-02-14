#ifndef GLOBALSTATICVARS_H
#define GLOBALSTATICVARS_H


#include <QObject>

enum GuiSubModules{
    guiNULL,
    guiChangePassword,
    guiMainWindow,
    guiMessageBoxx,
    guiInsertNew,
    guiPasswordWindow
};


#include <QThread>
#include <QTimer>
#include <QVector>
#include <QTextStream>
#include <QFile>
#include <QIODevice>
#include <QFileInfo>
#include <QFileDialog>
#include <QDir>
#include <QFileDialog>
#include <QDateTime>
#include <QTextCursor>
#include <QTextDocument>
#include <QPrinter>
#include <QMessageBox>
#include <QReadWriteLock>
#include <QCryptographicHash>
#include <QIntValidator>

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <QProcess>
#include <QPalette>

#include <QEvent>
#include <QKeyEvent>
#include <QEnterEvent>
#include <QDragMoveEvent>
#include <QPoint>
#include <QPointF>
#include <QMap>
#include <QPrinter>
#include <QPaintDevice>
#include <QPdfWriter>
#include <QPrinterInfo>
#include <QList>
#include <QPagedPaintDevice>

#include "math.h"
#include "qmath.h"

#include <QPixmap>

#include "qdebug.h"

#define ErrorNumber 15179
#define SuccessNumber 007

class GlobalVariables : public QObject
{
    Q_OBJECT
public:
    explicit GlobalVariables(QObject *parent = nullptr);

signals:

public:
    static bool globalDebugMode;
    static long globalDebugCounter;

    static QString mainFolderPath;
    static QString filePathPasswords;
    static QString fileLogPath;
    static QString fileConfig;


    static QByteArray passwordAdminHash;


    static void initializeAllStaticVars();

};

Q_DECLARE_METATYPE(GuiSubModules);


#endif // GLOBALSTATICVARS_H
