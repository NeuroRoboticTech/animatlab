// DataColumn.cpp: implementation of the DataColumn class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ActivatedItem.h"
#include "DataColumn.h"
#include "DataChart.h"

namespace AnimatLibrary
{
	namespace Charting
	{

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DataColumn::DataColumn()
{
	m_lpDataValue = NULL;
	m_iAppendSpaces = 0;
	m_bInitialized = FALSE;
	m_iIndex = -1;
	m_iColumnIndex = -1;
	m_iRowIndex = -1; 
}

DataColumn::~DataColumn()
{
	//Do not delete this pointer. It is just a reference.
	m_lpDataValue = NULL;
}

int DataColumn::ColumnCount()
{return 1;}

void DataColumn::ReInitialize(Simulator *lpSim)
{
	if(!m_bInitialized)
		Initialize(lpSim);
}

void DataColumn::SaveColumnNames(ofstream &oStream)
{
	oStream << m_strColumnName;

	if(m_iAppendSpaces > 0)
	{
		for(int iIndex=0; iIndex<m_iAppendSpaces; iIndex++)
			oStream << " \t";
	}
}

void DataColumn::StepSimulation(Simulator *lpSim, DataChart *lpChart)
{
	lpChart->AddData(m_iColumnIndex, m_iRowIndex, *m_lpDataValue);
}

BOOL DataColumn::operator<(DataColumn *lpColumn)
{
	if(this->m_iIndex < lpColumn->m_iIndex)
		return TRUE;

	//if(this->m_strID < lpColumn->m_strID)
	//	return TRUE;

	return FALSE;
}

void DataColumn::Load(Simulator *lpSim, CStdXml &oXml)
{
	oXml.IntoElem();  //Into DataColumn Element

	m_strColumnName = oXml.GetChildString("ColumnName");

	if(Std_IsBlank(m_strColumnName)) 
		THROW_TEXT_ERROR(Std_Err_lBlankAttrib, Std_Err_strBlankAttrib, "Attribute: ColumnName");

	m_strID = oXml.GetChildString("ID", m_strColumnName);

	m_iAppendSpaces = oXml.GetChildInt("AppendSpaces", m_iAppendSpaces);
	Std_InValidRange((int) 0, (int) 10, m_iAppendSpaces, TRUE, "AppendSpaces");

	m_iIndex = oXml.GetChildInt("Index", m_iIndex);

	m_iColumnIndex = oXml.GetChildInt("Column", m_iColumnIndex);
	m_iRowIndex = oXml.GetChildInt("Row", m_iColumnIndex);

	oXml.OutOfElem(); //OutOf DataColumn Element
}


BOOL LessThanDataColumnCompare(DataColumn *lpColumn1, DataColumn *lpColumn2)
{
	return lpColumn1->operator<(lpColumn2);
}

	}			//Charting
}				//AnimatLibrary
