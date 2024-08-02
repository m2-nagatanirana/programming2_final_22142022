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

    int N;                  // ����
    int maxtrycount;        // �ő厎�s��
    int trycount;           // ���݂̎��s��
    int Level;              // ���x��
    vector<int> answer;     // �����̐��l
    unordered_set<string> attemptedGuesses; // ���͂������l���L�^

    // ���l���S�Đ����ł��邩�𔻒肷��֐�
    bool Number(const string str) const {
        // �����񂪋�̏ꍇ�� false
        if (str.empty()) {
            return false;
        }
        // ������̂��ׂĂ̕����������ł��邩���m�F
        for (char c : str) {
            if (!isdigit(c)) {
                return false;
            }
        }
        return true;
    }

    // ���l���S�ĈقȂ鐔�����g���Ă��邩�𔻒肷��֐�
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


    // �V���b�t���͈̖͂�����ݒ肵�Ĕz����V���b�t��
    void shuffle(vector<int>& vec) {
        random_device rnd;
        mt19937 rondom(rnd());
        std::shuffle(vec.begin(), vec.end(), rondom);
    }

public:

    NumberGame(int digits, int level, int maxTries) : N(digits), Level(level), maxtrycount(maxTries), trycount(0) {

        if ((Level < 1 || Level>3) && (N < 2 || N > 6)) {
            throw invalid_argument("���x����1����3�C������2����6�͈̔͂Ŏw�肵�Ă��������D");
        }
        else if (N < 2 || N > 6) {
            throw invalid_argument("������2����6�͈̔͂Ŏw�肵�Ă��������D");
        }
        else if (Level < 1 || Level>3) {
            throw invalid_argument("���x����1����3�͈̔͂Ŏw�肵�Ă��������D");
        }

        answer.resize(10);                     //0����9�܂ł�10�̗v�f������vec
        iota(answer.begin(), answer.end(), 0); //0����9�܂ł̐����Ŗ��߂�
        shuffle(answer);                       //0����9�Ŗ��߂�ꂽvector�̗v�f���V���b�t��
        answer.resize(N);                      //N���Ƀg���~���O

    }

    void clear_last_char(int num_chars) {
        for (int i = 0; i < num_chars; ++i) {
            cout << '\b' << ' ' << '\b' << flush;
        }
    }

    void play() {

        cout << N << "���̐�������͂��Ă��������D" << endl;

        while (trycount < maxtrycount) {

            int hit = 0;
            int blow = 0;
            string number;
            vector<string> num;

            while (true) {

                cout << endl;
                cout << trycount + 1 << "��ڂ̉�" << "  ";

                cin >> number;


                if (number.length() != N) {
                    cout << "���l��" << N << "���ł͂���܂���D�ēx���͂��Ă��������D" << flush;
                }
                else if (!Number(number)) {
                    cout << "���͂��S�Đ����ł͂���܂���D�ēx���͂��Ă��������D" << flush;
                }
                else if (!Difference(number)) {
                    cout << "���͂��S�ĈقȂ鐔���ł͂���܂���D�ēx���͂��Ă��������D" << flush;
                }
                else if (attemptedGuesses.find(number) != attemptedGuesses.end()) {
                    cout << "���̐��l�͊��ɓ��͂���Ă��܂��D�ēx���͂��Ă��������D" << flush;
                }

                else {
                    attemptedGuesses.insert(number); // ���͂𗚗��ɒǉ�
                    break;
                }

                this_thread::sleep_for(chrono::seconds(3));// ��莞�ԑ҂�             
                cout << "\r" << string(100, ' ') << "\r" << flush;     // ���b�Z�[�W�������i�㏑�����ċ�ɂ���j 

            }

            vector<int> guess(N);
            transform(number.begin(), number.end(), guess.begin(), [](char c) { return c - '0'; });//���͂�����������x�N�g���ɕϊ�

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
                cout << trycount + 1 << "��ڂ�" << "gameclear�I" << endl;
                return;
            }

            cout << "Hit:" << hit << " Blow:" << blow << endl;
            trycount++;

        }

        cout << endl;
        cout << "gameover!" << "������ ";

        for (int i = 0; i < N; i++) {
            cout << answer[i] << " ";
        }
        cout << "�ł��D" << endl;
    }
};

int main() {
    try {
        cout << "/ Hit&Blow /" << endl;
        cout << "gamestart!!" << endl;
        cout << endl;

        int digits;
        cout << "���Ă��������̌�����2����6�őI�ѐ�����͂��Ă��������D" << endl;
        cout << "digit: ";
        cin >> digits;
        cout << endl;

        int maxTries;
        int level;
        cout << "���x����1����3�őI�ѐ�����͂��Ă��������D" << endl;
        cout << "1:�Ղ���(���s��:12��)�@2:����(���s��:9��)�@3:���(���s��:6��)" << endl;
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
        cerr << "�G���[:" << e.what() << endl;
    }
    return 0;
}


