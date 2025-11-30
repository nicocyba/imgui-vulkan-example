#!/bin/bash
set -euo pipefail

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

# rm -rf build
rm -rf bin
mkdir -p bin

cmake -S . -B build && cmake --build build

cp ./build/bin/main ./bin

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
# rm -rf build

# ls -LR
./bin/main
exit $FAILED