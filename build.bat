@echo off
echo ===============================================
echo CS:GO NAC (Not A Cheat) - Build Script
echo Professional Gaming Enhancement DLL
echo ===============================================
echo.

:: Check if Visual Studio is available
where cl >nul 2>nul
if %errorlevel% neq 0 (
    echo [ERROR] Visual Studio compiler not found!
    echo Please run this script from a Visual Studio Developer Command Prompt
    echo or install Visual Studio with C++ development tools.
    pause
    exit /b 1
)

:: Check if CMake is available
where cmake >nul 2>nul
if %errorlevel% neq 0 (
    echo [ERROR] CMake not found!
    echo Please install CMake and add it to your PATH.
    pause
    exit /b 1
)

:: Set build configuration
set BUILD_TYPE=Release
set ARCHITECTURE=x64

echo [INFO] Build Configuration:
echo   - Type: %BUILD_TYPE%
echo   - Architecture: %ARCHITECTURE%
echo   - Compiler: Visual Studio
echo.

:: Create build directory
if not exist "build" (
    echo [INFO] Creating build directory...
    mkdir build
)

cd build

:: Configure with CMake
echo [INFO] Configuring project with CMake...
cmake .. -G "Visual Studio 16 2019" -A %ARCHITECTURE% -DCMAKE_BUILD_TYPE=%BUILD_TYPE%

if %errorlevel% neq 0 (
    echo [ERROR] CMake configuration failed!
    cd ..
    pause
    exit /b 1
)

:: Build the project
echo [INFO] Building CS:GO NAC DLL...
cmake --build . --config %BUILD_TYPE% --parallel

if %errorlevel% neq 0 (
    echo [ERROR] Build failed!
    cd ..
    pause
    exit /b 1
)

cd ..

:: Check if DLL was created successfully
if exist "build\bin\csgo_nac.dll" (
    echo.
    echo ===============================================
    echo [SUCCESS] Build completed successfully!
    echo ===============================================
    echo.
    echo DLL Location: build\bin\csgo_nac.dll
    echo Output Directory: output\csgo_nac.dll
    echo.
    
    :: Copy to output directory
    if not exist "output" mkdir output
    copy "build\bin\csgo_nac.dll" "output\csgo_nac.dll" >nul
    
    echo [INFO] DLL copied to output directory
    echo.
    echo Next Steps:
    echo 1. Use a DLL injector to inject csgo_nac.dll into CS:GO
    echo 2. Press INSERT key in-game to open the NAC menu
    echo 3. Configure your preferred settings
    echo 4. Enjoy enhanced gaming experience!
    echo.
    echo Remember: NAC - Not A Cheat. Play fair, play better.
    echo.
) else (
    echo [ERROR] DLL was not created! Check build output for errors.
    pause
    exit /b 1
)

pause