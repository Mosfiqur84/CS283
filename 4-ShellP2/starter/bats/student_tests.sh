#!/usr/bin/env bats

# File: student_tests.sh
# 
# Create your unit tests suit in this file

@test "Example: check ls runs without errors" {
    run ./dsh <<EOF                
ls
EOF

    # Assertions
    [ "$status" -eq 0 ]
}

@test "Change Directory: with argument" {
    run "./dsh" <<EOF
cd /tmp
pwd
EOF

    stripped_output=$(echo "$output" | tr -d '\t\n\r\f\v')
    expected_output="~/tmpdsh2>dsh2>cmd loop returned 0"

    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
}

@test "Change Directory: with no argument (does not change directory)" {
    current=$(pwd)
    run "./dsh" <<EOF
cd
pwd
EOF

    stripped_output=$(echo "$output" | tr -d '\t\n\r\f\v')
    expected_output="~${current}dsh2>dsh2>cmd loop returned 0"

    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
}


@test "One Argument in cd" {
  current=$(pwd)
  run ./dsh <<EOF
cd
pwd
EOF
  stripped_output=$(echo "$output" | tr -d '[:space:]')
  expected=$(echo "$current" | tr -d '[:space:]')
  [[ "$stripped_output" == *"$expected"* ]]
  [ "$status" -eq 0 ]
}

@test "valid directory changes directory" {
  run ./dsh <<EOF
cd /tmp
pwd
EOF
  [[ "$output" =~ "/tmp" ]]
  [ "$status" -eq 0 ]
}

@test "non-existent directory shows error" {
  run ./dsh <<EOF
cd /nonexistent
pwd
EOF
  [[ "$output" =~ "cd error" ]]
  [ "$status" -eq 0 ]
}

@test "'which ls' returns a valid path" {
  which_ls=$(which ls)
  run ./dsh <<EOF
which ls
EOF
  [[ "$output" =~ "$which_ls" ]]
  [ "$status" -eq 0 ]
}

@test "echo saves spaces" {
  run ./dsh <<EOF
echo "   hello   world   "
EOF
  [[ "$output" =~ "   hello   world   " ]]
  [ "$status" -eq 0 ]
}

@test "rc returns the last error code for a failed command" {
  run ./dsh <<EOF
nonexistentcommand
rc
EOF
  [[ "$output" =~ "1" ]]
  [ "$status" -eq 0 ]
}

@test "exit terminates the shell" {
  run ./dsh <<EOF
exit
EOF
  [ "$status" -eq 0 ]
}


