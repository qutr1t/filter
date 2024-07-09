#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QPixmap>
#include "filter.h"

#include <QtMath>
#include <math.h>


Image::Image()
    : _width(0), _height(0), _depth(0)
{
}


Image::Image(int width, int height, int depth)
    : _width(width)
    , _height(height)
    , _depth(depth)
    , _buffer(_width * _height * _depth, 0.0f)
{
}

/*Image::Image(int width, int height, int depth, float* buffer)
    : _width(width)
    , _height(height)
    , _depth(depth)
    , _buffer(buffer, buffer + _width * _height * _depth)
{
}*/
Image::~Image()
{
}

void Image::fromQImage(const QImage &qImage)
{
        QTextStream out(stdout);
    if( !qImage.isNull() )
    {
        _buffer.clear();

        _width = qImage.width();
        _height = qImage.height();
        _depth = qImage.depth()/8;
        if (_depth >=4) _depth = 3;
        // out<<qImage.depth()<<endl;
        int pos = 0;
        /*if (_depth==2){
             _buffer.resize(_width*_height*3, 0);
             for( int y = 0; y < _height; y++ ){
                 for( int x = 0; x < _width; x++ ){
                     quint16 pixel = *reinterpret_cast<const quint16*>(qImage.scanLine(y) + x * 2);

                     // Extract the red, green, and blue components from the pixel value
                     quint8 red = (pixel >> 11) & 0x1F;
                     quint8 green = (pixel >> 5) & 0x3F;
                     quint8 blue = pixel & 0x1F;

                     // Calculate the index of the first byte of this pixel in the buffer
                     int index = (y * _width * 3) + (x * 3);

                     // Write the red, green, and blue components to the buffer
                     _buffer[index] = red;
                     _buffer[index + 1] = green;
                     _buffer[index + 2] = blue;

                 }
             }
         }else{*/
             _buffer.resize(_width*_height*_depth, 0);
             for( int y = 0; y < _height; y++ ){
                 for( int x = 0; x < _width; x++ ){
                     QColor color = qImage.pixel(x,y);
                     _buffer[pos++] = color.red()/255.f;
                     if( _depth >1  )
                     {
                         _buffer[pos++] = color.green()/255.f;

                     }
                     if( _depth >2  )
                     {

                                _buffer[pos++] = color.blue()/255.f;

                     }

                 }
             }
         //}

    }

   // out<<_buffer.size()<<endl;
}
unsigned char fix(float value){
    value=(value<0)?-value:value;
    value=(value>1)?1:value;
    return (unsigned char) (255*value);
}
QImage Image::toQImage()
{
    QTextStream out(stdout);
    QImage::Format format;
    int depth =_depth*8;
    if (depth == 8) {
        format = QImage::Format_Grayscale8;
    } else if (depth == 16) {
        format = QImage::Format_RGB16;
    } else if (depth == 24) {
        format = QImage::Format_RGB888;
    }

    QImage qImage(_width, _height, format);

    if( !_buffer.empty() )
    {
        int pos = 0;
        /*if (depth == 16)  {
            for (int y = 0; y < _height; ++y) {
                for (int x = 0; x < _width; ++x) {
                    // Calculate the index of the first byte of this pixel in the buffer
                    int index = (y * _width * 3) + (x * 3);

                    // Extract the red, green, and blue components from the buffer
                    quint8 red = _buffer[index];
                    quint8 green = _buffer[index + 1];
                    quint8 blue = _buffer[index + 2];

                    // Combine the red, green, and blue components into a 16-bit pixel value
                    quint16 pixel = (red << 11) | (green << 5) | blue;

                    // Write the pixel value to the image at (x, y)
                    *reinterpret_cast<quint16*>(qImage.scanLine(y) + x * 2) = pixel;
                }
            }
        }else{*/
            for( int y = 0; y < _height; y++ ){
                for( int x = 0; x < _width; x++ ){
                    float value = fix(_buffer[pos++]);
                    QColor color(value, value, value);
                    if( _depth > 1 ){
                        color.setGreen(fix(_buffer[pos++]));
                    }
                    if( _depth == 3 ){
                        color.setBlue(fix(_buffer[pos++]));
                    }
                    qImage.setPixelColor(x,y,color);
                }
            }
        //}
    }
       // out<<qImage.depth();
    return qImage;
}

