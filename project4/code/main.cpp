#include "IsingModel.hpp"


int main(int numArg, char *arguments[]) {

  int L, NumSamp, whichMatrix;
  double T;

  L = atoi(arguments[1]);
  T = atof(arguments[2]);
  //whichMatrix = atoi(arguments[3]);

  string filenameOrderedMatrix = "../output/OrderedOrientation.dat";
  string filenameRandomMatrix = "../output/RandomOrientation.dat";


  IsingModel isingModel;
  isingModel.BoltzFactor(T);

  // Caluclating observables for a random spin matrix
  // & writing to file for different number of Monte Carlo cycles
  for (NumSamp = 10; NumSamp < 10000; NumSamp += 100){
    isingModel.InitializeLattice(L, 2);
    isingModel.CalculateObservables();
    isingModel.MetropolisSampling(NumSamp);
    isingModel.WriteToFile(filenameRandomMatrix);
  }

  // Caluclating observables for an ordered spin matrix
  // & writing to file for different number of Monte Carlo cycles
  isingModel.ResetFile();
  for (NumSamp = 10; NumSamp < 10000; NumSamp += 100){
    isingModel.InitializeLattice(L, 1);
    isingModel.CalculateObservables();
    isingModel.MetropolisSampling(NumSamp);
    isingModel.WriteToFile(filenameOrderedMatrix);
  }

  return 0;
}
