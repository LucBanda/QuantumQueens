//
//  main.c
//  quantumQueens
//
//  Created by Luc on 16/10/2016.
//
//


#include "quantum.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define FAST_RESULT 1 //fast result, do not use grover algorithm till the end, read directly the max probability after 1 passes (gain for 5 queens : 2m40 -> 2s40)

int queen0[3] = {0,1,2};
int queen1[3] = {3,4,5};
int queen2[3] = {6,7,8};
int queen3[3] = {9,10,11};
int queen4[3] = {12,13,14};
/*int queen5[3] = {15,16,17};
int queen6[3] = {18,19,20};
int queen7[3] = {21,22,23};
*/

int queenWidth = 15;

int quControlBit = 15;

int quRAM[32] = {16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47};

int totalWidth = 48;



void QueenCmp(int *qu0, int *qu1, int OutReg, quantum_reg *quReg)
{
    int reg0 = OutReg+1;
    int reg1 = OutReg+2;
    int reg2 = OutReg+3;
    int reg3 = OutReg+4;
    int reg4 = OutReg+5;
    int reg5 = OutReg+6;
    int reg6 = OutReg+7;
    int reg7 = OutReg+8;
    
    
    //2 first gates 1,2
    quantum_sigma_x(qu0[1], quReg);
    quantum_sigma_x(qu1[0], quReg);
    quantum_toffoli(qu0[1], qu1[0], reg0, quReg);
    quantum_sigma_x(qu1[0], quReg);
    quantum_toffoli(qu0[0], qu0[1], reg1, quReg);
    quantum_sigma_x(qu0[1], quReg);
    
    //next gate 3
    quantum_toffoli(qu0[0], qu0[1], reg2, quReg);
    
    //2 next gates 4,5
    quantum_sigma_x(qu0[0], quReg);
    quantum_toffoli(qu0[0], qu1[1], reg3, quReg);
    quantum_sigma_x(qu0[2], quReg);
    quantum_toffoli(qu0[0], qu0[2], reg4, quReg);
    quantum_sigma_x(qu0[2], quReg);
    quantum_sigma_x(qu0[0], quReg);
    
    //3 next gates 6,7,8
    quantum_toffoli(qu1[1], qu1[0], reg5, quReg);
    quantum_toffoli(qu1[0], qu0[2], reg6, quReg);
    quantum_toffoli(qu0[2], qu0[1], reg7, quReg);
    
    //start rewinding registers 9
    quantum_sigma_x(qu1[0], quReg);
    quantum_toffoli(qu1[0], reg7, OutReg, quReg);
    quantum_sigma_x(qu1[0], quReg);
    
    
    
    //next rewind 10
    quantum_sigma_x(qu0[2], quReg);
    quantum_toffoli(reg2, qu0[2], OutReg, quReg);
    quantum_sigma_x(qu0[2], quReg);
    
    
    //11
    quantum_sigma_x(qu1[1], quReg);
    quantum_sigma_x(reg4, quReg);
    quantum_toffoli(qu1[1], reg4, OutReg, quReg);
    
    
    //12
    quantum_sigma_x(reg6, quReg);
    quantum_toffoli(qu1[1], reg6, OutReg, quReg);
    quantum_sigma_x(qu1[1], quReg);
    
    
    //13
    quantum_sigma_x(qu1[2], quReg);
    quantum_sigma_x(reg5, quReg);
    quantum_toffoli(qu1[2], reg5, OutReg, quReg);
    quantum_sigma_x(qu1[2], quReg);
    
    
    //14
    quantum_sigma_x(reg1, quReg);
    quantum_toffoli(qu1[2], reg1, OutReg, quReg);
    
    
    
    //15
    quantum_sigma_x(reg0, quReg);
    quantum_sigma_x(qu1[2], quReg);
    quantum_toffoli(qu1[2], reg0, OutReg, quReg);
    quantum_sigma_x(qu1[2], quReg);
    
    //16
    quantum_sigma_x(reg3, quReg);
    quantum_toffoli(reg3, qu1[2], OutReg, quReg);
}

