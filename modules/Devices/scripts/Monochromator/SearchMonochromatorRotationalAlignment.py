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

def SearchMonochromatorRotationalAlignment():
    logFileName = sys.argv[2]
    path_absolute = str(pathlib.Path().absolute()).split("\\")
    path_to_data_dir = path_absolute[0] + "\\" + path_absolute[1] + "\\" + path_absolute[2] + "\\" + path_absolute[3] + "\\" + path_absolute[4] 
    path_csv = path_to_data_dir +  "\\LogFiles\\DeviceScans\\" + logFileName
    path_datafile_omega_scans = path_to_data_dir + "\\" + "\\LogFiles\\DeviceScans\\MonochromatorOmegaAxisScans\\alignment_Monochromator_rotational"

    path_results_slopes = sys.argv[3]
    path_results_X_Monochromator = sys.argv[4]
    path_results_Omega_Monochromator = sys.argv[5]
    
    dataframe = pandas.read_csv(path_csv, skiprows=0, sep=";").drop("Unnamed: 2", axis=1)
    moduleFunctions.renameLogFileScan(dataframe, logFileName) # save logFile with date and time
    arg1 = sys.argv[1].split(".")
    #dataframe.to_csv(index=False, path_or_buf = path_datafile_omega_scans + arg1[0] + "_" + arg1[1] + "csv")

    # Smoothed DF
    dataframe_smoothed = savgol_filter(dataframe["X-Ray Sensor Data"], 10, 3) # Savitzky-Golay filter
    dictionary = {
        'X-Ray Sensor Data' : dataframe_smoothed
        }
    dataframe2_smoothed = pandas.DataFrame(dictionary)

     # FWHM
    x=dataframe['Stepper Motor Position'].to_numpy()
    y=dataframe2_smoothed['X-Ray Sensor Data'].to_numpy()
    hmx, max_y, min_y, half_y = moduleFunctions.half_max_x(x, y)
    maxPosition = (hmx[1] + hmx[0]) / 2

    # Linear Fit
    end = 20
    start = 0
    linear_model=np.polyfit(dataframe.iloc[start:end, 1], dataframe_smoothed[start:end] ,1)
    linear_model_fn=np.poly1d(linear_model)

    end2 = maxPosition + 20  # linear fit stop index
    start2 = maxPosition # linear fit start index
    linear_model2=np.polyfit(dataframe.iloc[start2:end2, 1], dataframe_smoothed[start2:end2] ,1)
    linear_model_fn2=np.poly1d(linear_model2)

    # Plot
    a4_dims = (13,8)
    sns.set(font_scale=2)
    sns.set_style("whitegrid")
    fig, (ax1) = plt.subplots(nrows=1, sharey=False, figsize=a4_dims)
    g=sns.lineplot(y=dataframe['X-Ray Sensor Data'], x=dataframe['Stepper Motor Position'], ax=ax1, color = 'blue', linewidth=2, markers=True, marker='o')
    g=sns.lineplot(y=dataframe_smoothed, x=dataframe['Stepper Motor Position'], ax=ax1, color = 'orangered', linewidth=2, markers=True, marker='o')
    g=sns.lineplot(y=linear_model_fn(dataframe.iloc[start:end, 1]), x=dataframe.iloc[start:end,1], ax=ax1, color = 'green', linewidth=2, markers=True, marker='o') # plot linear fit
    g=sns.lineplot(y=linear_model_fn2(dataframe.iloc[start2:end2, 1]), x=dataframe.iloc[start2:end2,1], ax=ax1, color = 'green', linewidth=2, markers=True, marker='o') # plot linear fit
    g.axvline(maxPosition, color = "")
    # Plot settings
    plt.xticks(rotation=0)
    plt.ylabel('Signal Intensity (arb. units)')
    plt.xlabel('Monochromator - Rotational Axis (deg)')
    plt.title("Omega Alignment; Slope 1: " + str(linear_model_fn[1])[:5] + "; Slope 2: " + str(linear_model_fn2[1])[:6])
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
    list_XAxis_position = [sys.argv[1]]
    list_Omega_position = [maxPosition]
    try:
        dataframe_results = pandas.read_csv(path_results_slopes, skiprows=0, sep=",")
        dataframe_results.loc[len(dataframe_results.index)] = [slope1[0], slope2[0], difference_slopes[0], list_XAxis_position [0], list_Omega_position[0]] 
    except:
        dictionary = {
            'Slope1': slope1,
            'Slope2': slope2,
            'Difference Slopes': difference_slopes,
            'X-Axis Position': list_XAxis_position,
            'Omega-Axis Position': list_Omega_position
            }
        dataframe_results = pandas.DataFrame(dictionary)
    
    dataframe_results.to_csv(index=False, path_or_buf = path_results_slopes)

    # Search minimum difference between the slopes
    # X-Axis
    min_slope = dataframe_results['Difference Slopes'].min()
    index_min_position_slope = dataframe_results['Difference Slopes'].idxmin()
    min_position_slope = dataframe_results.iloc[index_min_position_slope, 3]
    list_min_position_slope = [min_position_slope]
    dictionary = {
            'Monochromator Center X-Axis Stepper Motor Position': list_min_position_slope
            }
    dataframe_result_X_position = pandas.DataFrame(dictionary)
    dataframe_result_X_position.to_csv(index=False, path_or_buf = path_results_X_Monochromator)

    # Omega-Axis
    min_position_slope = dataframe_results.iloc[index_min_position_slope, 4]
    list_min_Omega_position_slope = [min_position_slope]
    dictionary = {
            'Monochromator Center Omega-Axis Stepper Motor Position': list_min_Omega_position_slope
            }
    dataframe_result_Omega_position = pandas.DataFrame(dictionary)
    dataframe_result_Omega_position.to_csv(index=False, path_or_buf = path_results_Omega_Monochromator)


# Main
SearchMonochromatorRotationalAlignment()