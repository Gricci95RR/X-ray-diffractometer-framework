
#include <iostream>
#include "ConsoleHelper.h"
#include "stringSplit.h"
#include "stringex.h"
#include <string.h>

using namespace stringSplit;

CConsoleHelper::CConsoleHelper(void)
{
	DppLibUsb.NumDevices = 0;
	LibUsb_isConnected = false;
	LibUsb_NumDevices = 0;
	DppStatusString = "";
}

CConsoleHelper::~CConsoleHelper(void)
{
}

bool CConsoleHelper::LibUsb_Connect_Default_DPP()
{

	// flag is for notifications, if already connect will return dusbStatNoAction
	// use bDeviceConnected to detect connection
	DppLibUsb.InitializeLibusb();
	LibUsb_isConnected = false;
	LibUsb_NumDevices = 0;
	DppLibUsb.NumDevices = DppLibUsb.CountDP5LibusbDevices();
	if (DppLibUsb.NumDevices > 1) {
		// choose dpp device here or default to first device
	} else if (DppLibUsb.NumDevices == 1) {
		// default to first device
	} else {
	}
	if (DppLibUsb.NumDevices > 0) {
		DppLibUsb.CurrentDevice = 1;	// set to default device
		DppLibUsb.DppLibusbHandle = DppLibUsb.FindUSBDevice(DppLibUsb.CurrentDevice); //connect
		if (DppLibUsb.bDeviceConnected) { // connection detected
			LibUsb_isConnected = true;
			LibUsb_NumDevices = DppLibUsb.NumDevices;
		}
	} else {
		LibUsb_isConnected = false;
		LibUsb_NumDevices = 0;
	}
	return (LibUsb_isConnected);
}

void CConsoleHelper::LibUsb_Close_Connection()
{
	if (DppLibUsb.bDeviceConnected) { // clean-up: close usb connection
		DppLibUsb.bDeviceConnected = false;
		DppLibUsb.CloseUSBDevice(DppLibUsb.DppLibusbHandle);
		LibUsb_isConnected = false;
		LibUsb_NumDevices = 0;
		DppLibUsb.DeinitializeLibusb();
	}
}

bool CConsoleHelper::LibUsb_SendCommand(TRANSMIT_PACKET_TYPE XmtCmd)
{
    bool bHaveBuffer;
    int bSentPkt;
	bool bMessageSent;

	bMessageSent = false;
	if (DppLibUsb.bDeviceConnected) { 
		memset(&DP5Proto.BufferOUT[0],0,sizeof(DP5Proto.BufferOUT));
		bHaveBuffer = (bool) SndCmd.DP5_CMD(DP5Proto.BufferOUT, XmtCmd);
		if (bHaveBuffer) {
			bSentPkt = DppLibUsb.SendPacketUSB(DppLibUsb.DppLibusbHandle, DP5Proto.BufferOUT, DP5Proto.PacketIn);
			if (bSentPkt) {
	            bMessageSent = true;
			}
		}
	}
	return (bMessageSent);
}

// COMMAND (CONFIG_OPTIONS Needed)
//					Command Description
//
// XMTPT_SEND_CONFIG_PACKET_TO_HW (HwCfgDP5Out,SendCoarseFineGain,PC5_PRESENT,DppType)
//					Processes a configuration string for errors before sending
//							Allows only one gain setting type (coarse,fine OR total gain)
//							Removes commands from string that should not be sent to device type
// XMTPT_SEND_CONFIG_PACKET_EX (HwCfgDP5Out)
//					Sends the a configuration with minimal error checking
// XMTPT_FULL_READ_CONFIG_PACKET (PC5_PRESENT,DppType)
//					Creates and sends a full readback command
void CConsoleHelper::CreateConfigOptions(CONFIG_OPTIONS *CfgOptions, string strCfg, CDP5Status DP5Stat, bool bUseCoarseFineGain)
{
	// ==== ==== This application supports Communications for DPP Devices Below
	//DEVICE_ID==0; //DP5		
	//DEVICE_ID==1; //PX5
	//DEVICE_ID==2; //DP5G
	//DEVICE_ID==3; //MCA8000D
	//DEVICE_ID==4; //TB5
	//DEVICE_ID==5; //DP5X
	CfgOptions->DppType = DP5Stat.m_DP5_Status.DEVICE_ID;		// DPP id to test for supported options
	CfgOptions->HwCfgDP5Out = strCfg;							//configuration, if being sent, empty otherwise
	CfgOptions->PC5_PRESENT = DP5Stat.m_DP5_Status.PC5_PRESENT; //PC5 present indicator (accepts pc5 cfg commands)
	// bUseCoarseFineGain=true;  //uses GAIA,GAIF (coarse with fine gain);  
	// bUseCoarseFineGain=false; //uses GAIN (total gain);
	CfgOptions->SendCoarseFineGain = bUseCoarseFineGain;		//select gain configuration control commands
	CfgOptions->isDP5_RevDxGains = DP5Stat.m_DP5_Status.isDP5_RevDxGains;
	CfgOptions->DPP_ECO = DP5Stat.m_DP5_Status.DPP_ECO;
}

