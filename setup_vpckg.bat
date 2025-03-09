@echo off
setlocal

:: Change directory to the project root
cd /d "%~dp0"

echo === Checking for vcpkg directory ===
if not exist "vcpkg" (
    echo Cloning vcpkg...
    git submodule update --init --recursive
) else (
    echo vcpkg already exists. Pulling latest updates...
    cd vcpkg
    git pull
    cd ..
)

:: Bootstrap vcpkg
echo === Bootstrapping vcpkg ===
cd vcpkg
if not exist "vcpkg.exe" (
    echo Running bootstrap-vcpkg...
    bootstrap-vcpkg.bat
) else (
    echo vcpkg already bootstrapped.
)

:: Install libpqxx
echo === Installing libpqxx ===
vcpkg install libpqxx

:: Integrate with Visual Studio
echo === Integrating vcpkg with Visual Studio ===
vcpkg integrate install

echo === Setup Complete! ===
pause
exit /b