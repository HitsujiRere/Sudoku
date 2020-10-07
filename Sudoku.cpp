
#include <bits/stdc++.h>

using namespace std;

/*****/

// 数独
class Sudoku
{
private:
    // サイズ
    int N;
    // サイズの平方根
    int NSqrt;

    // データ
    vector<vector<int>> G;

    // ランダム化するためのもの
    mt19937 mt;

public:
    Sudoku(int n)
        : N(n), NSqrt(sqrt(n))
    {
        G.resize(N, vector<int>(N));

        mt.seed((int)time(0));
    }

    int getN() const { return N; };
    int getNSqrt() const { return NSqrt; };
    const vector<vector<int>> &getG() const { return G; };

    // (x,y)の値を指定する
    void setG(int x, int y, int v)
    {
        if (0 <= x && x < N && 0 <= y && y < N && 0 <= v && v <= N)
        {
            G[y][x] = v;
        }
    }

    // 初期化
    void clear()
    {
        for (int y = 0; y < N; ++y)
            for (int x = 0; x < N; ++x)
            {
                G[y][x] = 0;
            }
    }

    // 解く
    bool solve(int x = 0, int y = 0)
    {
        if (x == N)
            return solve(0, y + 1);

        if (y == N)
        {
            return check();
        }

        if (G[y][x] == 0)
        {
            vector<bool> able = option(x, y);
            for (int i = 1; i <= N; ++i)
            {
                if (!able[i])
                    continue;

                G[y][x] = i;

                //if (!check(x, y))
                //    continue;

                bool isCorrect = solve(x + 1, y);

                if (isCorrect)
                    return true;
            }

            G[y][x] = 0;
        }
        else
        {
            return solve(x + 1, y);
        }

        return false;
    }

    // 成り立っているか調べる
    bool check() const
    {
        vector<int> cnt(N + 1);

        for (int cy = 0; cy < N; ++cy)
        {
            for (int cx = 0; cx < N; ++cx)
            {
                cnt[G[cy][cx]]++;
            }
            for (int i = 1; i <= N; ++i)
            {
                if (cnt[i] > 1)
                    return false;
            }
            for (auto &e : cnt)
                e = 0;
        }

        for (int cx = 0; cx < N; ++cx)
        {
            for (int cy = 0; cy < N; ++cy)
            {
                cnt[G[cy][cx]]++;
            }
            for (int i = 1; i <= N; ++i)
            {
                if (cnt[i] > 1)
                    return false;
            }
            for (auto &e : cnt)
                e = 0;
        }

        for (int sy = 0; sy < N / NSqrt; ++sy)
        {
            for (int sx = 0; sx < N / NSqrt; ++sx)
            {
                for (int cx = sx * NSqrt; cx < (sx + 1) * NSqrt; ++cx)
                {
                    for (int cy = sy * NSqrt; cy < (sy + 1) * NSqrt; ++cy)
                    {
                        cnt[G[cy][cx]]++;
                    }
                }
                for (int i = 1; i <= N; ++i)
                {
                    if (cnt[i] > 1)
                        return false;
                }
                for (auto &e : cnt)
                    e = 0;
            }
        }

        return true;
    }

    // (x,y)において成り立っているか調べる
    bool check(int x, int y) const
    {
        vector<int> cnt(N + 1);

        for (int cx = 0; cx < N; ++cx)
        {
            cnt[G[y][cx]]++;
        }
        for (int i = 1; i <= N; ++i)
        {
            if (cnt[i] > 1)
                return false;
        }
        for (auto &e : cnt)
            e = 0;

        for (int cy = 0; cy < N; ++cy)
        {
            cnt[G[cy][x]]++;
        }
        for (int i = 1; i <= N; ++i)
        {
            if (cnt[i] > 1)
                return false;
        }
        for (auto &e : cnt)
            e = 0;

        int sx = x / NSqrt;
        int sy = y / NSqrt;
        for (int cx = sx * NSqrt; cx < (sx + 1) * NSqrt; ++cx)
        {
            for (int cy = sy * NSqrt; cy < (sy + 1) * NSqrt; ++cy)
            {
                cnt[G[cy][cx]]++;
            }
        }
        for (int i = 1; i <= N; ++i)
        {
            if (cnt[i] > 1)
                return false;
        }

        return true;
    }

