#include "RootClass.h"
#include "GUI/PasswordWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);

    qRegisterMetaType<GuiSubModules>();


    RootClass root;
    return application.exec();
}
