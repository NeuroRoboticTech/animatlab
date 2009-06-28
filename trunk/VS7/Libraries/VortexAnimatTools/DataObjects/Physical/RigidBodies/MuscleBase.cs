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
	/// Summary description for MuscleBase.
	/// </summary>
	public abstract class MuscleBase : VortexAnimatTools.DataObjects.Physical.RigidBodies.RigidBody_DX, AnimatTools.DataObjects.Physical.IMuscle
	{

		#region Attributes

		protected System.Type m_tpBodyPartType;

		protected VortexAnimatTools.Collections.MuscleAttachments m_aryAttachmentPoints;
		protected ArrayList m_aryAttachmentPointIDs;

		protected AnimatTools.Framework.ScaledNumber m_snMaxTension;
		protected AnimatTools.Framework.ScaledNumber m_snMuscleLength;

		protected AnimatTools.DataObjects.Gains.MuscleGains.StimulusTension m_StimTension;
		protected AnimatTools.DataObjects.Gains.MuscleGains.LengthTension m_LengthTension;
		protected AnimatTools.DataObjects.Gains.MuscleGains.VelocityTension m_VelocityTension;

		#endregion

		#region Properties

		public override String ImageName {get{return "VortexAnimatTools.Graphics.Muscle_Treeview.gif";}}
		public override String ButtonImageName {get{return "VortexAnimatTools.Graphics.Muscle_Button.gif";}}
		public override bool AllowUserAdd {get{return true;}}
		public override bool CanBeRootBody {get{return false;}}
		public override bool UsesAJoint {get{return false;}}
		public override bool AllowCollisions {get{return false;}}

		public virtual VortexAnimatTools.Collections.MuscleAttachments AttachmentPoints
		{
				get{return m_aryAttachmentPoints;}
			set
			{
				if(value != null)
					m_aryAttachmentPoints = value;
			}
		}

		public virtual Crownwood.Magic.Controls.PropertyBag AttachmentPointsProps
		{
			get{return m_aryAttachmentPoints.Properties;}
			set{}
		}

		public virtual AnimatTools.Framework.ScaledNumber MuscleLength
		{get{return m_snMuscleLength;}}

		public virtual AnimatTools.Framework.ScaledNumber MaxTension
		{
			get{return m_snMaxTension;}
			set 
			{
				if(value != null)
				{
					if(value.ActualValue <= 0)
						throw new System.Exception("The Max tension must be greater than zero.");

					m_snMaxTension.CopyData(ref value);
				}
			}
		}
		

//		<Category("Equation Parameters"), 
//		Description("Sets the Y offset for the sigmoid for the ion-channel curve."), 
//		TypeConverter(GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter))>
		[Category("Stimulus-Tension"), Description("Sets the stmilus-tension properties of the muscle."), 
		 TypeConverter(typeof(Crownwood.Magic.Controls.ExpandablePropBagConverter))]
		public virtual AnimatTools.DataObjects.Gains.MuscleGains.StimulusTension StimTension
		{
			get
			{
				if(m_StimTension == null)
					m_StimTension = new AnimatTools.DataObjects.Gains.MuscleGains.StimulusTension(this);

				return m_StimTension;
			}
			set 
			{
				if(value != null)
					m_StimTension = value;
			}
		}

		public virtual AnimatTools.DataObjects.Gains.MuscleGains.LengthTension LengthTension
		{
			get
			{
				if(m_LengthTension == null)
					m_LengthTension = new AnimatTools.DataObjects.Gains.MuscleGains.LengthTension(this);

				return m_LengthTension;
			}
			set 
			{
				if(value != null)
					m_LengthTension = value;
			}
		}

		public virtual AnimatTools.DataObjects.Gains.MuscleGains.VelocityTension VelocityTension
		{
			get
			{
				if(m_VelocityTension == null)
					m_VelocityTension = new AnimatTools.DataObjects.Gains.MuscleGains.VelocityTension(this);

				return m_VelocityTension;
			}
			set 
			{
				if(value != null)
					m_VelocityTension = value;
			}
		}

		public override bool Enabled
		{
			get
			{
				return base.Enabled;
			}
			set
			{
				base.Enabled = value;

				if(this.BodyPlanTreeNode != null)
				{
					if(this.Enabled)
						this.BodyPlanTreeNode.BackColor = Color.White;
					else
						this.BodyPlanTreeNode.BackColor = Color.Gray;
				}
			}
		}

		public override float MaxDimension {get{return 0;}}
		public override float MinDimension {get{return 0;}}

		#endregion

		#region Methods

		public MuscleBase(AnimatTools.Framework.DataObject doParent) : base(doParent)
		{
			m_snMaxTension = new AnimatTools.Framework.ScaledNumber(this, "MaxTension", 100, AnimatTools.Framework.ScaledNumber.enumNumericScale.None, "Newtons", "N");
			m_snMuscleLength = new AnimatTools.Framework.ScaledNumber(this, "MuscleLength", 0, AnimatTools.Framework.ScaledNumber.enumNumericScale.None, "Meters", "m");

			RemoveCompatibleStimulus(new AnimatTools.DataObjects.ExternalStimuli.Force(null), false);

			AnimatTools.DataObjects.Physical.PhysicalStructure doStruct = null;
			if(doParent != null && doParent is AnimatTools.DataObjects.Physical.BodyPart)
			{
				AnimatTools.DataObjects.Physical.BodyPart bpPart = (AnimatTools.DataObjects.Physical.BodyPart) doParent;
				doStruct = bpPart.ParentStructure;
			}

			m_tpBodyPartType = typeof(VortexAnimatTools.DataObjects.Physical.RigidBodies.MuscleAttachment);
			m_aryAttachmentPoints = new VortexAnimatTools.Collections.MuscleAttachments(this);
			m_aryAttachmentPointIDs = new ArrayList();

			AddCompatibleStimulus(new AnimatTools.DataObjects.ExternalStimuli.Enabler(null));
		}

		protected override void CreateBody()
		{
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

		public override void AfterClone(AnimatTools.Framework.DataObject doParent, bool bCutData, AnimatTools.Framework.DataObject doOriginal, AnimatTools.Framework.DataObject doClone)
		{
			base.AfterClone (doParent, bCutData, doOriginal, doClone);

			if(Util.IsTypeOf(doClone.GetType(), typeof(AnimatTools.DataObjects.Physical.BodyPart), false))
			{
				//This is the new cloned root object, not the original root object
				AnimatTools.DataObjects.Physical.BodyPart doPart = (AnimatTools.DataObjects.Physical.BodyPart) doClone;
				VortexAnimatTools.Collections.MuscleAttachments aryAttach = (VortexAnimatTools.Collections.MuscleAttachments) m_aryAttachmentPoints.Copy(); 

				m_aryAttachmentPoints.Clear();
				foreach(MuscleAttachment doAttach in aryAttach)
				{
					AnimatTools.DataObjects.Physical.BodyPart doBase = doPart.FindBodyPartByCloneID(doAttach.ID);
					if(doBase != null)
						m_aryAttachmentPoints.Add((MuscleAttachment) doBase);
					else
						m_aryAttachmentPoints.Add(doAttach);
				}
			}
		}

		protected virtual float DrawAttachmentLine(MuscleAttachment doAttach1, MuscleAttachment doAttach2)
		{
			RigidBody_DX rbPrimary = (RigidBody_DX) doAttach1;
			RigidBody_DX rbSecondary = (RigidBody_DX) doAttach2;

			//Create the vertices that will represent each axis
			CustomVertex.PositionColored[] vertMuscle = new CustomVertex.PositionColored[2];

			Vector3 v3PrimLoc = rbPrimary.AbsoluteLocation;
			Vector3 v3SecLoc = rbSecondary.AbsoluteLocation;

			//calculate the muscle length;		
			float fltXd = v3PrimLoc.X - v3SecLoc.X;
			float fltYd = v3PrimLoc.Y - v3SecLoc.Y;
			float fltZd = v3PrimLoc.Z - v3SecLoc.Z;
			float fltLength = (float)Math.Sqrt( (fltXd*fltXd) + (fltYd*fltYd) + (fltZd*fltZd) ); 

			//define the muslce line
			vertMuscle[0].Position = v3PrimLoc;
			vertMuscle[1].Position = v3SecLoc;

			vertMuscle[0].Color = this.Color.ToArgb();
			vertMuscle[1].Color = this.Color.ToArgb();
				
			//tell the device what kind of vertex format we're using
			this.Device.VertexFormat = CustomVertex.PositionColored.Format;

			//draw the lines
			this.Device.RenderState.Lighting = false;
			//this.Device.Transform.World = Matrix.RotationX(DxRotation.X) * Matrix.RotationY(DxRotation.Y) * Matrix.RotationZ(DxRotation.Z) * this.Device.Transform.World1;  //moves the body to where it is in space
			this.Device.Transform.World = Device.Transform.World1;
			this.Device.DrawUserPrimitives(PrimitiveType.LineList, 1,vertMuscle);
			this.Device.RenderState.Lighting = true;

			return fltLength;
		}

		public override void Draw(AnimatTools.Forms.BodyPlan.Command.enumCommandMode cmCommand)
		{
			float fltLength = 0;

			if(m_aryAttachmentPoints.Count >= 2)
			{
				MuscleAttachment doAttach1 = m_aryAttachmentPoints[0], doAttach2;

				for(int iIndex=1; iIndex<m_aryAttachmentPoints.Count; iIndex++)
				{
					doAttach2 = m_aryAttachmentPoints[iIndex];
					fltLength += DrawAttachmentLine(doAttach1, doAttach2);
					doAttach1 = doAttach2;
				}
			}

			m_snMuscleLength.SetFromValue(fltLength * Util.Environment.DistanceUnitValue, (int) Util.Environment.DistanceUnits);
		}

		protected virtual float CalculateMuscleSegmentLength(MuscleAttachment doAttach1, MuscleAttachment doAttach2)
		{
			RigidBody_DX rbPrimary = (RigidBody_DX) doAttach1;
			RigidBody_DX rbSecondary = (RigidBody_DX) doAttach2;

			Vector3 v3PrimLoc = rbPrimary.AbsoluteLocation;
			Vector3 v3SecLoc = rbSecondary.AbsoluteLocation;

			//calculate the muscle length;		
			float fltXd = v3PrimLoc.X - v3SecLoc.X;
			float fltYd = v3PrimLoc.Y - v3SecLoc.Y;
			float fltZd = v3PrimLoc.Z - v3SecLoc.Z;
			return (float)Math.Sqrt( (fltXd*fltXd) + (fltYd*fltYd) + (fltZd*fltZd) ); 
		}

		public virtual float CalculateMuscleLength()
		{
			float fltLength = 0;

			if(m_aryAttachmentPoints.Count >= 2)
			{
				MuscleAttachment doAttach1 = m_aryAttachmentPoints[0], doAttach2;

				for(int iIndex=1; iIndex<m_aryAttachmentPoints.Count; iIndex++)
				{
					doAttach2 = m_aryAttachmentPoints[iIndex];
					fltLength += CalculateMuscleSegmentLength(doAttach1, doAttach2);
					doAttach1 = doAttach2;
				}
			}

			return fltLength * Util.Environment.DistanceUnitValue;
		}

		public override Vector3 FindPointOnSurface(Vector3 v3Start, Vector3 v3Direction)
		{
			return new Vector3(0,0,0);
		}

		protected override void BuildProperties()
		{
			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Name", m_strName.GetType(), "Name", 
				"Muscle Properties", "The name of this item.", m_strName));

			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Color", this.Color.GetType(), "Color",
				"Muscle Properties", "Sets the color used to draw the muscle.", this.Color));
