#include "MyConditionalPrior.h"
#include "DNest4/code/DNest4.h"
#include <cmath>
#include <exception>

namespace Celery
{

const DNest4::Cauchy MyConditionalPrior::cauchy(0.0, 5.0);
const boost::math::normal MyConditionalPrior::normal(0.0, 1.0);

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

    mu_quality = exp(log(1.0) + log(1000.0)*rng.rand());
    sig_log_quality = rng.rand();
}

double MyConditionalPrior::perturb_hyperparameters(DNest4::RNG& rng)
{
    double logH = 0.0;

    int which = rng.rand_int(3);

    if(which == 0)
    {
        scale_amplitude = log(scale_amplitude);
        logH += cauchy.perturb(scale_amplitude, rng);
        if(std::abs(scale_amplitude) > 50.0)
            return -1E300;
        scale_amplitude = exp(scale_amplitude);
    }
    else if(which == 1)
    {
        mu_quality = log(mu_quality);
        mu_quality += log(1000.0)*rng.randh();
        DNest4::wrap(mu_quality, log(1.0), log(1000.0));
        mu_quality = exp(mu_quality);
    }
    else
    {
        sig_log_quality += rng.randh();
        DNest4::wrap(sig_log_quality, 0.0, 1.0);
    }

    return logH;
}

// component = {amplitude, period, quality}
double MyConditionalPrior::log_pdf(const std::vector<double>& vec) const
{
    double logP = 0.0;

    if(vec[0] < 0.0)
        return -1E300;
    if(vec[1] < min_period || vec[1] > max_period)
        return -1E300;
    if(vec[2] < 0.0)
        return -1E300;

    logP += -log(scale_amplitude) - vec[0] / scale_amplitude;
    logP += -log(sig_log_quality) - log(vec[2])
            - 0.5*pow((log(vec[2]) - log(mu_quality))/sig_log_quality, 2);

    return logP;
}

void MyConditionalPrior::from_uniform(std::vector<double>& vec) const
{
    vec[0] = -scale_amplitude * log(1.0 - vec[0]);
    vec[1] = min_period + (max_period - min_period)*vec[1];

    vec[2] = log(mu_quality) + sig_log_quality*quantile(normal, vec[2]);
    vec[2] = exp(vec[2]);
}

void MyConditionalPrior::to_uniform(std::vector<double>& vec) const
{
    vec[0] = 1.0 - exp(-vec[0] / scale_amplitude);
    vec[1] = (vec[1] - min_period) / (max_period - min_period);

    vec[2] = log(vec[2]);
    vec[2] = (vec[2] - log(mu_quality)) / sig_log_quality;
    vec[2] = cdf(normal, vec[2]);
}

void MyConditionalPrior::print(std::ostream& out) const
{
    out<<scale_amplitude<<' '<<mu_quality<<' '<<sig_log_quality<<' ';
}

} // namespace Celery

