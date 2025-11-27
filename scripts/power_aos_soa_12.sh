#!/bin/bash

set -Eeuo pipefail
export LD_LIBRARY_PATH="/opt/gcc-14/lib64${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"

# Se debe ejecutar el script desde la raíz 

perf stat -r 5 -e power/energy-pkg/,power/energy-cores/,power/energy-ram/ ./out/build/default/aos/Release/render-aos ./config_full/config1.txt ./scene/scene1.txt ./out_full/1out.txt

perf stat -r 5 -e power/energy-pkg/,power/energy-cores/,power/energy-ram/ ./out/build/default/aos/Release/render-aos ./config_full/config2.txt ./scene/scene2.txt ./out_full/2out.txt

perf stat -r 5 -e power/energy-pkg/,power/energy-cores/,power/energy-ram/ ./out/build/default/soa/Release/render-soa ./config_full/config1.txt ./scene/scene1.txt ./out_full/1out.txt

perf stat -r 5 -e power/energy-pkg/,power/energy-cores/,power/energy-ram/ ./out/build/default/soa/Release/render-soa ./config_full/config2.txt ./scene/scene2.txt ./out_full/2out.txt