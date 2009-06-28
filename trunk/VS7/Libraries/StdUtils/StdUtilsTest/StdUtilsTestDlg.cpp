// StdUtilsTestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "StdUtilsTest.h"
#include "StdUtilsTestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

double genrand();

/////////////////////////////////////////////////////////////////////////////
// CStdUtilsTestDlg dialog

CStdUtilsTestDlg::CStdUtilsTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStdUtilsTestDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStdUtilsTestDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CStdUtilsTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStdUtilsTestDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CStdUtilsTestDlg, CDialog)
	//{{AFX_MSG_MAP(CStdUtilsTestDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_TEST_ERRORINFO, OnTestErrorinfo)
	ON_BN_CLICKED(IDC_TEST_XML, OnTestXml)
	ON_BN_CLICKED(IDC_TEST_MARKUP, OnTestMarkup)
	ON_BN_CLICKED(IDC_TEST_COMPRESS, OnTestCompress)
	ON_BN_CLICKED(IDC_TEST_SERIALIZE, OnTestSerialize)
	ON_BN_CLICKED(IDC_TEST_POINT, OnTestPoint)
	ON_BN_CLICKED(IDC_TEST_BINARY_SECTION, OnTestBinarySection)
	ON_BN_CLICKED(IDC_TEST_GREY_CODE, OnTestGreyCode)
	ON_BN_CLICKED(IDC_TEST_TRACE, OnTestTrace)
	ON_BN_CLICKED(IDC_TEST_POST_FIX_EVAL, OnTestPostFixEval)
	ON_BN_CLICKED(IDC_TEST_MAPS, OnTestMaps)
	ON_BN_CLICKED(IDC_TEST_STRINGS, OnTestStrings)
	ON_BN_CLICKED(IDC_TEST_VARIANT, OnTestVariant)
	ON_BN_CLICKED(IDC_TEST_ODBC, OnTestOdbc)
	ON_BN_CLICKED(IDC_TEST_RANDOM, OnTestRandom)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStdUtilsTestDlg message handlers

BOOL CStdUtilsTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	Std_SetLogFilePrefix("C:\\Temp\\StdUtilsTest");
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CStdUtilsTestDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CStdUtilsTestDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CStdUtilsTestDlg::OnTestErrorinfo() 
{
	CStdErrorInfo oError;

	oError.m_strError = "This is a test";
}

void CStdUtilsTestDlg::OnTestXml() 
{
	long lVal = Std_RGB(0, 0, 255);
	lVal = Std_RGB(0, 255, 0);
	lVal = Std_RGB(255, 0, 0);
	lVal = Std_RGB(255, 255, 0);
	lVal = Std_RGB(100, 100, 50);

}

