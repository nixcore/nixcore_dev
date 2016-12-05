/**********************************************************************
*  (c) 2015 protological.com\CLI Systems LLC clisystems.com
* 
* Application to control an RGB strip using a NixCore X1 SPI interface
* 
* Usage: ./rgb R G B DEVICE
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
#include "stdlib.h"

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
    color = 0x8000|color;
    data = (uint8_t*)&color;
    #if 1
    x = data[0];
    data[0]=data[1];
    data[1]=x;
    #endif
    for(x=0;x<count;x++)
    fwrite(data,1,2,fp);
    return;
}


int main(int argc,char** argv)
{
    FILE * fp;
    int r,g,b,num;
    char * device;
    uint16_t color;
    
    if(argc!= 6){
        return 1;
    }
    
    r = atoi(argv[1])>>3;
    g = atoi(argv[2])>>3;
    b = atoi(argv[3])>>3;
    device = argv[4];
    num = atoi(argv[5]);
    color = getcolor(r,g,b);
    printf("Red: %d\n",r);
    printf("Green: %d\n",g);
    printf("Blue: %d\n",b);
    printf("device: '%s'\n",device);
    printf("num: %d\n",num);
    printf("color: x%04X\n",color);

    fp = fopen(device,"w");
    if(!fp)
    {
        printf("Failed to open device '%s'\n",device);
        return 1;
    }
    reset(fp);
    fill(num,color,fp);
    
    fclose(fp);
 
   return 0;
}
