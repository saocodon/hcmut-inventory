/*
* Ho Chi Minh City University of Technology
* Faculty of Computer Science and Engineering
* Initial code for Assignment 1
* Programming Fundamentals Spring 2023
* Author: Vu Van Tien
* Date: 02.02.2023
*/

//The library here is concretely set, students are not allowed to include any other libraries.

#include "study_in_pink1.h"

using namespace std;

void sa_tc_01() {
    cout << "----- Sample Testcase 01 -----" << endl;
    string input_file("sa_tc_01_input");
    int HP1, HP2, EXP1, EXP2, M1, M2, E1, E2, E3;
    if (!readFile(input_file, HP1, HP2, EXP1, EXP2, M1, M2, E1, E2, E3)) {
        return;
    }

    cout << "EXP1: " << EXP1 << ", EXP2: " << EXP2 << ", E1: " << E1 << endl;
    int result = firstMeet(EXP1, EXP2, E1);
    cout << "EXP1: " << EXP1 << ", EXP2: " << EXP2 << ", result: " << result << endl;
}


void sa_tc_02() {
    cout << "----- Sample Testcase 02 -----" << endl;
    string input_file("sa_tc_01_input");
    int HP1, HP2, EXP1, EXP2, M1, M2, E1, E2, E3;
    if (!readFile(input_file, HP1, HP2, EXP1, EXP2, M1, M2, E1, E2, E3)) {
        return;
    }

    cout << "EXP1: " << EXP1 << ", HP1: " << HP1 << ", M1: " << M1 << ", E2: " << E2 << endl;
    int result = traceLuggage(HP1, EXP1, M1, E2);
    cout << "EXP1: " << EXP1 << ", HP1: " << HP1 << ", M1: " << M1 << ", result: " << result << endl;
}

void sa_tc_03() {
    cout << "----- Sample Testcase 03 -----" << endl;
    string input_file("sa_tc_01_input");
    int HP1, HP2, EXP1, EXP2, M1, M2, E1, E2, E3;
    if (!readFile(input_file, HP1, HP2, EXP1, EXP2, M1, M2, E1, E2, E3)) {
        return;
    }

    cout << "EXP1: " << EXP1 << ", HP1: " << HP1 << ", EXP2: " << EXP2 << ", HP2: " << HP2 << ", E3: " << E3 << endl;
    int result = chaseTaxi(HP1, EXP1, HP2, EXP2, E3);
    cout << "EXP1: " << EXP1 << ", HP1: " << HP1 << ", EXP2: " << EXP2 << ", HP2: " << HP2 << ", result: " << result << endl;
}

void sa_tc_04() {
    freopen("sa_tc_01_input", "r", stdin);
    cout << "----- Sample Testcase 04 -----" << endl; 
    char *s, *email;
    s = (char*)malloc(100 * sizeof(char));
    email = (char*)malloc(100 * sizeof(char));
    cin.getline(s, 100);
    cin.getline(email, 100);
    cout << "s: " << s << ", email: " << email << '\n';
    int result = checkPassword(s, email);
    cout << "result: " << result;
    free(s); free(email);
}

void sa_tc_05() {
    freopen("sa_tc_01_input", "r", stdin);
    cout << "----- Sample Testcase 04 -----" << endl; 
    const int MAX_LENGTH = 100;
    char **arr_pwds; int n;
    cin >> n;
    arr_pwds = (char**)malloc(sizeof(int*) * n);
    for (int i = 0; i < n; i++) {
        arr_pwds[i] = (char*)malloc(sizeof(char) * MAX_LENGTH);
        cin >> arr_pwds[i];
    }
    int result = findCorrectPassword((const char**)arr_pwds, n);
    cout << "result: " << result;
    for (int i = 0; i < n; i++) {
        free(arr_pwds[i]);
    }
    free(arr_pwds);
}

int main(int argc, const char * argv[]) {
    string arg = argv[1];

    if (arg == "bai1") sa_tc_01();
    if (arg == "bai2") sa_tc_02();
    if (arg == "bai3") sa_tc_03();
    if (arg == "bai4") sa_tc_04();
    if (arg == "bai5") sa_tc_05();

    return 0;
}