void CStdUtilsTestDlg::OnTestMarkup() 
{
	CStdXml oXml, oXml2;
	CStdString strVal;
	int iVal=0;
	long lVal=0;
	float fltVal=0;
	double dblVal=0;
	bool bVal=false;

try
{
	strVal = "This is a string";
	lVal = 55433;
	iVal = 43;
	dblVal = 34.224432;
	fltVal = (float) 34.22;
	bVal = true;

	//Testing the Add functions
	oXml.AddElement("World");
	oXml.AddChildElement("string", strVal);
	oXml.AddChildElement("long", lVal);
	oXml.AddChildElement("int", iVal);
	oXml.AddChildElement("double", dblVal);
	oXml.AddChildElement("float", fltVal);
	oXml.AddChildElement("bool", bVal);

	oXml.AddChildElement("Robots");
	oXml.IntoChildElement("Robots");

	CStdIPoint oPoint(1,2,3), oPoint2;
	CStdFPoint oFPoint;
	Std_SavePoint(oXml, "Point", oPoint);
	Std_LoadPoint(oXml, "Point", oPoint2);
	Std_LoadPoint(oXml, "Point", oFPoint);

	oXml.SetAttrib("DoubleVal", dblVal);
	oXml.AddChildElement("Robot");
	oXml.IntoChildElement("Robot");
	oXml.AddChildElement("bool", bVal);
	oXml.SetChildAttrib("lVal", lVal);
	oXml.AddChildCData("Description", "n<6");
	oXml.OutOfElem();

	oXml.OutOfElem();
	oXml.Save("C:\\Biomechs\\Libraries\\StdUtils\\Test.xml");
	
	// Testing the get functions
//	oXml.Load("C:\\Biomechs\\Darwins Playground\\Projects\\InsectSimulator\\Scripts\\Final_Insect.xml");
/*
	oXml.FindElement("Test");
	strVal = oXml.GetChildString("string");

	iVal = oXml.GetChildInt("int");
	iVal = oXml.GetChildInt("float", -1);
	iVal = oXml.GetChildInt("string", -1);

	lVal = oXml.GetChildLong("long");
	lVal = oXml.GetChildLong("float", -1);
	lVal = oXml.GetChildLong("string", -1);

	fltVal = oXml.GetChildFloat("float");
	fltVal = oXml.GetChildFloat("int");
	fltVal = oXml.GetChildFloat("string", (float) 0.2);

	dblVal = oXml.GetChildDouble("double");
	dblVal = oXml.GetChildDouble("int");
	dblVal = oXml.GetChildDouble("string", 0.3455);

	bVal = oXml.GetChildBool("bool");
	bVal = oXml.GetChildBool("bool2");
	bVal = oXml.GetChildBool("bool3");
	bVal = oXml.GetChildBool("bool4");
	bVal = oXml.GetChildBool("string", false);
	
	//Errors
	//iVal = oXml.GetChildInt("string");
	//iVal = oXml.GetChildInt("float");
	//lVal = oXml.GetChildLong("string");
	//lVal = oXml.GetChildLong("float");
	//fltVal = oXml.GetChildFloat("string");
	//dblVal = oXml.GetChildDouble("string");
	//bVal = oXml.GetChildBool("string");
	*/
}
catch(CStdErrorInfo oError)
{
	AfxMessageBox(oError.m_strError);
}
catch(...)
{
	AfxMessageBox("Unidentified Error");
}
}
/*
{
	CStdString strXml, strTagName, strTagValue;
	
	CMarkupSTL doc;
	doc.Load("C:\\Biomechs\\Libraries\\StdUtils\\StdUtilsTest\\Robots.xml");
	
	doc.FindElem("World");
	doc.FindChildElem("");

	doc.ResetChildPos();
	doc.FindChildElem("Name");
	strTagValue = doc.GetChildData();

	doc.AddElem( "ORDER" );
	doc.AddChildElem( "ITEM" );
	doc.IntoElem();
	doc.AddChildElem( "NAME", "carrots" );
	doc.AddChildElem( "QTY", "1" );
	doc.AddChildElem( "PRICE", ".98" );
	doc.AddChildAttrib( "unit", "1 lb" );
	doc.AddElem( "ITEM" );
	doc.AddChildElem( "NAME", "onions" );
	doc.AddChildElem( "QTY", "1" );
	doc.AddChildElem( "PRICE", "1.10" );
	doc.AddChildAttrib( "unit", "3 lb bag" );
	doc.AddChildElem( "SUPPLIER", "Hanover" );

	strXml = doc.GetDoc();

	int nTotal = 0;
	doc.ResetPos();
	while ( doc.FindChildElem("ITEM") )
	{
		doc.IntoElem();

		int nQty = 0;
		if ( doc.FindChildElem("QTY") )
			nQty = atoi(doc.GetChildData().c_str());
		doc.ResetChildPos();
		if ( doc.FindChildElem("PRICE") )
			nTotal += nQty * atoi(doc.GetChildData().c_str());

		doc.OutOfElem();
	}

	nTotal = 0;
	doc.ResetPos();
	while(doc.FindChildElem("ITEM"))
		nTotal++;

	nTotal = 0;
	doc.ResetPos();
	doc.FindChildElem("ITEM");
	doc.IntoElem();
	while(doc.FindChildElem())
	{
		strTagName = doc.GetChildTagName();
		strTagValue = doc.GetChildData();
		nTotal++;
	}
}
*/

