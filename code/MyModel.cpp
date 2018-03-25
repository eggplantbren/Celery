#include "Data.h"
#include "MyModel.h"
#include "DNest4/code/DNest4.h"
#include <iomanip>
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
    u_boost = rng.rand();
    compute_sigma_boost_factor();

    // For correlated noise
    correlated_noise_relative = exp(10*rng.randn());
    correlated_noise_amplitude = correlated_noise_relative *
               modes.get_conditional_prior().get_scale_amplitude();

    correlated_noise_timescale = exp(10*rng.randn())*
                Data::get_instance().get_t_range();
}

void MyModel::compute_sigma_boost_factor()
{
    if(u_boost <= 0.5)
    {
        sigma_boost_factor = 1.0;
    }
    else
    {
        double u = 2*(u_boost - 0.5);
        sigma_boost_factor = exp(-log(1.0 - u));
    }
}

double MyModel::perturb(DNest4::RNG& rng)
{
    double logH = 0.0;

    int which;
    if(rng.rand() <= 0.7)
        which = 0;
    else
        which = 1;

    if(which == 0)
    {
        logH += modes.perturb(rng);

        // Recompute derived quantity
        correlated_noise_amplitude = correlated_noise_relative *
              modes.get_conditional_prior().get_scale_amplitude();
    }
    else
    {
        int which2 = rng.rand_int(3);

        if(which2 == 0)
        {
            u_boost += rng.randh();
            DNest4::wrap(u_boost, 0.0, 1.0);
            compute_sigma_boost_factor();
        }
        else if(which2 == 1)
        {
            double& x = correlated_noise_relative;
            x = log(x);
            logH -= -0.5*pow(x / 10.0, 2);
            x += 10.0 * rng.randh();
            logH += -0.5*pow(x / 10.0, 2);
            x = exp(x);

            // Recompute derived quantity
            correlated_noise_amplitude = correlated_noise_relative *
                  modes.get_conditional_prior().get_scale_amplitude();
        }
        else
        {
            double& x = correlated_noise_timescale;
            x = log(x / Data::get_instance().get_t_range());
            logH -= -0.5*pow(x / 10.0, 2);
            x += 10.0 * rng.randh();
            logH += -0.5*pow(x / 10.0, 2);
            x = exp(x) * Data::get_instance().get_t_range();
        }
    }

    // Pre-reject
    if(rng.rand() >= exp(logH))
        return -1E300;
    else
        logH = 0.0;

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

    double omega0, Q, Qterm;
    size_t j=0; // Celerite term index

    for(size_t i=0; i<components.size(); ++i)
    {
        double A2 = pow(components[i][0], 2);
        omega0 = 2.0*M_PI/components[i][1];
        Q = components[i][2];

        if(Q >= 0.5)
        {
            Qterm = sqrt(4*Q*Q - 1.0);
            a(j) = A2; //components[i][0];
            b(j) = A2/Qterm;
            c(j) = omega0 / (2*Q);
            d(j) = c(j) * Qterm;
            ++j;
        }
        else
        {
            Qterm = sqrt(1.0 - 4*Q*Q);
            a(j)   = 0.5*A2*(1.0 + 1.0/Qterm);
            a(j+1) = 0.5*A2*(1.0 - 1.0/Qterm);
            b(j) = 0.0;
            b(j+1) = 0.0;
            c(j)   = omega0/(2*Q)*(1.0 - Qterm);
            c(j+1) = omega0/(2*Q)*(1.0 + Qterm);
            d(j)   = 0.0;
            d(j+1) = 0.0;
            j += 2;
        }
    }

    // Grab the data
    const Data& data = Data::get_instance();

    // Inflate variance
    Eigen::VectorXd var = data.get_var();
    double fsq = pow(sigma_boost_factor, 2);
    for(int i=0; i<var.size(); ++i)
        var[i] *= fsq;

    // For the correlated noise
    Eigen::VectorXd alpha_real(1), beta_real(1);
    alpha_real(0) = pow(correlated_noise_amplitude, 2);
    beta_real(0)  = 1.0 / correlated_noise_timescale;

    // Celerite solver
    celerite::solver::CholeskySolver<double> solver;

    try
    {
        solver.compute(0.0,
                       alpha_real, beta_real,
                       a, b, c, d,
                       data.get_tt(), var);
    }
    catch(...)
    {
        return -1E300;
    }
//    catch(...)
//    {
//        std::cout << "alpha_real: ";
//        for(int i=0; i<alpha_real.size(); ++i)
//            std::cout << alpha_real(i) << ' ';
//        std::cout << std::endl;

//        std::cout << "beta_real: ";
//        for(int i=0; i<beta_real.size(); ++i)
//            std::cout << beta_real(i) << ' ';
//        std::cout << std::endl;

//        std::cout << "a: ";
//        for(int i=0; i<a.size(); ++i)
//            std::cout << a(i) << ' ';
//        std::cout << std::endl;

//        std::cout << "b: ";
//        for(int i=0; i<b.size(); ++i)
//            std::cout << b(i) << ' ';
//        std::cout << std::endl;

//        std::cout << "c: ";
//        for(int i=0; i<c.size(); ++i)
//            std::cout << c(i) << ' ';
//        std::cout << std::endl;

//        std::cout << "d: ";
//        for(int i=0; i<d.size(); ++i)
//            std::cout << d(i) << ' ';
//        std::cout << std::endl;

//        std::cout << "var: ";
//        for(int i=0; i<var.size(); ++i)
//            std::cout << var(i) << ' ';
//        std::cout << std::endl;
//    }

    logL += -0.5*log(2*M_PI)*data.get_y().size();
    logL += -0.5*solver.log_determinant();
    logL += -0.5*solver.dot_solve(Data::get_instance().get_yy());

    return logL;
}

void MyModel::print(std::ostream& out) const
{
    out << std::setprecision(12);
    modes.print(out);
    out << ' ';
    out << sigma_boost_factor << ' ';
    out << correlated_noise_amplitude << ' ';
    out << correlated_noise_timescale << ' ';
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

    s << "sigma_boost_factor, ";
    s << "correlated_noise_amplitude, correlated_noise_timescale";

    return s.str();
}

} // namespace Celery

