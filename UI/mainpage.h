#ifndef MAINPAGE_H
#define MAINPAGE_H

#include "widgetinterface.h"

QT_FORWARD_DECLARE_CLASS(QPushButton)
QT_FORWARD_DECLARE_CLASS(QComboBox)
QT_FORWARD_DECLARE_CLASS(QSpinBox)
QT_FORWARD_DECLARE_CLASS(Klvideowidget)
QT_FORWARD_DECLARE_CLASS(QTextEdit)
QT_FORWARD_DECLARE_CLASS(QGroupBox)
QT_FORWARD_DECLARE_CLASS(QLabel)
QT_FORWARD_DECLARE_CLASS(QSettings)
QT_FORWARD_DECLARE_CLASS(QLineEdit)
class MainPage : public WidgetI
{
    Q_OBJECT
public:
    MainPage(QWidget *parent = nullptr);
    ~MainPage() override;

    void setUserStyle(int s = 0) override;

private slots:
    void slotSureBtnClicked();
    void slotStartBtnClicked();

private:
    void initUrlCombox(QComboBox*);
    void saveUrl();

    QPushButton *startBtn_, *stopBtn_, *sureBtn_;
    Klvideowidget *videoW_{nullptr};
    QSpinBox *viWidthSpinBox_, *viHeightSpinBox_, *viFpsSpinBox_;
    QTextEdit *textEdit_;
    QComboBox *inputUrlEdit_;
    QLabel *videoErrorL_;
    QLineEdit *cameraName_;
    QGroupBox *outSetGroupBox_, *inputGroupBox_;
    QString rtspServerHost_;
    QSettings *config_;
    bool isPushing_{false};
    const int url_limit_ = 5;
};

#endif // MAINPAGE_H
