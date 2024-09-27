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

def BendingAngleMeasurement(plot):
    r"""
    Brief:
    Function used to measure the bending angle of the crystal. 
    Firstly, the xray sensor data logged during the W-Scan are read and smoothed applying a savgol filter.
    Secondly, the Bragg angle is computed.
    Thirdly, if more than 2 movement on the Y-axis are perfomed the Bragg angles are plotted as a function of the horizontal impact points and a linear fit is applied.
    The slope of the linear fit is multiplied by the crystal thickness and the result is the bending angle.
    
    Input Args:
    1. Y-Axis Hexapod position.;
    2. csv filename of the data logged by the x-ray sensor;
    3. path of the .csv file where the result of the bending angle will be saved;
    4. path where the Bragg peaks and hxp y-axis are saved;
    5. y-axis scan range;
    6. crystal thickness;
    
    Input:
    plot (bool): If true the plots are diplayed.

    Note:
    The function relies on a module called 'moduleFunctions'.
    """
    path_absolute = str(pathlib.Path().absolute()).split("\\")
    path_to_data_dir = path_absolute[0] + "\\" + path_absolute[1] + "\\" + path_absolute[2] + "\\" + path_absolute[3] + "\\" + path_absolute[4]
    logFileName = sys.argv[2]
    path_csv = path_to_data_dir + "\\LogFiles\\DeviceScans\\" + logFileName

    path_result_Bending_Angle = sys.argv[3]
    path_Peaks = sys.argv[4]
    y_scan_range = sys.argv[5]
    crystal_thickness = float(sys.argv[6])
    
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
        #plt.title("W Alignment; Slope 1: " + str(linear_model_fn[1])[:5] + "; Slope 2: " + str(linear_model_fn2[1])[:6])
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
    
    # BendingAngle
    if(len(dataframe_results) > 2):
        print(dataframe_results)
        linear_model  = np.polyfit(dataframe_results.iloc[:, 1], dataframe_results.iloc[:, 2], 1)
        linear_model_fn = np.poly1d(linear_model)
        slope = float(linear_model_fn[1])
        if (plot == True):
            # Plot
            a4_dims = (13,8)
            sns.set(font_scale=2)
            sns.set_style("whitegrid")
            fig, (ax1) = plt.subplots(nrows=1, sharey=False, figsize=a4_dims)
            g=sns.scatterplot(x = dataframe_results.iloc[:, 1],
                              y = dataframe_results.iloc[:, 2],
                              ax = ax1,
                              color = "blue")
            g2=sns.lineplot(y = linear_model_fn(dataframe_results.iloc[:, 1]),
                            x = dataframe_results.iloc[:, 1],
                            ax=ax1,
                            color = 'green',
                            linewidth=2,
                            markers=True,
                            marker='o') # plot linear fit
            plt.title("Slope: " + str(slope))
            plt.show()
            # end Plot

        bendingAngle = [slope * crystal_thickness]
        dictionary = {
                'Bending Angle (urad)': bendingAngle
                }
        dataframe_result_bendingAngle = pandas.DataFrame(dictionary)
        dataframe_result_bendingAngle.to_csv(index=False, path_or_buf = path_result_Bending_Angle)

# Main
BendingAngleMeasurement(True)