#include <iostream>
#include <fstream>
#include <vector>

//#include "TH1F.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TH1.h"
#include "TCanvas.h"
#include "TMath.h"

#include "TGraph.h"
#include <cmath>

const double meanval=3.11538;
const double scaleparameter=234;
double poissonF(double mu, int k);

double prod(vector<int> zaehler, double mu){

  double LL=1; //likelihood init
  for ( unsigned int k = 0 ; k < zaehler . size () ; ++k) {
//cout<<"zaehler[k] "<<zaehler[k]<<endl;
    
    double p=poissonF(mu,zaehler[k]); //poisson probability single observation
    LL*=p; //likelihood is product of single-observation probabilities

//  cout<<"LL "<<LL<<endl;
	
  }
  
  return LL;
}

int likelihood() {
    using namespace std;

vector<int> zaehler;

    ifstream fin("datensumme.txt");
    int n_i;
    for(int i = 0 ; i < 234 ; ++i) {
        fin >> n_i;
	zaehler.push_back(n_i);
    }
    fin.close();


    double Likelihood=prod(zaehler,meanval);
    cout<<"Likelihood "<<Likelihood<<endl;

int NStep=600;
Double_t x[NStep], y[NStep];

vector<double> NLLX, NLLY;
vector<double> NLLYnorm;
double NormLL=-2*TMath::Log(prod(zaehler,3.11538));

int NLLGood=0;

    TH1F* LLScan=new TH1F("LLScan","LLScan",6,0,6);
    for(int i=0; i<NStep;i++){
      double step=i*0.01;
      Likelihood=prod(zaehler,step);
      LLScan->Fill(step,Likelihood);
      x[i]=step;
      y[i]=Likelihood;

double tmp=-2*TMath::Log(Likelihood);
if( !(std::isinf(tmp)|| std::isnan(tmp)) ){
NLLX.push_back(step);
NLLY.push_back(tmp);
NLLYnorm.push_back(tmp-NormLL);

NLLGood++;

}
}
TGraph* NLLfunc = new TGraph();
TGraph* NLLfuncnorm = new TGraph();

  for ( unsigned int k = 0 ; k < NLLX . size () ; ++k) {
   NLLfunc->SetPoint(k ,NLLX[k],NLLY[k]);
   NLLfuncnorm->SetPoint(k ,NLLX[k],NLLYnorm[k]);}

   TGraph* gr = new TGraph(NStep,x,y);
// TGraph* NLLfunc = new TGraph(NLLGood,NLLX,NLLY);


    TCanvas * c=new TCanvas("c", "c", 500,500);
//gr->Draw("AC*");
//   NLLfunc->Draw("AC*");
   NLLfuncnorm->Draw("AC*");

//LLScan->DrawCopy();   
    c->SaveAs("LLScan.pdf");

  double min_value = *std::min_element(NLLYnorm.begin(),NLLYnorm.end());
  auto it=find (NLLYnorm.begin(), NLLYnorm.end(), min_value);
int index=it-NLLYnorm.begin();
cout<<"index "<<index<<endl;
cout<<"minimal mu value "<<NLLX[index] <<endl;
cout<<"min_value "<<min_value <<endl;
cout<<"minimum mu "<<NLLX[*it] <<endl;

//below fetch first value from left for which NLL <1. and approch from right as well..

int leftbound, rightbound;
  for ( unsigned int k = 0 ; k < NLLYnorm . size () ; ++k) {
if(NLLYnorm[k]<1){leftbound=k; break;}
}



for ( unsigned int k = (NLLYnorm . size ())-1 ; k-- > 0 ;) {
 if(NLLYnorm[k]<1){rightbound=k; break;}}
cout<<"leftbound "<<leftbound <<" rightbound "<<rightbound <<endl;

double leftboundval, rightboundval;
leftboundval=NLLX[leftbound];
rightboundval=NLLX[rightbound];

cout<<"leftboundval "<<leftboundval <<" rightboundval "<<rightboundval <<endl;
    
    return 0;
}


double poissonF(double mu, int k) {

  return TMath::Poisson(k, mu); //root says it has issues when non-integer values.. don't expect but do by hand nonetheless..
}
