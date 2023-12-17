#include "DrawObstructions.h"
void DrawPixel(QPainter* Pix, int x, int y) {
    Pix->drawPoint(x + 1, y + 1);
    Pix->drawPoint(x + 1, y - 1);
    Pix->drawPoint(x - 1, y + 1);
    Pix->drawPoint(x - 1, y - 1);
    Pix->drawPoint(x, y);
    Pix->drawPoint(x, y - 1);
    Pix->drawPoint(x, y + 1);
    Pix->drawPoint(x - 1, y);
    Pix->drawPoint(x + 1, y);
}
int* N_of_obstructions(QPainter* Pix, QPixmap* Map, int *Number, int x0, int x1, int y0, int y1) {
    QImage image = Map->toImage();
    int deltax = abs(x1 - x0);
    int deltay = abs(y1 - y0);
    int signx = x0 < x1 ? 1 : -1;
    int signy = y0 < y1 ? 1 : -1;
    int error = deltax - deltay;

    int Xless=-50;
    int Yless=-50;

    QRgb color1=image.pixel(x1, y1);
    int red1 = qRed(color1);
    int green1 = qGreen(color1);
    int blue1 = qBlue(color1);
    if (red1==34 && green1==34 && blue1==34) { //если бетон
        Number[0]+=1;
        Xless=x1;
        Yless=y1;
    }
    if (red1==161 && green1==104 && blue1==7) { //дерево
        Number[1]+=1;
        Xless=x1;
        Yless=y1;
    }
    if (red1==77 && green1==101 && blue1==110) { //Стеклопакет
        Number[2]+=1;
        Xless=x1;
        Yless=y1;
    }
    if (red1==190 && green1==208 && blue1==215) { //IRR стекло
        Number[3]+=1;
        Xless=x1;
        Yless=y1;
    }
    while (x0 != x1 || y0 != y1) {

        QRgb color2=image.pixel(x0, y0);
        int red2 = qRed(color2);
        int green2 = qGreen(color2);
        int blue2 = qBlue(color2);

        if (red2==34 && green2==34 && blue2==34) { //если бетон
            if ((Xless==-50) && (Yless==-50)) {
                Number[0]+=1;
                Xless=x0;
                Yless=y0;
            }
            if (((x0-1==Xless) || (x0+1==Xless) || (x0==Xless)) && ((y0==Yless) || (y0-1==Yless) || (y0+1==Yless))) {
                Xless=x0;
                Yless=y0;
            }
            else {
                Number[0]+=1;
            }
        }
        if (red2==161 && green2==104 && blue2==7) { //дерево
            if ((Xless==-50) && (Yless==-50)) {
                Number[1]+=1;
                Xless=x0;
                Yless=y0;
            }
            if (((x0-1==Xless) || (x0+1==Xless) || (x0==Xless)) && ((y0==Yless) || (y0-1==Yless) || (y0+1==Yless))) {
                Xless=x0;
                Yless=y0;
            }
            else {
                Number[1]+=1;
            }
        }
        if (red2==77 && green2==101 && blue2==110) { //стеклопакет
            if ((Xless==-50) && (Yless==-50)) {
                Number[2]+=1;
                Xless=x0;
                Yless=y0;
            }
            if (((x0-1==Xless) || (x0+1==Xless) || (x0==Xless)) && ((y0==Yless) || (y0-1==Yless) || (y0+1==Yless))) {
                Xless=x0;
                Yless=y0;
            }
            else {
                Number[2]+=1;
            }
        }
        if (red2==190 && green2==208 && blue2==215) { //IRR стекло
            if ((Xless==-50) && (Yless==-50)) {
                Number[3]+=1;
                Xless=x0;
                Yless=y0;
            }
            if (((x0-1==Xless) || (x0+1==Xless) || (x0==Xless)) && ((y0==Yless) || (y0-1==Yless) || (y0+1==Yless))) {
                Xless=x0;
                Yless=y0;
            }
            else {
                Number[3]+=1;
            }
        }

        int error2 = error*2;
        if (error2 > -deltay) {
            error -= deltay;
            x0 += signx;
        }
        if (error2 < deltax) {
            error += deltax;
            y0 += signy;
        }
    }
    return Number;
}
