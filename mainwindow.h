#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "UI/widgetinterface.h"

QT_FORWARD_DECLARE_CLASS(QStackedLayout)
class MainWindow : public WidgetI
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void setUserStyle(int s = 0) override;

private:
    QStackedLayout *stackedLayout_{nullptr};
};

#endif // MAINWINDOW_H
