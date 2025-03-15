#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "dshlib.h"


/**** 
 **** FOR REMOTE SHELL USE YOUR SOLUTION FROM SHELL PART 3 HERE
 **** THE MAIN FUNCTION CALLS THIS ONE AS ITS ENTRY POINT TO
 **** EXECUTE THE SHELL LOCALLY
 ****
 */

/*
 * Implement your exec_local_cmd_loop function by building a loop that prompts the 
 * user for input.  Use the SH_PROMPT constant from dshlib.h and then
 * use fgets to accept user input.
 * 
 *      while(1){
 *        printf("%s", SH_PROMPT);
 *        if (fgets(cmd_buff, ARG_MAX, stdin) == NULL){
 *           printf("\n");
 *           break;
 *        }
 *        //remove the trailing \n from cmd_buff
 *        cmd_buff[strcspn(cmd_buff,"\n")] = '\0';
 * 
 *        //IMPLEMENT THE REST OF THE REQUIREMENTS
 *      }
 * 
 *   Also, use the constants in the dshlib.h in this code.  
 *      SH_CMD_MAX              maximum buffer size for user input
 *      EXIT_CMD                constant that terminates the dsh program
 *      SH_PROMPT               the shell prompt
 *      OK                      the command was parsed properly
 *      WARN_NO_CMDS            the user command was empty
 *      ERR_TOO_MANY_COMMANDS   too many pipes used
 *      ERR_MEMORY              dynamic memory management failure
 * 
 *   errors returned
 *      OK                     No error
 *      ERR_MEMORY             Dynamic memory management failure
 *      WARN_NO_CMDS           No commands parsed
 *      ERR_TOO_MANY_COMMANDS  too many pipes used
 *   
 *   console messages
 *      CMD_WARN_NO_CMD        print on WARN_NO_CMDS
 *      CMD_ERR_PIPE_LIMIT     print on ERR_TOO_MANY_COMMANDS
 *      CMD_ERR_EXECUTE        print on execution failure of external command
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 1+)
 *      malloc(), free(), strlen(), fgets(), strcspn(), printf()
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 2+)
 *      fork(), execvp(), exit(), chdir()
 */

 int code = 0;
 
 int alloc_cmd_buff(cmd_buff_t *cmd_buff) {
     if (!cmd_buff)
         return ERR_MEMORY;
     cmd_buff->argc = 0;
     cmd_buff->_cmd_buffer = NULL;
     for (int i = 0; i < CMD_ARGV_MAX; i++) {
         cmd_buff->argv[i] = NULL;
     }
     return OK;
 }
 
 int clear_cmd_buff(cmd_buff_t *cmd_buff) {
     if (!cmd_buff)
         return ERR_MEMORY;
     cmd_buff->argc = 0;
     for (int i = 0; i < CMD_ARGV_MAX; i++) {
         cmd_buff->argv[i] = NULL;
     }
     if (cmd_buff->_cmd_buffer) {
         free(cmd_buff->_cmd_buffer);
         cmd_buff->_cmd_buffer = NULL;
     }
     return OK;
 }
 
 int free_cmd_buff(cmd_buff_t *cmd_buff) {
     return clear_cmd_buff(cmd_buff);
 }
  
 int build_cmd_buff(char *cmd_line, cmd_buff_t *cmd_buff) {
     if (!cmd_line || !cmd_buff)
         return ERR_MEMORY;
     if (alloc_cmd_buff(cmd_buff) != OK)
         return ERR_MEMORY;
     cmd_buff->_cmd_buffer = strdup(cmd_line);
     if (!cmd_buff->_cmd_buffer)
         return ERR_MEMORY;
     char *s = cmd_buff->_cmd_buffer;
     int token_index = 0;
     while (*s && isspace((unsigned char)*s))
         s++;
     if (*s == '\0') {
         cmd_buff->argc = 0;
         return OK;
     }
     while (*s && token_index < CMD_ARGV_MAX - 1) {
         char *token = NULL;
         if (*s == '"') {
             s++;
             token = s;
             while (*s && *s != '"')
                 s++;
             if (*s == '"') {
                 *s = '\0';
                 s++;
             }
         } else {
             token = s;
             while (*s && !isspace((unsigned char)*s))
                 s++;
             if (*s) {
                 *s = '\0';
                 s++;
             }
         }
         cmd_buff->argv[token_index++] = token;
         while (*s && isspace((unsigned char)*s))
             s++;
     }
     cmd_buff->argv[token_index] = NULL;
     cmd_buff->argc = token_index;
     return OK;
 }
  
 int build_cmd_list(char *cmd_line, command_list_t *clist) {
     memset(clist, 0, sizeof(command_list_t));
     if (cmd_line == NULL || cmd_line[0] == '\0')
         return OK;
     char cmd_copy[SH_CMD_MAX];
     strncpy(cmd_copy, cmd_line, SH_CMD_MAX);
     cmd_copy[SH_CMD_MAX - 1] = '\0';
     char *saveptr;
     char *token = strtok_r(cmd_copy, PIPE_STRING, &saveptr);
     int count = 0;
     while (token != NULL) {
         while (*token && isspace((unsigned char)*token))
             token++;
         if (*token == '\0') {
             token = strtok_r(NULL, PIPE_STRING, &saveptr);
             continue;
         }
         if (count >= CMD_MAX) {
             return ERR_TOO_MANY_COMMANDS;
         }
         int ret = build_cmd_buff(token, &clist->commands[count]);
         if (ret != OK) {
             return ret;
         }
         count++;
         token = strtok_r(NULL, PIPE_STRING, &saveptr);
     }
     clist->num = count;
     return OK;
 }
 
 int free_cmd_list(command_list_t *cmd_lst) {
     for (int i = 0; i < cmd_lst->num; i++) {
         free_cmd_buff(&cmd_lst->commands[i]);
     }
     return OK;
 }
 
 Built_In_Cmds match_command(const char *input) {
     if (!input)
         return BI_NOT_BI;
     if (strcmp(input, EXIT_CMD) == 0)
         return BI_CMD_EXIT;
     else if (strcmp(input, "cd") == 0)
         return BI_CMD_CD;
     else if (strcmp(input, "pwd") == 0)
         return (Built_In_Cmds)6;
     return BI_NOT_BI;
 }
 
 Built_In_Cmds exec_built_in_cmd(cmd_buff_t *cmd) {
     Built_In_Cmds bic = match_command(cmd->argv[0]);
     if (bic == BI_CMD_EXIT) {
         exit(OK_EXIT);
     } else if (bic == BI_CMD_CD) {
         if (cmd->argc == 1) {
         } else if (cmd->argc == 2) {
             if (chdir(cmd->argv[1]) != 0) {
                 printf("cd error");
             }
         } else {
             printf("too many arg");
         }
         return BI_EXECUTED;
     } else if (bic == (Built_In_Cmds)6) {
         char cwd[1024];
         if (getcwd(cwd, sizeof(cwd)) != NULL) {
             printf("~%s", cwd);
         }
         return BI_EXECUTED;
     }
     return BI_NOT_BI;
 }
 
 int exec_cmd(cmd_buff_t *cmd) {
     pid_t pid = fork();
     if (pid < 0)
         return ERR_EXEC_CMD;
     if (pid == 0) {
         execvp(cmd->argv[0], cmd->argv);
         printf(CMD_WARN_NO_CMD);
         exit(1);
     } else {
         int status;
         waitpid(pid, &status, 0);
         if (WIFEXITED(status)) {
             code = WEXITSTATUS(status);
             return code;
         } else {
             return ERR_EXEC_CMD;
         }
     }
 }
 
 int execute_pipeline(command_list_t *clist) {
     int num = clist->num;
     if (num == 0)
         return WARN_NO_CMDS;
     if (num > CMD_MAX) {
         printf(CMD_ERR_PIPE_LIMIT, CMD_MAX);
         return ERR_TOO_MANY_COMMANDS;
     }
     if (num == 1)
         return exec_cmd(&clist->commands[0]);
     int i;
     int prev_fd = -1;
     int pipefd[2];
     pid_t pids[CMD_MAX];
     for (i = 0; i < num; i++) {
         if (i < num - 1) {
             if (pipe(pipefd) < 0) {
                 return ERR_EXEC_CMD;
             }
         }
         pid_t pid = fork();
         if (pid < 0) {
             return ERR_EXEC_CMD;
         }
         if (pid == 0) {
             if (i > 0) {
                 dup2(prev_fd, STDIN_FILENO);
                 close(prev_fd);
             }
             if (i < num - 1) {
                 dup2(pipefd[1], STDOUT_FILENO);
                 close(pipefd[0]);
                 close(pipefd[1]);
             }
             execvp(clist->commands[i].argv[0], clist->commands[i].argv);
             exit(1);
         } else {
             pids[i] = pid;
             if (i > 0)
                 close(prev_fd);
             if (i < num - 1) {
                 prev_fd = pipefd[0];
                 close(pipefd[1]);
             }
         }
     }
     int status;
     for (i = 0; i < num; i++) {
         waitpid(pids[i], &status, 0);
     }
     return OK;
 }

