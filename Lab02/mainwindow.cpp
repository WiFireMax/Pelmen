#include "mainwindow.h"
#include <QtWidgets>
#include <QPixmap>
#include <QPainter>
#include <time.h>
float PL(float f, float d){
    return 28 + 22 * log10(d) + 20*log10(f);
}
float Decay(int N, float f) { //N-номер места в массиве за определённый материал
    if (N == 3) {
        return 23+0.3*f;
    }
    if (N == 2) {
        return 2+0.2*f;
    }
    if (N == 0) {
        return 5+4*f;
    }
    if (N == 1) {
        return 4.85+0.12*f;
    }
    else {
        return 0;
    }
}
int* N_of_obstructions(QPainter* Pix, QPixmap* Map, int *Number, int x0, int x1, int y0, int y1) {
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
    if (red1==34 && green1==34 && blue1==34) { //если бетон
        Number[0]+=1;
    }
    if (red1==80 && green1==40 && blue1==5) { //дерево
        Number[1]+=1;
    }
    if (red1==77 && green1==101 && blue1==110) { //IRR стекло
        Number[2]+=1;
    }
    if (red1==190 && green1==208 && blue1==215) { //Стеклопакет
        Number[3]+=1;
    }
    while (x0 != x1 || y0 != y1) {

        QRgb color2=image.pixel(x0, y0);
        int red2 = qRed(color2);
        int green2 = qGreen(color2);
        int blue2 = qBlue(color2);

        if (red2==34 && green2==34 && blue2==34) { //если бетон
            Number[0]+=1;
        }
        if (red2==80 && green2==40 && blue2==5) { //дерево
            Number[1]+=1;
        }
        if (red2==77 && green2==101 && blue2==110) { //IRR стекло
            Number[2]+=1;
        }
        if (red2==190 && green2==208 && blue2==215) { //Стеклопакет
            Number[3]+=1;
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
    int line(QPainter* Pix, int x0, int x1, int y0, int y1) { //линия
        int deltax = abs(x1 - x0);
        int deltay = abs(y1 - y0);
        int signx = x0 < x1 ? 1 : -1;
        int signy = y0 < y1 ? 1 : -1;
        int error = deltax - deltay;
        if (concrete == true) {
            Pix->setPen(QColor(34, 34, 34, 255));
        }
        if (tree == true) {
            Pix->setPen(QColor(80, 40, 5, 255));
        }
        if (doublegrass == true) {
            Pix->setPen(QColor(77, 101, 110, 255));
        }
        if (irrgrass == true) {
            Pix->setPen(QColor(190, 208, 215, 255));
        }
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
    int circle(QPainter* Pix, int X1, int Y1, int R) { //круг
        int x = 0;
        int y = R;
        int delta = 1 - 2 * R;
        int error = 0;
        if (concrete == true) {
            Pix->setPen(QColor(34, 34, 34, 255));
        }
        if (tree == true) {
            Pix->setPen(QColor(80, 40, 5, 255));
        }
        if (doublegrass == true) {
            Pix->setPen(QColor(77, 101, 110, 255));
        }
        if (irrgrass == true) {
            Pix->setPen(QColor(190, 208, 215, 255));
        }
        while (y >= x) {
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
    int square(QPainter* Pix, int x0, int x1, int y0, int y1) { //прямоугольник
        if (concrete == true) {
            Pix->setPen(QColor(34, 34, 34, 255));
        }
        if (tree == true) {
            Pix->setPen(QColor(80, 40, 5, 255));
        }
        if (doublegrass == true) {
            Pix->setPen(QColor(77, 101, 110, 255));
        }
        if (irrgrass == true) {
            Pix->setPen(QColor(190, 208, 215, 255));
        }
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
    int onePixDistance = 20; //m
    int TxPower = 23;
    int antGain = 12;
    float freq=2.5;
    srand(time(NULL)); //установка точки 5G New Radio
    int cellPosX =rand()%999;
    int cellPosY=rand()%999;
    QPixmap map(maxX, maxY); // создаем карту для рисования
    QPainter p(&map);

    struct Obstruction Stone; //стена
    Stone.concrete = true;
    Stone.circle(&p, 700, 700, 200);

    struct Obstruction Fence; //забор
    Fence.tree = true;
    Fence.line(&p, 200, 700, 800, 600);

    struct Obstruction Office; //офис
    Office.doublegrass = true;
    Office.square(&p, 200, 500, 100, 400);

    QImage image = map.toImage();

    for(int i = 0; i < maxX; i++){
        for(int j = 0; j < maxY; j++){
            float distance = 0;
            distance =sqrt (pow(abs(cellPosX - i), 2) + pow(abs(cellPosY-j), 2));
            distance *= onePixDistance;
            float sigPower = TxPower + antGain - PL(freq, distance);
            int NumObs[4]{}; //пустой массив препятствий
            N_of_obstructions(&p, &map, NumObs, cellPosX, i, cellPosY, j); //заполнение массива
            if (NumObs[0] > 0 || NumObs[1] > 0 || NumObs[2] > 0 || NumObs[3] > 0) {
                    //затухание
                    sigPower=sigPower-NumObs[0]*Decay(0, freq)-NumObs[1]*Decay(1, freq)-NumObs[2]*Decay(2, freq)-NumObs[3]*Decay(3, freq);
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

            QRgb color=image.pixel(i, j); //оставляем нарисованные препятствия
            int red = qRed(color);
            int green = qGreen(color);
            int blue = qBlue(color);
            if (red==34 && green==34 && blue==34) {
                p.setPen(QColor(34, 34, 34, 255));
            }
            if (red==80 && green==40 && blue==5) {
                p.setPen(QColor(80, 40, 5, 255));
            }
            if (red==77 && green==101 && blue==110) {
                p.setPen(QColor(77, 101, 110, 255));
            }
            if (red==190 && green==208 && blue==215) {
                p.setPen(QColor(190, 208, 215, 255));
            }
            //рисуем пиксель
            p.drawPoint(i, j);
        }
    }
    p.setPen(QColor(255, 255, 255, 255)); //нарисованная точка доступа
    p.drawPoint(cellPosX, cellPosY);

    p.end();
    scene->addPixmap(map);
    QGraphicsView* view = new QGraphicsView(scene);
    setCentralWidget(view);
}

MainWindow::~MainWindow()
{
}
