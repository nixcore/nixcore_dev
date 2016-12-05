/******************************************************************************
 * Main mipsrx application, designed to run on an embedded Linux system and
 * receive commands from a client application.  Used to download files and
 * get/set settings
 *
 * Author:  Andrew Gaylo - http://protological.com
 * Created: Aug 2015
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
******************************************************************************/
#include "stdint.h"
#include "stdbool.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

#include <fcntl.h>
#include <termios.h>

#include <sys/select.h>

#include <getopt.h>


#define ENABLE_SERIAL_PORT

#define US_PER_SECOND       (1000000)

#define MAIN_TIMEOUT_SEC    3

#define strset(B,S) do{strncpy((B),(S),sizeof(S)-1);}while(0)

// Types
// -------------------------------------------------

// Variables
// -------------------------------------------------
bool g_running;
bool g_verbose;

int m_tty_fd;

int m_state;

// Local prototypes
// -------------------------------------------------
void _main_app(int timeoutlimit_sec);
void _main_sigint_handler(int sig);
int serial_open(char * device,int baud);
void serial_close(int tty_fd);
void serial_flush(int tty_fd);

// Public functions
// -------------------------------------------------

int main(int argc,char** argv)
{
    int ret;
    int baud,timeout=MAIN_TIMEOUT_SEC;
    int optchar;
    char device[50];
    char c;

    struct option longopts[] =
    {
    { "verbose", no_argument,       0, 'v' },
    { "serial" , required_argument, 0, 's' },
    { "baud"   , required_argument, 0, 'b' },
    { 0, 0, 0, 0 }
    };


    g_running = true;
    m_state = 0;
    strset(device,"/dev/ttyS1\0");

    // Process the command line options
    while ((optchar = getopt_long(argc, argv, "vb:s:h?", \
            longopts, NULL)) != -1)
    {
        switch (optchar)
        {
        case 'v':
            printf("Verbose\n");
            g_verbose = true;
            break;
        case 'b':
            if(optarg==NULL) break;
            baud= atoi(optarg);
            printf("Got baud: %d\n",baud);
            break;
        case 's':
            memset(device,0,sizeof(device));
            strncpy(device,optarg,sizeof(device));
            printf("Serial '%s'\n",device);
            break;
        default:
            printf("test\n");
            exit(0);
            break;
        }
    };
    printf("Application starting (%d)\n",argc);

    signal(SIGINT, _main_sigint_handler);
    printf("Starting serial port %s at %d baud\n",device,baud);

    // Make sure the baud is correct
    switch(baud){
    case 9600:
       baud = B9600;
       break;
    case 38400:
       baud=B38400;
       break;
    case 57600:
       baud = B57600;
       break;
    case 115200:
       baud=B115200;
       break;
    default:
       printf("Invalid baud rate: %d. Baudrates are 9600,38400,57600,115200\n",baud);
       exit(0);
    }

    // Now open the serial port
    #ifdef ENABLE_SERIAL_PORT
    m_tty_fd = serial_open(device,baud);
    #else
    m_tty_fd = 0;
    #endif
    if(m_tty_fd<0)
    {
        printf( "Can't open serial port '%s'\n",device);
        exit(0);
    }
   
    serial_flush(m_tty_fd);

   #if 0
    while(g_running)
    {
        // do things
        printf("Data\n");
        sleep(1);
        if (read(m_tty_fd,&c,1)>0)        write(STDOUT_FILENO,&c,1);
        if (read(STDIN_FILENO,&c,1)>0)  write(m_tty_fd,&c,1);
    }
    g_running = false;  // If we exit the main loop we want to terminate
    #else
    _main_app(0);
    #endif
   
    serial_flush(m_tty_fd);
    serial_close(m_tty_fd);

    printf("Done\n");


    return 1;
}


///////////////////////////////////////////////////////////////
//
// Private functions
//
///////////////////////////////////////////////////////////////

void _main_app(int timeoutlimit_sec)
{
    fd_set read_fds;
    struct timeval timeout;
    bool running = true;
    int result;
    uint8_t buf[100];
    int timeoutcount =0;
    int max_fds;

    // The idea is to look at the different file descriptors and
    // wait until there is some data on one of them.  When there
    // is we check which one and process it.
    FD_ZERO(&read_fds);
    while(running && g_running)
    {
        // Reset variables for select and call select()
        timeout.tv_sec = 0;
        timeout.tv_usec = (US_PER_SECOND * 0.1);   // 100ms select
        max_fds = 0;
        // Add the input fd to the set
        if(m_tty_fd)
        {
            FD_SET(m_tty_fd, &read_fds);
            if(m_tty_fd>max_fds) max_fds = m_tty_fd;
        }
        // Add stdin
        FD_SET(0, &read_fds);
        // Find out if we need to read a fd
        result = select(max_fds+1, &read_fds, NULL, NULL, &timeout);
        if(result>0)
        {
            timeoutcount = 0;
            // Handle input on the serial port
            if(FD_ISSET(m_tty_fd,&read_fds))
            {
                int x;
                // Read in as much as we can
                x=read(m_tty_fd,&buf,sizeof(buf));
                if(x>0)
                {
                    buf[x]=0;
                    printf("Got %d bytes '%s'\n",x,buf);
                }
            }
            // Handle stdin
            if(FD_ISSET(0,&read_fds))
            {
                int x;
                x = read(STDIN_FILENO,&buf,sizeof(buf));
                if(x>0)
                {
                    // Remove newline
                    buf[x-1]=0;
                    printf("Sending: %s\n",buf);
                    write(m_tty_fd,buf,x-1);
                }
            }
        } // if result>0
        else if(result<0){
            printf("Error %d\n",result);
        }else{
            // result ==0
            //DBGOUT("Timeout %d\n",timeoutcount);
            // Normal
            timeoutcount++;
            if(timeoutlimit_sec>0 && (timeoutcount/10 >= timeoutlimit_sec))
            {
                //DBGOUT(LOG_DEBUG, "Timeout %d sec reached, exiting\n",timeoutlimit_sec);
                //running = false;
            }
        }

    } // end while(running)

    g_running = false;


    return;
}


void _main_sigint_handler(int sig)
{
    g_running = false;
    return;
}

int serial_open(char * device,int baud)
{
    struct termios tio;
    int tty_fd;

    memset(&tio,0,sizeof(tio));
    tio.c_iflag=0;
    tio.c_oflag=0;
    tio.c_cflag=CS8|CREAD|CLOCAL;           // 8n1, see termios.h for more information
    tio.c_lflag=0;
    tio.c_cc[VMIN]=1;
    tio.c_cc[VTIME]=5;

    tty_fd=open(device, O_RDWR | O_NONBLOCK);
    cfsetospeed(&tio,baud);
    cfsetispeed(&tio,baud);

    tcsetattr(tty_fd,TCSANOW,&tio);

    // Need?
    //tcsetattr(tty_fd,TCSANOW,&stdio);
    //tcsetattr(tty_fd,TCSAFLUSH,&stdio);
    fcntl(tty_fd, F_SETFL, O_NONBLOCK);       // make the reads non-blocking

    return tty_fd;
}

void serial_close(int tty_fd)
{
    if(tty_fd) close(tty_fd);

    return;
}

void serial_flush(int tty_fd)
{
    char c;
    if(tty_fd>0)
        while(read(tty_fd,&c,1)>0);
    return;
}

//eof
