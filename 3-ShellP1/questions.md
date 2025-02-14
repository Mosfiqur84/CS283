1. In this assignment I suggested you use `fgets()` to get user input in the main while loop. Why is `fgets()` a good choice for this application?

    > **Answer**:  It is a good choice because it can handle space at the end of a file and read entire lines of input without overflowing the buffer.

2. You needed to use `malloc()` to allocte memory for `cmd_buff` in `dsh_cli.c`. Can you explain why you needed to do that, instead of allocating a fixed-size array?

    > **Answer**:  malloc allows you to select the amount of memory you need at run time. This is important because if you use a fixed-sized array you might be wasting memory if the input is too small. And if your imput is too big it could lead to other problems in your program. 


3. In `dshlib.c`, the function `build_cmd_list(`)` must trim leading and trailing spaces from each command before storing it. Why is this necessary? If we didn't trim spaces, what kind of issues might arise when executing commands in our shell?

    > **Answer**:  This is important because the users might be careless and add extra spaces in the command. If we don't remove the spaces the shell might think that the spaces are part of the program. This could lead to unexpected results or even break the program. 


4. For this question you need to do some research on STDIN, STDOUT, and STDERR in Linux. We've learned this week that shells are "robust brokers of input and output". Google _"linux shell stdin stdout stderr explained"_ to get started.

- One topic you should have found information on is "redirection". Please provide at least 3 redirection examples that we should implement in our custom shell, and explain what challenges we might have implementing them.

    > **Answer**:  1- ls > output.txt: sends standard output to a file. challenge: Have to make sure to correctly replace the stdout file descriptor.
    2- sort < input.txt: this allows for commands to be read from a file and not the keyboard. challenge: Need to find a way to open file and set it to stdout.
    3- grep foo file.txt > error.txt: write the error to a file. Challeneg: you have to handle stderr separately from stdout.

- You should have also learned about "pipes". Redirection and piping both involve controlling input and output in the shell, but they serve different purposes. Explain the key differences between redirection and piping.

    > **Answer**:  1- redirection: this is when you move the input or output from file or device.
    2- piping: this connects the output of one command directly to the input of another. 

- STDERR is often used for error messages, while STDOUT is for regular output. Why is it important to keep these separate in a shell?

    > **Answer**:  It important because it allows for easier debuging by stoping error messages from mixing with regular input. 

- How should our custom shell handle errors from commands that fail? Consider cases where a command outputs both STDOUT and STDERR. Should we provide a way to merge them, and if so, how?

    > **Answer**:  I think our shell should send the error to stderr to make things clear. I think merging should be an option that is given to the user, but by defult we should keep them seperate. 