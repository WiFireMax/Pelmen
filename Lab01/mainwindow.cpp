#include "mainwindow.h"
#include <QtWidgets>
#include <QPixmap>
#include <QPainter>
#include <time.h>
float PL(float f, float d){
    return 28 + 22 * log10(f) + 20*log10(d);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QGraphicsScene* scene = new QGraphicsScene();
    int maxX=1000, maxY=1000;
    int onePixDistance = 1500; //m
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
    p.end();
    scene->addPixmap(map);
    QGraphicsView* view = new QGraphicsView(scene);
    setCentralWidget(view);
}

MainWindow::~MainWindow()
{
}
