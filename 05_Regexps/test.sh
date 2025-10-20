#!/usr/bin/env bash
set -euo pipefail

GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m'

test_count=0
passed_count=0

run_test() {
    local pattern="$1"
    local replacement="$2"
    local input="$3"
    local description="$4"

    test_count=$((test_count + 1))

    esub_output=$(./esub "$pattern" "$replacement" "$input" 2>esub_err.txt) || esub_status=$? || true
    esub_status=${esub_status:-0}
    esub_err=$(cat esub_err.txt)

    sed_output=$(echo "$input" | sed -E "s/$pattern/$replacement/" 2>sed_err.txt) || sed_status=$? || true
    sed_status=${sed_status:-0}
    sed_err=$(cat sed_err.txt)

    if [[ "$esub_output" == "$sed_output" ]] && [[ "$esub_status" -eq "${sed_status:-0}" ]]; then
        echo -e "${GREEN}✓${NC} Test $test_count passed: $description"
        passed_count=$((passed_count + 1))
    else
        echo -e "${RED}✗${NC} Test $test_count failed: $description"
        echo "  Input: '$input'"
        echo "  Pattern: '$pattern'"
        echo "  Replacement: '$replacement'"
        echo "  Expected (sed, exit $sed_status):"
        echo "-----"
        echo "$sed_output"
        echo "-----"
        echo "  Got (esub, exit $esub_status):"
        echo "-----"
        echo "$esub_output"
        echo "-----"
        if [[ -n "$esub_err" ]]; then
            echo "  esub stderr: $esub_err"
        fi
        if [[ -n "$sed_err" ]]; then
            echo "  sed stderr: $sed_err"
        fi
    fi

    rm -f esub_err.txt sed_err.txt
}

run_test "world" "universe" "hello world" "Simple replacement"
run_test "[0-9]+" "X" "123-456-789" "Replace first number"
run_test "([a-z]+)([0-9]+)" "\\2-\\1" "abc123def456" "Backreferences"
run_test "(.*)@(.*)" "\\1 at \\2" "test@example.com" "Email parsing"
run_test "\\b\\w{3}\\b" "XXX" "foo bar baz" "Word boundary"
run_test "(.)\\1+" "\\1" "aaabbbccc" "Repeated characters"
run_test "^\\s+" "" "  spaces  " "Leading spaces"
run_test "\\s+$" "" "  spaces  " "Trailing spaces"
run_test "([A-Z])" "_\\1" "CamelCase" "CamelCase - prepend underscore to capitals"
run_test "([a-z])([0-9])" "\\2\\1" "a1b2c3" "Swap letter and digit"
run_test "test" "OK" "test test test" "Multiple occurrences - replace first"
run_test "xyz" "replaced" "no match here" "No match"
run_test ".*" "empty" "" "Empty string"
run_test "(.*)" "[\\1]" "abc" "Whole string capture"

echo ""
echo "================================"
echo "Tests passed: $passed_count/$test_count"
if [ "$passed_count" -eq "$test_count" ]; then
    echo -e "${GREEN}All tests passed!${NC}"
    exit 0
else
    echo -e "${RED}Some tests failed!${NC}"
    exit 1
fi

