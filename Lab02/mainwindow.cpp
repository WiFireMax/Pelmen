#include "mainwindow.h"
#include <QtWidgets>
#include <QPixmap>
#include <QPainter>
#include <time.h>
float PL(float f, float d){
    return 28 + 22 * log10(d) + 20*log10(f);
}
int N_of_obstructions(QPainter* Pix, QPixmap* Map, int x0, int x1, int y0, int y1) {
    int Number=0;
    QImage image = Map->toImage();
    int deltax = abs(x1 - x0);
    int deltay = abs(y1 - y0);
    int signx = x0 < x1 ? 1 : -1;
    int signy = y0 < y1 ? 1 : -1;
    int error = deltax - deltay;
    QRgb color1=image.pixel(x1, y1);
    int red1 = qRed(color1);
    int green1 = qGreen(color1);
    int blue1 = qBlue(color1);
    if (red1==34 && green1==34 && blue1==34) {
        Number+=1;
    }
    while (x0 != x1 || y0 != y1) {

        QRgb color=image.pixel(x0, y0);
        int red = qRed(color);
        int green = qGreen(color);
        int blue = qBlue(color);

        if (red==34 && green==34 && blue==34) {
            Number+=1;
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
struct Obstruction { //препятствия
    bool irrgrass; //IRR стекло
    bool doublegrass; //стеклопакет
    bool concrete; //бетон
    bool tree; //дерево
    float Decay(float f) {
        if (irrgrass == true) {
        return 23+0.3*f;
        }
        if (doublegrass == true) {
        return 2+0.2*f;
        }
        if (concrete == true) {
        return 5+4*f;
        }
        if (tree == true) {
        return 4.85+0.12*f;
        }
        else {
        return 0;
        }
    }
    int line(QPainter* Pix, int x0, int x1, int y0, int y1) { //линия
        int deltax = abs(x1 - x0);
        int deltay = abs(y1 - y0);
        int signx = x0 < x1 ? 1 : -1;
        int signy = y0 < y1 ? 1 : -1;
        int error = deltax - deltay;
        Pix->setPen(QColor(34, 34, 34, 255));
        Pix->drawPoint(x1, y1);
        while (x0 != x1 || y0 != y1) {
            Pix->drawPoint(x0, y0);
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
        return 0;
    }
    int circle(QPainter* Pix, int X1, int Y1, int R) {
        int x = 0;
        int y = R;
        int delta = 1 - 2 * R;
        int error = 0;
        while (y >= x) {
            Pix->setPen(QColor(34,34,34,255));
            Pix->drawPoint(X1 + x, Y1 + y);
            Pix->drawPoint(X1 + x, Y1 - y);
            Pix->drawPoint(X1 - x, Y1 + y);
            Pix->drawPoint(X1 - x, Y1 - y);
            Pix->drawPoint(X1 + y, Y1 + x);
            Pix->drawPoint(X1 + y, Y1 - x);
            Pix->drawPoint(X1 - y, Y1 + x);
            Pix->drawPoint(X1 - y, Y1 - x);
            error = 2 * (delta + y) - 1;
                if ((delta < 0) && (error <= 0)) {
                    delta += 2 * ++x + 1;
                    continue;
                    }
                if ((delta > 0) && (error > 0)) {
                    delta -= 2 * --y + 1;
                    continue;
                }
            delta += 2 * (++x - --y);
        }
        return 0;
    }
    int square(QPainter* Pix, int x0, int x1, int y0, int y1) {
        Pix->setPen(QColor(34,34,34,255));
        if (x0<x1) {
            for (int x=x0; x<x1; x++) {
                Pix->drawPoint(x, y0);
                Pix->drawPoint(x, y1);
                }
            }
        if (x1<x0) {
            for (int x=x1; x<x0; x++) {
                    Pix->drawPoint(x, y0);
                    Pix->drawPoint(x, y1);
            }
        }
        if (y0<y1) {
            for (int y=y0; y<y1; y++) {
                    Pix->drawPoint(x0, y);
                    Pix->drawPoint(x1, y);
                }
            }
        if (y1<y0) {
            for (int y=y1; y<y0; y++) {
                    Pix->drawPoint(x0, y);
                    Pix->drawPoint(x1, y);
                }
            }
        return 0;
    }
};
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QGraphicsScene* scene = new QGraphicsScene();
    int maxX=1000, maxY=1000;
    int onePixDistance = 30; //m
    int TxPower = 23;
    int antGain = 12;
    float freq=2.5;
    srand(time(NULL)); //установка точки 5G New Radio
    int cellPosX =rand()%999;
    int cellPosY=rand()%999;
    QPixmap map(maxX, maxY); // создаем карту для рисования
    QPainter p(&map);

    struct Obstruction Stone;
    Stone.concrete = true;
    Stone.circle(&p, 700, 700, 200);

    struct Obstruction Fence;
    Fence.tree = true;
    Fence.line(&p, 200, 700, 800, 600);

    struct Obstruction Office;
    Office.doublegrass = true;
    Office.square(&p, 200, 500, 100, 400);

    QImage image = map.toImage();

    for(int i = 0; i < maxX; i++){
        for(int j = 0; j < maxY; j++){
            float distance = 0;
            distance =sqrt (pow(abs(cellPosX - i), 2) + pow(abs(cellPosY-j), 2));
            distance *= onePixDistance;
            float sigPower = TxPower + antGain - PL(freq, distance);
            int NumObs=N_of_obstructions(&p, &map, cellPosX, i, cellPosY, j);
            if (NumObs > 0) {
                    sigPower=sigPower-NumObs*Stone.Decay(freq);
            }
            if(sigPower >= -54){
                p.setPen(QColor(255, 0, 0, 255)); // <-- задание цвета красный
            }
            if (sigPower < -54 && sigPower >= -64) {
                p.setPen(QColor(228, 95, 0, 255)); // <-- задание цвета красноорнжевый
            }
            if (sigPower < -64 && sigPower >= -74) {
                p.setPen(QColor(255, 175, 0, 255)); //оранжевый
            }
            if (sigPower < -74 && sigPower >= -84) {
                p.setPen(QColor(255, 255, 0, 255)); //желтый
            }
            if (sigPower < -84 && sigPower >= -94) {
                p.setPen(QColor(216, 255, 0, 255)); //лаймовый
            }
            if (sigPower < -94 && sigPower >= -104) {
                p.setPen(QColor(36, 255, 0, 255)); //зелёный
            }
            if (sigPower < -104 && sigPower >= -114) {
                p.setPen(QColor(0, 255, 143, 255)); //зелёноголубой
            }
            if (sigPower < -114 && sigPower >= -124) {
                p.setPen(QColor(0, 255, 255, 255)); //голубой
            }
            if (sigPower < -124 && sigPower >= -134) {
                p.setPen(QColor(0, 130, 255, 255)); //синий
            }
            if (sigPower < -134) {
                p.setPen(QColor(40, 0, 255, 255)); //глубокий синий
            }

            QRgb color=image.pixel(i, j);
            int red = qRed(color);
            int green = qGreen(color);
            int blue = qBlue(color);
            if (red==34 && green==34 && blue==34) {
                p.setPen(QColor(34, 34, 34, 255));
            }

            p.drawPoint(i, j);
        }
    }
    p.setPen(QColor(255, 255, 255, 255));
    p.drawPoint(cellPosX, cellPosY);

    p.end();
    scene->addPixmap(map);
    QGraphicsView* view = new QGraphicsView(scene);
    setCentralWidget(view);
}

MainWindow::~MainWindow()
{
}
