import dnest4.classic as dn4
import textwrap

# Load the text file and the indices
posterior_sample = dn4.my_loadtxt("posterior_sample.txt")
indices = dn4.load_column_names("posterior_sample.txt")["indices"]

def to_yaml():
    """
    Converts posterior samples to YAML
    """
    # Open the output file
    f = open("posterior_sample.yaml", "w")
    f.write("# Posterior samples in YAML format.\n")
    f.write("---\n\n")

    for i in range(0, posterior_sample.shape[0]):

        f.write("sample[{i}]:".format(i=i) + "\n")

        indent = "    - "
        num_peaks = int(posterior_sample[i, indices["num_components"]])
        f.write(indent + "num_peaks: " + str(num_peaks))
        f.write("\n")

        index = indices["amplitude[0]"]
        spaces = "".join([" " for i in range(0, 19)])
        amplitudes = [float(x)\
                        for x in posterior_sample[i, index:index+num_peaks]]
        amplitudes_str = textwrap.fill(str(amplitudes), width=60)
        amplitudes_str = amplitudes_str.replace("\n", "\n" + spaces)
        f.write(indent + "amplitudes: ")
        f.write(amplitudes_str)
        f.write("\n")

        index = indices["period[0]"]
        spaces = "".join([" " for i in range(0, 16)])
        periods = [float(x)\
                        for x in posterior_sample[i, index:index+num_peaks]]
        periods_str = textwrap.fill(str(periods), width=60)
        periods_str = periods_str.replace("\n", "\n" + spaces)
        f.write(indent + "periods: ")
        f.write(periods_str)
        f.write("\n")

        index = indices["quality[0]"]
        spaces = "".join([" " for i in range(0, 18)])
        qualities = [float(x)\
                        for x in posterior_sample[i, index:index+num_peaks]]
        qualities_str = textwrap.fill(str(qualities), width=60)
        qualities_str = qualities_str.replace("\n", "\n" + spaces)
        f.write(indent + "qualities: ")
        f.write(qualities_str)
        f.write("\n")

    # Close the output file
    f.close()

if __name__ == "__main__":
    to_yaml()


