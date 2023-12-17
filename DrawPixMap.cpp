#include "DrawPixMap.h"
#include "MatSignal.h"
#include "DrawObstructions.h"
#include <QtWidgets>
#include <QPixmap>
#include <QPainter>
void DrawPixMap(QPainter* p, QImage* image, QPixmap* map, int maxX, int maxY, int onePixDistance, int TxPower, int antGain, int freq) {
    srand(time(NULL)); //установка точки 5G New Radio
    int cellPosX =rand()%(maxX-1);
    int cellPosY=rand()%(maxY-1);
    for(int i = 0; i < maxX; i++){
        for(int j = 0; j < maxY; j++){
            float distance = 0;
            distance =sqrt (pow(abs(cellPosX - i), 2) + pow(abs(cellPosY-j), 2));
            distance *= onePixDistance;
            float sigPower = TxPower + antGain - PL(freq, distance);
            int NumObs[4]{}; //пустой массив препятствий
            N_of_obstructions(p, map, NumObs, cellPosX, i, cellPosY, j); //заполнение массива
            if (NumObs[0] > 0 || NumObs[1] > 0 || NumObs[2] > 0 || NumObs[3] > 0) {
                //затухание
                sigPower=sigPower-NumObs[0]*Delay(0, freq)-NumObs[1]*Delay(1, freq)-NumObs[2]*Delay(2, freq)-NumObs[3]*Delay(3, freq);
            }
            if(sigPower >= -54){
                p->setPen(QColor(255, 0, 0, 255)); // <-- задание цвета красный
            }
            if (sigPower < -54 && sigPower >= -59) {
                p->setPen(QColor(246, 70, 0, 255)); //
            }
            if (sigPower < -59 && sigPower >= -64) {
                p->setPen(QColor(228, 110, 0, 255)); // <-- задание цвета красноорнжевый
            }
            if (sigPower < -64 && sigPower >= -69) {
                p->setPen(QColor(228, 140, 0, 255)); //
            }
            if (sigPower < -69 && sigPower >= -74) {
                p->setPen(QColor(255, 175, 0, 255)); //оранжевый
            }
            if (sigPower < -74 && sigPower >= -79) {
                p->setPen(QColor(255, 200, 0, 255)); //
            }
            if (sigPower < -79 && sigPower >= -84) {
                p->setPen(QColor(255, 255, 0, 255)); //желтый
            }
            if (sigPower < -84 && sigPower >= -89) {
                p->setPen(QColor(230, 255, 0, 255)); //
            }
            if (sigPower < -89 && sigPower >= -94) {
                p->setPen(QColor(216, 255, 0, 255)); //лаймовый
            }
            if (sigPower < -94 && sigPower >= -99) {
                p->setPen(QColor(150, 255, 0, 255)); //
            }
            if (sigPower < -99 && sigPower >= -104) {
                p->setPen(QColor(36, 255, 0, 255)); //зелёный
            }
            if (sigPower < -104 && sigPower >= -109) {
                p->setPen(QColor(0, 255, 70, 255)); //
            }
            if (sigPower < -109 && sigPower >= -114) {
                p->setPen(QColor(0, 255, 143, 255)); //зелёноголубой
            }
            if (sigPower < -114 && sigPower >= -119) {
                p->setPen(QColor(0, 255, 180, 255)); //
            }
            if (sigPower < -119 && sigPower >= -124) {
                p->setPen(QColor(0, 255, 255, 255)); //голубой
            }
            if (sigPower < -124 && sigPower >= -129) {
                p->setPen(QColor(0, 180, 255, 255)); //
            }
            if (sigPower < -129 && sigPower >= -134) {
                p->setPen(QColor(0, 130, 255, 255)); //синий
            }
            if (sigPower < -134) {
                p->setPen(QColor(40, 0, 255, 255)); //глубокий синий
            }

            QRgb color=image->pixel(i, j); //оставляем нарисованные препятствия
            int red = qRed(color);
            int green = qGreen(color);
            int blue = qBlue(color);
            if (red==34 && green==34 && blue==34) {
                p->setPen(QColor(34, 34, 34, 255));
            }
            if (red==161 && green==104 && blue==7) {
                p->setPen(QColor(161, 104, 7, 255));
            }
            if (red==77 && green==101 && blue==110) {
                p->setPen(QColor(77, 101, 110, 255));
            }
            if (red==190 && green==208 && blue==215) {
                p->setPen(QColor(190, 208, 215, 255));
            }
            //рисуем пиксель
            p->drawPoint(i, j);
        }
    }
    p->setPen(QColor(255, 255, 255, 255)); //нарисованная точка доступа
    p->drawPoint(cellPosX, cellPosY);

    p->end();
}