void CStdUtilsTestDlg::OnTestCompress() 
{
	CStdXml oXml, oXml2;
	CStdCompress oZLib;
	string strIn, strOut, strOut2;
	char *strCompressed=NULL;
	//int iSize;

try
{

/*	
//	oXml.Load("C:\\Biomechs\\Libraries\\StdUtils\\StdUtilsTest\\Test.xml");
	oXml.Load("C:\\Biomechs\\Libraries\\StdUtils\\StdUtilsTest\\Speed Template.xml");
	strIn = oXml.Serialize();

	oZLib.CompressString(strIn, &strCompressed, iSize, 9);
	oZLib.DecompressString(strCompressed, iSize, strOut);
	
	oXml2.Deserialize(strOut);
	oXml2.Save("C:\\Biomechs\\Libraries\\StdUtils\\StdUtilsTest\\Test2.xml");

	if(strCompressed) delete[] strCompressed;
*/
}
catch(CStdErrorInfo oError)
{
	AfxMessageBox(oError.m_strError);
}
catch(...)
{
	AfxMessageBox("Unidentified Error");
}
}


void CStdUtilsTestDlg::OnTestSerialize() 
{
	CRobotWorld oWorld;

try
{
	oWorld.LoadFile("C:\\Biomechs\\Libraries\\StdUtils\\StdUtilsTest\\Robots.xml");

	oWorld.SaveFile("C:\\Biomechs\\Libraries\\StdUtils\\StdUtilsTest\\Robots_Out.xml");
	
}
catch(CStdErrorInfo oError)
{
	AfxMessageBox(oError.m_strError);
}
catch(...)
{
	AfxMessageBox("Unidentified Error");
}
}




void CStdUtilsTestDlg::OnTestPoint() 
{
	CStdPoint<int> oIntPoint1, oIntPoint2, oIntPoint3;
	CStdPoint<double> oDblPoint1, oDblPoint2;
	bool bVal;

	oIntPoint1.x = 100;
	oIntPoint1.y = 200;

	oIntPoint2 = oIntPoint1;
	if(oIntPoint1 == oIntPoint2) 
		bVal = true;

	if(oIntPoint1 != oIntPoint2) 
		bVal = true;

	oIntPoint1.y = 300;

	if(oIntPoint1 != oIntPoint2) 
		bVal = true;

	oIntPoint3 = oIntPoint1 + oIntPoint2;
	oIntPoint3 = oIntPoint3 - oIntPoint2;
	oIntPoint1+=oIntPoint2;
	oIntPoint1-=oIntPoint2;

	oDblPoint1.x = 1.222;
	oDblPoint1.y = 0.222;
}

