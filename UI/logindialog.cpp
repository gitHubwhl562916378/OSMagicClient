#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QSpinBox>
#include <QLineEdit>
#include <QLabel>
#include <QApplication>
#include <QDialogButtonBox>
#include <QSettings>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPainter>
#include <QTimer>
#include <QDebug>
#include <QMessageBox>
#include "Service/servicei.h"
#include "waitinglabel.h"
#include "logindialog.h"

#pragma execution_character_set("utf-8")
LoginDialog::LoginDialog(QWidget *parent, Qt::WindowFlags f):
    QDialog(parent,f)
{
    setObjectName(tr("登录"));
    QVBoxLayout *mainLay = new QVBoxLayout;
    logoLabel_ = new QLabel;
    logoSpitL_ = new QLabel;
    logoTitleL_ = new QLabel(qApp->applicationName());
    logoBackW_ = new QWidget;

    ptnMinsized_ = new QPushButton(tr("-"));
    ptnClose_ = new QPushButton(tr("×"));
    QHBoxLayout *hlay = new QHBoxLayout;
    hlay->setAlignment(Qt::AlignLeft);
    hlay->addWidget(logoLabel_);
    hlay->addWidget(logoSpitL_);
    hlay->addWidget(logoTitleL_);
    QHBoxLayout *topRightLay = new QHBoxLayout;
    topRightLay->addWidget(ptnMinsized_);
    topRightLay->addWidget(ptnClose_);
    topRightLay->setAlignment(Qt::AlignTop | Qt::AlignRight);
    hlay->addStretch();
    hlay->addLayout(topRightLay);

    logoBackW_->setLayout(hlay);
    mainLay->addWidget(logoBackW_);

    QGridLayout *centerLay = new QGridLayout;
    userL_ = new QLabel(tr("用户名"));
    userEdit_ = new QLineEdit;
    userEdit_->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    pswL_ = new QLabel(tr("密码"));
    pswEdit_ = new QLineEdit;
    pswEdit_->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    centerLay->addWidget(userL_,1,0,1,1);
    centerLay->addWidget(userEdit_,1,1,1,1);
    centerLay->addWidget(pswL_,2,0,1,1);
    centerLay->addWidget(pswEdit_,2,1,1,1);
    centerLay->setContentsMargins(0,10,0,0);

    centerLay->setAlignment(Qt::AlignCenter);
    mainLay->addLayout(centerLay);

    hlay = new QHBoxLayout;
    btnBox_ = new QDialogButtonBox;
    ptnSurce_ = btnBox_->addButton(tr("登录"),QDialogButtonBox::AcceptRole);
    ptnCancell_ = btnBox_->addButton(tr("退出"),QDialogButtonBox::RejectRole);
    hlay->addStretch();
    hlay->addWidget(btnBox_);
    hlay->addStretch();
    hlay->setContentsMargins(0,2,0,20);
    mainLay->addLayout(hlay);
    mainLay->setMargin(0);
    setLayout(mainLay);

    setWindowFlag(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    ptnMinsized_->setFixedSize(26,26);
    ptnClose_->setFixedSize(26,26);
    userEdit_->setMinimumHeight(40);
    pswEdit_->setMinimumHeight(40);
    ptnSurce_->setMinimumHeight(35);
    ptnSurce_->setMaximumWidth(60);
    ptnCancell_->setMinimumHeight(35);
    ptnCancell_->setMaximumWidth(60);

    QPixmap logoPix = QPixmap("images/userImg.png");
    logoLabel_->setPixmap(logoPix);
    logoSpitL_->setFrameStyle(QFrame::VLine | QFrame::Raised);
    logoSpitL_->setLineWidth(0);
    logoSpitL_->setMidLineWidth(1);
    logoSpitL_->setFixedHeight(30);

    config_ = new QSettings("config.ini",QSettings::IniFormat, this);
    userEdit_->setText(config_->value("User/username").toString());
    pswEdit_->setEchoMode(QLineEdit::Password);
    pswEdit_->setText(decodeC(config_->value("User/password").toString()));
    connect(ptnMinsized_,SIGNAL(clicked(bool)),this,SLOT(showMinimized()));
    connect(ptnClose_,SIGNAL(clicked(bool)),this,SLOT(reject()));
    connect(ptnSurce_,SIGNAL(clicked(bool)),this,SLOT(slotSureBtnClicked()));
    connect(ptnCancell_,SIGNAL(clicked(bool)),this,SLOT(reject()));

    setUserStyle();
}

LoginDialog::~LoginDialog()
{
}

void LoginDialog::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
    movieable_ = false;
}

void LoginDialog::mousePressEvent(QMouseEvent *event)
{
    if(logoBackW_->geometry().contains(event->pos())){
        movieable_ = true;
        startPos_ = event->globalPos();
    }
}

