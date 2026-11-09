$ErrorActionPreference='Stop'
$root=Split-Path $PSScriptRoot -Parent
$exe=Join-Path $root 'TowerDefense2D\x64\Release\TowerDefense2D.exe'
if(-not (Test-Path -LiteralPath $exe)){throw 'Compila Release x64 antes de preparar la entrega.'}
$out=Join-Path $root 'output\PC3'
New-Item -ItemType Directory -Force -Path (Join-Path $out 'data') | Out-Null
Copy-Item -LiteralPath $exe -Destination (Join-Path $out 'TowerDefense2D.exe') -Force
Copy-Item -LiteralPath (Join-Path $root 'README.md') -Destination (Join-Path $out 'README.md') -Force
Copy-Item -LiteralPath (Join-Path $root 'packages\raylib.6.2.0\build\native\LICENSE') -Destination (Join-Path $out 'LICENSE-raylib.txt') -Force
@'
TowerDefense2D - avance PC3

Abre TowerDefense2D.exe para jugar. Windows x64; requiere Microsoft Visual C++ Runtime x64.
JUGAR inicia. Selecciona una tarjeta y haz clic en una casilla marcada.
P pausa; Esc cancela la selección. La X de la ventana cierra.
Después de victoria o derrota puedes reiniciar. F12 guarda data/captura.png.
Los eventos se escriben en data/eventos.csv junto al ejecutable.
Los sprites son temporales; faltan las exportaciones originales de Figma.
Pruebas, código e instrucciones: https://github.com/Alexisga/TowerDefense2D (privado).
'@ | Set-Content -LiteralPath (Join-Path $out 'LEEME.txt') -Encoding utf8
$info=[ordered]@{
    artifact='TowerDefense2D.exe'
    configuration='Release x64'
    sha256=(Get-FileHash -LiteralPath (Join-Path $out 'TowerDefense2D.exe') -Algorithm SHA256).Hash
    source_commit=(& git -C $root rev-parse HEAD)
    packaged_at=(Get-Date).ToUniversalTime().ToString('o')
}
$info | ConvertTo-Json | Set-Content -LiteralPath (Join-Path $root 'docs\evidence\release-artifact.json') -Encoding utf8
Write-Output "Paquete preparado: $out"
