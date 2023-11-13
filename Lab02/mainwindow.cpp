#include "mainwindow.h"
#include <time.h>
#include <ctime>
#include <iostream>
#include "DrawObstructions.h"
#include "DrawPixMap.h"
unsigned int start_time =  clock(); // начальное время
using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QGraphicsScene* scene = new QGraphicsScene();
    int maxX=600, maxY=600;
    int onePixDistance = 10; //m
    int TxPower = 23;
    int antGain = 12;
    float freq=2.5;
    QPixmap map(maxX, maxY); // создаем карту для рисования
    QPainter p(&map);

    struct Obstruction Stone; //стена
    Stone.concrete = true;
    Stone.circle(&p, 500, 450, 130);

    struct Obstruction Fence; //забор
    Fence.wood = true;
    Fence.line(&p, 200, 500, 100, 50);

    struct Obstruction Office; //офис
    Office.doublegrass = true;
    Office.square(&p, 50, 300, 150, 400);

    struct Obstruction Stand;
    Stand.irrgrass = true;
    Stand.square(&p, 350, 500, 100, 300);

    QImage image = map.toImage();

    DrawPixMap(&p, &image, &map, maxX, maxY, onePixDistance, TxPower, antGain, freq);
    scene->addPixmap(map);
    QGraphicsView* view = new QGraphicsView(scene);
    setCentralWidget(view);
    unsigned int end_time = clock(); // конечное время
    unsigned int search_time = end_time - start_time; // искомое время
    cout << search_time/1000 << " seconds\n" << endl;
}

MainWindow::~MainWindow()
{
}
