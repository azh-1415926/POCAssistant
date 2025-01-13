#!/bin/bash

if [ -z "$1" ]; then
    BUILD_WASM=1
else
    BUILD_WASM=$1
fi

# WASM 配置
if [ $BUILD_WASM -eq 1 ]; then
    echo "Configure WASM."

    # WASM 项目启动的浏览器
    if [ -z "$2" ]; then
        BROWSER=safari
    else
        BROWSER=$2
    fi
    
    # 构建目录
    BUILD_PATH=build-wasm
    # Qt WebAssembly 工具链目录
    QT_WEBASSEMBLY=/Users/azh/Qt/6.7.3/wasm_singlethread
    # EMSDK
    EMSDK_ROOT=/Users/azh/Qt/WebAssembly/emsdk
    # python 路径
    PYTHON_PATH=$EMSDK_ROOT/python/3.9.2_64bit/bin/python3
# Executable 配置
else
    echo "Configure Executable."

    # 构建目录
    BUILD_PATH=build

    QT_TOOLCHAIN=/Users/azh/Qt/6.7.3/macos
fi

# 判断当前平台
OS="`uname`"
case $OS in
  'Linux')
    OS='Linux'
    alias ls='ls --color=auto'
    ;;
  'WindowsNT')
    OS='Windows'
    ;;
  'Darwin') 
    OS='Mac'
    ;;
  *) ;;
esac

if [ $OS = "Mac" ]; then
    echo "Platform : Mac."
elif [ $OS = "Linux"]; then
    echo "Platform : Linux."
else
    echo "Not Support."
    exit
fi

if [ $BUILD_WASM -eq 0 ]; then
    echo "Building for Executable."

    cmake -S . -B $BUILD_PATH -DCMAKE_PREFIX_PATH=$QT_TOOLCHAIN
else
    echo "Building for WASM."

    # Qt WebAssembly 工具链目录
    export QT_WEBASSEMBLY_TOOLCHAIN=$QT_WEBASSEMBLY
    # emsdk
    export EMSDK=$EMSDK_ROOT

    export TOOLCHAIN_FILE=$QT_WEBASSEMBLY_TOOLCHAIN/lib/cmake/Qt6/qt.toolchain.cmake

    export PYTHON=$PYTHON_PATH

    # 执行 cmake，构建、编译项目

    cmake -GNinja -S . -B $BUILD_PATH \
        -DCMAKE_MAKE_PROGRAM=$QT_WEBASSEMBLY_TOOLCHAIN/../../Tools/Ninja/ninja \
        -DCMAKE_PREFIX_PATH:PATH=$QT_WEBASSEMBLY_TOOLCHAIN \
        -DCMAKE_GENERATOR:STRING=Ninja \
        -DQT_QMAKE_EXECUTABLE:FILEPATH=$QT_WEBASSEMBLY_TOOLCHAIN/bin/qmake \
        -DCMAKE_BUILD_TYPE:STRING=Release \
        -DCMAKE_C_COMPILER:FILEPATH=$EMSDK/upstream/emscripten/emcc \
        -DCMAKE_TOOLCHAIN_FILE:FILEPATH=$TOOLCHAIN_FILE \
        -DCMAKE_CXX_COMPILER:FILEPATH=$EMSDK/upstream/emscripten/em++
fi

cmake --build $BUILD_PATH -j8 --target all

if [ $BUILD_WASM -eq 1 ]; then
   # 将 qt 的 logo 替换为校徽
    cp Jgsu.svg $BUILD_PATH/qtlogo.svg

    # 启动 WebAssembly 项目
    $PYTHON $EMSDK/upstream/emscripten/emrun.py --browser $BROWSER --port 30000 --no_emrun_detect --serve_after_close $BUILD_PATH/POCAssistant.html
elif [ $BUILD_WASM -eq 0 ]; then
    echo "Running on $OS."

    if [ $OS = "Mac" ]; then
        ./$BUILD_PATH/POCAssistant.app/Contents/MacOS/POCAssistant
    else
        ./$BUILD_PATH/POCAssistant
    fi
else
    echo "Please set BUILD_WASM to 0 or 1."
fi