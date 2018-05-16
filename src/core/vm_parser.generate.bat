del vm.parser.l.c
del vm.parser.tab.c
SET PATH=..\..\tool\bison-flex-win-2.7;%PATH%
win_flex -t vm_parser.l > vm_parser.l.c
win_bison -d vm_parser.y
pause