// COMMAND (CONFIG_OPTIONS Needed)
//					Command Description
//
// XMTPT_SEND_CONFIG_PACKET_TO_HW (HwCfgDP5Out,SendCoarseFineGain,PC5_PRESENT,DppType)
//					Processes a configuration string for errors before sending
//							Allows only one gain setting type (coarse,fine OR total gain)
//							Removes commands from string that should not be sent to device type
// XMTPT_SEND_CONFIG_PACKET_EX (HwCfgDP5Out)
//					Sends the a configuration with minimal error checking
// XMTPT_FULL_READ_CONFIG_PACKET (PC5_PRESENT,DppType)
//					Creates and sends a full readback command
bool CConsoleHelper::LibUsb_SendCommand_Config(TRANSMIT_PACKET_TYPE XmtCmd, CONFIG_OPTIONS CfgOptions)
{
    bool bHaveBuffer;
    int bSentPkt;
	bool bMessageSent;

	bMessageSent = false;
	if (DppLibUsb.bDeviceConnected) {
		memset(&DP5Proto.BufferOUT[0],0,sizeof(DP5Proto.BufferOUT));
		bHaveBuffer = (bool) SndCmd.DP5_CMD_Config(DP5Proto.BufferOUT, XmtCmd, CfgOptions);
		if (bHaveBuffer) {
			bSentPkt = DppLibUsb.SendPacketUSB(DppLibUsb.DppLibusbHandle, DP5Proto.BufferOUT, DP5Proto.PacketIn);
			if (bSentPkt) {
	            bMessageSent = true;
			}
		}
	}
	return (bMessageSent);
}

bool CConsoleHelper::LibUsb_ReceiveData()
{
	bool bDataReceived;

	bDataReceived = true;
	if (DppLibUsb.bDeviceConnected) { 
		bDataReceived = ReceiveData();
	}
	return (bDataReceived);
}

/** ReceiveData receives the incoming packet, 
 *  parses the packet, 
 *  then routes the packet to its final destination for further processing.
 *
 */
bool CConsoleHelper::ReceiveData()
{
	bool bDataReceived;

	bDataReceived = true;
	ParsePkt.DppState.ReqProcess = ParsePkt.ParsePacket(DP5Proto.PacketIn, &DP5Proto.PIN);
	switch (ParsePkt.DppState.ReqProcess) {
		case preqProcessStatus:
			long idxStatus;
			for(idxStatus=0;idxStatus<64;idxStatus++) {
				DP5Stat.m_DP5_Status.RAW[idxStatus] = DP5Proto.PIN.DATA[idxStatus];
			}
			DP5Stat.Process_Status(&DP5Stat.m_DP5_Status);
			DppStatusString = DP5Stat.ShowStatusValueStrings(DP5Stat.m_DP5_Status);
			break;
		case preqProcessSpectrum:
			ProcessSpectrumEx(DP5Proto.PIN, ParsePkt.DppState);
			break;
		//case preqProcessScopeData:
		//	ProcessScopeDataEx(DP5Proto.PIN, ParsePkt.DppState);
		//	break;
		//case preqProcessTextData:
		//	ProcessTextDataEx(DP5Proto.PIN, ParsePkt.DppState);
		//	break;
		//case preqProcessDiagData:
		//	ProcessDiagDataEx(DP5Proto.PIN, ParsePkt.DppState);
		//	break;
		case preqProcessCfgRead:
			ProcessCfgReadEx(DP5Proto.PIN, ParsePkt.DppState);
			break;
		//case preqProcessAck:
		//	ProcessAck(DP5Proto.PIN.PID2);
		//	break;
		//case preqProcessError:
		//	DisplayError(DP5Proto.PIN, ParsePkt.DppState);
		//	break;
		default:
			bDataReceived = false;
			break;
	}
	return (bDataReceived);
}

