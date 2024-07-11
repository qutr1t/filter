#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_load_image_clicked()
{
    QTextStream out(stdout);
    QString filename = QFileDialog::getOpenFileName(this, tr("Открытие файла"), "C://", tr("Images(*.png *.jpeg *.jpg *.bmp)"));
    if (QString::compare(filename,QString())!=0){   //если диалоговое окно не было закрыто

        bool valid = cur_img.load(filename);
        if (valid){ //если загрузка прошла успешно
            int w= ui->in_image->width();   //находим ширину label
            int h= ui->in_image->height();  //находим длину label
            out<<"Width: "<< w<<endl<<"Height: "<<h<<endl;
            cur_img=cur_img.scaled(w,h,Qt::KeepAspectRatio,Qt::SmoothTransformation);   //масштабирование изображения с учетом пропорций
            cur_img = cur_img.convertToFormat(QImage::Format_RGB888);
            ui->in_image->setPixmap(QPixmap::fromImage(cur_img));   //отображение изображения в label
            out<<"Depth: "<<cur_img.depth()<<endl;
            input.fromQImage(cur_img);
        }
    }
}

void MainWindow::on_bit24_clicked(){
    QTextStream out(stdout);
    QImage convertedImage = cur_img.convertToFormat(QImage::Format_RGB888);
    QPixmap pix = QPixmap::fromImage(convertedImage);
    ui->in_image->setPixmap(pix);
    out<<"Depth: "<<convertedImage.depth()<<endl;
    input.fromQImage(convertedImage);
}

void MainWindow::on_bit16_clicked(){
    QTextStream out(stdout);
    QImage convertedImage = cur_img.convertToFormat(QImage::Format_RGB16);
    QPixmap pix = QPixmap::fromImage(convertedImage);
    ui->in_image->setPixmap(pix);
    out<<"Depth: "<<convertedImage.depth()<<endl;
    input.fromQImage(convertedImage);
}

void MainWindow::on_bit8_clicked(){
    QTextStream out(stdout);
    QImage convertedImage = cur_img.convertToFormat(QImage::Format_Grayscale8);
    QPixmap pix = QPixmap::fromImage(convertedImage);
    ui->in_image->setPixmap(pix);
    out<<"Depth: "<<convertedImage.depth()<<endl;
    input.fromQImage(convertedImage);
}

void MainWindow::on_roberts_clicked()
{
    output=input.Roberts();
    QImage outpu = output.toQImage();
    ui->out_image->setPixmap(QPixmap::fromImage(outpu));
}

void MainWindow::on_sobel_clicked()
{
    output=input.Sobel();

    QImage outpu = output.toQImage();
    ui->out_image->setPixmap(QPixmap::fromImage(outpu));
}

void MainWindow::on_prewitt_clicked()
{
    output=input.Prewitt();
    QImage outpu = output.toQImage();
    ui->out_image->setPixmap(QPixmap::fromImage(outpu));
}

void MainWindow::on_gaussian_clicked()
{
    output=input.Gauss();
    QImage outpu = output.toQImage();
    ui->out_image->setPixmap(QPixmap::fromImage(outpu));
}

void MainWindow::on_pixel1_clicked()
{

}

void MainWindow::on_hist1_clicked()
{

}

void MainWindow::on_pixel2_clicked()
{

}

void MainWindow::on_hist2_clicked()
{

}

void MainWindow::on_save_image_clicked()
{
    QTextStream out(stdout);
    QString filename = QFileDialog::getSaveFileName(this,tr("Открытие файла"), "C://", tr("PNG Files (*.png);;JPEG Files (*.jpg);;BMP Files (*.bmp)"));
    if (QString::compare(filename,QString())!=0){   //на случай закрытия диалогового окна
        if (!filename.isEmpty()) {
            QImage image = output.toQImage();
            image.save(filename);
        }
    }
}
