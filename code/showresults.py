import dnest4.classic as dn4
import matplotlib.pyplot as plt

# Run postprocess from DNest4
dn4.postprocess()

# Load posterior samples
posterior_sample = dn4.my_loadtxt("posterior_sample.txt")
indices = dn4.load_column_names("posterior_sample.txt")["indices"]

# Periods
start = indices["period[0]"]
end   = indices["quality[0]"]
all_periods = posterior_sample[:, start:end].flatten()
all_periods = all_periods[all_periods != 0.0]

# Histogram of inferred periods
plt.hist(all_periods, 500, color=[0.2, 0.2, 0.4])
plt.xlabel("Period")
plt.ylabel("Relative probability")
plt.show()

# Histogram of number of modes
plt.hist(posterior_sample[:, indices["num_components"]], 100, color=[0.2, 0.2, 0.2])
plt.xlabel("num_components")
plt.show()

# Plot the marginal posterior for the error bar boost parameter
plt.hist(posterior_sample[:, indices["sigma_boost_factor"]], 100, color=[0.2, 0.2, 0.4])
plt.xlabel("sigma_boost_factor")
plt.show()

