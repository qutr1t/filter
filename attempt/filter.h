#ifndef FILTER_H
#define FILTER_H

#include <QImage>
#include <vector>
template<class T, size_t Rows, size_t Cols> using matrix = std::array<std::array<T, Cols>, Rows>;
class Image{
public:
    Image();
    Image(int width, int height, int depth);
    Image(int width, int heigth, int depth, float* buffer);
    virtual ~Image();
    Image Grayscale();
    std::vector<float>& getColorBuffer();
    void fromQImage(const QImage& qImage);

    QImage toQImage();
    //получение параметров изображжения
    void GetParams(int& width, int& height, int& depth);
    //попиксельное умножение на сверточное ядро
    void Multiply(int i, int j,int kernel[3][3],float rgb[3]);
    void Multiply2(int x, int y, int kernel[2][2], float rgb[3]);
    void SetBuffer(int pos,float rgb[3]);
     void magnitude(Image input, QImage convx, QImage convy);
    QImage convolution(int* kernel, const QImage& image);
    float GetBuffer(int pos, int channel);



protected:
    int _width;
    int _height;
    int _depth; //кол-во байт на запись пикселя
    std::vector<float> _buffer; //вектор с RGB составляющими каждого пикселя изображения
};
    void roberts(Image& input, Image& out_image);
void sobel(Image& input, Image& out_image); //обработка фильтром Собеля
void gray(Image& input, Image& out_image);
void prewitt(Image& input, Image& out_image);
    void gauss(Image& input, Image& out_image, int rad, float dev);

#endif // FILTER_H
