#ifndef Celery_Data
#define Celery_Data

#include <vector>
#include <Eigen/Dense>

namespace Celery
{

class Data
{
    private:
        std::vector<double> t, y, sig;
        Eigen::VectorXd tt, yy, var;

    public:
        Data();
        void load(const char* filename);

        // Getters
        const std::vector<double>& get_t() const { return t; }
        const std::vector<double>& get_y() const { return y; }
        const std::vector<double>& get_sig() const { return sig; }

        // Getters of eigen vectors
        const Eigen::VectorXd& get_tt() const { return tt; }
        const Eigen::VectorXd& get_yy() const { return yy; }
        const Eigen::VectorXd& get_var() const { return var; }

        // Summaries
        double get_t_range() const { return (t.back() - t[0]); }

    // Singleton
    private:
        static Data instance;
    public:
        static Data& get_instance() { return instance; }
};

} // namespace

#endif

