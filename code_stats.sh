#! /bin/sh
#
# author: Maciej Chałapuk
# license: apache2
# vim: ts=2 sw=2 expandtab
#
# Requires lc.awk script:
# http://code.google.com/p/line-counting/
#

echo
echo "LIBRARY CODE:"
echo
lc.awk \
  `find include/gyros -name "*.hpp"` \
  `find include/gyros -name "*.tpp"`

echo
echo "TEST CODE:"
echo
lc.awk \
  `find include/test -name "*.hpp"` \
  `find include/test -name "*.tpp"` \
  `find test -name "*.cpp"`

