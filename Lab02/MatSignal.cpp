#include "MatSignal.h"
#include "math.h"
float PL(float f, float d){
    return 28 + 22 * log10(d) + 20*log10(f);
}
float Delay(int N, float f) { //N-номер места в массиве за определённый материал
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
