
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

// TODO: Student have to implement all required functions here
// All functions is implement before main function
int fightOneByOne(int, int, int); // This function is only declared.
int acrossTheRiver(int[], int, int, double);
int serialFight(int**, int**, int, int, int, int);






int fightOneByOne(int HP1, int HP2, int tancong) {
    if (HP1 == 999) {
        if ((HP2 == 777) || (HP2 == 666)) {
            return 0;
        }
        else {
            HP2 = 0;
            return 1;
        }
    }
    else if ((HP2 == 777) || (HP2 == 666)) {
        HP1 = 0;
        return -1;
    }
    else {
        int muy;
        muy = round(2 * HP1 * HP2 / (HP1 + HP2) + 0.7);
        HP1 = HP1 - abs(HP2 - muy);
        HP2 = HP2 - abs(HP1 - muy);
        if (tancong == -1)//quan tong tan cong
        {
            HP1 = round(HP1 * 1.1 + 0.6);
            HP2 = round(HP2 * 1.3 + 0.6);
        }
        else if (tancong == 0) {
            HP1 = round(HP1 * 1.5 + 0.6);
            HP2 = round(HP2 * 1.5 + 0.6);
        }
        else if (tancong == 1) {
            HP1 *= 1.3;
        }

        if (HP1 < 0) {
            HP1 = 0;
        }
        if (HP2 < 0) {
            HP2 = 0;
        }

        if (HP1 > HP2)return 1;
        else if (HP1 < HP2)return -1;
        else return 0;
    } 
}
int acrossTheRiver(int arrHP[], int n, int trangthai, double a) {
    double c;
    bool isCaptain = true;
    if (trangthai == -1) {
        double tu = 1, mau = 1;
        for (int j = 0; j < n; j++) {
            tu += j * arrHP[j];
            mau += arrHP[j];
            if (mau / tu > 1)c = 1;
        }
        for (int i = n; i >= 0; i--) {

            if ((arrHP[i] != 777) || (arrHP[i] != 666)) {
                int tempHP = arrHP[i] * c;
                tempHP = round(arrHP[i] * (1 - a) - 0.6);
                if (tempHP < arrHP[i])arrHP[i] = tempHP;
            }
            else {
                if (isCaptain == true) {
                    isCaptain = false;
                }
                else {
                    int tempHP = arrHP[i] * c;
                    tempHP = round(arrHP[i] * (1 - a) - 0.6);
                    if (tempHP < arrHP[i])arrHP[i] = tempHP;
                }
            }
        }
    }
    else if (trangthai == 1) {
        for (int j = 1; j >=0; j--) {
            float c = 1 - (j + 1) / n * a;
            if (c > 1) {
                c = 1;
            }


            if (arrHP[j] != 999) {
                int tempHP = arrHP[j] * c;
                tempHP = round(arrHP[j] * (1 - a) - 0.6);
                if (tempHP < arrHP[j])
                    arrHP[j] = tempHP;
            }
            else {
                if (isCaptain == true) {
                    isCaptain = false;
                }
                else {
                    int tempHP = arrHP[j] * c;
                    tempHP = round(arrHP[j] * (1 - a) - 0.6);
                    if (tempHP < arrHP[j])
                        arrHP[j] = tempHP;
                }
            }
        }
        int tbdieuhoa = 0;;
        for (int j = 0; j < n; j++) {
            tbdieuhoa += (1 / arrHP[j]);
        }
        tbdieuhoa = n / tbdieuhoa;
        int count_pass = 0;
        for (int i = 0; i < n; i++) {
            if (arrHP[i] > tbdieuhoa) {
                count_pass++;
            }
        }
        if (count_pass > n / 2) {
            return 1;
        }
        else return 0;
    }
}
int serialFight(int** HP1, int** HP2, int row, int col, int trangthai, int a) {
    int surviveLTK = 0, survive_tong = 0;
    if (trangthai % 10 == 1) {//neu x=1
        //Nam quoc son ha
        for (int i = 0; i < row; i++) {
            for (int j = 0; j < col; j++) {
                if ((i != row) && (j != col)) {
                    HP1[i][j] *= 1.5;
                    HP2[i][j] /= 1.2;
                }
                //khong doi HP tuong o hang, cot cao nhat
            }
        }
    }
    if ((trangthai > -30) && (trangthai < -19)) {
        int row_Tong = 0;
        for (int i = 0; i < row; i++) {
            int j = 0;
            while (!acrossTheRiver(HP2[i], j, -1, a)) {
                j++;
                if (j > col) {
                    j = 0;
                    row_Tong++;
                }
            }
            int fight = fightOneByOne(HP1[i][j], HP2[row_Tong][j], -1);
            if (fight > -1)
                surviveLTK++;
            if (fight < 1)
                survive_tong++;
        }
    }
    else if ((trangthai > -20) && (trangthai < -9)) {
        //Tống đã vượt sông và chủ động tấn công
        for (int i = 0; i < row; i++) {
            for (int j = 0; j < col; j++) {
                int fight = fightOneByOne(HP1[i][j], HP2[i][j], -1);
                if (fight > -1)
                    surviveLTK++;
                if (fight < 1)
                    survive_tong++;
            }
        }  
    }
    else if ((trangthai > 9) & (trangthai < 20)) {
        //ta da vuot song danh tong
        for (int i = 0; i < row; i++) {
            for (int j = 0; j < col; j++) {
                int fight = fightOneByOne(HP1[i][j], HP2[i][j], 1);
                if (fight > -1)
                    surviveLTK++;
                if (fight < 1)
                    survive_tong++;
            }
        }
    }
    else if ((trangthai > 19) && (trangthai < 30)) {
        //ta phair vuot song danh tong
    }

    if (surviveLTK > survive_tong) {
        return 1;
    }
    else return -1;
}







// Student have to erase main function before submission.
int main(int argc, char** argv) {
    int n1 = 100;
    int n2 = 200;
    int n = -1;

    int result = fightOneByOne(n1, n2, n);
    printf("Fight result: %d\n", result);
    printf("HP1: %d\n", n1);
    printf("HP2: %d\n", n2);
}
