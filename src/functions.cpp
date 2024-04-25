#include "functions.hpp"
#include <random>
#include <iostream>
#include <vector>
#include <cstdlib> // pour rand()
#include <ctime> 

float rand01()
{
    thread_local std::default_random_engine gen{std::random_device{}()};
    thread_local auto                       distrib = std::uniform_real_distribution<float>{0.0, 1.0};

    return distrib(gen);
}

//Loi uniforme
double uniform(double a, double b)
{
    return a + (b - a) * rand01();
}

//Loi hypergeometrique
std::vector<int> generateHyperGeometric(int nombreTotal, int essaie, int nombreSucces) {
    std::vector<int> result;
    std::vector<int> TotalArray;

    // Initialiser TotalArray avec les succès et les échecs
    for (int i = 0; i < nombreSucces; i++) {
        TotalArray.push_back(1);
    }
    for (int i = 0; i < (nombreTotal - nombreSucces); i++) {
        TotalArray.push_back(0);
    }

    // Générer les résultats
    for (int i = 0; i < essaie; i++) {
        int randIndex = rand() % nombreTotal;
        result.push_back(TotalArray[randIndex]);
        TotalArray.erase(TotalArray.begin() + randIndex);
        nombreTotal--;
    }
    return result;
}

//Bernoulli
std::vector<int> generateBernoulliSchema(double probaSuccess, int essaie) {
    std::vector<int> result;

    if (probaSuccess > 1 || probaSuccess < 0) {
        std::cout << "La probabilité doit être comprise entre 0 et 1" << std::endl;
        return result;
    }

    for (int i = 0; i < essaie; i++) {
        double randNum = static_cast<double>(rand01()) / RAND_MAX; // Générer un nombre entre 0 et 1
        if (randNum <= probaSuccess) {
            result.push_back(1);
        } else {
            result.push_back(0);
        }
    }

    return result;
}

//Loi exponentielle
double generateExponential(double lambda) {
    double u = static_cast<double>(rand01()) / RAND_MAX; // Générer un nombre aléatoire U entre 0 et 1
    return -log(1 - u) / lambda; // Utiliser la fonction inverse de la CDF exponentielle
}

//Loi géométrique
double geometricTrial(double p) {
    int count = 0;
    while (true) {
        count++;
        if (rand() % 100 < p * 100) {
            return count;
        }
    }
}

//Loi de poisson
int poissonRandom(double lambda) {
    double L = exp(-lambda);
    double p = 1.0;
    int k = 0;

    do {
        k++;
        p *= static_cast<double>(rand01()) / RAND_MAX;
    } while (p > L);

    return k - 1;
}

//Loi de Laplace
double laplaceRandom(double mu, double b) {
    double p = static_cast<double>(rand01()) / RAND_MAX; // Génère un nombre aléatoire uniforme entre 0 et 1
    int sgn = (p < 0.5) ? -1 : 1; // Fonction signe basée sur p
    return mu - b * sgn * log(1 - 2 * fabs(p - 0.5));
}