void InvQueenCmp(int *qu0, int *qu1, int OutReg, quantum_reg *quReg)
{
    int reg0 = OutReg+1;
    int reg1 = OutReg+2;
    int reg2 = OutReg+3;
    int reg3 = OutReg+4;
    int reg4 = OutReg+5;
    int reg5 = OutReg+6;
    int reg6 = OutReg+7;
    int reg7 = OutReg+8;
    
    //16
    quantum_toffoli(reg3, qu1[2], OutReg, quReg);
    quantum_sigma_x(reg3, quReg);
    
    //15
    quantum_sigma_x(qu1[2], quReg);
    quantum_toffoli(qu1[2], reg0, OutReg, quReg);
    quantum_sigma_x(qu1[2], quReg);
    quantum_sigma_x(reg0, quReg);
    
    //14
    quantum_toffoli(qu1[2], reg1, OutReg, quReg);
    quantum_sigma_x(reg1, quReg);
    
    
    
    //13
    quantum_sigma_x(qu1[2], quReg);
    quantum_toffoli(qu1[2], reg5, OutReg, quReg);
    quantum_sigma_x(reg5, quReg);
    quantum_sigma_x(qu1[2], quReg);
    
    
    //12
    quantum_sigma_x(qu1[1], quReg);
    quantum_toffoli(qu1[1], reg6, OutReg, quReg);
    quantum_sigma_x(reg6, quReg);
    
    
    //11
    quantum_toffoli(qu1[1], reg4, OutReg, quReg);
    quantum_sigma_x(reg4, quReg);
    quantum_sigma_x(qu1[1], quReg);
    
    
    
    //next rewind 10
    quantum_sigma_x(qu0[2], quReg);
    quantum_toffoli(reg2, qu0[2], OutReg, quReg);
    quantum_sigma_x(qu0[2], quReg);
    
    
    
    //start rewinding registers 9
    quantum_sigma_x(qu1[0], quReg);
    quantum_toffoli(qu1[0], reg7, OutReg, quReg);
    quantum_sigma_x(qu1[0], quReg);
    
    //3 next gates 6,7,8
    quantum_toffoli(qu0[2], qu0[1], reg7, quReg);
    quantum_toffoli(qu1[0], qu0[2], reg6, quReg);
    quantum_toffoli(qu1[1], qu1[0], reg5, quReg);
    
    //2 next gates 4,5
    quantum_sigma_x(qu0[0], quReg);
    quantum_sigma_x(qu0[2], quReg);
    quantum_toffoli(qu0[0], qu0[2], reg4, quReg);
    quantum_sigma_x(qu0[2], quReg);
    quantum_toffoli(qu0[0], qu1[1], reg3, quReg);
    quantum_sigma_x(qu0[0], quReg);
    
    //next gate 3
    quantum_toffoli(qu0[0], qu0[1], reg2, quReg);
    
    //2 first gates 1,2
    quantum_sigma_x(qu0[1], quReg);
    quantum_toffoli(qu0[0], qu0[1], reg1, quReg);
    quantum_sigma_x(qu1[0], quReg);
    quantum_toffoli(qu0[1], qu1[0], reg0, quReg);
    quantum_sigma_x(qu1[0], quReg);
    quantum_sigma_x(qu0[1], quReg);
    
}

