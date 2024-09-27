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

def ZAxisAlignmentCrystal():
    """
    Brief:
    This function reads X-Ray sensor data and calculates the center of a crystal in the Z-axis direction (vertical).
    The input to the script is a csv file containing the X-Ray sensor data and the Z-axis position of the crystal during a scan.
    The script uses the Savitzky-Golay filter to smooth the X-Ray sensor data, calculates the average count rate of the fully opened beam, finds the position at which the smoothed X-Ray sensor data intersects the average count rate, and plots the result.
    The calculated center position of the crystal in the Z-axis direction is saved in a .csv file.

    Input Args:
    1. csv file name containing the X-Ray sensor data and the Z-axis position of the crystal during the scan.
    2. path where the result .csv file should be saved.

    Output:
    The function returns no direct outputs, but it saves the following results to file:
    - 'save_path': Z-Axis alignment position.

    Note:
    The function relies on a module called 'moduleFunctions'.
    """
    path_absolute = str(pathlib.Path().absolute()).split("\\")
    path_to_data_dir = path_absolute[0] + "\\" + path_absolute[1] + "\\" + path_absolute[2] + "\\" + path_absolute[3] + "\\" + path_absolute[4] 
    logFileName = sys.argv[1]
    path_csv = path_to_data_dir + "\\LogFiles\\DeviceScans\\" + logFileName
    save_path = sys.argv[2]
    dataframe = pandas.read_csv(path_csv, skiprows=0, sep=";")
    moduleFunctions.renameLogFileScan(dataframe, logFileName) # save logFile with date and time
    # Smoothed DF
    dataframe_smoothed = savgol_filter(dataframe["X-Ray Sensor Data"], 10, 3) # Savitzky-Golay filter

    # Avg count rate of the fully opened beam
    open_beam = dataframe_smoothed[0]
    open_beam_50 = open_beam / 2
    
    # Intersection mean_count_50 horizontal line and Counts
    y = dataframe_smoothed
    x = dataframe["HXP Z-Axis"].to_numpy()
    try:
        alignment_position = moduleFunctions.half_max_position_interpolation(x, y, open_beam_50)
    except:
        alignment_position = float(dataframe.iloc[-1, 3])
    
    # Plot
    a4_dims = (13,8)
    sns.set(font_scale=2)
    sns.set_style("whitegrid")
    fig, (ax1) = plt.subplots(nrows=1, sharey=False, figsize=a4_dims)
    g=sns.lineplot(y=dataframe['X-Ray Sensor Data'], x=dataframe['HXP Z-Axis'], ax=ax1, color = 'blue', linewidth=2, markers=True, marker='o', sort = False)
    g=sns.lineplot(y=dataframe_smoothed, x=dataframe['HXP Z-Axis'], ax=ax1, color = 'orangered', linewidth=2, markers=True, marker='o', sort = False)
    g.axhline(open_beam_50, color = 'black')
    g.axvline(alignment_position, color = 'lime')
    
    # Plot settings
    plt.xticks(rotation=0)
    plt.ylabel('Signal Intensity (arb. units)')
    plt.xlabel('Crystal - Z Axis Position (mm)')
    plt.show()

    # Save result position in a .csv file
    list_alignment_pos = [alignment_position]
    dictionary = {
        'Crystal Center HXP Z-Axis [mm]': list_alignment_pos,
        }
    df = pandas.DataFrame(dictionary)
    df.to_csv(index=False, path_or_buf = save_path)
    

# Main
ZAxisAlignmentCrystal()