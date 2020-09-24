//! @Cmd.c
//!
//! Function declaration and macro for cmd.c 
//!
//!




/* Include section
***************************************************/

#ifndef CLI_COMMANDS_H_
#define CLI_COMMANDS_H_
   
struct strCmdHelp {
     char const *str1;
     char const *str2;
};

const struct strCmdHelp cmdHelpRun[] = {
    {  "test", "\n\r Execute the command \n\r\n\r Usage: \n\r test \n\r "},
    {  "hello","\n\r Print the hello world  \n\r\n\r Usage: \n\r hello \n\r Ex: hello\n\r"},
    { NULL, "0" }  /* end marker */
};


/** @fn void vRegisterCLICommands( void )
 *  @brief 	Register the commands 
 *
 *          Register the commands as a call back function 
 *  @param  : none
 *  
 *  @return : none;
 *  @pre
 *  @invariant
 *  @post
 */
void vRegisterCLICommands( void );

/** @fn bool test(int argc,char * *argv)
 *  @brief  test command 
 *
 *          example command to confirm CLI is working 
 *  @param  : none
 *   int argc - No of arguments 
 *   char * *argv - hold the input data for command  
 *  @return : sucess 
 *  @pre
 *  @invariant
 *  @post
 */
bool test(int argc,char * *argv);

bool hello(int argc,char * *argv);
#endif /*  CLI_COMMANDS_H_ */


