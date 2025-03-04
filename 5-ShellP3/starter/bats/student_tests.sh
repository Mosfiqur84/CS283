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
    run "./dsh" <<EOF                
ls | grep dshlib.c
EOF
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="dshlib.cdsh3>dsh3>cmdloopreturned0"

    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"
    
    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}


@test "Change Directory: cd /tmp and pwd" {
    run "./dsh" <<EOF                
cd /tmp
pwd
exit
EOF

    expected_output="~/tmpdsh3>dsh3>cmdloopreturned0"

    stripped_output=$(echo "$output" | tr -d '[:space:]')
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"
    
    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "Empty Input: Blank line yields warning" {
    run "./dsh" <<EOF                
     
exit
EOF

    expected_output="dsh3>warning:nocommandsprovideddsh3>dsh3>cmdloopreturned0"

    stripped_output=$(echo "$output" | tr -d '[:space:]')
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"
    
    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "Echo: External command echo" {
    run "./dsh" <<EOF                
echo "Hello, World!"
exit
EOF

    expected_output="Hello,World!dsh3>dsh3>cmdloopreturned0"

    stripped_output=$(echo "$output" | tr -d '[:space:]')
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"
    
    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

# Test 5: Immediate Exit
@test "Exit: Immediately exit" {
    run "./dsh" <<EOF                
exit
EOF

    expected_output="dsh3>cmdloopreturned0"

    stripped_output=$(echo "$output" | tr -d '[:space:]')
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"
    
    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "Builtin pwd command" {
    run ./dsh <<EOF
pwd
exit
EOF

    current_dir=$(pwd)
    expected_output="~${current_dir}dsh3>dsh3>cmdloopreturned0"
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    
    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"
    
    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}



@test "Builtin cd with no argument does not change directory" {
    current=$(pwd)
    run ./dsh <<EOF
cd
pwd
exit
EOF

    expected_output="~${current}dsh3>dsh3>cmdloopreturned0"
    expected_output=$(echo "$expected_output" | tr -d '[:space:]')
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"
    
    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}
