#include "MatSignal.h"
#include "math.h"
float PL(float f, float d){
    return 28 + 22 * log10(d) + 20*log10(f);
}
float Delay(int N, float f) { //N-номер места в массиве за определённый материал
    if (N == 3) { //IRR стекло
        return 23+0.3*f;
    }
    if (N == 2) { //стеклопакет
        return 2+0.2*f;
    }
    if (N == 0) { //бетон
        return 5+4*f;
    }
    if (N == 1) { //дерево
        return 4.85+0.12*f;
    }
    else {
        return 0;
    }
}
