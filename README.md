Celery
======

DNest4 + Celerite = Celery

(c) 2017, 2018 Brendon J. Brewer

LICENSE: GNU General Public License, version 3.
http://www.gnu.org/licenses/gpl.txt

To clone and compile, you'll need git and Eigen 3.
Then do this:

```
git clone --recursive https://github.com/eggplantbren/Celery
cd Celery/code
make
```

## Example Data

There is an example dataset. To regenerate and view it, use

```
python generate_data.py
```

This plots the data and (once you close the first plot) the periodogram.

To run Celery on the example data using 8 threads (recommended), use

```
./Celery -t 8 -d example_data.txt
```

Celery will run and you will see
[DNest4](https://github.com/eggplantbren/DNest4) output. Give it about 20
minutes (on the example dataset) before doing any postprocessing.

## Postprocessing

Celery will run for a long time. You can manually terminate it, or you can
do the postprocessing without terminating the main process. This will require
that you've installed the DNest4 Python package
(see [here](https://github.com/eggplantbren/DNest4) for instructions).
Simply invoke

```
python showresults.py
```

This will generate a bunch of output plots. As you close them, more will appear.
Posterior samples (currently only a subset of the parameters)
will also be saved in a YAML
file for your convenience. These files have passed verification with
yamllint.

## The maximum number of modes

By default, Celery has a hard maximum of 30 on the number of modes it will
search for. The computational expense scales as the square of this maximum,
but it is recommended that you increase it (to 200, as a suggestion) in
serious research applications. To do this, edit the value in MyModel.h and
recompile by running `make`. Sorry it's hard coded at this point.

