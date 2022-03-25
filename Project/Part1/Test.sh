CODE_DIRECTORY=./Code
TEST_CASE_DIRECTORY=./Test_Cases

clear
(cd Code && make)

for in_file in $CODE_DIRECTORY/*.in; do
    in_file=${in_file::-3}
    echo $in_file
    $CODE_DIRECTORY/tips_lex $in_file.in | diff -I '^INFO.*' - $in_file.correct
done

for in_file in $TEST_CASE_DIRECTORY/*.in; do
    in_file=${in_file::-3}
    echo $in_file
    $CODE_DIRECTORY/tips_lex $in_file.in | diff -I '^INFO.*' - $in_file.correct
done

(cd code && make clean)