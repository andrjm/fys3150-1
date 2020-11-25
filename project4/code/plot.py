import numpy as np
import matplotlib.pyplot as plt
import subprocess
import os

#for nice plots
plt.style.use('seaborn')
plt.rc('text', usetex=True)
plt.rc('text.latex', preamble=r'\usepackage{amsmath}')


L = 2
LL = L**2
MCCs = 100_000
temperature = np.linspace(0.5,5,1000)
temperature_num = np.linspace(0.5,5,10)


expEnergy = np.zeros(len(temperature_num))
expEnergySquared = np.zeros(len(temperature_num))
expMagneticMoment = np.zeros(len(temperature_num))
expMagneticMomentSquared = np.zeros(len(temperature_num))

for i, T in enumerate(temperature_num):
    # Compiling the C++ script
    subprocess.call(['c++', '-o', 'main.exe', 'isingmodel.cpp', 'main.cpp', '-larmadillo', '-O3', '-march=native'])
    os.system(f'./main.exe 20 {T} 2 {MCCs}')
    values = np.loadtxt(f'../output/RandomOrientation_{T}.dat')
    expEnergy[i] = values[:,1][-1]#*LL
    expEnergySquared[i] = values[:,2][-1]#*LL*LL
    expMagneticMoment[i] = values[:,3][-1]#*LL
    expMagneticMomentSquared[i] = values[:,4][-1]#*LL*LL
    if T == 1:
        beta = 1/T
        cv_numerical = beta/T*(expEnergySquared[i] - expEnergy[i]**2)
        chi_numerical = beta*(expMagneticMomentSquared[i] - expMagneticMoment[i]**2)

        e_exp = -8*np.sinh(8*beta)/(3+np.cosh(8*beta))/LL
        e_squared_exp = 8**2*(np.cosh(8*beta)/(3+np.cosh(8*beta)))/LL/LL
        m_exp = 2*(2+np.exp(8*beta))/(3+np.cosh(8*beta))/LL
        m_squared_exp = 8*(1+np.exp(8*beta))/(3+np.cosh(8*beta))/LL/LL
        cv_analytical = beta/T*(e_squared_exp - e_exp**2)
        chi_analytical = beta*(m_squared_exp - m_exp**2)

        print("expEnergy=", expEnergy[i])
        print("Analytical expEnergy=", e_exp)
        print("expEnergySquared=", expEnergySquared[i])
        print("Analytical expEnergySquared=", e_squared_exp)
        print("expMagneticMoment=", expMagneticMoment[i])
        print("Analytical expMagneticMoment=", m_exp)
        print("expMagneticMomentSquared=", expMagneticMomentSquared[i])
        print("Analytical expMagneticMomentSquared=", m_squared_exp)
        print("cv_numerical=", cv_numerical)
        print("Analytical cv=", cv_analytical)
        print("chi_numerical=", chi_numerical)
        print("Analytical chi=", chi_analytical)


k_b = 1
beta = 1/temperature
beta_num = 1/temperature_num

# numerical values of heat capacity and susceptibiliy for random matrix
cv_numerical = beta_num/temperature_num*(expEnergySquared - expEnergy**2)
chi_numerical = beta_num*(expMagneticMomentSquared - expMagneticMoment**2)

#analytical values of the expectation values and partition function
e_exp = -8*np.sinh(8*beta)/(3+np.cosh(8*beta))/LL
e_squared_exp = 8**2*(np.cosh(8*beta)/(3+np.cosh(8*beta)))/LL/LL
m_exp = 2*(2+np.exp(8*beta))/(3+np.cosh(8*beta))/LL
m_squared_exp = 8*(1+np.exp(8*beta))/(3+np.cosh(8*beta))/LL/LL

# analytical values of heat capacity and susceptibiliy
cv_analytical = beta/temperature*(e_squared_exp - e_exp**2)
chi_analytical = beta*(m_squared_exp - m_exp**2)

# c_v
fig, ax = plt.subplots()
ax.plot(temperature, cv_analytical, color="#B1C084", label=f"Analytical heat capacity" )
ax.plot(temperature_num, cv_numerical, color='#1A7DA8', label=f"Numerical heat capacity")
ax.set_title(f"The specific heat capacity $C_V$ as a function of temperature $T$", fontsize=20)
ax.set_xlabel("$T$[J]", fontsize=15)
ax.set_ylabel("$C_V(T)$", fontsize=15)
ax.legend(fontsize=15)
fig.savefig(f'../plots/specific_heat_capacity{MCCs}.pdf')

# chi
fig, ax = plt.subplots()
ax.plot(temperature, chi_analytical, color="#B1C084", label=f"Analytical heat susceptibility" )
ax.plot(temperature_num, chi_numerical, color='#1A7DA8', label=f"Numerical heat susceptibility")
ax.set_title(f"The susceptibility $\chi$ as a function of temperature $T$", fontsize=20)
ax.set_xlabel("$T$[J]", fontsize=15)
ax.set_ylabel("$\chi(T)$", fontsize=15)
ax.legend(fontsize=15)
fig.savefig(f'../plots/susceptibility{MCCs}.pdf')

# Expectation magnetization
fig, ax = plt.subplots()
ax.plot(temperature, m_exp, color="#B1C084", label=f"Analytical")
ax.plot(temperature_num, expMagneticMoment, "--", color="#1A7DA8", label=f"Numerical")
ax.set_title(f"Analytical and numerical expectation value of magnetization", fontsize=20)
ax.set_xlabel("$T$[J]", fontsize=15)
ax.set_ylabel("$<M>$", fontsize=15)
ax.legend(fontsize=15)
fig.savefig(f'../plots/compare_expectation_magnetization{MCCs}.pdf')

# Expectation energy squared
fig, ax = plt.subplots()
ax.plot(temperature, e_exp, color="#B1C084", label=f"Analytical")
ax.plot(temperature_num, expEnergy, "--", color="#1A7DA8", label=f"Numerical")
ax.set_title(f"Analytical and numerical expectation value of energy", fontsize=20)
ax.set_xlabel("$T$[J]", fontsize=15)
ax.set_ylabel("$<E>$", fontsize=15)
ax.legend(fontsize=15)
fig.savefig(f'../plots/compare_expectation_energy{MCCs}.pdf')
