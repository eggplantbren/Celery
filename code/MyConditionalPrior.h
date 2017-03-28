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

        // Minimum and maximum allowable period
        double min_period, max_period;

        // Scale parameter for amplitudes.
        double scale_amplitude;

        // Median quality factor
        double mu_quality;
        double sig_log_quality;

        double perturb_hyperparameters(DNest4::RNG& rng);

    public:
        MyConditionalPrior(double min_period, double max_period);

        void from_prior(DNest4::RNG& rng);

        double log_pdf(const std::vector<double>& vec) const;
        void from_uniform(std::vector<double>& vec) const;
        void to_uniform(std::vector<double>& vec) const;

        void print(std::ostream& out) const;
};

} // namespace Celery

#endif

