#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <filter.h>
#include <QMainWindow>

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
    void on_load_image_clicked();

    void on_bit24_clicked();

    void on_bit16_clicked();

    void on_bit8_clicked();

    void on_roberts_clicked();

    void on_sobel_clicked();

    void on_prewitt_clicked();

    void on_gaussian_clicked();

    void on_pixel1_clicked();

    void on_hist1_clicked();

    void on_pixel2_clicked();

    void on_hist2_clicked();

    void on_save_image_clicked();

private:
    Ui::MainWindow *ui;
    Image input;
    Image output;
    QImage cur_img;
};

#endif // MAINWINDOW_H
