#ifndef Celery_Data
#define Celery_Data

#include <vector>
#include "Eigen/Dense"

namespace Celery
{

class Data
{
    private:
        std::vector<double> t, y, sig;
        Eigen::VectorXd y_eigen;

    public:
        Data();
        void load(const char* filename);

        // Getters
        const std::vector<double>& get_t() const { return t; }
        const std::vector<double>& get_y() const { return y; }
        const Eigen::VectorXd& get_y_eigen() const { return y_eigen; }
        const std::vector<double>& get_sig() const { return sig; }

    // Singleton
    private:
        static Data instance;
    public:
        static Data& get_instance() { return instance; }
};

} // namespace

#endif

