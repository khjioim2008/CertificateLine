#include "protect.h"
//#include "Drive.h"
#include "Resources.h"
#include "Configure.h"
#include "flash.h"
extern N8 CompareID(void);

N32 protection(void)
{
#if defined PROTECT
        if(CompareID() == 0) Protect.cont = 0;
        else Protect.cont ++;
#endif
#if !defined PROTECT
        Protect.cont = 0;
#endif
        
        return Protect.cont;
}

void clrAll(void)
{
        te12st();
}






//fromelf.exe --bin -o   ./Objects/@l.bin ./Objects/@l.axf