void CStdUtilsTestDlg::OnTestBinarySection() 
{
	CStdString strHex;
	CStdArray<unsigned char> aryBinaryData, aryTestData;
	unsigned char cOut;
	unsigned short iOut;
	unsigned long lOut;
	long lSize;

try
{
	strHex = "5A05AA58607AEA3A9F69F97A1B17139A8AC0735C";
	
	Std_HexStringToByteArray(strHex, aryBinaryData);
	lSize = Std_GetBitSize(aryBinaryData);
	Std_CopyBinarySection(aryBinaryData, 5, aryTestData, 0, 20);
	Std_ByteArrayToHexString(aryTestData, strHex);

	aryTestData.RemoveAll();
	Std_CopyBinarySection(aryBinaryData, 4, aryTestData, 0, 20);
	Std_ByteArrayToHexString(aryTestData, strHex);
	

	strHex = "5A05AA58607AEA3A9F69F97A1B17139A8AC0735C";

	Std_HexStringToByteArray(strHex, aryBinaryData);

	//GetBinarySection(m_aryBinaryData, 12, 16, aryOutData);
	Std_GetBinarySection(aryBinaryData, 12, 8, cOut);
	Std_GetBinarySection(aryBinaryData, 12, 16, iOut);
	Std_GetBinarySection(aryBinaryData, 12, 32, lOut);

	Std_GetBinarySection(aryBinaryData, 0, 4, cOut);
	Std_GetBinarySection(aryBinaryData, 4, 2, cOut);
	Std_GetBinarySection(aryBinaryData, 19, 6, cOut);
	Std_GetBinarySection(aryBinaryData, 19, 10, iOut);
	Std_GetBinarySection(aryBinaryData, 19, 11, iOut);


	strHex = "5A05";
	Std_HexStringToByteArray(strHex, aryBinaryData);
	Std_ByteArrayToHexString(aryBinaryData, strHex);

	Std_SetBitInArray(aryBinaryData, 32, true);
	Std_SetBitInArray(aryBinaryData, 33, false);
	Std_SetBitInArray(aryBinaryData, 34, true);
	Std_SetBitInArray(aryBinaryData, 35, false);

	Std_SetBitInArray(aryBinaryData, 16, true);
	Std_SetBitInArray(aryBinaryData, 17, true);
	Std_SetBitInArray(aryBinaryData, 18, true);
	Std_SetBitInArray(aryBinaryData, 19, true);
	Std_SetBitInArray(aryBinaryData, 20, true);
	Std_SetBitInArray(aryBinaryData, 21, true);
	Std_SetBitInArray(aryBinaryData, 22, true);
	Std_SetBitInArray(aryBinaryData, 23, true);

	Std_ByteArrayToHexString(aryBinaryData, strHex);

	Std_SetBitInArray(aryBinaryData, 17, false);

	Std_ByteArrayToHexString(aryBinaryData, strHex);

//	strHex = "0A05AA58607AEA3A9F69F97A1B17139A8AC0735C";
	strHex = "0000000000000000000000000000000000000000";
	Std_HexStringToByteArray(strHex, aryBinaryData);

	cOut = 0xFF;
	Std_SetBinarySection(aryBinaryData, 0, 2, cOut);
	Std_ByteArrayToHexString(aryBinaryData, strHex);

//	strHex = "FABC";
//	HexStringToByteArray(strHex, aryTestData);

//	SetBinarySection(aryBinaryData, 20, 16, aryTestData);
//	ByteArrayToHexString(aryBinaryData, strHex);

	cOut = 0xCF;
	Std_SetBinarySection(aryBinaryData, 16, 4, cOut);
	Std_ByteArrayToHexString(aryBinaryData, strHex);

	iOut = 0xF324;
	Std_SetBinarySection(aryBinaryData, 16, 12, iOut);
	Std_ByteArrayToHexString(aryBinaryData, strHex);

	lOut = 0xCF543210;
	Std_SetBinarySection(aryBinaryData, 16, 24, lOut);
	Std_ByteArrayToHexString(aryBinaryData, strHex);

}
catch(CStdErrorInfo oError)
{
	AfxMessageBox(oError.m_strError);
}
catch(...)
{
	AfxMessageBox("Unidentified Error");
}
}


void CStdUtilsTestDlg::OnTestGreyCode() 
{
	unsigned char cVal;
	unsigned char cOut;
	unsigned char cOut2;
	unsigned short iVal;
	unsigned short iOut;
	unsigned short iOut2;
//	unsigned long lOut;
//	double dblOut;

try
{
	for(cVal=0; cVal<16; cVal++)
	{
		cOut = Std_BinaryToGreyCode(cVal);
		cOut2 = Std_GreyCodeToBinary(cOut);
	}

	for(iVal=0; iVal<100; iVal++)
	{
		iOut = Std_BinaryToGreyCode(iVal);
		iOut2 = Std_GreyCodeToBinary(iOut);
	}


	iOut = Std_BinaryToGreyCode((unsigned short) 9);	//Membrane ligand
	iOut = Std_BinaryToGreyCode((unsigned short) 16);	//Diffusible ligand
	iOut = Std_BinaryToGreyCode((unsigned short) 34);	//membrane receptor
	iOut = Std_BinaryToGreyCode((unsigned short) 40);	//cell receptor
	iOut = Std_BinaryToGreyCode((unsigned short) 50);	//transcription factor.

	iOut = Std_BinaryToGreyCode((unsigned short) 128);	//transcription factor.
	iOut = Std_BinaryToGreyCode((unsigned short) 230);	//transcription factor.

}
catch(CStdErrorInfo oError)
{
	AfxMessageBox(oError.m_strError);
}
catch(...)
{
	AfxMessageBox("Unidentified Error");
}
}

