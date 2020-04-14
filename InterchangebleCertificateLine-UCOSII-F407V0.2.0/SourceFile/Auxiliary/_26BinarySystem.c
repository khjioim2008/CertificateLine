#include "Type.h"

N8 DecimalToOther(N64 Decimal,N8 Other,char *addr)
{
        N8 i = 0;
        while (1)
        {
                if(Decimal < Other) {
                        addr[i]    = Decimal;
                        break;        
                } else {
                        addr[i]    = Decimal % Other;
                        Decimal = (Decimal - addr[i]) / Other;            
                } 
                i=i+1;  
        }
        return i+1;
}
N32 OtherToDecimal(N8 Other,N8 Size,char *addr)
{
        N32 Decimal     = 0;
        Decimal         = addr[Size - 1];
        while(1)
        {
                Size --;
                if(addr[Size] < Other) ;else return 0XFFFFFFFF;
                Decimal = Decimal*Other + addr[Size - 1];
                if(Size == 1) break;    
        }
        return Decimal;
}
