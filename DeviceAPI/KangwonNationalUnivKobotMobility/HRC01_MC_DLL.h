#ifndef _HRC01_MC_DLL_H_
#define _HRC01_MC_DLL_H_

#pragma comment(lib,"HRC01_MC_DLL")

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// device open, close 관련
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern "C" __declspec(dllimport) int GetDeviceDescriptor(PUSHORT idBuffer);

extern "C" __declspec(dllimport) int OpenDevice(void);

extern "C" __declspec(dllimport) void CloseDevice(void);

extern "C" __declspec(dllimport) BOOL RunBoard(void);

extern "C" __declspec(dllimport) BOOL StopBoard(void);

extern "C" __declspec(dllimport) BOOL GetStatusData(UCHAR* Data);

extern "C" __declspec(dllimport) BOOL GetADCData(UCHAR* Data);

//extern "C" __declspec(dllimport) BOOL ADCStart(void);

//extern "C" __declspec(dllimport) BOOL ADCStop(void);

extern "C" __declspec(dllimport) int CurrentLimitChange(BYTE *pbyData);

extern "C" __declspec(dllimport) int CurrentLimitEnable(BYTE *pbyData);

extern "C" __declspec(dllimport) int CurrentLimitClear(void);

extern "C" __declspec(dllimport) int BumperLimitSetClear(UCHAR *pbyData);

extern "C" __declspec(dllimport) int BumperEventOnOff(UCHAR byData);

//extern "C" __declspec(dllimport) int BumperEventDataRad(UCHAR *pbyData);

extern "C" __declspec(dllimport) int BumperEventMask(UCHAR byData);

extern "C" __declspec(dllimport) int CameraTiltMotorCommand(UCHAR *pbyData);

extern "C" __declspec(dllimport) int CameraTiltMotorData(UCHAR *pbyData);

extern "C" __declspec(dllimport) char* GetError();
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*************************************************************/
//	motion controller relation
/**************************************************************/
extern "C" __declspec(dllimport) BOOL CR_kdil(int axis, WORD kd, WORD il);
extern "C" __declspec(dllimport) BOOL CR_kdil_3(WORD kd1, WORD il1,WORD kd2, WORD il2, WORD kd3, WORD il3);
extern "C" __declspec(dllimport) BOOL CR_kpki(int axis, WORD kp, WORD ki);
extern "C" __declspec(dllimport) BOOL CR_kpki_3(WORD kp1, WORD ki1,WORD kp2, WORD ki2, WORD kp3, WORD ki3);
//extern "C" __declspec(dllimport) BOOL CR_absolute_acc(int axis, unsigned int Acc);	// Acc를 UINT->double로 변경.
extern "C" __declspec(dllimport) BOOL CR_absolute_acc(int axis, double Acc);
extern "C" __declspec(dllimport) BOOL CR_absolute_acc_3(unsigned int Acc1, unsigned int Acc2, unsigned int Acc3);
extern "C" __declspec(dllimport) BOOL CR_absolute_velo(int axis, unsigned int Velo);
extern "C" __declspec(dllimport) BOOL CR_absolute_velo_3(unsigned int Velo1, unsigned int Velo2, unsigned int Velo3);
extern "C" __declspec(dllimport) BOOL CR_absolute_pos_start(int axis, int Pos);
extern "C" __declspec(dllimport) BOOL CR_absolute_pos_start_2(int Pos1, int Pos2);
extern "C" __declspec(dllimport) BOOL CR_absolute_pos_start_3(int Pos1, int Pos2, int Pos3);
extern "C" __declspec(dllimport) BOOL CR_relative_pos_start(int axis, int Pos);
extern "C" __declspec(dllimport) BOOL CR_relative_pos_start_2(int Pos1, int Pos2);
extern "C" __declspec(dllimport) BOOL CR_relative_pos_start_3(int Pos1, int Pos2, int Pos3);
extern "C" __declspec(dllimport) BOOL CR_absolute_velo_change(unsigned char axis, unsigned int Velo);
//extern "C" __declspec(dllimport) BOOL CR_absolute_velo_start(int axis, int dir ,unsigned int Velo);	// velo를 UINT->double로 변경.
extern "C" __declspec(dllimport) BOOL CR_absolute_velo_start(int axis, int dir ,double Velo);
extern "C" __declspec(dllimport) BOOL CR_absolute_velo_start_2(int dir1 ,unsigned int Velo1,int dir2 ,unsigned int Velo2);
extern "C" __declspec(dllimport) BOOL CR_absolute_velo_start_3(int dir1 ,unsigned int Velo1,int dir2 ,unsigned int Velo2,int dir3 ,unsigned int Velo3);
extern "C" __declspec(dllimport) BOOL CR_define_home(int axis);
extern "C" __declspec(dllimport) BOOL CR_define_home_3(void);
extern "C" __declspec(dllimport) BOOL CR_soft_Reset(int axis);
extern "C" __declspec(dllimport) BOOL CR_Soft_Reset_3(void);
extern "C" __declspec(dllimport) BOOL CR_stop_abruptly(int axis);
extern "C" __declspec(dllimport) BOOL CR_stop_abruptly_3(void);
extern "C" __declspec(dllimport) BOOL CR_stop_smoothly(int axis);
extern "C" __declspec(dllimport) BOOL CR_stop_smoothly_3(void);
extern "C" __declspec(dllimport) BOOL CR_motor_off(int axis);
extern "C" __declspec(dllimport) BOOL CR_motor_off_3(void);
extern "C" __declspec(dllimport) int CR_RRposition(int axis);
extern "C" __declspec(dllimport) void CR_RRposition_3(int *Pos);
extern "C" __declspec(dllimport) unsigned int CR_RRVelocity(int axis);
extern "C" __declspec(dllimport) void CR_RRVelocity_3(unsigned int *Velo);
extern "C" __declspec(dllimport) unsigned int CR_RDVelocity(int axis);
extern "C" __declspec(dllexport) int CR_Motion_Status(unsigned char axis);

extern "C" __declspec(dllimport) void CR_Limit_Set(int axis);
extern "C" __declspec(dllimport) void CR_Limit_Motion_Clear(int axis);
extern "C" __declspec(dllimport) void CR_Read_limit1(unsigned char *limit);
extern "C" __declspec(dllimport) unsigned char CR_Amp_status(void);

#endif
