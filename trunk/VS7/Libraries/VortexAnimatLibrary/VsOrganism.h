// VsOrganism.h: interface for the VsOrganism class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VSORGANISM_H__8E0C0060_8F52_4E17_BF36_B05EFE795684__INCLUDED_)
#define AFX_VSORGANISM_H__8E0C0060_8F52_4E17_BF36_B05EFE795684__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

namespace VortexAnimatLibrary
{
	namespace Environment
	{
		class VORTEX_PORT VsOrganism : public Organism   
		{
		public:
			VsOrganism();
			virtual ~VsOrganism();

			virtual void Initialize(Simulator *lpSim);
		};

	}			// Environment
}				//VortexAnimatLibrary

#endif // !defined(AFX_VSORGANISM_H__8E0C0060_8F52_4E17_BF36_B05EFE795684__INCLUDED_)
