# Workspace Instructions

## No changes without explicit permission

- Do not create, edit, rename, move, delete, format, or generate any workspace file unless the user explicitly asks for that change.
- A request to inspect, explain, review, debug, diagnose, or identify a problem is read-only. Report the findings and propose a fix, but do not apply it.
- Do not interpret phrases such as "not working," compiler errors, or runtime failures as permission to modify code.
- Before making a change, state the exact files and intended edits and wait for the user's explicit approval, unless the user's current message already clearly requests implementation.
- Permission applies only to the specific change requested. Do not make adjacent cleanups, refactors, formatting changes, or unrelated fixes without separate approval.
- Read-only commands and builds are allowed when useful for diagnosis, provided they do not alter source files. If a tool may generate or modify workspace artifacts, ask first.

## Matrix convention

- The rendering code uses column vectors. Preserve matrix-times-vector multiplication (`matrix * vector`) when reviewing, diagnosing, or changing transformation code.
