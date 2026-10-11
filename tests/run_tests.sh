#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")/.."
task_build_dir=$(mktemp -d "${TMPDIR:-/tmp}/campus-integration-tests.XXXXXX")
trap 'rm -rf "$task_build_dir"' EXIT
task_flags=(-std=c++17 -Wall -Wextra -Werror -pedantic -Iinclude)
for task_test in reservation_count_test reservation_sort_test reservation_load_test reservation_loader_errors_test test_favour_features; do
    task_sources=(src/Reservation.cpp src/ReservationManager.cpp)
    if [[ "$task_test" == test_favour_features ]]; then
        task_sources+=(src/Resource.cpp src/ResourceManager.cpp src/WaitingList.cpp src/ResourceReports.cpp)
    fi
    g++ "${task_flags[@]}" "${task_sources[@]}" "tests/$task_test.cpp" -o "$task_build_dir/$task_test"
    if ! "$task_build_dir/$task_test" 2>"$task_build_dir/$task_test.stderr"; then
        cat "$task_build_dir/$task_test.stderr" >&2
        exit 1
    fi
done
g++ "${task_flags[@]}" src/*.cpp -o "$task_build_dir/campus_reservation"
python3 tests/test_integrated_menu.py "$task_build_dir/campus_reservation"
echo "ALL INTEGRATION CHECKS PASSED"
