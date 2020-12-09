#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <string>


double poisson(double mu, int k) {
    return std::pow(mu, k) * std::exp(-mu) / std::tgamma(k + 1);
}

<<<<<<< HEAD
// Calculate the mean of a std::vector<double>
=======
>>>>>>> cd3e8758b06c366684f9328b5e848fe6cb5c7377
double calcmean(std::vector<double> x)
{
  double result = 0.;

  for (auto xi: x)
  {
    result += xi;
  }

  return result / x.size();
}

<<<<<<< HEAD
// Calculate the variance of a std::vector<double> given its mean m
=======
>>>>>>> cd3e8758b06c366684f9328b5e848fe6cb5c7377
double calcvar(std::vector<double> x, 
  double m)
{
  double result = 0.;

  for (auto xi: x)
  {
    result += (xi - m) * (xi - m);
  }

  return result / x.size();
}

// Loads a file with one column of doubles and saves 
// the sample size, its mean, variance 
// and standard deviation
void quickAnalyze(std::string const& file, 
  size_t& N, double& mean, double& var, double& stddev)
{
  std::ifstream dt(file);
  std::vector<double> data;
  N = 0;
  mean = 0.;
  var = 0.;
  stddev = 0.;

  double tmp;
  while (!dt.eof())
  {
    dt >> tmp;
    if (dt.fail())
    {
      continue;
    }
    else
    {
      data.push_back(tmp);
    }
  }
  dt.close();

  N = data.size();
  mean = calcmean(data);
  var = calcvar(data, mean);
  stddev = std::sqrt(var);

  return;
}

double prob(std::vector<int> data, double mu)
{
  double result = 1.;

  for (auto d: data)
  {
    result *= poisson(mu, d);
  }

  return result;
}

// Returns the index of the minimum of 
// a vector of doubles x
size_t min(std::vector<double> const& x)
{
  double min = x[0];
  size_t i_min = 0;

  for (size_t i = 0; i < x.size(); i++)
  {
    if (x[i] < min)
    {
      min = x[i];
      i_min = i;
    }
  }

  return i_min;
}

// Finds the indices i_low and i_high for which a vector
// of doubles x is smaller than a threshold t
bool findValidRange(std::vector<double> const& x, double t, size_t& i_low, size_t& i_high)
{
  bool bFoundRange = false;
  i_low = 0;
  i_high = x.size();
  for (size_t i = 0; i < x.size(); i++)
  {
    if (bFoundRange == false && x[i] < t)
    {
      bFoundRange = true;
      i_low = i;
    }
    else if (bFoundRange == true && x[i] >= t)
    {
      i_high = i - 1;
      return bFoundRange;
    }
  }

  return bFoundRange;
}

int main() {
    size_t const N = 234;
    double const mean = 3.11538;

    std::vector<int> data;

    std::ifstream fin("datensumme.txt");
    int n_i;
    for(size_t i = 0 ; i < N ; ++i) {
        fin >> n_i;

        data.push_back(n_i);
    }
    fin.close();

    // 2 a)
    double const L_mean = prob(data, mean);
    std::cout 
      << "L(mu=3.11538) = " 
      << L_mean 
      << std::endl;

    // 2 b)
    std::ofstream likelihood("likelihood.txt");
    for (double my_mu = 0.; my_mu < 6.; my_mu += 0.01)
    {
      likelihood << my_mu << " " 
        << prob(data, my_mu)
        << std::endl;
    }
    likelihood.close();

    // 2 c) and d)
    std::ofstream nll("nll.txt");
    std::ofstream deltanll("deltanll.txt");
    double const nll_mean = -2 * std::log(L_mean);
    size_t N_nll = 0;
    for (double my_mu = 0.; my_mu < 6.; my_mu += 0.01)
    {
      double const my_nll = -2 * std::log(prob(data, my_mu));
      nll << my_mu << " " 
        << my_nll
        << std::endl;
      
      deltanll << my_mu << " " 
        << my_nll - nll_mean
        << std::endl;
      
      N_nll++;
    }
    nll.close();
    deltanll.close();

    // 2 d) -> Find the minimum of deltanll
    std::vector<double> deltanll_mu;
    std::vector<double> deltanll_dnll;
    std::ifstream deltanll_in("deltanll.txt");
    size_t errors = 0;
    std::string dump;
    for (size_t i = 0; i < N_nll; i++)
    {
      std::string my_mu_str, my_dnll_str;
      double my_mu, my_dnll;

      deltanll_in >> my_mu_str;
      deltanll_in >> my_dnll_str;

      if (deltanll_in.fail())
      {
        errors++;
        continue;
      }

      if (my_dnll_str == "inf")
      {
        errors++;
        continue;
      }

      try 
      {
        my_mu = std::stod(my_mu_str);
        my_dnll = std::stod(my_dnll_str);
      }
      catch(std::invalid_argument)
      {
        errors++;
        continue;
      }
      
      deltanll_mu.push_back(my_mu);
      deltanll_dnll.push_back(my_dnll);
    }
    deltanll_in.close();

    std::cout << "Skipped " << errors 
      << " lines from deltanll.txt" << std::endl;
    size_t const N_dnll = N_nll - errors;

    size_t const i_min = min(deltanll_dnll);
    size_t i_low, i_high;
    findValidRange(deltanll_dnll, 1.0, i_low, i_high);
    double const mu_hat = deltanll_mu[i_min];
    double const mu_plus = deltanll_mu[i_high] - mu_hat;
    double const mu_minus = mu_hat - deltanll_mu[i_low];

    std::cout << "mu_hat = " << mu_hat 
      << "\t+" << mu_plus 
      << "\n\t\t\t\t-" << mu_minus 
      << std::endl;
    
    // To compare this to the sample mean uncertainty,
    // get the mean, sigma etc of the sample
    size_t N_sample;
    double mean_sample, var_sample, stddev_sample;
    quickAnalyze("datensumme.txt", N_sample, 
      mean_sample, var_sample, stddev_sample);

    double delta_sample_mean = stddev_sample / std::sqrt(N_sample);
    std::cout << "delta_<x> = " << delta_sample_mean << std::endl;
}