//processes spectrum and spectrum+status
void CConsoleHelper::ProcessSpectrumEx(Packet_In PIN, DppStateType DppState)
{
	long idxSpectrum;
	long idxStatus;
    
	DP5Proto.SPECTRUM.CHANNELS = (short)(256 * pow(2.0,(((PIN.PID2 - 1) & 14) / 2)));

	for(idxSpectrum=0;idxSpectrum<DP5Proto.SPECTRUM.CHANNELS;idxSpectrum++) {
        DP5Proto.SPECTRUM.DATA[idxSpectrum] = (long)(PIN.DATA[idxSpectrum * 3]) + (long)(PIN.DATA[idxSpectrum * 3 + 1]) * 256 + (long)(PIN.DATA[idxSpectrum * 3 + 2]) * 65536;
	}

    if ((PIN.PID2 & 1) == 0) {    // spectrum + status
		for(idxStatus=0;idxStatus<64;idxStatus++) {
			DP5Stat.m_DP5_Status.RAW[idxStatus] = PIN.DATA[idxStatus + DP5Proto.SPECTRUM.CHANNELS * 3];
		}
        DP5Stat.Process_Status(&DP5Stat.m_DP5_Status);
		DppStatusString = DP5Stat.ShowStatusValueStrings(DP5Stat.m_DP5_Status);
    }
}

void CConsoleHelper::ClearConfigReadFormatFlags()
{
	// configuration readback format control flags
	// these flags control how the configuration readback is formatted and processed
	DisplayCfg = false;			// format configuration for display
	DisplaySca = false;			// format sca for display (sca config sent separately)
	CfgReadBack = false;		// format configuration for general readback
	SaveCfg = false;			// format configuration for file save
	PrintCfg = false;			// format configuration for print
	HwCfgReady = false;			// configuration ready flag
	ScaReadBack = false;		// sca readback ready flag
}

