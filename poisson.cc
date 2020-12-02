#include <iostream>
#include <fstream>
#include <vector>

//#include "TH1F.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TH1.h"
#include "TCanvas.h"
#include "TMath.h"

const double meanval=3.11538;
const double scaleparameter=234;
double poissonF(double mu, int k);
int poisson() {
    using namespace std;

vector<int> zaehler(11);
 
    ifstream fin("datensumme.txt");
    int n_i;
    for(int i = 0 ; i < 234 ; ++i) {
        fin >> n_i;
	zaehler[n_i]++;
    }
    fin.close();

    //why not put driectly in th1F..
TH1F* PoisHist=new TH1F("PoisHist","PoisHist",11,-0.5,10.5);
TH1F* PoisHistAlgo=new TH1F("PoisHistAlgo","PoisHistAlgo",11,-0.5,10.5);

    for ( unsigned int k = 0 ; k < zaehler . size () ; ++k) {
      std :: cout << k << ":" << zaehler [k] << std :: endl ;
      PoisHist->Fill(k,zaehler [k]);
      std :: cout << k << ":" << poissonF(meanval,k) << std :: endl ;
      PoisHistAlgo->Fill(k, scaleparameter*poissonF(meanval,k));
    }

   TCanvas * c=new TCanvas("c", "c", 500,500);
   cout<<"PoisHist mean"<<PoisHist->GetMean()<<endl;
   PoisHist->DrawCopy();
   PoisHistAlgo->SetMarkerColor(2);
      PoisHistAlgo->SetMarkerStyle(4);

   PoisHistAlgo->DrawCopy("same p");
   c->SetLogy();
   c->Update();
   c->SaveAs("poisson.pdf");

   return 0;
}

double poissonF(double mu, int k) {

  return TMath::Poisson(k,mu); //root says it has issues when non-integer values.. don't expect but do by hand nonetheless..
}
