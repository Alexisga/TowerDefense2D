param([ValidateSet('Debug','Release')][string]$Configuration='Debug',[switch]$Tests)
$ErrorActionPreference='Stop'
$root=Split-Path $PSScriptRoot -Parent
$vswhere=Join-Path ${env:ProgramFiles(x86)} 'Microsoft Visual Studio\Installer\vswhere.exe'
$install=& $vswhere -latest -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath
if(-not $install){throw 'Instala Visual Studio con Desarrollo para escritorio con C++.'}
$msbuild=Join-Path $install 'MSBuild\Current\Bin\MSBuild.exe'
$project=if($Tests){'tests\TowerDefense2D.Tests.vcxproj'}else{'TowerDefense2D\TowerDefense2D.vcxproj'}
Push-Location $root
try {
    & $msbuild $project /p:Configuration=$Configuration /p:Platform=x64 /v:minimal
    if($LASTEXITCODE -ne 0){throw "Compilacion fallida: $LASTEXITCODE"}
    if($Tests){& "$root\build\tests\$Configuration\TowerDefense2D.Tests.exe";if($LASTEXITCODE -ne 0){throw 'Pruebas fallidas'}}
} finally {Pop-Location}
