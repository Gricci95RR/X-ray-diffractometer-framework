from matplotlib import pyplot as plt, style
import numpy as np
import pandas
import seaborn as sns
from scipy.signal import savgol_filter
import pathlib
import sys
from importlib.machinery import SourceFileLoader

path_absolute = str(pathlib.Path().absolute())
moduleFunctions = SourceFileLoader('functions', path_absolute + "\\modules\\Devices\\scripts\\functions.py").load_module()

def searchBraggPeak():
    """
    Brief:
    This function calculates the Bragg's Peak angle of the Monochromator crystal from a X-ray sensor signal. The X-ray sensor data is read from a .csv file
    whose path is passed as an argument. The function applies a Savitzky-Golay filter to the data to smooth it, and calculates the
    Full Width Half Maximum (FWHM) to determine the Bragg's Peak angle. The function then plots the original and smoothed data,
    along with the calculated Bragg's Peak angle and other relevant information. Finally, the result is saved to a .csv file,
    also specified as an argument.

    Args:
    sys.argv[1] (string): filename of the input .csv file xray sensor data.
    'save_path' (string): Path of the location where to save the result of the Bragg's Peak intensity and angle (rotational axis position of the monochromator).

    Returns:
    float: The calculated Bragg's Peak angle in degrees.

    Outputs:
    A CSV file containing the calculated Bragg's Peak angle in degrees.

    Note:
    The function relies on a module called 'moduleFunctions'.
    """
    logFileName = + sys.argv[1]
    path_absolute = str(pathlib.Path().absolute()).split("\\")
    path_to_data_dir = path_absolute[0] + "\\" + path_absolute[1] + "\\" + path_absolute[2] + "\\" + path_absolute[3] + "\\" + path_absolute[4] 
    path_csv = path_to_data_dir + "\\LogFiles\\DeviceScans\\" + logFileName
    save_path = sys.argv[2]
    dataframe = pandas.read_csv(path_csv, skiprows=0, sep=";").drop("Unnamed: 2", axis=1)
    moduleFunctions.renameLogFileScan(dataframe, logFileName) # save logFile with date and time

    # Print Informations about Smoothed DF
    dataframe_smoothed = savgol_filter(dataframe["X-Ray Sensor Data"], 10, 3) # Savitzky-Golay filter
    dictionary = {
        'X-Ray Sensor Data' : dataframe_smoothed
        }
    dataframe_smoothed = pandas.DataFrame(dictionary)

    # FWHM
    x=dataframe['Stepper Motor Position'].to_numpy()
    y=dataframe_smoothed['X-Ray Sensor Data'].to_numpy()
    hmx0, hmx1, max_y, min_y, half_y = moduleFunctions.half_max_x(x, y)

    bragg_angle = (hmx1 + hmx0) /2
    
    # Plot
    a4_dims = (13,8)
    sns.set(font_scale=2)
    sns.set_style("whitegrid")
    fig, (ax1) = plt.subplots(nrows=1, sharey=False, figsize=a4_dims)
    g1=sns.lineplot(y=dataframe['X-Ray Sensor Data'], x=dataframe['Stepper Motor Position'], ax=ax1, color = 'blue', linewidth=2, markers=True, marker='o')
    g=sns.lineplot(y=dataframe_smoothed['X-Ray Sensor Data'], x=dataframe['Stepper Motor Position'], ax=ax1, color = 'orangered', linewidth=2, markers=True, marker='o')
    g.axvline(bragg_angle, color = "lime")
    g.axhline(max_y, color = "black")
    g.axhline(half_y, color = "black")
    g.axvline(hmx0, color = "black")
    g.axvline(hmx1, color = "black")
    # Plot settings
    plt.xticks(rotation=0)
    plt.ylabel('Signal Intensity (arb. units)')
    plt.xlabel('Monochromator - Rotational Axis Position (deg)')
    plt.title("Bragg's Peak Angle: " + str(bragg_angle)[:5] + "°")
    ax1.legend({"X-Ray Sensor Data": dataframe["X-Ray Sensor Data"],
                "Smoothed X-Ray Sensor Data": dataframe["X-Ray Sensor Data"]},
                loc = "lower right",
                title_fontsize = "1")
    
    plt.show()

    # Save result in a .csv file
    list_position_in_idmax_X_Ray_Sensor_Data_sm = [bragg_angle]
    dictionary = {
        'Bragg Peak Angle': list_position_in_idmax_X_Ray_Sensor_Data_sm,
        }
    df = pandas.DataFrame(dictionary)
    df.to_csv(index=False, path_or_buf = save_path)

    return float(bragg_angle)

# Main
searchBraggPeak()