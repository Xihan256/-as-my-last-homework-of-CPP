#include "mypoint.h"

myPoint::myPoint()
{

}

myPoint::myPoint(int ix,int iy,int Iindex,int clr,bool Ioccupyed){
    this->x=ix;
    this->y=iy;
    this->index=Iindex;
    this->color=clr;
    this->occupyed=Ioccupyed;
}
