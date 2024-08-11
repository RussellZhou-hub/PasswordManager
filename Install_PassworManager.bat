@echo off
set src_dir=.\bin\x64\Release
set dest_dir=F:\Apps\PasswordManager

:: 创建目标目录（如果不存在）
if not exist "%dest_dir%" (
    mkdir "%dest_dir%"
)

:: 复制所有 .exe 文件到目标目录
copy "%src_dir%\*.exe" "%dest_dir%\"
copy "%src_dir%\*.ttf" "%dest_dir%\"
copy "%src_dir%\*.dll" "%dest_dir%\"

echo Files copied to %dest_dir%
pause