//! @Cli.c
//!
//! This modules register the commands and execute it.
//!
//!


/* Standard includes. */
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include  "uartStdio.h"
//#include <termios.h>
#include   "tx_api.h"
/* Utils includes. */
#include "Cli.h"
#include "Cmd.h"

#define pdTRUE		( 1 )
#define pdFALSE		( 0 )
#define pdFAIL		( 1 )
#define pdPASS		( 0 )

/* Global Variables */
char g_cHistory[HISTORY_SIZE][CLI_MAX_SHELL_COMMAND_LENGTH];
int8_t g_cHistory_head = SET_TO_ZERO;
int8_t g_cHistory_tail = SET_TO_ZERO;
int8_t g_cHistory_pos = SET_TO_ZERO;
void *globalFreeMemPtr;

extern TX_BYTE_POOL byte_pool_0;
TX_BYTE_POOL CliBytePool;
/* Defines section
*******************************************************************************/
/* Local function prototypes
******************************************************************************/

/* This structure defines the help command  */
static const CLI_Command_Definition_t xHelpCommand =
{
    ( const int8_t *) "help",
    ( const int8_t *) "\r\nhelp:\r\n Lists all the registered commands\r\n",
    helpCommand,
    SET_TO_ZERO
};


/* The definition of the list of commands.  Commands that are registered are
added to this list. */
static CLI_Definition_List_Item_t xRegisteredCommands =
{
	&xHelpCommand,	/* The first command in the list is always the help command, defined in this file. */
        		
        NULL /* The next pointer is initialised to NULL, as there are no other registered commands yet. */
};

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
long CLIRegisterCommand( const CLI_Command_Definition_t * const pxCommandToRegister )
{
	static CLI_Definition_List_Item_t *pxLastCommandInList = &xRegisteredCommands;
	CLI_Definition_List_Item_t *pxNewListItem;
	long xReturn = pdFAIL;
        unsigned char ucStatus = TX_SUCCESS;
   
#if 0
          ucStatus = tx_byte_pool_create(&CliBytePool, "CliBytePool",(unsigned char *)globalFreeMemPtr, 1024 );
          if (ucStatus != TX_SUCCESS)
          {
              ConsolePrintf ("Error in Allocating memory CLI Register  \n");
              return pdFAIL;
          }
#endif 
        /* Create a new list item that will reference the command being registered. */
          
          ucStatus = tx_byte_allocate(&byte_pool_0, (VOID * *) &pxNewListItem, sizeof( CLI_Definition_List_Item_t ), TX_NO_WAIT);
          if (ucStatus != TX_SUCCESS)
          {
              ConsolePrintf ("Error in Allocating memory for CLI register d\n");
              /* Free the memory allocated */
             // ucStatus = tx_byte_pool_delete(&CliBytePool);
              return pdFAIL;
          }
            	
	//pxNewListItem = ( CLI_Definition_List_Item_t * ) malloc(sizeof( CLI_Definition_List_Item_t ));
	
	if( pxNewListItem != NULL )
	{
		
		
			/* Reference the command being registered from the newly created
			list item. */
			pxNewListItem->pxCommandLineDefinition = pxCommandToRegister;

			/* The new list item will get added to the end of the list, so
			pxNext has nowhere to point. */
			pxNewListItem->pxNext = NULL;

			/* Add the newly created list item to the end of the already existing
			list. */
			pxLastCommandInList->pxNext = pxNewListItem;

			/* Set the end of list marker to the new list item. */
			pxLastCommandInList = pxNewListItem;
	

		xReturn = pdPASS;
	}
    
    //ucStatus = tx_byte_release((VOID *)pxNewListItem);
    //ucStatus = tx_byte_pool_delete(&CliBytePool);
    
	return xReturn;
}
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
long CLIProcessCommand( int8_t * pcWriteBuffer, size_t xWriteBufferLen,int argc, char **argv)
{
	volatile  CLI_Definition_List_Item_t *pxCommand = NULL;//NULL;
        //CLI_Definition_List_Item_t *pxCommand =NULL;
	long xReturn = pdTRUE;
	const int8_t *pcRegisteredCommandString;
	size_t xCommandStringLength;
        int8_t pcCommandInput[BUFFER_SIZE_100];
         
        strncpy((char *)pcCommandInput, (const char *) pcWriteBuffer, sizeof(pcCommandInput));
      

	/* Note:  This function is not re-entrant.  It must not be called from more
	thank one task. */

        if( pxCommand == NULL )
	{
		/* Search for the command string in the list of registered commands. */
		for( pxCommand = &xRegisteredCommands; pxCommand != NULL; pxCommand = pxCommand->pxNext )
		{
			pcRegisteredCommandString = pxCommand->pxCommandLineDefinition->pcCommand;
			xCommandStringLength = strlen( ( const char * ) pcRegisteredCommandString );

			/* To ensure the string lengths match exactly, so as not to pick up
			a sub-string of a longer command, check the byte after the expected
			end of the string is either the end of the string or a space before
			a parameter. */
            if( ( pcCommandInput[ xCommandStringLength ] == ' ' ) || ( pcCommandInput[ xCommandStringLength ] == SET_TO_ZERO ) )
			{
                if( strncasecmp( ( const char * ) pcCommandInput, ( const char * ) pcRegisteredCommandString, xCommandStringLength ) == SET_TO_ZERO )
				{
					/* The command has been found.  Check it has the expected
					number of parameters.  If cExpectedNumberOfParameters is -1,
					then there could be a variable number of parameters and no
					check is made. */
                    if( pxCommand->pxCommandLineDefinition->cExpectedNumberOfParameters >= SET_TO_ZERO )
			{
                                  if( (strcasecmp(argv[INDEX_0],"help")) )
                                   {
                                                  if( getNumberOfParameters( (const int8_t *)pcCommandInput ) != pxCommand->pxCommandLineDefinition->cExpectedNumberOfParameters )
                                                  {

                                                      xReturn = pdFALSE;

                                                  }
                                   }
			}

					break;
				}
			}
		}
	}

	if( ( pxCommand != NULL ) && ( xReturn == pdFALSE ) )
	{
		/* The command was found, but the number of parameters with the command
		was incorrect. */
        strncpy( ( char * ) pcWriteBuffer, "Incorrect command parameter(s).  Enter \"help 'Command'\" to view a Usage.\r\n\r\n", xWriteBufferLen );
		pxCommand = NULL;
	}
	else if( pxCommand != NULL )
    {
        memset( pcWriteBuffer, SET_TO_ZERO, CLI_MAX_OUTPUT_LENGTH  );
		/* Call the callback function that is registered to this command. */
        xReturn = pxCommand->pxCommandLineDefinition->pxCommandInterpreter(/*( int8_t *) pcWriteBuffer,xWriteBufferLen,*/
                argc,argv);
              

		/* If xReturn is pdFALSE, then no further strings will be returned
		after this one, and	pxCommand can be reset to NULL ready to search
		for the next entered command. */
        if( xReturn == pdFALSE )
		{
			pxCommand = NULL;
		}
	}
	else
	{
		/* pxCommand was NULL, the command was not found. */
		strncpy( ( char * ) pcWriteBuffer, ( const char * ) "Command not recognised.  Enter \"help\" to view a list of available commands.\r\n\r\n", xWriteBufferLen );
		xReturn = pdFALSE;
	}

	return xReturn;
}
/** @fn static long prvHelpCommand( int8_t *pcWriteBuffer, size_t xWriteBufferLen,  int argc,char * *argv )
 *  @brief 	Execute the help command 
 *           
 *  @param
 *  int8_t *pcWriteBuffer            - Points to the first byte of debug message or error message
 *  size_t xWriteBufferLen           - is the value of write buffer length
 *  int argc                         - is the number of strings pointed to by argv
 *  char * *argv                     - is a pointer to the arguments.
 *
 *  @return : xReturn - False - No more command in the list
 *  @pre                True  - Commands are available 
 *  @invariant
 *  @post
 */

