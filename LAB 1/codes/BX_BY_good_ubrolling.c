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


#define N 10    /*Frame dimension for QCIF format*/
#define M 10     /*Frame dimension for QCIF format*/

#define Bx 5      /*Block size*/
#define By 5      /*Block size*/

#define p 7       /*Search space. Restricted in a [-p,p] region around the
                    original location of the block.*/
//int B;   /*Block size*/

void phods_motion_estimation(const int current[N][M], const int previous[N][M],
        int vectors_x[N/Bx][M/By],int vectors_y[N/Bx][M/By])
{
    int x, y, i, j, k, l, p1, p2, q2, distx, disty, S, min1, min2, bestx, besty;



    /*For all blocks in the current frame*/
    for(x=0; x<N/Bx; x++)
    {
        for(y=0; y<M/By; y++)
        {
            /*Initialize the vector motion matrices*/
            vectors_x[x][y] = 0;
            vectors_y[x][y] = 0;
            S = 4;

            while(S > 0)
            {
                min1 = 255*Bx*By;
                min2 = 255*Bx*By;



                /*For all candidate blocks in X dimension*/
                for(i=-S; i<S+1; i+=S) // 3 loops for S =-4
                {
                    distx = 0;
                    disty = 0;

                    /*For all pixels in the block*/
                    for(k=0; k<Bx; k++)
                    {
                        for(l=0; l<By; l++)
                        {
                            p1 = current[Bx*x+k][By*y+l];

                            ///X calculations#######################################################
                            if((Bx*x + vectors_x[x][y] + i + k) < 0 ||
                                    (Bx*x + vectors_x[x][y] + i + k) > (N-1) ||
                                    (By*y + vectors_y[x][y] + l) < 0 ||
                                    (By*y + vectors_y[x][y] + l) > (M-1))

                            {
                                p2 = 0;
                            } else {
                                p2 = previous[Bx*x+vectors_x[x][y]+i+k][By*y+vectors_y[x][y]+l];
                            }


                            distx += abs(p1 - p2);
                            if (i !=0) {
                                ///Y calculations######################################################
                                if((Bx*x + vectors_x[x][y] + k) <0 ||
                                        (Bx*x + vectors_x[x][y] + k) > (N-1) ||
                                        (By*y + vectors_y[x][y] + i + l) < 0 ||
                                        (By*y + vectors_y[x][y] + i + l) > (M-1))
                                {
                                    q2 = 0;
                                } else {
                                    q2 = previous[Bx*x+vectors_x[x][y]+k][By*y+vectors_y[x][y]+i+l];
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
    float MO,MO2,time;
    float max_time = 0;
    float min_time = 10000;
    float sumtime =0;
    int B_array[4] = {1,2,5,10};
    float sum_cycyles=0;



//   for(int j =0; j<4;j++) {
//        B = B_array[j];
        int motion_vectors_x[N/Bx][M/By],
           motion_vectors_y[N/Bx][M/By], i, j;

    for( int a = 0; a<10;a++){
        /* Add timer code here */
        start_cycles = DWT->CYCCNT;

        phods_motion_estimation(current,previous,motion_vectors_x,motion_vectors_y);

        end_cycles = DWT->CYCCNT;
        execute_cycles = end_cycles - start_cycles;
        time = execute_cycles/f;
        sum_cycyles  +=  execute_cycles;
        sumtime +=time  ;
        if( time > max_time) max_time =time;
        if( time < min_time) min_time =time;

    }

     MO = sumtime/10 ;
     MO2 = (sum_cycyles)/(10*f);
     float diff = abs(MO-MO2);
  // }
    //  printf("M0: %d, max Time: %d, min Time: %d\n", MO, max_time, min_time);

    while(1){

    }



}
