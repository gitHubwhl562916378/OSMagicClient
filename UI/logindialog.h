#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include "widgetinterface.h"
QT_FORWARD_DECLARE_CLASS(QLabel)
QT_FORWARD_DECLARE_CLASS(QLineEdit)
QT_FORWARD_DECLARE_CLASS(QPushButton)
QT_FORWARD_DECLARE_CLASS(QSpinBox)
QT_FORWARD_DECLARE_CLASS(QDialogButtonBox)
QT_FORWARD_DECLARE_CLASS(QSettings)
class LoginDialog : public QDialog
{
    Q_OBJECT
public:
    LoginDialog(QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
    ~LoginDialog() override;
    void setUserStyle(int s = 0);
    QString userName() const;

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    QString encodeC(const QString &);
    QString decodeC(const QString &);

    QLabel *logoLabel_{nullptr},*logoSpitL_{nullptr},
    *logoTitleL_{nullptr},*userL_{nullptr},*pswL_{nullptr};
    QWidget *logoBackW_{nullptr};
    QLineEdit *userEdit_{nullptr},*pswEdit_{nullptr};
    QDialogButtonBox *btnBox_{nullptr};
    QPushButton *ptnSurce_{nullptr},*ptnCancell_{nullptr},*ptnMinsized_{nullptr},*ptnClose_{nullptr};
    bool autoLogin_ = false;

    QPoint startPos_;
    bool movieable_ = false;
    QSettings *config_;

private slots:
    void slotSureBtnClicked();
};

#endif // LOGINDIALOG_H
