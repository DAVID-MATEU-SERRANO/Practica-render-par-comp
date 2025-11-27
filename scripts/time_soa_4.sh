#!/bin/bash

set -Eeuo pipefail
export LD_LIBRARY_PATH="/opt/gcc-14/lib64${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"

# Se debe ejecutar el script desde la raíz 

perf stat -r 5 ./out/build/default/soa/Release/render-soa ./config_full/config4.txt ./scene/scene4.txt ./out_full/4out.txt