void CConsoleHelper::ProcessCfgReadEx(Packet_In PIN, DppStateType DppState)
{
	string strRawCfgIn;
	string strRawCfgOut;
	string strCh;
	string strCmdData;
	strRawCfgIn = "";
	string strCmdD;
	string strCfg;
	bool isScaCfg = false;
	string strDisplayCfgOut;
	stringex strfn;
	
	strRawCfgOut = "";
	// ==========================================================
	// ===== Create Raw Configuration Buffer From Hardware ======
	for (int idxCfg=0;idxCfg<PIN.LEN;idxCfg++) {
		strCh = strfn.Format("%c",PIN.DATA[idxCfg]);
		strRawCfgIn += strCh;
		strRawCfgOut += strCh;
		if (PIN.DATA[idxCfg] == ';') {
			strRawCfgIn += "\r\n";
		}
	}

	// ==========================================================
	if (DisplayCfg) {
		DisplayCfg = false;
		strCfg = strRawCfgIn;
		strDisplayCfgOut = strRawCfgIn;
		if (Dp5CmdList.size() > 0) {
			for (unsigned int idxCmd=0;idxCmd<=Dp5CmdList.size();idxCmd++) {
				strCmdD = Dp5CmdList[idxCmd];
				if (strCmdD.length() > 0) {
					strDisplayCfgOut = ReplaceCmdDesc(strCmdD,strDisplayCfgOut);
				}
			}
		}
		return;
	} else if (CfgReadBack) {
		CfgReadBack = false;
		HwCfgDP5 = strRawCfgIn;
		HwCfgReady = true;
	}
	SaveCfg = false;
	PrintCfg = false;
	DisplayCfg = false;
	CfgReadBack = false;

	isScaCfg = (bool)(ScaReadBack || DisplaySca);
	ScaReadBack = false;
	DisplaySca = false;
	if (isScaCfg) {
		return;
	}
	strCmdData = GetCmdData("MCAS",strRawCfgIn); // mca mode
	strCmdData = GetCmdData("MCAC",strRawCfgIn); // channels
	if ((atoi(strCmdData.c_str()) > 0) && (atoi(strCmdData.c_str()) <= 8192)) {
		mcaCH = atoi(strCmdData.c_str());
	} else {
		mcaCH = 1024;
	}
	
	strCmdData = GetCmdData("THSL",strRawCfgIn); // LLD thresh
	SlowThresholdPct = atof(strCmdData.c_str());

	strCmdData = GetCmdData("THFA",strRawCfgIn); // fast thresh
	FastChThreshold = atoi(strCmdData.c_str());

	strCmdData = GetCmdData("TPEA",strRawCfgIn); // peak time
	RiseUS = atof(strCmdData.c_str());

	strCmdData = GetCmdData("GAIN",strRawCfgIn); // gain
	strGainDisplayValue = strCmdData + "x";

	strPresetCmd = "";
	strPresetVal = "";

	strCmdData = GetCmdData("PREC",strRawCfgIn); //preset count
	PresetCount = atoi(strCmdData.c_str());
	if (PresetCount > 0) {
		if (strPresetCmd.length() > 0) { strPresetCmd += "/"; }
		if (strPresetVal.length() > 0) { strPresetVal += "/"; }
		strPresetCmd += "Cnt";
		strPresetVal += strCmdData;
	} 
	
	strCmdData = GetCmdData("PRET",strRawCfgIn); //preset actual time
	PresetAcq = atof(strCmdData.c_str());
	if (PresetAcq > 0) {
		if (strPresetCmd.length() > 0) { strPresetCmd += "/"; }
		if (strPresetVal.length() > 0) { strPresetVal += "/"; }
		strPresetCmd += "Acq";
		strPresetVal += strCmdData;
	}

	strCmdData = GetCmdData("PRER",strRawCfgIn); // preset real time
	PresetRt = atof(strCmdData.c_str());
	if (PresetRt > 0) {
		if (strPresetCmd.length() > 0) { strPresetCmd += "/"; }
		if (strPresetVal.length() > 0) { strPresetVal += "/"; }
		strPresetCmd += "Real";
		strPresetVal += strCmdData;
	}

	if (strPresetCmd.length() == 0) { 
		strPresetCmd += "None"; 
	}
	if (strPresetVal.length() == 0) { 
		//strPresetVal += ""; 
	}

	strCmdData = GetCmdData("CLCK",strRawCfgIn); // fpga clock mode
	if (atoi(strCmdData.c_str()) == 80) {
		b80MHzMode = true;
	} else {
		b80MHzMode = false;
	}

	// DP5 oscilloscope support
	strCmdData = GetCmdData("INOF",strRawCfgIn); // osc. Input offset
	strInputOffset = strCmdData;

	strCmdData = GetCmdData("DACO",strRawCfgIn); // osc. DAC output
	strAnalogOut = strCmdData;

	strCmdData = GetCmdData("DACF",strRawCfgIn); // osc. DAC offset
	strOutputOffset = strCmdData;

	strCmdData = GetCmdData("AUO1",strRawCfgIn); // osc. AUX_OUT1
	strTriggerSource = strCmdData;

	strCmdData = GetCmdData("SCOE",strRawCfgIn); // osc. Scope trigger edge
	strTriggerSlope = strCmdData;

	strCmdData = GetCmdData("SCOT",strRawCfgIn); // osc. Scope trigger position
	strTriggerPosition = strCmdData;

	strCmdData = GetCmdData("SCOG",strRawCfgIn); // osc. Scope gain
	strScopeGain = strCmdData;

	strCmdData = GetCmdData("MCAS",strRawCfgIn); // Acq Mode
	AcqMode = 0;
	strMcaMode = "MCA";
	if (strCmdData == "NORM") {
		strMcaMode = "MCA";
	} else if (strCmdData == "MCS") {
		strMcaMode = "MCS";
		AcqMode = 1;
	} else if (strCmdData.length() > 0) {
		strMcaMode = strCmdData;
	}
	UpdateScopeCfg = true;
}

string CConsoleHelper::GetCmdData(string strCmd, string strCfgData)
{
	int iStart,iEnd,iCmd;
	string strCmdData;

	strCmdData = "";
	if (strCfgData.length() < 7) { return strCmdData; }	// no data
	if (strCmd.length() != 4) {	return strCmdData; }		// bad command
	iCmd = (int)strCfgData.find(strCmd+"=",0);
	if (iCmd == -1) { return strCmdData; }			// cmd not found	
	iStart = (int)strCfgData.find("=",iCmd);						 
	if (iStart == -1) { return strCmdData; }		// data start not found	
	iEnd = (int)strCfgData.find(";",iCmd);
	if (iEnd == -1) {return strCmdData; }			// data end found	
	if (iStart >= iEnd) { return strCmdData; }		// data error
	strCmdData = strCfgData.substr(iStart+1,(iEnd - (iStart+1)));
	return strCmdData;
}

