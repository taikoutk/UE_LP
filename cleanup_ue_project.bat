@echo off
chcp 65001 >nul
title Unreal Engine Project Cleanup

echo ================================
echo  Unreal Engine Cleanup Script
echo ================================
echo.

set FOLDERS=.vs Binaries Intermediate Saved

for %%F in (%FOLDERS%) do (
    if exist "%%F" (
        echo Удаление папки: %%F
        rmdir /s /q "%%F"
    ) else (
        echo Папка не найдена: %%F
    )
)

echo.
echo Очистка завершена.
pause
