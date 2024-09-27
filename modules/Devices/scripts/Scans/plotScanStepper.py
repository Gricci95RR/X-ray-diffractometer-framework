from matplotlib import pyplot as plt, style
import numpy as np
import pandas as pd
import seaborn as sns
from scipy.signal import savgol_filter
import pathlib
import sys
from importlib.machinery import SourceFileLoader

path_absolute = str(pathlib.Path().absolute())
moduleFunctions = SourceFileLoader('functions', path_absolute + "\\modules\\Devices\\scripts\\functions.py").load_module()

def plotScan():
    """
    Brief:
    Reads in a CSV file specified by the first argument passed in via the command line, extracts the data and plots it using the Seaborn library.
    The plot displays the signal intensity (y-axis) as a function of the position of one of the stepper motor (x-axis) using two line plots: one for the raw data and the other for the smoothed data obtained by applying a Savitzky-Golay filter.
    
    Args:
    sys.argv[1] (string): filename of the input .csv file xray sensor data.

    Note:
    The function relies on a module called 'moduleFunctions'.
    """
    logFileName = sys.argv[1]
    pathToScanData = "C:\\Users\\giricci\\Desktop\\XRay_Machine\\LogFiles\\DeviceScans\\" + logFileName
    dataframe = pd.read_csv(pathToScanData, skiprows=0, sep=";")
    moduleFunctions.renameLogFileScan(dataframe, logFileName) # save logFile with date and time
    # Print Informations about Smoothed DF
    dataframe_smoothed = savgol_filter(dataframe.iloc[:, 0], 10, 3) # Savitzky-Golay filter
    dictionary = {
        'X-Ray Sensor Data' : dataframe_smoothed
        }
    dataframe_smoothed = pd.DataFrame(dictionary)

    # Plot
    a4_dims = (13,8)
    sns.set(font_scale=2)
    sns.set_style("whitegrid")
    fig, (ax1) = plt.subplots(nrows=1, sharey=False, figsize=a4_dims)
    g1=sns.lineplot(y=dataframe.iloc[:, 0],
                    x=dataframe.iloc[:, 1], ax=ax1, color = 'blue', linewidth=2, markers=True, marker='o')
    g=sns.lineplot(y=dataframe_smoothed.iloc[:, 0],
                   x=dataframe.iloc[:, 1], ax=ax1, color = 'orangered', linewidth=2, markers=True, marker='o')
    plt.ylabel('Signal Intensity (arb. units)')
    plt.xlabel('Stepper Motor Position')
    ax1.legend({"X-Ray Sensor Data": dataframe.iloc[:, 0],
                "Smoothed X-Ray Sensor Data": dataframe_smoothed.iloc[:, 0]
                },
                loc = "lower right",
                title_fontsize = "1")
    plt.show()

plotScan()