void CStdUtilsTestDlg::OnTestTrace() 
{
	int source[] = {1, 4, 4, 6, 1, 2, 2, 3, 1, 6, 6, 6, 5, 7, 5, 4, 4};
	int iSourceNum = sizeof(source)/sizeof(source[0]);
	CStdArray<int> aryValues;
	CRobotLeg *lpLeg=NULL;
	float aryVals[5];

try
{
	aryVals[0] = (float) 0.1;
	aryVals[1] = (float) 0.2;
	aryVals[2] = (float) 0.3;
	aryVals[3] = (float) 0.4;
	aryVals[4] = (float) 0.5;
	
	TRACE_STL_ARRAY(aryVals, 5);
	TRACE_STL_ARRAY_DESC(aryVals, 5, "Test");

	LOG_DB("Insert Into MutationLog (InstanceHandle, GeneID, ProteinID, Param, OldValue, NewValue) " + 
		     STR("Values ('{588BFBCF-8A45-499E-907D-6CEFCD482B80}', 0, 0, 'Test', 5.2, 4.3)")); 

	int iVal;
	for(int iIndex=0; iIndex<1000; iIndex++)
	{
		iVal = Std_IRand(-10, 0);
		TRACE_DEBUG_NS("Val: " + STR(iVal));
	}

	TRACE_DEBUG("Test");
	TRACE_DEBUG("TestVal: " + STR(10));

	copy(source, source + iSourceNum, back_inserter(aryValues));
	TRACE_STL_CONTAINER_DESC(aryValues, "Orignal Values");

	sort(aryValues.begin(), aryValues.end());
	TRACE_STL_CONTAINER_DESC(aryValues, "Sorted Values");

	unique(aryValues.begin(), aryValues.end());
	TRACE_STL_CONTAINER_DESC(aryValues, "Unique Values");

	lpLeg = new CRobotLeg;
	lpLeg->m_iLength=100;
	lpLeg->m_fltRotation = (float) 2.332;
	TRACE_STL_OBJ(lpLeg);
	delete lpLeg;

	Std_ResetLog();
	TRACE_DEBUG("This is a test");
}
catch(CStdErrorInfo oError)
{
	AfxMessageBox(oError.m_strError);
}
catch(...)
{
	AfxMessageBox("Unidentified Error");
}
}

void CStdUtilsTestDlg::OnTestPostFixEval() 
{
	CStdPostFixEval oEval;
	float fltX, fltOut;

try
{
	oEval.AddVariable("x");
	oEval.AddVariable("y");

	oEval.Equation("rnd,5,*,4,+,sin,rnd,+");

	oEval.SetVariable("y", 2.0);
	for(fltX=0; fltX<1; fltX+=(float)0.2)
	{
		oEval.SetVariable("x", fltX);
		fltOut = oEval.Solve();
	}


}
catch(CStdErrorInfo oError)
{
	AfxMessageBox(oError.m_strError);
}
catch(...)
{
	AfxMessageBox("Unidentified Error");
}
}

