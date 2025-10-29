@echo off
echo ===============================================
echo    COMPILADOR PARA PROJETOS OPENGL/FREEGLUT
echo ===============================================

:: Verifica se o compilador existe
where g++ >nul 2>nul
if %errorlevel% neq 0 (
    echo ERRO: g++ nao encontrado!
    echo.
    echo Instale o MinGW-w64 ou MSYS2:
    echo - MinGW-w64: https://www.mingw-w64.org/downloads/
    echo - MSYS2: https://www.msys2.org/
    echo.
    echo Ou instale via chocolatey:
    echo choco install mingw
    pause
    exit /b 1
)

:: Configurações de compilação
set INCLUDE_DIRS=-I"C:\Program Files\freeglut\include" -I"C:\msys64\mingw64\include"
set LIB_DIRS=-L"C:\Program Files\freeglut\lib" -L"C:\msys64\mingw64\lib"
set LIBS=-lfreeglut -lopengl32 -lglu32
set COMPILER_FLAGS=-Wall -O2 -std=c++17

echo.
echo Arquivos .cpp encontrados:
echo.
for %%f in (*.cpp) do echo   %%f

echo.
echo Escolha uma opcao:
echo.
echo 1. Compilar triangles_basic.cpp
echo 2. Compilar triangles_lighting.cpp  
echo 3. Compilar triangles_textured.cpp
echo 4. Compilar todos os arquivos
echo 5. Limpar arquivos executaveis
echo 0. Sair
echo.

set /p choice="Digite sua escolha (0-6): "

if "%choice%"=="1" goto compile_basic
if "%choice%"=="2" goto compile_lighting
if "%choice%"=="3" goto compile_textured
if "%choice%"=="4" goto compile_all
if "%choice%"=="5" goto clean
if "%choice%"=="0" goto end

echo Opcao invalida!
goto end

:compile_basic
echo.
echo Compilando triangles_basic.cpp...
g++ %COMPILER_FLAGS% %INCLUDE_DIRS% triangles_basic.cpp %LIB_DIRS% %LIBS% -o triangles_basic.exe
if %errorlevel%==0 (
    echo ✓ Compilacao bem-sucedida: triangles_basic.exe
) else (
    echo ✗ Erro na compilacao!
)
goto end

:compile_lighting
echo.
echo Compilando triangles_lighting.cpp...
g++ %COMPILER_FLAGS% %INCLUDE_DIRS% triangles_lighting.cpp %LIB_DIRS% %LIBS% -o triangles_lighting.exe
if %errorlevel%==0 (
    echo ✓ Compilacao bem-sucedida: triangles_lighting.exe
) else (
    echo ✗ Erro na compilacao!
)
goto end

:compile_textured
echo.
echo Compilando triangles_textured.cpp...
g++ %COMPILER_FLAGS% %INCLUDE_DIRS% triangles_textured.cpp %LIB_DIRS% %LIBS% -o triangles_textured.exe
if %errorlevel%==0 (
    echo ✓ Compilacao bem-sucedida: triangles_textured.exe
) else (
    echo ✗ Erro na compilacao!
)
goto end

:compile_all
echo.
echo Compilando todos os arquivos...
echo.

echo Compilando triangles_basic.cpp...
g++ %COMPILER_FLAGS% %INCLUDE_DIRS% triangles_basic.cpp %LIB_DIRS% %LIBS% -o triangles_basic.exe
if %errorlevel%==0 (
    echo ✓ triangles_basic.exe compilado
) else (
    echo ✗ Erro ao compilar triangles_basic.cpp
)

echo Compilando triangles_lighting.cpp...
g++ %COMPILER_FLAGS% %INCLUDE_DIRS% triangles_lighting.cpp %LIB_DIRS% %LIBS% -o triangles_lighting.exe
if %errorlevel%==0 (
    echo ✓ triangles_lighting.exe compilado
) else (
    echo ✗ Erro ao compilar triangles_lighting.cpp
)

echo Compilando triangles_textured.cpp...
g++ %COMPILER_FLAGS% %INCLUDE_DIRS% triangles_textured.cpp %LIB_DIRS% %LIBS% -o triangles_textured.exe
if %errorlevel%==0 (
    echo ✓ triangles_textured.exe compilado
) else (
    echo ✗ Erro ao compilar triangles_textured.cpp
)

echo Compilando main.cpp...
g++ %COMPILER_FLAGS% %INCLUDE_DIRS% main.cpp %LIB_DIRS% %LIBS% -o main.exe
if %errorlevel%==0 (
    echo ✓ main.exe compilado
) else (
    echo ✗ Erro ao compilar main.cpp
)

echo.
echo ✓ Compilacao completa!
goto end

:clean
echo.
echo Limpando arquivos executaveis...
if exist *.exe (
    del *.exe
    echo ✓ Arquivos .exe removidos
) else (
    echo Nenhum arquivo .exe encontrado
)
goto end

:end
echo.
echo ===============================================
echo Para executar:
echo   triangles_basic.exe     - Triangulos basicos
echo   triangles_lighting.exe  - Com iluminacao
echo   triangles_textured.exe  - Com texturas
echo   main.exe               - Programa original
echo.
echo Para monitorar sistema:
echo   python system_monitor.py --monitor
echo.
echo Para analisar resultados:
echo   python analyze_fps.py
echo ===============================================
pause