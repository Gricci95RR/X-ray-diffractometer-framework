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

def XAxisAlignmentCrystal():
    """
    Brief:
    This function aligns the X-axis of the crystal in an X-ray diffraction device.
    It takes the measurement data from an X-ray sensor, performs data smoothing, and finds the 50% of the count rate of the fully opened beam.
    The result is then plotted and saved in a .csv file.

    Input Args:
    sys.argv[1] (string): a string representing the filename of the X-ray sensor data file in the LogFiles/DeviceScans directory.
    sys.argv[2] (string): a string representing the path to the .csv file where the result will be saved.

    Output:
    A .csv file containing the X-axis position of the crystal center, 
    which is the intersection of the 50% of the count rate of the fully opened beam and the X-axis.

    Details:
    1. The function starts by reading the X-ray sensor data from the .csv file specified in the input. 
    2. The data is then smoothed using the savgol_filter() function from the SciPy library, which applies a Savitzky-Golay filter to the data.
    3. The 50% of the count rate of the fully opened beam is calculated as the average of the count rate at the start and end of the measurement data.
    4. This value is then used to find the X-axis position of the crystal center by interpolating the position where the smoothed data intersects the 50% of the count rate value.
    5. The X-ray sensor data and the smoothed data are plotted on a single graph.
       The calculated 50% of the count rate of the fully opened beam is plotted as a horizontal line, and the X-axis position of the crystal center is marked as a vertical line.
       The start and end positions of the measurement data are also marked as vertical lines.
    6. The X-axis position of the crystal center is saved in a .csv file at the location specified in the input.
       The file contains a single column with the title "Crystal Center HXP X-Axis [mm]" and the value of the X-axis position.

    Note:
    The function relies on a module called 'moduleFunctions' for the 'half_max_position_interpolation' function, which is used to find the X-axis position of the crystal center.
    """
    logFileName = sys.argv[1]
    path_absolute = str(pathlib.Path().absolute()).split("\\")
    path_to_data_dir = path_absolute[0] + "\\" + path_absolute[1] + "\\" + path_absolute[2] + "\\" + path_absolute[3] + "\\" + path_absolute[4] 
    path_csv = path_to_data_dir + "\\LogFiles\\DeviceScans\\" + logFileName
    save_path = sys.argv[2]
    dataframe = pandas.read_csv(path_csv, skiprows=0, sep=";")#.drop("Unnamed: 2", axis=1)
    now = moduleFunctions.renameLogFileScan(dataframe, logFileName) # save logFile with date and time

    # Smoothed DF
    dataframe_smoothed = savgol_filter(dataframe["X-Ray Sensor Data"], 7, 2) # Savitzky-Golay filter

    # Avg count rate of the fully opened beam
    start_avg = 0
    end_avg = 5
    mean_count_start = dataframe_smoothed[start_avg:end_avg].mean(axis=0)
    mean_count_end = dataframe_smoothed[-end_avg:].mean(axis=0)
    mean_count_50 = (mean_count_start + mean_count_end) / 2
    
    # Intersection mean_count_50 horizontal line and Counts
    y = dataframe_smoothed
    x = dataframe["HXP X-Axis"].to_numpy()
    pos_mean_count_50 = moduleFunctions.half_max_position_interpolation(x, y, mean_count_start, mean_count_end)
    
    # Plot
    a4_dims = (15, 10)
    sns.set(font_scale = 1.8)
    sns.set_style("whitegrid")
    fig, (ax1) = plt.subplots(nrows=1, sharey=False, figsize=a4_dims)
    g=sns.lineplot(y=dataframe['X-Ray Sensor Data'], x=dataframe['HXP X-Axis'], ax=ax1, color = 'blue', linewidth=2, markers=True, marker='o', sort = False)
    g=sns.lineplot(y=dataframe_smoothed, x=dataframe['HXP X-Axis'], ax=ax1, color = 'orangered', linewidth=2, markers=True, marker='o', sort = False)
    #g.axvline(dataframe.iloc[start_avg, 1], color = "orange")
    #g.axvline(dataframe.iloc[end_avg-1, 1], color = "orange")
    g.axhline(mean_count_start, color = "gold")
    g.axhline(mean_count_50, color = "black")
    g.axvline(pos_mean_count_50, color = "lime")
    g.axhline(mean_count_end, color = "gold")
    
    # Plot settings
    plt.xticks(rotation=0)
    plt.ylabel('Signal Intensity (arb. units)')
    plt.xlabel('Crystal - X Axis Position (mm)')
    plt.title("50% of the count rate of the fully opened beam: " + str(mean_count_50)[:5])
    ax1.legend({"X-Ray Sensor Data": dataframe["X-Ray Sensor Data"],
                "Smoothed X-Ray Sensor Data": dataframe["X-Ray Sensor Data"],
                #"Start": start_avg,
                #"Stop": end_avg,
                "Mean count rate": mean_count_start,
                "50% of the count rate of the fully opened beam": mean_count_50,
                "Alignment Position": pos_mean_count_50},
                loc = "upper left",
                title_fontsize = "1")
    #plt.savefig("X scan" + str(now) + ".png", dpi = 200)
    plt.show()

    # Save result position in a .csv file
    list_pos_mean_count_50 = [pos_mean_count_50]
    dictionary = {
        'Crystal Center HXP X-Axis [mm]': list_pos_mean_count_50,
        }
    df = pandas.DataFrame(dictionary)
    df.to_csv(index=False, path_or_buf = save_path)

# Main
XAxisAlignmentCrystal()