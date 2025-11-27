#!/bin/bash

set -Eeuo pipefail
export LD_LIBRARY_PATH="/opt/gcc-14/lib64${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"

# Se debe ejecutar el script desde la raíz 
# Como la generación del PPM es idéntica para aos y soa, se utilizará aos

# Ejecutar para los 4 casos de prueba
./out/build/default/aos/Release/render-aos ./config_full/config1.txt ./scene/scene1.txt ./out_full/1out.ppm
./out/build/default/aos/Release/render-aos ./config_full/config2.txt ./scene/scene2.txt ./out_full/2out.ppm
./out/build/default/aos/Release/render-aos ./config_full/config3.txt ./scene/scene3.txt ./out_full/3out.ppm
./out/build/default/aos/Release/render-aos ./config_full/config4.txt ./scene/scene4.txt ./out_full/4out.ppm

# Comparar con las referencias
python3 ./scripts/compare.py ./out_full/1out.ppm ./out_full/1s.ppm
python3 ./scripts/compare.py ./out_full/2out.ppm ./out_full/2s.ppm
python3 ./scripts/compare.py ./out_full/3out.ppm ./out_full/3s.ppm
python3 ./scripts/compare.py ./out_full/4out.ppm ./out_full/4s.ppm