#include "frakszal.h"

FrakSzal::FrakSzal(double a, double b, double c, double d,
                   int szelesseg, int magassag, int iteraciosHatar, FrakAblak *frakAblak)
{
    this->a = a;
    this->b = b;
    this->c = c;
    this->d = d;
    this->szelesseg = szelesseg;
    this->iteraciosHatar = iteraciosHatar;
    this->frakAblak = frakAblak;
    this->magassag = magassag;

    egySor = new int[szelesseg];
}

FrakSzal::~FrakSzal()
{
    delete[] egySor;
}

void FrakSzal::run()
{

    double dx = (b-a)/szelesseg;
    double dy = (d-c)/magassag;
    double reC, imC, reZ, imZ, ujreZ, ujimZ;

    int iteracio = 0;

    for(int j=0; j<magassag; ++j) {

        for(int k=0; k<szelesseg; ++k) {

            reC = a+k*dx;
            imC = d-j*dy;

            reZ = 0;
            imZ = 0;
            iteracio = 0;

            while(reZ*reZ + imZ*imZ < 4 && iteracio < iteraciosHatar) {


                ujreZ = reZ*reZ - imZ*imZ + reC;
                ujimZ = 2*reZ*imZ + imC;

                reZ = ujreZ;
                imZ = ujimZ;

                ++iteracio;

            }


            iteracio %= 256;


            egySor[k] = iteracio;
        }

        frakAblak->vissza(j, egySor, szelesseg);
    }
    frakAblak->vissza();

}


