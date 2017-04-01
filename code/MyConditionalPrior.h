#ifndef Celery_MyConditionalPrior
#define Celery_MyConditionalPrior

#include <boost/math/distributions/normal.hpp>
#include "DNest4/code/DNest4.h"

namespace Celery
{

class MyConditionalPrior:public DNest4::ConditionalPrior
{
    private:
        // A cauchy distribution
        static const DNest4::Cauchy cauchy;

        // A standard normal distribution
        static const boost::math::normal normal;

        // Scale parameter for amplitudes.
        double scale_amplitude;

        // Median period
        double mu_period;
        double sig_log_period;

        // Median quality factor
        double mu_quality;
        double sig_log_quality;

        double perturb_hyperparameters(DNest4::RNG& rng);

    public:
        MyConditionalPrior();

        void from_prior(DNest4::RNG& rng);

        double log_pdf(const std::vector<double>& vec) const;
        void from_uniform(std::vector<double>& vec) const;
        void to_uniform(std::vector<double>& vec) const;

        // A getter for one of the hyperparameters
        double get_scale_amplitude() const { return scale_amplitude; }

        void print(std::ostream& out) const;
};

} // namespace Celery

#endif

