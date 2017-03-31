#include <iostream>
#include "Data.h"
#include "DNest4/code/DNest4.h"
#include "MyModel.h"

int main(int argc, char** argv)
{
    // Process command line options
    DNest4::CommandLineOptions clos(argc, argv);

    // Get specified data file. If none, ask the user.
    std::string data_file = clos.get_data_file();
    if(data_file.length() == 0)
    {
        std::cout << "# Enter the name of the data file you want to load: ";
        std::cin >> data_file;
    }

    // Save the data filename
    std::fstream fout("run_data.txt", std::ios::out);
    fout<<data_file;
    fout.close();

    // Load data
    Celery::Data::get_instance().load(data_file.c_str());

    // Run DNest4.
    DNest4::start<Celery::MyModel>(clos);

    return 0;
}