void LoginDialog::mouseMoveEvent(QMouseEvent *event)
{
    if(movieable_){
        move(pos() + event->globalPos() - startPos_);
        startPos_ = event->globalPos();
    }
}

void LoginDialog::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    painter.setPen(QColor(Qt::blue).lighter());
    painter.setBrush(QColor(0,0,0,255));
    painter.drawRoundRect(rect().adjusted(0,0,-painter.pen().width(),-painter.pen().width()),5,5);
}

QString LoginDialog::encodeC(const QString &src)
{
    QString result;
    for(auto sc : src)
    {
        result += sc.toLatin1() + 32;
    }
    return  result;
}

QString LoginDialog::decodeC(const QString &src)
{
    QString result;
    for(auto sc : src)
    {
        result += sc.toLatin1() - 32;
    }
    return  result;
}

void LoginDialog::setUserStyle(int s)
{
    QPalette pal;
    QFont f = font();
    f.setPixelSize(16);
    setFont(f);

    pal = palette();
    pal.setColor(QPalette::Background,Qt::transparent);
    pal.setColor(QPalette::Foreground,Qt::white);
    setPalette(pal);
    setAutoFillBackground(true);
    if(s == 0){
        pal.setColor(QPalette::Background,QColor(48, 54, 68,85));
        logoBackW_->setPalette(pal);
        logoBackW_->setAutoFillBackground(true);


        pal = logoSpitL_->palette();
        pal.setColor(QPalette::Mid,Qt::white);
        logoSpitL_->setPalette(pal);

        userEdit_->setStyleSheet("QLineEdit{"
                               "color:white;"
                               "border:1px solid rgb(71, 65, 152);"
                               "border-radius:5px;"
                               "background-color:transparent;"
                               "}");
        pswEdit_->setStyleSheet("QLineEdit{"
                               "color:white;"
                               "border:1px solid rgb(71, 65, 152);"
                               "border-radius:5px;"
                               "background-color:transparent;"
                               "lineedit-password-character: 42;"
                               "}");
        ptnSurce_->setStyleSheet("QPushButton{"
                                 "color:rgba(255,255,255,0.75);"
                                 "background-color:rgb(65,73,92);"

                                 "}"
                                 "QPushButton:hover{"
                                 "padding:1px;"
                                 "color:rgba(0,0,0,0.4);"
                                 "background-color:rgb(65,73,82);"
                                 "}");

        ptnCancell_->setStyleSheet("QPushButton{"
                                 "color:rgba(255,255,255,0.75);"
                                 "background-color:rgb(65,73,92);"

                                 "}"
                                 "QPushButton:hover{"
                                 "padding:1px;"
                                 "color:rgba(0,0,0,0.4);"
                                 "background-color:rgb(65,73,82);"
                                 "}");

        ptnClose_->setStyleSheet("QPushButton{"
                                 "color:rgba(255,255,255,0.75);"
                                 "border-radius:4px;"
                                 "font-size: 18px;"
                                 "background-color:rgb(65,73,92);"
                                 "}"
                                 "QPushButton:hover{"
                                 "color:rgba(0,0,0,0.4);"
                                 "background-color:rgb(65,73,82);"
                                 "}");
        ptnMinsized_->setStyleSheet("QPushButton{"
                                 "color:rgba(255,255,255,0.75);"
                                 "border-radius:4px;"
                                 "font-size: 18px;"
                                 "background-color:rgb(65,73,82);"
                                 "}"
                                 "QPushButton:hover{"
                                 "color:rgba(0,0,0,0.4);"
                                 "background-color:rgb(65,73,82);"
                                 "}");
    }
}

QString LoginDialog::userName() const
{
    return userEdit_->text();
}

void LoginDialog::slotSureBtnClicked()
{
    WaitingLabel *waitL = new WaitingLabel(this);
    waitL->setAttribute(Qt::WA_DeleteOnClose);
    waitL->setMovieFile("images/login.gif");
    ServiceFactoryI *factory = reinterpret_cast<ServiceFactoryI*>(qApp->property(FACETORY_KEY).toULongLong());
    RestServiceI *serviceI = factory->makeRestServiceI();
    connect(serviceI, &RestServiceI::sigError, this, [this, waitL](QString msg){
        waitL->close();
        setEnabled(true);
        QMessageBox::warning(this, objectName(), msg);
    });
    connect(serviceI, &RestServiceI::sigLoginSuccessed, this, [this, waitL](QString resData){
        waitL->close();
        qInfo() << tr("登录成功") << resData;
        qApp->setProperty(USER, userEdit_->text());
        qApp->setProperty(PASSWORD, resData);
        config_->setValue("User/username", userEdit_->text());
        config_->setValue("User/password", encodeC(pswEdit_->text()));
        setEnabled(true);
        accept();
    });

    waitL->show(500);
    setEnabled(false);
    serviceI->login(userEdit_->text(), pswEdit_->text());
}
