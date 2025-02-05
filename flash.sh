#!/bin/bash

# Extract project name from CMakeLists.txt
PROJECT_NAME=$(grep -oP '(?<=^project\().*(?=\))' ../CMakeLists.txt)

# Exit if project name is not found
if [[ -z "$PROJECT_NAME" ]]; then
  echo "Project name not found in CMakeLists.txt"
  exit 1
fi

# Run cmake, make, and then flash using the project name
cmake -DCMAKE_BUILD_TYPE=Release .. &&
make -j4 &&
sudo openocd -f ../pico-debug.cfg -c "program ${PROJECT_NAME}.elf verify reset exit"
