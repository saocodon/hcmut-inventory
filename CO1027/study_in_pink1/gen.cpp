#include <iostream>
#include <random>
using namespace std;

int main(int argc, char* argv[]) {
    //freopen("sa_tc_01_input", "w", stdout);
    random_device rd;
    mt19937 gen(rd());
    string arg = argv[1];
    if (arg == "bai1" || arg == "bai2" || arg == "bai3") {
        uniform_int_distribution<int> HP(-800, 800), EXP(-100, 700), M(-100, 3100), E(-10, 110);
        cout << HP(gen) << ' ' << HP(gen) << '\n';
        cout << EXP(gen) << ' ' << EXP(gen) << '\n';
        cout << M(gen) << ' ' << M(gen) << '\n';
        cout << E(gen) << ' ' << E(gen) << ' ' << E(gen);
    } else if (arg == "bai4") {
        string c = "!@#$%(0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
        uniform_int_distribution<int> C(0, c.length() - 1), L(1,30);
        int n = L(gen);
        for (int i = 1; i <= n; i++) cout << c[C(gen)];
        cout << '\n';
        int usernameLength = L(gen);
        int domainLength = L(gen);
        for (int i = 0; i < usernameLength; i++) {
            int ch = C(gen);
            while (c[ch] == '@') ch = C(gen);
            cout << c[ch];
        }
        cout << '@';
        for (int i = 0; i < domainLength; i++) {
            int ch = C(gen);
            while (c[ch] == '@') ch = C(gen);
            cout << c[ch];
        }
    } else if (arg == "bai5") {
        string c = "!@#$%(0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
        uniform_int_distribution<int> C(0, c.length() - 1), L(1,30), U(5, 30);
        int uniqnum = U(gen);
        uniform_int_distribution<int> N(uniqnum*1.5, uniqnum*3);
        int pwds_num = N(gen);
        uniform_int_distribution<int> INDEX(0, uniqnum - 1);
        cout << pwds_num << '\n';
        vector<string> pwds_arr;
        for (int i = 0; i < uniqnum; i++) {
            int len = L(gen);
            string pwd;
            for (int j = 0; j < len; j++) {
                int ch = C(gen);
                pwd += c[ch];
            }
            pwds_arr.push_back(pwd);
        }
        for (int i = 0; i < pwds_num; i++) {
            int ind = INDEX(gen);
            cout << pwds_arr[ind] << '\n'; // Separator
        }
        cout << endl;
    }
}
