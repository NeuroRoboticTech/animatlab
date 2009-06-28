using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Diagnostics;
using System.IO;
using System.Xml;
using Crownwood.Magic.Controls;
using AnimatTools.Framework;
using Microsoft.DirectX;
using Microsoft.DirectX.Direct3D;
using VortexAnimatTools.DataObjects.Physical.PropertyHelpers;

namespace VortexAnimatTools.DataObjects.Physical.RigidBodies
{
	/// <summary>
	/// Summary description for Sensor.
	/// </summary>
	public abstract class Sensor : VortexAnimatTools.DataObjects.Physical.RigidBodies.Sphere
	{
		#region Attributes

		#endregion

		#region Properties

		public override bool CanBeRootBody {get{return false;}}
		public override bool UsesAJoint {get{return false;}}
		public override bool AllowCollisions {get{return false;}}

		#endregion

		#region Methods

		public Sensor(AnimatTools.Framework.DataObject doParent) : base(doParent)
		{
			Alpha = 128;
			Color = Color.LightGreen;
			this.m_fRadius = 0.1f;

			m_thDataTypes.DataTypes.Clear();
			m_thDataTypes.DataTypes.Add(new AnimatTools.DataObjects.DataType("BodyPositionX", "Position X Axis", "Meters", "m", -10, 10));
			m_thDataTypes.DataTypes.Add(new AnimatTools.DataObjects.DataType("BodyPositionY", "Position Y Axis", "Meters", "m", -10, 10));
			m_thDataTypes.DataTypes.Add(new AnimatTools.DataObjects.DataType("BodyPositionZ", "Position Z Axis", "Meters", "m", -10, 10));
			m_thDataTypes.ID = "BodyPositionX";

			m_aryCompatibleStimuli.Clear();
		}

		protected override void DrawReceptiveFields(AnimatTools.Forms.BodyPlan.Command.enumCommandMode cmCommand)
		{
			this.DrawRigidBody(cmCommand);
		}

		public override void AddChildBody(AnimatTools.DataObjects.Physical.RigidBody rbChildBody)
		{
			//If you try and add a body to a muscle attachment then you really need to add it to the attachment parent.
			if(this.Parent != null)
			{
				AnimatTools.DataObjects.Physical.RigidBody rbParent = (AnimatTools.DataObjects.Physical.RigidBody) this.Parent;
				
				rbChildBody.Parent = rbParent;
				rbParent.AddChildBody(rbChildBody);
			}
		}

		protected override void BuildProperties()
		{
			base.BuildProperties();

			if(m_Properties.Properties.Contains("Density") == true)
				m_Properties.Properties.Remove("Density");

			if(m_Properties.Properties.Contains("Texture") == true)
				m_Properties.Properties.Remove("Texture");

			if(m_Properties.Properties.Contains("Odor Sources") == true)
				m_Properties.Properties.Remove("Odor Sources");

			if(m_Properties.Properties.Contains("FoodSource") == true)
				m_Properties.Properties.Remove("FoodSource");

			//remove rotation
			if(m_Properties.Properties.Contains("X") == true)
				m_Properties.Properties.Remove("X");

			if(m_Properties.Properties.Contains("Y") == true)
				m_Properties.Properties.Remove("Y");

			if(m_Properties.Properties.Contains("Z") == true)
				m_Properties.Properties.Remove("Z");


		}

		protected override void SaveLocation(ref AnimatTools.DataObjects.Simulation dsSim, ref AnimatTools.DataObjects.Physical.PhysicalStructure doStructure, ref AnimatTools.Interfaces.StdXml oXml)
		{
			RigidBodies.RigidBody_DX rbParent = (RigidBodies.RigidBody_DX)this.Parent;
			Vector3 v3ParentRotation = Util_DX.DecomposeXYZRotationMatrix(rbParent.CombinedTransformationMatrix);

			//m_mtxTransformation.Multiply(Matrix.Translation(RelativeLocation.X, RelativeLocation.Y,RelativeLocation.Z));

			Matrix rotation = Matrix.RotationX(v3ParentRotation.X) * Matrix.RotationY(v3ParentRotation.Y) * Matrix.RotationZ(v3ParentRotation.Z);
			Matrix rotationInverse = rotation;
			rotationInverse.Invert();
			
			//get the absolute location of the parent and of this body
			Vector3 v3ParentAbsLoc= new Vector3(rbParent.CombinedTransformationMatrix.M41, rbParent.CombinedTransformationMatrix.M42, rbParent.CombinedTransformationMatrix.M43);
			Vector3 v3AbsLoc = new Vector3(this.CombinedTransformationMatrix.M41, this.CombinedTransformationMatrix.M42, this.CombinedTransformationMatrix.M43);
				
			//subtract the two vectors to get the relative location of this body
			Vector3 v3Loc = v3AbsLoc - v3ParentAbsLoc;		
		
			v3Loc.TransformCoordinate(rotationInverse);

			//m_v3Location.TransformCoordinate(rotationInverse);
			//Save the Simulator location of this body which is v3Loc
			Util.SaveVector(ref oXml, "RelativePosition", new Vec3d(null, v3Loc.X, v3Loc.Y, v3Loc.Z));

			//m_v3Location.TransformCoordinate(rotation);

			
		}

		public override void LoadData(ref AnimatTools.DataObjects.Simulation dsSim, ref AnimatTools.DataObjects.Physical.PhysicalStructure doStructure, ref AnimatTools.Interfaces.StdXml oXml)
		{
			m_aryChildBodies.Clear();

			oXml.IntoElem(); //'Into RigidBody Element

			m_strID = oXml.GetChildString("ID");
			m_strName = oXml.GetChildString("Name", m_strID);
			m_bVisible = oXml.GetChildBool("IsVisible", m_bVisible);

			if(m_strID.Trim().Length == 0)
				m_strID = System.Guid.NewGuid().ToString();

			if(m_strName.Trim().Length == 0)
				m_strName = m_strID;

			if(oXml.FindChildElement("Color", false))
			{
				System.Drawing.Color oColor = Util.LoadColor(ref oXml, "Color");
				this.Alpha = oColor.A;
				this.Color = oColor;			
			}

			m_fRadius = oXml.GetChildFloat("Radius", m_fRadius);

			if(oXml.FindChildElement("Direction",false))
			{
				this.m_v3Direction = new Vector3();
				Vec3d vDirection = Util.LoadVec3d(ref oXml, "Direction", this);
				m_v3Direction.X = (float)vDirection.X;
				m_v3Direction.Y = (float)vDirection.Y;
				m_v3Direction.Z = (float)vDirection.Z;
			}
			if(oXml.FindChildElement("OrientationMatrix", false))
			{
				m_mtxOrientation = Util_DX.LoadMatrix(ref oXml, "OrientationMatrix");
				m_v3Rotation = Util_DX.DecomposeXYZRotationMatrix(Orientation);
				this.Rotation = new Vec3d(null,Geometry.RadianToDegree(m_v3Rotation.X),Geometry.RadianToDegree(m_v3Rotation.Y),Geometry.RadianToDegree(m_v3Rotation.Z));
			}

			if(oXml.FindChildElement("TranslationMatrix",false))
				m_mtxTranslation = Util_DX.LoadMatrix(ref oXml,"TranslationMatrix");				
	
			oXml.OutOfElem(); //Outof RigidBody Element	
		}

		#endregion

	}
}