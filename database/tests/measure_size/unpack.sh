# set EXAMPLES_PATH to "../../build/examples"
EXAMPLES_PATH="../../build/examples"

echo "unpacking files"

# scan files in directory 'data'
for file in data/*; do
    # show file name
    echo $file
    # remove path part from file name
    filename=$(basename $file)
    # run command `unpack_file db file_name out/file_name`
    $EXAMPLES_PATH/unpack_file db $file out/$filename
    # compare files 'data/file_name' and 'out/file_name'
    diff $file out/$filename
done


