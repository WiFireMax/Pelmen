#ifndef DRAWPIXMAP_H
#define DRAWPIXMAP_H
#include <QtWidgets>
#include <QPixmap>
#include <QPainter>
void DrawPixMap(QPainter* p, QImage* image, QPixmap* map, int maxX, int maxY, int onePixDistance, int TxPower, int antGain, int freq);
#endif // DRAWPIXMAP_H
