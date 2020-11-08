#include "mainfunc.hpp"
SolarSystem solarSystem;


//Function returning power of 1/r for the gravitational force
void MainFunc::initializeBeta(int choice){
  if (choice == 1){
    beta_vec.push_back(3.0);
  }

  else{
    int idx = 0;
    for (int b=30; b<41; b+=2){
      beta_vec.push_back(b/10.0);
      idx += 1;
    }
  }
}


// Developes our solar system if the gravitational force is proportional to 1/r^2
void MainFunc::integration1(int method, int numTimesteps, double dt, double beta){
  if (method == 1){
    writeToFile_Position("../output/euler_positions.xyz", 0);
    writeToFile_Energy("../output/euler_energies.dat", 0);
    writeToFile_AngMom("../output/euler_angmom.dat", 0);

    Euler integrator(dt);
    for (int timestep=0; timestep<numTimesteps; timestep++){
      double t = timestep*dt;
      solarSystem.calculateAngMomentum();
      writeToFile_Position("../output/euler_positions.xyz", t);
      writeToFile_Energy("../output/euler_energies.dat", t);
      writeToFile_AngMom("../output/euler_angmom.dat", t);
      integrator.integrateOneStep(solarSystem, beta);
    }
  }

  else if (method == 2){
    writeToFile_Position("../output/verlet_positions.xyz", 0);
    writeToFile_Energy("../output/verlet_energies.dat", 0);
    writeToFile_AngMom("../output/verlet_angmom.dat", 0);

    VelocityVerlet integrator(dt);
    for(int timestep=0; timestep<numTimesteps; timestep++) {
      double t = timestep*dt;
      solarSystem.calculateAngMomentum();
      integrator.integrateOneStep(solarSystem, beta);
      writeToFile_Position("../output/verlet_positions.xyz", t);
      writeToFile_Energy("../output/verlet_energies.dat", t);
      writeToFile_AngMom("../output/verlet_angmom.dat", t);
    }
  }
}


// Developes our solar system if the gravitational force is tested for various betas
void MainFunc::integration2(int method, int numTimesteps, double dt, double beta){
  if (beta == 3.0){
    writeToFile_Position("../output/verlet_test_positions.xyz", 0, beta);
  }

  VelocityVerlet integrator(dt);
  for(int timestep=0; timestep<numTimesteps; timestep++){
    double t = timestep*dt;
    integrator.integrateOneStep(solarSystem, beta);
    writeToFile_Position("../output/verlet_test_positions.xyz", t, beta);
  }
}


// Checks if file is open
void MainFunc::openFile(ofstream &file, string filename){
  if (!file.good()){
    file.open(filename.c_str(), ofstream::out);
    if(!file.good()){
        cout << "Error opening file " << filename << ". Aborting!" << endl;
        terminate();
    }
  }
}


// Writes position for the celestial bodies to file
void MainFunc::writeToFile_Position(string filename, double t){
  openFile(m_file_pos, filename);

  for (CelestialBody &body : solarSystem.bodies()){
    m_file_pos << body.nameOfBody << " " << t << " "
    << body.position.x() << " "
    << body.position.y() << " "
    << body.position.z() << endl;
  }
}


// Writes position for the celestial bodies to file if beta varies
void MainFunc::writeToFile_Position(string filename, double t, double beta){
  openFile(m_file_pos, filename);

  if (t == 0){
    m_file_pos << beta << endl;
  }

  for (CelestialBody &body : solarSystem.bodies()){
    m_file_pos << body.nameOfBody << " " << t << " "
    << body.position.x() << " "
    << body.position.y() << " "
    << body.position.z() << endl;
  }
}


// Writes energy for the solar system to file
void MainFunc::writeToFile_Energy(string filename, double t){
  openFile(m_file_E, filename);
  m_file_E << t << " " << solarSystem.potentialEnergy() << " " << solarSystem.kineticEnergy() << " " << solarSystem.totalEnergy() << endl;
}


// Writes angular momentum for the solar system to file
void MainFunc::writeToFile_AngMom(string filename, double t){
  openFile(m_file_AM, filename);
  int i = 0;
  for (vec3 &angMom : solarSystem.angularMomentum()){
      CelestialBody &body = solarSystem.bodies()[i];
      m_file_AM << body.nameOfBody << " " << t << " "
      << angMom(0) << " "
      << angMom(1) << " "
      << angMom(2) << endl;
      i += 1;
  }
}
