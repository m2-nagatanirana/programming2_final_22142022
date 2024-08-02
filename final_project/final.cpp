#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <string>
#include <set>
#include <algorithm>
#include <cctype>
#include <numeric>
#include <unordered_set>
#include <thread>

using namespace std;

class NumberGame {

private:

    int N;                  // 桁数
    int maxtrycount;        // 最大試行回数
    int trycount;           // 現在の試行回数
    int Level;              // レベル
    vector<int> answer;     // 正解の数値
    unordered_set<string> attemptedGuesses; // 入力した数値を記録

    // 数値が全て数字であるかを判定する関数
    bool Number(const string str) const {
        // 文字列が空の場合は false
        if (str.empty()) {
            return false;
        }
        // 文字列のすべての文字が数字であるかを確認
        for (char c : str) {
            if (!isdigit(c)) {
                return false;
            }
        }
        return true;
    }

    // 数値が全て異なる数字を使っているかを判定する関数
    bool Difference(const string str) const {
        set<char> digits;
        for (char c : str) {
            if (digits.find(c) != digits.end()) {
                return false;
            }
            digits.insert(c);
        }
        return true;
    }


    // シャッフル範囲の末尾を設定して配列をシャッフル
    void shuffle(vector<int>& vec) {
        random_device rnd;
        mt19937 rondom(rnd());
        std::shuffle(vec.begin(), vec.end(), rondom);
    }

public:

    NumberGame(int digits, int level, int maxTries) : N(digits), Level(level), maxtrycount(maxTries), trycount(0) {

        if ((Level < 1 || Level>3) && (N < 2 || N > 6)) {
            throw invalid_argument("レベルを1から3，桁数を2から6の範囲で指定してください．");
        }
        else if (N < 2 || N > 6) {
            throw invalid_argument("桁数を2から6の範囲で指定してください．");
        }
        else if (Level < 1 || Level>3) {
            throw invalid_argument("レベルを1から3の範囲で指定してください．");
        }

        answer.resize(10);                     //0から9までの10個の要素を持つvec
        iota(answer.begin(), answer.end(), 0); //0から9までの整数で埋める
        shuffle(answer);                       //0から9で埋められたvectorの要素をシャッフル
        answer.resize(N);                      //N桁にトリミング

    }

    void clear_last_char(int num_chars) {
        for (int i = 0; i < num_chars; ++i) {
            cout << '\b' << ' ' << '\b' << flush;
        }
    }

    void play() {

        cout << N << "桁の整数を入力してください．" << endl;

        while (trycount < maxtrycount) {

            int hit = 0;
            int blow = 0;
            string number;
            vector<string> num;

            while (true) {

                cout << endl;
                cout << trycount + 1 << "回目の解答" << "  ";

                cin >> number;


                if (number.length() != N) {
                    cout << "数値が" << N << "桁ではありません．再度入力してください．" << flush;
                }
                else if (!Number(number)) {
                    cout << "入力が全て数字ではありません．再度入力してください．" << flush;
                }
                else if (!Difference(number)) {
                    cout << "入力が全て異なる数字ではありません．再度入力してください．" << flush;
                }
                else if (attemptedGuesses.find(number) != attemptedGuesses.end()) {
                    cout << "この数値は既に入力されています．再度入力してください．" << flush;
                }

                else {
                    attemptedGuesses.insert(number); // 入力を履歴に追加
                    break;
                }

                this_thread::sleep_for(chrono::seconds(3));// 一定時間待つ             
                cout << "\r" << string(100, ' ') << "\r" << flush;     // メッセージを消去（上書きして空にする） 

            }

            vector<int> guess(N);
            transform(number.begin(), number.end(), guess.begin(), [](char c) { return c - '0'; });//入力した文字列をベクトルに変換

            for (int s = 0; s < N; ++s) {
                if (answer[s] == guess[s]) {
                    hit++;
                }
                else if (find(answer.begin(), answer.end(), guess[s]) != answer.end()) {
                    blow++;
                }
            }

            if (hit == N) {
                cout << "Hit:" << hit << " Blow:" << blow << endl;
                cout << endl;
                cout << trycount + 1 << "回目で" << "gameclear！" << endl;
                return;
            }

            cout << "Hit:" << hit << " Blow:" << blow << endl;
            trycount++;

        }

        cout << endl;
        cout << "gameover!" << "正解は ";

        for (int i = 0; i < N; i++) {
            cout << answer[i] << " ";
        }
        cout << "です．" << endl;
    }
};

int main() {
    try {
        cout << "/ Hit&Blow /" << endl;
        cout << "gamestart!!" << endl;
        cout << endl;

        int digits;
        cout << "当てたい数字の桁数を2から6で選び数を入力してください．" << endl;
        cout << "digit: ";
        cin >> digits;
        cout << endl;

        int maxTries;
        int level;
        cout << "レベルを1から3で選び数を入力してください．" << endl;
        cout << "1:易しい(試行回数:12回)　2:普通(試行回数:9回)　3:難しい(試行回数:6回)" << endl;
        cout << "level:";
        cin >> level;
        cout << endl;

        if (level == 1) {
            maxTries = 12;
        }
        else if (level == 2) {
            maxTries = 9;
        }
        else {
            maxTries = 6;
        }

        NumberGame game(digits, level, maxTries);
        game.play();
    }
    catch (const invalid_argument& e) {
        cerr << "エラー:" << e.what() << endl;
    }
    return 0;
}


