#!/bin/bash
#
# Builds every target and reports which succeeded.
# Exits non-zero if any required target fails. The WebAssembly build is
# optional: it needs a toolchain that may not be installed, so it is reported
# but not fatal.

set -u
cd "$(dirname "$0")/.." || exit 1

GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m'

required_failed=0

# run_build <label> <make target> <optional: reason it may be skipped>
run_build() {
    local label="$1" target="$2" optional="${3:-}"

    printf '%-40s' "$label"

    if make "$target" >/dev/null 2>&1; then
        printf "${GREEN}PASS${NC}\n"
        return 0
    fi

    if [ -n "$optional" ]; then
        printf "${YELLOW}SKIP${NC}  (%s)\n" "$optional"
        return 0
    fi

    printf "${RED}FAIL${NC}\n"
    required_failed=$((required_failed + 1))
}

echo "Word Hunt Solver - build verification"
echo "====================================="

make clean >/dev/null 2>&1

run_build "terminal solver (Trie)"     bin/wordhunt
run_build "terminal solver (std::map)" bin/wordhunt_map
run_build "browser build"              web "emscripten not installed"
run_build "test suite (std::map)"      bin/test_map
run_build "test suite (Trie)"          bin/test_trie

echo "====================================="

if [ "$required_failed" -eq 0 ]; then
    printf "${GREEN}All required builds succeeded.${NC}\n"
    exit 0
fi

printf "${RED}%d required build(s) failed.${NC}\n" "$required_failed"
exit 1
