#ifndef MYPOINT_H
#define MYPOINT_H


class myPoint
{
public:
    myPoint();
    myPoint(int ix,int iy,int Iindex,int clr,bool Ioccupyed=0);

    int x;
    int y;
    bool occupyed;
    int index;//0123456车马相士将炮卒
    int color;//1黑，0红
};

#endif // MYPOINT_H