std::vector<float>& Image::getColorBuffer()
{
    return _buffer;
}
void Image::GetParams(int& w, int& h, int& d){
    w=_width;
    h=_height;
    d=_depth;
}

void gray(Image& input, Image& output){
    output=input;
    output=output.Grayscale();
}

Image Image::Grayscale(){
    if (_depth==1) return *this;
         QTextStream out(stdout);
        Image outImage(_width,_height, 1);
        std::vector<float>& dst = outImage.getColorBuffer();
        for( int y = 0; y < _height; y++ ){
            for( int x = 0; x < _width; x++ ){
                int k = y*_width*_depth+x*_depth;
                dst[y*_width+x]=0.299*_buffer[k] +0.587*_buffer[k+1]+0.114*_buffer[k+2];
            }
        }
    return outImage;
}



void sobel(Image& input, Image& output){
    output=Image(input);
    output=output.Grayscale();
    int width,height,depth;
    input.GetParams(width,height,depth);
    int kernelx[3][3];
    kernelx[0][0]=-1;kernelx[0][1]=0;kernelx[0][2]=1;
    kernelx[1][0]=-2;kernelx[1][1]=0;kernelx[1][2]=2;
    kernelx[2][0]=-1;kernelx[2][1]=0;kernelx[2][2]=1;
    int kernely[3][3];
    kernely[0][0]=-1;kernely[0][1]=-2;kernely[0][2]=-1;
    kernely[1][0]=0;kernely[1][1]=0;kernely[1][2]=0;
    kernely[2][0]=1;kernely[2][1]=2;kernely[2][2]=1;
    for( int y = 1; y <height-1; y++ ){
            for( int x = 1; x < width-1; x++ ){

                int pos=(y*width+x);
                float rgbx[3];
                input.Multiply(x,y,kernelx,rgbx);

                float rgby[3];
                input.Multiply(x,y,kernely,rgby);
                float rgb[3];
                rgb[0]=sqrt(pow(rgbx[0],2)+pow(rgby[0],2));
                rgb[1]=sqrt(pow(rgbx[1],2)+pow(rgby[1],2));
                rgb[2]=sqrt(pow(rgbx[2],2)+pow(rgby[2],2));
                output.SetBuffer(pos,rgb);
            }
    }
}

void Image::Multiply(int i, int j,int kernel[3][3],float rgb[3]){
    rgb[0]=rgb[1]=rgb[2]=0;
    int maxpos=_width*_height*_depth;

    for (int x=0;x<3;++x){
        for (int y=0;y<3;++y){
            int index=((j+y-1)*_width*_depth+(i+x-1)*_depth);
            if (index<0||index>=maxpos) continue;
            rgb[0]+=kernel[x][y]*_buffer[index];     

            if(_depth>1) {
            rgb[1]+=kernel[x][y]*_buffer[index+1];
            rgb[2]+=kernel[x][y]*_buffer[index+2];
            }
        }
    }
if(kernel[1][1]==0){
        rgb[0]/=4.0;
    }


else if(kernel[1][1]==4){
    rgb[0]/=16.0;
    rgb[1]/=16.0;
    rgb[2]/=16.0;
}
}

void Image::SetBuffer(int pos,float rgb[3]){
    _buffer[pos]=rgb[0];
    if(_depth==1) return;
    _buffer[pos+1]=rgb[1];
    _buffer[pos+2]=rgb[2];
}

