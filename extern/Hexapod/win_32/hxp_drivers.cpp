/*
 * Created source file HXP_drivers.cpp for API description 
 */


#include <stdio.h> 
#include <stdlib.h> 
#include <stdarg.h> 
#include <string.h> 
#include "Socket.h" 

#ifdef _WIN32
	#define DLL _declspec(dllexport)
	#include "strtok_r.h"
#else
	#define DLL 
#endif

#include "HXP_drivers.h" 



#define SIZE_SMALL 1023
#define SIZE_NOMINAL 1023
#define SIZE_BIG 2047
#define SIZE_HUGE 65535

#define SIZE_EXECUTE_METHOD 1023

#define SIZE_NAME    100
#ifdef __cplusplus
extern "C"
{
#else
#typedef int bool;  /* C does not know bool, only C++ */
#endif



#define DLL_VERSION "Library version for HXP Firmware V3.1.x"

/************************************************************************* 
* Replace 'oldChar' by 'newChar' only between the startChar and endChar 
*************************************************************************/ 
void ReplaceCharacter (char *strSourceInOut, char oldChar, char newChar, char startChar, char endChar)
{
	char *pt;
	char *ptNext;
	ptNext = strSourceInOut;
	do 
	{ 
		pt = strchr(ptNext, startChar); 
		if (pt != NULL) 
		{
			*pt++;
			while ((pt != NULL) && (*pt != endChar))
			{
				if (*pt == oldChar)
					*pt = newChar;
				pt++;
			}
			ptNext = pt++;
		}
	}
	while ((pt != NULL) && (ptNext != NULL));
}

/*************************************************************************
* Delete space and tabulation characters between 'startChar' and 'endChar' 
*************************************************************************/ 
void CleanString (char *strSourceInOut, char startChar, char endChar)
{
	int len = 0;
	int startIndex = 0;
	int endIndex = 0;
	int outputIndex = 0;
	char outputString[SIZE_NOMINAL+1];
	len = strlen(strSourceInOut);
	do
	{
		while ((strSourceInOut[startIndex] != startChar) && (startIndex < len) && (outputIndex < SIZE_NOMINAL))
		{
			outputString[outputIndex] = strSourceInOut[startIndex];
			outputIndex++;
			startIndex++;
		}
		while ((strSourceInOut[endIndex] != endChar) && (endIndex < len))
			endIndex++;
		if ((startIndex != endIndex) && (startIndex < len))
		{
			for (int i = startIndex; (i <= endIndex) && (outputIndex < SIZE_NOMINAL); i++)
			{
				if ((strSourceInOut[i] != ' ') && (strSourceInOut[i] != '\t'))
				{
					outputString[outputIndex] = strSourceInOut[i];
					outputIndex++;
				}
			}
			endIndex++;
			startIndex = endIndex;
		}
	}
	while (startIndex < len);
	outputString[outputIndex] = '\0';
	strcpy (strSourceInOut, outputString);
}

/*************************************************************************
* Delete a specified characters 
*************************************************************************/ 
void DeleteCharacters (char *strSourceInOut, char *charactersToDelete)
{
	int len = 0;
	int nbChar = 0;
	int outputIndex = 0;
	bool bCopy;
	char outputString[SIZE_NOMINAL+1];
	len = strlen(strSourceInOut);
	nbChar = strlen(charactersToDelete);
	for (int i = 0; (i <= len) && (outputIndex < SIZE_NOMINAL); i++)
	{
		bCopy = true;
		for (int j = 0; (j < nbChar) && (bCopy == true); j++)
		{
			if (strSourceInOut[i] == charactersToDelete[j])
				bCopy = false;
		}
		if (bCopy)
		{
			outputString[outputIndex] = strSourceInOut[i];
			outputIndex++;
		}
	}
	outputString[outputIndex] = '\0';
	strcpy (strSourceInOut, outputString);
}

/***********************************************************************/
int __stdcall TCP_ConnectToServer(char *Ip_Address, int Ip_Port, double TimeOut)
{
	return (ConnectToServer(Ip_Address, Ip_Port, TimeOut));
}
/***********************************************************************/
void __stdcall TCP_SetTimeout(int SocketIndex, double Timeout) 
{
	SetTCPTimeout(SocketIndex, Timeout); 
}
/***********************************************************************/
void __stdcall TCP_CloseSocket(int SocketIndex) 
{
	CloseSocket(SocketIndex); 
}
/***********************************************************************/
char * __stdcall TCP_GetError(int SocketIndex) 
{
	return (GetError(SocketIndex));
}
/***********************************************************************/
char * __stdcall GetLibraryVersion(void) 
{
	return (DLL_VERSION);
}

/*********************************************************************** 
 * ControllerMotionKernelTimeLoadGet :  Get controller motion kernel time load
 *
 *     - Parameters :
 *            int SocketIndex
 *            double *CPUTotalLoadRatio
 *            double *CPUCorrectorLoadRatio
 *            double *CPUProfilerLoadRatio
 *            double *CPUServitudesLoadRatio
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall ControllerMotionKernelTimeLoadGet (int SocketIndex, double * CPUTotalLoadRatio, double * CPUCorrectorLoadRatio, double * CPUProfilerLoadRatio, double * CPUServitudesLoadRatio) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "ControllerMotionKernelTimeLoadGet (double *,double *,double *,double *)");

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (ret == 0) 
	{ 
		char * pt;
		char * ptNext;

		pt = ReturnedValue;
		ptNext = NULL;
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", CPUTotalLoadRatio);
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", CPUCorrectorLoadRatio);
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", CPUProfilerLoadRatio);
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", CPUServitudesLoadRatio);
	} 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * ElapsedTimeGet :  Return elapsed time from controller power on
 *
 *     - Parameters :
 *            int SocketIndex
 *            double *ElapsedTime
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall ElapsedTimeGet (int SocketIndex, double * ElapsedTime) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "ElapsedTimeGet (double *)");

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (ret == 0) 
	{ 
		char * pt;
		char * ptNext;

		pt = ReturnedValue;
		ptNext = NULL;
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", ElapsedTime);
	} 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * ErrorStringGet :  Return the error string corresponding to the error code
 *
 *     - Parameters :
 *            int SocketIndex
 *            int ErrorCode
 *            char *ErrorString
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall ErrorStringGet (int SocketIndex, int ErrorCode, char * ErrorString) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "ErrorStringGet (%d,char *)", ErrorCode);

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (ret == 0) 
	{ 
		char * pt;
		char * ptNext;

		pt = ReturnedValue;
		ptNext = NULL;
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) strcpy (ErrorString, pt);
		ptNext = strchr (ErrorString, ',');
		if (ptNext != NULL) *ptNext = '\0';
	} 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * FirmwareVersionGet :  Return firmware version
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *Version
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall FirmwareVersionGet (int SocketIndex, char * Version) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "FirmwareVersionGet (char *)");

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (ret == 0) 
	{ 
		char * pt;
		char * ptNext;

		pt = ReturnedValue;
		ptNext = NULL;
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) strcpy (Version, pt);
		ptNext = strchr (Version, ',');
		if (ptNext != NULL) *ptNext = '\0';
	} 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * TCLScriptExecute :  Execute a TCL script from a TCL file
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *TCLFileName
 *            char *TaskName
 *            char *ParametersList
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall TCLScriptExecute (int SocketIndex, char * TCLFileName, char * TaskName, char * ParametersList) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "TCLScriptExecute (%s,%s,%s)", TCLFileName, TaskName, ParametersList);

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * TCLScriptExecuteAndWait :  Execute a TCL script from a TCL file and wait the end of execution to return
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *TCLFileName
 *            char *TaskName
 *            char *InputParametersList
 *            char *OutputParametersList
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall TCLScriptExecuteAndWait (int SocketIndex, char * TCLFileName, char * TaskName, char * InputParametersList, char * OutputParametersList) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "TCLScriptExecuteAndWait (%s,%s,%s,char *)", TCLFileName, TaskName, InputParametersList);

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (ret == 0) 
	{ 
		char * pt;
		char * ptNext;

		pt = ReturnedValue;
		ptNext = NULL;
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) strcpy (OutputParametersList, pt);
		ptNext = strchr (OutputParametersList, ',');
		if (ptNext != NULL) *ptNext = '\0';
	} 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * TCLScriptKill :  Kill TCL Task
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *TaskName
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall TCLScriptKill (int SocketIndex, char * TaskName) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "TCLScriptKill (%s)", TaskName);

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * TCLScriptKillAll :  Kill all TCL Tasks
 *
 *     - Parameters :
 *            int SocketIndex
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall TCLScriptKillAll (int SocketIndex) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "TCLScriptKillAll ()");

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * TimerGet :  Get a timer
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *TimerName
 *            int *FrequencyTicks
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall TimerGet (int SocketIndex, char * TimerName, int * FrequencyTicks) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "TimerGet (%s,int *)", TimerName);

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (ret == 0) 
	{ 
		char * pt;
		char * ptNext;

		pt = ReturnedValue;
		ptNext = NULL;
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%d", FrequencyTicks);
	} 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * TimerSet :  Set a timer
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *TimerName
 *            int FrequencyTicks
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall TimerSet (int SocketIndex, char * TimerName, int FrequencyTicks) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "TimerSet (%s,%d)", TimerName, FrequencyTicks);

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * Reboot :  Reboot the controller
 *
 *     - Parameters :
 *            int SocketIndex
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall Reboot (int SocketIndex) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "Reboot ()");

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * Login :  Log in
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *Name
 *            char *Password
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall Login (int SocketIndex, char * Name, char * Password) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "Login (%s,%s)", Name, Password);

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * CloseAllOtherSockets :  Close all socket beside the one used to send this command
 *
 *     - Parameters :
 *            int SocketIndex
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall CloseAllOtherSockets (int SocketIndex) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "CloseAllOtherSockets ()");

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * EventAdd :  ** OBSOLETE ** Add an event
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *PositionerName
 *            char *EventName
 *            char *EventParameter
 *            char *ActionName
 *            char *ActionParameter1
 *            char *ActionParameter2
 *            char *ActionParameter3
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall EventAdd (int SocketIndex, char * PositionerName, char * EventName, char * EventParameter, char * ActionName, char * ActionParameter1, char * ActionParameter2, char * ActionParameter3) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "EventAdd (%s,%s,%s,%s,%s,%s,%s)", PositionerName, EventName, EventParameter, ActionName, ActionParameter1, ActionParameter2, ActionParameter3);

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * EventGet :  ** OBSOLETE ** Read events and actions list
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *PositionerName
 *            char *EventsAndActionsList
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall EventGet (int SocketIndex, char * PositionerName, char * EventsAndActionsList) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_BIG+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "EventGet (%s,char *)", PositionerName);

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_BIG); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (ret == 0) 
	{ 
		char * pt;
		char * ptNext;

		pt = ReturnedValue;
		ptNext = NULL;
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) strcpy (EventsAndActionsList, pt);
		ptNext = strchr (EventsAndActionsList, ',');
		if (ptNext != NULL) *ptNext = '\0';
	} 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * EventRemove :  ** OBSOLETE ** Delete an event
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *PositionerName
 *            char *EventName
 *            char *EventParameter
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall EventRemove (int SocketIndex, char * PositionerName, char * EventName, char * EventParameter) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "EventRemove (%s,%s,%s)", PositionerName, EventName, EventParameter);

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * EventWait :  ** OBSOLETE ** Wait an event
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *PositionerName
 *            char *EventName
 *            char *EventParameter
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall EventWait (int SocketIndex, char * PositionerName, char * EventName, char * EventParameter) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "EventWait (%s,%s,%s)", PositionerName, EventName, EventParameter);

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * EventExtendedConfigurationTriggerSet :  Configure one or several events
 *
 *     - Parameters :
 *            int SocketIndex
 *            int nbElement
 *            char *ExtendedEventName
 *            char *EventParameter1
 *            char *EventParameter2
 *            char *EventParameter3
 *            char *EventParameter4
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall EventExtendedConfigurationTriggerSet (int SocketIndex, int NbElements, char * ExtendedEventNameList, char * EventParameter1List, char * EventParameter2List, char * EventParameter3List, char * EventParameter4List) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 
	char temp[SIZE_NOMINAL+1];

	/* Split list */ 
	char *token;
	char seps[] = " \t;";
	int indice;
	char list [SIZE_NOMINAL+1];
	char *list_r;
	char subString[] = "{}";

	char (*stringArray0)[SIZE_NAME];
	stringArray0 = new char [NbElements][SIZE_NAME];
	indice = 0;
	strncpyWithEOS(list, ExtendedEventNameList, SIZE_NOMINAL, SIZE_NOMINAL);
	ReplaceCharacter(list, ';', ':', '{', '}'); /* for argument {x1;x2} */ 
	CleanString(list, '{', '}');
	list_r = NULL;
	token = strtok_r (list, seps, &list_r);
	while ((NULL != token) && (indice < NbElements))
	{
		memset(stringArray0[indice],'\0', SIZE_NAME);
		strncpyWithEOS(stringArray0[indice], token, SIZE_NAME, SIZE_NAME);
		ReplaceCharacter (stringArray0[indice], ':', ';', '{', '}');
		DeleteCharacters (stringArray0[indice], subString);
		token = strtok_r (NULL, seps, &list_r);
		indice++;
	}
	char (*stringArray1)[SIZE_NAME];
	stringArray1 = new char [NbElements][SIZE_NAME];
	indice = 0;
	strncpyWithEOS(list, EventParameter1List, SIZE_NOMINAL, SIZE_NOMINAL);
	ReplaceCharacter(list, ';', ':', '{', '}'); /* for argument {x1;x2} */ 
	CleanString(list, '{', '}');
	list_r = NULL;
	token = strtok_r (list, seps, &list_r);
	while ((NULL != token) && (indice < NbElements))
	{
		memset(stringArray1[indice],'\0', SIZE_NAME);
		strncpyWithEOS(stringArray1[indice], token, SIZE_NAME, SIZE_NAME);
		ReplaceCharacter (stringArray1[indice], ':', ';', '{', '}');
		DeleteCharacters (stringArray1[indice], subString);
		token = strtok_r (NULL, seps, &list_r);
		indice++;
	}
	char (*stringArray2)[SIZE_NAME];
	stringArray2 = new char [NbElements][SIZE_NAME];
	indice = 0;
	strncpyWithEOS(list, EventParameter2List, SIZE_NOMINAL, SIZE_NOMINAL);
	ReplaceCharacter(list, ';', ':', '{', '}'); /* for argument {x1;x2} */ 
	CleanString(list, '{', '}');
	list_r = NULL;
	token = strtok_r (list, seps, &list_r);
	while ((NULL != token) && (indice < NbElements))
	{
		memset(stringArray2[indice],'\0', SIZE_NAME);
		strncpyWithEOS(stringArray2[indice], token, SIZE_NAME, SIZE_NAME);
		ReplaceCharacter (stringArray2[indice], ':', ';', '{', '}');
		DeleteCharacters (stringArray2[indice], subString);
		token = strtok_r (NULL, seps, &list_r);
		indice++;
	}
	char (*stringArray3)[SIZE_NAME];
	stringArray3 = new char [NbElements][SIZE_NAME];
	indice = 0;
	strncpyWithEOS(list, EventParameter3List, SIZE_NOMINAL, SIZE_NOMINAL);
	ReplaceCharacter(list, ';', ':', '{', '}'); /* for argument {x1;x2} */ 
	CleanString(list, '{', '}');
	list_r = NULL;
	token = strtok_r (list, seps, &list_r);
	while ((NULL != token) && (indice < NbElements))
	{
		memset(stringArray3[indice],'\0', SIZE_NAME);
		strncpyWithEOS(stringArray3[indice], token, SIZE_NAME, SIZE_NAME);
		ReplaceCharacter (stringArray3[indice], ':', ';', '{', '}');
		DeleteCharacters (stringArray3[indice], subString);
		token = strtok_r (NULL, seps, &list_r);
		indice++;
	}
	char (*stringArray4)[SIZE_NAME];
	stringArray4 = new char [NbElements][SIZE_NAME];
	indice = 0;
	strncpyWithEOS(list, EventParameter4List, SIZE_NOMINAL, SIZE_NOMINAL);
	ReplaceCharacter(list, ';', ':', '{', '}'); /* for argument {x1;x2} */ 
	CleanString(list, '{', '}');
	list_r = NULL;
	token = strtok_r (list, seps, &list_r);
	while ((NULL != token) && (indice < NbElements))
	{
		memset(stringArray4[indice],'\0', SIZE_NAME);
		strncpyWithEOS(stringArray4[indice], token, SIZE_NAME, SIZE_NAME);
		ReplaceCharacter (stringArray4[indice], ':', ';', '{', '}');
		DeleteCharacters (stringArray4[indice], subString);
		token = strtok_r (NULL, seps, &list_r);
		indice++;
	}

	/* Convert to string */ 
	sprintf (ExecuteMethod, "EventExtendedConfigurationTriggerSet (");
	for (int i = 0; i < NbElements; i++)
	{
		sprintf (temp, "%s,%s,%s,%s,%s", stringArray0[i], stringArray1[i], stringArray2[i], stringArray3[i], stringArray4[i]);
		strncat (ExecuteMethod, temp, SIZE_SMALL);
		if ((i + 1) < NbElements) 
		{
			strncat (ExecuteMethod, ",", SIZE_SMALL);
		}
	}
	strcat (ExecuteMethod, ")");

	/* Clear memory */ 
	delete [] stringArray0;
	delete [] stringArray1;
	delete [] stringArray2;
	delete [] stringArray3;
	delete [] stringArray4;

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * EventExtendedConfigurationTriggerGet :  Read the event configuration
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *EventTriggerConfiguration
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall EventExtendedConfigurationTriggerGet (int SocketIndex, char * EventTriggerConfiguration) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_BIG+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "EventExtendedConfigurationTriggerGet (char *)");

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_BIG); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (ret == 0) 
	{ 
		char * pt;
		char * ptNext;

		pt = ReturnedValue;
		ptNext = NULL;
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) strcpy (EventTriggerConfiguration, pt);
		ptNext = strchr (EventTriggerConfiguration, ',');
		if (ptNext != NULL) *ptNext = '\0';
	} 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * EventExtendedConfigurationActionSet :  Configure one or several actions
 *
 *     - Parameters :
 *            int SocketIndex
 *            int nbElement
 *            char *ExtendedActionName
 *            char *ActionParameter1
 *            char *ActionParameter2
 *            char *ActionParameter3
 *            char *ActionParameter4
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall EventExtendedConfigurationActionSet (int SocketIndex, int NbElements, char * ExtendedActionNameList, char * ActionParameter1List, char * ActionParameter2List, char * ActionParameter3List, char * ActionParameter4List) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 
	char temp[SIZE_NOMINAL+1];

	/* Split list */ 
	char *token;
	char seps[] = " \t;";
	int indice;
	char list [SIZE_NOMINAL+1];
	char *list_r;
	char subString[] = "{}";

	char (*stringArray0)[SIZE_NAME];
	stringArray0 = new char [NbElements][SIZE_NAME];
	indice = 0;
	strncpyWithEOS(list, ExtendedActionNameList, SIZE_NOMINAL, SIZE_NOMINAL);
	ReplaceCharacter(list, ';', ':', '{', '}'); /* for argument {x1;x2} */ 
	CleanString(list, '{', '}');
	list_r = NULL;
	token = strtok_r (list, seps, &list_r);
	while ((NULL != token) && (indice < NbElements))
	{
		memset(stringArray0[indice],'\0', SIZE_NAME);
		strncpyWithEOS(stringArray0[indice], token, SIZE_NAME, SIZE_NAME);
		ReplaceCharacter (stringArray0[indice], ':', ';', '{', '}');
		DeleteCharacters (stringArray0[indice], subString);
		token = strtok_r (NULL, seps, &list_r);
		indice++;
	}
	char (*stringArray1)[SIZE_NAME];
	stringArray1 = new char [NbElements][SIZE_NAME];
	indice = 0;
	strncpyWithEOS(list, ActionParameter1List, SIZE_NOMINAL, SIZE_NOMINAL);
	ReplaceCharacter(list, ';', ':', '{', '}'); /* for argument {x1;x2} */ 
	CleanString(list, '{', '}');
	list_r = NULL;
	token = strtok_r (list, seps, &list_r);
	while ((NULL != token) && (indice < NbElements))
	{
		memset(stringArray1[indice],'\0', SIZE_NAME);
		strncpyWithEOS(stringArray1[indice], token, SIZE_NAME, SIZE_NAME);
		ReplaceCharacter (stringArray1[indice], ':', ';', '{', '}');
		DeleteCharacters (stringArray1[indice], subString);
		token = strtok_r (NULL, seps, &list_r);
		indice++;
	}
	char (*stringArray2)[SIZE_NAME];
	stringArray2 = new char [NbElements][SIZE_NAME];
	indice = 0;
	strncpyWithEOS(list, ActionParameter2List, SIZE_NOMINAL, SIZE_NOMINAL);
	ReplaceCharacter(list, ';', ':', '{', '}'); /* for argument {x1;x2} */ 
	CleanString(list, '{', '}');
	list_r = NULL;
	token = strtok_r (list, seps, &list_r);
	while ((NULL != token) && (indice < NbElements))
	{
		memset(stringArray2[indice],'\0', SIZE_NAME);
		strncpyWithEOS(stringArray2[indice], token, SIZE_NAME, SIZE_NAME);
		ReplaceCharacter (stringArray2[indice], ':', ';', '{', '}');
		DeleteCharacters (stringArray2[indice], subString);
		token = strtok_r (NULL, seps, &list_r);
		indice++;
	}
	char (*stringArray3)[SIZE_NAME];
	stringArray3 = new char [NbElements][SIZE_NAME];
	indice = 0;
	strncpyWithEOS(list, ActionParameter3List, SIZE_NOMINAL, SIZE_NOMINAL);
	ReplaceCharacter(list, ';', ':', '{', '}'); /* for argument {x1;x2} */ 
	CleanString(list, '{', '}');
	list_r = NULL;
	token = strtok_r (list, seps, &list_r);
	while ((NULL != token) && (indice < NbElements))
	{
		memset(stringArray3[indice],'\0', SIZE_NAME);
		strncpyWithEOS(stringArray3[indice], token, SIZE_NAME, SIZE_NAME);
		ReplaceCharacter (stringArray3[indice], ':', ';', '{', '}');
		DeleteCharacters (stringArray3[indice], subString);
		token = strtok_r (NULL, seps, &list_r);
		indice++;
	}
	char (*stringArray4)[SIZE_NAME];
	stringArray4 = new char [NbElements][SIZE_NAME];
	indice = 0;
	strncpyWithEOS(list, ActionParameter4List, SIZE_NOMINAL, SIZE_NOMINAL);
	ReplaceCharacter(list, ';', ':', '{', '}'); /* for argument {x1;x2} */ 
	CleanString(list, '{', '}');
	list_r = NULL;
	token = strtok_r (list, seps, &list_r);
	while ((NULL != token) && (indice < NbElements))
	{
		memset(stringArray4[indice],'\0', SIZE_NAME);
		strncpyWithEOS(stringArray4[indice], token, SIZE_NAME, SIZE_NAME);
		ReplaceCharacter (stringArray4[indice], ':', ';', '{', '}');
		DeleteCharacters (stringArray4[indice], subString);
		token = strtok_r (NULL, seps, &list_r);
		indice++;
	}

	/* Convert to string */ 
	sprintf (ExecuteMethod, "EventExtendedConfigurationActionSet (");
	for (int i = 0; i < NbElements; i++)
	{
		sprintf (temp, "%s,%s,%s,%s,%s", stringArray0[i], stringArray1[i], stringArray2[i], stringArray3[i], stringArray4[i]);
		strncat (ExecuteMethod, temp, SIZE_SMALL);
		if ((i + 1) < NbElements) 
		{
			strncat (ExecuteMethod, ",", SIZE_SMALL);
		}
	}
	strcat (ExecuteMethod, ")");

	/* Clear memory */ 
	delete [] stringArray0;
	delete [] stringArray1;
	delete [] stringArray2;
	delete [] stringArray3;
	delete [] stringArray4;

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * EventExtendedConfigurationActionGet :  Read the action configuration
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *ActionConfiguration
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall EventExtendedConfigurationActionGet (int SocketIndex, char * ActionConfiguration) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_BIG+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "EventExtendedConfigurationActionGet (char *)");

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_BIG); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (ret == 0) 
	{ 
		char * pt;
		char * ptNext;

		pt = ReturnedValue;
		ptNext = NULL;
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) strcpy (ActionConfiguration, pt);
		ptNext = strchr (ActionConfiguration, ',');
		if (ptNext != NULL) *ptNext = '\0';
	} 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * EventExtendedStart :  Launch the last event and action configuration and return an ID
 *
 *     - Parameters :
 *            int SocketIndex
 *            int *ID
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall EventExtendedStart (int SocketIndex, int * ID) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "EventExtendedStart (int *)");

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (ret == 0) 
	{ 
		char * pt;
		char * ptNext;

		pt = ReturnedValue;
		ptNext = NULL;
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%d", ID);
	} 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * EventExtendedAllGet :  Read all event and action configurations
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *EventActionConfigurations
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall EventExtendedAllGet (int SocketIndex, char * EventActionConfigurations) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_NOMINAL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "EventExtendedAllGet (char *)");

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_NOMINAL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (ret == 0) 
	{ 
		char * pt;
		char * ptNext;

		pt = ReturnedValue;
		ptNext = NULL;
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) strcpy (EventActionConfigurations, pt);
		ptNext = strchr (EventActionConfigurations, ',');
		if (ptNext != NULL) *ptNext = '\0';
	} 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * EventExtendedGet :  Read the event and action configuration defined by ID
 *
 *     - Parameters :
 *            int SocketIndex
 *            int ID
 *            char *EventTriggerConfiguration
 *            char *ActionConfiguration
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall EventExtendedGet (int SocketIndex, int ID, char * EventTriggerConfiguration, char * ActionConfiguration) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_BIG+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "EventExtendedGet (%d,char *,char *)", ID);

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_BIG); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (ret == 0) 
	{ 
		char * pt;
		char * ptNext;

		pt = ReturnedValue;
		ptNext = NULL;
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) strcpy (EventTriggerConfiguration, pt);
		ptNext = strchr (EventTriggerConfiguration, ',');
		if (ptNext != NULL) *ptNext = '\0';
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) strcpy (ActionConfiguration, pt);
		ptNext = strchr (ActionConfiguration, ',');
		if (ptNext != NULL) *ptNext = '\0';
	} 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * EventExtendedRemove :  Remove the event and action configuration defined by ID
 *
 *     - Parameters :
 *            int SocketIndex
 *            int ID
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall EventExtendedRemove (int SocketIndex, int ID) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "EventExtendedRemove (%d)", ID);

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * EventExtendedWait :  Wait events from the last event configuration
 *
 *     - Parameters :
 *            int SocketIndex
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall EventExtendedWait (int SocketIndex) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "EventExtendedWait ()");

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * GatheringConfigurationGet :  Read different mnemonique type
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *Type
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall GatheringConfigurationGet (int SocketIndex, char * Type) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_HUGE+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "GatheringConfigurationGet (char *)");

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_HUGE); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (ret == 0) 
	{ 
		char * pt;
		char * ptNext;

		pt = ReturnedValue;
		ptNext = NULL;
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) strcpy (Type, pt);
		ptNext = strchr (Type, ',');
		if (ptNext != NULL) *ptNext = '\0';
	} 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * GatheringConfigurationSet :  Configuration acquisition
 *
 *     - Parameters :
 *            int SocketIndex
 *            int nbElement
 *            char *Type
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall GatheringConfigurationSet (int SocketIndex, int NbElements, char * TypeList) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 
	char temp[SIZE_NOMINAL+1];

	/* Split list */ 
	char *token;
	char seps[] = " \t;";
	int indice;
	char list [SIZE_NOMINAL+1];
	char *list_r;
	char subString[] = "{}";

	char (*stringArray0)[SIZE_NAME];
	stringArray0 = new char [NbElements][SIZE_NAME];
	indice = 0;
	strncpyWithEOS(list, TypeList, SIZE_NOMINAL, SIZE_NOMINAL);
	ReplaceCharacter(list, ';', ':', '{', '}'); /* for argument {x1;x2} */ 
	CleanString(list, '{', '}');
	list_r = NULL;
	token = strtok_r (list, seps, &list_r);
	while ((NULL != token) && (indice < NbElements))
	{
		memset(stringArray0[indice],'\0', SIZE_NAME);
		strncpyWithEOS(stringArray0[indice], token, SIZE_NAME, SIZE_NAME);
		ReplaceCharacter (stringArray0[indice], ':', ';', '{', '}');
		DeleteCharacters (stringArray0[indice], subString);
		token = strtok_r (NULL, seps, &list_r);
		indice++;
	}

	/* Convert to string */ 
	sprintf (ExecuteMethod, "GatheringConfigurationSet (");
	for (int i = 0; i < NbElements; i++)
	{
		sprintf (temp, "%s", stringArray0[i]);
		strncat (ExecuteMethod, temp, SIZE_SMALL);
		if ((i + 1) < NbElements) 
		{
			strncat (ExecuteMethod, ",", SIZE_SMALL);
		}
	}
	strcat (ExecuteMethod, ")");

	/* Clear memory */ 
	delete [] stringArray0;

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * GatheringCurrentNumberGet :  Maximum number of samples and current number during acquisition
 *
 *     - Parameters :
 *            int SocketIndex
 *            int *CurrentNumber
 *            int *MaximumSamplesNumber
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall GatheringCurrentNumberGet (int SocketIndex, int * CurrentNumber, int * MaximumSamplesNumber) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "GatheringCurrentNumberGet (int *,int *)");

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (ret == 0) 
	{ 
		char * pt;
		char * ptNext;

		pt = ReturnedValue;
		ptNext = NULL;
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%d", CurrentNumber);
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%d", MaximumSamplesNumber);
	} 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * GatheringStopAndSave :  Stop acquisition and save data
 *
 *     - Parameters :
 *            int SocketIndex
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall GatheringStopAndSave (int SocketIndex) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "GatheringStopAndSave ()");

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * GatheringDataAcquire :  Acquire a configured data
 *
 *     - Parameters :
 *            int SocketIndex
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall GatheringDataAcquire (int SocketIndex) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "GatheringDataAcquire ()");

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * GatheringDataGet :  Get a data line from gathering buffer
 *
 *     - Parameters :
 *            int SocketIndex
 *            int IndexPoint
 *            char *DataBufferLine
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall GatheringDataGet (int SocketIndex, int IndexPoint, char * DataBufferLine) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_NOMINAL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "GatheringDataGet (%d,char *)", IndexPoint);

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_NOMINAL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (ret == 0) 
	{ 
		char * pt;
		char * ptNext;

		pt = ReturnedValue;
		ptNext = NULL;
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) strcpy (DataBufferLine, pt);
		ptNext = strchr (DataBufferLine, ',');
		if (ptNext != NULL) *ptNext = '\0';
	} 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * GatheringDataMultipleLinesGet :  Get multiple data lines from gathering buffer
 *
 *     - Parameters :
 *            int SocketIndex
 *            int IndexPoint
 *            int NumberOfLines
 *            char *DataBufferLine
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall GatheringDataMultipleLinesGet (int SocketIndex, int IndexPoint, int NumberOfLines, char * DataBufferLine) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_HUGE+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "GatheringDataMultipleLinesGet (%d,%d,char *)", IndexPoint, NumberOfLines);

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_HUGE); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (ret == 0) 
	{ 
		char * pt;
		char * ptNext;

		pt = ReturnedValue;
		ptNext = NULL;
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) strcpy (DataBufferLine, pt);
		ptNext = strchr (DataBufferLine, ',');
		if (ptNext != NULL) *ptNext = '\0';
	} 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * GatheringReset :  Empty the gathered data in memory to start new gathering from scratch
 *
 *     - Parameters :
 *            int SocketIndex
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall GatheringReset (int SocketIndex) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "GatheringReset ()");

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * GatheringRun :  Start a new gathering
 *
 *     - Parameters :
 *            int SocketIndex
 *            int DataNumber
 *            int Divisor
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall GatheringRun (int SocketIndex, int DataNumber, int Divisor) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "GatheringRun (%d,%d)", DataNumber, Divisor);

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * GatheringStop :  Stop the data gathering (without saving to file)
 *
 *     - Parameters :
 *            int SocketIndex
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall GatheringStop (int SocketIndex) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "GatheringStop ()");

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * GatheringExternalConfigurationSet :  Configuration acquisition
 *
 *     - Parameters :
 *            int SocketIndex
 *            int nbElement
 *            char *Type
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall GatheringExternalConfigurationSet (int SocketIndex, int NbElements, char * TypeList) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 
	char temp[SIZE_NOMINAL+1];

	/* Split list */ 
	char *token;
	char seps[] = " \t;";
	int indice;
	char list [SIZE_NOMINAL+1];
	char *list_r;
	char subString[] = "{}";

	char (*stringArray0)[SIZE_NAME];
	stringArray0 = new char [NbElements][SIZE_NAME];
	indice = 0;
	strncpyWithEOS(list, TypeList, SIZE_NOMINAL, SIZE_NOMINAL);
	ReplaceCharacter(list, ';', ':', '{', '}'); /* for argument {x1;x2} */ 
	CleanString(list, '{', '}');
	list_r = NULL;
	token = strtok_r (list, seps, &list_r);
	while ((NULL != token) && (indice < NbElements))
	{
		memset(stringArray0[indice],'\0', SIZE_NAME);
		strncpyWithEOS(stringArray0[indice], token, SIZE_NAME, SIZE_NAME);
		ReplaceCharacter (stringArray0[indice], ':', ';', '{', '}');
		DeleteCharacters (stringArray0[indice], subString);
		token = strtok_r (NULL, seps, &list_r);
		indice++;
	}

	/* Convert to string */ 
	sprintf (ExecuteMethod, "GatheringExternalConfigurationSet (");
	for (int i = 0; i < NbElements; i++)
	{
		sprintf (temp, "%s", stringArray0[i]);
		strncat (ExecuteMethod, temp, SIZE_SMALL);
		if ((i + 1) < NbElements) 
		{
			strncat (ExecuteMethod, ",", SIZE_SMALL);
		}
	}
	strcat (ExecuteMethod, ")");

	/* Clear memory */ 
	delete [] stringArray0;

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * GatheringExternalConfigurationGet :  Read different mnemonique type
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *Type
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall GatheringExternalConfigurationGet (int SocketIndex, char * Type) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_HUGE+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "GatheringExternalConfigurationGet (char *)");

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_HUGE); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (ret == 0) 
	{ 
		char * pt;
		char * ptNext;

		pt = ReturnedValue;
		ptNext = NULL;
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) strcpy (Type, pt);
		ptNext = strchr (Type, ',');
		if (ptNext != NULL) *ptNext = '\0';
	} 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * GatheringExternalCurrentNumberGet :  Maximum number of samples and current number during acquisition
 *
 *     - Parameters :
 *            int SocketIndex
 *            int *CurrentNumber
 *            int *MaximumSamplesNumber
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall GatheringExternalCurrentNumberGet (int SocketIndex, int * CurrentNumber, int * MaximumSamplesNumber) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "GatheringExternalCurrentNumberGet (int *,int *)");

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (ret == 0) 
	{ 
		char * pt;
		char * ptNext;

		pt = ReturnedValue;
		ptNext = NULL;
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%d", CurrentNumber);
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%d", MaximumSamplesNumber);
	} 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * GatheringExternalStopAndSave :  Stop acquisition and save data
 *
 *     - Parameters :
 *            int SocketIndex
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall GatheringExternalStopAndSave (int SocketIndex) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "GatheringExternalStopAndSave ()");

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * GlobalArrayGet :  Get global array value
 *
 *     - Parameters :
 *            int SocketIndex
 *            int Number
 *            char *ValueString
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall GlobalArrayGet (int SocketIndex, int Number, char * ValueString) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "GlobalArrayGet (%d,char *)", Number);

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (ret == 0) 
	{ 
		char * pt;
		char * ptNext;

		pt = ReturnedValue;
		ptNext = NULL;
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) strcpy (ValueString, pt);
		ptNext = strchr (ValueString, ',');
		if (ptNext != NULL) *ptNext = '\0';
	} 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * GlobalArraySet :  Set global array value
 *
 *     - Parameters :
 *            int SocketIndex
 *            int Number
 *            char *ValueString
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall GlobalArraySet (int SocketIndex, int Number, char * ValueString) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "GlobalArraySet (%d,%s)", Number, ValueString);

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * DoubleGlobalArrayGet :  Get double global array value
 *
 *     - Parameters :
 *            int SocketIndex
 *            int Number
 *            double *DoubleValue
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall DoubleGlobalArrayGet (int SocketIndex, int Number, double * DoubleValue) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "DoubleGlobalArrayGet (%d,double *)", Number);

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (ret == 0) 
	{ 
		char * pt;
		char * ptNext;

		pt = ReturnedValue;
		ptNext = NULL;
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", DoubleValue);
	} 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * DoubleGlobalArraySet :  Set double global array value
 *
 *     - Parameters :
 *            int SocketIndex
 *            int Number
 *            double DoubleValue
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall DoubleGlobalArraySet (int SocketIndex, int Number, double DoubleValue) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "DoubleGlobalArraySet (%d,%.13g)", Number, DoubleValue);

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * GPIOAnalogGet :  Read analog input or analog output for one or few input
 *
 *     - Parameters :
 *            int SocketIndex
 *            int nbElement
 *            char *GPIOName
 *            double *AnalogValue
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall GPIOAnalogGet (int SocketIndex, int NbElements, char * GPIONameList, double AnalogValue[]) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 
	char temp[SIZE_NOMINAL+1];

	/* Split list */ 
	char *token;
	char seps[] = " \t;";
	int indice;
	char list [SIZE_NOMINAL+1];
	char *list_r;
	char subString[] = "{}";

	char (*stringArray0)[SIZE_NAME];
	stringArray0 = new char [NbElements][SIZE_NAME];
	indice = 0;
	strncpyWithEOS(list, GPIONameList, SIZE_NOMINAL, SIZE_NOMINAL);
	ReplaceCharacter(list, ';', ':', '{', '}'); /* for argument {x1;x2} */ 
	CleanString(list, '{', '}');
	list_r = NULL;
	token = strtok_r (list, seps, &list_r);
	while ((NULL != token) && (indice < NbElements))
	{
		memset(stringArray0[indice],'\0', SIZE_NAME);
		strncpyWithEOS(stringArray0[indice], token, SIZE_NAME, SIZE_NAME);
		ReplaceCharacter (stringArray0[indice], ':', ';', '{', '}');
		DeleteCharacters (stringArray0[indice], subString);
		token = strtok_r (NULL, seps, &list_r);
		indice++;
	}

	/* Convert to string */ 
	sprintf (ExecuteMethod, "GPIOAnalogGet (");
	for (int i = 0; i < NbElements; i++)
	{
		sprintf (temp, "%s,double *", stringArray0[i]);
		strncat (ExecuteMethod, temp, SIZE_SMALL);
		if ((i + 1) < NbElements) 
		{
			strncat (ExecuteMethod, ",", SIZE_SMALL);
		}
	}
	strcat (ExecuteMethod, ")");

	/* Clear memory */ 
	delete [] stringArray0;

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (ret == 0) 
	{ 
		char * pt;
		char * ptNext;

		pt = ReturnedValue;
		ptNext = NULL;

		for (int i = 0; i < NbElements; i++)
		{
			if (pt != NULL) pt = strchr (pt, ',');
			if (pt != NULL) pt++;
			if (pt != NULL) sscanf (pt, "%lf", &AnalogValue[i]);
		}
	} 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * GPIOAnalogSet :  Set analog output for one or few output
 *
 *     - Parameters :
 *            int SocketIndex
 *            int nbElement
 *            char *GPIOName
 *            double AnalogOutputValue
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall GPIOAnalogSet (int SocketIndex, int NbElements, char * GPIONameList, double AnalogOutputValue[]) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 
	char temp[SIZE_NOMINAL+1];

	/* Split list */ 
	char *token;
	char seps[] = " \t;";
	int indice;
	char list [SIZE_NOMINAL+1];
	char *list_r;
	char subString[] = "{}";

	char (*stringArray0)[SIZE_NAME];
	stringArray0 = new char [NbElements][SIZE_NAME];
	indice = 0;
	strncpyWithEOS(list, GPIONameList, SIZE_NOMINAL, SIZE_NOMINAL);
	ReplaceCharacter(list, ';', ':', '{', '}'); /* for argument {x1;x2} */ 
	CleanString(list, '{', '}');
	list_r = NULL;
	token = strtok_r (list, seps, &list_r);
	while ((NULL != token) && (indice < NbElements))
	{
		memset(stringArray0[indice],'\0', SIZE_NAME);
		strncpyWithEOS(stringArray0[indice], token, SIZE_NAME, SIZE_NAME);
		ReplaceCharacter (stringArray0[indice], ':', ';', '{', '}');
		DeleteCharacters (stringArray0[indice], subString);
		token = strtok_r (NULL, seps, &list_r);
		indice++;
	}

	/* Convert to string */ 
	sprintf (ExecuteMethod, "GPIOAnalogSet (");
	for (int i = 0; i < NbElements; i++)
	{
		sprintf (temp, "%s,%.13g", stringArray0[i], AnalogOutputValue[i]);
		strncat (ExecuteMethod, temp, SIZE_SMALL);
		if ((i + 1) < NbElements) 
		{
			strncat (ExecuteMethod, ",", SIZE_SMALL);
		}
	}
	strcat (ExecuteMethod, ")");

	/* Clear memory */ 
	delete [] stringArray0;

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * GPIOAnalogGainGet :  Read analog input gain (1, 2, 4 or 8) for one or few input
 *
 *     - Parameters :
 *            int SocketIndex
 *            int nbElement
 *            char *GPIOName
 *            int *AnalogInputGainValue
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall GPIOAnalogGainGet (int SocketIndex, int NbElements, char * GPIONameList, int AnalogInputGainValue[]) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 
	char temp[SIZE_NOMINAL+1];

	/* Split list */ 
	char *token;
	char seps[] = " \t;";
	int indice;
	char list [SIZE_NOMINAL+1];
	char *list_r;
	char subString[] = "{}";

	char (*stringArray0)[SIZE_NAME];
	stringArray0 = new char [NbElements][SIZE_NAME];
	indice = 0;
	strncpyWithEOS(list, GPIONameList, SIZE_NOMINAL, SIZE_NOMINAL);
	ReplaceCharacter(list, ';', ':', '{', '}'); /* for argument {x1;x2} */ 
	CleanString(list, '{', '}');
	list_r = NULL;
	token = strtok_r (list, seps, &list_r);
	while ((NULL != token) && (indice < NbElements))
	{
		memset(stringArray0[indice],'\0', SIZE_NAME);
		strncpyWithEOS(stringArray0[indice], token, SIZE_NAME, SIZE_NAME);
		ReplaceCharacter (stringArray0[indice], ':', ';', '{', '}');
		DeleteCharacters (stringArray0[indice], subString);
		token = strtok_r (NULL, seps, &list_r);
		indice++;
	}

	/* Convert to string */ 
	sprintf (ExecuteMethod, "GPIOAnalogGainGet (");
	for (int i = 0; i < NbElements; i++)
	{
		sprintf (temp, "%s,int *", stringArray0[i]);
		strncat (ExecuteMethod, temp, SIZE_SMALL);
		if ((i + 1) < NbElements) 
		{
			strncat (ExecuteMethod, ",", SIZE_SMALL);
		}
	}
	strcat (ExecuteMethod, ")");

	/* Clear memory */ 
	delete [] stringArray0;

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (ret == 0) 
	{ 
		char * pt;
		char * ptNext;

		pt = ReturnedValue;
		ptNext = NULL;

		for (int i = 0; i < NbElements; i++)
		{
			if (pt != NULL) pt = strchr (pt, ',');
			if (pt != NULL) pt++;
			if (pt != NULL) sscanf (pt, "%d", &AnalogInputGainValue[i]);
		}
	} 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * GPIOAnalogGainSet :  Set analog input gain (1, 2, 4 or 8) for one or few input
 *
 *     - Parameters :
 *            int SocketIndex
 *            int nbElement
 *            char *GPIOName
 *            int AnalogInputGainValue
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall GPIOAnalogGainSet (int SocketIndex, int NbElements, char * GPIONameList, int AnalogInputGainValue[]) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 
	char temp[SIZE_NOMINAL+1];

	/* Split list */ 
	char *token;
	char seps[] = " \t;";
	int indice;
	char list [SIZE_NOMINAL+1];
	char *list_r;
	char subString[] = "{}";

	char (*stringArray0)[SIZE_NAME];
	stringArray0 = new char [NbElements][SIZE_NAME];
	indice = 0;
	strncpyWithEOS(list, GPIONameList, SIZE_NOMINAL, SIZE_NOMINAL);
	ReplaceCharacter(list, ';', ':', '{', '}'); /* for argument {x1;x2} */ 
	CleanString(list, '{', '}');
	list_r = NULL;
	token = strtok_r (list, seps, &list_r);
	while ((NULL != token) && (indice < NbElements))
	{
		memset(stringArray0[indice],'\0', SIZE_NAME);
		strncpyWithEOS(stringArray0[indice], token, SIZE_NAME, SIZE_NAME);
		ReplaceCharacter (stringArray0[indice], ':', ';', '{', '}');
		DeleteCharacters (stringArray0[indice], subString);
		token = strtok_r (NULL, seps, &list_r);
		indice++;
	}

	/* Convert to string */ 
	sprintf (ExecuteMethod, "GPIOAnalogGainSet (");
	for (int i = 0; i < NbElements; i++)
	{
		sprintf (temp, "%s,%d", stringArray0[i], AnalogInputGainValue[i]);
		strncat (ExecuteMethod, temp, SIZE_SMALL);
		if ((i + 1) < NbElements) 
		{
			strncat (ExecuteMethod, ",", SIZE_SMALL);
		}
	}
	strcat (ExecuteMethod, ")");

	/* Clear memory */ 
	delete [] stringArray0;

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * GPIODigitalGet :  Read digital output or digital input 
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *GPIOName
 *            unsigned short *DigitalValue
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall GPIODigitalGet (int SocketIndex, char * GPIOName, unsigned short * DigitalValue) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "GPIODigitalGet (%s,unsigned short *)", GPIOName);

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (ret == 0) 
	{ 
		char * pt;
		char * ptNext;

		pt = ReturnedValue;
		ptNext = NULL;
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%hu", DigitalValue);
	} 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * GPIODigitalSet :  Set Digital Output for one or few output TTL
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *GPIOName
 *            unsigned short Mask
 *            unsigned short DigitalOutputValue
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall GPIODigitalSet (int SocketIndex, char * GPIOName, unsigned short Mask, unsigned short DigitalOutputValue) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "GPIODigitalSet (%s,%hu,%hu)", GPIOName, Mask, DigitalOutputValue);

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * GroupCorrectorOutputGet :  Return corrector outputs
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *GroupName
 *            int nbElement
 *            double *CorrectorOutput
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall GroupCorrectorOutputGet (int SocketIndex, char * GroupName, int NbElements, double CorrectorOutput[]) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 
	char temp[SIZE_NOMINAL+1];

	/* Convert to string */ 
	sprintf (ExecuteMethod, "GroupCorrectorOutputGet (%s,", GroupName);
	for (int i = 0; i < NbElements; i++)
	{
		sprintf (temp, "double *");
		strncat (ExecuteMethod, temp, SIZE_SMALL);
		if ((i + 1) < NbElements) 
		{
			strncat (ExecuteMethod, ",", SIZE_SMALL);
		}
	}
	strcat (ExecuteMethod, ")");

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (ret == 0) 
	{ 
		char * pt;
		char * ptNext;

		pt = ReturnedValue;
		ptNext = NULL;

		for (int i = 0; i < NbElements; i++)
		{
			if (pt != NULL) pt = strchr (pt, ',');
			if (pt != NULL) pt++;
			if (pt != NULL) sscanf (pt, "%lf", &CorrectorOutput[i]);
		}
	} 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * GroupHomeSearch :  Start home search sequence
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *GroupName
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall GroupHomeSearch (int SocketIndex, char * GroupName) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "GroupHomeSearch (%s)", GroupName);

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * GroupHomeSearchAndRelativeMove :  Start home search sequence and execute a displacement
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *GroupName
 *            double TargetDisplacement
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall GroupHomeSearchAndRelativeMove (int SocketIndex, char * GroupName, int NbElements, double TargetDisplacement[]) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 
	char temp[SIZE_NOMINAL+1];

	/* Convert to string */ 
	sprintf (ExecuteMethod, "GroupHomeSearchAndRelativeMove (%s,", GroupName);
	for (int i = 0; i < NbElements; i++)
	{
		sprintf (temp, "%.13g", TargetDisplacement[i]);
		strncat (ExecuteMethod, temp, SIZE_SMALL);
		if ((i + 1) < NbElements) 
		{
			strncat (ExecuteMethod, ",", SIZE_SMALL);
		}
	}
	strcat (ExecuteMethod, ")");

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * GroupReadyAtPosition :  Go to READY state with the users positions (hexapod group)
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *GroupName
 *            double EncoderPosition1
 *            double EncoderPosition2
 *            double EncoderPosition3
 *            double EncoderPosition4
 *            double EncoderPosition5
 *            double EncoderPosition6
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall GroupReadyAtPosition (int SocketIndex, char * GroupName, double EncoderPosition1, double EncoderPosition2, double EncoderPosition3, double EncoderPosition4, double EncoderPosition5, double EncoderPosition6) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "GroupReadyAtPosition (%s,%.13g,%.13g,%.13g,%.13g,%.13g,%.13g)", GroupName, EncoderPosition1, EncoderPosition2, EncoderPosition3, EncoderPosition4, EncoderPosition5, EncoderPosition6);

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * GroupInitialize :  Start the initialization
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *GroupName
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall GroupInitialize (int SocketIndex, char * GroupName) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "GroupInitialize (%s)", GroupName);

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * GroupInitializeWithEncoderCalibration :  Start the initialization with encoder calibration
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *GroupName
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall GroupInitializeWithEncoderCalibration (int SocketIndex, char * GroupName) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "GroupInitializeWithEncoderCalibration (%s)", GroupName);

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * GroupKill :  Kill the group
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *GroupName
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall GroupKill (int SocketIndex, char * GroupName) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "GroupKill (%s)", GroupName);

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * GroupMoveAbort :  Abort a move
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *GroupName
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall GroupMoveAbort (int SocketIndex, char * GroupName) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "GroupMoveAbort (%s)", GroupName);

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * GroupMoveAbsolute :  Do an absolute move
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *GroupName
 *            double TargetPosition
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall GroupMoveAbsolute (int SocketIndex, char * GroupName, int NbElements, double TargetPosition[]) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 
	char temp[SIZE_NOMINAL+1];

	/* Convert to string */ 
	sprintf (ExecuteMethod, "GroupMoveAbsolute (%s,", GroupName);
	for (int i = 0; i < NbElements; i++)
	{
		sprintf (temp, "%.13g", TargetPosition[i]);
		strncat (ExecuteMethod, temp, SIZE_SMALL);
		if ((i + 1) < NbElements) 
		{
			strncat (ExecuteMethod, ",", SIZE_SMALL);
		}
	}
	strcat (ExecuteMethod, ")");

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * GroupMoveRelative :  Do a relative move
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *GroupName
 *            double TargetDisplacement
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall GroupMoveRelative (int SocketIndex, char * GroupName, int NbElements, double TargetDisplacement[]) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 
	char temp[SIZE_NOMINAL+1];

	/* Convert to string */ 
	sprintf (ExecuteMethod, "GroupMoveRelative (%s,", GroupName);
	for (int i = 0; i < NbElements; i++)
	{
		sprintf (temp, "%.13g", TargetDisplacement[i]);
		strncat (ExecuteMethod, temp, SIZE_SMALL);
		if ((i + 1) < NbElements) 
		{
			strncat (ExecuteMethod, ",", SIZE_SMALL);
		}
	}
	strcat (ExecuteMethod, ")");

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * GroupMotionDisable :  Set Motion disable on selected group
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *GroupName
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall GroupMotionDisable (int SocketIndex, char * GroupName) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "GroupMotionDisable (%s)", GroupName);

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * GroupMotionEnable :  Set Motion enable on selected group
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *GroupName
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall GroupMotionEnable (int SocketIndex, char * GroupName) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "GroupMotionEnable (%s)", GroupName);

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * GroupPositionCorrectedProfilerGet :  Return corrected profiler positions
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *GroupName
 *            double PositionX
 *            double PositionY
 *            double *CorrectedProfilerPositionX
 *            double *CorrectedProfilerPositionY
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall GroupPositionCorrectedProfilerGet (int SocketIndex, char * GroupName, double PositionX, double PositionY, double * CorrectedProfilerPositionX, double * CorrectedProfilerPositionY) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "GroupPositionCorrectedProfilerGet (%s,%.13g,%.13g,double *,double *)", GroupName, PositionX, PositionY);

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (ret == 0) 
	{ 
		char * pt;
		char * ptNext;

		pt = ReturnedValue;
		ptNext = NULL;
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", CorrectedProfilerPositionX);
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", CorrectedProfilerPositionY);
	} 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * GroupPositionCurrentGet :  Return current positions
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *GroupName
 *            int nbElement
 *            double *CurrentEncoderPosition
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall GroupPositionCurrentGet (int SocketIndex, char * GroupName, int NbElements, double CurrentEncoderPosition[]) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 
	char temp[SIZE_NOMINAL+1];

	/* Convert to string */ 
	sprintf (ExecuteMethod, "GroupPositionCurrentGet (%s,", GroupName);
	for (int i = 0; i < NbElements; i++)
	{
		sprintf (temp, "double *");
		strncat (ExecuteMethod, temp, SIZE_SMALL);
		if ((i + 1) < NbElements) 
		{
			strncat (ExecuteMethod, ",", SIZE_SMALL);
		}
	}
	strcat (ExecuteMethod, ")");

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (ret == 0) 
	{ 
		char * pt;
		char * ptNext;

		pt = ReturnedValue;
		ptNext = NULL;

		for (int i = 0; i < NbElements; i++)
		{
			if (pt != NULL) pt = strchr (pt, ',');
			if (pt != NULL) pt++;
			if (pt != NULL) sscanf (pt, "%lf", &CurrentEncoderPosition[i]);
		}
	} 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * GroupPositionSetpointGet :  Return setpoint positions
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *GroupName
 *            int nbElement
 *            double *SetPointPosition
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall GroupPositionSetpointGet (int SocketIndex, char * GroupName, int NbElements, double SetPointPosition[]) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 
	char temp[SIZE_NOMINAL+1];

	/* Convert to string */ 
	sprintf (ExecuteMethod, "GroupPositionSetpointGet (%s,", GroupName);
	for (int i = 0; i < NbElements; i++)
	{
		sprintf (temp, "double *");
		strncat (ExecuteMethod, temp, SIZE_SMALL);
		if ((i + 1) < NbElements) 
		{
			strncat (ExecuteMethod, ",", SIZE_SMALL);
		}
	}
	strcat (ExecuteMethod, ")");

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (ret == 0) 
	{ 
		char * pt;
		char * ptNext;

		pt = ReturnedValue;
		ptNext = NULL;

		for (int i = 0; i < NbElements; i++)
		{
			if (pt != NULL) pt = strchr (pt, ',');
			if (pt != NULL) pt++;
			if (pt != NULL) sscanf (pt, "%lf", &SetPointPosition[i]);
		}
	} 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * GroupPositionTargetGet :  Return target positions
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *GroupName
 *            int nbElement
 *            double *TargetPosition
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall GroupPositionTargetGet (int SocketIndex, char * GroupName, int NbElements, double TargetPosition[]) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 
	char temp[SIZE_NOMINAL+1];

	/* Convert to string */ 
	sprintf (ExecuteMethod, "GroupPositionTargetGet (%s,", GroupName);
	for (int i = 0; i < NbElements; i++)
	{
		sprintf (temp, "double *");
		strncat (ExecuteMethod, temp, SIZE_SMALL);
		if ((i + 1) < NbElements) 
		{
			strncat (ExecuteMethod, ",", SIZE_SMALL);
		}
	}
	strcat (ExecuteMethod, ")");

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (ret == 0) 
	{ 
		char * pt;
		char * ptNext;

		pt = ReturnedValue;
		ptNext = NULL;

		for (int i = 0; i < NbElements; i++)
		{
			if (pt != NULL) pt = strchr (pt, ',');
			if (pt != NULL) pt++;
			if (pt != NULL) sscanf (pt, "%lf", &TargetPosition[i]);
		}
	} 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * GroupReferencingActionExecute :  Execute an action in referencing mode
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *PositionerName
 *            char *ReferencingAction
 *            char *ReferencingSensor
 *            double ReferencingParameter
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall GroupReferencingActionExecute (int SocketIndex, char * PositionerName, char * ReferencingAction, char * ReferencingSensor, double ReferencingParameter) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "GroupReferencingActionExecute (%s,%s,%s,%.13g)", PositionerName, ReferencingAction, ReferencingSensor, ReferencingParameter);

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * GroupReferencingStart :  Enter referencing mode
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *GroupName
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall GroupReferencingStart (int SocketIndex, char * GroupName) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "GroupReferencingStart (%s)", GroupName);

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * GroupReferencingStop :  Exit referencing mode
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *GroupName
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall GroupReferencingStop (int SocketIndex, char * GroupName) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "GroupReferencingStop (%s)", GroupName);

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * GroupStatusGet :  Return group status
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *GroupName
 *            int *Status
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall GroupStatusGet (int SocketIndex, char * GroupName, int * Status) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "GroupStatusGet (%s,int *)", GroupName);

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (ret == 0) 
	{ 
		char * pt;
		char * ptNext;

		pt = ReturnedValue;
		ptNext = NULL;
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%d", Status);
	} 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * GroupStatusStringGet :  Return the group status string corresponding to the group status code
 *
 *     - Parameters :
 *            int SocketIndex
 *            int GroupStatusCode
 *            char *GroupStatusString
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall GroupStatusStringGet (int SocketIndex, int GroupStatusCode, char * GroupStatusString) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_NOMINAL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "GroupStatusStringGet (%d,char *)", GroupStatusCode);

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_NOMINAL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (ret == 0) 
	{ 
		char * pt;
		char * ptNext;

		pt = ReturnedValue;
		ptNext = NULL;
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) strcpy (GroupStatusString, pt);
		ptNext = strchr (GroupStatusString, ',');
		if (ptNext != NULL) *ptNext = '\0';
	} 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * KillAll :  Put all groups in 'Not initialized' state
 *
 *     - Parameters :
 *            int SocketIndex
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall KillAll (int SocketIndex) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "KillAll ()");

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * RestartApplication :  Restart the Controller
 *
 *     - Parameters :
 *            int SocketIndex
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall RestartApplication (int SocketIndex) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "RestartApplication ()");

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * PositionerBacklashGet :  Read backlash value and status
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *PositionerName
 *            double *BacklashValue
 *            char *BacklaskStatus
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall PositionerBacklashGet (int SocketIndex, char * PositionerName, double * BacklashValue, char * BacklaskStatus) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "PositionerBacklashGet (%s,double *,char *)", PositionerName);

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (ret == 0) 
	{ 
		char * pt;
		char * ptNext;

		pt = ReturnedValue;
		ptNext = NULL;
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", BacklashValue);
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) strcpy (BacklaskStatus, pt);
		ptNext = strchr (BacklaskStatus, ',');
		if (ptNext != NULL) *ptNext = '\0';
	} 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * PositionerBacklashSet :  Set backlash value
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *PositionerName
 *            double BacklashValue
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall PositionerBacklashSet (int SocketIndex, char * PositionerName, double BacklashValue) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "PositionerBacklashSet (%s,%.13g)", PositionerName, BacklashValue);

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * PositionerBacklashEnable :  Enable the backlash
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *PositionerName
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall PositionerBacklashEnable (int SocketIndex, char * PositionerName) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "PositionerBacklashEnable (%s)", PositionerName);

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * PositionerBacklashDisable :  Disable the backlash
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *PositionerName
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall PositionerBacklashDisable (int SocketIndex, char * PositionerName) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "PositionerBacklashDisable (%s)", PositionerName);

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * PositionerCorrectorNotchFiltersSet :  Update filters parameters 
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *PositionerName
 *            double NotchFrequency1
 *            double NotchBandwith1
 *            double NotchGain1
 *            double NotchFrequency2
 *            double NotchBandwith2
 *            double NotchGain2
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall PositionerCorrectorNotchFiltersSet (int SocketIndex, char * PositionerName, double NotchFrequency1, double NotchBandwith1, double NotchGain1, double NotchFrequency2, double NotchBandwith2, double NotchGain2) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "PositionerCorrectorNotchFiltersSet (%s,%.13g,%.13g,%.13g,%.13g,%.13g,%.13g)", PositionerName, NotchFrequency1, NotchBandwith1, NotchGain1, NotchFrequency2, NotchBandwith2, NotchGain2);

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * PositionerCorrectorNotchFiltersGet :  Read filters parameters 
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *PositionerName
 *            double *NotchFrequency1
 *            double *NotchBandwith1
 *            double *NotchGain1
 *            double *NotchFrequency2
 *            double *NotchBandwith2
 *            double *NotchGain2
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall PositionerCorrectorNotchFiltersGet (int SocketIndex, char * PositionerName, double * NotchFrequency1, double * NotchBandwith1, double * NotchGain1, double * NotchFrequency2, double * NotchBandwith2, double * NotchGain2) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "PositionerCorrectorNotchFiltersGet (%s,double *,double *,double *,double *,double *,double *)", PositionerName);

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (ret == 0) 
	{ 
		char * pt;
		char * ptNext;

		pt = ReturnedValue;
		ptNext = NULL;
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", NotchFrequency1);
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", NotchBandwith1);
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", NotchGain1);
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", NotchFrequency2);
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", NotchBandwith2);
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", NotchGain2);
	} 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * PositionerCorrectorPIDFFAccelerationSet :  Update corrector parameters
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *PositionerName
 *            bool ClosedLoopStatus
 *            double KP
 *            double KI
 *            double KD
 *            double KS
 *            double IntegrationTime
 *            double DerivativeFilterCutOffFrequency
 *            double GKP
 *            double GKI
 *            double GKD
 *            double KForm
 *            double FeedForwardGainAcceleration
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall PositionerCorrectorPIDFFAccelerationSet (int SocketIndex, char * PositionerName, bool ClosedLoopStatus, double KP, double KI, double KD, double KS, double IntegrationTime, double DerivativeFilterCutOffFrequency, double GKP, double GKI, double GKD, double KForm, double FeedForwardGainAcceleration) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "PositionerCorrectorPIDFFAccelerationSet (%s,%d,%.13g,%.13g,%.13g,%.13g,%.13g,%.13g,%.13g,%.13g,%.13g,%.13g,%.13g)", PositionerName, ClosedLoopStatus, KP, KI, KD, KS, IntegrationTime, DerivativeFilterCutOffFrequency, GKP, GKI, GKD, KForm, FeedForwardGainAcceleration);

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * PositionerCorrectorPIDFFAccelerationGet :  Read corrector parameters
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *PositionerName
 *            bool *ClosedLoopStatus
 *            double *KP
 *            double *KI
 *            double *KD
 *            double *KS
 *            double *IntegrationTime
 *            double *DerivativeFilterCutOffFrequency
 *            double *GKP
 *            double *GKI
 *            double *GKD
 *            double *KForm
 *            double *FeedForwardGainAcceleration
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall PositionerCorrectorPIDFFAccelerationGet (int SocketIndex, char * PositionerName, bool * ClosedLoopStatus, double * KP, double * KI, double * KD, double * KS, double * IntegrationTime, double * DerivativeFilterCutOffFrequency, double * GKP, double * GKI, double * GKD, double * KForm, double * FeedForwardGainAcceleration) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 
	int boolScanTmp;

	/* Convert to string */ 
	sprintf (ExecuteMethod, "PositionerCorrectorPIDFFAccelerationGet (%s,bool *,double *,double *,double *,double *,double *,double *,double *,double *,double *,double *,double *)", PositionerName);

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (ret == 0) 
	{ 
		char * pt;
		char * ptNext;

		pt = ReturnedValue;
		ptNext = NULL;
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%d", &boolScanTmp);
		*ClosedLoopStatus = (bool) boolScanTmp;
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", KP);
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", KI);
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", KD);
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", KS);
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", IntegrationTime);
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", DerivativeFilterCutOffFrequency);
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", GKP);
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", GKI);
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", GKD);
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", KForm);
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", FeedForwardGainAcceleration);
	} 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * PositionerCorrectorPIDFFVelocitySet :  Update corrector parameters
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *PositionerName
 *            bool ClosedLoopStatus
 *            double KP
 *            double KI
 *            double KD
 *            double KS
 *            double IntegrationTime
 *            double DerivativeFilterCutOffFrequency
 *            double GKP
 *            double GKI
 *            double GKD
 *            double KForm
 *            double FeedForwardGainVelocity
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall PositionerCorrectorPIDFFVelocitySet (int SocketIndex, char * PositionerName, bool ClosedLoopStatus, double KP, double KI, double KD, double KS, double IntegrationTime, double DerivativeFilterCutOffFrequency, double GKP, double GKI, double GKD, double KForm, double FeedForwardGainVelocity) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "PositionerCorrectorPIDFFVelocitySet (%s,%d,%.13g,%.13g,%.13g,%.13g,%.13g,%.13g,%.13g,%.13g,%.13g,%.13g,%.13g)", PositionerName, ClosedLoopStatus, KP, KI, KD, KS, IntegrationTime, DerivativeFilterCutOffFrequency, GKP, GKI, GKD, KForm, FeedForwardGainVelocity);

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * PositionerCorrectorPIDFFVelocityGet :  Read corrector parameters
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *PositionerName
 *            bool *ClosedLoopStatus
 *            double *KP
 *            double *KI
 *            double *KD
 *            double *KS
 *            double *IntegrationTime
 *            double *DerivativeFilterCutOffFrequency
 *            double *GKP
 *            double *GKI
 *            double *GKD
 *            double *KForm
 *            double *FeedForwardGainVelocity
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall PositionerCorrectorPIDFFVelocityGet (int SocketIndex, char * PositionerName, bool * ClosedLoopStatus, double * KP, double * KI, double * KD, double * KS, double * IntegrationTime, double * DerivativeFilterCutOffFrequency, double * GKP, double * GKI, double * GKD, double * KForm, double * FeedForwardGainVelocity) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 
	int boolScanTmp;

	/* Convert to string */ 
	sprintf (ExecuteMethod, "PositionerCorrectorPIDFFVelocityGet (%s,bool *,double *,double *,double *,double *,double *,double *,double *,double *,double *,double *,double *)", PositionerName);

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (ret == 0) 
	{ 
		char * pt;
		char * ptNext;

		pt = ReturnedValue;
		ptNext = NULL;
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%d", &boolScanTmp);
		*ClosedLoopStatus = (bool) boolScanTmp;
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", KP);
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", KI);
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", KD);
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", KS);
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", IntegrationTime);
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", DerivativeFilterCutOffFrequency);
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", GKP);
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", GKI);
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", GKD);
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", KForm);
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", FeedForwardGainVelocity);
	} 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * PositionerCorrectorPIDDualFFVoltageSet :  Update corrector parameters
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *PositionerName
 *            bool ClosedLoopStatus
 *            double KP
 *            double KI
 *            double KD
 *            double KS
 *            double IntegrationTime
 *            double DerivativeFilterCutOffFrequency
 *            double GKP
 *            double GKI
 *            double GKD
 *            double KForm
 *            double FeedForwardGainVelocity
 *            double FeedForwardGainAcceleration
 *            double Friction
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall PositionerCorrectorPIDDualFFVoltageSet (int SocketIndex, char * PositionerName, bool ClosedLoopStatus, double KP, double KI, double KD, double KS, double IntegrationTime, double DerivativeFilterCutOffFrequency, double GKP, double GKI, double GKD, double KForm, double FeedForwardGainVelocity, double FeedForwardGainAcceleration, double Friction) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "PositionerCorrectorPIDDualFFVoltageSet (%s,%d,%.13g,%.13g,%.13g,%.13g,%.13g,%.13g,%.13g,%.13g,%.13g,%.13g,%.13g,%.13g,%.13g)", PositionerName, ClosedLoopStatus, KP, KI, KD, KS, IntegrationTime, DerivativeFilterCutOffFrequency, GKP, GKI, GKD, KForm, FeedForwardGainVelocity, FeedForwardGainAcceleration, Friction);

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * PositionerCorrectorPIDDualFFVoltageGet :  Read corrector parameters
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *PositionerName
 *            bool *ClosedLoopStatus
 *            double *KP
 *            double *KI
 *            double *KD
 *            double *KS
 *            double *IntegrationTime
 *            double *DerivativeFilterCutOffFrequency
 *            double *GKP
 *            double *GKI
 *            double *GKD
 *            double *KForm
 *            double *FeedForwardGainVelocity
 *            double *FeedForwardGainAcceleration
 *            double *Friction
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall PositionerCorrectorPIDDualFFVoltageGet (int SocketIndex, char * PositionerName, bool * ClosedLoopStatus, double * KP, double * KI, double * KD, double * KS, double * IntegrationTime, double * DerivativeFilterCutOffFrequency, double * GKP, double * GKI, double * GKD, double * KForm, double * FeedForwardGainVelocity, double * FeedForwardGainAcceleration, double * Friction) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 
	int boolScanTmp;

	/* Convert to string */ 
	sprintf (ExecuteMethod, "PositionerCorrectorPIDDualFFVoltageGet (%s,bool *,double *,double *,double *,double *,double *,double *,double *,double *,double *,double *,double *,double *,double *)", PositionerName);

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (ret == 0) 
	{ 
		char * pt;
		char * ptNext;

		pt = ReturnedValue;
		ptNext = NULL;
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%d", &boolScanTmp);
		*ClosedLoopStatus = (bool) boolScanTmp;
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", KP);
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", KI);
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", KD);
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", KS);
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", IntegrationTime);
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", DerivativeFilterCutOffFrequency);
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", GKP);
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", GKI);
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", GKD);
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", KForm);
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", FeedForwardGainVelocity);
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", FeedForwardGainAcceleration);
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", Friction);
	} 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * PositionerCorrectorPIPositionSet :  Update corrector parameters
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *PositionerName
 *            bool ClosedLoopStatus
 *            double KP
 *            double KI
 *            double IntegrationTime
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall PositionerCorrectorPIPositionSet (int SocketIndex, char * PositionerName, bool ClosedLoopStatus, double KP, double KI, double IntegrationTime) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "PositionerCorrectorPIPositionSet (%s,%d,%.13g,%.13g,%.13g)", PositionerName, ClosedLoopStatus, KP, KI, IntegrationTime);

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * PositionerCorrectorPIPositionGet :  Read corrector parameters
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *PositionerName
 *            bool *ClosedLoopStatus
 *            double *KP
 *            double *KI
 *            double *IntegrationTime
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall PositionerCorrectorPIPositionGet (int SocketIndex, char * PositionerName, bool * ClosedLoopStatus, double * KP, double * KI, double * IntegrationTime) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 
	int boolScanTmp;

	/* Convert to string */ 
	sprintf (ExecuteMethod, "PositionerCorrectorPIPositionGet (%s,bool *,double *,double *,double *)", PositionerName);

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (ret == 0) 
	{ 
		char * pt;
		char * ptNext;

		pt = ReturnedValue;
		ptNext = NULL;
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%d", &boolScanTmp);
		*ClosedLoopStatus = (bool) boolScanTmp;
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", KP);
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", KI);
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", IntegrationTime);
	} 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * PositionerCorrectorTypeGet :  Read corrector type
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *PositionerName
 *            char *CorrectorType
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall PositionerCorrectorTypeGet (int SocketIndex, char * PositionerName, char * CorrectorType) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "PositionerCorrectorTypeGet (%s,char *)", PositionerName);

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (ret == 0) 
	{ 
		char * pt;
		char * ptNext;

		pt = ReturnedValue;
		ptNext = NULL;
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) strcpy (CorrectorType, pt);
		ptNext = strchr (CorrectorType, ',');
		if (ptNext != NULL) *ptNext = '\0';
	} 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * PositionerCurrentVelocityAccelerationFiltersSet :  Set current velocity and acceleration cut off frequencies
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *PositionerName
 *            double CurrentVelocityCutOffFrequency
 *            double CurrentAccelerationCutOffFrequency
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall PositionerCurrentVelocityAccelerationFiltersSet (int SocketIndex, char * PositionerName, double CurrentVelocityCutOffFrequency, double CurrentAccelerationCutOffFrequency) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "PositionerCurrentVelocityAccelerationFiltersSet (%s,%.13g,%.13g)", PositionerName, CurrentVelocityCutOffFrequency, CurrentAccelerationCutOffFrequency);

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * PositionerCurrentVelocityAccelerationFiltersGet :  Get current velocity and acceleration cut off frequencies
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *PositionerName
 *            double *CurrentVelocityCutOffFrequency
 *            double *CurrentAccelerationCutOffFrequency
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall PositionerCurrentVelocityAccelerationFiltersGet (int SocketIndex, char * PositionerName, double * CurrentVelocityCutOffFrequency, double * CurrentAccelerationCutOffFrequency) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "PositionerCurrentVelocityAccelerationFiltersGet (%s,double *,double *)", PositionerName);

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (ret == 0) 
	{ 
		char * pt;
		char * ptNext;

		pt = ReturnedValue;
		ptNext = NULL;
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", CurrentVelocityCutOffFrequency);
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", CurrentAccelerationCutOffFrequency);
	} 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * PositionerDriverStatusGet :  Read positioner driver status
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *PositionerName
 *            int *DriverStatus
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall PositionerDriverStatusGet (int SocketIndex, char * PositionerName, int * DriverStatus) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "PositionerDriverStatusGet (%s,int *)", PositionerName);

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (ret == 0) 
	{ 
		char * pt;
		char * ptNext;

		pt = ReturnedValue;
		ptNext = NULL;
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%d", DriverStatus);
	} 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * PositionerDriverStatusStringGet :  Return the positioner driver status string corresponding to the positioner error code
 *
 *     - Parameters :
 *            int SocketIndex
 *            int PositionerDriverStatus
 *            char *PositionerDriverStatusString
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall PositionerDriverStatusStringGet (int SocketIndex, int PositionerDriverStatus, char * PositionerDriverStatusString) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_NOMINAL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "PositionerDriverStatusStringGet (%d,char *)", PositionerDriverStatus);

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_NOMINAL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (ret == 0) 
	{ 
		char * pt;
		char * ptNext;

		pt = ReturnedValue;
		ptNext = NULL;
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) strcpy (PositionerDriverStatusString, pt);
		ptNext = strchr (PositionerDriverStatusString, ',');
		if (ptNext != NULL) *ptNext = '\0';
	} 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * PositionerEncoderAmplitudeValuesGet :  Read analog interpolated encoder amplitude values
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *PositionerName
 *            double *CalibrationSinusAmplitude
 *            double *CurrentSinusAmplitude
 *            double *CalibrationCosinusAmplitude
 *            double *CurrentCosinusAmplitude
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall PositionerEncoderAmplitudeValuesGet (int SocketIndex, char * PositionerName, double * CalibrationSinusAmplitude, double * CurrentSinusAmplitude, double * CalibrationCosinusAmplitude, double * CurrentCosinusAmplitude) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "PositionerEncoderAmplitudeValuesGet (%s,double *,double *,double *,double *)", PositionerName);

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (ret == 0) 
	{ 
		char * pt;
		char * ptNext;

		pt = ReturnedValue;
		ptNext = NULL;
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", CalibrationSinusAmplitude);
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", CurrentSinusAmplitude);
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", CalibrationCosinusAmplitude);
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", CurrentCosinusAmplitude);
	} 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * PositionerEncoderCalibrationParametersGet :  Read analog interpolated encoder calibration parameters
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *PositionerName
 *            double *SinusOffset
 *            double *CosinusOffset
 *            double *DifferentialGain
 *            double *PhaseCompensation
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall PositionerEncoderCalibrationParametersGet (int SocketIndex, char * PositionerName, double * SinusOffset, double * CosinusOffset, double * DifferentialGain, double * PhaseCompensation) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "PositionerEncoderCalibrationParametersGet (%s,double *,double *,double *,double *)", PositionerName);

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (ret == 0) 
	{ 
		char * pt;
		char * ptNext;

		pt = ReturnedValue;
		ptNext = NULL;
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", SinusOffset);
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", CosinusOffset);
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", DifferentialGain);
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", PhaseCompensation);
	} 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * PositionerErrorGet :  Read and clear positioner error code
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *PositionerName
 *            int *ErrorCode
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall PositionerErrorGet (int SocketIndex, char * PositionerName, int * ErrorCode) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "PositionerErrorGet (%s,int *)", PositionerName);

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (ret == 0) 
	{ 
		char * pt;
		char * ptNext;

		pt = ReturnedValue;
		ptNext = NULL;
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%d", ErrorCode);
	} 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * PositionerErrorRead :  Read only positioner error code without clear it
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *PositionerName
 *            int *ErrorCode
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall PositionerErrorRead (int SocketIndex, char * PositionerName, int * ErrorCode) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "PositionerErrorRead (%s,int *)", PositionerName);

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (ret == 0) 
	{ 
		char * pt;
		char * ptNext;

		pt = ReturnedValue;
		ptNext = NULL;
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%d", ErrorCode);
	} 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * PositionerErrorStringGet :  Return the positioner status string corresponding to the positioner error code
 *
 *     - Parameters :
 *            int SocketIndex
 *            int PositionerErrorCode
 *            char *PositionerErrorString
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall PositionerErrorStringGet (int SocketIndex, int PositionerErrorCode, char * PositionerErrorString) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_NOMINAL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "PositionerErrorStringGet (%d,char *)", PositionerErrorCode);

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_NOMINAL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (ret == 0) 
	{ 
		char * pt;
		char * ptNext;

		pt = ReturnedValue;
		ptNext = NULL;
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) strcpy (PositionerErrorString, pt);
		ptNext = strchr (PositionerErrorString, ',');
		if (ptNext != NULL) *ptNext = '\0';
	} 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * PositionerHardwareStatusGet :  Read positioner hardware status
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *PositionerName
 *            int *HardwareStatus
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall PositionerHardwareStatusGet (int SocketIndex, char * PositionerName, int * HardwareStatus) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "PositionerHardwareStatusGet (%s,int *)", PositionerName);

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (ret == 0) 
	{ 
		char * pt;
		char * ptNext;

		pt = ReturnedValue;
		ptNext = NULL;
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%d", HardwareStatus);
	} 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * PositionerHardwareStatusStringGet :  Return the positioner hardware status string corresponding to the positioner error code
 *
 *     - Parameters :
 *            int SocketIndex
 *            int PositionerHardwareStatus
 *            char *PositionerHardwareStatusString
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall PositionerHardwareStatusStringGet (int SocketIndex, int PositionerHardwareStatus, char * PositionerHardwareStatusString) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_NOMINAL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "PositionerHardwareStatusStringGet (%d,char *)", PositionerHardwareStatus);

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_NOMINAL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (ret == 0) 
	{ 
		char * pt;
		char * ptNext;

		pt = ReturnedValue;
		ptNext = NULL;
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) strcpy (PositionerHardwareStatusString, pt);
		ptNext = strchr (PositionerHardwareStatusString, ',');
		if (ptNext != NULL) *ptNext = '\0';
	} 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * PositionerHardInterpolatorFactorGet :  Get hard interpolator parameters
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *PositionerName
 *            int *InterpolationFactor
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall PositionerHardInterpolatorFactorGet (int SocketIndex, char * PositionerName, int * InterpolationFactor) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "PositionerHardInterpolatorFactorGet (%s,int *)", PositionerName);

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (ret == 0) 
	{ 
		char * pt;
		char * ptNext;

		pt = ReturnedValue;
		ptNext = NULL;
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%d", InterpolationFactor);
	} 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * PositionerHardInterpolatorFactorSet :  Set hard interpolator parameters
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *PositionerName
 *            int InterpolationFactor
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall PositionerHardInterpolatorFactorSet (int SocketIndex, char * PositionerName, int InterpolationFactor) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "PositionerHardInterpolatorFactorSet (%s,%d)", PositionerName, InterpolationFactor);

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * PositionerMaximumVelocityAndAccelerationGet :  Return maximum velocity and acceleration of the positioner
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *PositionerName
 *            double *MaximumVelocity
 *            double *MaximumAcceleration
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall PositionerMaximumVelocityAndAccelerationGet (int SocketIndex, char * PositionerName, double * MaximumVelocity, double * MaximumAcceleration) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "PositionerMaximumVelocityAndAccelerationGet (%s,double *,double *)", PositionerName);

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (ret == 0) 
	{ 
		char * pt;
		char * ptNext;

		pt = ReturnedValue;
		ptNext = NULL;
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", MaximumVelocity);
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", MaximumAcceleration);
	} 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * PositionerMotionDoneGet :  Read motion done parameters
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *PositionerName
 *            double *PositionWindow
 *            double *VelocityWindow
 *            double *CheckingTime
 *            double *MeanPeriod
 *            double *TimeOut
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall PositionerMotionDoneGet (int SocketIndex, char * PositionerName, double * PositionWindow, double * VelocityWindow, double * CheckingTime, double * MeanPeriod, double * TimeOut) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "PositionerMotionDoneGet (%s,double *,double *,double *,double *,double *)", PositionerName);

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (ret == 0) 
	{ 
		char * pt;
		char * ptNext;

		pt = ReturnedValue;
		ptNext = NULL;
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", PositionWindow);
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", VelocityWindow);
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", CheckingTime);
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", MeanPeriod);
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", TimeOut);
	} 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * PositionerMotionDoneSet :  Update motion done parameters
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *PositionerName
 *            double PositionWindow
 *            double VelocityWindow
 *            double CheckingTime
 *            double MeanPeriod
 *            double TimeOut
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall PositionerMotionDoneSet (int SocketIndex, char * PositionerName, double PositionWindow, double VelocityWindow, double CheckingTime, double MeanPeriod, double TimeOut) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "PositionerMotionDoneSet (%s,%.13g,%.13g,%.13g,%.13g,%.13g)", PositionerName, PositionWindow, VelocityWindow, CheckingTime, MeanPeriod, TimeOut);

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * PositionerSGammaExactVelocityAjustedDisplacementGet :  Return adjusted displacement to get exact velocity
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *PositionerName
 *            double DesiredDisplacement
 *            double *AdjustedDisplacement
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall PositionerSGammaExactVelocityAjustedDisplacementGet (int SocketIndex, char * PositionerName, double DesiredDisplacement, double * AdjustedDisplacement) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "PositionerSGammaExactVelocityAjustedDisplacementGet (%s,%.13g,double *)", PositionerName, DesiredDisplacement);

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (ret == 0) 
	{ 
		char * pt;
		char * ptNext;

		pt = ReturnedValue;
		ptNext = NULL;
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", AdjustedDisplacement);
	} 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * PositionerSGammaParametersGet :  Read dynamic parameters for one axe of a group for a future displacement 
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *PositionerName
 *            double *Velocity
 *            double *Acceleration
 *            double *MinimumTjerkTime
 *            double *MaximumTjerkTime
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall PositionerSGammaParametersGet (int SocketIndex, char * PositionerName, double * Velocity, double * Acceleration, double * MinimumTjerkTime, double * MaximumTjerkTime) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "PositionerSGammaParametersGet (%s,double *,double *,double *,double *)", PositionerName);

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (ret == 0) 
	{ 
		char * pt;
		char * ptNext;

		pt = ReturnedValue;
		ptNext = NULL;
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", Velocity);
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", Acceleration);
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", MinimumTjerkTime);
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", MaximumTjerkTime);
	} 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * PositionerSGammaParametersSet :  Update dynamic parameters for one axe of a group for a future displacement
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *PositionerName
 *            double Velocity
 *            double Acceleration
 *            double MinimumTjerkTime
 *            double MaximumTjerkTime
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall PositionerSGammaParametersSet (int SocketIndex, char * PositionerName, double Velocity, double Acceleration, double MinimumTjerkTime, double MaximumTjerkTime) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "PositionerSGammaParametersSet (%s,%.13g,%.13g,%.13g,%.13g)", PositionerName, Velocity, Acceleration, MinimumTjerkTime, MaximumTjerkTime);

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * PositionerSGammaParametersDistanceGet :  Returns distance during acceleration phase and distance during constant velocity phase 
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *PositionerName
 *            double Displacement
 *            double Velocity
 *            double Acceleration
 *            double MinJerkTime
 *            double MaxJerkTime
 *            double *DisplacementDuringAcc
 *            double *DisplacementDuringVel
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall PositionerSGammaParametersDistanceGet (int SocketIndex, char * PositionerName, double Displacement, double Velocity, double Acceleration, double MinJerkTime, double MaxJerkTime, double * DisplacementDuringAcc, double * DisplacementDuringVel) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "PositionerSGammaParametersDistanceGet (%s,%.13g,%.13g,%.13g,%.13g,%.13g,double *,double *)", PositionerName, Displacement, Velocity, Acceleration, MinJerkTime, MaxJerkTime);

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (ret == 0) 
	{ 
		char * pt;
		char * ptNext;

		pt = ReturnedValue;
		ptNext = NULL;
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", DisplacementDuringAcc);
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", DisplacementDuringVel);
	} 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * PositionerSGammaPreviousMotionTimesGet :  Read SettingTime and SettlingTime
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *PositionerName
 *            double *SettingTime
 *            double *SettlingTime
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall PositionerSGammaPreviousMotionTimesGet (int SocketIndex, char * PositionerName, double * SettingTime, double * SettlingTime) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "PositionerSGammaPreviousMotionTimesGet (%s,double *,double *)", PositionerName);

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (ret == 0) 
	{ 
		char * pt;
		char * ptNext;

		pt = ReturnedValue;
		ptNext = NULL;
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", SettingTime);
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", SettlingTime);
	} 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * PositionerStageParameterGet :  Return the stage parameter
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *PositionerName
 *            char *ParameterName
 *            char *ParameterValue
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall PositionerStageParameterGet (int SocketIndex, char * PositionerName, char * ParameterName, char * ParameterValue) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "PositionerStageParameterGet (%s,%s,char *)", PositionerName, ParameterName);

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (ret == 0) 
	{ 
		char * pt;
		char * ptNext;

		pt = ReturnedValue;
		ptNext = NULL;
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) strcpy (ParameterValue, pt);
		ptNext = strchr (ParameterValue, ',');
		if (ptNext != NULL) *ptNext = '\0';
	} 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * PositionerStageParameterSet :  Save the stage parameter
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *PositionerName
 *            char *ParameterName
 *            char *ParameterValue
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall PositionerStageParameterSet (int SocketIndex, char * PositionerName, char * ParameterName, char * ParameterValue) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "PositionerStageParameterSet (%s,%s,%s)", PositionerName, ParameterName, ParameterValue);

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * PositionerUserTravelLimitsGet :  Read UserMinimumTarget and UserMaximumTarget
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *PositionerName
 *            double *UserMinimumTarget
 *            double *UserMaximumTarget
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall PositionerUserTravelLimitsGet (int SocketIndex, char * PositionerName, double * UserMinimumTarget, double * UserMaximumTarget) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "PositionerUserTravelLimitsGet (%s,double *,double *)", PositionerName);

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (ret == 0) 
	{ 
		char * pt;
		char * ptNext;

		pt = ReturnedValue;
		ptNext = NULL;
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", UserMinimumTarget);
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", UserMaximumTarget);
	} 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * PositionerUserTravelLimitsSet :  Update UserMinimumTarget and UserMaximumTarget
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *PositionerName
 *            double UserMinimumTarget
 *            double UserMaximumTarget
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall PositionerUserTravelLimitsSet (int SocketIndex, char * PositionerName, double UserMinimumTarget, double UserMaximumTarget) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "PositionerUserTravelLimitsSet (%s,%.13g,%.13g)", PositionerName, UserMinimumTarget, UserMaximumTarget);

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * HexapodMoveAbsolute :  Hexapod absolute move in a specific coordinate system
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *GroupName
 *            char *CoordinateSystem
 *            double X
 *            double Y
 *            double Z
 *            double U
 *            double V
 *            double W
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall HexapodMoveAbsolute (int SocketIndex, char * GroupName, char * CoordinateSystem, double X, double Y, double Z, double U, double V, double W) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "HexapodMoveAbsolute (%s,%s,%.13g,%.13g,%.13g,%.13g,%.13g,%.13g)", GroupName, CoordinateSystem, X, Y, Z, U, V, W);

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * HexapodMoveIncremental :  Hexapod incremental move in a specific coordinate system
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *GroupName
 *            char *CoordinateSystem
 *            double dX
 *            double dY
 *            double dZ
 *            double dU
 *            double dV
 *            double dW
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall HexapodMoveIncremental (int SocketIndex, char * GroupName, char * CoordinateSystem, double dX, double dY, double dZ, double dU, double dV, double dW) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "HexapodMoveIncremental (%s,%s,%.13g,%.13g,%.13g,%.13g,%.13g,%.13g)", GroupName, CoordinateSystem, dX, dY, dZ, dU, dV, dW);

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * HexapodCoordinatesGet :  Get coordinates in a specific coordinate system of a point specified in another coordinate system
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *GroupName
 *            char *CoordinateSystemIn
 *            char *CoordinateSystemOut
 *            double Xin
 *            double Yin
 *            double Zin
 *            double Uin
 *            double Vin
 *            double Win
 *            double *Xout
 *            double *Yout
 *            double *Zout
 *            double *Uout
 *            double *Vout
 *            double *Wout
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall HexapodCoordinatesGet (int SocketIndex, char * GroupName, char * CoordinateSystemIn, char * CoordinateSystemOut, double Xin, double Yin, double Zin, double Uin, double Vin, double Win, double * Xout, double * Yout, double * Zout, double * Uout, double * Vout, double * Wout) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "HexapodCoordinatesGet (%s,%s,%s,%.13g,%.13g,%.13g,%.13g,%.13g,%.13g,double *,double *,double *,double *,double *,double *)", GroupName, CoordinateSystemIn, CoordinateSystemOut, Xin, Yin, Zin, Uin, Vin, Win);

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (ret == 0) 
	{ 
		char * pt;
		char * ptNext;

		pt = ReturnedValue;
		ptNext = NULL;
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", Xout);
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", Yout);
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", Zout);
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", Uout);
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", Vout);
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", Wout);
	} 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * HexapodCoordinateSystemSet :  Modify the position of a coordinate system
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *GroupName
 *            char *CoordinateSystem
 *            double X
 *            double Y
 *            double Z
 *            double U
 *            double V
 *            double W
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall HexapodCoordinateSystemSet (int SocketIndex, char * GroupName, char * CoordinateSystem, double X, double Y, double Z, double U, double V, double W) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "HexapodCoordinateSystemSet (%s,%s,%.13g,%.13g,%.13g,%.13g,%.13g,%.13g)", GroupName, CoordinateSystem, X, Y, Z, U, V, W);

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * HexapodCoordinateSystemGet :  Get the position of a coordinate system
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *GroupName
 *            char *CoordinateSystem
 *            double *X
 *            double *Y
 *            double *Z
 *            double *U
 *            double *V
 *            double *W
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall HexapodCoordinateSystemGet (int SocketIndex, char * GroupName, char * CoordinateSystem, double * X, double * Y, double * Z, double * U, double * V, double * W) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "HexapodCoordinateSystemGet (%s,%s,double *,double *,double *,double *,double *,double *)", GroupName, CoordinateSystem);

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (ret == 0) 
	{ 
		char * pt;
		char * ptNext;

		pt = ReturnedValue;
		ptNext = NULL;
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", X);
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", Y);
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", Z);
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", U);
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", V);
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", W);
	} 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * HexapodMoveIncrementalControl :  Hexapod trajectory (Line, Arc or Rotation) execution with the maximum velocity 
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *GroupName
 *            char *CoordinateSystem
 *            char *HexapodTrajectoryType
 *            double dX
 *            double dY
 *            double dZ
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall HexapodMoveIncrementalControl (int SocketIndex, char * GroupName, char * CoordinateSystem, char * HexapodTrajectoryType, double dX, double dY, double dZ) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "HexapodMoveIncrementalControl (%s,%s,%s,%.13g,%.13g,%.13g)", GroupName, CoordinateSystem, HexapodTrajectoryType, dX, dY, dZ);

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * HexapodMoveIncrementalControlWithTargetVelocity :  Hexapod trajectory (Line, Arc or Rotation) execution with a target velocity 
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *GroupName
 *            char *CoordinateSystem
 *            char *HexapodTrajectoryType
 *            double dX
 *            double dY
 *            double dZ
 *            double Velocity
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall HexapodMoveIncrementalControlWithTargetVelocity (int SocketIndex, char * GroupName, char * CoordinateSystem, char * HexapodTrajectoryType, double dX, double dY, double dZ, double Velocity) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "HexapodMoveIncrementalControlWithTargetVelocity (%s,%s,%s,%.13g,%.13g,%.13g,%.13g)", GroupName, CoordinateSystem, HexapodTrajectoryType, dX, dY, dZ, Velocity);

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * HexapodMoveIncrementalControlPulseAndGatheringSet :  Configure gathering with pulses : gathered data are X, Y, Z, U, V, W and pulses will be generated during only constant velocity
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *GroupName
 *            int Divisor
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall HexapodMoveIncrementalControlPulseAndGatheringSet (int SocketIndex, char * GroupName, int Divisor) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "HexapodMoveIncrementalControlPulseAndGatheringSet (%s,%d)", GroupName, Divisor);

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * HexapodMoveIncrementalControlLimitGet :  Returns the maximum velocity of carriage and the percent of the trajectory executable
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *GroupName
 *            char *CoordinateSystem
 *            char *HexapodTrajectoryType
 *            double dX
 *            double dY
 *            double dZ
 *            double *MaximumVelocityCarriage
 *            double *TrajectoryPercent
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall HexapodMoveIncrementalControlLimitGet (int SocketIndex, char * GroupName, char * CoordinateSystem, char * HexapodTrajectoryType, double dX, double dY, double dZ, double * MaximumVelocityCarriage, double * TrajectoryPercent) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "HexapodMoveIncrementalControlLimitGet (%s,%s,%s,%.13g,%.13g,%.13g,double *,double *)", GroupName, CoordinateSystem, HexapodTrajectoryType, dX, dY, dZ);

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (ret == 0) 
	{ 
		char * pt;
		char * ptNext;

		pt = ReturnedValue;
		ptNext = NULL;
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", MaximumVelocityCarriage);
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", TrajectoryPercent);
	} 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * HexapodSGammaParametersDistanceGet :  Returns distance during acceleration phase and distance during constant velocity phase for a virtual SGamma profiler
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *PositionerName
 *            double Displacement
 *            double Velocity
 *            double Acceleration
 *            double MinJerkTime
 *            double MaxJerkTime
 *            double *DisplacementDuringAcc
 *            double *DisplacementDuringVel
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall HexapodSGammaParametersDistanceGet (int SocketIndex, char * PositionerName, double Displacement, double Velocity, double Acceleration, double MinJerkTime, double MaxJerkTime, double * DisplacementDuringAcc, double * DisplacementDuringVel) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "HexapodSGammaParametersDistanceGet (%s,%.13g,%.13g,%.13g,%.13g,%.13g,double *,double *)", PositionerName, Displacement, Velocity, Acceleration, MinJerkTime, MaxJerkTime);

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (ret == 0) 
	{ 
		char * pt;
		char * ptNext;

		pt = ReturnedValue;
		ptNext = NULL;
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", DisplacementDuringAcc);
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", DisplacementDuringVel);
	} 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * SingleAxisSlaveModeEnable :  Enable the slave mode
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *GroupName
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall SingleAxisSlaveModeEnable (int SocketIndex, char * GroupName) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "SingleAxisSlaveModeEnable (%s)", GroupName);

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * SingleAxisSlaveModeDisable :  Disable the slave mode
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *GroupName
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall SingleAxisSlaveModeDisable (int SocketIndex, char * GroupName) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "SingleAxisSlaveModeDisable (%s)", GroupName);

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * SingleAxisSlaveParametersSet :  Set slave parameters
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *GroupName
 *            char *PositionerName
 *            double Ratio
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall SingleAxisSlaveParametersSet (int SocketIndex, char * GroupName, char * PositionerName, double Ratio) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "SingleAxisSlaveParametersSet (%s,%s,%.13g)", GroupName, PositionerName, Ratio);

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * SingleAxisSlaveParametersGet :  Get slave parameters
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *GroupName
 *            char *PositionerName
 *            double *Ratio
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall SingleAxisSlaveParametersGet (int SocketIndex, char * GroupName, char * PositionerName, double * Ratio) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "SingleAxisSlaveParametersGet (%s,char *,double *)", GroupName);

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (ret == 0) 
	{ 
		char * pt;
		char * ptNext;

		pt = ReturnedValue;
		ptNext = NULL;
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) strcpy (PositionerName, pt);
		ptNext = strchr (PositionerName, ',');
		if (ptNext != NULL) *ptNext = '\0';
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", Ratio);
	} 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * OptionalModuleExecute :  Execute an optional module
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *ModuleFileName
 *            char *TaskName
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall OptionalModuleExecute (int SocketIndex, char * ModuleFileName, char * TaskName) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "OptionalModuleExecute (%s,%s)", ModuleFileName, TaskName);

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * OptionalModuleKill :  Kill an optional module
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *TaskName
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall OptionalModuleKill (int SocketIndex, char * TaskName) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "OptionalModuleKill (%s)", TaskName);

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * ControllerStatusGet :  Read controller current status
 *
 *     - Parameters :
 *            int SocketIndex
 *            int *ControllerStatus
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall ControllerStatusGet (int SocketIndex, int * ControllerStatus) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "ControllerStatusGet (int *)");

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (ret == 0) 
	{ 
		char * pt;
		char * ptNext;

		pt = ReturnedValue;
		ptNext = NULL;
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%d", ControllerStatus);
	} 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * ControllerStatusStringGet :  Return the controller status string corresponding to the controller status code
 *
 *     - Parameters :
 *            int SocketIndex
 *            int ControllerStatusCode
 *            char *ControllerStatusString
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall ControllerStatusStringGet (int SocketIndex, int ControllerStatusCode, char * ControllerStatusString) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "ControllerStatusStringGet (%d,char *)", ControllerStatusCode);

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (ret == 0) 
	{ 
		char * pt;
		char * ptNext;

		pt = ReturnedValue;
		ptNext = NULL;
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) strcpy (ControllerStatusString, pt);
		ptNext = strchr (ControllerStatusString, ',');
		if (ptNext != NULL) *ptNext = '\0';
	} 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * EEPROMCIESet :  Set CIE EEPROM reference string
 *
 *     - Parameters :
 *            int SocketIndex
 *            int CardNumber
 *            char *ReferenceString
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall EEPROMCIESet (int SocketIndex, int CardNumber, char * ReferenceString) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "EEPROMCIESet (%d,%s)", CardNumber, ReferenceString);

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * EEPROMDACOffsetCIESet :  Set CIE DAC offsets
 *
 *     - Parameters :
 *            int SocketIndex
 *            int PlugNumber
 *            double DAC1Offset
 *            double DAC2Offset
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall EEPROMDACOffsetCIESet (int SocketIndex, int PlugNumber, double DAC1Offset, double DAC2Offset) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "EEPROMDACOffsetCIESet (%d,%.13g,%.13g)", PlugNumber, DAC1Offset, DAC2Offset);

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * EEPROMDriverSet :  Set Driver EEPROM reference string
 *
 *     - Parameters :
 *            int SocketIndex
 *            int PlugNumber
 *            char *ReferenceString
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall EEPROMDriverSet (int SocketIndex, int PlugNumber, char * ReferenceString) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "EEPROMDriverSet (%d,%s)", PlugNumber, ReferenceString);

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * EEPROMINTSet :  Set INT EEPROM reference string
 *
 *     - Parameters :
 *            int SocketIndex
 *            int CardNumber
 *            char *ReferenceString
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall EEPROMINTSet (int SocketIndex, int CardNumber, char * ReferenceString) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "EEPROMINTSet (%d,%s)", CardNumber, ReferenceString);

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * CPUCoreAndBoardSupplyVoltagesGet :  Get power informations
 *
 *     - Parameters :
 *            int SocketIndex
 *            double *VoltageCPUCore
 *            double *SupplyVoltage1P5V
 *            double *SupplyVoltage3P3V
 *            double *SupplyVoltage5V
 *            double *SupplyVoltage12V
 *            double *SupplyVoltageM12V
 *            double *SupplyVoltageM5V
 *            double *SupplyVoltage5VSB
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall CPUCoreAndBoardSupplyVoltagesGet (int SocketIndex, double * VoltageCPUCore, double * SupplyVoltage1P5V, double * SupplyVoltage3P3V, double * SupplyVoltage5V, double * SupplyVoltage12V, double * SupplyVoltageM12V, double * SupplyVoltageM5V, double * SupplyVoltage5VSB) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "CPUCoreAndBoardSupplyVoltagesGet (double *,double *,double *,double *,double *,double *,double *,double *)");

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (ret == 0) 
	{ 
		char * pt;
		char * ptNext;

		pt = ReturnedValue;
		ptNext = NULL;
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", VoltageCPUCore);
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", SupplyVoltage1P5V);
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", SupplyVoltage3P3V);
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", SupplyVoltage5V);
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", SupplyVoltage12V);
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", SupplyVoltageM12V);
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", SupplyVoltageM5V);
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", SupplyVoltage5VSB);
	} 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * CPUTemperatureAndFanSpeedGet :  Get CPU temperature and fan speed
 *
 *     - Parameters :
 *            int SocketIndex
 *            double *CPUTemperature
 *            double *CPUFanSpeed
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall CPUTemperatureAndFanSpeedGet (int SocketIndex, double * CPUTemperature, double * CPUFanSpeed) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "CPUTemperatureAndFanSpeedGet (double *,double *)");

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (ret == 0) 
	{ 
		char * pt;
		char * ptNext;

		pt = ReturnedValue;
		ptNext = NULL;
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", CPUTemperature);
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", CPUFanSpeed);
	} 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * RunTraceloggerProcessWithTimeSetting :  Run Tracelogger Process With Time Setting
 *
 *     - Parameters :
 *            int SocketIndex
 *            int TraceloggerTime
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall RunTraceloggerProcessWithTimeSetting (int SocketIndex, int TraceloggerTime) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "RunTraceloggerProcessWithTimeSetting (%d)", TraceloggerTime);

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * RunTraceloggerProcessWithRollingBuffer :  Run Tracelogger Process With Rolling Buffer
 *
 *     - Parameters :
 *            int SocketIndex
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall RunTraceloggerProcessWithRollingBuffer (int SocketIndex) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "RunTraceloggerProcessWithRollingBuffer ()");

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * CreateQNXEvent :  Run Create Event
 *
 *     - Parameters :
 *            int SocketIndex
 *            int EventNumber
 *            char *EventName
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall CreateQNXEvent (int SocketIndex, int EventNumber, char * EventName) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "CreateQNXEvent (%d,%s)", EventNumber, EventName);

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * StartEventsAcqusition :  Start Events Acqusition
 *
 *     - Parameters :
 *            int SocketIndex
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall StartEventsAcqusition (int SocketIndex) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "StartEventsAcqusition ()");

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * StopEventsAcqusition :  Stop Events Acqusition
 *
 *     - Parameters :
 *            int SocketIndex
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall StopEventsAcqusition (int SocketIndex) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "StopEventsAcqusition ()");

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * EventTriggerSet :  Generate internal event
 *
 *     - Parameters :
 *            int SocketIndex
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall EventTriggerSet (int SocketIndex) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "EventTriggerSet ()");

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * ActionListGet :  Action list
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *ActionList
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall ActionListGet (int SocketIndex, char * ActionList) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_BIG+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "ActionListGet (char *)");

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_BIG); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (ret == 0) 
	{ 
		char * pt;
		char * ptNext;

		pt = ReturnedValue;
		ptNext = NULL;
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) strcpy (ActionList, pt);
		ptNext = strchr (ActionList, ',');
		if (ptNext != NULL) *ptNext = '\0';
	} 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * ActionExtendedListGet :  Action extended list
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *ActionList
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall ActionExtendedListGet (int SocketIndex, char * ActionList) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_BIG+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "ActionExtendedListGet (char *)");

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_BIG); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (ret == 0) 
	{ 
		char * pt;
		char * ptNext;

		pt = ReturnedValue;
		ptNext = NULL;
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) strcpy (ActionList, pt);
		ptNext = strchr (ActionList, ',');
		if (ptNext != NULL) *ptNext = '\0';
	} 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * APIExtendedListGet :  API method list
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *Method
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall APIExtendedListGet (int SocketIndex, char * Method) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_HUGE+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "APIExtendedListGet (char *)");

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_HUGE); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (ret == 0) 
	{ 
		char * pt;
		char * ptNext;

		pt = ReturnedValue;
		ptNext = NULL;
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) strcpy (Method, pt);
		ptNext = strchr (Method, ',');
		if (ptNext != NULL) *ptNext = '\0';
	} 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * APIListGet :  API method list without extended API
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *Method
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall APIListGet (int SocketIndex, char * Method) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_HUGE+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "APIListGet (char *)");

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_HUGE); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (ret == 0) 
	{ 
		char * pt;
		char * ptNext;

		pt = ReturnedValue;
		ptNext = NULL;
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) strcpy (Method, pt);
		ptNext = strchr (Method, ',');
		if (ptNext != NULL) *ptNext = '\0';
	} 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * ErrorListGet :  Error list
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *ErrorsList
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall ErrorListGet (int SocketIndex, char * ErrorsList) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_HUGE+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "ErrorListGet (char *)");

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_HUGE); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (ret == 0) 
	{ 
		char * pt;
		char * ptNext;

		pt = ReturnedValue;
		ptNext = NULL;
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) strcpy (ErrorsList, pt);
		ptNext = strchr (ErrorsList, ',');
		if (ptNext != NULL) *ptNext = '\0';
	} 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * EventListGet :  General event list
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *EventList
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall EventListGet (int SocketIndex, char * EventList) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_BIG+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "EventListGet (char *)");

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_BIG); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (ret == 0) 
	{ 
		char * pt;
		char * ptNext;

		pt = ReturnedValue;
		ptNext = NULL;
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) strcpy (EventList, pt);
		ptNext = strchr (EventList, ',');
		if (ptNext != NULL) *ptNext = '\0';
	} 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * GatheringListGet :  Gathering type list
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *list
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall GatheringListGet (int SocketIndex, char * list) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_BIG+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "GatheringListGet (char *)");

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_BIG); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (ret == 0) 
	{ 
		char * pt;
		char * ptNext;

		pt = ReturnedValue;
		ptNext = NULL;
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) strcpy (list, pt);
		ptNext = strchr (list, ',');
		if (ptNext != NULL) *ptNext = '\0';
	} 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * GatheringExtendedListGet :  Gathering type extended list
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *list
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall GatheringExtendedListGet (int SocketIndex, char * list) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_BIG+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "GatheringExtendedListGet (char *)");

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_BIG); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (ret == 0) 
	{ 
		char * pt;
		char * ptNext;

		pt = ReturnedValue;
		ptNext = NULL;
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) strcpy (list, pt);
		ptNext = strchr (list, ',');
		if (ptNext != NULL) *ptNext = '\0';
	} 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * GatheringExternalListGet :  External Gathering type list
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *list
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall GatheringExternalListGet (int SocketIndex, char * list) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_BIG+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "GatheringExternalListGet (char *)");

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_BIG); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (ret == 0) 
	{ 
		char * pt;
		char * ptNext;

		pt = ReturnedValue;
		ptNext = NULL;
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) strcpy (list, pt);
		ptNext = strchr (list, ',');
		if (ptNext != NULL) *ptNext = '\0';
	} 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * GroupStatusListGet :  Group status list
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *GroupStatusList
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall GroupStatusListGet (int SocketIndex, char * GroupStatusList) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_HUGE+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "GroupStatusListGet (char *)");

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_HUGE); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (ret == 0) 
	{ 
		char * pt;
		char * ptNext;

		pt = ReturnedValue;
		ptNext = NULL;
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) strcpy (GroupStatusList, pt);
		ptNext = strchr (GroupStatusList, ',');
		if (ptNext != NULL) *ptNext = '\0';
	} 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * HardwareInternalListGet :  Internal hardware list
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *InternalHardwareList
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall HardwareInternalListGet (int SocketIndex, char * InternalHardwareList) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_NOMINAL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "HardwareInternalListGet (char *)");

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_NOMINAL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (ret == 0) 
	{ 
		char * pt;
		char * ptNext;

		pt = ReturnedValue;
		ptNext = NULL;
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) strcpy (InternalHardwareList, pt);
		ptNext = strchr (InternalHardwareList, ',');
		if (ptNext != NULL) *ptNext = '\0';
	} 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * HardwareDriverAndStageGet :  Smart hardware
 *
 *     - Parameters :
 *            int SocketIndex
 *            int PlugNumber
 *            char *DriverName
 *            char *StageName
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall HardwareDriverAndStageGet (int SocketIndex, int PlugNumber, char * DriverName, char * StageName) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_NOMINAL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "HardwareDriverAndStageGet (%d,char *,char *)", PlugNumber);

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_NOMINAL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (ret == 0) 
	{ 
		char * pt;
		char * ptNext;

		pt = ReturnedValue;
		ptNext = NULL;
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) strcpy (DriverName, pt);
		ptNext = strchr (DriverName, ',');
		if (ptNext != NULL) *ptNext = '\0';
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) strcpy (StageName, pt);
		ptNext = strchr (StageName, ',');
		if (ptNext != NULL) *ptNext = '\0';
	} 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * HexapodTrajectoryListGet :  Hexapod trajectory type list
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *list
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall HexapodTrajectoryListGet (int SocketIndex, char * list) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "HexapodTrajectoryListGet (char *)");

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (ret == 0) 
	{ 
		char * pt;
		char * ptNext;

		pt = ReturnedValue;
		ptNext = NULL;
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) strcpy (list, pt);
		ptNext = strchr (list, ',');
		if (ptNext != NULL) *ptNext = '\0';
	} 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * ObjectsListGet :  Group name and positioner name
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *ObjectsList
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall ObjectsListGet (int SocketIndex, char * ObjectsList) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_HUGE+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "ObjectsListGet (char *)");

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_HUGE); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (ret == 0) 
	{ 
		char * pt;
		char * ptNext;

		pt = ReturnedValue;
		ptNext = NULL;
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) strcpy (ObjectsList, pt);
		ptNext = strchr (ObjectsList, ',');
		if (ptNext != NULL) *ptNext = '\0';
	} 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * PositionerErrorListGet :  Positioner error list
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *PositionerErrorList
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall PositionerErrorListGet (int SocketIndex, char * PositionerErrorList) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_BIG+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "PositionerErrorListGet (char *)");

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_BIG); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (ret == 0) 
	{ 
		char * pt;
		char * ptNext;

		pt = ReturnedValue;
		ptNext = NULL;
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) strcpy (PositionerErrorList, pt);
		ptNext = strchr (PositionerErrorList, ',');
		if (ptNext != NULL) *ptNext = '\0';
	} 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * PositionerHardwareStatusListGet :  Positioner hardware status list
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *PositionerHardwareStatusList
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall PositionerHardwareStatusListGet (int SocketIndex, char * PositionerHardwareStatusList) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_BIG+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "PositionerHardwareStatusListGet (char *)");

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_BIG); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (ret == 0) 
	{ 
		char * pt;
		char * ptNext;

		pt = ReturnedValue;
		ptNext = NULL;
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) strcpy (PositionerHardwareStatusList, pt);
		ptNext = strchr (PositionerHardwareStatusList, ',');
		if (ptNext != NULL) *ptNext = '\0';
	} 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * PositionerDriverStatusListGet :  Positioner driver status list
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *PositionerDriverStatusList
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall PositionerDriverStatusListGet (int SocketIndex, char * PositionerDriverStatusList) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_BIG+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "PositionerDriverStatusListGet (char *)");

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_BIG); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (ret == 0) 
	{ 
		char * pt;
		char * ptNext;

		pt = ReturnedValue;
		ptNext = NULL;
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) strcpy (PositionerDriverStatusList, pt);
		ptNext = strchr (PositionerDriverStatusList, ',');
		if (ptNext != NULL) *ptNext = '\0';
	} 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * ReferencingActionListGet :  Get referencing action list
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *list
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall ReferencingActionListGet (int SocketIndex, char * list) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_BIG+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "ReferencingActionListGet (char *)");

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_BIG); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (ret == 0) 
	{ 
		char * pt;
		char * ptNext;

		pt = ReturnedValue;
		ptNext = NULL;
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) strcpy (list, pt);
		ptNext = strchr (list, ',');
		if (ptNext != NULL) *ptNext = '\0';
	} 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * ReferencingSensorListGet :  Get referencing sensor list
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *list
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall ReferencingSensorListGet (int SocketIndex, char * list) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_BIG+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "ReferencingSensorListGet (char *)");

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_BIG); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (ret == 0) 
	{ 
		char * pt;
		char * ptNext;

		pt = ReturnedValue;
		ptNext = NULL;
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) strcpy (list, pt);
		ptNext = strchr (list, ',');
		if (ptNext != NULL) *ptNext = '\0';
	} 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * GatheringUserDatasGet :  Return UserDatas values
 *
 *     - Parameters :
 *            int SocketIndex
 *            double *UserData1
 *            double *UserData2
 *            double *UserData3
 *            double *UserData4
 *            double *UserData5
 *            double *UserData6
 *            double *UserData7
 *            double *UserData8
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall GatheringUserDatasGet (int SocketIndex, double * UserData1, double * UserData2, double * UserData3, double * UserData4, double * UserData5, double * UserData6, double * UserData7, double * UserData8) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "GatheringUserDatasGet (double *,double *,double *,double *,double *,double *,double *,double *)");

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (ret == 0) 
	{ 
		char * pt;
		char * ptNext;

		pt = ReturnedValue;
		ptNext = NULL;
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", UserData1);
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", UserData2);
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", UserData3);
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", UserData4);
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", UserData5);
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", UserData6);
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", UserData7);
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", UserData8);
	} 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * ControllerMotionKernelPeriodMinMaxGet :  Get controller motion kernel min/max periods
 *
 *     - Parameters :
 *            int SocketIndex
 *            double *MinimumCorrectorPeriod
 *            double *MaximumCorrectorPeriod
 *            double *MinimumProfilerPeriod
 *            double *MaximumProfilerPeriod
 *            double *MinimumServitudesPeriod
 *            double *MaximumServitudesPeriod
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall ControllerMotionKernelPeriodMinMaxGet (int SocketIndex, double * MinimumCorrectorPeriod, double * MaximumCorrectorPeriod, double * MinimumProfilerPeriod, double * MaximumProfilerPeriod, double * MinimumServitudesPeriod, double * MaximumServitudesPeriod) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "ControllerMotionKernelPeriodMinMaxGet (double *,double *,double *,double *,double *,double *)");

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (ret == 0) 
	{ 
		char * pt;
		char * ptNext;

		pt = ReturnedValue;
		ptNext = NULL;
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", MinimumCorrectorPeriod);
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", MaximumCorrectorPeriod);
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", MinimumProfilerPeriod);
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", MaximumProfilerPeriod);
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", MinimumServitudesPeriod);
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) sscanf (pt, "%lf", MaximumServitudesPeriod);
	} 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * ControllerMotionKernelPeriodMinMaxReset :  Reset controller motion kernel min/max periods
 *
 *     - Parameters :
 *            int SocketIndex
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall ControllerMotionKernelPeriodMinMaxReset (int SocketIndex) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "ControllerMotionKernelPeriodMinMaxReset ()");

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * TestTCP :  Test TCP/IP transfert
 *
 *     - Parameters :
 *            int SocketIndex
 *            char *InputString
 *            char *ReturnString
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall TestTCP (int SocketIndex, char * InputString, char * ReturnString) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "TestTCP (%s,char *)", InputString);

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (ret == 0) 
	{ 
		char * pt;
		char * ptNext;

		pt = ReturnedValue;
		ptNext = NULL;
		if (pt != NULL) pt = strchr (pt, ',');
		if (pt != NULL) pt++;
		if (pt != NULL) strcpy (ReturnString, pt);
		ptNext = strchr (ReturnString, ',');
		if (ptNext != NULL) *ptNext = '\0';
	} 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}


/*********************************************************************** 
 * PrepareForUpdate :  Kill QNX processes for firmware update
 *
 *     - Parameters :
 *            int SocketIndex
 *     - Return :
 *            int errorCode
 ***********************************************************************/ 
int __stdcall PrepareForUpdate (int SocketIndex) 
{ 
	int ret = -1; 
	char ExecuteMethod[SIZE_EXECUTE_METHOD+1]; 
	char *ReturnedValue = (char *) malloc (sizeof(char) * (SIZE_SMALL+1)); 

	/* Convert to string */ 
	sprintf (ExecuteMethod, "PrepareForUpdate ()");

	/* Send this string and wait return function from controller */ 
	/* return function : ==0 -> OK ; < 0 -> NOK */ 
	SendAndReceive (SocketIndex, ExecuteMethod, ReturnedValue, SIZE_SMALL); 
	if (strlen (ReturnedValue) > 0) 
		sscanf (ReturnedValue, "%i", &ret); 

	/* Get the returned values in the out parameters */ 
	if (NULL != ReturnedValue)
		free (ReturnedValue);

	return (ret); 
}



#ifdef __cplusplus
}
#endif