bool helpCommand(int argc,char * *argv )
{
        volatile CLI_Definition_List_Item_t * pxCommand = NULL;
        bool xReturn;

        (void) argc;

	if( pxCommand == NULL )
	{
		/* Reset the pxCommand pointer back to the start of the list. */
		pxCommand = &xRegisteredCommands;
	}


    if(argc>=2)
     {
        /* Process the help command for individual Module */
        xReturn=processHelpCmd(argc , argv);

        if(xReturn==pdTRUE)
        {
            return pdTRUE;

        }
     }

    while(pxCommand)
    {
         ConsolePrintf("\n\r %s",pxCommand->pxCommandLineDefinition->pcHelpString);
         pxCommand = pxCommand->pxNext;
    }

    if( pxCommand == NULL )
    {
        /* There are no more commands in the list, so there will be no more
        strings to return after this one and pdFALSE should be returned. */
        xReturn = pdFALSE;
    }

       ConsolePrintf("\r\n");
      
	return xReturn;
}



/** @fn static int8_t getNumberOfParameters( const int8_t * pcProcessString )
 *  @brief 	get the number of parameter entered in console 
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
 int8_t getNumberOfParameters( const int8_t * pcProcessString )
{

    int8_t ciParameters = SET_TO_ZERO;
	long xLastCharacterWasSpace = pdFALSE;

	/* Count the number of space delimited words in pcProcessString. */
    while( *pcProcessString != SET_TO_ZERO )
	{
        if( ( *pcProcessString ) == ' ')
		{
			if( xLastCharacterWasSpace != pdTRUE )
			{
				ciParameters++;

				xLastCharacterWasSpace = pdTRUE;
			}
		}
		else
		{
			xLastCharacterWasSpace = pdFALSE;
		}

		pcProcessString++;

	}

	/* Check the spaces and find the no of parameters*/
	if( xLastCharacterWasSpace == pdTRUE )
	{
        //ciParameters--;
	}

	/* The value returned is one less than the number of space delimited words,as the first word should be the command itself. */
	return ciParameters;
}


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
char get_command(char * cpCommand_line, uint32_t ulLen)

