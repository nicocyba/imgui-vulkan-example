#!/bin/bash
set -euo pipefail

clear

# Usage: ./eval.sh [--tidy|--no-tidy]
# Default is to run clang-tidy. Pass --no-tidy to skip it.
TIDY=0
for arg in "$@"; do
	case "$arg" in
		--no-tidy) TIDY=0 ;;
		--tidy)    TIDY=1 ;;
		-h|--help)
			echo "Usage: $0 [--tidy|--no-tidy]"
			exit 0
			;;
		*) ;;
	esac
done

rm -rf build
mkdir -p build
rm -rf bin
mkdir -p bin

# cmake -S . -B build && cmake --build build
#cp ./build/bin/main ./bin

bazel build //:main 

mkdir -p bin
cp build/bin/main bin/main

# docker build . --target export --output "type=local,dest=."

clang-format-20 -i src/*.*pp

FAILED=0
if (( TIDY )); then
	echo "Running clang-tidy..."
	clang-tidy-20 -p build/compile-commands.json --config-file="./.clang-tidy" src/*/*.*pp \
		|| { FAILED=1; echo 'FAILED: clang-tidy failed'; }
else
	echo "Skipping clang-tidy (--no-tidy provided)"
fi

[[ $FAILED == 0 ]] && echo "Checks PASSED." || echo "Some checks FAILED."


# Compile protobuf
SRC_DIR=src/proto
FRONTEND_DST=src/frontend
BACKEND_DST=src/backend

protoc --version
protoc -I=$SRC_DIR --cpp_out=$FRONTEND_DST $SRC_DIR/addressbook.proto
protoc -I=$SRC_DIR --cpp_out=$BACKEND_DST $SRC_DIR/addressbook.proto

echo "Protobuf compiled to $FRONTEND_DST and $BACKEND_DST"

# exit $FAILED

# Launch app in background so we can position its window
LSAN_OPTIONS=suppressions=lsan.supp ./bin/main &
APP_PID=$!

# Wait for the window to appear (up to ~10s)
WIN_TITLE='My Title'
for i in {1..100}; do
	if wmctrl -l | grep -Fq "$WIN_TITLE"; then
		break
	fi
	sleep 0.1
done

# Move/resize the window; guard errors to avoid set -e exits
if wmctrl -r "$WIN_TITLE" -e 0,1305,0,1255,665; then
	echo "Positioned window."
else
	echo "Warning: wmctrl couldn't find the window to position."
fi

# Wait for app to exit without failing the script if it returns non-zero
# APP_EXIT=0
# wait $APP_PID || APP_EXIT=$?
# [[ $APP_EXIT != 0 ]] && echo "Note: app exited with code $APP_EXIT"

exit $FAILED