void QueensAreNotInDiagonal(int *q0, int* q1, int OutReg, int n, quantum_reg *quReg)
{
    //max depth = 6 + 8 = 14
    int reg0 = OutReg+1;
    int reg1 = OutReg+2;
    int reg2 = OutReg+3;
    int reg4 = OutReg+5;
    int reg5 = OutReg+6;
    


    switch (n) {
        case 1:
            //(q0 != 7)
            quantum_toffoli(q0[0], q0[1], reg0, quReg);
            quantum_toffoli(q0[2], reg0, reg1, quReg);
            quantum_sigma_x(reg1, quReg);
            
            //add 1 to q0
            quantum_toffoli(q0[0], q0[1], q0[2], quReg);
            quantum_cnot(q0[0], q0[1], quReg);
            quantum_sigma_x(q0[0], quReg);
            break;
        case 2:
            //q0 < 6
                //quantum_print_qureg(*quReg);
            quantum_toffoli(q0[1], q0[2], reg1, quReg);
            quantum_sigma_x(reg1, quReg);
            
            //add 2 to q0
            quantum_toffoli(q0[0], q0[1], q0[2], quReg);
            quantum_cnot(q0[0], q0[1], quReg);
            quantum_sigma_x(q0[0], quReg);
            quantum_toffoli(q0[0], q0[1], q0[2], quReg);
            quantum_cnot(q0[0], q0[1], quReg);
            quantum_sigma_x(q0[0], quReg);
            break;
        default:
            break;
    }

    //(q0 != n) & !(q0 +n != q1)
    QueenCmp(q0, q1, reg5, quReg);
    quantum_sigma_x(reg5, quReg);
    quantum_toffoli(reg5, reg1, reg2, quReg); //symetrie
    quantum_sigma_x(reg2, quReg);
    
    //rewind
    quantum_sigma_x(reg5, quReg);
    InvQueenCmp(q0, q1, reg5, quReg);
    
    switch(n) {
        case 1:
            //add one to q0
            quantum_sigma_x(q0[0], quReg);
            quantum_cnot(q0[0], q0[1], quReg);
            quantum_toffoli(q0[0], q0[1], q0[2], quReg);
            
            //(q0 != 7)
            quantum_sigma_x(reg1, quReg);
            quantum_toffoli(q0[2], reg0, reg1, quReg);
            quantum_toffoli(q0[0], q0[1], reg0, quReg);
            break;
        case 2:
            //add 2 to q0
            quantum_sigma_x(q0[0], quReg);
            quantum_cnot(q0[0], q0[1], quReg);
            quantum_toffoli(q0[0], q0[1], q0[2], quReg);
            quantum_sigma_x(q0[0], quReg);
            quantum_cnot(q0[0], q0[1], quReg);
            quantum_toffoli(q0[0], q0[1], q0[2], quReg);
            
            //q0 < 6
            quantum_toffoli(q0[1], q0[2], reg1, quReg);
            quantum_sigma_x(reg1, quReg);
            break;
        default:
            break;
    }

    //end rewind
    
    switch (n) {
        case 1:
            //(q0 != 0)
            quantum_sigma_x(q0[0], quReg);
            quantum_sigma_x(q0[1], quReg);
            quantum_sigma_x(q0[2], quReg);
            quantum_toffoli(q0[0], q0[1], reg0, quReg);
            quantum_toffoli(q0[2], reg0, reg1, quReg);
            quantum_sigma_x(reg1, quReg);
            quantum_sigma_x(q0[2], quReg);
            quantum_sigma_x(q0[1], quReg);
            quantum_sigma_x(q0[0], quReg);
            
            //q0 = q0 - 1
            quantum_sigma_x(q0[0], quReg);
            quantum_cnot(q0[0], q0[1], quReg);
            quantum_toffoli(q0[0], q0[1], q0[2], quReg);
            break;
        case 2:
            //(q0 > 2)
            quantum_sigma_x(q0[1], quReg);
            quantum_sigma_x(q0[2], quReg);
            quantum_toffoli(q0[2], q0[1], reg1, quReg);
            quantum_sigma_x(reg1, quReg);
            quantum_sigma_x(q0[2], quReg);
            quantum_sigma_x(q0[1], quReg);
            
            //q0 = q0 - 2
            quantum_sigma_x(q0[0], quReg);
            quantum_cnot(q0[0], q0[1], quReg);
            quantum_toffoli(q0[0], q0[1], q0[2], quReg);
            quantum_sigma_x(q0[0], quReg);
            quantum_cnot(q0[0], q0[1], quReg);
            quantum_toffoli(q0[0], q0[1], q0[2], quReg);
            break;
            
    }

    
    //(q0 > n) & !(q0 -n != q1)
    QueenCmp(q0, q1, reg5, quReg);
    quantum_sigma_x(reg5, quReg);
    quantum_toffoli(reg1, reg5, reg4, quReg);
    quantum_sigma_x(reg5, quReg);
    quantum_sigma_x(reg4, quReg);
    
    //ni l'un ni l'autre
    quantum_toffoli(reg4, reg2, OutReg, quReg); // Main symetrie

    
    //main rewind
    //(q0 != 0) & (q0 -1 != q1)
    quantum_sigma_x(reg4, quReg);
    quantum_sigma_x(reg5, quReg);
    quantum_toffoli(reg1, reg5, reg4, quReg);
    quantum_sigma_x(reg5, quReg);
    InvQueenCmp(q0, q1, reg5, quReg);
    
    switch (n) {
        case 1:
            //q0 = q0 - 1
            quantum_toffoli(q0[0], q0[1], q0[2], quReg);
            quantum_cnot(q0[0], q0[1], quReg);
            quantum_sigma_x(q0[0], quReg);
            
            //(q0 != 0)
            quantum_sigma_x(q0[0], quReg);
            quantum_sigma_x(q0[1], quReg);
            quantum_sigma_x(q0[2], quReg);
            quantum_sigma_x(reg1, quReg);
            quantum_toffoli(q0[2], reg0, reg1, quReg);
            quantum_toffoli(q0[0], q0[1], reg0, quReg);
            quantum_sigma_x(q0[2], quReg);
            quantum_sigma_x(q0[1], quReg);
            quantum_sigma_x(q0[0], quReg);

            break;
        case 2:
            //q0 = q0 - 2
            quantum_toffoli(q0[0], q0[1], q0[2], quReg);
            quantum_cnot(q0[0], q0[1], quReg);
            quantum_sigma_x(q0[0], quReg);
            quantum_toffoli(q0[0], q0[1], q0[2], quReg);
            quantum_cnot(q0[0], q0[1], quReg);
            quantum_sigma_x(q0[0], quReg);
            
            //(q0 > 2)
            quantum_sigma_x(q0[1], quReg);
            quantum_sigma_x(q0[2], quReg);
            quantum_sigma_x(reg1, quReg);
            quantum_toffoli(q0[2], q0[1], reg1, quReg);
            quantum_sigma_x(q0[2], quReg);
            quantum_sigma_x(q0[1], quReg);
            break;
            
    }
    
    //end rewind
    
    switch (n) {
        case 1:
            //(q0 != 7)
            quantum_toffoli(q0[0], q0[1], reg0, quReg);
            quantum_toffoli(q0[2], reg0, reg1, quReg);
            quantum_sigma_x(reg1, quReg);
            
            //add 1 to q0
            quantum_toffoli(q0[0], q0[1], q0[2], quReg);
            quantum_cnot(q0[0], q0[1], quReg);
            quantum_sigma_x(q0[0], quReg);
            break;
        case 2:
            //q0 < 6
            quantum_sigma_x(reg1, quReg);
            quantum_toffoli(q0[1], q0[2], reg1, quReg);
            
            //add 2 to q0
            quantum_toffoli(q0[0], q0[1], q0[2], quReg);
            quantum_cnot(q0[0], q0[1], quReg);
            quantum_sigma_x(q0[0], quReg);
            quantum_toffoli(q0[0], q0[1], q0[2], quReg);
            quantum_cnot(q0[0], q0[1], quReg);
            quantum_sigma_x(q0[0], quReg);
            break;
        default:
            break;
    }
    
    //rewind
    //(q0 != 7) & (q0 +1 != q1)
    QueenCmp(q0, q1, reg5, quReg);
    quantum_sigma_x(reg5, quReg);
    quantum_sigma_x(reg2, quReg);
    quantum_toffoli(reg5, reg1, reg2, quReg); //symetrie
    quantum_sigma_x(reg5, quReg);
    InvQueenCmp(q0, q1, reg5, quReg);
    
    switch(n) {
        case 1:
            //add one to q0
            quantum_sigma_x(q0[0], quReg);
            quantum_cnot(q0[0], q0[1], quReg);
            quantum_toffoli(q0[0], q0[1], q0[2], quReg);
            
            //(q0 != 7)
            quantum_sigma_x(reg1, quReg);
            quantum_toffoli(q0[2], reg0, reg1, quReg);
            quantum_toffoli(q0[0], q0[1], reg0, quReg);
            break;
        case 2:
            //add 2 to q0
            quantum_sigma_x(q0[0], quReg);
            quantum_cnot(q0[0], q0[1], quReg);
            quantum_toffoli(q0[0], q0[1], q0[2], quReg);
            quantum_sigma_x(q0[0], quReg);
            quantum_cnot(q0[0], q0[1], quReg);
            quantum_toffoli(q0[0], q0[1], q0[2], quReg);
            
            //q0 < 6
            quantum_sigma_x(reg1, quReg);
            quantum_toffoli(q0[1], q0[2], reg1, quReg);

            break;
        default:
            break;
    }
}

