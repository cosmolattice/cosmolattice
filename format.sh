#!/bin/bash

echo "Running clang-format on all source files..."
find ./include -type f \( -name "*.cpp" -o -name "*.h" -o -name "*.hpp" -o -name "*.cc" -o -name "*.cxx" \) -exec clang-format -i {} +
echo "Clang-format completed."
