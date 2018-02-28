Celery
======

[DNest4](https://github.com/eggplantbren/DNest4) +
[Celerite](https://github.com/dfm/celerite) = *Celery*.
This program does inference of an unknown
number of damped, stochastically excited signals from noisy data.
The basic modelling ideas are covered in
[this paper](https://arxiv.org/abs/0902.3907), but the technology is
much better.

(c) 2017, 2018 Brendon J. Brewer

LICENSE: GNU General Public License, version 3.
http://www.gnu.org/licenses/gpl.txt

You'll need: git, Eigen3, LAPACK (including header files),
a recent C++ compiler, Python (3?), numpy,
matplotlib, numba.

## Download and compile

Do this:

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
The data consists of a single mode with a period of 20 (log10-period of 1.301),
and a quality factor of 50 (mode lifetime = 20x50 = 1000).

The data also contains red noise and the error bars are too small. The
modelling assumptions explicitly allow for both of these features.


## Running Celery

To run Celery on the example data using 8 threads (recommended), use

```
./Celery -t 8 -d example_data.txt
```

Celery will run and you will see
DNest4 output. Give it about 20
minutes (on the example dataset) before doing any postprocessing.

## Postprocessing

Celery will run for a long time. The longer you run it, the more reliable the
output will be. You can manually terminate it, or you can
do the postprocessing without terminating the main process.

The postprocessing will require
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