void CheckDiagonals(int OutReg, quantum_reg *quReg)
{
    int reg0 = OutReg+1;
    int reg1 = OutReg+16;
    int reg2 = OutReg+17;
    int reg3 = OutReg+18;
    
    
    QueensAreNotInDiagonal(queen0, queen1, reg0, 1, quReg);
    quantum_cnot(reg0, reg1, quReg);
    //quantum_print_qureg(*quReg);
    QueensAreNotInDiagonal(queen0, queen1, reg0, 1, quReg);
    
    QueensAreNotInDiagonal(queen1, queen2, reg0, 1, quReg);
    quantum_toffoli(reg0, reg1, reg2, quReg);
    QueensAreNotInDiagonal(queen1, queen2, reg0, 1, quReg);
    
    QueensAreNotInDiagonal(queen2, queen3, reg0, 1, quReg);
    quantum_toffoli(reg0, reg2, reg3, quReg);
    QueensAreNotInDiagonal(queen2, queen3, reg0, 1, quReg);
    
    QueensAreNotInDiagonal(queen3, queen4, reg0, 1, quReg);
    quantum_toffoli(reg0, reg3, OutReg, quReg);             //MAIN symetrie
    QueensAreNotInDiagonal(queen3, queen4, reg0, 1, quReg);
    
    QueensAreNotInDiagonal(queen2, queen3, reg0, 1, quReg);
    quantum_toffoli(reg0, reg2, reg3, quReg);
    QueensAreNotInDiagonal(queen2, queen3, reg0, 1, quReg);
    
    QueensAreNotInDiagonal(queen1, queen2, reg0, 1, quReg);
    quantum_toffoli(reg0, reg1, reg2, quReg);
    QueensAreNotInDiagonal(queen1, queen2, reg0, 1, quReg);
    
    QueensAreNotInDiagonal(queen0, queen1, reg0, 1, quReg);
    quantum_cnot(reg0, reg1, quReg);
    QueensAreNotInDiagonal(queen0, queen1, reg0, 1, quReg);
    
}

