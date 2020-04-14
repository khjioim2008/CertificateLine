#include "cpucrj.h"
#include "Configure.h"
#include "ram.h"
#include "CRC16.h"
#include "Resources.h"
#include "includes.h"
#include "SerialPort.h"
#include "certificateControl.h"
#include "iap.h"
#include "DataDeal.h"

/*--------------------------Sign---------------------------------*/
void signalStability(Sign *Name,N8 (* fun)(void))
{
        N8 temp = fun();

		Name->Detect = 3;

        if(temp != Name->SIOB) {
                Name->DetectV	= 0;
                Name->SIOB		= temp;
        }
        if(Name->DetectV < Name->Detect) Name->DetectV ++;
        else {Name->DetectV = 255;Name->SIO = Name->SIOB;}
}

void signalStabilityHengDao(Sign *Name,N8 (* fun)(void))
{
        N8 temp = fun();

		Name->Detect = 5;

        if(temp != Name->SIOB) {
                Name->DetectV	= 0;
                Name->SIOB		= temp;
        }
        if(Name->DetectV < Name->Detect) Name->DetectV ++;
        else {Name->DetectV = 255;Name->SIO = Name->SIOB;}
}

void signalStabilitySend(Sign *Name,N8 (* fun)(void))
{
        N8 temp = fun();

			Name->Detect = 1000;

        if(temp != Name->SIOB) {
                Name->DetectV	= 0;
                Name->SIOB		= temp;
        }
        if(Name->DetectV < Name->Detect) Name->DetectV ++;
        else {Name->DetectV = 65535;Name->SIO = Name->SIOB;}
}

/*--------------------------Sign---------------------------------*/
void signalStabilityT(Sign *Name,N8 (* fun)(void),N16 DeTemp)
{
				N8 temp = fun();
				Name->Detect = DeTemp;

        if(temp != Name->SIOB) {
                Name->DetectV	= 0;
                Name->SIOB		= temp;
        }
        if(Name->DetectV < Name->Detect) Name->DetectV ++;
        else {Name->DetectV = 255;Name->SIO = Name->SIOB;}
}

