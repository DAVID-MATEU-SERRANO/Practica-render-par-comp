#!/bin/bash

set -Eeuo pipefail
export LD_LIBRARY_PATH="/opt/gcc-14/lib64${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"

# Se debe ejecutar el script desde la raíz
rm -rf out

cmake --preset default 
cmake --build --preset gcc-release --config Release --target all --parallel

perf stat -r 5 ./out/build/default/par/Release/render-par ./config_full/config4.txt ./scene/scene4.txt ./out_full/4out.txt