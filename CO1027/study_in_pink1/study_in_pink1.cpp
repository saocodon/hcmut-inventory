#include "study_in_pink1.h"

bool readFile(
        const string & filename,
        int & HP1,
        int & HP2,
        int & EXP1,
        int & EXP2,
        int & M1,
        int & M2,
        int & E1,
        int & E2,
        int & E3
) {
    // This function is used to read the input file,
    // DO NOT MODIFY THIS FUNTION
    ifstream ifs(filename);
    if (ifs.is_open()) {
        ifs >> HP1 >> HP2
            >> EXP1 >> EXP2
            >> M1 >> M2
            >> E1 >> E2 >> E3;
        return true;
    }
    else {
        cerr << "The file is not found" << endl;
        return false;
    }
}

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
/// Complete the following functions
/// DO NOT modify any parameters in the functions.
////////////////////////////////////////////////////////////////////////

#define MAX_HP  666
#define MAX_EXP 600
#define MAX_M   3000
#define MAX_E   99

void mE(int& EXP) { EXP = max(0, EXP); EXP = min(MAX_EXP, EXP); }
void mH(int& HP) { HP = max(0, HP); HP = min(MAX_HP, HP); }
void mM(int& M) { M = max(0, M); M = min(MAX_M, M); }
int _ceil(double x) {
    if (abs(x - (int)x) < 1e-4)
        return x;
    return x + 1;
}

// Task 1
int firstMeet(int & exp1, int & exp2, int e1) {
    // TODO: Complete this function
    // limit
    if (e1 < 0 || e1 > MAX_E) return -99;
    mE(exp1); mE(exp2);
    // case 1
    if (e1 >= 0 && e1 <= 3) {
        switch (e1) {
            case 0: exp2 += 29;  mE(exp2); break; 
            case 1: exp2 += 45;  mE(exp2); break;
            case 2: exp2 += 75;  mE(exp2); break;
            case 3: exp2 += 149; mE(exp2); break;
        }
        int d = e1 * 3 + exp1 * 7;
        exp1 = ((d % 2 == 0) ? _ceil(exp1 + d / 200.0) : _ceil(exp1 - d / 100.0)); mE(exp1);
    }
    // case 2
    if (e1 >= 4 && e1 <= 99) {
        bool dd[3] = { 0, 0, 0 };
        if (e1 >= 4 && e1 <= 19) {
            exp2 = _ceil(exp2 + e1 / 4.0) + 19; mE(exp2);
            dd[0] = true;
        }    
        if (e1 >= 20 && e1 <= 49) {
            exp2 = _ceil(exp2 + e1 / 9.0) + 21; mE(exp2);
            dd[1] = true;
        } 
        if (e1 >= 50 && e1 <= 65) {
            exp2 = _ceil(exp2 + e1 / 16.0) + 17; mE(exp2);
            dd[2] = true;
        } 
        if (e1 >= 66 && e1 <= 79) {
            exp2 = _ceil(exp2 + e1 / 4.0) + 19; mE(exp2);
            dd[0] = true;
            if (exp2 > 200) {
                exp2 = _ceil(exp2 + e1 / 9.0) + 21; mE(exp2);
                dd[1] = true;
            }
        } 
        if (e1 >= 80 && e1 <= 99) {
            exp2 = _ceil(exp2 + e1 / 4.0 + 19) + _ceil(e1 / 9.0 + 21); mE(exp2);
            dd[0] = dd[1] = true;
            if (exp2 > 400) {
                exp2 = _ceil(exp2 + e1 / 16.0) + 17; mE(exp2);
                dd[2] = true;
            }
        }
        exp2 = (((dd[0] & dd[1] & dd[2]) == 1) ? _ceil(exp2 * 1.15) : exp2); mE(exp2);
        exp1 -= e1; mE(exp1);
    }
    return exp1 + exp2;
}

