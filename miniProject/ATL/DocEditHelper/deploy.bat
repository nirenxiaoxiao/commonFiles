@echo off

SETLOCAL
echo ##### ǩ�� ^& ���ɲ���cab��

rem ActiveX �ļ���
set CONTROL_NAME=DocEditHelper

set PATH=%~dp0\DeployTools;%~dp0\DeployTools\cert;%PATH%

pushd %~dp0

md Deploy
md Deploy\tmp

echo ##### ����Ҫ������ļ�
copy Release\DocEditHelper.dll Deploy\tmp\DocEditHelper.dll
copy DocEditHelper.inf Deploy\tmp\DocEditHelper.inf

echo ##### ���Ʋ�����ҳ
copy TestDeploy.htm Deploy\TestDeploy.htm

echo ##### ��ActiveX����ǩ��
signcode.exe -cn ����������Ϣ�������޹�˾ -t http://timestamp.wosign.com/timestamp "Deploy\tmp\DocEditHelper.dll"

echo ##### ���cab�ļ�
makecab /V3 /F DocEditHelper.ddf

echo ##### ǩ��cab�ļ�
signcode.exe -cn ����������Ϣ�������޹�˾ -t http://timestamp.wosign.com/timestamp "Deploy\DocEditHelper.cab"

rem ɾ����ʱ�ļ�
rmdir /S /Q Deploy\tmp

popd

ENDLOCAL