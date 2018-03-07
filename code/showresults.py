import dnest4.classic as dn4
import matplotlib.pyplot as plt
import numpy as np

print("Generating DNest4 plots. Close these to continue.")

# Run postprocess from DNest4
dn4.postprocess()

print("Converting posterior samples to YAML for extra convenience.")
from to_yaml import posterior_sample, indices, to_yaml
to_yaml()

# Extract amplitudes
start = indices["amplitude[0]"]
end   = indices["period[0]"]
all_amplitudes = posterior_sample[:, start:end].flatten()
all_amplitudes = all_amplitudes[all_amplitudes != 0.0]

# Periods
start = indices["period[0]"]
end   = indices["quality[0]"]
all_periods = posterior_sample[:, start:end].flatten()
all_periods = all_periods[all_periods != 0.0]

# Extract quality factors
start = indices["quality[0]"]
end   = indices["sigma_boost_factor"]
all_qualities = posterior_sample[:, start:end].flatten()
all_qualities = all_qualities[all_qualities != 0.0]

# Histogram of inferred log-periods
plt.hist(np.log10(all_periods), 1000, alpha=0.3)
plt.xlabel(r"$\log_{10}$(period)")
plt.ylabel("Relative probability")
plt.show()

# Histogram of inferred periods, weighted by amplitude
plt.hist(np.log10(all_periods), bins=1000,
         weights=all_amplitudes, alpha=0.3)
plt.xlabel(r"$\log_{10}$(period)")
plt.ylabel("Relative expected amplitude")
plt.show()

# Plot period vs. quality factor
plt.loglog(all_periods,
           all_qualities,
           ".", alpha=0.2)
plt.xlabel("Period")
plt.ylabel("Quality factor")
plt.show()

# Histogram of number of modes
width = 0.7
bins = np.arange(0, posterior_sample[0, indices["max_num_components"]]+1)\
        - 0.5*width
plt.hist(posterior_sample[:, indices["num_components"]],
         bins,
         width=width,
         alpha=0.3,
         normed=True)
plt.xlabel("num_components")
plt.show()

# Plot the marginal posterior for the error bar boost parameter
plt.hist(posterior_sample[:, indices["sigma_boost_factor"]],
         100, alpha=0.3)
plt.xlabel("sigma_boost_factor")
plt.show()


# Plot the joint posterior for the correlated noise
plt.loglog(posterior_sample[:, indices["correlated_noise_timescale"]],
           posterior_sample[:, indices["correlated_noise_amplitude"]],
           ".", alpha=0.2)
plt.xlabel("correlated_noise_timescale")
plt.ylabel("correlated_noise_amplitude")
plt.show()

