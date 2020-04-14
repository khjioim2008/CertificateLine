#ifndef __Configure_H
#define __Configure_H
#include "BasicTypes.h"
#include "systemValues.h"

#define POSITIVE	1//正	
#define REVERSE		2//反
#define STOP		0//停
#define PRECISION 100 //占空比精度

//CutPostion传感器和倒角刀的距离 
//#define DIS_SIGNCUT_AND_CHCHAMFER 8 //单位毫米
#define DIS_SIGNCUT_AND_CHCHAMFER 20 //单位毫米
//废料刀和倒角刀的距离 
#define DIS_CHCHAMFER_AND_FLOTASM 73 //单位毫米
//证书长度 存在  废料刀和倒角刀距离大于 证书长度的情况 （小证书）
#define DIS_CERTIFICATE           125 //单位毫米
#define DIS_CORNER                4   //单位毫米

#define DIS_STAMP_AND_SENSOR      19200 // 单位是脉冲数




#define CUT_STEP_SEEP                 75  //这里的单位的弧度/秒

#define STAMP_STEP_SPEED               50 

#define FRICTION_STEP_SPEED             70 

#define ROLLER_INITIAL_SPEED               80

#define ROLLER_SYNC_SPEED               16 

#define CERTIFICATE_SPACE  							9000 // 证书间距



#define BEHIND_TRUE             	   0x01

#define NO_PAPER_TRUE                0x02

#define RELOAD_PAPER                 0x04

#define WORK_FINISH                  0x08

#define PLASTIC_STEP_MOVE            0x10

#endif
