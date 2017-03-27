#ifndef Celery_MyConditionalPrior
#define Celery_MyConditionalPrior

#include "DNest4/code/DNest4.h"

namespace Celery
{

class MyConditionalPrior:public DNest4::ConditionalPrior
{
    private:

        double perturb_hyperparameters(DNest4::RNG& rng);

    public:
        MyConditionalPrior();

        void from_prior(DNest4::RNG& rng);

        double log_pdf(const std::vector<double>& vec) const;
        void from_uniform(std::vector<double>& vec) const;
        void to_uniform(std::vector<double>& vec) const;

        void print(std::ostream& out) const;
};

} // namespace Celery

#endif