void CStdUtilsTestDlg::OnTestMaps() 
{
	CStdMap<string, int> oMap;
	CStdPtrMap<string, CRobotLeg> oLegMap, oCopyMap;
	CRobotLeg *lpLeg = NULL;
	CStdPtrArray<CRobotLeg> aryLegs;

try
{
//	oMap.Add("Test1", 100);
//	oMap.Add("Test2", 101);
//	oMap.Add("Test3", 101);
//	oMap.Add("Test4", 103);

	LOG_MSG(StdLogInfo, "Testing Maps");

	lpLeg = new CRobotLeg;
	lpLeg->m_iLength = 1;
	aryLegs.Add(lpLeg);
	lpLeg = NULL;

	lpLeg = new CRobotLeg;
	lpLeg->m_iLength = 2;
	aryLegs.Add(lpLeg);
	lpLeg = NULL;

	lpLeg = new CRobotLeg;
	lpLeg->m_iLength = 3;
	aryLegs.Add(lpLeg);
	lpLeg = NULL;

	lpLeg = new CRobotLeg;
	lpLeg->m_iLength = 4;
	aryLegs.Add(lpLeg);
	lpLeg = NULL;

	vector<CRobotLeg*>::iterator oPos;
	oPos = aryLegs.begin()+1;
	aryLegs.erase(oPos);

	aryLegs.RemoveAt(2);
	aryLegs.RemoveAll();

/*
	lpLeg = new CRobotLeg;
	lpLeg->m_iLength = 1;
	oLegMap.Add("Leg1", lpLeg);
	lpLeg = NULL;

	lpLeg = new CRobotLeg;
	lpLeg->m_iLength = 2;
	oLegMap.Add("Leg2", lpLeg);
	lpLeg = NULL;

	lpLeg = new CRobotLeg;
	lpLeg->m_iLength = 3;
	oLegMap.Add("Leg3", lpLeg);
	lpLeg = NULL;

	CopyPtrMap(oLegMap, oCopyMap);
	int iSize = oCopyMap.GetSize();

	lpLeg = oLegMap["Leg3"];
	lpLeg = oLegMap["Leg4"];

	oLegMap.Remove("Leg2");
	oLegMap.Remove("Leg4");
	
	oCopyMap.erase(oCopyMap.begin(), oCopyMap.end());
	iSize = oCopyMap.GetSize();

	oLegMap.RemoveAll();
	oCopyMap.RemoveAll();

	LOG_MSG(StdLogInfo, "Done Testing Maps");

	THROW_PARAM_ERROR(-1000, "Test", "MyParam", true);
*/
}
catch(CStdErrorInfo oError)
{
	if(lpLeg) delete lpLeg;
	AfxMessageBox(oError.m_strError);
}
catch(...)
{
	if(lpLeg) delete lpLeg;
	AfxMessageBox("Unidentified Error");
}
}


void Test2()
{
	THROW_ERROR(Std_Err_lNotBoolType, Std_Err_strNotBoolType);
}


void Test1()
{

try
{
	Test2();
}
catch(CStdErrorInfo oError)
{RELAY_ERROR(oError);}
catch(...)
{THROW_ERROR(Std_Err_lUnspecifiedError, Std_Err_strUnspecifiedError);}
}


void CStdUtilsTestDlg::OnTestStrings() 
{
	unsigned char iUChar = 1;
	unsigned short iUShort=5;
	int iInt = 10;
	long lLong = 15;
	float fltFloat = (float) 5.3;
	double dblDouble = 7.5;
	bool bBool = true;

try
{
//	LOG_TEMP(StdLogInfo, "Test Val %d, Float: %f", 5403, dblDouble);

	CStdString strVal;

	strVal = (int) iUChar;
	strVal = iUShort;
	strVal = iInt;
	strVal = lLong;
	strVal = fltFloat;
	strVal = dblDouble;

	CStdString strUShort(iUShort), strInt(iInt), strLong(lLong);
	CStdString strFloat(fltFloat), strDouble(dblDouble);
	CStdString strTest1("The Test value is : ");
	CStdString strTest2(" Is the test value.");

	strVal = strTest1 + STR(iInt);
	strVal = strTest1 + STR(lLong);
	strVal = strTest1 + STR(fltFloat);
	strVal = strTest1 + STR(dblDouble);

	strVal = STR(iInt) + strTest2;
	strVal = STR(lLong) + strTest2;
	strVal = STR(fltFloat) + strTest2;
	strVal = STR(dblDouble) + strTest2;

	strVal = "This is a test: " + STR(iInt);
	strVal = "This is a test: " + STR(lLong);
	strVal = "This is a test: " + STR(fltFloat);
	strVal = "This is a test: " + STR(bBool);
	strVal = "This is a test: " + STR("100");

	TRACE_INFO("This is a test. Test Value: " + STR(lLong));
	TRACE_DETAIL("Fitness: " + STR(fltFloat));

	strVal = "This is a test: " + FSTR("%5.2f", fltFloat);

	Test1();
}
catch(CStdErrorInfo oError)
{
	CStdString strError = oError.Log();
	CString strMSError = strError.c_str();
	AfxMessageBox(strMSError);
}
catch(...)
{AfxMessageBox("Unidentified Error");}
}






