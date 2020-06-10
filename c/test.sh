#!/bin/bash
make -C src
rm -r output
mkdir -p output/test_data
for f in test_data/*.lsp
do
    # TA's version
    echo -n "Judging $f "
    ./csmli_mac $f > $f.ta.out
    echo -n " [TA]"
    ./src/minilisp $f > $f.me.out
    echo -n "[ME] "
    msg=$(diff $f.ta.out $f.me.out)
    if [[ $? -eq 0   ]]; then
        echo "[AC]"
    else
        echo "[WA]"
        cat $f
        echo "Correct Output:"
        printf "$msg\n"
        echo "^ Your Output."
    fi
done

