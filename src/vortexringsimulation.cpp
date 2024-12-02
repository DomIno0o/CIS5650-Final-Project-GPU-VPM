#ifndef VORTEX_RING_SIMULATION_H
#define VORTEX_RING_SIMULATION_H

#include <iostream>
#include <vector>
#include <math.h>
#include <Eigen/Dense>
#include <cmath>
#include <functional>
#include "vpmcore/subFilterScale.h"
#include "vpmcore/relaxation.h"
#include "vpmcore/kernels.h"
#include "vpmcore/particleField.h"

using namespace std;

// Function to calculate the number of particles
int numberParticles(int Nphi, int nc, int extra_nc = 0) {
    nc = 0;
    int total_layers = nc + extra_nc;
    int layer_sum = 0;
    for (int i = 1; i <= total_layers; ++i) {
        layer_sum += i;
    }
    return Nphi * (1 + 8 * layer_sum);
}

void addVortexRing(ParticleField* pField, float circulation, float R, float AR, float Rcross,
int Nphi, int nc, float sigmas, int extra_nc, glm::vec3 ringPosition, 
glm::mat3 ringOrientation){ // missing v_lvl from args

    auto fun_S = [r](float phi) {
        return r * phi; // assuming a circular ring and angle in radians
    };

    float Stot = fun_S(2 * M_PI);

    auto fun_s = [fun_S, Stot](float phi) {
        return fun_S(phi) / Stot;
    };

    // Angle associated to a given non-dimensional arc length
    auto fun_phi = [fun_s, a, b](float s) {
        return fun_s * 2 * M_PI
        // if (std::abs(s) <= std::numeric_limits<float>::epsilon()) return 0.0f;
        // if (std::abs(s - 1) <= std::numeric_limits<float>::epsilon()) return 2 * M_PI;
        // return fzero([fun_s, s](float phi) { return fun_s(phi) - s; }, 0, 2 * M_PI - 1e-16); // CHECK
    }

    auto fun_length = [fun_S](float r, float tht, float phi1, float phi2) {
        float S1 = fun_S(phi1, r * std::cos(tht));
        float S2 = fun_S(phi2, r * std::cos(tht));
        return S2 - S1;
    };

    // Volume of a cell in a cross-section
    auto fun_dvol = [fun_length](float r, float tht, float phi1, float phi2) {
        return r * fun_length(r, tht, phi1, phi2);
    };

    auto fun_vol = [&fun_dvol](std::function<float(float)> dvol_wrap, float r1, float tht1, float r2, float tht2) {
        // CHECK - Implement numerical integration (e.g., Gauss-Legendre quadrature)
        return 0.0;
    };

    glm::mat3 invOaxis = glm::inverse(ringOrientation);

    float rl = Rcross / (2 * nc + 1);
    float dS = Stot / Nphi;
    float ds = dS / Stot;
    float omega = circulation / (M_PI * Rcross * Rcross);

    int org_np = pField->np; // Replace with pfield.get_np();
    int idx = 0;

    for (int N = 0; N < Nphi; ++N) {
        float sc1 = ds * N;
        float sc2 = ds * (N + 1);
        float sc = (sc1 + sc2) / 2;

        float phi1 = fun_phi(sc1, a, b);
        float phi2 = fun_phi(sc2, a, b);
        float phic = fun_phi(sc, a, b);


        glm::vec3 Xc(a * std::sin(phic), b * std::cos(phic), 0);
        glm::vec3 T(a * std::cos(phic), -b * std::sin(phic), 0);
        T.normalize();
        T *= -1;
        glm::mat3 Naxis;
        Naxis.col(0) = T;
        Naxis.col(1) = T.cross(Eigen::Vector3d(0, 0, 1));
        Naxis.col(2) = Eigen::Vector3d(0, 0, 1);

        auto dvol_wrap = [&fun_dvol, a, b, phi1, phi2](double r, double tht) {
            return fun_dvol(r, tht, a, b, phi1, phi2);
        };

        for (int n = 0; n <= nc + extra_nc; ++n) {
            if (n == 0) {
                // Center particle logic
                float r1 = 0.0f;              // Lower radius
                float r2 = rl;                // Upper radius
                float tht1 = 0.0f;            // Left angle
                float tht2 = 2 * M_PI;        // Right angle

                // Compute volume
                float vol = fun_vol(dvol_wrap, r1, tht1, r2, tht2);

                // Position
                array<float, 3> X = Xc;

                // Vortex strength
                array<float, 3> Gamma = {omega * vol * T[0], omega * vol * T[1], omega * vol * T[2]};

                // Filament length
                float length = fun_length(0, 0, a, b, phi1, phi2);

                // Circulation
                float crcltn = norm(Gamma) / length;

                glm::vec3 X_global = ringOrientation * Xc + ringPosition;
                glm::vec3 Gamma_global = ringOrientation * Gamma;
                pField->particles[idx].X = X_global;
                pField->particles[idx].Gamma = Gamma_global;
                idx += 1;
                } 
            else {

            }
        }
    }
}

