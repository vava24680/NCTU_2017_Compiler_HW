This is Compiler project2.

The grammar is derived from mini pascal grammar text and with some additions.
There are four important files for this project.
* parser.y is the bison file with grammar derived from mini_pascal_grammar this text.
* scanner.l is the lex file which is used to scan each token and return it to bison.
* node.c and node.h is the additional C-file which is responsible to construct the parsing tree.

To compile and run the program with all the test files.
type "make clean_all run_all", the "Parsing result" will show on the terminal and the parsing details(including reducing/shifting details and parsing tree) will be writen to the file in the Test_result(each test files has its own files, e.g. parser_test01_result.txt is the result of parser_test01.p)

There are totally 10 test files.
10 test files:
parser_test01.p
parser_test02.p
parser_test03.p
parser_test04.p
parser_test05.p
parser_test06.p
parser_test07.p
parser_test08.p
parser_test09.p
parser_test10.p
