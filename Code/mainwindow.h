#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QPixmap>
#include <QImage>
#include <QLabel>
#include <QVector>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    QVector<QString> getCodeMask();
    void setCodeMask(QVector<QString> a);
    void clearCodeMask();
    void DisplayImage(QImage &img, QString title);
    int w;
    int h;
    ~MainWindow();

private slots:
    void on_btn_randomkey_2_clicked();

    void on_bt_browse_2_clicked();

    void on_btn_display1_clicked();

    void on_btn_Load_2_clicked();

    void on_btn_encrypted_2_clicked();

    void on_bt_decrypted_2_clicked();

    void on_bt_loadfile_2_clicked();

    void on_btn_display1_2_clicked();

    void on_bt_loadmaskfile_clicked();

    void on_bt_reset_2_clicked();

private:
    Ui::MainWindow *ui;
    QVector<QString> code_mask;

};

#endif // MAINWINDOW_H
