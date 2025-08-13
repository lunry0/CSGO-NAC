@echo off
echo ===============================================
echo CS:GO NAC (Not A Cheat) - Injection Helper
echo Professional Gaming Enhancement DLL
echo ===============================================
echo.

:: Check if DLL exists
if not exist "output\csgo_nac.dll" (
    echo [ERROR] csgo_nac.dll not found in output directory!
    echo Please build the project first using build.bat
    pause
    exit /b 1
)

echo [INFO] CS:GO NAC DLL found: output\csgo_nac.dll
echo.

:: Check if CS:GO is running
tasklist /FI "IMAGENAME eq cs2.exe" 2>NUL | find /I /N "cs2.exe">NUL
if "%ERRORLEVEL%"=="0" (
    echo [INFO] CS:GO 2 process detected
    set PROCESS_NAME=cs2.exe
    goto :inject
)

tasklist /FI "IMAGENAME eq csgo.exe" 2>NUL | find /I /N "csgo.exe">NUL
if "%ERRORLEVEL%"=="0" (
    echo [INFO] CS:GO process detected
    set PROCESS_NAME=csgo.exe
    goto :inject
)

echo [WARNING] CS:GO process not found!
echo Please make sure CS:GO is running before injection.
echo.
echo Supported processes:
echo   - cs2.exe (Counter-Strike 2)
echo   - csgo.exe (Counter-Strike: Global Offensive)
echo.
echo Press any key to continue anyway or Ctrl+C to cancel...
pause >nul

:inject
echo.
echo ===============================================
echo INJECTION INSTRUCTIONS
echo ===============================================
echo.
echo Since this is a demonstration script, you'll need to use
echo a proper DLL injector to inject the NAC DLL into CS:GO.
echo.
echo Recommended DLL Injectors:
echo   1. Process Hacker (Free, Open Source)
echo   2. Extreme Injector (Free)
echo   3. Xenos Injector (Free)
echo   4. Manual DLL injection tools
echo.
echo Steps to inject:
echo   1. Open your preferred DLL injector as Administrator
echo   2. Select the CS:GO process (%PROCESS_NAME%)
echo   3. Browse and select: %CD%\output\csgo_nac.dll
echo   4. Click Inject
echo   5. Wait for successful injection message
echo   6. Press INSERT key in CS:GO to open NAC menu
echo.
echo ===============================================
echo IMPORTANT SAFETY NOTES
echo ===============================================
echo.
echo - NAC is designed for legitimate gaming enhancement only
echo - Use at your own risk and responsibility
echo - Ensure you comply with game terms of service
echo - This tool is for educational purposes
echo - No cheating functionality is included
echo.
echo Features available after injection:
echo   - Performance optimization (FPS boost, low latency)
echo   - Visual customization (crosshair, HUD, themes)
echo   - Enhanced user interface with smooth animations
echo   - Professional purple-black theme
echo   - 50+ configuration options
echo.
echo Press INSERT key in-game to access the NAC menu.
echo.
echo Remember: NAC - Not A Cheat. Play fair, play better.
echo.

pause