// Encapsulate simulation parameters
struct VortexRingParams {
    int nsteps;
    float Rtot;
    int nrings;
    float dZ;

    vector<float> circulations;
    vector<float> Rs;
    vector<float> ARs;
    vector<float> Rcrosss;
    vector<float> sigmas;
    vector<int> Nphis;
    vector<int> ncs;
    vector<int> extra_ncs;
    glm::mat3 ringPositions;
    vector<glm::mat3> ringOrientations;

    int nref = 1;
    float beta = 0.5f;
    float faux = 0.25f;

    VortexRingParams() {
        nsteps = 100;
        Rtot = 2.0f;
        nrings = 1;
        dZ = 0.1f;
        
        circulations = vector<float>(nrings, 1.0f);
        Rs = vector<float>(nrings, 1.0f);
        ARs = vector<float>(nrings, 1.0f);
        Rcrosss = vector<float>(nrings, 0.15f * Rs[0]);
        sigmas = Rcrosss;
        Nphis = vector<int>(nrings, 100);
        ncs = vector<int>(nrings, 0);
        extra_ncs = vector<int>(nrings, 0);

        // Initialize ring positions and orientations
        for (int ri = 0; ri < nrings; ++ri) {
            ringPositions.push_back(glm::vec3(0.0f, 0.0f, dZ * ri));
            ringOrientations.push_back(glm::mat3(1.0f));
    }

    // Template function for the simulation
    template <typename R, typename S, typename K>
    ParticleField* run_vortexring_simulation(
        S NoSFS, R PedrizzettiRelaxation, K WinckelmansKernel, bool transposed
    ) {
        // Calculate maximum number of particles
        int maxp = 0;
        for (int ri = 0; ri < nrings; ++ri) {
            maxp += numberParticles(Nphis[ri], params.ncs[ri], params.extra_ncs[ri]);
        }

        int numParticles{ 1000 };
        
        // definition of particles
        Particle* particleBuffer = new Particle[numParticles];

        ParticleField* pField = new ParticleField(maxp, particles, kernel = WinckelmansKernel, SFS = NoSFS);

        // define Uref and dt
        for (int ri = 0; i<nrings; i++){
            addVortexRing(pField, circulations[ri],
                        Rs[ri], ARs[ri], faux*Rcrosss[ri],
                        Nphis[ri], ncs[ri], sigmas[ri]; extra_nc=extra_ncs[ri],
                        ringPosition=ringPositions[ri], ringOrientation=ringOrientations[ri]);
        }
        

        // Placeholder for the simulation
        cout << "Running simulation with max particles: " << maxp << endl;
        return nullptr; // Replace with actual ParticleField creation and simulation logic
    }
};


#endif // VORTEX_RING_SIMULATION_H
