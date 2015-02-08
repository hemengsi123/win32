@echo off
@rem By simon 2014.12.31 �ļ���ʽ������ΪUTF8
@rem to make Makefile depend list
:main
REM Դ�ļ�Ŀ¼
REM set SRCDIRS=. Control Utils CORE Layout
set SRCDIRS=
set DepDir=
set EXTS=.H .CPP .C .RC
set OBJS=
REM �ų���
set EXCLUDE=%~4

REM �����Ҫʹ�����������������Ҫ��ǰ���ټ�һ�Կ����ţ�������·��������� makedep.bat "" . ". Control Utils CORE Layout"
REM %~1 - ɾ���κ�����("); ��ӡ��ǰ·�� chdir
if "%~3" neq "" (
	REM ���������ļ�����Ŀ¼
	if "%~2" neq "" (
	if not exist "%~2" (
		echo %~2 isn't exist
		echo Usage[1]: makedep.bat dir
		echo Usage[2]: makedep.bat
		exit /b 1
	) else ( set DepDir=%~2)
	REM ���������ļ������ڵ�ǰĿ¼
	) else ( if "%DepDir%" equ "" set DepDir=.)
	REM ����Դ��Ŀ¼
	if "%~3" equ "" (
		if "%SRCDIRS%" equ "" (
			REM ����Ĭ��Դ���ڵ�ǰĿ¼
			set SRCDIRS=.
		)
	) else ( set SRCDIRS=%~3)
) else (
REM ======= ʹ��Ĭ������ ========
	if "%~1" neq "" (
		if not exist "%~1" (
			echo %~1 isn't exist
			echo Usage[1]: makedep.bat dir
			echo Usage[2]: makedep.bat
			exit /b 1
		) else ( set DepDir=%~1)
		REM ���������ļ������ڵ�ǰĿ¼
	) else ( if "%DepDir%" equ "" set DepDir=.)
	if "%~2" equ "" (
		if "%SRCDIRS%" equ "" (
			REM ����Ĭ��Դ���ڵ�ǰĿ¼
			set SRCDIRS=.
		)
	) else ( set SRCDIRS=%~2)
)

if "%DepDir%" neq ""  (
	echo depDir  = %DepDir%
)
REM %SRCDIRS:~1,-1% ȥ����һ�������һ���ַ�
if "%SRCDIRS%" neq "" (
	echo srcDirs = %SRCDIRS%
)
REM echo %~dp0
if "%EXCLUDE%" neq "" (
	echo exclude = %EXCLUDE%
) else ( set EXCLUDE=.empty)
REM goto:eof
REM set OUTDIR=.\bin
set curdate=%date:~0,4%-%date:~5,2%-%date:~8,2%[%time:~0,2%:%time:~3,2%]
REM ��������

REM ============== �������� ====================
set srcfile=%DepDir%\msrcfile.lst
set depfile=%DepDir%\mdepfile.lst

call:makeDeplist

REM if "%OBJS%" neq "" (
	if exist "%srcfile%" echo %srcfile% was generated
	if exist "%depfile%" echo %depfile% was generated	
REM ) else ( 
	REM if exist "%srcfile%" del /f %srcfile%
	REM if exist "%depfile%" del /f %depfile%
	REM echo OBJS is null
	REM ������
	REM exit /b 1
REM )
REM ����
REM set TAG=%1

REM nmake.exe /nologo /f Makefile.mak %TAG%
rem ============= main end ===================
goto:eof 

REM ========= ����һ =============
:makeDeplist
set CPPSRCS=
set CSRCS=
set HSRCS=
REM set EXTS=.H .CPP .C
set OBJS2=
set depcmd=
REM set srcfile=%DepDir%\msrcfile.lst
REM set depfile=%DepDir%\mdepfile.lst

REM H%SUFFIX% ��׺
set SUFFIX=SRCS
echo # >%srcfile%
echo # By Simon %curdate% >>%srcfile%
echo # >>%srcfile%

echo # >%depfile%
echo # By Simon %curdate% >>%depfile%
echo # >>%depfile%
SETLOCAL enabledelayedexpansion
rem ֧�ֵ� ��չ���ļ�
for %%x in (%EXTS%) do (
	REM Դ�ļ�Ŀ¼
	for %%i in (%SRCDIRS%) do (
		if exist %%i (
			REM echo %%i\*%%x
			if exist "%%i\*%%x" (
				set /a oneTime=1
				rem �о� /s ��Ŀ¼
				for /f "delims=" %%j in ('dir /b %%i\*%%x') do (
					set /a isExist=0
					for %%e in (%EXCLUDE%) do (
						if "%%~ne" equ "%%~nj" (
							REM echo %%~nj %%~xj
							set /a isExist=1
						)
					)
					if "!isExist!" neq "1" (
						if "!oneTime!" == "1" (
							rem =============== make srcfile =============
							rem ȥ��'.'
							set ddot=%%x
							REM echo !ddot:~1!
							echo.>>%srcfile%
							echo !ddot:~1!%SUFFIX% = ^$^(!ddot:~1!%SUFFIX%^) \>>%srcfile%
						)
						REM echo %%i\%%j \
						echo.			%%i\%%j \>>%srcfile%
						rem *.obj �ų� *.h �ļ�
						if "%%x" neq ".H" (
							set OBJS=!OBJS! ^$^(OUTDIR^)\%%~nj.obj
							set OBJS2=!OBJS2! %%~nj.obj
							rem == make depfile ==
							if "!oneTime!" == "1" (
								echo.>>%depfile%
								echo {%%i}%%x{^$^(OUTDIR^)}.obj::>>%depfile%
								echo.	^$^(CC^) ^$^(CFLAGS^) ^$^(DEFINE^) ^$^(ENCODE^) ^$^(INCDIRS^) /Fo"$(OUTDIR)\\" ^$^(CDBGFLAGS^) ^$^< >>%depfile%
							)
						)
						rem *.h
						if "%%x" equ ".H" (
							set HSRCS=!HSRCS! %%i\%%j
						)
						rem *.c *.obj
						if "%%x" equ ".C" (
							set CSRCS=!CSRCS! %%i\%%j
							rem *.obj
							REM set OBJS=!OBJS! ^$^(OUTDIR^)\%%~nj.obj
						)
						rem *.cpp *.obj
						if "%%x" equ ".CPP" (
							set CPPSRCS=!CPPSRCS! %%i\%%j
							rem *.obj
							REM set OBJS=!OBJS! ^$^(OUTDIR^)\%%~nj.obj
						)
						if "!oneTime!" == "1" (
							set /a oneTime=0
						)
					)
				)
			)
		)
	)
)
REM if "%HSRCS%" neq "" (
	REM echo %HSRCS%
	REM echo.>>%srcfile%
	REM echo HSRCS = ^$^(HSRCS^) \>>%srcfile%
	REM echo.			%HSRCS%>>%srcfile%
REM )
REM if "%CSRCS%" neq "" (
	REM echo %CSRCS%
	REM echo.>>%srcfile%
	REM echo CSRCS = ^$^(CSRCS^) \>>%srcfile%
	REM echo.			%CSRCS%>>%srcfile%
REM )
REM if "%CPPSRCS%" neq "" (
	REM echo %CPPSRCS%
	REM echo.>>%srcfile%
	REM echo CPPSRCS = ^$^(CPPSRCS^) \>>%srcfile%
	REM echo.			%CPPSRCS%>>%srcfile%
REM )
REM *.obj
REM if "%OBJS%" neq "" (
	REM echo %OBJS%
	echo.>>%srcfile%
	echo OBJS = ^$^(OBJS^) \>>%srcfile%
	echo.			%OBJS%>>%srcfile%
REM )
echo.>>%srcfile%
REM === ��������ϵ: �����obj�ļ�����ͬ��ͷ�ļ�*.h ���� ===================
REM ��������ϵ
REM add 2015.02.06
if "%HSRCS%" neq "" (
	echo.>>%depfile%
	echo ############# �����obj�ļ�����ͬ��ͷ�ļ�*.h ���� ###############>>%depfile%
	for %%o in (%OBJS2%) do (
		set depcmd=^$^(OUTDIR^)\%%o:
		REM *.cpp
		for %%s in (%CPPSRCS%) do (
			if "%%~no" equ "%%~ns" (
				set depcmd=!depcmd! %%s
			)
		)
		REM *.c
		for %%s in (%CSRCS%) do (
			if "%%~no" equ "%%~ns" (
				set depcmd=!depcmd! %%s
			)
		)
		REM *.h
		for %%h in (%HSRCS%) do (
			if "%%~no" equ "%%~nh" (
				set depcmd=!depcmd! %%h
				echo.>>%depfile%
				echo !depcmd!>>%depfile%
			)
		)
	)
)
REM ====================== END ============================
ENDLOCAL
goto:eof

rem ======== ������ ����������� ===========
:makeDeplistByOne
rem to make *.cpp
set CPPSRCS=
set CSRCS=
set HSRCS=
REM set OBJS=
set EXTS=.H .C .CPP
set tmpfile=temp.txt
echo.>%tmpfile%
SETLOCAL enabledelayedexpansion
for %%i in (%SRCDIRS%) do (
	if exist %%i (
		if exist "%%i\*.cpp" (
			echo.>>%tmpfile%
			echo CPPSRCS = ^$^(CPPSRCS^) \>>%tmpfile%
			for /f "delims=" %%j in ('dir /b %%i\*.cpp') do (
				REM echo %%i\%%j \
				set CPPSRCS=!CPPSRCS! %%i\%%j
				set OBJS=!OBJS! ^$^(OUTDIR^)\%%~nj.obj
				echo.		%%i\%%j \>>%tmpfile%
			)
		)
	)
)
if "%CPPSRCS%" neq "" (echo %CPPSRCS%)
rem to make *.c 
for %%i in (%SRCDIRS%) do (
	if exist %%i (
		if exist "%%i\*.c" (
			echo.>>%tmpfile%
			echo CSRCS = ^$^(CSRCS^) \>>%tmpfile%
			for /f "delims=" %%j in ('dir /b %%i\*.c') do (
				REM echo %%i\%%j \
				set CSRCS=!CSRCS! %%i\%%j
				set OBJS=!OBJS! ^$^(OUTDIR^)\%%~nj.obj
				echo.		%%i\%%j \>>%tmpfile%
			)
		)
	)
)
if "%CSRCS%" neq "" echo %CSRCS%
rem to make *.h
for %%i in (%SRCDIRS%) do (
	if exist %%i (
		if exist "%%i\*.h" (
			echo.>>%tmpfile%
			echo HSRCS = ^$^(HSRCS^) \>>%tmpfile%
			for /f "delims=" %%j in ('dir /b %%i\*.h') do (
				REM echo %%i\%%j \
				set HSRCS=!HSRCS! %%i\%%j
				echo.		%%i\%%j \>>%tmpfile%
			)
		)
	)
)
echo %OBJS%

if "%HSRCS%" neq "" echo %HSRCS%
if "%CPPSRCS%" neq "" (
	echo.>>%tmpfile%
	echo # to compile *.cpp>>%tmpfile%
	for %%i in (%CPPSRCS%) do (
		echo ^$^(OUTDIR^)\%%~ni.obj: %%i>>%tmpfile%
		echo.	^$^(CC^) ^$^(CFLAGS^) ^$^(DEFINE^) ^$^(ENCODE^) ^$^(INCDIRS^) /Fo"$(OUTDIR)\\" ^$^(CDBGFLAGS^) ^$^?>>%tmpfile%
	)
)
if "%CSRCS%" neq "" (
	echo.>>%tmpfile%
	echo # to compile *.c>>%tmpfile%
	for %%i in (%CSRCS%) do (
		echo ^$^(OUTDIR^)\%%~ni.obj: %%i>>%tmpfile%
		echo.	^$^(CC^) ^$^(CFLAGS^) ^$^(DEFINE^) ^$^(ENCODE^) ^$^(INCDIRS^) /Fo"$(OUTDIR)\\" ^$^(CDBGFLAGS^) ^$^?>>%tmpfile%
	)
)
echo.>>%tmpfile%
ENDLOCAL
goto:eof
