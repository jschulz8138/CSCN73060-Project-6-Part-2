@echo off
setlocal

:: Change directory to the project root
cd /d "%~dp0"

echo === Checking for vcpkg directory ===
if not exist "vcpkg" (
    echo Cloning vcpkg...
    git submodule update --init --recursive
    if errorlevel 1 (
        echo Failed to clone vcpkg. Exiting...
        pause
        exit /b
    )
) else (
    echo vcpkg already exists. Pulling latest updates...
    cd vcpkg
    git pull
    if errorlevel 1 (
        echo Failed to update vcpkg. Exiting...
        pause
        exit /b
    )
    cd ..
)

:: Bootstrap vcpkg
echo === Bootstrapping vcpkg ===
echo === Julian is a poopy pants ===
cd vcpkg
if not exist "vcpkg.exe" (
    echo Running bootstrap-vcpkg...
    bootstrap-vcpkg.bat
    if errorlevel 1 (
        echo Failed to bootstrap vcpkg. Exiting...
        pause
        exit /b
    )
) else (
    echo vcpkg already bootstrapped.
)

:: Install libpqxx
echo === Installing libpqxx ===
vcpkg install libpqxx
if errorlevel 1 (
    echo Failed to install libpqxx. Continuing...
)

:: Integrate with Visual Studio
echo === Integrating vcpkg with Visual Studio ===
vcpkg integrate install
if errorlevel 1 (
    echo Failed to integrate vcpkg with Visual Studio. Continuing...
)

echo === Setup Complete! ===
pause
exit /b
