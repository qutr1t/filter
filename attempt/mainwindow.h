#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMessageBox>
#include <QMainWindow>
#include <QFileDialog>

#include "filter.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_Sobel_clicked();

    void on_Monochrome_clicked();

    void on_bit16_clicked();

    void on_bit24_clicked();

    void on_SaveButton_clicked();

    void on_robert_clicked();

    void on_prewitt_clicked();

    void on_gauss_clicked();

    void on_hist2_clicked();

    void on_pixel1_clicked();

    void on_pixel2_clicked();

private:
    Ui::MainWindow *ui;
    QImage curimage;
    Image image;
    Image out_image;
};

#endif // MAINWINDOW_H
