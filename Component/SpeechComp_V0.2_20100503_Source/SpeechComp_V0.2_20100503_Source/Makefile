
OPROS_INC=C:/2012OPRoSRobotContest_0730/OPRoS/OprosDevelopment/IDE/CommonLibrary/include
OPROS_LIB=C:/2012OPRoSRobotContest_0730/OPRoS/OprosDevelopment/IDE/CommonLibrary/lib

PROJECT1=SpeechComp
OUTDIR1=.\$(PROJECT1)\Release
INTDIR1=.\$(PROJECT1)\Release
OutDir1=.\$(PROJECT1)\Release
TARGET1=$(PROJECT1).dll
OBJ1=$(PROJECT1).obj

ALL:	$(OUTDIR1)\$(TARGET1)

CLEAN:
	-@erase "$(OUTDIR1)\$(TARGET1)"
	-@erase "$(OUTDIR1)\$(OBJ1)"
	-@erase "$(OUTDIR1)\$(PROJECT1).lib"

$(OUTDIR1) :
	if not exist "$(OUTDIR1)/$(NULL)" mkdir "$(OUTDIR1)"

INC = /I "$(OPROS_INC)" -IC:/2012OPRoSRobotContest_0730/OPRoS/OprosDevelopment/IDE/CommonLibrary/include/device

CPP=cl.exe

CPP_PROJ=/O2 /Oi $(INC) /GL /D "WIN32" /D "_WINDOWS" /D "NRELEASE" /D "MAKE_COMPONENT_DLL" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" \
/D "MBCS" /FD /EHsc /MD /Gy /Fo"$(INTDIR1)\\" /Fd"$(INTDIR1)\\" /W0 /nologo /c /Zi /TP /errorReport:prompt

{$(PROJECT1)}.cpp{$(INTDIR1)}.obj::
	$(CPP) $(CPP_PROJ) $<

LINK32=link.exe
LINK32_FLAGS1=/OUT:"$(OUTDIR1)\$(TARGET1)" /INCREMENTAL:NO /NOLOGO /libpath:"$(OPROS_LIB)" \
/DLL /MANIFEST /RELEASE /SUBSYSTEM:WINDOWS /OPT:REF /OPT:ICF /LTCG /DYNAMICBASE /NXCOMPAT /MACHINE:X86 /ERRORREPORT:PROMPT OPRoSCDL.lib


LINK32_OBJS1= \
	"$(INTDIR1)/$(OBJ1)"


$(OUTDIR1)/$(TARGET1) : $(OUTDIR1) $(DEF_FILE) $(LINK32_OBJS1)
	$(LINK32) $(LINK32_FLAGS1) $(LINK32_OBJS1)
