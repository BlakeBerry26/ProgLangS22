CODE_DIRECTORY=./Code
TEST_CASE_DIRECTORY=./Test_Cases

clear
(cd Code && make)

for in_file in $CODE_DIRECTORY/*.pas; do
    in_file=${in_file::-4}
    echo $in_file
    $CODE_DIRECTORY/tips $in_file.pas | diff -I '^INFO.*' - $in_file.correct
done

for in_file in $TEST_CASE_DIRECTORY/*.pas; do
    in_file=${in_file::-4}
    echo $in_file
    $CODE_DIRECTORY/tips $in_file.pas | diff -I '^INFO.*' - $in_file.correct
done

(cd Code && make clean)