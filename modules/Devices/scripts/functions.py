import numpy as np
import datetime

def lin_interp(x, y, i, half):
    """
    Brief
    --------
    Finds the x-axis interpolation point between a function and an horizontal line.
    
    Parameters
    --------
    x (numpy ndarray): The x-axis values of the signal.
    y (numpy ndarray): The y-axis values of the signal.
    i (int): The intersection index.
    half (float): The value of the horizontal line.
    
    Returns
    --------
    float: The x-axis interpolation point between the y values and the horizontal line.
    
    The function uses linear interpolation to estimate the x-axis value of the point where the y values intersect the horizontal line.
    """
    return x[i] + (x[i+1] - x[i]) * ((half - y[i]) / (y[i+1] - y[i]))

def compute_linear_fit(x, y):
    # Ensure x and y are numpy arrays
    x = np.asarray(x)
    y = np.asarray(y)
    
    # Compute the coefficients of the linear fit
    # Using np.polyfit to fit a 1st degree polynomial (linear fit)
    coefficients = np.polyfit(x, y, 1)
    
    # coefficients[0] is the slope, coefficients[1] is the intercept
    slope = coefficients[0]
    intercept = coefficients[1]
    
    # Return the slope and the linear fit coefficients

    x_fit = np.linspace(x.min(), x.max(), 100)
    y_fit = np.polyval(coefficients, x_fit)

    return slope, coefficients, x_fit, y_fit

def lin_interp_y(x, y, i, half):
    """
    Brief
    --------
    Finds the y-axis interpolation point between a function and a vertical line.
    
    Parameters
    --------
    x (numpy ndarray): The x-axis values of the signal.
    y (numpy ndarray): The y-axis values of the signal.
    i (int): The intersection index.
    half (float): The value of the vertical line.
    
    Returns
    --------
    float: The y-axis interpolation point between the y values and the vertical line.
    
    The function uses linear interpolation to estimate the y-axis value of the point where the y values intersect the vertical line.
    """
    return y[i] + (y[i+1] - y[i]) * ((half - x[i]) / (x[i+1] - x[i]))

def half_max_x(x, y):
    """
    Brief
    --------
    Function used to find interserction points (x-axis values) between a function and an horizontal line. 
    The horizontal line is the middle value between min e max of the signal on the y axis (half = (max_y + min_y) / 2).
    
    Input Parameters
    --------
    x (numpy ndarray): x-axis values of the signal (NumPy ndarray).
    y (numpy ndarray): y-axis values of the signal (NumPy ndarray).

    Returns
    --------
    float: first x-axis intersection point between y and horizontal line.
    float: second x-axis intersection point between y and horizontal line.
    float: max of the signal on the y axis.
    float: min of the signal on the y axis.
    float: middle value between min e max of the signal on the y axis.
    """
    max_y = max(y)
    min_y = min(y)
    half = (max_y + min_y) / 2
    #half = max(y)/2.0
    signs = np.sign(np.add(y, -half))
    zero_crossings = (signs[0:-2] != signs[1:-1])
    zero_crossings_i = np.where(zero_crossings)[0]
    return [lin_interp(x, y, zero_crossings_i[0], half),
            lin_interp(x, y, zero_crossings_i[1], half),
            max_y,
            min_y,
            half]

def searchPeakPosition(x, y, xPos):
    """
    Brief
    --------
    Function used to find interserction points (y-axis values) between a function and a vertical line. 
    
    Input Parameters
    --------
    x : x-axis values of the signal (NumPy ndarray).
    y : y-axis values of the signal (NumPy ndarray).
    xPos : vertical line.

    Returns
    --------
    y-axis intersection point between y and vertical line.
    """
    signs = np.sign(np.add(x, -xPos))
    zero_crossings = (signs[0:-2] != signs[1:-1])
    zero_crossings_i = np.where(zero_crossings)[0]
    value = lin_interp_y(x, y, zero_crossings_i[0], xPos)
    return value

def half_max_position_interpolation(x, y, mean_count_start, mean_count_end):
    r"""
    Brief
    --------
    Finds the position of the half-max of a signal using linear interpolation.
    
    Parameters
    --------
    x (numpy ndarray): The x-axis values of the signal.
    y (numpy ndarray): The y-axis values of the signal.
    mean_count_start (float): The start of the mean count of the signal.
    mean_count_end (float): The end of the mean count of the signal.
    
    Returns
    --------
    float: The x-axis position of the half-max of the signal.
    
    The function calculates the half-max of the signal by taking the average of the mean count start and end, and then uses linear interpolation to estimate the x-axis position of the half-max.
    """
    half = (mean_count_start + mean_count_end) /2
    signs = np.sign(np.add(y, -half))
    zero_crossings = (signs[0:-2] != signs[1:-1])
    zero_crossings_i = np.where(zero_crossings)[0]
    return lin_interp(x, y, zero_crossings_i[0], half)

def Remove_Outlier_Indices(df):
    """
    Brief
    --------
    This function removes outlier indices from a pandas DataFrame based on the interquartile range (IQR) method.
    
    Parameters
    --------
    df (pandas DataFrame): The input DataFrame that you want to remove the outlier indices from.
    
    Returns
    --------
    trueList (pandas DataFrame): A DataFrame containing True/False values for each row, indicating whether the row is an outlier or not. Rows with True values are considered inliers, while rows with False values are considered outliers.
    
    The function uses the quantile method from the pandas DataFrame to calculate the first quartile (Q1) and third quartile (Q3) of the input DataFrame.
    The interquartile range (IQR) is calculated as Q3 - Q1, and outliers are identified as values that are less than (Q1 - 1.5 * IQR) or greater than (Q3 + 1.5 * IQR).
    These outliers are then excluded from the final output.
    """
    Q1 = df.quantile(0.25)
    Q3 = df.quantile(0.75)
    IQR = Q3 - Q1
    trueList = ~((df < (Q1 - 1.5 * IQR)) | (df > (Q3 + 1.5 * IQR)))
    return trueList

def peak(x, c):
    """
    Brief
    --------
    Calculates the value of a Gaussian peak at a given x-axis position.
    
    Parameters
    --------
    x (numpy ndarray): The x-axis values of the signal.
    c (float): The center position of the Gaussian peak.
    
    Returns
    --------
    numpy ndarray: The y-axis values of the Gaussian peak at the given x-axis position.
    
    The function calculates the y-axis values of a Gaussian peak centered at `c` for each x-axis position in `x`.
    """
    return np.exp(-np.power(x - c, 2) / 16.0)

def renameLogFileScan(dataframe, logFileName):
    """
    Brief
    --------
    Renames the log file adding current date and time.
    
    Parameters
    --------
    dataframe (pandas dataframe): dataframe with the x-ray sensor and positions logged.
    filename (string)           : name of the file.
    """
    now = datetime.datetime.now()
    now = str(now.strftime('_%Hh_%Mm_%Ss_%A_%B_%dth_%Y'))
    logFileName = logFileName[:-4] + now + logFileName[-4:]
    dataframe.to_csv("C:\\Users\\giricci\\Desktop\\XRay_Machine\\LogFiles\\DeviceScans\\" + logFileName, index = False)
    return now