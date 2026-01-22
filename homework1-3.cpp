#include <iostream>
#include <cmath>
#include <iomanip>
using namespace std;

int main() {
    double a, b;
    cin >> a;
    cin >> b;
    double low = 0.0, high = 10.0, mid = (low + high) / 2;
    double eps = 1e-7;
    int flag = (b >= 1) ? 1 : -1;
    while (high - low > eps) {
        double minus = (exp(mid) - a * mid - b) * flag;
        if (fabs(minus) < eps) break;
        if (minus > 0) high = mid;
        else low = mid;
        mid = (low + high) / 2;
    }
    cout << fixed << setprecision(6) << mid << endl;
    return 0;
}