    // そのマスに入る可能性があるかどうか
    vector<bool> option(int x, int y, bool isSee = true) const
    {
        if (isSee && G[y][x] > 0)
        {
            vector<bool> op(N + 1, false);
            op[G[x][y]] = true;
            return op;
        }

        vector<bool> op(N + 1, true);
        op[0] = false;

        for (int cx = 0; cx < N; ++cx)
        {
            op[G[y][cx]] = false;
        }

        for (int cy = 0; cy < N; ++cy)
        {
            op[G[cy][x]] = false;
        }

        int sx = x / NSqrt;
        int sy = y / NSqrt;
        for (int cy = sy * NSqrt; cy < (sy + 1) * NSqrt; ++cy)
        {
            for (int cx = sx * NSqrt; cx < (sx + 1) * NSqrt; ++cx)
            {
                op[G[cy][cx]] = false;
            }
        }

        return op;
    }

    // ランダムに初期化する
    bool setG_random(int x = 0, int y = 0)
    {
        if (x == N)
            return setG_random(0, y + 1);

        if (y == N)
        {
            return check();
            //return true;
        }

        int rnd = mt() % N;
        auto able = option(x, y);

        for (int _i = 0; _i < N; ++_i)
        {
            int i = (rnd + _i) % N + 1;

            if (!able[i])
                continue;

            G[y][x] = i;

            //if (!check(x, y))
            //    continue;

            bool isCorrect = setG_random(x + 1, y);

            if (isCorrect)
                return true;
        }

        G[y][x] = 0;

        return false;
    }

    // 穴を掘る
    void drill()
    {
        for (int i = 0; i < N * N; ++i)
        {
            int x = mt() % N;
            int y = mt() % N;

            auto able = option(x, y, false);

            if (count(able.begin(), able.end(), true) == 0)
            {
                G[y][x] = 0;
                drill();
                return;
            }
        }
    }

    // 表示する
    void print() const
    {
        int w = to_string(N).size();

        cout << "+"
             << string((w + 1) * N + 2 * NSqrt - 1, '-')
             << "+\n";
        for (int y = 0; y < N; ++y)
        {
            cout << "| ";
            for (int x = 0; x < N; ++x)
            {
                if (G[y][x] == 0)
                {
                    cout << string(w + 1, ' ');
                }
                else
                {
                    cout << std::right
                         << std::setw(w)
                         << G[y][x]
                         << " ";
                }
                if (x % NSqrt == NSqrt - 1 && x != N - 1)
                    cout << "| ";
            }
            cout << "|";
            cout << "\n";

            if (y % NSqrt == NSqrt - 1 && y != N - 1)
            {
                cout << "| ";
                for (int x = 0; x < N; ++x)
                {
                    cout << string(w, '-')
                         << " ";
                    if (x % NSqrt == NSqrt - 1 && x != N - 1)
                        cout << "+ ";
                }
                cout << "|";
                cout << "\n";
            }
        }
        cout << "+"
             << string((w + 1) * N + 2 * NSqrt - 1, '-')
             << "+\n";

        cout << flush;
    }

    // そのマスに入る可能性のある数を表示する
    void printOption() const
    {
        int w = to_string(N).size();

        cout << "+"
             << string((w + 1) * N + 2 * NSqrt - 1, '-')
             << "+\n";
        for (int y = 0; y < N; ++y)
        {
            cout << "| ";
            for (int x = 0; x < N; ++x)
            {
                auto op = option(x, y);
                cout << std::right
                     << std::setw(w)
                     << count(op.begin(), op.end(), true)
                     << " ";
                if (x % NSqrt == NSqrt - 1 && x != N - 1)
                    cout << "| ";
            }
            cout << "|";
            cout << "\n";

            if (y % NSqrt == NSqrt - 1 && y != N - 1)
            {
                cout << "| ";
                for (int x = 0; x < N; ++x)
                {
                    cout << string(w, '-')
                         << " ";
                    if (x % NSqrt == NSqrt - 1 && x != N - 1)
                        cout << "+ ";
                }
                cout << "|";
                cout << "\n";
            }
        }
        cout << "+"
             << string((w + 1) * N + 2 * NSqrt - 1, '-')
             << "+\n";

        cout << flush;
    }
};

/*****/

int main()
{
    Sudoku sudoku(9);

    //*

    // ユーザ入力
    for (int y = 0; y < 9; ++y)
    {
        for (int x = 0; x < 9; ++x)
        {
            int v;
            cin >> v;
            sudoku.setG(x, y, v);
        }
    }

    cout << (sudoku.solve() ? "Correct" : "Failed") << endl;
    sudoku.print();

    /*/

    // ランダム化
    sudoku.setG_random();
    sudoku.print();

    sudoku.drill();
    sudoku.print();

    sudoku.solve();
    sudoku.print();

    //*/

    return 0;
}
