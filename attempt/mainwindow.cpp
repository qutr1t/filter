#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QPixmap>
#include <QImage>
#include <QtWidgets>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_grid.h>
//#include <bitset>



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

void MainWindow::on_pushButton_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Открытие файла"), "C://", tr("Images(*.png *.jpeg *.jpg *.bmp)"));
    if (QString::compare(filename,QString())!=0){   //на случай закрытия диалогового окна


        bool valid = curimage.load(filename);
        QTextStream out(stdout);
         //   out<<curimage.depth()<<endl;
        if (valid)
        {
            int w = ui->label->width();
            int h = ui->label->height();
            //масштабирование изображения
            curimage = curimage.scaledToWidth(w,Qt::SmoothTransformation);
            curimage = curimage.scaledToHeight(h,Qt::SmoothTransformation);
            QPixmap pix = QPixmap::fromImage(curimage);
            image.fromQImage(curimage);
            //ui->label->setPixmap(QPixmap::fromImage(image));
            ui->label->setPixmap(pix);
        }
    }
}

void MainWindow::on_Sobel_clicked()
{
    sobel(image,out_image);
    QImage qImage = out_image.toQImage();
    ui->label_2->setPixmap(QPixmap::fromImage(qImage));
    QTextStream out(stdout);
    //out<<qImage.depth();
}
void MainWindow::on_robert_clicked()
{
        QTextStream out(stdout);
    roberts(image,out_image);
    QImage qImage;
    qImage = out_image.toQImage();

    ui->label_2->setPixmap(QPixmap::fromImage(qImage));

    //out<<qImage.depth();
}


void MainWindow::on_Monochrome_clicked()
{
    QTextStream out(stdout);
    QImage convertedImage = curimage.convertToFormat(QImage::Format_Grayscale8);
    QPixmap pix = QPixmap::fromImage(convertedImage);
    ui->label->setPixmap(pix);
    image.fromQImage(convertedImage);


}

void MainWindow::on_bit16_clicked()
{
    QTextStream out(stdout);
    QImage convertedImage = curimage.convertToFormat(QImage::Format_RGB16);
    int width = curimage.width();
    int height = curimage.height();

    for (int y = 0; y < height; y++) {
        QRgb *srcPixel = (QRgb *)curimage.scanLine(y);
        quint16 *dstPixel = (quint16 *)convertedImage.scanLine(y);

        for (int x = 0; x < width; x++) {
            QRgb rgb = srcPixel[x];
            quint16 rgb565 = ((qRed(rgb) >> 3) << 11) |
                              ((qGreen(rgb) >> 2) << 5) |
                               (qBlue(rgb) >> 3);

            dstPixel[x] = rgb565;
        }
    }
    QPixmap pix = QPixmap::fromImage(convertedImage);
    ui->label->setPixmap(pix);
        //out<<convertedImage.depth();
    image.fromQImage(convertedImage);
}


void MainWindow::on_bit24_clicked()
{
    QTextStream out(stdout);
    QImage convertedImage = curimage.convertToFormat(QImage::Format_RGB888);
    QPixmap pix = QPixmap::fromImage(convertedImage);
    ui->label->setPixmap(pix);
    image.fromQImage(convertedImage);
}

void MainWindow::on_SaveButton_clicked()
{
            QTextStream out(stdout);
    QString filename = QFileDialog::getSaveFileName(this,tr("Открытие файла"), "C://", tr("PNG Files (*.png);;JPEG Files (*.jpg);;BMP Files (*.bmp)"));
    if (QString::compare(filename,QString())!=0){   //на случай закрытия диалогового окна
        if (!filename.isEmpty()) {
            QImage image = out_image.toQImage();
            image.save(filename);
        }
    }
}


void MainWindow::on_prewitt_clicked()
{
    prewitt(image,out_image);
    QImage qImage = out_image.toQImage();
    ui->label_2->setPixmap(QPixmap::fromImage(qImage));
    QTextStream out(stdout);
    //out<<qImage.depth();
}

void MainWindow::on_gauss_clicked()
{
    gauss(image,out_image,8,2);
    QImage qImage = out_image.toQImage();
    ui->label_2->setPixmap(QPixmap::fromImage(qImage));
    QTextStream out(stdout);
    //out<<qImage.depth();
}
void MainWindow::on_hist2_clicked()
{
        QTextStream out(stdout);
        QImage image = out_image.toQImage();

      // QwtPlot plot(this);
}

void MainWindow::on_pixel1_clicked()
{
    // Предположим, у вас есть объект QImage, называемый "image"
      QImage img =image.toQImage(); // инициализация образ

      // Преобразуем QImage в QPixmap
      QPixmap pixmap = QPixmap::fromImage(img);

      // Создаем QFileDialog для сохранения файла
      QFileDialog fileDialog(this);
      fileDialog.setFileMode(QFileDialog::AnyFile);
      fileDialog.setAcceptMode(QFileDialog::AcceptSave);
      fileDialog.setDefaultSuffix("png"); // расширение файла по умолчанию
      fileDialog.setNameFilters({"Файлы изображений (*.png *.jpg *.bmp)"}); // фильтры файлов

      // Показываем файловый диалог и получаем выбранный путь файла
      if (fileDialog.exec() == QFileDialog::Accepted) {
          QString filePath = fileDialog.selectedFiles().first();

          // сохраяем pixmap в выбранный путь файла
          if (!pixmap.save(filePath)) {
              QMessageBox::warning(this, "Ошибка", "Не удалось сохранить изображение");
          } else {
              QMessageBox::information(this, "Успех", "Изображение сохранено успешно");
          }
      }
}

void MainWindow::on_pixel2_clicked()
{
    // Предположим, у вас есть объект QImage, называемый "image"
      QImage img =out_image.toQImage(); // инициализация образа

      // Преобразуем QImage в QPixmap
      QPixmap pixmap = QPixmap::fromImage(img);

      // Создаем QFileDialog для сохранения файла
      QFileDialog fileDialog(this);
      fileDialog.setFileMode(QFileDialog::AnyFile);
      fileDialog.setAcceptMode(QFileDialog::AcceptSave);
      fileDialog.setDefaultSuffix("png"); // расширение файла по умолчанию
      fileDialog.setNameFilters({"Файлы изображений (*.bmp)"}); // фильтры файлов

      // Показываем файловый диалог и получаем выбранный путь файла
      if (fileDialog.exec() == QFileDialog::Accepted) {
          QString filePath = fileDialog.selectedFiles().first();

          // сохраяем pixmap в выбранный путь файла
          if (!pixmap.save(filePath)) {
              QMessageBox::warning(this, "Ошибка", "Не удалось сохранить изображение");
          } else {
              QMessageBox::information(this, "Успех", "Изображение сохранено успешно");
          }
      }
}
