#!/bin/bash

PYTHON=/usr/bin/python3

BASE=`dirname $0`
BASE=`cd ${BASE} && pwd`
cd ${BASE}

PATCHES=${BASE}/patches

VERSION=$(cat ${BASE}/VERSION)
V1=upstream
V2=upstream-patched

if [ ! -e "${V1}" ]
then
  echo "${BASE}/${V1} not found"
  exit
fi

if [ ! -e "${V2}" ]
then
  echo "${BASE}/${V2} not found"
  exit
fi

rm -rf ${PATCHES}
mkdir -p ${PATCHES}/tmp/

diff -ruN --no-dereference ${V1} ${V2} > ${PATCHES}/tmp/p

( cd ${PATCHES}/tmp/
  splitpatch ${PATCHES}/tmp/p
  rm ${PATCHES}/tmp/p
  for X in *
  do
    Y=$(echo ${X} \
        | head -n 1 ${X} \
        | tr '\t' ' ' \
        | cut -d ' ' -f 2 \
        | cut -d'/' -f 2- \
        | tr '/' '_')
  mv ${X} ${Y}
  sed -i '1,2 s/\t.*$//' ${Y}
  sed -i '$ s/diff.*//' ${Y}
  done
)
mv ${PATCHES}/tmp/* ${PATCHES}
rm -rf ${PATCHES}/tmp/
