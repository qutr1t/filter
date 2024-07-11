#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QPixmap>
#include "filter.h"


Image::Image()
    : _width(0), _height(0), _depth(0)
{
}

Image::Image(Image &s){
    _width=s._width;
    _height=s._height;
    _depth=s._depth;
    _buffer=s._buffer;
}

Image::~Image()
{
}

void Image::fromQImage(QImage& qImage){
    if( qImage.isNull()){
        return;
    }
     QTextStream out(stdout);
    _buffer.clear();
    _width = qImage.width();    //ширина изображения
    _height = qImage.height();  //высота изображения
    _depth = qImage.depth();    //количество бит на пиксель

    _buffer.resize(uint(_width*_height*3),0);
    uint pos =0; //позиция компоненты цвета пикселя
    for( int y = 0; y < _height; y++ ){
        for( int x = 0; x < _width; x++ ){
            QColor color = qImage.pixel(x,y);
            _buffer[pos++] = color.red();
            _buffer[pos++] = color.green();
            _buffer[pos++] = color.blue();
       }
    }
    out<<"Depth: "<<qImage.depth()<<"\t"<<_buffer.size()<<endl;

}

QImage Image::toQImage(){

    QImage::Format format=QImage::Format_Invalid;
    if (_depth == 8) {
        format = QImage::Format_Grayscale8;
    } else if (_depth == 16) {
        format = QImage::Format_RGB16;
    } else if (_depth == 24) {
        format = QImage::Format_RGB888;
    }

    QImage qImage(_width, _height, format);

    uint pos = 0;
    for( int y = 0; y < _height; y++ ){
        for( int x = 0; x < _width; x++ ){
            QColor color;
            color.setRed(_buffer[pos++]);
            color.setGreen(_buffer[pos++]);
            color.setBlue(_buffer[pos++]);

            qImage.setPixelColor(x,y,color);
        }
    }
    return qImage;
}

Image Image::Grayscale(){
    Image inst(*this);
    if (_depth==8){
        return inst;
    }
        QTextStream out(stdout);

        uint pos=0;
        for( int y = 0; y < inst._height; y++ ){
            for( int x = 0; x <inst._width; x++ ){
                uint k = uint(y*inst._width*3+x*3);
                inst._buffer[pos]=int(0.299*inst._buffer[k] +0.587*inst._buffer[k+1]+0.114*inst._buffer[k+2]);
                inst._buffer[pos+1]=inst._buffer[pos];
                inst._buffer[pos+2]=inst._buffer[pos];
                //out<<"HIIII "<<_buffer[pos]<<" "<<_buffer[pos+1]<<" "<<_buffer[pos+2]<<endl;
                pos+=3;
            }
        }
    return inst;
}

Image Image::Sobel(){
    int kernelx[3][3]={
        {-1,0,1},
        {-2,0,2},
        {-1,0,1}
    };
    int kernely[3][3]={
        {1,2,1},
        {0,0,0},
        {-1,-2,-1}
    };
    Image out(*this);

    uint pos=0;
    out = this->Grayscale();
    int width=out._width;
    int height=out._height;

    for(int y = 1; y<height-1;y++){
        for(int x = 1; x<width-1; x++){
            int Gx = 0;
            for(int i=0;i<int(sizeof kernelx/sizeof kernelx[0]);i++){
                for(int j=0;j<int(sizeof kernelx[0]/sizeof(int));j++){
                    uint k = uint((y+i-1)*width*3+(x+j-1)*3);
                    Gx+=kernelx[i][j]*this->_buffer[k];
                }
            }
            int Gy = 0;
            for(int i=0;i<int(sizeof kernely/sizeof kernely[0]);i++){
                for(int j=0;j<int(sizeof kernely[0]/sizeof(int));j++){
                    uint k = uint((y+i-1)*width*3+(x+j-1)*3);
                    Gy+=kernely[i][j]*this->_buffer[k];
                }
            }
            int G= int(sqrt(pow(Gx,2)+pow(Gy,2)));
            if (G>255)
                G=255;
            else if (G<0)
                G=0;
            pos =uint (y*width*3+x*3);
            for(uint t =0; t<3;t++){
                out._buffer[pos+t]=G;
            }
        }
    }
    return out;
}

