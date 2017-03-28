#include "MyModel.h"
#include "DNest4/code/DNest4.h"

namespace Celery
{

MyModel::MyModel()
:modes(3,       // Dimensionality of a component
       100,     // Maximum number of components
       false,   // Fixed number of components?
       MyConditionalPrior(0.0, 1.0),    // Conditional prior
       DNest4::PriorType::log_uniform)
{

}

void MyModel::from_prior(DNest4::RNG& rng)
{
    modes.from_prior(rng);
}

double MyModel::perturb(DNest4::RNG& rng)
{
    double logH = 0.0;

    logH += modes.perturb(rng);

    return logH;
}

double MyModel::log_likelihood() const
{
    double logL = 0.0;
    return logL;
}

void MyModel::print(std::ostream& out) const
{
    modes.print(out);
    out<<' ';
}

std::string MyModel::description() const
{
    return std::string("");
}

} // namespace Celery

