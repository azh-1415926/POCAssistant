@REM 构建路径
set BUILD_PATH=build-wasm

@REM Qt WebAssembly 工具链目录
set QT_WEBASSEMBLY_TOOLCHAIN=D:/Qt/6.5.3/wasm_singlethread

@REM emsdk 路径
set EMSDK=D:/WebAssembly/emsdk

set TOOLCHAIN_FILE=%QT_WEBASSEMBLY_TOOLCHAIN%/lib/cmake/Qt6/qt.toolchain.cmake

@REM emsdk 的程序、文件的路径
set CROSSCOMPILING_EMULATOR=%EMSDK%/node/18.20.3_64bit/bin/node.exe
set CHAINLOAD_TOOLCHAIN_FILE=%EMSDK%/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake
set PYTHONPATH=%EMSDK%/python/3.9.2-nuget_64bit/python.exe

@REM 执行 cmake，构建、编译项目
cmake -B %BUILD_PATH% -S . -GNinja -DCMAKE_TOOLCHAIN_FILE=%TOOLCHAIN_FILE% -DCMAKE_PREFIX_PATH=%QT_WEBASSEMBLY_TOOLCHAIN% -DCMAKE_CROSSCOMPILING_EMULATOR=%CROSSCOMPILING_EMULATOR% -DQT_CHAINLOAD_TOOLCHAIN_FILE=%CHAINLOAD_TOOLCHAIN_FILE%
cmake --build %BUILD_PATH% -j8

@REM 将 qt 的 logo 替换为校徽
copy Jgsu.svg %BUILD_PATH%\qtlogo.svg

@REM 启动 WebAssembly 项目
%PYTHONPATH% %EMSDK%/upstream/emscripten/emrun.py --browser iexplore --port 30000 --no_emrun_detect --serve_after_close %BUILD_PATH%/POCAssistant.html...