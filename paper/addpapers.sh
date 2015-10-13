#!/bin/sh
./paperclient localhost -a 'Manolis' 'UNIX Multiprocessing' assignment-1.pdf
./paperclient localhost -a 'Spyros Voulgaris' 'Sockets' assignment-2.pdf
./paperclient localhost -a 'Spyros Voulgaris' 'RPC' assignment-3.pdf
./paperclient localhost -a 'Patricia Lago' 'Project Template SOD' ProjectTemplate-SOD15.docx
./paperclient localhost -l

echo "Performing md5sum checks:"
md5sum assignment-1.pdf
./paperclient localhost -f 0 | md5sum
md5sum assignment-2.pdf
./paperclient localhost -f 1 | md5sum
md5sum assignment-3.pdf
./paperclient localhost -f 2 | md5sum
md5sum ProjectTemplate-SOD15.docx
./paperclient localhost -f 3 | md5sum

# ./paperclient localhost -f 1