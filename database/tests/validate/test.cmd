set NODE_SIZE=%1
set INPUT_SIZE=%2

cmake -S../.. -Bbuild -DBTREE_NODE_SIZE=%NODE_SIZE% -DBTREE_KEY_SIZE=128
cmake --build build --target examples --config Debug

randomfile.exe %INPUT_SIZE% > randomkeys.txt
build\examples\Debug\pack db randomkeys.txt randomkeys.txt
build\examples\Debug\unpack db randomkeys.txt randomkeys.out
fc randomkeys.txt randomkeys.out