//
//			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Alpha", typeof(int), "Alpha", 
//				"Muscle Properties", "Determines the transparency for this object. Valid values are between 0 and 255.", this.Alpha));
			
			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Description", m_strDescription.GetType(), "Description", 
				"Muscle Properties", "Sets the description for this body part.", m_strDescription, 
				typeof(AnimatTools.TypeHelpers.MultiLineStringTypeEditor)));

			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Enabled", m_bEnabled.GetType(), "Enabled",
				"Muscle Properties", "Determines whether the muscle will actually develop tension.", m_bEnabled));

			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Visible", m_bVisible.GetType(), "Visible",
				"Muscle Properties", "Determines whether the muscle will be visible in the simulation.", m_bVisible));

			Crownwood.Magic.Controls.PropertyBag pbSubBag = null;
			if(m_snMaxTension != null)
			{
				pbSubBag = m_snMaxTension.Properties;
				m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Maximum Tension", pbSubBag.GetType(), "MaxTension", 
					"Muscle Properties", "A param that determines the maximum tension this muscle can possible generate.", pbSubBag,
					"", typeof(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)));
			}

			if(m_snMuscleLength != null)
			{
				pbSubBag = m_snMuscleLength.Properties;
				m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Muscle Length", pbSubBag.GetType(), "MuscleLength", 
					"Muscle Properties", "The current length of the muscle between the two attachment points.", pbSubBag,
					"", typeof(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter), true));
			}

			if(m_aryAttachmentPoints != null)
			{
				pbSubBag = m_aryAttachmentPoints.Properties;
				m_Properties.Properties.Add(new PropertySpec("Muscle Attachments", pbSubBag.GetType(), "AttachmentPointsProps", 
					"Muscle Properties", "The list of muscle attachment points.", pbSubBag,
					typeof(VortexAnimatTools.TypeHelpers.MuscleAttachmentsTypeEditor),  typeof(Crownwood.Magic.Controls.ExpandablePropBagConverter)));
			}
		}

		public override void LoadData(ref AnimatTools.DataObjects.Simulation dsSim, ref AnimatTools.DataObjects.Physical.PhysicalStructure doStructure, ref AnimatTools.Interfaces.StdXml oXml)
		{
			m_aryChildBodies.Clear();

			oXml.IntoElem(); //'Into RigidBody Element

			m_strID = oXml.GetChildString("ID");
			m_strName = oXml.GetChildString("Name", m_strID);

            m_strDescription = oXml.GetChildString("Description", "");

			if(m_strName.Trim().Length == 0)
				m_strName = m_strID;

			if(oXml.FindChildElement("Color", false))
			{
				System.Drawing.Color oColor = Util.LoadColor(ref oXml, "Color");
				this.Alpha = oColor.A;
				this.Color = oColor;			
			}

			m_aryAttachmentPointIDs.Clear();
			if(oXml.FindChildElement("MuscleAttachments", false))
			{
				oXml.IntoElem();

				string strID;
				int iCount = oXml.NumberOfChildren();
				for(int iIndex=0; iIndex<iCount; iIndex++)
				{
					oXml.FindChildByIndex(iIndex);
					strID = oXml.GetChildString();
					m_aryAttachmentPointIDs.Add(strID);
				}

				oXml.OutOfElem();
			}

			m_bEnabled = oXml.GetChildBool("Enabled", m_bEnabled);
			m_bVisible = oXml.GetChildBool("IsVisible", m_bVisible);

			if(oXml.FindChildElement("MaximumTension", false))
				m_snMaxTension.LoadData(ref oXml, "MaximumTension");

			oXml.OutOfElem(); //Outof RigidBody Element			
		}

		public override void InitializeAfterLoad(ref AnimatTools.DataObjects.Simulation dsSim, ref AnimatTools.DataObjects.Physical.PhysicalStructure doStructure)
		{
			AnimatTools.DataObjects.Physical.PhysicalStructure doStruct = this.ParentStructure;
			VortexAnimatTools.DataObjects.Physical.RigidBodies.MuscleAttachment doAttach;
			m_aryAttachmentPoints.Clear();
			foreach(string strID in m_aryAttachmentPointIDs)
			{
				doAttach = (VortexAnimatTools.DataObjects.Physical.RigidBodies.MuscleAttachment) doStruct.FindBodyPart(strID, true);
				m_aryAttachmentPoints.Add(doAttach);
			}
		}

		public override void SaveData(ref AnimatTools.DataObjects.Simulation dsSim, ref AnimatTools.DataObjects.Physical.PhysicalStructure doStructure, ref AnimatTools.Interfaces.StdXml oXml)
		{
			oXml.AddChildElement(this.BodyPartType);

			oXml.IntoElem(); //Into RigidBody Element
 
			oXml.AddChildElement("Name", m_strName);
			oXml.AddChildElement("ID", m_strID);
			oXml.AddChildElement("Type", this.Type);
			oXml.AddChildElement("PartType", this.PartType.ToString());
			oXml.AddChildElement("ModuleName", this.ModuleName);

			Util.SaveColor(ref oXml, "Color", m_d3dMaterial.Diffuse);
            oXml.AddChildElement("Description", m_strDescription);

			oXml.AddChildElement("MuscleAttachments");
			oXml.IntoElem();  //Into MuscleAttachments

			AnimatTools.DataObjects.Physical.PhysicalStructure doStruct = this.ParentStructure;
			foreach(MuscleAttachment doAttach in m_aryAttachmentPoints)
			{
				//If it is a copy/cut in progress then it may be trying to save parts that are not on the main structure yet.
				if(doStruct.FindBodyPart(doAttach.ID, false) != null || Util.CopyInProgress || Util.CutInProgress)
					oXml.AddChildElement("AttachID", doAttach.ID);
			}

			oXml.OutOfElem();  //Outof MuscleAttachments

			oXml.AddChildElement("Enabled", m_bEnabled);
			oXml.AddChildElement("IsVisible", m_bVisible);
			m_snMaxTension.SaveData(ref oXml, "MaximumTension");

			oXml.OutOfElem(); //Outof RigidBody Element			
		}

		#endregion
	
	}
}
