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
	/// Summary description for Stomach.
	/// </summary>
	public class Stomach : VortexAnimatTools.DataObjects.Physical.RigidBodies.Sphere
	{
		#region Attributes

		private bool m_bAdjusted = false;
		private bool m_bKillOrganism = true;
		protected ScaledNumber m_snEnergyLevel;
		protected ScaledNumber m_snMaxEnergyLevel;
		protected ScaledNumber m_snBaseConsumptionRate;

		#endregion

		#region Properties

		public override String ImageName {get{return "VortexAnimatTools.Graphics.Stomach_Treeview.gif";}}
		public override String ButtonImageName {get{return "VortexAnimatTools.Graphics.Stomach_Button.gif";}}
		public override String Type {get{return "Stomach";}}
		public override String BodyPartName {get{return "Stomach";}}
		public override System.Type PartType {get{return typeof(VortexAnimatTools.DataObjects.Physical.RigidBodies.Stomach);}}

		public override bool CanBeRootBody {get{return false;}}
		public override bool UsesAJoint {get{return false;}}
		public override bool AllowCollisions {get{return false;}}
		public override bool HasDynamics {get{return false;}}

		[Browsable(false)]
		public bool KillOrganism
		{
			get
			{ 
				return m_bKillOrganism;
			}
			set
			{
				m_bKillOrganism = value;
			}
		}

		[Browsable(false)]
		public AnimatTools.Framework.ScaledNumber EnergyLevel
		{
			get
			{ 
				return m_snEnergyLevel;
			}
			set
			{
				if(value.ActualValue <= 0)
					throw new System.Exception("The energy level must be larger than 0.");

				m_snEnergyLevel.CopyData(ref value);
			}
		}

		[Browsable(false)]
		public AnimatTools.Framework.ScaledNumber MaxEnergyLevel
		{
			get
			{ 
				return m_snMaxEnergyLevel;
			}
			set
			{
				if(value.ActualValue <= 0)
					throw new System.Exception("The max energy level must be larger than 0.");

				m_snMaxEnergyLevel.CopyData(ref value);
			}
		}

		[Browsable(false)]
		public AnimatTools.Framework.ScaledNumber BaseConsumptionRate
		{
			get
			{ 
				return m_snBaseConsumptionRate;
			}
			set
			{
				if(value.ActualValue <= 0)
					throw new System.Exception("The base consumption rate must be larger than 0.");

				m_snBaseConsumptionRate.CopyData(ref value);
			}
		}

		#endregion

		#region Methods

		public Stomach(AnimatTools.Framework.DataObject doParent) : base(doParent)
		{
			Alpha = 128;
			Color = Color.LightBlue;
			this.m_fRadius = 0.1f;

			m_snMaxEnergyLevel = new ScaledNumber(this, "MaxEnergyLevel", 100, ScaledNumber.enumNumericScale.Kilo, "Calories", "C");
			m_snEnergyLevel = new ScaledNumber(this, "EnergyLevel", 10, ScaledNumber.enumNumericScale.Kilo, "Calories", "C");
			m_snBaseConsumptionRate = new ScaledNumber(this, "BaseConsumptionRate", 10, ScaledNumber.enumNumericScale.None, "C/s", "C/s");

			m_thDataTypes.DataTypes.Clear();
			m_thDataTypes.DataTypes.Add(new AnimatTools.DataObjects.DataType("BodyPositionX", "Position X Axis", "Meters", "m", -10, 10));
			m_thDataTypes.DataTypes.Add(new AnimatTools.DataObjects.DataType("BodyPositionY", "Position Y Axis", "Meters", "m", -10, 10));
			m_thDataTypes.DataTypes.Add(new AnimatTools.DataObjects.DataType("BodyPositionZ", "Position Z Axis", "Meters", "m", -10, 10));
			m_thDataTypes.DataTypes.Add(new AnimatTools.DataObjects.DataType("EnergyLevel", "Energy Level", "", "", -10000, 10000));
			m_thDataTypes.DataTypes.Add(new AnimatTools.DataObjects.DataType("ConsumptionRate", "Consumption Rate", "", "", -10000, 10000));
			m_thDataTypes.DataTypes.Add(new AnimatTools.DataObjects.DataType("ConsumptionForStep", "Consumption For Step", "", "", -10000, 10000));
			m_thDataTypes.DataTypes.Add(new AnimatTools.DataObjects.DataType("AdapterConsumptionRate", "Adapter Consumption Rate", "", "", -10000, 10000));
			m_thDataTypes.ID = "EnergyLevel";

			this.ID = "STOMACH";
		}

		protected override void CreateBody()
		{
			RigidBodies.RigidBody_DX rbParent = (RigidBodies.RigidBody_DX)this.Parent;
			if(!m_bAdjusted)
			{
				//dwc I am not sure what this is supposed to be doing???
//				Vector3 v3ParentRotation = rbParent.DxRotation;
//				m_v3LocalLocation.TransformCoordinate(Matrix.RotationX(-v3ParentRotation.X) * Matrix.RotationY(v3ParentRotation.Y) * Matrix.RotationZ(-v3ParentRotation.Z));
				m_bAdjusted = true;
			}
			base.CreateBody ();

		}

		protected override void DrawReceptiveFields(AnimatTools.Forms.BodyPlan.Command.enumCommandMode cmCommand)
		{
			this.DrawRigidBody(cmCommand);
		}

		public override void BeforeAddBody()
		{
			if(this.ParentStructure != null && this.ParentStructure.FindBodyPart("STOMACH", false) != null)
				throw new System.Exception("You can only add one Stomach to an organism.");
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

		public override AnimatTools.Collections.BodyParts SwapBodyPartList()
		{
			AnimatTools.Collections.BodyParts aryList = new AnimatTools.Collections.BodyParts(null);

			foreach(AnimatTools.DataObjects.Physical.RigidBody doPart in Util.Application.RigidBodyTypes)
			{
				if(Util.IsTypeOf(doPart.GetType(), typeof(VortexAnimatTools.DataObjects.Physical.RigidBodies.Sensor), false))
					aryList.Add(doPart);
			}
			aryList.Add(new VortexAnimatTools.DataObjects.Physical.RigidBodies.Mouth(null));
			aryList.Add(new VortexAnimatTools.DataObjects.Physical.RigidBodies.MuscleAttachment(null));
			aryList.Add(new VortexAnimatTools.DataObjects.Physical.RigidBodies.Stomach(null));

			return aryList;
		}

		protected override void BuildProperties()
		{
			base.BuildProperties();

			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Kill Organism", m_bKillOrganism.GetType(), "KillOrganism", 
				"Part Properties", "If this is true and the energy level reaches 0 then it will kill this organism.", m_bKillOrganism));

			Crownwood.Magic.Controls.PropertyBag pbNumberBag  = m_snMaxEnergyLevel.Properties;
			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Max Energy Level", pbNumberBag.GetType(), "MaxEnergyLevel", 
				"Part Properties", "Sets the maximum energy level of the organism.", pbNumberBag,
				"", typeof(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)));

			pbNumberBag  = m_snEnergyLevel.Properties;
			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Energy Level", pbNumberBag.GetType(), "EnergyLevel", 
																	"Part Properties", "Sets the initial energy level of the organism.", pbNumberBag,
																	"", typeof(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)));

			pbNumberBag  = m_snBaseConsumptionRate.Properties;
			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Base Consumption Rate", pbNumberBag.GetType(), "BaseConsumptionRate", 
																	"Part Properties", "Sets the rate of consumption of calories for the resting state.", pbNumberBag,
																	"", typeof(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)));

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

		public override void SaveData(ref AnimatTools.DataObjects.Simulation dsSim, ref AnimatTools.DataObjects.Physical.PhysicalStructure doStructure, ref AnimatTools.Interfaces.StdXml oXml)
		{
			base.SaveData (ref dsSim, ref doStructure, ref oXml);

			oXml.IntoElem();
					
			oXml.AddChildElement("KillOrganism", m_bKillOrganism);
			m_snMaxEnergyLevel.SaveData(ref oXml, "MaxEnergyLevel");
			m_snEnergyLevel.SaveData(ref oXml, "EnergyLevel");
			m_snBaseConsumptionRate.SaveData(ref oXml, "BaseConsumptionRate");

			oXml.OutOfElem(); //out of body			
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

			this.ID = "Stomach";
			m_strName = oXml.GetChildString("Name", m_strID);
			m_bVisible = oXml.GetChildBool("IsVisible", m_bVisible);

			if(m_strID.Trim().Length == 0)
				m_strID = System.Guid.NewGuid().ToString();

			if(m_strName.Trim().Length == 0)
				m_strName = m_strID;

			m_bKillOrganism = oXml.GetChildBool("KillOrganism", m_bKillOrganism);

			if(oXml.FindChildElement("Color", false))
			{
				System.Drawing.Color oColor = Util.LoadColor(ref oXml, "Color");
				this.Alpha = oColor.A;
				this.Color = oColor;			
			}

			m_fRadius = oXml.GetChildFloat("Radius", m_fRadius);
			m_snMaxEnergyLevel.LoadData(ref oXml, "MaxEnergyLevel");
			m_snEnergyLevel.LoadData(ref oXml, "EnergyLevel");
			m_snBaseConsumptionRate.LoadData(ref oXml, "BaseConsumptionRate");

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

		public override AnimatTools.DataObjects.Physical.BodyPart CreateNewBodyPart(AnimatTools.Framework.DataObject doParent)
		{return new Stomach(doParent);}		

		public override AnimatTools.Framework.DataObject Clone(AnimatTools.Framework.DataObject doParent, bool bCutData, AnimatTools.Framework.DataObject doRoot)
		{
			Stomach bnPart = new Stomach(doParent);
			bnPart.CloneInternal(this, bCutData, doRoot);
			bnPart.m_bKillOrganism = m_bKillOrganism;
			bnPart.m_snMaxEnergyLevel = (ScaledNumber) m_snMaxEnergyLevel.Clone(bnPart, bCutData, doRoot);
			bnPart.m_snEnergyLevel = (ScaledNumber) m_snEnergyLevel.Clone(bnPart, bCutData, doRoot);
			bnPart.m_snBaseConsumptionRate = (ScaledNumber) m_snBaseConsumptionRate.Clone(bnPart, bCutData, doRoot);
			if(doRoot != null && doRoot == this) bnPart.AfterClone(this, bCutData, doRoot, bnPart);
			return bnPart;
		}

		#endregion

	}
}
