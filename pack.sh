#!/bin/sh
make
cp taisto2 t
7z a t2.7z t
echo 'tail -n+2 $0|7z -si x;chmod +x t;./t;rm t;exit' > taisto
cat t2.7z >> taisto
chmod +x taisto
du -b taisto
