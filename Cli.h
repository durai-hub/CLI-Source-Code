//! @Cli.h
//!
//! The header file for function declare and Macro constants  
//!
//!



#ifndef CLI_H
#define CLI_H
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>


#define SET_TO_ZERO 0
#define BUFFER_SIZE_100 100
#define INDEX_0 0
#define INDEX_1 1 
#define INIT_SIGNED_VALUE           -1
#define TRUE  1
#define FALSE 0
#define COMMAND_INT_MAX_OUTPUT_SIZE 1024
#define CLI_MAX_SHELL_COMMAND_LENGTH 64
#define HISTORY_SIZE  32
#define CLI_MAX_OUTPUT_LENGTH  128
#define CLI_MAX_COMMAND_LENGTH 128
#define TASKDELAY 100


//********************************
#define  IAC        255
#define  DONT       254
#define  DO         253
#define  WONT       252
#define  WILL       251
#define  SB         250
#define  GA         249
#define  EL         248
#define  EC         247
#define  AYT        246
#define  AO         245
#define  IP         244
#define  BRK        243
#define  DATA_MARK  242
#define  NOP        241
#define  SE         240

#define ESCAPE_CHAR                                (0x1B)
#define ARROW                                      (0x5B)
#define UP_ARROW                                   (0x41)
#define DOWN_ARROW                                 (0x42)


#define U	0x01	/* upper */
#define L	0x02	/* lower */
#define D	0x04	/* digit */
#define C	0x08	/* cntrl */
#define P	0x10	/* punct */
#define S	0x20	/* white space (space/lf/tab) */
#define X	0x40	/* hex digit */
#define SP	0x80	/* hard space (0x20) */
#define _U	0x01	/* upper */
#define _L	0x02	/* lower */
#define _D	0x04	/* digit */
#define _C	0x08	/* cntrl */
#define _P	0x10	/* punct */
#define _S	0x20	/* white space (space/lf/tab) */
#define _X	0x40	/* hex digit */
#define _SP	0x80	/* hard space (0x20) */
#define __ismask(x) (_ctype[(int)(unsigned char)(x)])
#define isalnum(c)	((__ismask(c)&(_U|_L|_D)) != 0)
#define isalpha(c)	((__ismask(c)&(_U|_L)) != 0)
#define iscntrl(c)	((__ismask(c)&(_C)) != 0)
#define isdigit(c)	((__ismask(c)&(_D)) != 0)
#define isgraph(c)	((__ismask(c)&(_P|_U|_L|_D)) != 0)
#define islower(c)	((__ismask(c)&(_L)) != 0)
#define isprint(c)	((__ismask(c)&(_P|_U|_L|_D|_SP)) != 0)
#define ispunct(c)	((__ismask(c)&(_P)) != 0)
#define isspace(c)	((__ismask(c)&(_S)) != 0)
#define isupper(c)	((__ismask(c)&(_U)) != 0)
#define isxdigit(c)	((__ismask(c)&(_D|_X)) != 0)
/*
 * Rather than doubling the size of the _ctype lookup table to hold a 'blank'
 * flag, just check for space or tab.
 */
#define isblank(c)	(c == ' ' || c == '\t')
#define isascii(c) (((unsigned char)(c))<=0x7f)
#define toascii(c) (((unsigned char)(c))&0x7f)

#define xCommandLineInput CLI_Command_Definition_t


static const unsigned char _ctype[] = {
_C,_C,_C,_C,_C,_C,_C,_C,			/* 0-7 */
_C,_C|_S,_C|_S,_C|_S,_C|_S,_C|_S,_C,_C,		/* 8-15 */
_C,_C,_C,_C,_C,_C,_C,_C,			/* 16-23 */
_C,_C,_C,_C,_C,_C,_C,_C,			/* 24-31 */
_S|_SP,_P,_P,_P,_P,_P,_P,_P,			/* 32-39 */
_P,_P,_P,_P,_P,_P,_P,_P,			/* 40-47 */
_D,_D,_D,_D,_D,_D,_D,_D,			/* 48-55 */
_D,_D,_P,_P,_P,_P,_P,_P,			/* 56-63 */
_P,_U|_X,_U|_X,_U|_X,_U|_X,_U|_X,_U|_X,_U,	/* 64-71 */
_U,_U,_U,_U,_U,_U,_U,_U,			/* 72-79 */
_U,_U,_U,_U,_U,_U,_U,_U,			/* 80-87 */
_U,_U,_U,_P,_P,_P,_P,_P,			/* 88-95 */
_P,_L|_X,_L|_X,_L|_X,_L|_X,_L|_X,_L|_X,_L,	/* 96-103 */
_L,_L,_L,_L,_L,_L,_L,_L,			/* 104-111 */
_L,_L,_L,_L,_L,_L,_L,_L,			/* 112-119 */
_L,_L,_L,_P,_P,_P,_P,_C,			/* 120-127 */
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		/* 128-143 */
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		/* 144-159 */
_S|_SP,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,   /* 160-175 */
_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,       /* 176-191 */
_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,       /* 192-207 */
_U,_U,_U,_U,_U,_U,_U,_P,_U,_U,_U,_U,_U,_U,_U,_L,       /* 208-223 */
_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,       /* 224-239 */
_L,_L,_L,_L,_L,_L,_L,_P,_L,_L,_L,_L,_L,_L,_L,_L};      /* 240-255 */

