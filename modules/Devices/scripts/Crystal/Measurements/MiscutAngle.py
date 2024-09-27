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

def MiscutAngleMeasurement(plot):
    r"""
    Brief:
    Function used to measure the miscut angle of the crystal. 
    Firstly, the xray sensor data logged during the W-Scan are read and smoothed applying a savgol filter.
    Secondly, the Bragg angle is computed.
    Thirdly, the Bragg angles in "0 orientation" are subtracted to the Bragg angles in "180 deg orientation" and the difference is divided by two.
    The results of this computations are the miscut angles as a function of the position on the hxp y axis.  
    Finally the average of the miscut angles is computed and they are plotted as a function of the hxp y axis positon.
    
    Input Args:
    1. ;
    2. csv filename of the data logged by the x-ray sensor;
    3. path of the .csv file where the result of the miscut angle will be saved;
    4. path where the Bragg peaks and hxp y-axis are saved;
    5. y-axis scan range;
    6. crystal thickness;
    
    Input:
    plot (bool):If true the plots are diplayed.

    Note:
    The function relies on a module called 'moduleFunctions'.
    """
    path_absolute = str(pathlib.Path().absolute()).split("\\")
    path_to_data_dir = path_absolute[0] + "\\" + path_absolute[1] + "\\" + path_absolute[2] + "\\" + path_absolute[3] + "\\" + path_absolute[4]
    logFileName = sys.argv[2]
    path_csv = path_to_data_dir + "\\LogFiles\\DeviceScans\\" + logFileName

    path_result_Miscut_Angle = sys.argv[3]
    path_Peaks = sys.argv[4]
    y_scan_range = sys.argv[5]
    crystal_thickness = sys.argv[6]
    path_csv_0deg = sys.argv[7]
    
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
    hmx = moduleFunctions.half_max_x(x, y)
    xPos = (hmx[1] + hmx[0]) / 2
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
        ax1.legend({"X-Ray Sensor Data": dataframe["X-Ray Sensor Data"],
                    "Smoothed X-Ray Sensor Data": dataframe["X-Ray Sensor Data"]
                    },
                    loc = "lower right",
                    title_fontsize = "1")
        plt.show()

    # Save slopes in a .csv file
    peak = [peakValueOnXPos]
    list_YAxis_position = [float(sys.argv[1])]
    list_W_position = [xPos]
    try:
        dataframe_results = pandas.read_csv(path_Peaks, skiprows=0, sep=",")
        dataframe_results.loc[len(dataframe_results.index)] = [peak[0], list_YAxis_position [0], list_W_position[0]] 
    except:
        dictionary = {
            'Peak Value (au)': peak,
            'Y-Axis Position (mm)': list_YAxis_position,
            'W-Axis Position (deg)': list_W_position
            }
        dataframe_results = pandas.DataFrame(dictionary)
    
    dataframe_results.to_csv(index=False, path_or_buf = path_Peaks)
    
    # --- MiscutAngle ---
    # Load dataframe YW scans in 0deg orientation
    dataframe_0deg = pandas.read_csv(path_csv_0deg, skiprows=0, sep=",")
    print(len(dataframe_results))
    print(len(dataframe_0deg))
    # Compute Miscut angle
    if(len(dataframe_results) == len(dataframe_0deg)):
        miscut_angles = dataframe_0deg['W-Axis Position (deg)'].sub(dataframe_results['W-Axis Position (deg)'], axis = 0)/2
        miscut_angles = miscut_angles.tolist()
        dictionary = {
                'Miscut Angle (urad)': miscut_angles
                }
        miscut_angles = pandas.DataFrame(dictionary)
        # Plot
        a4_dims = (13,8)
        sns.set(font_scale=2)
        sns.set_style("whitegrid")
        fig, (ax1) = plt.subplots(nrows=1, sharey=False, figsize=a4_dims)
        g=sns.scatterplot(x = dataframe_results.iloc[:, 1], y = miscut_angles['Miscut Angle (urad)'], ax = ax1, color = "blue")
        plt.title("Avg Miscut Angle (urad): " + str(miscut_angles['Miscut Angle (urad)'].mean()))
        plt.show()
        # end Plot
        miscut_angles.to_csv(index=False, path_or_buf = path_result_Miscut_Angle.split(".")[0] + "_fullscan.csv")

        miscutAngle_avg = [miscut_angles['Miscut Angle (urad)'].mean()]
        dictionary = {
                'Avg Miscut Angle (urad)': miscutAngle_avg
                }
        dataframe_result_bendingAngle = pandas.DataFrame(dictionary)
        dataframe_result_bendingAngle.to_csv(index=False, path_or_buf = path_result_Miscut_Angle)
        print(dataframe_result_bendingAngle)
# Main
MiscutAngleMeasurement(True)