int exec_local_cmd_loop() {
    char line[SH_CMD_MAX];
    bool first = false;
    bool exitFlag = false;
    
    while (1) {
        if (first) {
            printf("%s", SH_PROMPT);
            fflush(stdout);
        }
        
        if (fgets(line, sizeof(line), stdin) == NULL) {
            printf("\n");
            break;
        }
        
        line[strcspn(line, "\n")] = '\0';
        
        char *p = line;
        while (*p && isspace((unsigned char)*p))
            p++;
        
        if (*p == '\0') {
            if (!first) {
                printf("%s", SH_PROMPT);
                fflush(stdout);
            }
            printf(CMD_WARN_NO_CMD);
            first = true;
            continue;
        }
        
        if (strcmp(p, EXIT_CMD) == 0) {
            if (!first) {
                printf("%s", SH_PROMPT);
                fflush(stdout);
                exitFlag = true;
            }
            break;
        }
        
        if (strchr(p, PIPE_CHAR) != NULL) {
            command_list_t clist;
            int ret = build_cmd_list(p, &clist);
            if (ret != OK) {
                if (ret == ERR_TOO_MANY_COMMANDS)
                    printf(CMD_ERR_PIPE_LIMIT, CMD_MAX);
                first = true;
                continue;
            }
            if (clist.num == 0) {
                printf(CMD_WARN_NO_CMD);
                first = true;
                continue;
            }
            execute_pipeline(&clist);
            free_cmd_list(&clist);
            first = true;
        } else {
            cmd_buff_t cmd;
            int ret = build_cmd_buff(p, &cmd);
            if (ret != OK) {
                first = true;
                continue;
            }
            if (cmd.argc == 0) {
                printf(CMD_WARN_NO_CMD);
                free_cmd_buff(&cmd);
                first = true;
                continue;
            }
            Built_In_Cmds bic = match_command(cmd.argv[0]);
            if (bic != BI_NOT_BI) {
                if (bic == BI_CMD_CD) {
                    exec_built_in_cmd(&cmd);
                } else {
                    exec_built_in_cmd(&cmd);
                    first = true;
                }
            } else {
                exec_cmd(&cmd);
                first = true;
            }
            free_cmd_buff(&cmd);
        }
    }
    
    if (!exitFlag) {
        printf("%s", SH_PROMPT);
    }
    return 0;
}


 
 
