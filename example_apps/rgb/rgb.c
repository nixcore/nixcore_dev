/**********************************************************************
*  (c) 2015 protological.com\CLI Systems LLC clisystems.com
* 
* Application to blink an RGB strip using a NixCore X1 SPI interface
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
***********************************************************************/

#include <stdio.h>
#include <signal.h>
#include "stdint.h"
#include <unistd.h>
#include "string.h"

#define DEVICE  "/dev/spidev1.0"
#define COLOR_OFF   0x8000
#define COLOR_WHITE 0xFFFF

uint8_t g_running;

// Given RGB values, make a 16bit value of 5 byte colors
// RGB values are 0x00-0x1F (0-31)
uint16_t getcolor(uint8_t r, uint8_t g, uint8_t b)
{
    int16_t tmp16=0;
    tmp16|=(g&0x1F);
    tmp16<<=5;
    tmp16|=(r&0x1F);
    tmp16<<=5;
    tmp16|=(b&0x1F);
    return tmp16;
}
void reset(FILE * fp)
{
    uint8_t c = 0,x;
     for(x=0;x<8;x++)
        fwrite(&c,1,1,fp);
    return;
}
void fill(int count,uint16_t color,FILE * fp)
{
    int x;
    uint8_t * data;
    data = (uint8_t*)&color;
    for(x=0;x<count;x++)
    fwrite(data,1,2,fp);
    return;
}

void _main_sigint_handler(int sig)
{
    g_running = 0;
    return;
}

int main(int argc,char** argv)
{
    FILE * fp;
    uint8_t outbuf[10];
    uint16_t * outbuf_16;
    int x;
    
	g_running = 1;
    
    outbuf_16 = (uint16_t*)outbuf;
    
    fp = fopen(DEVICE,"w");
    if(!fp)
    {
        printf("Failed to open device '%s'\n",DEVICE);
        return 1;
    }
 
 
    // Register the ctrl-c handler
    signal(SIGINT, _main_sigint_handler);
 
    #if 1
    while(g_running)
    {
        reset(fp);
        //fill(50,getcolor(31,31,31));
        fill(52,COLOR_WHITE,fp);
        fflush(fp);
        sleep(1);
        reset(fp);
        fill(52,COLOR_OFF,fp);
        fflush(fp);
        sleep(1);
    
    }
    #endif

    fclose(fp);
 
   return 0;
}