{
    uint8_t ulRes = get(cpCommand_line, ulLen, INDEX_1);
        if (ulRes)
        {
            if (strcmp(cpCommand_line, "") != SET_TO_ZERO)
            {
                history_add(cpCommand_line);
            }
        }
        return ulRes;
    }
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
void get_arguments(char * cpCommand_line, int32_t * argc, char * argv[])
{
    char * cpCur =NULL;
    int32_t in_quote = SET_TO_ZERO;
    int32_t in_escape = SET_TO_ZERO;
    argv[*argc] = cpCur = cpCommand_line;
   
    while (*cpCur)
    {
        switch (*cpCur)
        {
            case '"':
                if (in_escape)
                {
                    break;
                }
                else
                {
                    in_quote = !in_quote;
                    delete_char(cpCur);
                    continue;
                }

            case ' ':
            case '\t':
                if (in_quote || in_escape)
                {
                    break;
                }
                else
                {  
                    *cpCur = SET_TO_ZERO;
                    if (argv[*argc])
                    {
                        (*argc)++;

                        argv[*argc] = NULL;
                    }
                    cpCur++;
                    continue;
                }

            case '\\':
                if (in_escape)
                {
                    break;
                }
                else
                {
                    in_escape = INDEX_1;
                    delete_char(cpCur);
                    continue;
                }

            default:
                break;
        }
        if (!argv[*argc])
        {
            argv[*argc] = cpCur;
        }

        cpCur++;
        in_escape = SET_TO_ZERO;
    }

    if (argv[*argc])
    {
        argv[++(*argc)] = NULL;
    }
}
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
char get(char * cpLine, uint32_t ulLen, int8_t lGet_history)
{
    uint32_t ulPos = SET_TO_ZERO;
   // bool complete_tab = 1;
    while (ulPos < ulLen)
    {

        char ch =  UARTConsoleGetc(); //getch();

        if ((signed char)ch == INIT_SIGNED_VALUE)
        {
            return FALSE;
        }else if (ch== 0x03 ) /* Check the end of text */
        {
              break;
        }
        else if (ch == '\b')
        {
            if (ulPos > SET_TO_ZERO)
            {
                ulPos--;
               ConsolePrintf("\b \b");
            }
        }else if (ch == '\r')
        {
            /* end of lines are sent as either \r\n or \r\x00, in
             * either case we will just eat the next char */

             ConsolePrintf("\n");

            break;
        }
        else if (ch == '\n')
        {

             ConsolePrintf("\n");
             break;

        }
        else if (ch == '\t')
        {
             /*if (complete_tab)
                {
                   tab_complete(cpLine, ulLen, ulPos);
                }*/

        }
        else if (ch == ESCAPE_CHAR)
        {
            /* Handle ANSI escape codes.  See: http://en.wikipedia.org/wiki/ANSI_escape_code */

            /* we are handling a very limited subset of the codes..
             * infact right now we only support up or down one line
             * (for history) */

            const int32_t clOpen_bracket = UARTConsoleGetc(); //getch();


            if (clOpen_bracket == INIT_SIGNED_VALUE)
            {
                return FALSE;
            }
            else if (clOpen_bracket == '[')
            {
                int junk;
                do
                {

                    junk = UARTConsoleGetc(); //getch();
           if (junk == INIT_SIGNED_VALUE)
                    {
                        return FALSE;
                    }
                } while (junk < 64 || junk > 126);

                if (junk == 'A')
                {

                    /* cursor UP */
                    if (lGet_history)
                    {
                        history_get_prev(cpLine, ulLen, &ulPos);
                    }
                }
                else if (junk == 'B')
                {

                    /* cursor DOWN */
                    if (lGet_history)
                    {
                        history_get_next(cpLine, ulLen, &ulPos);
                    }
                }
            }
            else
            {
                /* Not an escape code after all.. ignore */
            }
        }
        else if (ch == IAC)
        {

            const int32_t clCommand = UARTConsoleGetc(); //getch();
            if (clCommand == INIT_SIGNED_VALUE)
            {
                return FALSE;
            }
            switch (clCommand) {
                case DONT:
                case DO:
                case WONT:
                case WILL:
                {
                    const int option = UARTConsoleGetc(); //getch();
                    if (option == INIT_SIGNED_VALUE)
                      {
                         return FALSE;
                       }

                    /* ignore option for now */
                }
                break;

                case SB:
                case GA:
                case EL:
                case EC:
                case AYT:
                case AO:
                case IP:
                case BRK:
                case DATA_MARK:
                case NOP:
                case SE:
                    break;

                default:
                    break;
            }
        }
        else
        {
            cpLine[ulPos++] = (char) ch;
            UARTConsolePutc(ch); //putchar(ch);
        }
    }
    if (ulPos >= ulLen)
    {
        ulPos = ulLen - INDEX_1;
    }
    cpLine[ulPos] = SET_TO_ZERO;
    return TRUE;
}
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
void history_add(char * cpCommand_line)
{
    strncpy(g_cHistory[g_cHistory_tail], cpCommand_line, sizeof(g_cHistory[g_cHistory_tail]));
    g_cHistory_tail = (g_cHistory_tail + INDEX_1) % HISTORY_SIZE;
    if (g_cHistory_tail == g_cHistory_head)
    {
        g_cHistory_head = (g_cHistory_head + INDEX_1) % HISTORY_SIZE;
    }
    g_cHistory_pos = g_cHistory_tail;
}
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
void history_get_prev(char * cpCommand_line, uint32_t ulLen, uint32_t *ulPos)
{
    uint32_t ulCount=SET_TO_ZERO;
    /* If the history is empty don't do anything */
        if (g_cHistory_head == g_cHistory_tail)
        {
            return;
        }

        /* If we are at the top, don't do anything */
        if (g_cHistory_pos == g_cHistory_head)
        {
            return;
        }

        cpCommand_line[*ulPos] = SET_TO_ZERO;
        const uint32_t ulOld_line_len = strlen(cpCommand_line);

        /* If we are at the bottom, save the partial command line so the
         * user can retrieve it */
        if (g_cHistory_pos == g_cHistory_tail)
        {
            strncpy(g_cHistory[g_cHistory_tail],cpCommand_line, sizeof(g_cHistory[g_cHistory_tail]));
        }

        if (SET_TO_ZERO == g_cHistory_pos)
        {
            g_cHistory_pos = HISTORY_SIZE - INDEX_1;
        }
        else
        {
            g_cHistory_pos--;
        }

        strncpy(cpCommand_line, g_cHistory[g_cHistory_pos], ulLen);
        *ulPos = strlen(cpCommand_line);

        char cClear_string[(CLI_MAX_SHELL_COMMAND_LENGTH * 3) + INDEX_1];
        strncpy(cClear_string, "", sizeof(cClear_string));
        for ( ulCount = SET_TO_ZERO; ulCount < ulOld_line_len; ulCount++)
        {
            strncat(cClear_string, "\b \b", sizeof(cClear_string));
        }

        ConsolePrintf("%s%s", cClear_string, cpCommand_line);
}
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
void history_get_next( char * cpCommand_line, uint32_t ulLen, uint32_t *ulPos)
{
    uint32_t ulCount=SET_TO_ZERO;
     /* If the history is empty don't do anything */
        if (g_cHistory_head == g_cHistory_tail)
        {
            return;
        }

        /* If we are at the bottom, don't do anything */
        if (g_cHistory_pos == g_cHistory_tail)
        {
            return;
        }

        cpCommand_line[*ulPos] = SET_TO_ZERO;
        const uint32_t ulOld_line_len = strlen(cpCommand_line);

        g_cHistory_pos = (g_cHistory_pos + INDEX_1) % HISTORY_SIZE;

        strncpy(cpCommand_line, g_cHistory[g_cHistory_pos], ulLen);
        *ulPos = strlen(cpCommand_line);

        for ( ulCount = SET_TO_ZERO; ulCount < ulOld_line_len; ulCount++)
        {
            ConsolePrintf("\b \b");
        }
       ConsolePrintf("%s", cpCommand_line);
}
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
void delete_char(char * cpChar)
{
    do
    {
        *cpChar = *(cpChar + INDEX_1);
    } while (*++cpChar);
}

