1. Your shell forks multiple child processes when executing piped commands. How does your implementation ensure that all child processes complete before the shell continues accepting user input? What would happen if you forgot to call waitpid() on all child processes?

I ensure that all child processes is complete by calling waitpid() on each child process after I fork then form a pipe command. This makes sure that the parents shell process wait for al the child processes to finsh execution before it proceeds. If I forgot to call waitpid() on my child processes acting like a zombie processes b/c their exit status would not be collected. The shell could also print new promts before the command fnish, which could lead to commands output appearing after the next prompt. 

2. The dup2() function is used to redirect input and output file descriptors. Explain why it is necessary to close unused pipe ends after calling dup2(). What could go wrong if you leave pipes open?

We have to close the unused pipe ends because if we left them open it could cause a resource leaks which could lead to unexpected behavior. If we leave the pipes open and we try to process an EOF condition, b/c the another process still has a write end open, the EOF might never be detected. This could lead to infinte waits. Thus, closing the pipes ends is very important, it allows for data to flow correctly b/w processes. 

3. Your shell recognizes built-in commands (cd, exit, dragon). Unlike external commands, built-in commands do not require execvp(). Why is cd implemented as a built-in rather than an external command? What challenges would arise if cd were implemented as an external process?

cd is a built-in command b/c cd allows you to change directories, and changing directories effects the parent shell process. External commands can not change parent's enviroment. If we made cd into an external command, the newly spawned child process would be able to change its own directory, but once it exits the parent would remain in the same directory, which would make the command ineffective. 


4. Currently, your shell supports a fixed number of piped commands (CMD_MAX). How would you modify your implementation to allow an arbitrary number of piped commands while still handling memory allocation efficiently? What trade-offs would you need to consider?

I would replace the fixed-size command_list_t array with a dynamically allocated list (linked list). And instead of statically defining CMD_MAX, the shell would allocate memory for each command dynamically. Some trade off to consider, the shell must be able to assign memory and free up memory to stop a memory leak. Another is if memory allocation fails, the shell must be able to figure out the error to not crash. 