string CConsoleHelper::ReplaceCmdDesc(string strCmd, string strCfgData)
{
	int iStart,iCmd;	//iEnd,
	string strNew;
	string strDesc;

	strNew = "";
	if (strCfgData.length() < 7) { return strCfgData; }	// no data
	if (strCmd.length() != 4) {	return strCfgData; }		// bad command
	iCmd = (int)strCfgData.find(strCmd+"=",0);
	if (iCmd == -1) { return strCfgData; }						// cmd not found	

	strDesc = GetCmdDesc(strCmd);
	if (strDesc.length() == 0) { return strCfgData; }		// cmd desc  not found
	iStart = (int)strCfgData.find("=",iCmd);						 
	if (iStart != (iCmd + 4)) { return strCfgData; }			// data start not found	
	if (iCmd <= 0) {	// usually left string has iCmd-1
		iCmd = 1;		// in this case left has 0 chars, set offset
	}
	strNew = strCfgData.substr(0,iCmd-1) + strDesc + strCfgData.substr(iStart);
	return strNew;
}

string CConsoleHelper::AppendCmdDesc(string strCmd, string strCfgData)
{
	int iStart,iEnd,iCmd;
	string strNew;
	string strDesc;

	strNew = "";
	if (strCfgData.length() < 7) { return strCfgData; }	// no data
	if (strCmd.length() != 4) {	return strCfgData; }		// bad command
	iCmd = (int)strCfgData.find(strCmd+"=",0);
	if (iCmd == -1) { return strCfgData; }						// cmd not found	
	iEnd = (int)strCfgData.find(";",iCmd);
	if (iEnd == -1) {return strCfgData; }			// data end found	
	iStart = (int)strCfgData.find("=",iCmd);						 
	if (iStart != (iCmd + 4)) { return strCfgData; }			// data start not found	
	if (iStart >= iEnd) { return strCfgData; }		// data error
	strDesc = GetCmdDesc(strCmd);
	if (strDesc.length() == 0) { return strCfgData; }		// cmd desc  not found
	strNew = strCfgData.substr(0,iEnd+1) + "    " + strDesc + strCfgData.substr(iEnd+1);
 	return strNew;
}