/* Global Variables */

/* The prototype to which callback functions used to process command line
* commands must comply.  pcWriteBuffer is a buffer into which the output from 
* executing the command can be written, xWriteBufferLen is the ullength, in bytes of 
* the pcWriteBuffer buffer, and pcCommandString is the entire string as input by
* the user (from which parameters can be extracted).
*/

typedef bool (*pdCOMMAND_LINE_CALLBACK)(int argc, char **argv );

/* The structure that defines command line commands.  A command line command
should be defined by declaring a const structure of this type. */
typedef struct xCOMMAND_LINE_INPUT
{
     const int8_t * const pcCommand;/* The command that causes pxCommandInterpreter to be executed.  For example "help".  Must be all lower case. */
     const int8_t * const pcHelpString;/* String that describes how to use the command.Should start with the command itself,and end with "\r\n".For example "help: Returns a list of all the commands\r\n". */
     const pdCOMMAND_LINE_CALLBACK pxCommandInterpreter;/* A pointer to the callback function that will return the output generated by the command. */
     int8_t cExpectedNumberOfParameters;/* Commands expect a fixed number of parameters, which may be zero. */
} CLI_Command_Definition_t;


typedef struct xCOMMAND_INPUT_LIST
{
	const CLI_Command_Definition_t *pxCommandLineDefinition;
	struct xCOMMAND_INPUT_LIST *pxNext;
} CLI_Definition_List_Item_t;


/** @fn static long helpCommand( int8_t *pcWriteBuffer, size_t xWriteBufferLen,  int argc,char * *argv );
 *  @brief 	Call back function of help command 
 *
 *         
 *  @param  :
 *  int8_t *pcWriteBuffer            - Points to the first byte of debug message or error message
 *  size_t xWriteBufferLen           - is the value of write buffer length
 *  int argc                         - is the number of strings pointed to by argv
 *  char * *argv                     - is a pointer to the arguments.
 *
 *  @return : 0 - success
 *  @pre
 *  @invariant
 *  @post
 */
bool helpCommand(int argc,char * *argv );

/** @fn long CLIRegisterCommand( const CLI_Command_Definition_t * const pxCommandToRegister )
 *  @brief 	Register the command 
 *           
 *  @param
 * const CLI_Command_Definition_t * const pxCommandToRegister - Structure ptr ponit to the 
 *                                                              new command 
 *  @return : xReturn - False - Fail to register 
 *  @pre                True  - Successfully registered 
 *  @invariant
 *  @post
 */
long CLIRegisterCommand( const CLI_Command_Definition_t * const pxCommandToRegister );

/** @fn long CLIProcessCommand( int8_t * pcWriteBuffer, size_t xWriteBufferLen,int argc, char * * argv)
 *  @brief 	Process the each command 
 *              CLIProcessCommand should be called repeatedly until it returns FALSE.
 *  @param
 *  int8_t *pcWriteBuffer            - Points to the first byte of debug message or error message
 *  size_t xWriteBufferLen           - is the value of write buffer length
 *  int argc                         - is the number of strings pointed to by argv
 *  char * *argv                     - is a pointer to the arguments.
 *
 *  @return : xReturn - False - Command not found 
 *  @pre                True  - Success  
 *  @invariant
 *  @post
 */
long CLIProcessCommand(int8_t *pcWriteBuffer, size_t xWriteBufferLen ,int argc, char * * argv );



/** @fn static int8_t getNumberOfParameters( const int8_t * pcProcessString )
 *  @brief 	get the number of parameter entered after the command in console 
 *          get the number of parameter entered in console 
 *
 *  @param :
 *  const int8_t * pcProcessString - Points to the string
 *
 *  @return : ciParameters -The value returned is one less than the number of space delimited words
 *  @pre
 *  @invariant
 *  @post
 */
int8_t getNumberOfParameters( const int8_t * pcCommandString );


/** @fn history_add(char * command_line)
 *  @brief 	Add the command to history
 *
 *          User enter command will stored to history buffer
 *  @param  :
 *  char * command_line - Point to the string
 *  @return : none
 *  @pre
 *  @invariant
 *  @post
 */
void history_add(char * cpCommand_line);

