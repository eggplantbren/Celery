#ifndef Celery_MyModel
#define Celery_MyModel

#include "DNest4/code/DNest4.h"
#include "MyConditionalPrior.h"
#include <ostream>
#include "celerite/celerite.h"

namespace Celery
{

class MyModel
{
    private:

        // Maximum number of modes
        static constexpr size_t max_num_modes = 200;

        // The modes
        DNest4::RJObject<MyConditionalPrior> modes;

        // Error bar boost parameter
        double u_boost;
        double sigma_boost_factor;
        void compute_sigma_boost_factor();

        // For correlated noise
        double correlated_noise_relative;
        double correlated_noise_amplitude;
        double correlated_noise_timescale;

    public:
        // Constructor only gives size of params
        MyModel();

        // Generate the point from the prior
        void from_prior(DNest4::RNG& rng);

        // Metropolis-Hastings proposals
        double perturb(DNest4::RNG& rng);

        // Likelihood function
        double log_likelihood() const;

        // Print to stream
        void print(std::ostream& out) const;

        // Return string with column information
        std::string description() const;
};

} // namespace Celery

#endif