void Image::Multiply2(int i, int j, int kernel[2][2], float rgb[3]) {
    rgb[0]=rgb[1]=rgb[2]=0;
    int maxpos=_width*_height*_depth;

    for (int x=0;x<2;++x){
        for (int y=0;y<2;++y){
            int index=((j+y-1)*_width*_depth+(i+x-1)*_depth);
            if (index<0||index>=maxpos) continue;
            rgb[0]+=kernel[x][y]*_buffer[index];

            if(_depth>1) {
            rgb[1]+=kernel[x][y]*_buffer[index+1];
            rgb[2]+=kernel[x][y]*_buffer[index+2];
            }
        }
    }
}
void roberts(Image& input, Image& output){

        output=Image(input);
        output=output.Grayscale();
        int width,height,depth;
        input.GetParams(width,height,depth);
        int kernelx[2][2];
        kernelx[0][0]=0;kernelx[0][1]=-1;
        kernelx[1][0]=1;kernelx[1][1]=0;
        int kernely[2][2];
        kernely[0][0]=-1;kernely[0][1]=0;
        kernely[1][0]=0;kernely[1][1]=1;
        for( int y = 0; y <height-1; y++ ){
                for( int x = 0; x < width-1; x++ ){

                    int pos=(y*width+x);
                    float rgbx[3];
                    input.Multiply2(x,y,kernelx,rgbx);

                    float rgby[3];
                    input.Multiply2(x,y,kernely,rgby);
                    float rgb[3];
                    rgb[0]=sqrt(pow(rgbx[0],2)+pow(rgby[0],2));
                    rgb[1]=sqrt(pow(rgbx[1],2)+pow(rgby[1],2));
                    rgb[2]=sqrt(pow(rgbx[2],2)+pow(rgby[2],2));
                    output.SetBuffer(pos,rgb);
                }
        }
}

void prewitt(Image& input, Image& output){

        output=Image(input);
        output=output.Grayscale();
        int width,height,depth;
        input.GetParams(width,height,depth);
        int kernelx[3][3];
        kernelx[0][0]=-1;kernelx[0][1]=0;kernelx[0][2]=1;
        kernelx[1][0]=-1;kernelx[1][1]=0;kernelx[1][2]=1;
        kernelx[2][0]=-1;kernelx[2][1]=0;kernelx[2][2]=1;
        int kernely[3][3];
        kernely[0][0]=-1;kernely[0][1]=-1;kernely[0][2]=-1;
        kernely[1][0]=0;kernely[1][1]=0;kernely[1][2]=0;
        kernely[2][0]=1;kernely[2][1]=1;kernely[2][2]=1;
        for( int y = 1; y <height-1; y++ ){
                for( int x = 1; x < width-1; x++ ){

                    int pos=(y*width+x);
                    float rgbx[3];
                    input.Multiply(x,y,kernelx,rgbx);

                    float rgby[3];
                    input.Multiply(x,y,kernely,rgby);
                    float rgb[3];
                    rgb[0]=sqrt(pow(rgbx[0],2)+pow(rgby[0],2));
                    rgb[1]=sqrt(pow(rgbx[1],2)+pow(rgby[1],2));
                    rgb[2]=sqrt(pow(rgbx[2],2)+pow(rgby[2],2));
                    output.SetBuffer(pos,rgb);
                }
        }
}

float Image::GetBuffer(int pos, int channel) {
        if (channel < 0 || channel >= _depth) {
            throw std::runtime_error("Ошибка");
        }
        return _buffer[pos * _depth + channel];
}

void gauss(Image& input, Image& output,int radius, float sigma) {
        output = Image(input);
        output = output.Grayscale();
        int width, height, depth;
        input.GetParams(width, height, depth);

        int kernelSize = 2 * radius + 1;
        float kernel[kernelSize][kernelSize];

        float sum = 0.0f;
        for (int y = -radius; y <= radius; y++) {
            for (int x = -radius; x <= radius; x++) {
                float g = exp(-(x * x + y * y) / (2 * sigma * sigma));
                kernel[y + radius][x + radius] = g;
                sum += g;
            }
        }

        for (int y = 0; y < kernelSize; y++) {
            for (int x = 0; x < kernelSize; x++) {
                 kernel[y][x] /= sum;
            }
        }

        for (int y = radius; y < height - radius; y++) {
            for (int x = radius; x < width - radius; x++) {
                int pos = (y * width + x);
                float rgb[3] = {0, 0, 0};

                for (int ky = -radius; ky <= radius; ky++) {
                    for (int kx = -radius; kx <= radius; kx++) {
                        int sx = x + kx;
                        int sy = y + ky;
                        int spos = (sy * width + sx);
                        float weight = kernel[ky + radius][kx + radius];

                        rgb[0] += input.GetBuffer(spos, 0) * weight;
                        if (depth>1){
                            rgb[1] += input.GetBuffer(spos, 1) * weight;
                        }
                        if (depth == 3){
                            rgb[2] += input.GetBuffer(spos, 2) * weight;
                        }
                    }
                }
                output.SetBuffer(pos, rgb);
            }
        }
    }