void Check2ndDiagonal(int OutReg, quantum_reg *quReg)
{
    int reg0 = OutReg+1;
    int reg1 = OutReg+16;
    int reg2 = OutReg+17;
    
    
    QueensAreNotInDiagonal(queen0, queen2, reg0, 2, quReg);
    quantum_cnot(reg0, reg1, quReg);
    QueensAreNotInDiagonal(queen0, queen2, reg0, 2, quReg);
    
    QueensAreNotInDiagonal(queen1, queen3, reg0, 2, quReg);
    quantum_toffoli(reg0, reg1, reg2, quReg);
    QueensAreNotInDiagonal(queen1, queen3, reg0, 2, quReg);
    
    QueensAreNotInDiagonal(queen2, queen4, reg0, 2, quReg);
    quantum_toffoli(reg0, reg2, OutReg, quReg);                   //MAIN Symetrie
    QueensAreNotInDiagonal(queen2, queen4, reg0, 2, quReg);
    
    QueensAreNotInDiagonal(queen1, queen3, reg0, 2, quReg);
    quantum_toffoli(reg0, reg1, reg2, quReg);
    QueensAreNotInDiagonal(queen1, queen3, reg0, 2, quReg);
    
    QueensAreNotInDiagonal(queen0, queen2, reg0, 2, quReg);
    quantum_cnot(reg0, reg1, quReg);
    QueensAreNotInDiagonal(queen0, queen2, reg0, 2, quReg);
    
}

void CheckAllLinesForQueen(int *q, int OutReg, quantum_reg *quReg)
{
//max depth = nb queens + 11
    
    int *queens[5] = {queen0, queen1, queen2, queen3, queen4};
    
    int reg0 = OutReg + 1;
    int reg1 = OutReg + 11;
    int i;
    
    quantum_sigma_x(reg1, quReg);
    
    for (i = 0; i<5; i++) {
        reg1 = reg1-1;
        if (queens[i] != q) {
            reg1=reg1+1;
            QueenCmp(q, queens[i], reg0, quReg);
            quantum_toffoli(reg0, reg1+i, reg1+i+1, quReg); //symetrie
            //printf("i = %d : %d + %d<-%d\n", i, reg0, reg1+i, reg1+i+1);
            InvQueenCmp(q, queens[i], reg0, quReg);
        }
    }
    
    quantum_cnot(reg1+i, OutReg, quReg); //symetrie
    
    for (i = 5-1; i>=0; i--) {
        reg1 = reg1 + 1;
        if (queens[i] != q) {
            reg1 = reg1 - 1;
            QueenCmp(q, queens[i], reg0, quReg);
            quantum_toffoli(reg0, reg1+i, reg1+i+1, quReg); //symetrie
            //printf("i = %d : %d + %d<-%d\n", i, reg0, reg1+i, reg1+i+1);
            InvQueenCmp(q, queens[i], reg0, quReg);
        }
    }
    
    quantum_sigma_x(reg1, quReg);
}

