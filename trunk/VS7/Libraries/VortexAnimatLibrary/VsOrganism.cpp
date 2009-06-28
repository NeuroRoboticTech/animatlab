// VsMuscle.cpp: implementation of the VsMuscle class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "VsOrganism.h"
#include "VsJoint.h"
#include "VsRigidBody.h"
#include "VsSimulator.h"

namespace VortexAnimatLibrary
{
	namespace Environment
	{

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

VsOrganism::VsOrganism()
{

}

VsOrganism::~VsOrganism()
{

}


void VsOrganism::Initialize(Simulator *lpSim)
{
	Organism::Initialize(lpSim);

	//Now lets disable any collisions that have been added to the exclusion list.
	int iCount = m_aryExcludeCollisionList.GetSize();
	CollisionPair *lpPair = NULL;
	RigidBody *lpPart1=NULL, *lpPart2=NULL;
	VsRigidBody *lpVsPart1=NULL, *lpVsPart2=NULL;

	for(int iIndex=0; iIndex<iCount; iIndex++)
	{
		lpPair =	m_aryExcludeCollisionList[iIndex];
		lpPart1 = this->FindRigidBody(lpPair->m_strPart1ID);
		lpPart2 = this->FindRigidBody(lpPair->m_strPart2ID);

		lpVsPart1 = dynamic_cast<VsRigidBody *>(lpPart1);
		lpVsPart2 = dynamic_cast<VsRigidBody *>(lpPart2);

		McdSpaceDisablePair(lpVsPart1->ModelID(), lpVsPart2->ModelID());
	}
}

	}			// Environment
}				//VortexAnimatLibrary