string CConsoleHelper::GetCmdDesc(string strCmd)
{
	string strCmdName = "";
	if (strCmd == "RESC") {
		strCmdName = "Reset Configuration";
	} else if (strCmd == "CLCK") {
		strCmdName = "20MHz/80MHz";
	} else if (strCmd == "TPEA") {
		strCmdName = "peaking time";
	} else if (strCmd == "GAIF") {
		strCmdName = "Fine gain";
	} else if (strCmd == "GAIN") {
		strCmdName = "Total Gain (analog * fine)";
	} else if (strCmd == "RESL") {
		strCmdName = "Detector Reset lockout";
	} else if (strCmd == "TFLA") {
		strCmdName = "Flat top";
	} else if (strCmd == "TPFA") {
		strCmdName = "Fast channel peaking time";
	} else if (strCmd == "RTDE") {
		strCmdName = "RTD on/off";
	} else if (strCmd == "MCAS") {
		strCmdName = "MCA Source";
	} else if (strCmd == "RTDD") {
		strCmdName = "Custom RTD oneshot delay";
	} else if (strCmd == "RTDW") {
		strCmdName = "Custom RTD oneshot width";
	} else if (strCmd == "PURE") {
		strCmdName = "PUR interval on/off";
	} else if (strCmd == "SOFF") {
		strCmdName = "Set spectrum offset";
	} else if (strCmd == "INOF") {
		strCmdName = "Input offset";
	} else if (strCmd == "ACKE") {
		strCmdName = "ACK / Don't ACK packets with errors";
	} else if (strCmd == "AINP") {
		strCmdName = "Analog input pos/neg";
	} else if (strCmd == "AUO1") {
		strCmdName = "AUX_OUT selection";
	} else if (strCmd == "AUO2") {
		strCmdName = "AUX_OUT2 selection";
	} else if (strCmd == "BLRD") {
		strCmdName = "BLR down correction";
	} else if (strCmd == "BLRM") {
		strCmdName = "BLR mode";
	} else if (strCmd == "BLRU") {
		strCmdName = "BLR up correction";
	} else if (strCmd == "BOOT") {
		strCmdName = "Turn supplies on/off at power up";
	} else if (strCmd == "CUSP") {
		strCmdName = "Non-trapezoidal shaping";
	} else if (strCmd == "DACF") {
		strCmdName = "DAC offset";
	} else if (strCmd == "DACO") {
		strCmdName = "DAC output";
	} else if (strCmd == "GAIA") {
		strCmdName = "Analog gain index";
	} else if (strCmd == "GATE") {
		strCmdName = "Gate control";
	} else if (strCmd == "GPED") {
		strCmdName = "G.P. counter edge";
	} else if (strCmd == "GPGA") {
		strCmdName = "G.P. counter uses GATE?";
	} else if (strCmd == "GPIN") {
		strCmdName = "G.P. counter input";
	} else if (strCmd == "GPMC") {
		strCmdName = "G.P. counter cleared with MCA counters?";
	} else if (strCmd == "GPME") {
		strCmdName = "G.P. counter uses MCA_EN?";
	} else if (strCmd == "HVSE") {
		strCmdName = "HV set";
	} else if (strCmd == "MCAC") {
		strCmdName = "MCA/MCS channels";
	} else if (strCmd == "MCAE") {
		strCmdName = "MCA/MCS enable";
	} else if (strCmd == "MCSL") {
		strCmdName = "MCS low threshold";
	} else if (strCmd == "MCSH") {
		strCmdName = "MCS high threshold";
	} else if (strCmd == "MCST") {
		strCmdName = "MCS timebase";
	} else if (strCmd == "PAPS") {
		strCmdName = "preamp 8.5/5 (N/A)";
	} else if (strCmd == "PAPZ") {
		strCmdName = "Pole-Zero";
	} else if (strCmd == "PDMD") {
		strCmdName = "Peak detect mode (min/max)";
	} else if (strCmd == "PRCL") {
		strCmdName = "Preset counts low threshold";
	} else if (strCmd == "PRCH") {
		strCmdName = "Preset counts high threshold";
	} else if (strCmd == "PREC") {
		strCmdName = "Preset counts";
	} else if (strCmd == "PRER") {
		strCmdName = "Preset Real Time";
	} else if (strCmd == "PRET") {
		strCmdName = "Preset time";
	} else if (strCmd == "RTDS") {
		strCmdName = "RTD sensitivity";
	} else if (strCmd == "RTDT") {
		strCmdName = "RTD threshold";
	} else if (strCmd == "SCAH") {
		strCmdName = "SCAx high threshold";
	} else if (strCmd == "SCAI") {
		strCmdName = "SCA index";
	} else if (strCmd == "SCAL") {
		strCmdName = "SCAx low theshold";
	} else if (strCmd == "SCAO") {
		strCmdName = "SCAx output (SCA1-8 only)";
	} else if (strCmd == "SCAW") {
		strCmdName = "SCA pulse width (not indexed - SCA1-8)";
	} else if (strCmd == "SCOE") {
		strCmdName = "Scope trigger edge";
	} else if (strCmd == "SCOG") {
		strCmdName = "Digital scope gain";
	} else if (strCmd == "SCOT") {
		strCmdName = "Scope trigger position";
	} else if (strCmd == "TECS") {
		strCmdName = "TEC set";
	} else if (strCmd == "THFA") {
		strCmdName = "Fast threshold";
	} else if (strCmd == "THSL") {
		strCmdName = "Slow threshold";
	} else if (strCmd == "TLLD") {
		strCmdName = "LLD threshold";
	} else if (strCmd == "TPMO") {
		strCmdName = "Test pulser on/off";
	} else if (strCmd == "VOLU") {
		strCmdName = "Speaker On/Off";
	} else if (strCmd == "CON1") {
		strCmdName = "Connector 1";
	} else if (strCmd == "CON2") {
		strCmdName = "Connector 2";
	}
	return strCmdName;
}

