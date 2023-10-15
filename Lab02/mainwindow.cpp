#include "mainwindow.h"
#include <QtWidgets>
#include <QPixmap>
#include <QPainter>
#include <time.h>
float PL(float f, float d){
    return 28 + 22 * log10(f) + 20*log10(d);
}
int N_of_obstructions(QPainter* Pix, QPixmap* Map, int x0, int x1, int y0, int y1) {
    int Number=0;
    int deltax = abs(x1 - x0);
    int deltay = abs(y1 - y0);
    float error = 0;
    float deltaerr = (deltay + 1) / (deltax + 1);
    int y = y0;
    int diry = y1 - y0;
    if (diry > 0) {
        diry = 1;
    }
    if (diry < 0) {
        diry = -1;
    }
    for (int x=x0; x<x1; x++) {
        QImage image = Map->toImage();
        QRgb color=image.pixel(x, y);
        int red = qRed(color);
        int green = qGreen(color);
        int blue = qBlue(color);
        if (red==0 && green==0 && blue==0) {
            Number+=1;
        }
        Pix->setPen(QColor(0, 100, 0, 255));
        Pix->drawPoint(x, y);
        error = error + deltaerr;
        if (error >= 1.0) {
            y = y + diry;
            error = error - 1.0;
        }
    }
    return Number;
}
struct Obstruction { //препятствия
    struct Material {
        int irrgrass; //IRR стекло
        int doublegrass; //стеклопакет
        int concrete; //бетон
        int tree; //дерево
    };
    int line(QPainter* Pix, int x0, int x1, int y0, int y1) { //линия
        int deltax = abs(x1 - x0);
        int deltay = abs(y1 - y0);
        float error = 0;
        float deltaerr = (deltay + 1) / (deltax + 1);
        int y = y0;
        int diry = y1 - y0;
        if (diry > 0) {
            diry = 1;
        }
        if (diry < 0) {
            diry = -1;
        }
        for (int x=x0; x<x1; x++) {
            Pix->setPen(QColor(0, 0, 0, 255));
            Pix->drawPoint(x, y);
            error = error + deltaerr;
            if (error >= 1.0) {
                y = y + diry;
                error = error - 1.0;
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
            Pix->setPen(QColor(0,0,0,255));
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
        Pix->setPen(QColor(0,0,0,255));
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
    int onePixDistance = 1000; //m
    int TxPower = 23;
    int antGain = 12;
    float freq=2.5;
    srand(time(NULL)); //установка точки 5G New Radio
    int cellPosX =rand()%999;
    int cellPosY=rand()%999;
    QPixmap map(maxX, maxY); // создаем карту для рисования
    QPainter p(&map);
    for(int i = 0; i < maxX; i++){
        for(int j = 0; j < maxY; j++){
            float distance = 0;
            distance =sqrt (pow(abs(cellPosX - i), 2) + pow(abs(cellPosY-j), 2));
            distance *= onePixDistance;
            float sigPower = TxPower + antGain - PL(freq, distance);
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
            p.drawPoint(i, j);
        }
    }
    p.setPen(QColor(255, 255, 255, 127));
    p.drawPoint(cellPosX, cellPosY);
    // /////////////////////////////////////////////////////////////////////////////////////
    struct Obstruction Stone;
    Stone.line(&p, 50, 400, 300, 800);
    Stone.circle(&p, 300, 500, 200);
    Stone.square(&p, 700, 900, 600, 800);
    int Num=N_of_obstructions(&p, &map, cellPosX, 500, cellPosY, 500);
    QString text=QString::number(Num);
    QLabel *label = new QLabel();
    label->setText(text);
    label->show();
    // /////////////////////////////////////////////////////////////////////////////////////
    p.end();
    scene->addPixmap(map);
    QGraphicsView* view = new QGraphicsView(scene);
    setCentralWidget(view);
}

MainWindow::~MainWindow()
{
}
