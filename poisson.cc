#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>


double poisson(double mu, int k) {
    return std::pow(mu, k) * std::exp(-mu) / std::tgamma(k + 1);
}

int main() {
    std::vector<int> zaehler(11);
    size_t const N = 234;
    double const mean = 3.11538;

    std::ifstream fin("datensumme.txt");
    int n_i;
    for(size_t i = 0 ; i < N ; ++i) {
        fin >> n_i;

        // 1a)
        zaehler[n_i]++;
    }
    fin.close();

    std::ofstream hist("hist.txt");
    std::ofstream histpoi("histpoi.txt");
    for (size_t i = 0; i < zaehler.size(); i++)
    {
      // a)
      std::cout << i << ": " << zaehler[i] << std::endl;
      // b)
      hist << i << " " << zaehler[i] << std::endl;
      // c)

      histpoi << i << " " 
        << zaehler[i] << " "
        << N * poisson(i, mean) 
        << std::endl;
    }
    hist.close();
}