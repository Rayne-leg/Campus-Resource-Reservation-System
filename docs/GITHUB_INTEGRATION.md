# GitHub integration

This archive is a runnable source copy. It is not a Git clone. The changes_from_reservations.patch file contains the reviewed differences from the supplied reservations branch snapshot, including the integration documentation and tests.

Use a clean clone of the shared Campus-Resource-Reservation-System repository for real commits. First inspect git status and git remote -v. The remote must refer to the team's repository and existing local edits must be preserved.

After confirming a clean team checkout, run:

```bash
git fetch origin
git checkout -b favour-final-reports origin/reservations
git apply /PATH/TO/Ken_Favour_Integration/integration/changes_from_reservations.patch
git diff --check
git status --short
git diff --stat
```

Replace /PATH/TO with the actual extracted package location. If the branch name already exists, inspect that existing branch rather than resetting or overwriting it. If applying the patch fails, the current reservations branch differs from the supplied snapshot: compare and merge the new changes before proceeding.

Run the tests on the resulting source version. After reviewing the diff and recording the actual CELL results, stage the intended changed files, make a real commit, and push the separate branch. Open a pull request for team review before the final main merge. Base main includes Ken's reservations-branch commits plus the new integration commit; the team can choose reservations as the pull-request base if it wants to review only Favour's changes first.

Do not upload the ZIP itself as a substitute for the source changes. This package does not overwrite Ken's ReservationManager files or claim any new GitHub commits. Ademola's latest changes and review remain pending.
