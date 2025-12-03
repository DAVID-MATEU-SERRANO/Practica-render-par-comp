#!/bin/bash

set -Eeuo pipefail
export LD_LIBRARY_PATH="/opt/gcc-14/lib64${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"

# Se debe ejecutar el script desde la raíz
 
perf stat -r 5 -e power/energy-pkg/,power/energy-cores/,power/energy-ram/ ./out/build/default/par/Release/render-par ./config_full/config3.txt ./scene/scene3.txt ./out_full/3out.txt
