#ifndef _COMMANDS_H_
#define _COMMANDS_H_

#ifdef __cplusplus
extern "C" {
#endif


void format_command(int argc, char **argv);
void ls_command(int argc, char **argv); 
void rm_command(int argc, char **argv); 
void mv_command(int argc, char **argv); 
void cp_command(int argc, char **argv);
void mkdir_command(int argc, char **argv);
void upload_command(int argc, char **argv); 

#ifdef __cplusplus
}
#endif

#endif

