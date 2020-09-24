//! @Cmd.c
//!
//! This modules provide the API for test commands to test the HW
//!
//!


/* Standard Lib */
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "math.h"
#include <string.h>
#include <stdbool.h>

/* CLI includes. */
#include "uartStdio.h"
#include "Cli.h"
#include "tx_api.h"

extern void CLI_Task( ULONG thread_input); 
extern void vRegisterCLICommands( void );

/* Variables: file scope */
#define PROMPT "Chrysalis# "
#define INIT_STRING_NULL            '\0'
#define SUCCESS                      	0
#define FAILURE                      	1
#define pdFALSE		( 0 )
/******************************************************************************/

/* Global Variables */


#if 0
int main()
{
    vRegisterCLICommands();
    CLI_Task();
    return SUCCESS;
}
#endif


void CLI_Task( ULONG thread_input)
{
   
    bool bCliProcessReturn;
    /* The input and output buffers are declared static to keep them off the stack. */
    char caOutputString[ CLI_MAX_OUTPUT_LENGTH ],caMaxCommand[CLI_MAX_COMMAND_LENGTH];

    /* Register the all commands */
    vRegisterCLICommands();
    
    /* Print the command prompt name */
    ConsolePrintf("\r\n");
    ConsolePrintf(PROMPT);
 
    /* Initialize the output buffer with Zero */
    memset( caOutputString, SET_TO_ZERO, CLI_MAX_OUTPUT_LENGTH  );

    while(1)
    {
        /* Get the command from uart console */
        if (get_command(caMaxCommand, CLI_MAX_SHELL_COMMAND_LENGTH))
        {
            int32_t argc = SET_TO_ZERO;
            char  *argv[12];
            argv[INDEX_0]=SET_TO_ZERO;
       
            strncpy(caOutputString, caMaxCommand, sizeof(caOutputString));
            memset( argv, INIT_STRING_NULL, sizeof(argv)  );
            /* get the actual command to execute */
            get_arguments(caMaxCommand, &argc, argv);
          
            /* Check the entered command with already registered commands and execute */
            if((strcmp(argv[INDEX_0], "")))
            {
                do{
                    /*Command string process with below function and out value return back with caOutputString*/
                    bCliProcessReturn = CLIProcessCommand(
                                (int8_t *)caOutputString,   /* The output buffer string of command output. */
                                CLI_MAX_OUTPUT_LENGTH,      /* The maximum size of the output buffer. */
                                argc,                       /* No of parameter for given command */
                                argv                        /* pointer to hold the command string */
                                );

                    /* Send the ouput to uart console */

                    ConsolePrintf("%s",caOutputString);
                    ConsolePrintf("\r\n");

                    if(FAILURE==bCliProcessReturn)
                    {
                        bCliProcessReturn=SET_TO_ZERO;
                        break;
                    }
                } while( bCliProcessReturn != pdFALSE );

                  /* Clear the output buffer */
                  memset( caOutputString, SET_TO_ZERO, CLI_MAX_OUTPUT_LENGTH  );
                  /* Clear the input buffer */
                  memset( caMaxCommand, SET_TO_ZERO, CLI_MAX_OUTPUT_LENGTH  );
                  /* send the executed command output to uart console */
                  ConsolePrintf(PROMPT);
            }else {
              
                      /* Clear the output buffer */
                      memset( caOutputString, SET_TO_ZERO, CLI_MAX_OUTPUT_LENGTH  );
                      /* Clear the input buffer */
                      memset( caMaxCommand, SET_TO_ZERO, CLI_MAX_OUTPUT_LENGTH  );
                                             
                      ConsolePrintf("\r\n");
                      /* send the executed command output to uart console */
                      ConsolePrintf(PROMPT);
                  }
       }
       
    /* Sleep for 10 ticks.  */
    tx_thread_sleep(10);
   }

}

/* CLI commands Implmentation */ 
bool test(int argc,char * *argv)
{

  ConsolePrintf("\n\r Test Command executed successfully \n\r"); 
  
  return 0;
}

bool hello(int argc,char * *argv)
{

  ConsolePrintf("\n\r Hello World \n\r"); 
  
  return 0;
}

static const CLI_Command_Definition_t testCmd =
{
    ( const int8_t * ) "test",              /*command string*/
    ( const int8_t * ) "test:               test the command",
    test,                               /*command function*/
    0                                       /* Number of parameters accepted by command*/
};

static const CLI_Command_Definition_t helloCmd =
{
    ( const int8_t * ) "hello",              /*command string*/
    ( const int8_t * ) "hello:               Hello World",
    hello,                               /*command function*/
    0                                       /* Number of parameters accepted by command*/
};

//! This function will registers the diagnostics commands
//!
//! \param[in]  none
//!
//! \param[out] none
//!
//! \return none
//!
void vRegisterCLICommands( void )
{
    CLIRegisterCommand(&testCmd);
    CLIRegisterCommand(&helloCmd);
   
}


/************************************************** END ----- END ----- END ******************************************************/
