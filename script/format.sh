astyle -f -N -K --style=google --recursive --indent=spaces --keep-one-line-statements ../src/*.c
astyle -f -N -K --style=google --recursive --indent=spaces --keep-one-line-statements ../src/*.h
astyle -f -N -K --style=google --recursive --indent=spaces --keep-one-line-statements ../include/*.h
find ../src -name "*.orig" -type f -delete
