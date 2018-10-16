#!/bin/sh

source=$(pwd);

cd $(mktemp -d)

$source/xorGraphviz

for filename in xor-[1-4]-*[0-9]; do
    echo "Adding title to file $filename"
    sed -i "/headport/a label=\"$filename\"" $filename 
done

wait

for filename in xor-[1-4]-*[0-9]; do
    echo "Creating image $filename"
    cat $filename | dot -Tpng > $filename.png 
done

wait

for file1 in xor-1-*.png; do
    file2=$(echo $file1 | sed "s/-1-/-2-/")
    file3=$(echo $file1 | sed "s/-1-/-3-/")
    file4=$(echo $file1 | sed "s/-1-/-4-/")
    output=$(echo $file1 | sed "s/xor-1-/neural-/")
    echo "Creating image $output"
    convert \( $file1 $file2 -append \) \
            \( $file3 $file4 -append \) \
            +append $output
done

echo "Creating gif"
convert -delay 5 -loop 0 neural-*.png xor.gif

mv xor.gif $source/xor.gif

rm *