//lData=spectrum data,chan=numberof channels,bLog=display as log
void CConsoleHelper::ConsoleGraph(long lData[], long chan, bool bLog, std::string strStatus)
{
	const int ScreenW = 80;
	const int ScreenH = 24;
	char plot[ScreenW][ScreenH];
	char chEmpty = char(32);
#ifdef _WIN32
	char chBlock = char(219);
#else
	char chBlock = char(79);
#endif
	//long xMax;
	long yMax;
	long idxX;
	long idxY;
	double xM;
	double yM;
	long i;
	long x;
	long y;
	double logY;
	long logYLong;

	yMax = 0;
	for(i=0;i<chan;i++) {
		if (bLog) {
			logY = log(1.0 + lData[i]);
			if (yMax < (long)logY) {
				yMax = (long)logY;
			}
		} else {
			if (yMax < lData[i]) {
				yMax = lData[i];
			}
		}
	}

	xM = (double)ScreenW / (double)chan;
	if (yMax < 1) yMax = 1;
	yM = (double)(((double)(ScreenH)-(double)2.0) / (double)yMax);

	for (i=0;i<chan;i++) {
		idxX = (long)((double)i * xM);
		if (idxX >= ScreenW) idxX = ScreenW-1;
		
		if (bLog) {
			logY = log(1.0  + lData[i]);
			logYLong = (long)logY;
			idxY = (long)((double)logYLong * yM);
		} else {
			idxY = (long)((double)lData[i] * yM);
		}
		if (idxY >= ScreenH) idxY = ScreenH-2;

		for(y=0;y<ScreenH;y++) {
			if (y <= idxY) {
				plot[idxX][y] = chBlock;
			} else {
				plot[idxX][y] = chEmpty;
			}
		}
	}

	unsigned int iRow=0;
	unsigned int iCol=0;
	unsigned int iRowLen;
	std::string strRow;

	if (strStatus.length() > 0) {
		std::vector<string> vstr = Split(strStatus,"\r\n",true,true);	// create rows
		for(iRow=0;iRow<(size_t)vstr.size();iRow++) {					// copy to plot
			strRow = vstr[iRow];
			iRowLen = (unsigned int)strRow.length();
			for(iCol=0;iCol<iRowLen;iCol++){
				plot[iCol][(ScreenH-2)-iRow] = strRow.at(iCol);
			}
		}
	}

	for (y=0;y<ScreenH;y++){
		for (x=0;x<ScreenW;x++){
			cout << plot[x][(ScreenH-1)-y];
		}
#ifndef _WIN32
		cout << endl;
#endif
	}
}

string CConsoleHelper::CreateMCAData(long m_larDataBuffer[], SpectrumFileType sfInfo, DP4_FORMAT_STATUS cfgStatusLst)
{
	string strMCA;
	string strText;
	string strADC;
	long idxChan;
	string strData;
	stringex strfn;

	if (cfgStatusLst.SerialNumber <= 0) 
	{
		return ("");
	}
	strText = "";
   	strMCA = "";
    switch(sfInfo.m_iNumChan){
        case 16384:
            strADC = "6";
				break;
        case 8192:
            strADC = "5";
				break;
        case 4096:
            strADC = "4";
				break;
        case 2048:
            strADC = "3";
				break;
        case 1024:
            strADC = "2";
				break;
        case 512:
            strADC = "1";
				break;
        case 256:
            strADC = "0";
				break;
    }
    strMCA += "<<PMCA SPECTRUM>>\r\n";
    strMCA += "TAG - " + sfInfo.strTag + "\r\n";
    strMCA += "DESCRIPTION - " + sfInfo.strDescription + "\r\n";
    strMCA += "GAIN - " + strADC + "\r\n";
    strMCA += "THRESHOLD - 0\r\n";
    strMCA += "LIVE_MODE - 0\r\n";
    strMCA += "PRESET_TIME - \r\n";
	strText = strfn.Format("%lf", cfgStatusLst.AccumulationTime);
    strMCA += "LIVE_TIME - " + strText + "\r\n";
	strText = strfn.Format("%lf", cfgStatusLst.RealTime);
    strMCA += "REAL_TIME - " + strText + "\r\n";
    strMCA += "START_TIME - \r\n";
	strText = strfn.Format("%i", sfInfo.SerialNumber);
    strMCA += "SERIAL_NUMBER - " + strText + "\r\n";
    strMCA += "<<DATA>>\r\n";
	for (idxChan=0;idxChan<sfInfo.m_iNumChan;idxChan++) {
		strData = strfn.Format("%i", m_larDataBuffer[idxChan]);
		strMCA += strData + "\r\n";
	}
    strMCA += "<<END>>\r\n";
	strMCA += "<<DP5 CONFIGURATION>>\r\n";
	strMCA += sfInfo.strSpectrumConfig;
	strMCA += "<<DP5 CONFIGURATION END>>\r\n";
	strMCA += "<<DPP STATUS>>\r\n";
	strMCA += sfInfo.strSpectrumStatus;	// this functin is included in most examples
	strMCA += "<<DPP STATUS END>>\r\n";
	return (strMCA);
}

