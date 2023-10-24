# read a file containing numbers seprated with , and plot it
# using matplotlib

import matplotlib.pyplot as plt

NUM_OF_SIM = 3

for i in range(1, NUM_OF_SIM + 1):
    # Open the file and read the contents
    with open(f'result{i}.txt', 'r') as f:
        data = f.read()

    # Split the contents by commas and convert to a list of floats
    data = data.split('\n')
    data = [int(x) for x in data]
    print(int(data[1]))

    # plot x = "time" and y = "cwnd
    plt.xlabel('Time')
    plt.ylabel('CWND')

    # Plot the data
    plt.plot(data)

    # find mean of data and plot it
    mean = sum(data) / len(data)
    plt.plot([mean for _ in range(len(data))])

    # show mean value in top of plot
    plt.title(f'Mean: {mean:.2f}')

    plt.show()