// Task 2
void SK1(int &HP1, int &M1) {
    if (HP1 < 200) {
        HP1 = _ceil(1.3 * HP1); mH(HP1);
        M1 -= 150; mM(M1);
    } else {
        HP1 = _ceil(1.1 * HP1); mH(HP1);
        M1 -= 70; mM(M1);
    }
}
void SK2(int &EXP1, int &M1) {
    if (EXP1 < 400) { M1 -= 200; mM(M1); }
    else { M1 -= 120; mM(M1); }
    EXP1 = _ceil(1.13 * EXP1); mE(EXP1);
}
void SK3(int &EXP1, int &M1) {
    if (EXP1 < 300) { M1 -= 100; mM(M1); }
    else { M1 -= 120; mM(M1); }
    EXP1 = _ceil(0.9 * EXP1); mE(EXP1);
}
int traceLuggage(int & HP1, int & EXP1, int & M1, int E2) {
    // TODO: Complete this function
    if (E2 < 0 || E2 > 99) return -99;
    mH(HP1); mE(EXP1); mM(M1);
    double P1, P2, P3;
    // Con duong 1
    int sqrtS = sqrt(EXP1), S;
    S = ((abs(sqrtS * sqrtS - EXP1) < abs((sqrtS + 1) * (sqrtS + 1) - EXP1)) ? sqrtS * sqrtS : (sqrtS + 1) * (sqrtS + 1));
    P1 = ((EXP1 >= S) ? 1 : (EXP1 * 1.0 / S + 80) / 123.0);
    // Con duong 2
    if (E2 % 2 == 1) {
        int _M = M1;
        if (M1 > 0) {
            while (1) {
                SK1(HP1, M1);
                if (2 * M1 < _M) break;
                SK2(EXP1, M1);
                if (2 * M1 < _M) break;
                SK3(EXP1, M1);
                if (2 * M1 < _M) break;
            }
        }
        HP1 = _ceil(HP1 * 0.83); mH(HP1);
        EXP1 = _ceil(1.17 * EXP1); mE(EXP1);
    } else {
        SK1(HP1, M1);
        if (M1 > 0) {
            SK2(EXP1, M1);
            if (M1 > 0) {
                SK3(EXP1, M1);
            }
        }
        HP1 = _ceil(0.83 * HP1); mH(HP1);
        EXP1 = _ceil(1.17 * EXP1); mE(EXP1);
    }
    sqrtS = sqrt(EXP1);
    S = ((abs(sqrtS * sqrtS - EXP1) < abs((sqrtS + 1) * (sqrtS + 1) - EXP1)) ? sqrtS * sqrtS : (sqrtS + 1) * (sqrtS + 1));
    P2 = ((EXP1 >= S) ? 1 : (EXP1 * 1.0 / S + 80) / 123.0);
    // Con duong 3
    const double P[] = { 0.32, 0.47, 0.28, 0.79, 1, 0.5, 0.22, 0.83, 0.64, 0.11 };
    P3 = ((E2 / 10 == 0) ? P[E2] : P[((E2 / 10) + (E2 % 10)) % 10]);
    // Hau tinh toan
    if (P1 == 1 && P2 == 1 && P3 == 1) {
        EXP1 = _ceil(0.75 * EXP1); mE(EXP1);
    } else {
        double avg = (P1 + P2 + P3) / 3;
        HP1 = ((avg < 0.5) ? _ceil(0.85 * HP1) : _ceil(0.9 * HP1)); mH(HP1);
        EXP1 = ((avg < 0.5) ? _ceil(1.15 * EXP1) : _ceil(1.2 * EXP1)); mE(EXP1);
    }
    return HP1 + EXP1 + M1;
}

