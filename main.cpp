#include <QApplication>
#include "Service/servicefacetory.h"
#include "mainwindow.h"
#include "UI/logindialog.h"

#pragma execution_character_set("utf-8")
int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
    QApplication a(argc, argv);
    a.setApplicationName(QObject::tr("Rtsp推流工具"));
    ServiceFactoryI *facetoryI = new ServiceFactory;
    a.setProperty(FACETORY_KEY, reinterpret_cast<unsigned long long>(facetoryI));

    LoginDialog loginD;
    loginD.setUserStyle(0);
    loginD.resize(400,310);
    QDialog::DialogCode returnCode = QDialog::DialogCode(loginD.exec());
    if(returnCode == QDialog::Rejected){
        return -1;
    }

    MainWindow w;
    w.resize(755, 491);
    w.show();

    QObject::connect(qApp, &QApplication::aboutToQuit, [&]{
        delete facetoryI;
    });
    return a.exec();
}
