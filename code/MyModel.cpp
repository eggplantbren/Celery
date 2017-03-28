#include "Data.h"
#include "MyModel.h"
#include "DNest4/code/DNest4.h"
#include <sstream>

namespace Celery
{

MyModel::MyModel()
:modes(3,                               // Dimensionality of a component
       max_num_modes,                   // Maximum number of components
       false,                           // Fixed number of components?
       MyConditionalPrior(),            // Conditional prior
       DNest4::PriorType::log_uniform)  // Prior on N
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

    // Get the RJObject components
    const auto& components = modes.get_components();
    size_t num_modes = components.size();

    // Count number of modes with Q < 0.5,
    // as these need *two* Celerite terms.
    size_t lowQ = 0;
    for(size_t i=0; i<components.size(); ++i)
        if(components[i][2] <= 0.5)
            ++lowQ;

    // Only need these four
    Eigen::VectorXd a(num_modes + lowQ);
    Eigen::VectorXd b(num_modes + lowQ);
    Eigen::VectorXd c(num_modes + lowQ);
    Eigen::VectorXd d(num_modes + lowQ);

    // Note: amplitude = S0*omega0*Q
    double omega0, Q, Qterm;
    size_t j=0; // Celerite term index

    for(size_t i=0; i<components.size(); ++i)
    {
        omega0 = 2.0*M_PI/components[i][1];
        Q = components[i][2];

        if(Q >= 0.5)
        {
            Qterm = sqrt(4*Q*Q - 1.0);
            a(j) = components[i][0];
            b(j) = components[i][0] / Qterm;
            c(j) = omega0 / (2*Q);
            d(j) = c(j) * Qterm;
            ++j;
        }
        else
        {
            Qterm = sqrt(1.0 - 4*Q*Q);
            a(j)   = 0.5*components[i][0]*(1.0 + 1.0/Qterm);
            a(j+1) = 0.5*components[i][0]*(1.0 - 1.0/Qterm);
            b(j) = 0.0;
            b(j+1) = 0.0;
            c(j)   = omega0/(2*Q)*(1.0 - Qterm);
            c(j+1) = omega0/(2*Q)*(1.0 + Qterm);
            j += 2;
        }
    }

    // Grab the data
    const Data& data = Data::get_instance();

    // Two empty vectors
    Eigen::VectorXd junk1, junk2;

    // Celerite solver
    celerite::solver::BandSolver<double> solver;
    solver.compute(junk1, junk2,
                   a, b, c, d,
                   data.get_tt(), data.get_var());

    logL += -0.5*log(2*M_PI)*data.get_y().size();
    logL += -0.5*solver.log_determinant();
    logL += -0.5*solver.dot_solve(Data::get_instance().get_yy());

    return logL;
}

void MyModel::print(std::ostream& out) const
{
    modes.print(out);
    out<<' ';
}

std::string MyModel::description() const
{
    std::stringstream s;

    s << "num_dimensions, max_num_components, ";
    s << "scale_amplitude, ";
    s << "mu_period, sig_log_period, ";
    s << "mu_quality, sig_log_quality, ";
    s << "num_components, ";

    for(size_t i=0; i<max_num_modes; ++i)
        s << "amplitude[" << i << "], ";
    for(size_t i=0; i<max_num_modes; ++i)
        s << "period[" << i << "], ";
    for(size_t i=0; i<max_num_modes; ++i)
        s << "quality[" << i << "], ";

    return s.str();
}

} // namespace Celery

