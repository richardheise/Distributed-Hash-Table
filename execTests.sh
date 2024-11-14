#!/bin/bash

for t in tests/*.sol; do
  echo $t
  i=tests/$(basename $t .sol).in
  o=tests/$(basename $t .sol).sol
  ./mydht <$i | diff -u --color $o -
done