void CheckLines(int OutReg, quantum_reg *quReg)
{
    //depth = nb_queens + 17
    int *queens[5] = {queen0, queen1, queen2, queen3, queen4};
    int i;
    int reg0 = OutReg+1;
    int reg1 = OutReg+17;
    
    quantum_sigma_x(reg1, quReg);
    
    for (i = 0; i<5; i++) {
        CheckAllLinesForQueen(queens[i], reg0, quReg);
        quantum_toffoli(reg0, reg1+i, reg1+i+1, quReg);
        CheckAllLinesForQueen(queens[i], reg0, quReg);
    }
    
    quantum_cnot(reg1+i, OutReg, quReg);
    
    
    for (i =5-1; i>=0; i--) {
        CheckAllLinesForQueen(queens[i], reg0, quReg);
        quantum_toffoli(reg0, reg1+i, reg1+i+1, quReg);
        CheckAllLinesForQueen(queens[i], reg0, quReg);
    }
    
    quantum_sigma_x(reg1, quReg);

}

void CheckLimit(int OutReg, quantum_reg *quReg) {
    int *queens[5] = {queen0, queen1, queen2, queen3, queen4};
    int i;
    int reg0 = OutReg+1;
    int reg1 = OutReg+2;
    int reg2 = OutReg+3;
    
    
    quantum_sigma_x(reg2, quReg);
        //quantum_print_qureg(*quReg);
    
    for (i = 0; i< 5; i++) {
        quantum_sigma_x(queens[i][1], quReg);
        quantum_sigma_x(queens[i][0], quReg);
        quantum_toffoli(queens[i][0], queens[i][2], reg0, quReg);
        quantum_toffoli(queens[i][1], reg0, reg1, quReg);
        quantum_sigma_x(queens[i][2], quReg);
        quantum_cnot(queens[i][2], reg1, quReg);
        quantum_toffoli(reg1, reg2+i, reg2+i+1, quReg);
        quantum_cnot(queens[i][2], reg1, quReg);
        quantum_sigma_x(queens[i][2], quReg);
        quantum_toffoli(queens[i][1], reg0, reg1, quReg);
        quantum_toffoli(queens[i][0], queens[i][2], reg0, quReg);
        quantum_sigma_x(queens[i][0], quReg);
        quantum_sigma_x(queens[i][1], quReg);
        
    }
    
    quantum_cnot(reg2+i, OutReg, quReg);
    
    //quantum_print_qureg(*quReg);
    
    for (i = 5-1; i>=0; i--) {
        quantum_sigma_x(queens[i][1], quReg);
        quantum_sigma_x(queens[i][0], quReg);
        quantum_toffoli(queens[i][0], queens[i][2], reg0, quReg);
        quantum_toffoli(queens[i][1], reg0, reg1, quReg);
        quantum_sigma_x(queens[i][2], quReg);
        quantum_cnot(queens[i][2], reg1, quReg);
        quantum_toffoli(reg1, reg2+i, reg2+i+1, quReg);
        quantum_cnot(queens[i][2], reg1, quReg);
        quantum_sigma_x(queens[i][2], quReg);
        quantum_toffoli(queens[i][1], reg0, reg1, quReg);
        quantum_toffoli(queens[i][0], queens[i][2], reg0, quReg);
        quantum_sigma_x(queens[i][0], quReg);
        quantum_sigma_x(queens[i][1], quReg);
        
    }
    
    quantum_sigma_x(reg2, quReg);
    
    //quantum_print_qureg(*quReg);
}

