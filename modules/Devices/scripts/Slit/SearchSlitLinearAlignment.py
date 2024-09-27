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

def searchSlitAlignment(plotFlag):
    """
    Brief:
    Searches for the optimal alignment of the slit. The alignment positions (for the linear and rotational axis) correspond to the maximum FWHM.

    Args:
    sys.argv[1] (string): rotational position of the slit.
    sys.argv[2] (string): filename of the x-ray sensor data.
    'path_results_xPos_Slit' (string): path of the location where to save the alignment position (linear axis) of the slit.
    'path_results_RotPos_Slit' (string): path of the location where to save the alignment position (rotational axis) of the slit.
    'path_results_slit_alignment' (string): path of the location where to save the results of the FWHM, rotational positions and linear positions of the slit during the alignment.

    Input:
    plot (bool):If true the plots are diplayed.
    """

    logFileName = sys.argv[2]
    path_absolute = str(pathlib.Path().absolute()).split("\\")
    path_to_data_dir = path_absolute[0] + "\\" + path_absolute[1] + "\\" + path_absolute[2] + "\\" + path_absolute[3] + "\\" + path_absolute[4] 
    path_csv = path_to_data_dir + "\\LogFiles\\DeviceScans\\" + logFileName
    path_results_xPos_Slit = sys.argv[3]
    path_results_RotPos_Slit = sys.argv[4]
    path_results_slit_alignment = sys.argv[5]
    path_datafile_xaxis_scans = path_to_data_dir + "\\LogFiles\\DeviceScans\\SlitAlignmentXAxisScans\\alignment_XAxis_Slit_scan"
    dataframe = pandas.read_csv(path_csv, skiprows=0, sep=";").drop("Unnamed: 2", axis=1)
    arg1 = sys.argv[1].split(".")
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
    xPos = (hmx1 + hmx0) / 2
    rotPos = sys.argv[1]
    fwhm = abs(hmx1 - hmx0)

    # Plot
    if (plotFlag == True):
        a4_dims = (13,8)
        sns.set(font_scale=2)
        sns.set_style("whitegrid")
        fig, (ax1) = plt.subplots(nrows=1, sharey=False, figsize=a4_dims)
        g1=sns.lineplot(y=dataframe['X-Ray Sensor Data'], x=dataframe['Stepper Motor Position'], ax=ax1, color = 'blue', linewidth=2, markers=True, marker='o', sort=False)
        g=sns.lineplot(y=dataframe_smoothed['X-Ray Sensor Data'], x=dataframe['Stepper Motor Position'], ax=ax1, color = 'orangered', linewidth=2, markers=True, marker='o', sort=False)
        g.axvline(xPos, color = "lime")
        g.axhline(max_y, color = "black")
        g.axhline(half_y, color = "black")
        g.axvline(hmx0, color = "black")
        g.axvline(hmx1, color = "black")

        # Plot settings
        plt.xticks(rotation=0)
        plt.ylabel('Signal Intensity (arb. units)')
        plt.xlabel('Slit - Linear Axis Position (mm)')
        plt.title("FWHM: " + str(fwhm)[:5])
        ax1.legend({"X-Ray Sensor Data": dataframe["X-Ray Sensor Data"],
                    "Smoothed X-Ray Sensor Data": dataframe["X-Ray Sensor Data"]},
                    loc = "lower right",
                    title_fontsize = "1")
        
        plt.show()
    
    # Save result in a .csv file
    list_fwhm = [fwhm]
    list_xPos = [xPos]
    list_rotPos = [rotPos]
    try:
        dataframe_results = pandas.read_csv(path_results_slit_alignment, skiprows=0, sep=",")
        dataframe_results.loc[len(dataframe_results.index)] = [fwhm, xPos, rotPos] 
    except:
        dictionary = {
            'FWHM': list_fwhm,
            'X-Axis Position': list_xPos,
            'Rotational-Axis Position': list_rotPos
            }
        dataframe_results = pandas.DataFrame(dictionary)
    dataframe_results.to_csv(index=False, path_or_buf = path_results_slit_alignment)

    # X-Axis
    index_position_fwhm_max = dataframe_results['FWHM'].idxmax()
    position_fwhm_max = dataframe_results.iloc[index_position_fwhm_max, 1]
    list_position_fwhm_max = [position_fwhm_max]
    dictionary = {
            'Slit Center X-Axis Stepper Motor Position': list_position_fwhm_max
            }
    dataframe_result_X_position = pandas.DataFrame(dictionary)
    dataframe_result_X_position.to_csv(index=False, path_or_buf = path_results_xPos_Slit)

    # Rotational-Axis
    rot_position_fwhm_max = dataframe_results.iloc[index_position_fwhm_max, 2]
    list_rot_position_fwhm_max = [rot_position_fwhm_max]
    dictionary = {
            'Slit Center Rotational-Axis Stepper Motor Position': list_rot_position_fwhm_max
            }
    dataframe_result_Rotational_position = pandas.DataFrame(dictionary)
    dataframe_result_Rotational_position.to_csv(index=False, path_or_buf = path_results_RotPos_Slit)


# Main
searchSlitAlignment(True)