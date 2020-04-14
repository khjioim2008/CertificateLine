#ifndef __Constant_H
#define __Constant_H
#include "BasicTypes.h"
/*
�����ϵΪ: 1bar=100,000Pa=0.98692327atm=750.06168mm Hg[1] 
1Pa=1N/m^2 1MPa=1N/mm^2
1 Torr = 133.3 Pa
1 Pa = 7.5��10-3 Torr
1 Torr = 1 mmHg ��
1 Torr = 1/760 ����ѹ
1 bar = 0.1MPa
P = 1013.25hPa(����) = 101325 N/M*M
altitude = 44330 * (1 - (p / P)^(1 / 5.255))

P = p / (1 - altitude / 44330)^5.255


*/
struct Math_Const_struct
{
        const int P;
}Math_Const = {
        101325
};


#endif



//mathematics
//constant

//Math_Const
