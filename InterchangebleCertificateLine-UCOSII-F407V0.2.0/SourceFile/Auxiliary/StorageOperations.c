#include "Configure.h"


N8 copyRAM(N8 *Sources,N8 *Destination,N32 Size)
{//RAM_SIZE RAM_ADDR
        N32 i = 0;
        
        if(((Sources - Destination) > RAM_SIZE) || ((Destination - Sources) > RAM_SIZE)) return 1;
        if(((N32 )Sources > (RAM_ADDR + RAM_SIZE)) || ((N32 )Destination > (RAM_ADDR + RAM_SIZE))) return 2;

        if(Sources > Destination) {
                for(i = Size;i > 0;i --)
                {
                        *(Destination + i) = *(Sources + i);
                }
                
        } else if(Sources < Destination) {
                for(i = 0;i < Size;i ++)
                {
                        *(Destination + i) = *(Sources + i);
                }        
        } 
        return 0;
}



/*

Sources         1 2 3 4 5 6 7 8 9
            1 2 3 4 5 6 7 8 9
                    1 2 3 4 5 6 7 8 9
               
                


Destination
*/
