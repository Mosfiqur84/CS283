#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "dshlib.h"

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
    if (!cmd_buff) return ERR_MEMORY;
    cmd_buff->argc = 0;
    cmd_buff->_cmd_buffer = NULL;
    for (int i = 0; i < CMD_ARGV_MAX; i++) {
        cmd_buff->argv[i] = NULL;
    }
    return OK;
}

int clear_cmd_buff(cmd_buff_t *cmd_buff) {
    if (!cmd_buff) return ERR_MEMORY;
    cmd_buff->argc = 0;
    for (int i = 0; i < CMD_ARGV_MAX; i++)
        cmd_buff->argv[i] = NULL;
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
    if (!cmd_line || !cmd_buff) return ERR_MEMORY;
    if (alloc_cmd_buff(cmd_buff) != OK) return ERR_MEMORY;

    cmd_buff->_cmd_buffer = strdup(cmd_line);
    if (!cmd_buff->_cmd_buffer) return ERR_MEMORY;

    char *s = cmd_buff->_cmd_buffer;
    int token_index = 0;

    while (*s && isspace((unsigned char)*s)) s++;
    if (*s == '\0') {
        cmd_buff->argc = 0;
        return OK;
    }
    while (*s && token_index < CMD_ARGV_MAX - 1) {
        char *token = NULL;
        if (*s == '"') {
            s++; 
            token = s;
            while (*s && *s != '"') s++;
            if (*s == '"') {
                *s = '\0';  
                s++;
            }
        } else {
            token = s;
            while (*s && !isspace((unsigned char)*s)) s++;
            if (*s) {
                *s = '\0';
                s++;
            }
        }
        cmd_buff->argv[token_index++] = token;
        while (*s && isspace((unsigned char)*s)) s++;
    }
    cmd_buff->argv[token_index] = NULL;
    cmd_buff->argc = token_index;
    return OK;
}

Built_In_Cmds match_command(const char *input) {
    if (!input) return BI_NOT_BI;
    if (strcmp(input, EXIT_CMD) == 0) return BI_CMD_EXIT;
    else if (strcmp(input, "cd") == 0)   return BI_CMD_CD;
    else if (strcmp(input, "pwd") == 0)  return (Built_In_Cmds)6; 
    else if (strcmp(input, "rc") == 0)   return BI_RC;
    else return BI_NOT_BI;
}

Built_In_Cmds exec_built_in_cmd(cmd_buff_t *cmd) {
    Built_In_Cmds bic = match_command(cmd->argv[0]);
    if (bic == BI_CMD_EXIT) {
        exit(OK_EXIT);
    }
    else if (bic == BI_CMD_CD) {
        if (cmd->argc == 1) {
        } else if (cmd->argc == 2) {
            if (chdir(cmd->argv[1]) != 0) {
                printf("cd error");
            }
        } else {
            printf("too many arguments");
        }
        return BI_EXECUTED;
    }
    else if (bic == (Built_In_Cmds)6) {
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("~%s", cwd);
        }
        return BI_EXECUTED;
    }
    else if (bic == BI_RC) {
        printf("%d", code);
        return BI_EXECUTED;
    }
    else if (bic == BI_CMD_DRAGON) {
        return BI_EXECUTED;
    }
    return BI_NOT_BI;
}

int exec_cmd(cmd_buff_t *cmd) {
    pid_t pid = fork();
    if (pid < 0) {
        return ERR_EXEC_CMD;
    }
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

int exec_local_cmd_loop() {
    char line[1024];
    bool first_output = true; 
    while (1) {
        if (!first_output) {
            printf("dsh2>");
            fflush(stdout);
        }
        if (fgets(line, sizeof(line), stdin) == NULL) {
            break;
        }
        line[strcspn(line, "\n")] = '\0';

        if (line[0] == '\0') {
            continue;
        }
        if (strcmp(line, EXIT_CMD) == 0) {
            break;
        }

        cmd_buff_t cmd;
        if (build_cmd_buff(line, &cmd) != OK) {
            fprintf(stderr, "Error building command buffer\n");
            continue;
        }
        if (cmd.argc == 0) {
            free_cmd_buff(&cmd);
            continue;
        }

        bool produces_output = false;
        Built_In_Cmds bic = match_command(cmd.argv[0]);
        if (bic != BI_NOT_BI) {
            if (bic != BI_CMD_CD) {
                produces_output = true;
            }
            exec_built_in_cmd(&cmd);
        } else {
            produces_output = true;
            exec_cmd(&cmd);
        }
        free_cmd_buff(&cmd);
        if (produces_output && first_output) {
            first_output = false;
        }
    }

    if (!first_output) {
        printf("dsh2>");
    }

    return 0;
}

