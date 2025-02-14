#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "dshlib.h"

/*
 *  build_cmd_list
 *    cmd_line:     the command line from the user
 *    clist *:      pointer to clist structure to be populated
 *
 *  This function builds the command_list_t structure passed by the caller
 *  It does this by first splitting the cmd_line into commands by spltting
 *  the string based on any pipe characters '|'.  It then traverses each
 *  command.  For each command (a substring of cmd_line), it then parses
 *  that command by taking the first token as the executable name, and
 *  then the remaining tokens as the arguments.
 *
 *  NOTE your implementation should be able to handle properly removing
 *  leading and trailing spaces!
 *
 *  errors returned:
 *
 *    OK:                      No Error
 *    ERR_TOO_MANY_COMMANDS:   There is a limit of CMD_MAX (see dshlib.h)
 *                             commands.
 *    ERR_CMD_OR_ARGS_TOO_BIG: One of the commands provided by the user
 *                             was larger than allowed, either the
 *                             executable name, or the arg string.
 *
 *  Standard Library Functions You Might Want To Consider Using
 *      memset(), strcmp(), strcpy(), strtok(), strlen(), strchr()
 */


int build_cmd_list(char *cmd_line, command_list_t *clist) {
    memset(clist, 0, sizeof(command_list_t));

    if (cmd_line == NULL || cmd_line[0] == '\0') {
        return OK;
    }

    char cmd_copy[SH_CMD_MAX];
    strncpy(cmd_copy, cmd_line, SH_CMD_MAX);
    cmd_copy[SH_CMD_MAX - 1] = '\0';

    const char *c = "\t"; 
    char *saveptr;
    char *token = strtok_r(cmd_copy, PIPE_STRING, &saveptr);
    int count = 0;
    

    while (token != NULL) {
        if (count >= CMD_MAX) {
            return ERR_TOO_MANY_COMMANDS;
        }

        token += strspn(token, c);

        int token_len = strlen(token);
        while (token_len > 0 && strchr(c, token[token_len - 1]) != NULL) {
            token[--token_len] = '\0';
        }

        char *saveptr_in;
        char *cmd_exe = strtok_r(token, " ", &saveptr_in);
        if (cmd_exe == NULL || strlen(cmd_exe) >= EXE_MAX) {
            return ERR_CMD_OR_ARGS_TOO_BIG;
        }
        strcpy(clist->commands[count].exe, cmd_exe);

        char format[ARG_MAX - 2] = "";
        char *arg = strtok_r(NULL, " ", &saveptr_in);
        while (arg != NULL) {
            if (strlen(format) + strlen(arg) >= (ARG_MAX - 2)) {
                return ERR_CMD_OR_ARGS_TOO_BIG;
            }
            strcat(format, arg);
            arg = strtok_r(NULL, " ", &saveptr_in);
        }
        if (format[0] != '\0') {
            snprintf(clist->commands[count].args, ARG_MAX, "[%s]", format);
        } else {
            clist->commands[count].args[0] = '\0';
        }

        count++;
        token = strtok_r(NULL, PIPE_STRING, &saveptr);
    }

    clist->num = count;
    return OK;
}