Image Image::Roberts(){
    int kernelx[2][2]={
        {1,0},
        {0,-1}
    };
    int kernely[2][2]={
        {0,1},
        {-1,0}
    };
    Image out(*this);

    uint pos=0;
    out = this->Grayscale();
    int width=out._width;
    int height=out._height;

    for(int y = 0; y<height-1;y++){
        for(int x = 0; x<width-1; x++){
            int Gx = 0;
            for(int i=0;i<int(sizeof kernelx/sizeof kernelx[0]);i++){
                for(int j=0;j<int(sizeof kernelx[0]/sizeof(int));j++){
                    uint k = uint((y+i)*width*3+(x+j)*3);
                    Gx+=kernelx[i][j]*this->_buffer[k];
                }
            }
            int Gy = 0;
            for(int i=0;i<int(sizeof kernely/sizeof kernely[0]);i++){
                for(int j=0;j<int(sizeof kernely[0]/sizeof(int));j++){
                    uint k = uint((y+i)*width*3+(x+j)*3);
                    Gy+=kernely[i][j]*this->_buffer[k];
                }
            }
            int G= int(sqrt(pow(Gx,2)+pow(Gy,2)));
            if (G>255)
                G=255;
            else if (G<0)
                G=0;
            pos =uint (y*width*3+x*3);
            for(uint t =0; t<3;t++){
                out._buffer[pos+t]=G;
            }
        }
    }
    return out;
}

Image Image::Prewitt(){
    int kernelx[3][3]={
        {-1,0,1},
        {-1,0,1},
        {-1,0,1}
    };
    int kernely[3][3]={
        {1,1,1},
        {0,0,0},
        {-1,-1,-1}
    };
    Image out(*this);

    uint pos=0;
    out = this->Grayscale();
    int width=out._width;
    int height=out._height;

    for(int y = 1; y<height-1;y++){
        for(int x = 1; x<width-1; x++){
            int Gx = 0;
            for(int i=0;i<int(sizeof kernelx/sizeof kernelx[0]);i++){
                for(int j=0;j<int(sizeof kernelx[0]/sizeof(int));j++){
                    uint k = uint((y+i-1)*width*3+(x+j-1)*3);
                    Gx+=kernelx[i][j]*this->_buffer[k];
                }
            }
            int Gy = 0;
            for(int i=0;i<int(sizeof kernely/sizeof kernely[0]);i++){
                for(int j=0;j<int(sizeof kernely[0]/sizeof(int));j++){
                    uint k = uint((y+i-1)*width*3+(x+j-1)*3);
                    Gy+=kernely[i][j]*this->_buffer[k];
                }
            }
            int G= int(sqrt(pow(Gx,2)+pow(Gy,2)));
            if (G>255)
                G=255;
            else if (G<0)
                G=0;
            pos =uint (y*width*3+x*3);
            for(uint t =0; t<3;t++){
                out._buffer[pos+t]=G;
            }
        }
    }
    return out;
}

Image Image::Gauss(){
        Image out(*this);
        out=this->Grayscale();
        int width=out._width;
        int height=out._height;
        //int kern[3][3]={
        //    {1, 2,  1},
        //    {2, 4,  2},
        //    {1, 2,  1}
        //};
        int kernMid[5][5]={
            {1, 4,  7,  4,  1},
            {4, 16, 26, 16, 4},
            {7, 26, 41, 26, 7},
            {4, 16, 26, 16, 4},
            {1, 4,  7,  4,  1}
        };
        uint pos =0;
        for(int y = 2; y<height-2;y++){             //+1 и -1 или +2 и -2
            for(int x = 2; x<width-2;x++){          //+1 и -1 или +2 и -2
                pos =uint (y*width*3+x*3);
                for(uint t =0; t<3;t++){
                    int G=0;
                    for(int i=0;i<5;i++){           //3 или 5
                        for(int j=0;j<5;j++){       //3 или 5
                            uint k = uint((y+i-2)*width*3+(x+j-2)*3);   //-1 или -2
                            G+=this->_buffer[k+t]*kernMid[i][j];    //kern или kernMid
                        }
                    }
                    out._buffer[pos+t]=int(G/273);   //16 или 273
                }
            }
        }
        return out;
}
