# check if clang-format is installed
if ! command -v clang-format &> /dev/null
then
    echo "clang-format could not be found"
    exit
fi

for src_file in $(find $PWD/Classes -iname "*.cpp" -or -iname "*.h"); do
    echo "Format $src_file"
    clang-format -i $src_file
done

# find "./Classes" \( -iname "*.cpp" -o -iname "*.h" \) -exec clang-format -i {} \;
