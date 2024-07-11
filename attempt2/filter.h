#ifndef FILTER_H
#define FILTER_H

#include <QImage>
#include <vector>
template<class T, size_t Rows, size_t Cols> using matrix = std::array<std::array<T, Cols>, Rows>;
class Image{
private:
    int _width;
    int _height;
    int _depth; //кол-во байт на запись пикселя
    std::vector<int> _buffer; //вектор с RGB составляющими каждого пикселя изображения
public:
    Image();
    Image(Image &t);
    ~Image();

    Image Grayscale();

    Image Sobel();
    void fromQImage(QImage& qImage);
    QImage toQImage();
    Image Roberts();
    Image Prewitt();
    Image Gauss();
    //QImage toQImage();
    //получение параметров изображжения
    //void GetParams(int& width, int& height, int& depth);
    //попиксельное умножение на сверточное ядро
    //void Multiply(int i, int j,int kernel[3][3],float rgb[3]);
    //void Multiply2(int x, int y, int kernel[2][2], float rgb[3]);
    //void SetBuffer(int pos,float rgb[3]);
     //void magnitude(Image input, QImage convx, QImage convy);
    //QImage convolution(int* kernel, const QImage& image);
    //float GetBuffer(int pos, int channel);
};
#endif // FILTER_H
