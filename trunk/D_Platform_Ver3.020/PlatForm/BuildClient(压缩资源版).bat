@echo off

attrib -r -h -s BuildTime.* /s

@set path=C:\Program Files\Microsoft Visual Studio 8\Common7\IDE;%path%

echo ���±���Record\PRStructExchange
del .\Record\PRStructExchange\release\*.* /Q
devenv ".\Record\PRStructExchange\PRStructExchange.vcproj" /build release >nul

rem echo ���±���Client\Resource2003
rem del .\Client\Resource2003\�޼���֧��(��Դѹ��)\*.* /Q
rem devenv ".\Client\Resource2003\AFCResDll.vcproj" /build �޼���֧��(��Դѹ��) >nul

rem echo ���±���Client\UILibrary2003
rem del .\Client\UILibrary2003\�޼���֧��(��Դѹ��)\*.* /Q
rem devenv ".\Client\UILibrary2003\AFC.vcproj" /build �޼���֧��(��Դѹ��) >nul

echo ���±���Record\RecordModule
del .\Record\RecordModule\release\*.* /Q
devenv ".\Record\RecordModule\RecordModule.vcproj" /build release >nul

echo ���±���Record\RecordPlayer
del .\Record\RecordPlayer\release\*.* /Q
devenv ".\Record\RecordPlayer\RecordPlayer.vcproj" /build release >nul

rem echo ���±���Client\MainFrame
rem del .\Client\MainFrame\release\*.* /Q
rem devenv ".\Client\MainFrame\GamePlace.vcproj" /build release >nul

pause ������ɣ���������رձ�����