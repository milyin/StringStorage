# set EXAMPLES_PATH to "../../build/examples"
EXAMPLES_PATH="../../build/examples"

echo "cleaning up"
# remove db.* if exists
rm -f db.*

echo "packing files"

# scan files in directory 'data'
for file in data/*; do
    # show file name
    echo $file
    # run command 'pack_file db data/file_name'
    $EXAMPLES_PATH/pack_file db $file
done

