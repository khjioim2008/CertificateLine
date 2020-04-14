#ifndef __Configure_H
#define __Configure_H
#include "BasicTypes.h"
#include "systemValues.h"

#define POSITIVE	1//��	
#define REVERSE		2//��
#define STOP		0//ͣ
#define PRECISION 100 //ռ�ձȾ���

//CutPostion�������͵��ǵ��ľ��� 
//#define DIS_SIGNCUT_AND_CHCHAMFER 8 //��λ����
#define DIS_SIGNCUT_AND_CHCHAMFER 20 //��λ����
//���ϵ��͵��ǵ��ľ��� 
#define DIS_CHCHAMFER_AND_FLOTASM 73 //��λ����
//֤�鳤�� ����  ���ϵ��͵��ǵ�������� ֤�鳤�ȵ���� ��С֤�飩
#define DIS_CERTIFICATE           125 //��λ����
#define DIS_CORNER                4   //��λ����

#define DIS_STAMP_AND_SENSOR      19200 // ��λ��������




#define CUT_STEP_SEEP                 75  //����ĵ�λ�Ļ���/��

#define STAMP_STEP_SPEED               50 

#define FRICTION_STEP_SPEED             70 

#define ROLLER_INITIAL_SPEED               80

#define ROLLER_SYNC_SPEED               16 

#define CERTIFICATE_SPACE  							9000 // ֤����



#define BEHIND_TRUE             	   0x01

#define NO_PAPER_TRUE                0x02

#define RELOAD_PAPER                 0x04

#define WORK_FINISH                  0x08

#define PLASTIC_STEP_MOVE            0x10

#endif