void CConsoleHelper::SaveSpectrumStringToFile(string strData)
{
	FILE  *out;
	string strFilename;
	string strError;
	stringex strfn;

	strFilename = "SpectrumData.mca";

	if ( (out = fopen(strFilename.c_str(),"wb")) == (FILE *) NULL)
		strError = strfn.Format("Couldn't open %s for writing.\n", strFilename.c_str());
	else
	{
		fprintf(out,"%s",strData.c_str());
	}
	fclose(out);
}

void CConsoleHelper::SaveSpectrumStringToFile(string strData, string strFilename) {
	FILE  *out;
	string strError;
	stringex strfn;

	if ( (out = fopen(strFilename.c_str(),"wb")) == (FILE *) NULL)
		strError = strfn.Format("Couldn't open %s for writing.\n", strFilename.c_str());
	else
	{
		fprintf(out,"%s",strData.c_str());
	}
	fclose(out);
}

string CConsoleHelper::CreateSpectrumConfig(string strRawCfgIn) 
{
	string strDisplayCfgOut;
	string strCmdD;

	strDisplayCfgOut = strRawCfgIn;
	if (Dp5CmdList.size() > 0) {
		for (unsigned int idxCmd=0;idxCmd<Dp5CmdList.size();idxCmd++) {
			strCmdD = Dp5CmdList[idxCmd];
			if (strCmdD.length() > 0) {
				strDisplayCfgOut = AppendCmdDesc(strCmdD,strDisplayCfgOut);
			}
		}
	}
	return strDisplayCfgOut;
}

vector<string> CConsoleHelper::MakeDp5CmdList()
{
	vector<string> strCfgArr;
	strCfgArr.clear();
	strCfgArr.push_back("RESC");
	strCfgArr.push_back("CLCK");
	strCfgArr.push_back("TPEA");
	strCfgArr.push_back("GAIF");
	strCfgArr.push_back("GAIN");
	strCfgArr.push_back("RESL");
	strCfgArr.push_back("TFLA");
	strCfgArr.push_back("TPFA");
	strCfgArr.push_back("PURE");
	strCfgArr.push_back("RTDE");
	strCfgArr.push_back("MCAS");
	strCfgArr.push_back("MCAC");
	strCfgArr.push_back("SOFF");
	strCfgArr.push_back("AINP");
	strCfgArr.push_back("INOF");
	strCfgArr.push_back("GAIA");
	strCfgArr.push_back("CUSP");
	strCfgArr.push_back("PDMD");
	strCfgArr.push_back("THSL");
	strCfgArr.push_back("TLLD");
	strCfgArr.push_back("THFA");
	strCfgArr.push_back("DACO");
	strCfgArr.push_back("DACF");
	strCfgArr.push_back("RTDS");
	strCfgArr.push_back("RTDT");
	strCfgArr.push_back("BLRM");
	strCfgArr.push_back("BLRD");
	strCfgArr.push_back("BLRU");
	strCfgArr.push_back("GATE");
	strCfgArr.push_back("AUO1");
	strCfgArr.push_back("PRET");
	strCfgArr.push_back("PRER");
	strCfgArr.push_back("PREL");
	strCfgArr.push_back("PREC");
	strCfgArr.push_back("PRCL");
	strCfgArr.push_back("PRCH");
	strCfgArr.push_back("HVSE");
	strCfgArr.push_back("TECS");
	strCfgArr.push_back("PAPZ");
	strCfgArr.push_back("PAPS");
	strCfgArr.push_back("SCOE");
	strCfgArr.push_back("SCOT");
	strCfgArr.push_back("SCOG");
	strCfgArr.push_back("MCSL");
	strCfgArr.push_back("MCSH");
	strCfgArr.push_back("MCST");
	strCfgArr.push_back("AUO2");
	strCfgArr.push_back("TPMO");
	strCfgArr.push_back("GPED");
	strCfgArr.push_back("GPIN");
	strCfgArr.push_back("GPME");
	strCfgArr.push_back("GPGA");
	strCfgArr.push_back("GPMC");
	strCfgArr.push_back("MCAE");
	strCfgArr.push_back("VOLU");
	strCfgArr.push_back("CON1");
	strCfgArr.push_back("CON2");
	strCfgArr.push_back("BOOT");
	return strCfgArr;
}
