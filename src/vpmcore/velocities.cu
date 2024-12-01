#include <glm/glm.hpp>
#include "particlefield.h"
#include "particle.h"
#include "kernels.h"
#include "velocities.h"

#define EPS 1e-6f

template <class Kernel>
__device__ void calcVelJacNaive(int index, ParticleField* source, ParticleField* target, Kernel kernel) {
    Particle& targetParticle = target->particles[index];

    for (int i = 0; i < source->np; ++i) {
        if (i == index) continue;

        Particle& sourceParticle = source->particles[i];

        glm::vec3 dX = targetParticle.X - sourceParticle.X;
        float r = glm::length(dX);
        float r3 = r*r*r;

        // is this needed?
        if (r < EPS) continue;

        // Kernel evaluation
        float g_sgm = kernel.g(r / sourceParticle.sigma);
        float dg_sgmdr = kernel.dgdr(r / sourceParticle.sigma);

        // Compute velocity
        glm::vec3 crossProd = (- const4 / r3) * glm::cross(dX, sourceParticle.Gamma);
        targetParticle.U += g_sgm * crossProd;

        // Compute Jacobian
        float tmp = dg_sgmdr / (sourceParticle.sigma * r) - 3.0f * g_sgm / (r*r);
        glm::vec3 dX_norm = dX / r;

        for (int l = 0; l < 3; ++l) {
            for (int k = 0; k < 3; ++k) {
                targetParticle.J[l][k] += tmp * crossProd[k] * dX_norm[l];
            }
        }

        tmp = - const4 * g_sgm / r3;

        // Account for kronecker delta term
        targetParticle.J[0][1] -= tmp * sourceParticle.Gamma[2];
        targetParticle.J[0][2] += tmp * sourceParticle.Gamma[1];
        targetParticle.J[1][0] += tmp * sourceParticle.Gamma[2];
        targetParticle.J[1][2] -= tmp * sourceParticle.Gamma[0];
        targetParticle.J[2][0] -= tmp * sourceParticle.Gamma[1];
        targetParticle.J[2][1] += tmp * sourceParticle.Gamma[0];
    }
}

__device__ void calcVelJacNaive(int index, ParticleField* field) {
    calcVelJacNaive(index, field, field, field->kernel);
}