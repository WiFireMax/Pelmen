#include "mainwindow.h"
#include <QtWidgets>
#include <QPixmap>
#include <QPainter>
#include <time.h>
float PL(float f, float d){
    return 28 + 22 * log10(f) + 20*log10(d);
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
    // /////////////////////////////////////////////////////////////////////////////////////
    p.end();
    scene->addPixmap(map);
    QGraphicsView* view = new QGraphicsView(scene);
    setCentralWidget(view);
}

MainWindow::~MainWindow()
{
}
