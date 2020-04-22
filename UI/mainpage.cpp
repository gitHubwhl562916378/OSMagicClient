#include <QComboBox>
#include <QApplication>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QMessageBox>
#include <QSpinBox>
#include <QGroupBox>
#include <QLabel>
#include <QTextEdit>
#include <QSettings>
#include <QDebug>
#include <QUuid>
#include "Service/servicei.h"
#include "klvideowidget.h"
#include "mainpage.h"

#pragma execution_character_set("utf-8")
MainPage::MainPage(QWidget *parent):
    WidgetI (parent)
{
    setObjectName(tr("首页"));
    QVBoxLayout *mainLay = new QVBoxLayout;
    inputUrlEdit_ = new QComboBox;
    inputUrlEdit_->setEditable(true);
    inputUrlEdit_->lineEdit()->setPlaceholderText(tr("rtsp://192.168.2.55/person.avi"));
    sureBtn_ = new QPushButton(tr("确定"));
    videoW_ = new Klvideowidget;
    videoErrorL_ = new QLabel();
    videoErrorL_->setAlignment(Qt::AlignCenter);
    videoErrorL_->hide();
    cameraName_ = new QLineEdit(tr("摄像头1"));
    viWidthSpinBox_ = new QSpinBox;
    viWidthSpinBox_->setMaximum(4096);
    viWidthSpinBox_->setValue(1920);
    viHeightSpinBox_ = new QSpinBox;
    viHeightSpinBox_->setMaximum(2160);
    viHeightSpinBox_->setValue(1080);
    viFpsSpinBox_ = new QSpinBox;
    viFpsSpinBox_->setMaximum(60);
    viFpsSpinBox_->setValue(15);
    inputGroupBox_ = new QGroupBox(tr("视频源"));
    outSetGroupBox_ = new QGroupBox(tr("编码设置"));
    textEdit_ = new QTextEdit;
    textEdit_->setReadOnly(true);
    startBtn_ = new QPushButton(tr("开始"));
    stopBtn_ = new QPushButton(tr("停止"));
    QFormLayout *formLay = new QFormLayout;
    formLay->addRow(tr("请输入网络URL"), inputUrlEdit_);
    QHBoxLayout *topLHLay = new QHBoxLayout;
    topLHLay->addLayout(formLay, 11);
    topLHLay->addWidget(sureBtn_, 3);
    QHBoxLayout *hlay = new QHBoxLayout;
    hlay->addWidget(videoErrorL_);
    videoW_->setLayout(hlay);
    QVBoxLayout *vlay = new QVBoxLayout;
    vlay->addLayout(topLHLay);
    vlay->addWidget(videoW_);
    inputGroupBox_->setLayout(vlay);

    formLay = new QFormLayout;
    formLay->addRow(tr("摄像机名称"), cameraName_);
    formLay->addRow(tr("宽度"), viWidthSpinBox_);
    formLay->addRow(tr("高度"), viHeightSpinBox_);
    formLay->addRow(tr("帧率"), viFpsSpinBox_);
    formLay->setVerticalSpacing(30);
    formLay->setRowWrapPolicy(QFormLayout::DontWrapRows);
    //    formLay->setFieldGrowthPolicy(QFormLayout::FieldsStayAtSizeHint);
    formLay->setFormAlignment(Qt::AlignHCenter | Qt::AlignLeft);
    //    formLay->setLabelAlignment(Qt::AlignLeft);
    hlay = new QHBoxLayout;
    hlay->addWidget(startBtn_);
    hlay->addWidget(stopBtn_);
    vlay = new QVBoxLayout;
    vlay->addLayout(formLay);
    vlay->addStretch();
    vlay->addLayout(hlay);
    outSetGroupBox_->setLayout(vlay);

    hlay = new QHBoxLayout;
    hlay->addWidget(inputGroupBox_, 14);
    hlay->addWidget(outSetGroupBox_, 3);

    mainLay->addLayout(hlay, 14);
    mainLay->addWidget(textEdit_, 3);
    mainLay->setMargin(10);
    setLayout(mainLay);
    setUserStyle();

    connect(startBtn_, SIGNAL(clicked()), this, SLOT(slotStartBtnClicked()));
    connect(sureBtn_, SIGNAL(clicked()), this, SLOT(slotSureBtnClicked()));

    connect(videoW_, &Klvideowidget::sigError, this, [&](QString str){
        videoW_->stop();
        QTimer::singleShot(5000, this, [&]{videoW_->startPlay(videoW_->url(), videoW_->decoderName());});
        int times = videoErrorL_->property("reconnectInterval").toInt() + 1;
        videoErrorL_->setProperty("reconnectInterval", times);
        videoErrorL_->setText(str + tr(" 正在重连---") + QString::number(times));
        videoErrorL_->show();
    });
    connect(videoW_, &Klvideowidget::sigVideoStart, this, [&](int w, int h){
        videoErrorL_->hide();
        textEdit_->append(videoW_->url() + tr(" 播放成功"));
    });

    config_ = new QSettings("config.ini", QSettings::IniFormat, this);
    rtspServerHost_ = config_->value("RTSP/host").toString();
    initUrlCombox(inputUrlEdit_);
}

