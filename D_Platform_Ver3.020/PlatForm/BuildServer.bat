@echo off

copy ..\SDK\lib\Server\mac\wCore2.lib ..\SDK\lib\Server\ /Y

@set path=C:\Program Files\Microsoft Visual Studio 8\Common7\IDE;%path%

echo ���±���Server-AFCComDllEx.dll...
del .\Server\AFCComEx\release\*.* /Q
devenv ".\Server\AFCComEx\AFCComDllEx.vcproj" /build release >nul

echo ���±���Server-WServer.exe...
del .\Server\WServer\release\*.* /Q
devenv ".\Server\WServer\AFCLoader.vcproj" /build release >nul

echo ���±���Server-ZServer.exe...
del .\Server\MainServer\release\*.* /Q
devenv ".\Server\MainServer\MainServer.vcproj" /build release >nul

echo ���±���Server-BCS.exe...
del .\Server\CenterServer\release\*.* /Q
devenv ".\Server\CenterServer\CenterServer.vcproj" /build release >nul

echo ���±���Server-GMTools...
del .\Server\GMTools\release\*.* /Q
devenv ".\Server\GMTools\GMTools.vcproj" /build release >nul

pause ������ɣ���������رձ����ڡ