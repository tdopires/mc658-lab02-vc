#!/bin/bash
# EXACT
./color.e -e -i in04/gr_15 -o out04/gr_15.exact.out
./color.e -e -i in04/gr_20 -o out04/gr_20.exact.out
./color.e -e -i in04/gr_30 -o out04/gr_30.exact.out
./color.e -e -i in04/gr_40 -o out04/gr_40.exact.out
./color.e -e -i in04/gr_70 -o out04/gr_70.exact.out
./color.e -e -i in04/gr_100 -o out04/gr_100.exact.out
./color.e -e -i in04/gr_sparse_50 -o out04/gr_sparse_50.exact.out
./color.e -e -i in04/gr_sparse_60 -o out04/gr_sparse_60.exact.out
#HEURISTIC
./color.e -h -i in04/gr_15 -o out04/gr_15.heuristic.out
./color.e -h -i in04/gr_20 -o out04/gr_20.heuristic.out
./color.e -h -i in04/gr_30 -o out04/gr_30.heuristic.out
./color.e -h -i in04/gr_40 -o out04/gr_40.heuristic.out
./color.e -h -i in04/gr_70 -o out04/gr_70.heuristic.out
./color.e -h -i in04/gr_100 -o out04/gr_100.heuristic.out
./color.e -h -i in04/gr_sparse_50 -o out04/gr_sparse_50.heuristic.out
./color.e -h -i in04/gr_sparse_60 -o out04/gr_sparse_60.heuristic.out
