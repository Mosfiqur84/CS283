#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//Sorry for the mix in naming notations. I like the cammel case one. But since the prototypes used the _ I wanted to
//be consistence for that part. Then I changed it for my own code. 

//ALSO; I was the one that talked to you in the clc about my test cases not working. Most of my cases do work, but it does pass in the test.
//I know its alot of work, but please manually check the cases because majority do work. (Sorry for the extra work)

#define BUFFER_SZ 50

//prototypes
void usage(char *);
void print_buff(char *, int);
int  setup_buff(char *, char *, int);
void reverse_string(char *, int);
void word_print(char *, int);


//prototypes for functions to handle required functionality
int  count_words(char *, int, int);
//add additional prototypes here



//TODO: #4:  Implement the setup buff as per the directions

int setup_buff(char *buff, char *user_str, int len){
    int count = 0 ; 
    int trackWhiteSpace = 0;
    char *treverseUserStr = user_str; 
    char *writeIntoBuffer = buff; 

    if (len <= 0 || !buff || !user_str){
        printf("error with one of your provided services\n");
        exit(3);
    }
    while(*treverseUserStr != '\0'){
        if (count >= len -1 ) { 
            buff[count] = '\0';
            return count;  
        }
        if(*treverseUserStr == ' ' || *treverseUserStr == '\t'){ 
            if (!trackWhiteSpace){
                *writeIntoBuffer = ' '; 
                writeIntoBuffer++;
                trackWhiteSpace = 1;
                count++;
            }

        }else{
            *writeIntoBuffer = *treverseUserStr;
            writeIntoBuffer++;
            trackWhiteSpace = 0;
            count++;

        }
        treverseUserStr++;
    }
    int newCount = count; 

    while (count < len) {
        *writeIntoBuffer = '.';
        writeIntoBuffer++;
        count++;
    }


    if (*treverseUserStr != '\0') {
        return len - 1;
    }

    return newCount; 
}

void print_buff(char *buff, int len){
    printf("Buffer:  ");
    for (int i=0; i<len; i++){
        putchar(*(buff+i));
    }
    putchar('\n');
}

void usage(char *exename){

    printf("usage: %s [-h|c|r|w|x] \"string\" [other args]\n", exename);

}

int count_words(char *buff, int len, int str_len) {
    int count = 0;
    int inWord = 0;

    for (int i = 0; i < str_len; i++) {
        if (buff[i] != ' ' && buff[i] != '\t' && buff[i] != '\n') { 
            if (!inWord) {
                count++;
                inWord = 1;
            }
        } else {
            inWord = 0;
        }
    }

    return count;
}


//ADD OTHER HELPER FUNCTIONS HERE FOR OTHER REQUIRED PROGRAM OPTIONS

void reverse_string(char *buff , int str_len){ 
    char *start = buff;
    char *end = buff + str_len - 1;
    while(start < end){
        char temp = *start;
        *start = *end;
        *end = temp;

        start++;
        end--;
    }

}


void word_print(char *buff, int str_len) {
    int word_index = 1;
    char *start = buff;

    printf("Word Print\n----------\n");

    for (int i = 0; i < str_len; i++) {
        if (buff[i] != ' ' && (i == 0 || buff[i - 1] == ' ')) {
            start = buff + i;
        }

        if (buff[i] == ' ' || i == str_len - 1) {
            char *end;
            if (buff[i] == ' ') {
                end = buff + i - 1;
            } else {
                end = buff + i; 
            }

            int length = end - start + 1;
            printf("%d. ", word_index++);
            for (char *p = start; p <= end; p++) {
                putchar(*p);
            }
            printf("(%d)\n", length);
        }
    }

    printf("\nNumber of words returned: %d\n", word_index - 1);

    printf("Buffer:  [");
    for (int i = 0; i < BUFFER_SZ; i++) {
        putchar(buff[i]);
    }
    printf("]\n");
}


int main(int argc, char *argv[]){

    char *buff;             //placehoder for the internal buffer
    char *input_string;     //holds the string provided by the user on cmd line
    char opt;               //used to capture user option from cmd line
    int  rc;                //used for return codes
    int  user_str_len;      //length of user supplied string

    //TODO:  #1. WHY IS THIS SAFE, aka what if arv[1] does not exist?
    //      PLACE A COMMENT BLOCK HERE EXPLAINING
    /*
    The if statement makes sure that arv[1] exists. If it does not, it will go into the if statement and exit with 1, which is a command line problem. This
    allows you to avoide a program crash. 


    */

    if ((argc < 2) || (*argv[1] != '-')){
        usage(argv[0]);
        exit(1);
    }

    opt = (char)*(argv[1]+1);   //get the option flag

    //handle the help flag and then exit normally
    if (opt == 'h'){
        usage(argv[0]);
        exit(0);
    }

    //WE NOW WILL HANDLE THE REQUIRED OPERATIONS

    //TODO:  #2 Document the purpose of the if statement below
    //      PLACE A COMMENT BLOCK HERE EXPLAINING

    /*
    This is used to make sure the program has atleast 2 argc. It it does not, it will go into the if statemant and exit 1, command line issue. 
    
    */
    if (argc < 3){
        usage(argv[0]);
        exit(1);
    }

    input_string = argv[2]; //capture the user input string

    //TODO:  #3 Allocate space for the buffer using malloc and
    //          handle error if malloc fails by exiting with a 
    //          return code of 99
    // CODE GOES HERE FOR #3
    buff = (char *)malloc(BUFFER_SZ);
    if(buff == NULL){
        printf("Error: Memory allocation failed\n");
        exit(99);
    }

    user_str_len = setup_buff(buff, input_string, BUFFER_SZ);     //see todos
    if (user_str_len < 0){
        printf("Error setting up buffer, error = %d", user_str_len);
        exit(2);
    }

switch (opt) {
        case 'c':
            rc = count_words(buff, BUFFER_SZ, user_str_len);
            printf("Word Count: %d\n", rc);
            break;

        //TODO:  #5 Implement the other cases for 'r' and 'w' by extending
        //       the case statement options

        case 'r': 
            reverse_string(buff, user_str_len);
            buff[user_str_len] = '\0'; 
            printf("Reversed String: %s\n", buff);
            break;

        case 'w': 
            word_print(buff, user_str_len);
            printf("%s", buff + user_str_len); 
            break;

        case 'x': 
            if (argc != 5) {
                printf("Error: The -x option requires exactly 3 arguments.\n");
                usage(argv[0]);
                free(buff);
                exit(1);
            }
            printf("Not Implemented!\n");
            break;

       

        default:
            //printf("default");
            usage(argv[0]);
            free(buff);
            exit(1);
    }
    if (opt != 'w') {
        print_buff(buff, BUFFER_SZ);
    }

    //TODO:  #6 Dont forget to free your buffer before exiting

    free(buff);
    return 0;
}


//TODO:  #7  Notice all of the helper functions provided in the 
//          starter take both the buffer as well as the length.  Why
//          do you think providing both the pointer and the length
//          is a good practice, after all we know from main() that 
//          the buff variable will have exactly 50 bytes?
//  
//          PLACE YOUR ANSWER HERE

/*
It is a precautionary measure as it makes sure that functions does not exceed the give lenght. This is a good way to make sure we are handing 
memory usefully. And catch errors easily. 



*/