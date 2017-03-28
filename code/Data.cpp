#include "Data.h"
#include <exception>
#include <fstream>
#include <iostream>

namespace Celery
{

Data Data::instance;

Data::Data()
{

}

void Data::load(const char* filename)
{
    std::fstream fin(filename, std::ios::in);
    if(!fin)
    {
        std::cerr<<"# Error. Couldn't open file "<<filename<<"."<<std::endl;
        return;
    }

    // Empty the vectors
    t.clear();
    y.clear();
    sig.clear();

    double temp1, temp2, temp3;
    while(fin>>temp1 && fin>>temp2 && fin>>temp3)
    {
        t.push_back(temp1);
        y.push_back(temp2);
        sig.push_back(temp3);

        if(t.size() > 0 && t.back() <= t[t.size() - 2])
            throw std::invalid_argument("Unsorted t-values in file.");
    }
    std::cout<<"# Loaded "<<t.size()<<" data points from file "
            <<filename<<"."<<std::endl;
    fin.close();

    // Copy into eigen vectors
    tt.resize(t.size());
    yy.resize(y.size());
    var.resize(sig.size());
    for(size_t i=0; i<y.size(); ++i)
    {
        tt(i) = t[i];
        yy(i) = y[i];
        var(i) = 1.0 / (sig[i] * sig[i]);
    }
}

} // namespace Celery

