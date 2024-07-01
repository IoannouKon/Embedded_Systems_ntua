/***********************************************************************************************************************
 * Copyright [2015-2021] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
 * 
 * This file is part of Renesas SynergyTM Software Package (SSP)
 *
 * The contents of this file (the "contents") are proprietary and confidential to Renesas Electronics Corporation
 * and/or its licensors ("Renesas") and subject to statutory and contractual protections.
 *
 * This file is subject to a Renesas SSP license agreement. Unless otherwise agreed in an SSP license agreement with
 * Renesas: 1) you may not use, copy, modify, distribute, display, or perform the contents; 2) you may not use any name
 * or mark of Renesas for advertising or publicity purposes or in connection with your use of the contents; 3) RENESAS
 * MAKES NO WARRANTY OR REPRESENTATIONS ABOUT THE SUITABILITY OF THE CONTENTS FOR ANY PURPOSE; THE CONTENTS ARE PROVIDED
 * "AS IS" WITHOUT ANY EXPRESS OR IMPLIED WARRANTY, INCLUDING THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE, AND NON-INFRINGEMENT; AND 4) RENESAS SHALL NOT BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, OR
 * CONSEQUENTIAL DAMAGES, INCLUDING DAMAGES RESULTING FROM LOSS OF USE, DATA, OR PROJECTS, WHETHER IN AN ACTION OF
 * CONTRACT OR TORT, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THE CONTENTS. Third-party contents
 * included in this file may be subject to different terms.
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * File Name    : hal_entry.c
 * Description  : This is a very simple example application that blinks all LEDs on a board.
 ***********************************************************************************************************************/

#include "hal_data.h"
#include "stdio.h"
#include "string.h"
#include "images.h"
#include  "math.h"

#define N 10    /*Frame dimension for QCIF format*/
#define M 10     /*Frame dimension for QCIF format*/

#define B 5      /*Block size*/

#define p 7       /*Search space. Restricted in a [-p,p] region around the
                    original location of the block.*/


void phods_motion_estimation(const int current[N][M], const int previous[N][M],
        int vectors_x[N/B][M/B],int vectors_y[N/B][M/B])
{
    int j,a,b, x, y, i, k, l, p1, p2, q2, distx, disty, S, min1, min2, bestx, besty;
    int temp_x,temp_y;
    int current_line[B][N];


    /*For all blocks in the current frame*/
    for(x=0; x<N/B; x++)
    {
        for(a=0;a<B;a++)  { /* Copy data from current to buffer current_line */
         for(b=0;b<N;b++) {
         current_line[a][b]=current[a +B*x][b];
         }
        }

        for(y=0; y<M/B; y++)
        {
            /*Initialize the vector motion matrices*/
            vectors_x[x][y] = 0;
            vectors_y[x][y] = 0;

           S = 4;

            while(S > 0)
            {
                min1 = 255*B*B;
                min2 = min1;

                temp_x = B*x + vectors_y[x][y] ;
                temp_y = B*y + vectors_y[x][y];

                /*For all candidate blocks in X dimension*/
                for(i=-S; i<S+1; i+=S) // 3 loops for S =-4
                {
                    distx = 0;
                    disty = 0;

                    /*For all pixels in the block*/
                    for(k=0; k<B; k++)
                    {
                        for(l=0; l<B; l++)
                        {
                             //p1 = current[B*x+k][B*y+l];
                             p1=current_line[k][B*y+l]; /* Read the data from buffer current_line */


                            ///X calculations#######################################################
                            if((temp_x + i + k) < 0 ||
                                    (temp_x + i + k) > (N-1) ||
                                    ( temp_y + l) < 0 ||
                                    ( temp_y + l) > (M-1))

                            {
                                p2 = 0;
                            } else {
                                p2 = previous[ temp_x+i+k][ temp_y+l];
                            }


                            distx += abs(p1 - p2);
                            if (i !=0) {
                                ///Y calculations######################################################
                                if(( temp_x + k) <0 ||
                                        ( temp_x + k) > (N-1) ||
                                        ( temp_y + i + l) < 0 ||
                                        ( temp_y + i + l) > (M-1))
                                {
                                    q2 = 0;
                                } else {
                                    q2 = previous[ temp_x+k][ temp_x+i+l];
                                }


                                disty += abs(p1 - q2);
                            }
                            else {
                                disty =distx;
                            }

                        }
                    }


                    if(distx < min1)
                    {
                        min1 = distx;
                        bestx = i;
                    }
                    if(disty < min2)
                    {
                        min2 = disty;
                        besty = i;
                    }


                }

                S = S/2;
                vectors_x[x][y] += bestx;
                vectors_y[x][y] += besty;
            }
        }
    }

}
/*******************************************************************************************************************//**
 * @brief  Blinky example application
 *
 * Blinks all leds at a rate of 1 second using the software delay function provided by the BSP.
 * Only references two other modules including the BSP, IOPORT.
 *
 **********************************************************************************************************************/
void hal_entry(void) {


    // Code to initialize the DWT->CYCCNT register
    CoreDebug->DEMCR |= 0x01000000; // This line sets the Trace Enable bit in the Debug Exception and Monitor Control Register (DEMCR) of the CoreDebug unit.
    ITM->LAR = 0xC5ACCE55;   //This line writes the unlock value 0xC5ACCE55 to the Lock Access Register (LAR) of the Instrumentation Trace Macrocell (ITM).Writing this specific value to LAR is required to unlock write access to certain registers in the ITM.
    DWT->CYCCNT = 0; //This line resets the cycle counter in the Data Watchpoint and Trace (DWT) unit
    DWT->CTRL |= 1;  //This line sets the ENABLE bit (bit 0) in the Control Register (CTRL) of the DWT.Enabling this bit activates the DWT unit, allowing it to start counting cycles.

    /* Initialize your variables here */
    float start_cycles ,end_cycles,execute_cycles;
    float f = 240000000; // Here a constant of frequency
    float MO,time;
    float max_time = 0;
    float min_time = 10000;
    float sumtime =0;

    int motion_vectors_x[N/B][M/B],
    motion_vectors_y[N/B][M/B], i, j;

    for( int a = 0; a<10;a++){
        /* Add timer code here */
        start_cycles = DWT->CYCCNT;

        phods_motion_estimation(current,previous,motion_vectors_x,motion_vectors_y);

        end_cycles = DWT->CYCCNT;
        execute_cycles = end_cycles - start_cycles;
        time = execute_cycles/f;
        sumtime +=time  ;
        if( time > max_time) max_time =time;
        if( time < min_time) min_time =time;

    }

    MO = sumtime/10 ;

    //  printf("M0: %d, max Time: %d, min Time: %d\n", MO, max_time, min_time);

    while(1){

    }



}
