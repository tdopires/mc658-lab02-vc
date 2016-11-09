#!/bin/bash
# EXACT
./color.e -e -v -i in04/gr_15 -o out04/gr_15.exact.out && read -p 'Press ENTER key...'
./color.e -e -v -i in04/gr_20 -o out04/gr_20.exact.out && read -p 'Press ENTER key...'
./color.e -e -v -i in04/gr_30 -o out04/gr_30.exact.out && read -p 'Press ENTER key...'
./color.e -e -v -i in04/gr_40 -o out04/gr_40.exact.out && read -p 'Press ENTER key...'
./color.e -e -v -i in04/gr_70 -o out04/gr_70.exact.out && read -p 'Press ENTER key...'
./color.e -e -v -i in04/gr_100 -o out04/gr_100.exact.out && read -p 'Press ENTER key...'
./color.e -e -v -i in04/gr_sparse_50 -o out04/gr_sparse_50.exact.out && read -p 'Press ENTER key...'
./color.e -e -v -i in04/gr_sparse_60 -o out04/gr_sparse_60.exact.out && read -p 'Press ENTER key...'
# HEURISTIC
./color.e -h -v -i in04/gr_15 -o out04/gr_15.heuristic.out && read -p 'Press ENTER key...'
./color.e -h -v -i in04/gr_20 -o out04/gr_20.heuristic.out && read -p 'Press ENTER key...'
./color.e -h -v -i in04/gr_30 -o out04/gr_30.heuristic.out && read -p 'Press ENTER key...'
./color.e -h -v -i in04/gr_40 -o out04/gr_40.heuristic.out && read -p 'Press ENTER key...'
./color.e -h -v -i in04/gr_70 -o out04/gr_70.heuristic.out && read -p 'Press ENTER key...'
./color.e -h -v -i in04/gr_100 -o out04/gr_100.heuristic.out && read -p 'Press ENTER key...'
./color.e -h -v -i in04/gr_sparse_50 -o out04/gr_sparse_50.heuristic.out && read -p 'Press ENTER key...'
./color.e -h -v -i in04/gr_sparse_60 -o out04/gr_sparse_60.heuristic.out && read -p 'Press ENTER key...'
