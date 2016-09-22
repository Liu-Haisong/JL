cd  %~dp0

set OR32_NAME=ac690x_MINI
set OBJDUMP=C:\JL\pi32\bin\llvm-objdump
   
set OBJCOPY=C:\JL\pi32\bin\llvm-objcopy
set SDK_TYPE=MINI

cd ../post_build

call download.bat %OR32_NAME% %OBJDUMP% %OBJCOPY% %SDK_TYPE%



pause
