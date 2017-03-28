#include <iostream>
#include "Data.h"
#include "DNest4/code/DNest4.h"
#include "MyModel.h"

int main(int argc, char** argv)
{
    // Load data
    Celery::Data::get_instance().load("data.txt");

    DNest4::start<Celery::MyModel>(argc, argv);
    return 0;
}

