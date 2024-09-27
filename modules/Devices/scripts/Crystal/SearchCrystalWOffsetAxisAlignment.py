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

def YScanLinearFit():
    """
    Brief:
    

    Args:
    sys.argv[1] (string): filename of the input .csv file xray sensor data
    'save_path' (string): Path of the location where to save the result of the Bragg's Peak intensity and angle (W-Axis).
    sys.argv[3] (string): Position of the W-axis of the hexapod.
    sys.argv[4] (string): Position of the X-axis of the hexapod.

    Returns:
    float: The calculated offset to apply to the W axis in degrees.

    Outputs:
    A CSV file containing the calculated offset angle in degrees.

    Note:
    The function relies on a module called 'moduleFunctions'.
    """
    logFileName = sys.argv[1]
    position_Waxis = sys.argv[3]
    position_Xaxis = sys.argv[4]
    path_absolute = str(pathlib.Path().absolute()).split("\\")
    path_to_data_dir = path_absolute[0] + "\\" + path_absolute[1] + "\\" + path_absolute[2] + "\\" + path_absolute[3] + "\\" + path_absolute[4] 
    path_csv = path_to_data_dir + "\\LogFiles\\DeviceScans\\" + logFileName
    save_path = sys.argv[2]
    save_path_W_position = sys.argv[6]
    save_path_X_position = sys.argv[7]
    dataframe = pandas.read_csv(path_csv, skiprows=0, sep=";")

    now = moduleFunctions.renameLogFileScan(dataframe, logFileName) # save logFile with date and time
    # Print Informations about Smoothed DF
    dataframe_smoothed = savgol_filter(dataframe["X-Ray Sensor Data"], 10, 3) # Savitzky-Golay filter
    dictionary = {
        'X-Ray Sensor Data' : dataframe_smoothed
        }
    dataframe_smoothed = pandas.DataFrame(dictionary)

    # FWHM
    x=dataframe['HXP Y-Axis'].to_numpy()
    y=dataframe_smoothed['X-Ray Sensor Data'].to_numpy()
    slope, coefficients, x_fit, y_fit  = moduleFunctions.compute_linear_fit(x, y)
    
    
    # Plot
    a4_dims = (13,8)
    sns.set(font_scale=2)
    sns.set_style("whitegrid")
    fig, (ax1) = plt.subplots(nrows=1, sharey=False, figsize=a4_dims)
    g1=sns.lineplot(y=dataframe['X-Ray Sensor Data'], x=dataframe['HXP Y-Axis'], ax=ax1, color = 'blue', linewidth=2, markers=True, marker='o')
    g=sns.lineplot(y=dataframe_smoothed['X-Ray Sensor Data'], x=dataframe['HXP Y-Axis'], ax=ax1, color = 'orangered', linewidth=2, markers=True, marker='o')
    plt.plot(x_fit, y_fit, color='lime', label='Linear Fit')

    # Plot settings
    plt.xticks(rotation=0)
    plt.ylabel('Signal Intensity (arb. units)')
    plt.xlabel('Crystal - Y Axis Position (mm)')
    plt.title("Slope of the linear fit: " + str(slope))
    ax1.legend({"X-Ray Sensor Data": dataframe["X-Ray Sensor Data"],
                "Smoothed X-Ray Sensor Data": dataframe["X-Ray Sensor Data"]},
                loc = "lower right",
                title_fontsize = "1")
    #plt.savefig("Y scan_Position W axis_ " + str(position_Waxis) + " Position X axis_ " + str(position_Xaxis) + str(now) + ".png", dpi = 200)
    plt.show()

    # Save in a file
    list_slopes = [slope]
    list_w_positions = [position_Waxis]
    list_x_positions = [position_Xaxis]
    try:
        dataframe_results = pandas.read_csv(save_path, skiprows=0, sep=",")
        dataframe_results.loc[len(dataframe_results.index)] = [list_slopes[0], list_w_positions[0], list_x_positions[0]] 
    except:
        dictionary = {
        'Slope of the linear fit': list_slopes,
        'W-Axis Position (deg)': list_w_positions,
        'X-Axis Position (mm)': list_x_positions,
        }
        dataframe_results = pandas.DataFrame(dictionary)

    # Find the row with the minimum value in the "Slope of the linear fit" column
    min_row = dataframe_results.loc[dataframe_results["Slope of the linear fit"].idxmin()]
    # Get the corresponding "W-Axis Position (deg)"
    min_w_axis_position = min_row["W-Axis Position (deg)"]
    min_x_axis_position = min_row["X-Axis Position (mm)"]

    # Save .csv
    dataframe_results.to_csv(index=False, path_or_buf = save_path)
    # Save .csv result W Position
    df_min_w_axis = pandas.DataFrame({
        "Min W-Axis Position (deg)": [min_w_axis_position]
    })
    # Save the DataFrame to a CSV file
    df_min_w_axis.to_csv(save_path_W_position, index=False)
    # Save .csv result X Position
    df_min_x_axis = pandas.DataFrame({
        "Min X-Axis Position (mm)": [min_x_axis_position]
    })
    # Save the DataFrame to a CSV file
    df_min_x_axis.to_csv(save_path_X_position, index=False)

# Main
YScanLinearFit()