#include <iostream>
#include <fstream>
#include <iomanip>
// #include "lib.h"

#include "QuantumDot.hpp"

using namespace std;
using namespace arma;

int main(int numArg, char *arguments[]){
  int dimension, numberofParticles, whichMethod;
  long int maxVariations, equilibrationTime, MCCs;
  double charge, step, alpha0, deltaAlpha, omega;
  string filename;

  dimension = 3;
  numberofParticles = 2;
  step = 1.0;
  alpha0 = 0.5;
  deltaAlpha = 0.05;
  omega = 1;


// Read in output file, abort if there are too few command-line arguments
if (numArg != 6){
  cout << "Incorrect number of arguments!" << endl; exit(1);
}

else {
  // Defining input arguments
  maxVariations = atol(arguments[1]);
  equilibrationTime = atol(arguments[2]);
  MCCs = atol(arguments[3]);
  charge = atol(arguments[4]);
  whichMethod = atoi(arguments[5]);
}

// Initializing the system, read in data
QuantumDot quantumDot(dimension, numberofParticles, charge, maxVariations,
  equilibrationTime, MCCs, step, alpha0, deltaAlpha, omega);

quantumDot.Initialize();

 // Do the mc sampling
quantumDot.MonteCarlo(whichMethod);

// Print out results
quantumDot.WriteToFile();


return 0;

}