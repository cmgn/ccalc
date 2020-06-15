#!/usr/bin/env bash

make
while true; do
  EXPR=$(./fuzzer.py -c 25)
  echo "running ${EXPR}"
  ./main "${EXPR}" >/dev/null
  if [[ $? -ne 0 ]] && [[ $? -ne 2 ]]; then
    echo "failed for ${EXPR}"
    exit 1
  fi
done
