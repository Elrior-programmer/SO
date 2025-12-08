#!/bash/bin
K="$1"
P="$2"
pathK=$(realpath "$K")
pathP=$(realpath "$P")
dir="$pathK"
back=""
echo "$dir"
while [ "$dir" != "/" ]; do
    back="../$back"
    dir=$(dirname "$dir")
    echo "$dir"
done
echo "$back$pathP"
ln -s "$back$pathP" "$pathK"