/** @fn void history_get_next( char * cpCommand_line, unsigned int len, unsigned int *pos)
 *  @brief 	Read the prev command from history
 *          Read and display the previous command from history
 *
 *  @param :
 *  char * cpCommand_line - Points to the string
 *  unsigned int len      - is the length of string
 *  unsigned int *pos     - is the position in the history
 *
 *  @return : none
 *  @pre
 *  @invariant
 *  @post
 */
void history_get_prev(char * cpCommand_line, uint32_t ulLen, uint32_t *ulPos);

/** @fn void history_get_next( char * cpCommand_line, unsigned int len, unsigned int *pos)
 *  @brief 	Read the next command from history
 *          Read and display the next command from history
 *
 *  @param :
 *  char * cpCommand_line - Points to the string
 *  unsigned int len      - is the length of string
 *  unsigned int *pos     - is the position in the history
 *
 *  @return : none
 *  @pre
 *  @invariant
 *  @post
 */
void history_get_next(char * cpCommand_line, uint32_t ulLen, uint32_t *ulPos);

/** @fn void delete_char(int8_t * cpChar)
 *  @brief 	Delete the char
 *
 *   		Delete the char in buffer
 *
 *  @param : *cpChar - Pointer to the first char
 *  @return :None
 *  @pre
 *  @invariant
 *  @post
 */
void delete_char(char * cpChar);

/** @fn void get_arguments(char * command_line, int * argc, char * argv[])
 *  @brief 	To get the argc and argv
 *
 *          parse the command and segregate the argc and argv
 *
 *  @param  :
 *  char * command_line  - point to the string
 *  int argc             - is the number of strings pointed to by argv
 *  char * *argv         - is a pointer to the arguments.
 *  @return : None
 *  @pre
 *  @invariant
 *  @post
 */
void get_arguments(char * cpCommand_line, int32_t * argc, char * argv[]);

/** @fn char get_command(char * command_line, unsigned int len)
 *  @brief 	To get the command
 *
 *          get the command and store to history
 *  @param  :
 *  char * command_line     - point to the string
 *  unsigned int len        - is the length of string
 *  @return : res-1         - command entered
 *            res-0         - command failure
 *  @pre
 *  @invariant
 *  @post
 */
char get_command(char * cpCommand_line, uint32_t ulLen);

/** @fn char get(char * line, unsigned int len, char complete_tab, char get_history)
 *  @brief 	get the char
 *
 *          Get the char from uart and store to line buffer
 *
 *  @param  :
 *  char * line       - Point to the first byte in buffer
 *  unsigned int len  - is the length of char received
 *  char get_history  - is the history flag for enable Prev and next command select
 *  @return : 1 - success; 0-failure
 *  @pre
 *  @invariant
 *  @post
 */
char get(char * cpLine, uint32_t ulLen, int8_t lGet_history);

/** @fn bool processHelpCmd(int argc,char * *argv)
 *  @brief   Process the help command 
 *
 *           Display the help message for given command 	
 *
 *  @param : None
 *  @return : Success 
 *  @pre
 *  @invariant
 *  @post
 */
bool processHelpCmd(int argc,char * *argv);

/** @fn char getch(void)
 *  @brief 	 Read 1 character without echo
 *
 *   		 Read 1 character without echo
 *
 *  @param : None
 *  @return : uart output char without echo
 *  @pre
 *  @invariant
 *  @post
 */
char getch(void);

/** @fn int32_t ctrlc (void)
 *  @brief 	Ctrl+C use for Break the loop
 *
 *          Ctrl+C which is used for break the infinite loop
 *  @param  :
 *  *pvParameters - point to the task parameter which is pass from task create
 *  @return : 1 - success;
 *  @pre
 *  @invariant
 *  @post
 */
extern int ctrlc (void);


/** @fn uint8_t toupper_strtoul(uint8_t ucData)
 *  @brief 	Convert lowercase letter to uppercase
 *
 *          Convert lowercase letter to uppercase
 *  @param  :
 *  uint8_t ucData - Character to be converted, casted to an int, or EOF.

 *
 *  @return : ucData
 *  @pre
 *  @invariant
 *  @post

 */
uint8_t toupper_strtoul(uint8_t ucData);


/** @fn uint32_t simple_strtoul(const char *cp,char **endp,unsigned int base)
 *  @brief 	Convert string to an unsigned long
 *
 *          Convert string to an unsigned long
 *  @param  :
 *  const char *cp - Pointer to the start of string
 *  char **endp    -  pointer to the end of the parsed string will be placed here
 *  unsigned int base - Number base(16)
 *  @return : result- Data
 *  @pre
 *  @invariant
 *  @post
 */
uint32_t strtoul_simple(const char *cp,char **endp,unsigned int base);


#endif /* CLI_H */













