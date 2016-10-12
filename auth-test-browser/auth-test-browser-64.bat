@echo off

call "%~dp0\o4w-build-env-64.bat"

start "QGIS Auth Test Browser" /B Z:\DepsSrc\qgis-auth-test-browser\bin\qgisauthtestbrowser.exe %*
