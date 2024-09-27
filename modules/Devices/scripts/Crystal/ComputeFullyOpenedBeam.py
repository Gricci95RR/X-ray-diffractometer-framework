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

def ComputeFullyOpenedBeamValue(plot):
    """
    This function computes the fully opened beam value by reading the X-Ray sensor data from a CSV file and plotting it
    after smoothing it using Savitzky-Golay filter. The center of the beam is computed using the FWHM, and the peak value
    at this center is extracted. The results are saved in a CSV file and can be plotted if desired.

    Parameters:
    plot (bool): Whether to plot the X-Ray sensor data and the smoothed X-Ray sensor data.

    Args:
    sys.argv[1] (string): filename of the input .csv file xray sensor data
    'path_result_Fully_Opened_Beam' (string): Path of the location where to save the result of the peak intensity of the signal analyzed.

    Returns:
    None

    Outputs:
    A CSV file containing the peak value of the X-ray beam intensity in the log file. The name of the CSV file is specified in path_result_Fully_Opened_Beam.
    
    Details:
    The function performs the following steps:
    1. Reads the log file specified in csv_filename using the pandas.read_csv function.
    2. Applies a Savitzky-Golay filter to the X-ray sensor data to smooth the data.
    3. Computes the Full-Width-Half-Maximum (FWHM) of the smoothed X-ray data by finding the half-maximum x positions and computing the x-position at the center of the FWHM.
    4. Extracts the peak value of the X-ray beam intensity at the x-position computed in step 3.
    5. If plot=True, plots the X-ray sensor data and its smoothed version with a vertical line indicating the x-position of the peak value.
    6. Stores the peak value in a CSV file specified in path_result_Fully_Opened_Beam.

    Note:
    The function relies on a module called 'moduleFunctions'.
    """
    logFileName = sys.argv[1]
    path_absolute = str(pathlib.Path().absolute()).split("\\")
    path_to_data_dir = path_absolute[0] + "\\" + path_absolute[1] + "\\" + path_absolute[2] + "\\" + path_absolute[3] + "\\" + path_absolute[4]
    path_csv = path_to_data_dir + "\\LogFiles\\DeviceScans\\" + logFileName

    path_result_Fully_Opened_Beam = sys.argv[2]
    
    dataframe = pandas.read_csv(path_csv, skiprows=0, sep=";")
    moduleFunctions.renameLogFileScan(dataframe, logFileName) # save logFile with date and time
    
    # Smoothed DF
    dataframe_smoothed = savgol_filter(dataframe["X-Ray Sensor Data"], 10, 3) # Savitzky-Golay filter
    dictionary = {
        'X-Ray Sensor Data' : dataframe_smoothed
        }
    dataframe2_smoothed = pandas.DataFrame(dictionary)

    # FWHM
    x=dataframe['HXP W-Axis'].to_numpy()
    y=dataframe2_smoothed['X-Ray Sensor Data'].to_numpy()
    hmx0, hmx1, max_y, min_y, half_y = moduleFunctions.half_max_x(x, y)
    xPos = (hmx1 + hmx0) / 2
    peakValueOnXPos = moduleFunctions.searchPeakPosition(x, y, xPos)
  
    if (plot == True):
        # Plot
        a4_dims = (13,8)
        sns.set(font_scale=2)
        sns.set_style("whitegrid")
        fig, (ax1) = plt.subplots(nrows=1, sharey=False, figsize=a4_dims)
        g=sns.lineplot(y=dataframe['X-Ray Sensor Data'], x=dataframe['HXP W-Axis'], ax=ax1, color = 'blue', linewidth=2, markers=True, marker='o')
        g=sns.lineplot(y=dataframe_smoothed, x=dataframe['HXP W-Axis'], ax=ax1, color = 'orangered', linewidth=2, markers=True, marker='o')
        
        g.axvline(xPos, color = "lime")
        # Plot settings
        plt.xticks(rotation=0)
        plt.ylabel('Signal Intensity (arb. units)')
        plt.xlabel('Crystal - W Axis (deg)')
        plt.title(" ")
        ax1.legend({"X-Ray Sensor Data": dataframe["X-Ray Sensor Data"],
                    "Smoothed X-Ray Sensor Data": dataframe["X-Ray Sensor Data"]
                    },
                    loc = "lower right",
                    title_fontsize = "1")
        plt.show()

    # Save Results in a .csv file
    center_y_value = [peakValueOnXPos]
    try:
        dataframe_results = pandas.read_csv(path_result_Fully_Opened_Beam, skiprows=0, sep=",")
        dataframe_results.loc[len(dataframe_results.index)] = [peakValueOnXPos] 
    except:
        dictionary = {
            'Fully Opened Beam Value (au)': center_y_value
            }
        dataframe_results = pandas.DataFrame(dictionary)
    
    dataframe_results.to_csv(index=False, path_or_buf = path_result_Fully_Opened_Beam)

# Main
ComputeFullyOpenedBeamValue(True)