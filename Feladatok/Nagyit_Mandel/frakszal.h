#ifndef FRAKSZAL_H
#define FRAKSZAL_H

#include <QThread>
#include <math.h>
#include "frakablak.h"

class FrakAblak;

class FrakSzal : public QThread
{
    Q_OBJECT

public:
    FrakSzal(double a, double b, double c, double d,
             int szelesseg, int magassag, int iteraciosHatar, FrakAblak *frakAblak);
    ~FrakSzal();
    void run();

protected:

    double a, b, c, d;

    int szelesseg, magassag;

    int iteraciosHatar;

    FrakAblak* frakAblak;

    int* egySor;

};

#endif // FRAKSZAL_H
