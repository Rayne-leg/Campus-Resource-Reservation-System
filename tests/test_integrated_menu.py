"""Check the actual menu with the combined team menu, production counts, and cancellation behavior."""
from pathlib import Path
import re
import shutil
import subprocess
import sys
import tempfile

root = Path(__file__).resolve().parents[1]
executable = Path(sys.argv[1]).resolve()


def run_menu(actions, empty_reservations=False, expected_errors=False):
    with tempfile.TemporaryDirectory() as directory:
        work = Path(directory)
        shutil.copytree(root / "data", work / "data")
        if empty_reservations:
            (work / "data/reservations.txt").write_text("")
        inputs = ("\n".join(line for action in actions for line in action) + "\n") if actions else ""
        result = subprocess.run([str(executable)], input=inputs, cwd=work,
                                text=True, capture_output=True, timeout=15, check=True)
    if expected_errors:
        assert result.stderr, "Expected loader rejection diagnostics"
    else:
        assert not result.stderr, result.stderr
    segments = result.stdout.split("Choose an option: ")[1:]
    assert len(segments) == (len(actions) if actions else 1), (len(segments), len(actions))
    return segments, result.stdout


def row(segment, resource):
    return next((line for line in segment.splitlines()
                 if line.startswith(resource + " ")), "")


actions = [
    ["10"], ["11"], ["12"], ["15", "301"], ["15", "999"], ["13"],
    ["1", "R101", "S1", "Favour", "2026-10-11"],
    ["1", "R101", "S2", "Student Two", "10/12/2026"],
    ["1", "R101", "S3", "Student Three", "2026-10-12"],
    ["1", "R101", "S4", "Student Four", "2026-10-11"],
    ["1", "R101", "S4", "Student Four", "10/11/2026"],
    ["12"], ["4", "RES1"], ["8"], ["12"], ["5"], ["9"],
    ["4", "RES3"], ["8"], ["5"], ["4", "RES2"], ["8"], ["12"],
    ["10"], ["11"], ["7", "10/13/2026"], ["7", "2026-02-30", ""],
    ["abc"], ["99"], ["1", "UNKNOWN"], ["1", "R101", "S5", ""], ["0"],
]
s, transcript = run_menu(actions)
assert "Total active reservations: 20" in s[0]
assert row(s[0], "R101").split()[-1] == "2"
assert row(s[0], "R120").split()[-1] == "0"
assert len([line for line in s[0].splitlines() if re.match(r"^R\d{3}\s", line)]) == 20
assert row(s[1], "R101").split()[-1] == "2"
assert not row(s[1], "R120")
assert row(s[2], "R101").split()[-2:] == ["0", "0"]
assert "Reservation 301 is an active reservation." in s[3]
assert "Reservation ID not found." in s[4]
dates = re.findall(r"Reservation Date: (\d{4}-\d{2}-\d{2})", s[5])
assert len(dates) == 20 and dates == sorted(dates)
assert "Reservation RES1 created for R101 on 2026-10-11." in s[6]
assert "Reservation RES2 created for R101 on 2026-10-12." in s[7]
assert ". Adding you to the waiting list." in s[8] and ". Adding you to the waiting list." in s[9]
assert "already waiting" in s[10]
assert row(s[11], "R101").split()[-2:] == ["2", "2"]
assert "Promoted Student Four from the waiting list to reservation RES3." in s[12]
assert "Student ID: S3" in s[13] and "Student ID: S4" not in s[13]
assert row(s[14], "R101").split()[-2:] == ["1", "1"]
assert "The history record was kept." in s[15]
assert "RES1" in s[16]
assert "Promoted" not in s[17]
assert "Student ID: S3" in s[18]
assert "Restored reservation RES3 for R101 on 2026-10-11." in s[19]
assert "Promoted Student Three from the waiting list to reservation RES4." in s[20]
assert "Waiting list is empty." in s[21]
assert row(s[22], "R101").split()[-2:] == ["0", "0"]
assert "Total active reservations: 22" in s[23]
assert row(s[23], "R101").split()[-1] == "4"
assert row(s[24], "R101").split()[-1] == "4" and not row(s[24], "R103")
assert "RESOURCE AVAILABILITY FOR 2026-10-13" in s[25]
assert row(s[25], "R101").split()[-1] == "Available"
assert "Invalid date. Use YYYY-MM-DD" in s[26]
assert "Please enter a number." in s[27]
assert "Invalid option." in s[28]
assert "That resource does not exist." in s[29]
assert "Student name is required." in s[30]
assert "Goodbye." in s[31]

