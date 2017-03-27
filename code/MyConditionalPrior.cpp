#include "MyConditionalPrior.h"
#include "DNest4/code/DNest4.h"
#include <cmath>
#include <exception>

namespace Celery
{

const DNest4::Cauchy MyConditionalPrior::cauchy(0.0, 5.0);

MyConditionalPrior::MyConditionalPrior(double min_period, double max_period)
:min_period(min_period)
,max_period(max_period)
{
    if(max_period <= min_period)
        throw std::invalid_argument("Invalid input to MyConditionalPrior \
                                     constructor.");
}

void MyConditionalPrior::from_prior(DNest4::RNG& rng)
{
    do
    {
        scale_amplitude = cauchy.generate(rng);
    }while(std::abs(scale_amplitude) > 50.0);
    scale_amplitude = exp(scale_amplitude);
}

double MyConditionalPrior::perturb_hyperparameters(DNest4::RNG& rng)
{
    double logH = 0.0;

    scale_amplitude = log(scale_amplitude);
    logH += cauchy.perturb(scale_amplitude, rng);
    if(std::abs(scale_amplitude) > 50.0)
        return -1E300;
    scale_amplitude = exp(scale_amplitude);

    return logH;
}

double MyConditionalPrior::log_pdf(const std::vector<double>& vec) const
{
    return 0.;
}

void MyConditionalPrior::from_uniform(std::vector<double>& vec) const
{

}

void MyConditionalPrior::to_uniform(std::vector<double>& vec) const
{

}

void MyConditionalPrior::print(std::ostream& out) const
{
    out<<scale_amplitude<<' ';
}

} // namespace Celery