#if 0 /* These function may be useful for char receive in linux userspace */

/** @fn void initTermios(int echo)
 *  @brief 	get the one char from uart console
 *
 *   		get the one char from uart console
 *

 *  @param : int echo - enable or disable the "\n" char
 *  @return :None
 *  @pre
 *  @invariant
 *  @post
 */
/* Initialize new terminal i/o settings */
void initTermios(int echo)
{
  tcgetattr(SET_TO_ZERO, &old); /* grab old terminal i/o settings */
  new = old; /* make new settings same as old settings */
  new.c_lflag &= ~ICANON; /* disable buffered i/o */
  new.c_lflag &= echo ? ECHO : ~ECHO; /* set echo mode */
  tcsetattr(SET_TO_ZERO, TCSANOW, &new); /* use these new terminal i/o settings now */
}
 

/** @fn void resetTermios(void)
 *  @brief 	Restore old terminal i/o settings
 *
 *   		Restore old terminal i/o settings
 *
 *  @param : None
 *  @return :None
 *  @pre
 *  @invariant
 *  @post
 */
void resetTermios(void)
{
  tcsetattr(SET_TO_ZERO, TCSANOW, &old);
}

/** @fn char getch_(int echo)
 *  @brief 	 Read 1 character - echo defines echo mode
 *
 *   		 Read 1 character - echo defines echo mode
 *
 *  @param : int echo - enable or disable the "\n" char
 *  @return : uart output char
 *  @pre
 *  @invariant
 *  @post
 */
