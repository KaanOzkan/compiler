#!/bin/bash

# Usage: make test [folder] or  ./test_runner.sh [folder]

set -e

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
NC='\033[0m'

PASSED=0
FAILED=0
SKIPPED=0
TOTAL_ATTEMPTED=0

# Determine test directory based on argument
if [ $# -eq 0 ]; then
    echo "Running all tests..."
    # Find all test directories (root test dir + all subdirectories)
    test_dirs="test"
    for dir in test/*/; do
        if [ -d "$dir" ]; then
            test_dirs="$test_dirs $(echo "$dir" | sed 's:/$::')"
        fi
    done
else
    test_folder="$1"
    echo "Running $test_folder tests..."
    test_dirs="test/$test_folder"
fi

for test_dir in $test_dirs; do
    if [ ! -d "$test_dir" ]; then
        continue
    fi
    
    for test_file in "$test_dir"/*.ko; do
        if [ ! -f "$test_file" ]; then
            continue
        fi
        
        test_name=$(basename "$test_file" .ko)
        expected_file="${test_file%.ko}.exp"
        
        echo -n "Testing $test_dir/$test_name... "
        
        if [ ! -f "$expected_file" ]; then
            echo -e "${YELLOW}SKIP${NC} (no .exp file)"
            SKIPPED=$((SKIPPED + 1))
            continue
        fi
        
        TOTAL_ATTEMPTED=$((TOTAL_ATTEMPTED + 1))
        
        # Run parser and capture output
        if [[ "$test_dir" == *"parser"* ]]; then
            run_cmd="./run.out \"$test_file\" --print=parser --no-assembly"
        else
            run_cmd="./run.out \"$test_file\""
        fi

        if actual_output=$(eval $run_cmd 2>&1); then
            expected_output=$(cat "$expected_file" | tr -d '\n')
            actual_output=$(echo "$actual_output" | tr -d '\n')
            
            if [ "$actual_output" = "$expected_output" ]; then
                echo -e "${GREEN}PASS${NC}"
                PASSED=$((PASSED + 1))
            else
                echo -e "${RED}FAIL${NC}"
                echo "  Expected: '$expected_output'"
                echo "  Actual:   '$actual_output'"
                FAILED=$((FAILED + 1))
            fi
        else
            echo -e "${RED}FAIL${NC} (crashed)"
            FAILED=$((FAILED + 1))
        fi
    done
done

echo ""

if [ $TOTAL_ATTEMPTED -eq 0 ]; then
    echo -e "${RED}Error: No tests found to run!${NC}"
    exit 1
fi

echo "Results: $PASSED passed, $FAILED failed, $SKIPPED skipped"

if [ $FAILED -eq 0 ]; then
    echo -e "${GREEN}All tests passed!${NC}"
    exit 0
else
    echo -e "${RED}Some tests failed.${NC}"
    exit 1
fi 