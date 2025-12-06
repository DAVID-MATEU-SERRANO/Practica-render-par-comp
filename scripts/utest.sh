#!/bin/bash

set -Eeuo pipefail
export LD_LIBRARY_PATH="/opt/gcc-14/lib64${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"

# Se debe ejecutar el script desde la raíz
rm -rf out

cmake --preset clang-tidy 
cmake --build --preset clang-tidy-debug --config Debug --target coverage-utcommon --parallel