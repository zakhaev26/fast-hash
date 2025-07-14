#!/bin/bash

echo "Running cpplint on project..."

FILES=$(find ./src ./include -name '*.cpp' -o -name '*.hpp' -o -name '*.h' -o -name '*.cc')

for file in $FILES; do
  echo "Linting $file"
  cpplint "$file"
done