// Task 3
int chaseTaxi(int & HP1, int & EXP1, int & HP2, int & EXP2, int E3) {
    // TODO: Complete this function
    #define maximize(i, j) sherlockPoints = max(sherlockPoints, taxiPoints[i][j]);
    if (E3 < 0 || E3 > 99) return -99;
    mH(HP1); mE(EXP1); mH(HP2); mE(EXP2);
    int taxiPoints[10][10], i = 0, j = 0;
    for (int r = 0; r < 10; r++) {
        for (int c = 0; c < 10; c++) {
            taxiPoints[r][c] = (E3 * c + r * 2) * (r - c);
            if (taxiPoints[r][c] > 2 * E3) i++;
            if (taxiPoints[r][c] < -E3) j++;
        }
    }
    if (i == 100) i = 1;
    if (j == 100) j = 1;
    while (i > 9) i = i / 10 + i % 10;
    while (j > 9) j = j / 10 + j % 10;
    int sherlockPoints = taxiPoints[i][j];
    for (int k = 1; k <= min(i, j); k++)         maximize(i - k, j - k);
    for (int k = 1; k <= min(9 - i, 9 - j); k++) maximize(i + k, j + k);
    for (int k = 1; k <= min(i, 9 - j); k++)     maximize(i - k, j + k);
    for (int k = 1; k <= min(9 - i, j); k++)     maximize(i + k, j - k);
    bool win = false;
    if (abs(taxiPoints[i][j]) <= sherlockPoints) {
        EXP1 = _ceil(1.12 * EXP1); mE(EXP1);
        HP1 = _ceil(1.1 * HP1); mH(HP1);
        EXP2 = _ceil(1.12 * EXP2); mE(EXP2);
        HP2 = _ceil(1.1 * HP2); mH(HP2);
        win = true;
    } else {
        EXP1 = _ceil(0.88 * EXP1); mE(EXP1);
        HP1 = _ceil(0.9 * HP1); mH(HP1);
        EXP2 = _ceil(0.88 * EXP2); mE(EXP2);
        HP2 = _ceil(0.9 * HP2); mH(HP2);
    }
    return (win ? sherlockPoints : taxiPoints[i][j]);
}

// Task 4
int checkPassword(const char * s, const char * email) {
    // TODO: Complete this function
	char* se = new char[strlen(email) + 1];
    for (int i = 0; i < strlen(email); i++) {
        if (email[i] == '@') {
            strncpy(se, email, i);
            break;
        }
    }
    int n = strlen(s);
    if (n < 8) return -1;
    if (n > 20) return -2;
    bool specialChar = false;
    int returnValue = -1;
    for (int i = 0; i < n; i++) {
        char* subString = new char[strlen(email) + 1];
        strncpy(subString, s + min(n - (int)strlen(se), i), strlen(se));
        if (strcmp(subString, se) == 0) return -(300 + i);

        if (s[i] == s[i + 1] && s[i] == s[i + 2]) return -(400 + i);

        if (s[i] == '@' || s[i] == '#' || s[i] == '%' || s[i] == '$' || s[i] == '!')
            specialChar = true;
        
        if (s[i] < 33 || s[i] == 34 || s[i] >= 38 && s[i] <= 47 || s[i] >= 58 && s[i] <= 63 || s[i] >= 91 && s[i] <= 96 || s[i] > 122) {
            if (returnValue == -1)
                returnValue = i;
        }
    }

    delete[] se;

    if (!specialChar) return -5;
    if (returnValue != -1) return returnValue;
    return -10;
}

// Task 5
int findCorrectPassword(const char * arr_pwds[], int num_pwds) {
    // TODO: Complete this function
    pwdItem pwdList[30];
    int ctr = 0, maxctr = 0, maxlen = 0, ans = 0;
    for (int i = 0; i < num_pwds; i++) {
        bool found = false;
        for (int j = 0; j < ctr; j++) {
            if (strcmp(pwdList[j].body, arr_pwds[i]) == 0) {
                found = true;
                pwdList[j].count++;
            }
        }
        if (!found) {
            pwdList[ctr] = { (char*)arr_pwds[i], 1, i };
            ctr++;
        }
    }
    for (int i = 0; i < ctr; i++) {
        if (pwdList[i].count > maxctr) {
            ans = pwdList[i].origin;
            maxctr = pwdList[i].count;
            maxlen = strlen(pwdList[i].body);
        }
        if (pwdList[i].count == maxctr && strlen(pwdList[i].body) > maxlen) {
            ans = pwdList[i].origin;
            maxlen = strlen(pwdList[i].body);
        }
        //cout << maxctr << ' ' << maxlen << '\n';
    }
    return ans;
}

////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////
