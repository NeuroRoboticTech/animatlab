using System;

namespace VortexAnimatTools.DataObjects
{
	/// <summary>
	/// Summary description for Simulation.
	/// </summary>
	public class Simulation : AnimatTools.DataObjects.Simulation
	{
		enum enumClassTypes
		{
			RigidBody,
			Joint
		};

		enum enumRigidBodyTypes
		{
			Box,
			BoxContactSensor,
			Cone,
			Cylinder,
			CylinderContactSensor,
			Mesh,
			Mouth,
			MuscleAttachment,
			Muscle,
			MuscleSpindle,
			LinearHillMuscle,
			LinearHillStretchReceptor,
			OdorSensor,
			Plane,
			Sensor,
			Sphere,
			Spring,
			MultiSegmentSpring,
			Stomach
	};

		enum enumJointTypes
		{
			BallSocket,
			Hinge,
			Prismatic,
			Static
		};

		public Simulation() : base(null)
		{
		}

		public Simulation(AnimatTools.Framework.DataObject doParent) : base(doParent)
		{
		}

		public override AnimatTools.Framework.DataObject CreateObject(string strClassType, string strObjectType, AnimatTools.Framework.DataObject doParent)
		{
			enumClassTypes eClass;

			try
			{
				eClass = (enumClassTypes) Enum.Parse(typeof(enumClassTypes), strClassType, true);
			}
			catch(System.Exception)
			{throw new System.Exception("Could not convert the class type '" + strClassType + "' to an enumeration.");}

			switch(eClass)
			{
				case enumClassTypes.Joint:
					return CreateJoint(strObjectType, doParent);

				case enumClassTypes.RigidBody:
					return CreateRigidBody(strObjectType, doParent);
			}

			return null;
		}

		protected AnimatTools.Framework.DataObject CreateJoint(string strObjectType, AnimatTools.Framework.DataObject doParent)
		{
			enumJointTypes eJoint;

			try
			{
				eJoint = (enumJointTypes) Enum.Parse(typeof(enumJointTypes), strObjectType, true);
			}
			catch(System.Exception)
			{throw new System.Exception("Could not convert the joint type '" + strObjectType + "' to an enumeration.");}

			switch(eJoint)
			{
				case enumJointTypes.BallSocket:
					return new VortexAnimatTools.DataObjects.Physical.Joints.BallSocket(doParent);

				case enumJointTypes.Hinge:
					return new VortexAnimatTools.DataObjects.Physical.Joints.Hinge(doParent);

				case enumJointTypes.Prismatic:
					return new VortexAnimatTools.DataObjects.Physical.Joints.Prismatic(doParent);

				case enumJointTypes.Static:
					return new VortexAnimatTools.DataObjects.Physical.Joints.Static(doParent);
			}

			throw new System.Exception("Could not create the joint type '" + strObjectType + "'.");
		}

		protected AnimatTools.Framework.DataObject CreateRigidBody(string strObjectType, AnimatTools.Framework.DataObject doParent)
		{
			enumRigidBodyTypes eBody;

			try
			{
				eBody = (enumRigidBodyTypes) Enum.Parse(typeof(enumRigidBodyTypes), strObjectType, true);
			}
			catch(System.Exception)
			{throw new System.Exception("Could not convert the rigid body type '" + strObjectType + "' to an enumeration.");}

			switch(eBody)
			{
				case enumRigidBodyTypes.Box:
					return new VortexAnimatTools.DataObjects.Physical.RigidBodies.Box(doParent);

				case enumRigidBodyTypes.BoxContactSensor:
					return new VortexAnimatTools.DataObjects.Physical.RigidBodies.BoxContactSensor(doParent);

				case enumRigidBodyTypes.Cone:
					return new VortexAnimatTools.DataObjects.Physical.RigidBodies.Cone(doParent);

				case enumRigidBodyTypes.Cylinder:
					return new VortexAnimatTools.DataObjects.Physical.RigidBodies.Cylinder(doParent);

				case enumRigidBodyTypes.CylinderContactSensor:
					return new VortexAnimatTools.DataObjects.Physical.RigidBodies.CylinderContactSensor(doParent);

				case enumRigidBodyTypes.Mesh:
					return new VortexAnimatTools.DataObjects.Physical.RigidBodies.BodyMesh(doParent);
 
				case enumRigidBodyTypes.Mouth:
					return new VortexAnimatTools.DataObjects.Physical.RigidBodies.Mouth(doParent);

				case enumRigidBodyTypes.LinearHillMuscle:
					return new VortexAnimatTools.DataObjects.Physical.RigidBodies.LinearHillMuscle(doParent);

				case enumRigidBodyTypes.LinearHillStretchReceptor:
					return new VortexAnimatTools.DataObjects.Physical.RigidBodies.LinearHillStretchReceptor(doParent);

				case enumRigidBodyTypes.MuscleAttachment:
					return new VortexAnimatTools.DataObjects.Physical.RigidBodies.MuscleAttachment(doParent);

				case enumRigidBodyTypes.OdorSensor:
					return new VortexAnimatTools.DataObjects.Physical.RigidBodies.OdorSensor(doParent);

				case enumRigidBodyTypes.Plane:
					return new VortexAnimatTools.DataObjects.Physical.RigidBodies.Plane(doParent);
	
				case enumRigidBodyTypes.Sphere:
					return new VortexAnimatTools.DataObjects.Physical.RigidBodies.Sphere(doParent);

				case enumRigidBodyTypes.Spring:
					return new VortexAnimatTools.DataObjects.Physical.RigidBodies.Spring(doParent);

				case enumRigidBodyTypes.MultiSegmentSpring:
					return new VortexAnimatTools.DataObjects.Physical.RigidBodies.MultiSegmentSpring(doParent);

				case enumRigidBodyTypes.Stomach:
					return new VortexAnimatTools.DataObjects.Physical.RigidBodies.Stomach(doParent);
			}

			throw new System.Exception("Could not create the rigid body type '" + strObjectType + "'.");
		}

		public override AnimatTools.Framework.DataObject Clone(AnimatTools.Framework.DataObject doParent, bool bCutData, AnimatTools.Framework.DataObject doRoot)
		{
			return null;
		}

	}
}
