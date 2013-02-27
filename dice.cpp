#include <vector>
#include <iostream>
#include <iomanip>
#include <sstream>

using namespace std;

class Binomial {
    vector<size_t> dp;
    size_t max;

public:
    Binomial(size_t max)
        : dp(max*max)
        , max(max)
    {
        size_t idx = 0;
        for (size_t n = 0; n < max; ++n) {
            for (size_t k = 0; k < max; ++k) {
                if (k == 0)
                    dp[idx] = 1;
                else if (n == 0)
                    dp[idx] = 0;
                else
                    dp[idx] = dp[idx - max - 1] + dp[idx - max];

                ++idx;
            }
        }
    }

    size_t operator()(size_t n, size_t k) {
        return dp[n*max + k];
    }
};

class app {
    size_t n, m;
    vector<int> roll;
    Binomial binomial;

public:
    app(size_t n, size_t m)
        : n(n)
        , m(m)
        , roll(n+m, 1)
        , binomial(n+m+3)
    {
    }

    void go() {
        size_t outcomes = 0;
        size_t maxCount = n+m+3;
        vector<size_t> incidents(maxCount * 7);
        do {

            vector<int> common(7);
            vector<int> a(7);
            vector<int> b(7);

            for (size_t i = 0; i < n; ++i) {
                a[roll[i]]++;
            }

            for (size_t i = n; i < n+m; ++i) {
                b[roll[i]]++;
            }

            bool specialA = true;
            for (size_t i = 1; i <= n; ++i) {
                if (a[i] != 1) {
                    specialA = false;
                    break;
                }
            }

            bool specialB = true;
            for (size_t i = 1; i <= m; ++i) {
                if (b[i] != 1) {
                    specialB = false;
                    break;
                }
            }

            size_t extra = (specialA ? (n+1) : a[1])
                + (specialB ? (m+1) : b[1]);

            if (!specialA) {
                for (size_t i = 0; i < n; ++i) {
                    common[roll[i]]++;
                }
            }

            if (!specialB) {
                for (size_t i = n; i < n+m; ++i) {
                    common[roll[i]]++;
                }
            }

            size_t currentOutcomes = 1;
            size_t remainingA = n;
            size_t remainingB = m;
            for (size_t i = 1; i <= 6; ++i) {
                if (a[i] != 0) {
                    currentOutcomes *= binomial(remainingA, a[i]);
                    remainingA -= a[i];
                }
                if (b[i] != 0) {
                    currentOutcomes *= binomial(remainingB, b[i]);
                    remainingB -= b[i];
                }
            }

            //for (size_t i = 0; i < n+m; ++i) std::cout << roll[i] << ", ";
            //std::cout << currentOutcomes;
            //std::cout << '\n';
            //for (size_t i = 2; i <= 6; ++i) std::cout << common[i] + extra << ", ";
            //std::cout << '\n';
            //for (size_t i = 2; i <= 6; ++i) std::cout << a[i] << ", ";
            //std::cout << '\n';
            //for (size_t i = 2; i <= 6; ++i) std::cout << b[i] << ", ";
            //std::cout << '\n';

            for (size_t i = 2; i <= 6; ++i) {
                for (size_t j = 0; j <= common[i] + extra && j < maxCount; ++j) {
                    incidents[i*maxCount + j] += currentOutcomes;
                }
            }

            outcomes += currentOutcomes;

        } while (next_roll());

        std::cout << "Outcomes: " << outcomes << '\n';

        std::cout << "Dice:\t";
        for (size_t j = 1; j < maxCount; ++j)
            std::cout << j << '\t';
        std::cout << "\n";

        for (size_t i = 2; i <= 6; ++i) {
            std::cout << i << ":\t";
            for (size_t j = 1; j < maxCount; ++j) {
                std::cout << incidents[i*maxCount + j] << '\t';
            }
            std::cout << '\n';
        }

        std::cout << '\n';

        for (size_t i = 2; i <= 6; ++i) {
            std::cout << i << ":\t";
            for (size_t j = 1; j < maxCount; ++j) {
                std::cout << fixed << setprecision(1) << 100.0 * incidents[i*maxCount + j] / outcomes << "%\t";
            }
            std::cout << '\n';
        }
    }

private:
    bool next_lexicographically_increasing(vector<int>::iterator from,
                                           vector<int>::iterator to) {
        auto i = from;
        for (; i != to; ++i) {
            if (*i == 6) continue;
            ++*i;
            break;
        }
        int v = (i == to) ? 1 : *i;
        for (auto j = from; j != i; ++j) {
            *j = v;
        }
        return i != to;
    }

    bool next_roll() {
        if (!next_lexicographically_increasing(roll.begin(), roll.begin()+n)) {
            return next_lexicographically_increasing(roll.begin()+n, roll.end());
        } else {
            return true;
        }
    }
};

int main(int argc, char ** argv) {
    if (argc < 3) {
        std::cout << "Usage: " << argv[0] << " [player A dice] [player B dice]\n";
        return 0;
    }
    size_t n;
    size_t m;
    {
        std::stringstream ss(argv[1]);
        ss >> n;
    }
    {
        std::stringstream ss(argv[2]);
        ss >> m;
    }
    app a(n, m);
    a.go();
    return 0;
}
// vim: set sw=4 sts=4 et:
