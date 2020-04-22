#include <QStackedLayout>
#include <QVBoxLayout>
#include "UI/mainpage.h"
#include "mainwindow.h"

#pragma execution_character_set("utf-8")
MainWindow::MainWindow(QWidget *parent)
    : WidgetI(parent)
{
    stackedLayout_ = new QStackedLayout;
    QWidget *page = new MainPage;
    stackedLayout_->addWidget(page);
    setLayout(stackedLayout_);
    setUserStyle();
}

MainWindow::~MainWindow()
{

}

void MainWindow::setUserStyle(int s)
{

}
