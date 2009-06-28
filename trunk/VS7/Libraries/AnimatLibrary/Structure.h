// AlStructure.h: interface for the Structure class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ALSTRUCTURE_H__9FEE3153_B3B6_4064_B93B_35265C06E366__INCLUDED_)
#define AFX_ALSTRUCTURE_H__9FEE3153_B3B6_4064_B93B_35265C06E366__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

namespace AnimatLibrary
{
	namespace Environment
	{
		class ANIMAT_PORT CollisionPair
		{
		public:
			string m_strPart1ID;
			string m_strPart2ID;

			CollisionPair() {};
			~CollisionPair() {};
		};


		/*! \brief 
			A "static" structure in the simulation.

			\remarks
			This object is a "static" structure in the simulation. I say "static" 
			because can move, but it is part of the background environment and does
			not have a brain controlling its movement. For example, if you wanted to
			have a house as an obstacle in the environemt then that house would probably
			have a door. The door would be connected to the wall of the house using a 
			hinge joint. So the door can open and close, but it does not have a brain 
			to control its movement. Another example is a rock. If you create a rock and
			place it at a precarious position on the top of a hill then it will move by
			rolling down the hill, but this is a passive effect of gravity acting on the
			body of the rock, and not something that the rock actively did. An organism
			on the other hand does have a neural network that actively controls the 
			movements of its limbs to change its position and orientation within the world.
					
			\sa
			Structure, Organism, Body, Joint
				
			\ingroup AnimatLibrary
		*/

		class ANIMAT_PORT Structure : public CStdSerialize 
		{
		protected:
			///The unique Id for this structure. .
			string m_strID;

			///The name for this structure. .
			string m_strName;

			///The path to the directory to use for this project.
			string m_strProjectPath;

			///The filename of the layout configuration of this structure.
			string m_strLayoutFile;

			///The root rigid body object of this structure.
			RigidBody *m_lpBody;

			///The initial position of this structure in world coordinates.
			///The root rigid body position is relative to this position.
			CStdFPoint m_oPosition;

			///A list of rigid bodies contained within this structure.
			///The objects in this list are references only. They are not
			///destroyed by this list. It is used primarily for finding
			///rigid bodies and ensuring that only unique ID's are used
			///for the rigid bodies.
			CStdMap<string, RigidBody *> m_aryRigidBodies;

			///A list of joints contained within this structure.
			///The objects in this list are references only. They are not
			///destroyed by this list. It is used primarily for finding
			///joints and ensuring that only unique ID's are used
			///for the joints.
			CStdMap<string, Joint *> m_aryJoints;

			///This is the list of other body part ID's to exclude from collision tests.
			CStdPtrArray<CollisionPair> m_aryExcludeCollisionList;

			CStdPtrArray<Microcontroller> m_aryMicrocontrollers;

			virtual Microcontroller *LoadMicrocontroller(Simulator *lpSim, CStdXml &oXml);

			virtual void LoadLayout(Simulator *lpSim);
			virtual void LoadLayout(Simulator *lpSim, CStdXml &oXml);
			void Structure::LoadCollisionPair(CStdXml &oXml);

		public:
			Structure();
			virtual ~Structure();

			string ID() {return m_strID;};
			void ID(string strValue) {m_strID = strValue;};

			string Name() {return m_strName;};
			void Name(string strValue) {m_strName = strValue;};

			string ProjectPath() {return m_strProjectPath;};
			void ProjectPath(string strValue) {m_strProjectPath = strValue;};

			string LayoutFile() {return m_strLayoutFile;};
			void LayoutFile(string strValue) {m_strLayoutFile = strValue;};

			CStdFPoint Position() {return m_oPosition;};
			void Position(CStdFPoint &oPoint) {m_oPosition = oPoint;};

			CStdPtrArray<CollisionPair> ExclusionList() {return m_aryExcludeCollisionList;};

			RigidBody *Body() {return m_lpBody;};

			CStdPtrArray<Microcontroller> *Microcontrollers() {return &m_aryMicrocontrollers;};

			virtual void Initialize(Simulator *lpSim);
			virtual void StepPhysicsEngine(Simulator *lpSim);

			void AddJoint(Joint *lpJoint);
			void AddRigidBody(RigidBody *lpBody);

			Joint *FindJoint(string strJointID, BOOL bThrowError = TRUE);
			RigidBody *FindRigidBody(string strBodyID, BOOL bThrowError = TRUE);
			Node *FindNode(string strID, BOOL bThrowError = TRUE);

			virtual void EnableMotor(string strJointID, BOOL bVal);
			virtual void SetMotorInput(string strJointID, float fltInput);

			virtual void EnableCollision(RigidBody *lpCollisionBody);
			virtual void DisableCollision(RigidBody *lpCollisionBody);

			virtual void Reset(Simulator *lpSim);

			virtual long CalculateSnapshotByteSize();
			virtual void SaveKeyFrameSnapshot(byte *aryBytes, long &lIndex);
			virtual void LoadKeyFrameSnapshot(byte *aryBytes, long &lIndex);

			virtual void Load(Simulator *lpSim, CStdXml &oXml);
		};

	}			// Environment
}				//AnimatLibrary

#endif // !defined(AFX_ALSTRUCTURE_H__9FEE3153_B3B6_4064_B93B_35265C06E366__INCLUDED_)
