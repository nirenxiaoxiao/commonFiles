@echo off

SETLOCAL
echo ##### 签名 ^& 生成部署cab包

rem ActiveX 文件名
set CONTROL_NAME=DocEditHelper

set PATH=%~dp0\DeployTools;%~dp0\DeployTools\cert;%PATH%

pushd %~dp0

md Deploy
md Deploy\tmp

echo ##### 复制要打包的文件
copy Release\DocEditHelper.dll Deploy\tmp\DocEditHelper.dll
copy DocEditHelper.inf Deploy\tmp\DocEditHelper.inf

echo ##### 复制测试网页
copy TestDeploy.htm Deploy\TestDeploy.htm

echo ##### 对ActiveX进行签名
signcode.exe -cn 北京华宇信息技术有限公司 -t http://timestamp.wosign.com/timestamp "Deploy\tmp\DocEditHelper.dll"

echo ##### 打包cab文件
makecab /V3 /F DocEditHelper.ddf

echo ##### 签名cab文件
signcode.exe -cn 北京华宇信息技术有限公司 -t http://timestamp.wosign.com/timestamp "Deploy\DocEditHelper.cab"

rem 删除临时文件
rmdir /S /Q Deploy\tmp

popd

ENDLOCAL