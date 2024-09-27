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

def SearchYAxisFineCrystalAlignment(plot):
    """
    Brief:
    This function is used to perform Y-axis fine crystal alignment for X-ray diffractometer.

    Input Args:
    1. Y-Axis HXP position.
    2. CSV filename containing the X-ray sensor data.
    3. path of the CSV file where the Y-Axis position is saved.
    4. path of the CSV file where the peaks positions are saved.
    5. number of steps to perform on the Y-Axis during this fine alignment.
    6. fully opened beam value.

    Args:
    plot (bool): A boolean value that determines whether to plot the results or not.

    Details:
    1. Reads the path to the CSV file containing the X-ray sensor data.
    2. Applies a smoothing filter (Savitzky-Golay filter) on the X-ray sensor data in the CSV file.
    3. Calculates the full-width at half-maximum (FWHM) for the smoothed data.
    4. Plots the original X-ray sensor data and the smoothed data if the plot parameter is set to True.
    5. Writes the results (peak value, Y-axis position, and W-axis position) to a CSV file.
    6. Calculates the Y-axis in/out alignment position based on the number of steps and the fully opened beam value.

    Output:
    - 'path_results_Y_Crystal' path of the file where the Y-Axis position is saved.

    Note:
    The function relies on a module called 'moduleFunctions'.
    """
    path_absolute = str(pathlib.Path().absolute()).split("\\")
    path_to_data_dir = path_absolute[0] + "\\" + path_absolute[1] + "\\" + path_absolute[2] + "\\" + path_absolute[3] + "\\" + path_absolute[4]
    logFileName = sys.argv[2]
    path_csv = path_to_data_dir + "\\LogFiles\\DeviceScans\\" + logFileName

    path_results_Y_Crystal = sys.argv[3]
    path_results_Yin_Yout_Crystal = sys.argv[4]
    num_steps_y_axis = int(sys.argv[5][0])
    fully_opened_beam_value = float(sys.argv[6])
    
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
    list_YAxis_position = [sys.argv[1]]
    list_W_position = [xPos]
    try:
        dataframe_results = pandas.read_csv(path_results_Yin_Yout_Crystal, skiprows=0, sep=",")
        dataframe_results.loc[len(dataframe_results.index)] = [peakValueOnXPos, list_YAxis_position [0], list_W_position[0]] 
    except:
        dictionary = {
            'Peak Value in center position (au)': center_y_value,
            'Y-Axis Position (mm)': list_YAxis_position,
            'W-Axis Position (deg)': list_W_position
            }
        dataframe_results = pandas.DataFrame(dictionary)
    
    dataframe_results.to_csv(index=False, path_or_buf = path_results_Yin_Yout_Crystal)
    
    # Yin/out alignment position
    ''' 
    If the length of a given pandas dataframe 'dataframe_results' is greater than or equal to a specified number 'num_steps_y_axis':
    1. creates a new pandas series by subtracting a value ('fully_opened_beam_value/2') from the first column ('Peak Value in center position (au)') of the dataframe.
    2. sorts the new series and retrieves the index of the first element in the sorted series.
    3. uses the retrieved index to obtain the corresponding value from the second column of the original dataframe and saves it in a list.
    4. creates a new pandas dataframe using the list and a predefined column name and saves it to a CSV file at a specified location ('path_results_Y_Crystal').
    '''
    if (len(dataframe_results) >= num_steps_y_axis):
        df_sort = dataframe_results.iloc[:, 0].sub(fully_opened_beam_value/2)
        df_sort = df_sort.argsort()
        index_closest = df_sort.index.tolist()
        index_closest = index_closest[0]
        
        Yin_Yout_pos = [float(dataframe_results.iloc[index_closest, 1])]
        dictionary = {
                'Crystal Yin / Yout Position (mm)': Yin_Yout_pos
                }
        dataframe_result_Y_position = pandas.DataFrame(dictionary)
        dataframe_result_Y_position.to_csv(index=False, path_or_buf = path_results_Y_Crystal)

# Main
SearchYAxisFineCrystalAlignment(True)