void Oracle(quantum_reg *quReg)
{
  
    
    //quantum_print_qureg(*quReg);
    
    CheckLines(quRAM[0], quReg);
    quantum_cnot(quRAM[0], quRAM[29], quReg);
    CheckLines(quRAM[0], quReg);
    
    CheckDiagonals(quRAM[0], quReg);
    quantum_toffoli(quRAM[29], quRAM[0], quRAM[30], quReg);
    CheckDiagonals(quRAM[0], quReg);
    
    Check2ndDiagonal(quRAM[0], quReg);
    quantum_toffoli(quRAM[0], quRAM[30], quRAM[31], quReg);
    Check2ndDiagonal(quRAM[0], quReg);
    
    CheckLimit(quRAM[0], quReg);
    quantum_toffoli(quRAM[0], quRAM[31], quControlBit, quReg);
    CheckLimit(quRAM[0], quReg);
    
    Check2ndDiagonal(quRAM[0], quReg);
    quantum_toffoli(quRAM[0], quRAM[30], quRAM[31], quReg);
    Check2ndDiagonal(quRAM[0], quReg);
    
    CheckDiagonals(quRAM[0], quReg);
    quantum_toffoli(quRAM[29], quRAM[0], quRAM[30], quReg);
    CheckDiagonals(quRAM[0], quReg);

    
    CheckLines(quRAM[0], quReg);
    quantum_cnot(quRAM[0], quRAM[29], quReg);
    CheckLines(quRAM[0], quReg);
    
            //quantum_print_qureg(*quReg);
    
    
}

void Inversion(quantum_reg *quReg)
{
    int i = 0;
    
    for (int i = 0; i< queenWidth; i++)
        quantum_hadamard(i, quReg);
    for (int i = 0; i< queenWidth; i++)
        quantum_sigma_x(i, quReg);
    
    quantum_sigma_x(quRAM[0], quReg);
    
    for (i = 0; i< queenWidth; i++) {
        quantum_toffoli(queen0[0] + i, quRAM[i], quRAM[i+1], quReg);
    }

    quantum_cnot(quRAM[i], quControlBit, quReg);
    
    for (i = queenWidth - 1; i >=0; i--) {
        quantum_toffoli(queen0[0] + i, quRAM[i], quRAM[i+1], quReg);
    }
    
    quantum_sigma_x(quRAM[0], quReg);
    
    for (int i = 0; i< queenWidth; i++)
        quantum_sigma_x(i, quReg);
    for (int i = 0; i< queenWidth; i++)
        quantum_hadamard(i, quReg);
    
}

void printQueensFromState(unsigned long long state) {
    
    
    int Queens[5] = {state % 8, (state >> 3) % 8, (state >> 6) % 8, (state >> 9) % 8, (state >> 12) % 8};
    
    //printf("D0 : %d\nD1 : %d\nD2 : %d\nD3 : %d\nD4 : %d\n", Queens[0], Queens[1], Queens[2], Queens[3], Queens[4]);
    
    for (int i = 0; i< 5; i++) {
        printf("\n|");
        for (int j = 0; j<5; j++) {
            if (Queens[j] == 4-i) {
                printf("x|");
            }
            else {
                printf(" |");
            }
        }
    }
    printf("\n");
}
unsigned long long quantum_max_proba_state (quantum_reg quReg) {
    
    double maxProba = 0.0;
    int maxProbaIndex=0;
    for (int index=0; index < quReg.size; index++) {
        double proba = 0.0;
        proba = quantum_prob(quReg.amplitude[index])*2.0;
        if (proba > maxProba) {
            maxProbaIndex = index;
            maxProba = proba;
        }
    }
    printf("cheating DeltaP = %.15f\n", maxProba - quantum_prob(quReg.amplitude[0]*2.0));
    
    return quReg.state[maxProbaIndex];

}


int main(int argc, const char * argv[])
{
    quantum_reg quReg;
    
    
    srand((unsigned int)clock()*100000);

    quReg = quantum_new_qureg(0, queenWidth);
    quantum_addscratch(totalWidth-queenWidth, &quReg);
    
    //superpose queens states
    for (int i = 0; i< queenWidth; i++)
        quantum_hadamard(i, &quReg);
    
    //initialize control bit
    quantum_sigma_x(quControlBit, &quReg);
    quantum_hadamard(quControlBit, &quReg);
#ifdef FAST_RESULT
    int order = 1;
#else
    int order = 43;
#endif
    for (int i = 0; i< order; i++) {
        Oracle(&quReg);
        Inversion(&quReg);
    }

    quantum_sigma_z(quControlBit, &quReg);
    quantum_hadamard(quControlBit, &quReg);
    
    unsigned long long returnVal = 0;
    
#ifdef FAST_RESULT
    returnVal = quantum_max_proba_state(quReg);

#else
    returnVal = quantum_measure(quReg);
#endif
    
    quantum_delete_qureg(&quReg);
    
    
    printQueensFromState(returnVal);
    
    return 0;
}