void CStdUtilsTestDlg::OnTestVariant() 
{
	
try
{
	CStdVariant oV1, oV2;
	long lVal=5;
	unsigned char ucVal=10;
	CStdString strVal;

	oV1.SetValue(15);
	strVal = oV1.GetString();
	lVal = oV1.GetLong();
	TRACE_STL_OBJ(&oV1);

	oV1.SetValue(lVal);
	lVal=20;
	TRACE_STL_OBJ(&oV1);

	oV1.SetPtr(&lVal);
	lVal=30;
	TRACE_STL_OBJ(&oV1);
	strVal = oV1.GetString();

	oV1.Reset();
	TRACE_STL_OBJ(&oV1);

	oV1.SetValue(35);
	oV2 = oV1;
	TRACE_STL_OBJ(&oV2);

	oV1.SetPtr(&ucVal);
	oV2 = oV1;
	ucVal=68;
	strVal = oV1.GetString();
	strVal = oV2.GetString();

	oV1.Reset();
	oV2.Reset();
}
catch(CStdErrorInfo oError)
{
	CStdString strError = oError.Log();
	CString strMSError = strError.c_str();
	AfxMessageBox(strMSError);
}
catch(...)
{AfxMessageBox("Unidentified Error");}
}


void CStdUtilsTestDlg::OnTestOdbc() 
{
	CStdOdbcConnection oConn;

try
{
	oConn.Connect("GeneticData", "DevSimMgr", "DevSim");

//	int iRowsAffected = oConn.ExecuteSQL("Insert Into Test (StringVal, IntVal, FloatVal) Values ('This is a test', 65, 43.21)");
	long lID = oConn.GetNextID("Proteins", "ProteinDBID");

	oConn.Close();
}
catch(CStdErrorInfo oError)
{
	CStdString strError = oError.Log();
	CString strMSError = strError.c_str();
	AfxMessageBox(strMSError);
}
catch(...)
{AfxMessageBox("Unidentified Error");}
}

void CStdUtilsTestDlg::OnTestRandom() 
{

try
{
	CStdString strTest = "50, 100, 200, 300", strVal, strCombined;
	CStdArray<string> aryVals;

	CStdString strTest11 = "Unable to open file.";
  strTest11 += "C:\\Projects\\AnimatLab\\VortexSimulator\\Crayfish\\TestNeuron.adc";
	strTest11 += "End tag does not correspond to OutputFilename11";

	int iCount = Std_Split(strTest, ",", aryVals);
	iCount = aryVals.GetSize();
	for(int iIndex=0; iIndex<iCount; iIndex++)
		strVal = aryVals[iIndex];

	strCombined = Std_Combine(aryVals, ",");
	
	CStdString strFullPath = "C:\\Projects\\Test\\Test.dat";
	CStdString strFilePath = "Test.ccd";
	CStdString strPath, strFile;

	if(Std_IsFullPath(strFullPath))
		strVal = "1";

	if(Std_IsFullPath(strFilePath))
		strVal = "2";

	Std_SplitPathAndFile(strFullPath, strPath, strFile);
	Std_SplitPathAndFile(strFilePath, strPath, strFile);

	Std_SRand(4357);
	for(long lVal=0; lVal<2000; lVal++)
		TRACE_DEBUG_NS(STR(Std_IRand(0, 10)));

	iCount = Std_Split("100", ",", aryVals);
	iCount = Std_Split("  ", ",", aryVals);
	

}
catch(CStdErrorInfo oError)
{
	CStdString strError = oError.Log();
	CString strMSError = strError.c_str();
	AfxMessageBox(strMSError);
}
catch(...)
{AfxMessageBox("Unidentified Error");}
}
