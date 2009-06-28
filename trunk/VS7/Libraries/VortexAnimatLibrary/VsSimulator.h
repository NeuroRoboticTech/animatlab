// VsSimulator.h: interface for the VsSimulator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VSSIMULATOR_H__2CAD431A_1D6E_446F_90CD_728509081187__INCLUDED_)
#define AFX_VSSIMULATOR_H__2CAD431A_1D6E_446F_90CD_728509081187__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

namespace VortexAnimatLibrary
{

	class VORTEX_PORT VsSimulator : public Simulator  
	{
	protected:

		MstUniverseID m_iUniverse;
		MdtWorldID m_iWorld;
		MstBridgeID m_iBridge;
		MeApp *m_lpApp;
		RRender *m_lpRenderContext;
		
		MeVector3 m_oTrackPos;
		VsRigidBody *m_lpTrackBody;

		MeMatrix4 m_aryGroundTM; 

		long m_lTimer;

		virtual AnimatLibrary::Recording::SimulationRecorder *CreateSimulationRecorder();
		virtual void SnapshotStopFrame();

	public:
		VsSimulator();
		virtual ~VsSimulator();

		MstUniverseID Universe();
		MdtWorldID World();
		MeApp *App();
		RRender *RenderContext();
		VsRigidBody *TrackBody();

		virtual void PlaybackRate(float fltVal);
		virtual void UseAlphaBlending(BOOL bVal);
		virtual void TrackCamera(BOOL bTrackCamera, string strLookAtStructureID, string strLookAtBodyID);

		virtual void Reset();
		virtual void Initialize(int argc, const char **argv);
		virtual void StepSimulation();
		virtual void Simulate();
		virtual void ShutdownSimulation();
		virtual void TogglePauseSimulation();
		virtual void StopSimulation();
		virtual BOOL StartSimulation();
		virtual BOOL PauseSimulation();

		static VsSimulator *ConvertSimulator(Simulator *lpSim);
		//static VsSimulator *ConvertSimulator(Simulator *lpSim);
	};

}			//VortexAnimatLibrary

#endif // !defined(AFX_VSSIMULATOR_H__2CAD431A_1D6E_446F_90CD_728509081187__INCLUDED_)
