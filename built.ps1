#ps1 para compilar
Write-Host "Compilando el proyecto en Donut..." -ForegroundColor Green

#configurar ruta
$EMSDKPath = "C:\emsdk"
. "$EMSDKPath\emsdk_env.ps1"

#compilar
em++ .\docs\donut_2.cpp `
    -O3 `
    -s WASM=1 `
    -s MODULARIZE=1 `
    -s EXPORT_ES6=1 `
    -s EXPORTED_FUNCTIONS='["_render_donut"]' `
    -s EXPORTED_RUNTIME_METHODS='["cwrap"]' `
    -o public\donut_engine.js

Write-Host "Compilando HASHING..." -ForegroundColor Cyan

em++ .\docs\hashing.cpp `
    -O3 `
    -s WASM=1 `
    -s MODULARIZE=1 `
    -s EXPORT_ES6=1 `
    -s EXPORTED_FUNCTIONS='["_calcular_hash_wasm", "_malloc", "_free"]' `
    -s EXPORTED_RUNTIME_METHODS='["cwrap", "ccall", "stringToUTF8", "lengthBytesUTF8"]' `
    -o public\hashing_engine.js