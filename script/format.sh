astyle -f -N -K --style=google --recursive --indent=spaces --keep-one-line-statements ../src/*.*
astyle -f -N -K --style=google --recursive --indent=spaces --keep-one-line-statements ../include/*.*
find ../src -name "*.orig" -type f -delete