MainPage::~MainPage()
{
    saveUrl();
}

void MainPage::setUserStyle(int s)
{
    QPalette pal;
    if(s == 0)
    {
        pal = videoErrorL_->palette();
        pal.setColor(QPalette::Foreground, Qt::red);
        videoErrorL_->setPalette(pal);
    }
}

void MainPage::slotSureBtnClicked()
{
    if(inputUrlEdit_->currentText().isEmpty())
    {
        QMessageBox::warning(this, tr("输入源"), tr("URL不能为空"));
        return;
    }
    if(isPushing_)
    {
        QMessageBox::warning(this, tr("输入源"), tr("请先停掉正在推送的流"));
        return;
    }

    if(inputUrlEdit_->count() > url_limit_)
    {
        int size = inputUrlEdit_->count() - url_limit_ + 1;
        for(int i = 0; i < size; i++)
        {
            inputUrlEdit_->removeItem(inputUrlEdit_->count() - 1);
        }
    }
    if(inputUrlEdit_->findText(inputUrlEdit_->currentText()) == -1)
    {
        inputUrlEdit_->insertItem(0, inputUrlEdit_->currentText());
    }
    videoW_->stop();
    videoW_->startPlay(inputUrlEdit_->currentText(), "ffmpeg");
}

void MainPage::slotStartBtnClicked()
{
    if(videoW_->playState() != Klvideowidget::Playing)
    {
        QMessageBox::information(this, tr("推流"), tr("请先输入网络流URL, 并播放"));
        return;
    }
    if(isPushing_)
    {
        QMessageBox::warning(this, tr("推流"), tr("只允许推送一路视频流, 请先停止正在推送的流"));
        return;
    }
    ServiceFactoryI *facetoryI = reinterpret_cast<ServiceFactoryI*>(qApp->property(FACETORY_KEY).toULongLong());
    VideoEncodeI* encodeI = facetoryI->makeVideoEncodeI();
    VideoEncodeI::EncodeParams params;
    QString input_url = inputUrlEdit_->currentText();
    QString out_url = "rtsp://" + rtspServerHost_ + "/osmagic/experience/" + QUuid::createUuid().toString().remove('{').remove('}');

    connect(encodeI, &VideoEncodeI::sigStarted, this, [=]{
        textEdit_->append(tr("已推流到URL: ") + out_url);
        isPushing_ = true;

        RestServiceI* serI = facetoryI->makeRestServiceI();
        connect(serI, &RestServiceI::sigError, textEdit_, [=](QString msg){
            textEdit_->append(tr("添加相机到平台错误: ") + msg);
        });
        connect(serI, &RestServiceI::sigAddCamera, textEdit_, [=](int id){
            textEdit_->append(tr("添加相机成功，设备id为 ") + QString::number(id));
        });
        RestServiceI::AddCameraParams params;
        params.rtsp = out_url;
        params.groupId = 5;
        params.groupName = "";
        params.tableName = "kl_device_camera";
        params.cameraName = cameraName_->text();
        serI->addCamera(params);

    });
    connect(encodeI, &VideoEncodeI::sigStoped, this, [=]{
        textEdit_->append(tr("已停止推送URL: ") + input_url + " >> " + out_url);
        isPushing_ = false;
    });
    connect(encodeI, &VideoEncodeI::sigError, this, [=](QString msg){
        textEdit_->append(tr("推流错误: ") + msg);
        delete encodeI;
    });
    connect(stopBtn_, &QPushButton::clicked, this, [=]{
        encodeI->stopEncode();
        stopBtn_->disconnect();
        textEdit_->append(tr("开始停止推送 ") + out_url);
    });
    qstrcpy(params.input_file, inputUrlEdit_->currentText().toStdString().data());
    qstrcpy(params.out_file, out_url.toStdString().data());
    params.fps = viFpsSpinBox_->value();
    params.qmax = 24;
    params.qmin = 16;
    params.width = viWidthSpinBox_->value();
    params.height = viHeightSpinBox_->value();
    params.code_id = 27;
    params.pix_fmt = 0;
    params.bit_rate = 1600000;
    params.gop_size = 150;
    params.me_range = 16;
    params.max_qdiff = 4;
    params.qcompress = 0.6;
    params.keyint_min = 30;
    params.media_type = 0;
    params.max_b_frames = 0;
    params.thread_count = 15;
    params.b_frame_strategy = true;
    encodeI->startEncode(params);
    textEdit_->append(tr("开始视频推流: ") + input_url + " >> " + out_url);
}

void MainPage::initUrlCombox(QComboBox *combox)
{
    int count = config_->beginReadArray("Url");
    for(int i = 0; i < count; i++)
    {
        config_->setArrayIndex(i);
        combox->addItem(config_->value("url").toString());
    }
    if(count)
    {
        combox->setCurrentIndex(0);
    }
    config_->endArray();
}

void MainPage::saveUrl()
{
    config_->beginWriteArray("Url", inputUrlEdit_->count());
    for(int i = 0; i < inputUrlEdit_->count(); i++)
    {
        config_->setArrayIndex(i);
        config_->setValue("url", inputUrlEdit_->itemText(i));
    }
    config_->endArray();
}
