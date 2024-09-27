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

def SearchYWAxesCrystalAlignment(plot):
    """
    Brief:
    The function SearchYWAxesCrystalAlignment is used to determine the Y-W axis alignment of a crystal by analyzing X-Ray sensor data.

    Inputs:
    plot (bool): A boolean value that indicates whether a plot should be created to visualize the analysis results.
                 If set to True, the function will create a plot, if set to False, no plot will be created.
    
    Details:
    1. The function reads from a csv file the X-Ray sensor data and filters the data using a Savitzky-Golay filter.
    2. Calculates the FWHM and two linear fits for the filtered data.
       The FWHM is calculated based on the full-width at half-maximum (FWHM) of the filtered data, and the linear fits are calculated for the start and end of the filtered data.
    3. The results are stored in csvs files.

    Outputs:
    The function returns no direct outputs, but it saves the following results to file:
    - path_results_slopes: The slopes of the two linear fits.
    - path_results_Y_Crystal: The Y position of the crystal at the maximum intensity.
    - path_results_std_dev_Crystal: The standard deviation of the crystal intensity.

    Note:
    The function relies on a module called 'moduleFunctions'.
    """
    path_absolute = str(pathlib.Path().absolute()).split("\\")
    path_to_data_dir = path_absolute[0] + "\\" + path_absolute[1] + "\\" + path_absolute[2] + "\\" + path_absolute[3] + "\\" + path_absolute[4]
    logFileName = sys.argv[2]
    path_csv = path_to_data_dir + "\\LogFiles\\DeviceScans\\" + logFileName

    path_results_slopes = sys.argv[5]
    path_results_Y_Crystal = sys.argv[3]
    path_results_std_dev_Crystal = sys.argv[4]
    
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
    
    # Linear Fit
    end = 12
    start = 0
    linear_model=np.polyfit(dataframe.iloc[start:end, 6], dataframe_smoothed[start:end] ,1)
    linear_model_fn=np.poly1d(linear_model)

    end2 = len(dataframe)
    start2 = len(dataframe) - 12
    linear_model2=np.polyfit(dataframe.iloc[start2:end2, 6], dataframe_smoothed[start2:end2] ,1)
    linear_model_fn2=np.poly1d(linear_model2)
    if (plot == True):
        # Plot
        a4_dims = (13,8)
        sns.set(font_scale=2)
        sns.set_style("whitegrid")
        fig, (ax1) = plt.subplots(nrows=1, sharey=False, figsize=a4_dims)
        g=sns.lineplot(y=dataframe['X-Ray Sensor Data'], x=dataframe['HXP W-Axis'], ax=ax1, color = 'blue', linewidth=2, markers=True, marker='o')
        g=sns.lineplot(y=dataframe_smoothed, x=dataframe['HXP W-Axis'], ax=ax1, color = 'orangered', linewidth=2, markers=True, marker='o')
        g=sns.lineplot(y=linear_model_fn(dataframe.iloc[start:end, 6]), x=dataframe.iloc[start:end, 6], ax=ax1, color = 'green', linewidth=2, markers=True, marker='o') # plot linear fit
        g=sns.lineplot(y=linear_model_fn2(dataframe.iloc[start2:end2, 6]), x=dataframe.iloc[start2:end2, 6], ax=ax1, color = 'green', linewidth=2, markers=True, marker='o') # plot linear fit
        g.axvline(xPos, color = "lime")
        # Plot settings
        plt.xticks(rotation=0)
        plt.ylabel('Signal Intensity (arb. units)')
        plt.xlabel('Crystal - W Axis (deg)')
        plt.title("W Alignment; Slope 1: " + str(linear_model_fn[1])[:5] + "; Slope 2: " + str(linear_model_fn2[1])[:6])
        ax1.legend({"X-Ray Sensor Data": dataframe["X-Ray Sensor Data"],
                    "Smoothed X-Ray Sensor Data": dataframe["X-Ray Sensor Data"]
                    },
                    loc = "lower right",
                    title_fontsize = "1")
        plt.show()

    # Save slopes in a .csv file
    slope1 = [linear_model_fn[1]]
    slope2 = [linear_model_fn2[1]]
    difference_slopes = [abs(linear_model_fn[1] + linear_model_fn2[1])]
    list_YAxis_position = [sys.argv[1]]
    list_W_position = [xPos]
    try:
        dataframe_results = pandas.read_csv(path_results_slopes, skiprows=0, sep=",")
        dataframe_results.loc[len(dataframe_results.index)] = [slope1[0], slope2[0], difference_slopes[0], list_YAxis_position [0], list_W_position[0]] 
    except:
        dictionary = {
            'Slope1': slope1,
            'Slope2': slope2,
            'Difference Slopes': difference_slopes,
            'Y-Axis Position (mm)': list_YAxis_position,
            'W-Axis Position (deg)': list_W_position
            }
        dataframe_results = pandas.DataFrame(dictionary)
    
    dataframe_results.to_csv(index=False, path_or_buf = path_results_slopes)
    # Index List of Non-Outliers
    nonOutlierList = moduleFunctions.Remove_Outlier_Indices(dataframe_results)
    # Non-Outlier Subset of the Given Dataset
    dataframe_results = dataframe_results[nonOutlierList]
    std_dev_diff_slopes = dataframe_results['Difference Slopes'].std()
    # Plot density
    if (plot == True):
        sns.set(font_scale=2)
        sns.displot(x=dataframe_results["Difference Slopes"], kde=True, height = 8, aspect = 2)
        plt.title("Standard Deviation of Slopes Differences: " + str(std_dev_diff_slopes)[:5])
        plt.xticks(rotation=0)
        plt.show()

    # Search minimum difference between the slopes
    # X-Axis
    index_min_position_slope = dataframe_results['Difference Slopes'].idxmin()
    min_position_slope = dataframe_results.iloc[index_min_position_slope, 3]
    list_min_position_slope = [min_position_slope]
    dictionary = {
            'Crystal Y-Axis Position (mm)': list_min_position_slope
            }
    dataframe_result_Y_position = pandas.DataFrame(dictionary)
    dataframe_result_Y_position.to_csv(index=False, path_or_buf = path_results_Y_Crystal)

    # STD Deviation
    standard_deviation_list = [std_dev_diff_slopes]
    dictionary = {
            'Standard Deviation of slopes': standard_deviation_list
            }
    dataframe_result_std_dev = pandas.DataFrame(dictionary)
    dataframe_result_std_dev.to_csv(index=False, path_or_buf = path_results_std_dev_Crystal)

# Main
SearchYWAxesCrystalAlignment(False)