#include <stdio.h>
#include <termios.h>
#include <sys/signal.h>
#include <stdlib.h>
struct termios old_save;

void error_quit(char* err_string)
{
    fprintf(stderr,"%s\n",err_string);
    exit(1);
}

void change_mode(void)
{
    struct termios new_term;

    //不是终端
    if(!isatty(0))
    {
        error_quit("standard input is not a terminal");
    }

    if(tcgetattr(0,&new_term)==-1)
    {
        error_quit("get the attributes of the terminal");
    }

    old_save=new_term;

    new_term.c_lflag&=~ISIG;

    if(tcsetattr(0,TCSANOW,&new_term)==-1)
    {
        error_quit("set new terminal attributes");
    }
}

void restore_mode(void)
{
    //恢复终端的初始设置
    if(tcsetattr(0,TCSANOW,&old_save)==-1)
    {
        error_quit("restore terminal attrbutes");
    }
}

void handle_ctrl_c(int signo)
{
    printf("The process receive SIGINT,signal number=%d\n",signo);
    printf("Now The process will change its terminal attributes.\n");
    change_mode();
}


void handle_term_c(int signo)
{
    printf("The process receive SIGTERM,signal number=%d\n",signo);
    printf("The process will restore the terminal attributes and extis.\n");
    restore_mode();
}

void main()
{
    signal(SIGINT,handle_ctrl_c);
    signal(SIGTERM,handle_term_c);
    printf("please press the buttons ctrl+c\n");
    while(1);

}
