SET PATH=tool\astyle-2.5.1\bin;%PATH%
cd ..\
astyle -f -N -K --style=google --indent=spaces --keep-one-line-statements --recursive src\*.*
astyle -f -N -K --style=google --indent=spaces --keep-one-line-statements --recursive include\*.*
del /S *.orig
pause
