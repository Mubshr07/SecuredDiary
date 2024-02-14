#include "RootClass.h"
#include "GUI/PasswordWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);

    qRegisterMetaType<GuiSubModules>();

    QIcon icon(":/icons/if_Superman_sigil_logo_movie_dc_comics_3151556.png");
    application.setWindowIcon(icon);

    RootClass root;
    return application.exec();
}