fifo = [
    ["1", "R101", "A", "Active", "2026-10-11"],
    ["1", "R101", "B", "First Waiter", "2026-10-11"],
    ["1", "R101", "C", "Second Waiter", "2026-10-11"],
    ["4", "RES1"], ["8"], ["4", "RES2"], ["8"], ["0"],
]
f, fifo_transcript = run_menu(fifo, empty_reservations=True)
assert "Promoted First Waiter from the waiting list to reservation RES2." in f[3]
assert "Student ID: C" in f[4] and "Student ID: B" not in f[4]
assert "Promoted Second Waiter from the waiting list to reservation RES3." in f[5]
assert "Waiting list is empty." in f[6]

# Exercise Ademola's newest-first restore, trimmed input, retry, and EOF paths.
undo_actions = [
    ["1", " R101 ", " A ", " First Student ", "bad date", "2026-10-11"],
    ["1", "R101", "B", "Second Student", "2026-10-12"],
    ["3", "RES1", "2026-10-12"], ["2"],
    ["4", "RES1"], ["4", "RES2"], ["9"], ["5"], ["9"], ["5"],
    ["9"], ["10"], ["3", "MISSING"], ["4", "MISSING"],
    ["1", "R101", "   "], ["1", "R101", "C", "Third Student", ""],
    ["1xyz"], ["0"],
]
u, undo_transcript = run_menu(undo_actions, empty_reservations=True)
assert "Invalid date." in u[0] and "Reservation RES1 created" in u[0]
assert "Could not update" in u[2]
assert "2026-10-11" in u[3] and "2026-10-12" in u[3]
assert u[6].index("RES2") < u[6].index("RES1")
assert "Restored reservation RES2" in u[7]
assert "RES1" in u[8] and "RES2" not in u[8]
assert "Restored reservation RES1" in u[9]
assert "No cancellations have been made." in u[10]
assert "Total active reservations: 2" in u[11]
assert "Reservation ID not found." in u[12] and "Reservation ID not found." in u[13]
assert "Student ID is required." in u[14]
assert "operation cancelled" in u[15]
assert "Please enter a number." in u[16]
_, eof_transcript = run_menu([])
assert "Input ended. Goodbye." in eof_transcript
# Reloading preserves active IDs and counts, and an ID collision preserves undo history.
l, load_transcript = run_menu([["4", "301"], ["14", ""], ["5"], ["9"], ["10"], ["0"]], expected_errors=True)
assert "that reservation ID is already in use" in l[2]
assert "The history record was kept." in l[2] and "301" in l[3]
assert "Total active reservations: 20" in l[4]

(root / "tests/integration_menu_results.txt").write_text(
    transcript + "\n===== EMPTY-DATA FIFO SCENARIO =====\n" + fifo_transcript + "\n===== UNDO AND INPUT SCENARIO =====\n" + undo_transcript
    + "\n===== RELOAD AND ID COLLISION SCENARIO =====\n" + load_transcript
    + "\n===== EOF SCENARIO =====\n" + eof_transcript)
print("PASS: production report counts, search, sorted loaded dates, date matching, "
      "FIFO promotions, duplicate prevention, undo history, live statistics, LIFO restore, reload collisions, date retry, EOF, and input handling.")
