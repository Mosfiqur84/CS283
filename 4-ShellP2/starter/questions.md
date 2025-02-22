1. Can you think of why we use `fork/execvp` instead of just calling `execvp` directly? What value do you think the `fork` provides?

    > **Answer**:  This is doen so that the shell process is not replaced when excuting commands. Fork created a child process,
    which makes sure the shell runs while the child deals with external commands using the execvp.

2. What happens if the fork() system call fails? How does your implementation handle this scenario?

    > **Answer**:  when the fork fails it means that the system can not create a new process. In my code this is handled by retunring -1, and the function printing ERR_EXEC_CMD. This makes sure the shell can not execute commands.

3. How does execvp() find the command to execute? What system environment variable plays a role in this process?

    > **Answer**:  execvp serches for commands in the directories listed in the path enviroment variable. The path stores a list of directories where executable files are stored. execvp checks each one to find and run the specific command. If execvp cant find it then it returns an error.  

4. What is the purpose of calling wait() in the parent process after forking? What would happen if we didn’t call it?

    > **Answer**:  This makes sure that the sheel waits for the child process to finish. If it did not wait, then the child process would become a zombie until an exit status is read. 

5. In the referenced demo code we used WEXITSTATUS(). What information does this provide, and why is it important?

    > **Answer**:  weitstatus gives us exit code of a child process after it terminates, letting us know if the command executed or failed. This is important because it allows the shell to track command outcomes and then make decisions beased on the commands success or failure. 

6. Describe how your implementation of build_cmd_buff() handles quoted arguments. Why is this necessary?

    > **Answer**:  It finds double quotes and treats them like a single argument. This is important because maybe commands needs to be multiple be processed together and not seperate. By making it into a single argument we makes sure that each command is process correctly. 

7. What changes did you make to your parsing logic compared to the previous assignment? Were there any unexpected challenges in refactoring your old code?

    > **Answer**:  I had to change build_cmd_buff to make sure it can handle the double quoted arguments and I made sure that white space handeling skips leading/trailling spaces. I had a hard time figuring out the double quoted argument part and making sure it does not get broken up. 

8. For this quesiton, you need to do some research on Linux signals. You can use [this google search](https://www.google.com/search?q=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&oq=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&gs_lcrp=EgZjaHJvbWUyBggAEEUYOdIBBzc2MGowajeoAgCwAgA&sourceid=chrome&ie=UTF-8) to get started.

- What is the purpose of signals in a Linux system, and how do they differ from other forms of interprocess communication (IPC)?

    > **Answer**:  Singles are used to process,stop,or restart processes. This allows for the system to handle whne a user presses ctrl +c and/or when the program crashes. This is different from other IPC because signals are simple, quick, and does not carray complex data. All you need is a signal number. 

- Find and describe three commonly used signals (e.g., SIGKILL, SIGTERM, SIGINT). What are their typical use cases?

    > **Answer**:  SIGKILL: this forcefully kills a process. This is usually used for when a process is stuck or you need it stop it for whatever reason.
    SIGTERM: This is more like a request to kill a process. This allows for things to be cleaned up before terminating.
    SIGINT: This is when you press, ctrl + c and ask the program to be killed. The program can handle this to exit nicely, but it can also choose to ignore it.


- What happens when a process receives SIGSTOP? Can it be caught or ignored like SIGINT? Why or why not?

    > **Answer**:  SIGSTOP: puts a pause to a process as soon as the command is run. It ca be resumed with the sigcont command. Unlike SIGINT, SIGSTOP can not be ignored, it always stops the process because it is kernel enforced making sure that no process can bypass being stopped.
