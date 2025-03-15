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

@test "Pipes" {
    run ./dsh <<EOF
ls | grep dshlib.c
EOF
    filtered_output=$(echo "$output" | grep -v "^local mode$")
    stripped_output=$(echo "$filtered_output" | tr -d '[:space:]')
    expected_output="dshlib.cdsh4>dsh4>cmdloopreturned0"
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"
    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "pwd prints current directory" {
    current=$(pwd)
    run ./dsh <<EOF
pwd
exit
EOF
    filtered_output=$(echo "$output" | grep -v "^local mode$")
    stripped_output=$(echo "$filtered_output" | tr -d '[:space:]')
    expected_output="~${current}dsh4>dsh4>cmdloopreturned0"
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"
    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}


@test "Blank line gives warning" {
    run ./dsh <<EOF
     
exit
EOF
    filtered_output=$(echo "$output" | grep -v "^local mode$")
    stripped_output=$(echo "$filtered_output" | tr -d '[:space:]')
    expected_output="dsh4>warning:nocommandsprovideddsh4>dsh4>cmdloopreturned0"
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"
    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "External command echo" {
    run ./dsh <<EOF
echo "Hello, World!"
exit
EOF
    filtered_output=$(echo "$output" | grep -v "^local mode$")
    stripped_output=$(echo "$filtered_output" | tr -d '[:space:]')
    expected_output="Hello,World!dsh4>dsh4>cmdloopreturned0"
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"
    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "Immediately exit" {
    run ./dsh <<EOF
exit
EOF
    filtered_output=$(echo "$output" | grep -v "^local mode$")
    stripped_output=$(echo "$filtered_output" | tr -d '[:space:]')
    expected_output="dsh4>cmdloopreturned0"
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"
    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}
