#!/bin/bash

set -euo pipefail
IFS=$'\n\t'

SRC=""
FILE=""

while getopts ":s:f:" opt; do
  case "$opt" in
    s) SRC="$OPTARG" ;;
    f) FILE="$OPTARG" ;;
    \?) printf "Invalid option: -%s\n" "$OPTARG" >&2; exit 2 ;;
    :)  printf "Option -%s requires an argument.\n" "$OPTARG" >&2; exit 2 ;;
  esac
done
shift $((OPTIND-1))

if [[ -z "$SRC" ]]; then
  echo "Source (-s) is required"
  exit 1
fi

if [[ -z "$FILE" ]]; then
  [[ -d "$SRC" ]] || { echo "Not a valid directory: $SRC"; exit 1; }

  OUT="$(dirname "$SRC")/$(basename "$SRC").tar.gz"
  tar -czf "$OUT" -C "$(dirname "$SRC")" "$(basename "$SRC")"
  printf "backup created: %s\n" "$OUT"
else
  [[ -d "$SRC" ]] || { echo "Not a valid directory: $SRC"; exit 1; }
  [[ -e "$SRC/$FILE" ]] || { echo "File does not exist: $SRC/$FILE"; exit 1; }

  OUT="$SRC/$FILE.tar.gz"
  tar -czf "$OUT" -C "$SRC" "$FILE"
  printf "backup created: %s\n" "$OUT"
fi