char getch_(int echo)
{
  char ch;
  initTermios(echo);
  ch = getchar();
  resetTermios();
  return ch;
}
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
char getch(void)
{
  return getch_(SET_TO_ZERO);
}
/** @fn char getche(void)
 *  @brief 	 Read 1 character with echo
 *
 *   		 Read 1 character with echo
 *
 *  @param : None
 *  @return : uart output char with echo
 *  @pre
 *  @invariant
 *  @post
 */
char getche(void)
{
  return getch_(INDEX_1);
}

#endif

/** @fn uint8_t toupper(uint8_t ucData)
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
uint8_t toupper_strtoul(uint8_t ucData)
{
    if (islower(ucData))
        ucData -= 'a'-'A';
    return ucData;
}


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
uint32_t strtoul_simple(const char *cp,char **endp,unsigned int base)
{
    unsigned long result = SET_TO_ZERO,value;

    if ('0' == *cp) {
        cp++;
        if (('x' == *cp) && isxdigit(cp[INDEX_1]))
        {
            base = 16;
            cp++;
        }
        if (!base) {
            base = 8;
        }
    }
    if (!base) {
        base = 10;
    }
    while (isxdigit(*cp) && (value = isdigit(*cp) ? *cp-'0' : (islower(*cp) ? toupper_strtoul(*cp) : *cp)-'A'+10) < base)
    {
        result = result*base + value;
        cp++;
    }
    if (endp)
        *endp = (char *)cp;
    return result;
}
bool  processHelpCmd(int argc,char * *argv)
{

  uint8_t u8Cnt=SET_TO_ZERO;

  if(argc>=INDEX_1)
  {
        for(u8Cnt=SET_TO_ZERO; ;u8Cnt++)
        {
            /* if command is not found */
            if(strcmp("0", cmdHelpRun[u8Cnt].str2) ==SET_TO_ZERO)
             {
                ConsolePrintf("\n\r Incorrect command Parameter!! ");
                return pdFALSE;
             }
             /* Compare the argv[1] for help "cmd"; Compare argv[0] for giving wrong input for available list commands so cmd name store in argv[0]*/
            if(argc>=2)
            {

                 if (  (strcmp(argv[INDEX_1], cmdHelpRun[u8Cnt].str1) == SET_TO_ZERO) )

                    {
                        ConsolePrintf(cmdHelpRun[u8Cnt].str2);
                       return pdTRUE;
                    }

            }else {
                        if ( (strcmp(argv[INDEX_0], cmdHelpRun[u8Cnt].str1) == SET_TO_ZERO) )

                           {
                               ConsolePrintf(cmdHelpRun[u8Cnt].str2);
                              return pdTRUE;
                           }
                   }
         }
    }

   return pdFALSE;

}


#if 0
/** @fn int ctrlc (void)
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
int ctrlc (void)
{
    if (ROM_UARTCharsAvail(CONSOLE))
    {
        switch(ROM_UARTCharGetNonBlocking(CONSOLE))
        {
        case 0x03:		/* ^C - Control C */
            return TRUE;
        default:
            break;
        }
    }
    return CMDSUCCESS;
}
#endif


/************************************************** END ----- END ----- END ******************************************************/

