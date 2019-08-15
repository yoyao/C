#ifndef MAINHEADER_H
#define MAINHEADER_H

#include <QWidget>

namespace Ui {
class MainHeader;
}

class MainHeader : public QWidget
{
    Q_OBJECT

public:
    explicit MainHeader(QWidget *parent = 0);
    ~MainHeader();
    void SetUserName(QString &name);
signals:
    void SigGetFileList(int index);
    void SigToUpload(int index);
    void SigGetMyShare(int index);
    void SigGetShares(int index);
    void SigGetTransRecord(int index);
protected:
    virtual void paintEvent(QPaintEvent *event);


private slots:
    void on_tbtn_exit_clicked();

    void on_tbtn_filelist_clicked();

    void on_tbtn_upload_clicked();

    void on_tbtn_myshare_clicked();

    void on_tbtn_share_list_clicked();

    void on_tbtn_trans_clicked();

private:
    Ui::MainHeader *ui;
    QWidget *m_parent;
};

#endif // MAINHEADER_H
