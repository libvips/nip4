#!/bin/bash

for test in *.def; do
  echo testing $test ...
  snip --test $test
done

