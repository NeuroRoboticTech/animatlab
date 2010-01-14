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
	/// Summary description for Muscle.
	/// </summary>
	public class MultiSegmentSpring : VortexAnimatTools.DataObjects.Physical.RigidBodies.MuscleBase
	{
		#region Attributes

		public enum enumForceTypes
		{
			ExtensionOnly,
			CompressionOnly,
			Both
		};

		protected AnimatTools.Framework.ScaledNumber m_snNaturalLength;
		protected AnimatTools.Framework.ScaledNumber m_snStiffness;
		protected AnimatTools.Framework.ScaledNumber m_snDamping;
		protected VortexAnimatTools.DataObjects.Physical.RigidBodies.MultiSegmentSpring.enumForceTypes m_eForceType = enumForceTypes.Both;

		#endregion

		#region Properties

		public override String ImageName {get{return "VortexAnimatTools.Graphics.Spring_Treeview.gif";}}
		public override String ButtonImageName {get{return "VortexAnimatTools.Graphics.MultisegmentSpring_Button.gif";}}
		public override String Type {get{return "MultiSegmentSpring";}}
		public override String BodyPartName {get{return "MultiSegment Spring";}}
		public override System.Type PartType {get{return typeof(VortexAnimatTools.DataObjects.Physical.RigidBodies.MultiSegmentSpring);}}

		public virtual AnimatTools.Framework.ScaledNumber NaturalLength
		{
			get{return m_snNaturalLength;}
			set 
			{
				if(value != null)
				{
					if(value.ActualValue <= 0)
						throw new System.Exception("The natrual length must be greater than zero.");

					m_snNaturalLength.CopyData(ref value);
				}
			}
		}

		public virtual AnimatTools.Framework.ScaledNumber Stiffness
		{
			get{return m_snStiffness;}
			set 
			{
				if(value != null)
				{
					if(value.ActualValue <= 0)
						throw new System.Exception("The stiffness must be greater than zero.");

					m_snStiffness.CopyData(ref value);
				}
			}
		}
 
		public virtual AnimatTools.Framework.ScaledNumber Damping
		{
			get{return m_snDamping;}
			set 
			{
				if(value != null)
				{
					if(value.ActualValue < 0)
						throw new System.Exception("The damping must be greater than or equal to zero.");

					m_snDamping.CopyData(ref value);
				}
			}
		}

		public virtual VortexAnimatTools.DataObjects.Physical.RigidBodies.MultiSegmentSpring.enumForceTypes ForceType
		{
			get {return m_eForceType;}
			set {m_eForceType = value;}
		}

		#endregion

		#region Methods

		public MultiSegmentSpring(AnimatTools.Framework.DataObject doParent) : base(doParent)
		{
			m_thDataTypes.DataTypes.Clear();

			m_thDataTypes.DataTypes.Add(new AnimatTools.DataObjects.DataType("Tension", "Tension", "Newtons", "N", 0, 1000));
			m_thDataTypes.DataTypes.Add(new AnimatTools.DataObjects.DataType("SpringTension", "Spring Tension", "Newtons", "N", 0, 1000));
			m_thDataTypes.DataTypes.Add(new AnimatTools.DataObjects.DataType("DampingTension", "Damping Tension", "Newtons", "N", 0, 1000));
			m_thDataTypes.DataTypes.Add(new AnimatTools.DataObjects.DataType("SpringLength", "Spring Length", "Meters", "m", 0, 1));
			m_thDataTypes.DataTypes.Add(new AnimatTools.DataObjects.DataType("Vspring", "Spring Velocity", "m/s", "m/s", -3, 3));
			m_thDataTypes.DataTypes.Add(new AnimatTools.DataObjects.DataType("Displacement", "Muscle Displacement", "Meters", "m", 0, 1));
			m_thDataTypes.DataTypes.Add(new AnimatTools.DataObjects.DataType("DisplacementRatio", "Muscle Displacement Ratio", "", "", -1, 1));
			m_thDataTypes.DataTypes.Add(new AnimatTools.DataObjects.DataType("Enable", "Enable", "", "", 0, 1));
			m_thDataTypes.DataTypes.Add(new AnimatTools.DataObjects.DataType("Aspring", "Acceleration", "m/s^2", "m/s^2", -100, 100));
			m_thDataTypes.DataTypes.Add(new AnimatTools.DataObjects.DataType("PotEnergy", "Potential Energy", "J", "J", -10, 10));
			m_thDataTypes.ID = "Tension";

			//Holdover stuff from using the muscle base class
			m_StimTension = new AnimatTools.DataObjects.Gains.MuscleGains.StimulusTension(this);
			m_LengthTension = new AnimatTools.DataObjects.Gains.MuscleGains.LengthTension(this);
			m_LengthTension.ShowLcePercentage = false;

			m_snNaturalLength = new AnimatTools.Framework.ScaledNumber(this, "NaturalLength", 1, AnimatTools.Framework.ScaledNumber.enumNumericScale.None, "meters", "m");
			m_snStiffness = new AnimatTools.Framework.ScaledNumber(this, "Stiffness", 50, AnimatTools.Framework.ScaledNumber.enumNumericScale.Kilo, "N/m", "N/m");
			m_snDamping = new AnimatTools.Framework.ScaledNumber(this, "Damping", 1, AnimatTools.Framework.ScaledNumber.enumNumericScale.Kilo, "g/s", "g/s");

			m_thIncomingDataType = new AnimatTools.DataObjects.DataType("Enabled", "Enabled", "", "", 0, 1, ScaledNumber.enumNumericScale.None, ScaledNumber.enumNumericScale.None);
		}

		protected override void BuildProperties()
		{
			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Name", m_strName.GetType(), "Name", 
				"Spring Properties", "The name of this item.", m_strName));

			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("ID", this.ID.GetType(), "ID", 
				"Spring Properties", "ID", this.ID, true));

			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Color", this.Color.GetType(), "Color",
				"Spring Properties", "Sets the color used to draw the spring.", this.Color));
//
//			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Alpha", typeof(int), "Alpha", 
//				"Spring Properties", "Determines the transparency for this object. Valid values are between 0 and 255.", this.Alpha));

			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Enabled", m_bEnabled.GetType(), "Enabled",
				"Spring Properties", "Determines whether the spring will actually develop tension.", m_bEnabled));

			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Visible", m_bVisible.GetType(), "Visible",
				"Spring Properties", "Determines whether the spring will be visible in the simulation.", m_bVisible));

			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Force Type", m_eForceType.GetType(), "ForceType",
				"Spring Properties", "Determines the type of forces this spring generates. If it is both then it acts like a standard spring, but if it is extension only then it is more like what you would get from a wire or rope.", m_eForceType));

			Crownwood.Magic.Controls.PropertyBag pbSubBag = m_snMaxTension.Properties;
			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Maximum Tension", pbSubBag.GetType(), "MaxTension", 
				"Spring Properties", "A param that determines the maximum tension this spring can possible generate.", pbSubBag,
				"", typeof(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)));

			Crownwood.Magic.Controls.PropertyBag pbNumberBag  = this.NaturalLength.Properties;
			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Natural Length", pbNumberBag.GetType(), "NaturalLength", 
				"Spring Properties", "Sets the natrual length of this spring.", pbNumberBag,
				"", typeof(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)));

			pbSubBag = m_snMuscleLength.Properties;
			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Spring Length", pbSubBag.GetType(), "MuscleLength", 
				"Spring Properties", "The current length of the spring between the two attachment points.", pbSubBag,
				"", typeof(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter), true));

			pbSubBag = m_aryAttachmentPoints.Properties;
			m_Properties.Properties.Add(new PropertySpec("Spring Attachments", pbSubBag.GetType(), "AttachmentPointsProps", 
				"Spring Properties", "The list of spring attachment points.", pbSubBag,
				typeof(VortexAnimatTools.TypeHelpers.MuscleAttachmentsTypeEditor),  typeof(Crownwood.Magic.Controls.ExpandablePropBagConverter)));

			pbSubBag = m_snStiffness.Properties;
			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Stiffness", pbSubBag.GetType(), "Stiffness", 
																	"Spring Properties", "Determines the stiffness of the spring.", pbSubBag,
																	"", typeof(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)));

			pbSubBag = m_snDamping.Properties;
			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Damping", pbSubBag.GetType(), "Damping", 
																	"Spring Properties", "Determines the damping of the spring.", pbSubBag,
																	"", typeof(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)));
		}

		public override AnimatTools.DataObjects.Physical.BodyPart CreateNewBodyPart(AnimatTools.Framework.DataObject doParent)
		{return new MultiSegmentSpring(doParent);}

		protected override void CloneInternal(AnimatTools.Framework.DataObject doOriginal, bool bCutData, AnimatTools.Framework.DataObject doRoot)
		{
			base.CloneInternal (doOriginal, bCutData, doRoot);

			MultiSegmentSpring doOrigBody = (MultiSegmentSpring) doOriginal;

			m_aryAttachmentPoints = (VortexAnimatTools.Collections.MuscleAttachments) doOrigBody.m_aryAttachmentPoints.Copy();
			m_bEnabled = doOrigBody.m_bEnabled;
			m_StimTension = (AnimatTools.DataObjects.Gains.MuscleGains.StimulusTension) doOrigBody.m_StimTension.Clone(this, bCutData, doRoot);
			m_LengthTension = (AnimatTools.DataObjects.Gains.MuscleGains.LengthTension) doOrigBody.m_LengthTension.Clone(this, bCutData, doRoot);
			m_snMaxTension = (AnimatTools.Framework.ScaledNumber) doOrigBody.m_snMaxTension.Clone(this, bCutData, doRoot);
			m_snMuscleLength = (AnimatTools.Framework.ScaledNumber) doOrigBody.m_snMuscleLength.Clone(this, bCutData, doRoot);
			m_snNaturalLength = (AnimatTools.Framework.ScaledNumber) doOrigBody.m_snNaturalLength.Clone(this, bCutData, doRoot);
			m_snStiffness = (AnimatTools.Framework.ScaledNumber) doOrigBody.m_snStiffness.Clone(this, bCutData, doRoot);
			m_snDamping = (AnimatTools.Framework.ScaledNumber) doOrigBody.m_snDamping.Clone(this, bCutData, doRoot);
			m_eForceType = doOrigBody.m_eForceType;
		}

		public override AnimatTools.Framework.DataObject Clone(AnimatTools.Framework.DataObject doParent, bool bCutData, AnimatTools.Framework.DataObject doRoot)
		{
			MultiSegmentSpring bnPart = new MultiSegmentSpring(doParent);
			bnPart.CloneInternal(this, bCutData, doRoot);
			if(doRoot != null && doRoot == this) bnPart.AfterClone(this, bCutData, doRoot, bnPart);
			return bnPart;
		}
		
		#region Loading and Saving Data

		public override void LoadData(ref AnimatTools.DataObjects.Simulation dsSim, ref AnimatTools.DataObjects.Physical.PhysicalStructure doStructure, ref AnimatTools.Interfaces.StdXml oXml)
		{
			base.LoadData(ref dsSim, ref doStructure, ref oXml);

			oXml.IntoElem(); //'Into RigidBody Element

			if(oXml.FindChildElement("NaturalLength", false))
				m_snNaturalLength.LoadData(ref oXml, "NaturalLength");

			m_snStiffness.LoadData(ref oXml, "Stiffness");
			m_snDamping.LoadData(ref oXml, "Damping");

			//This code converts old versions of the spring code into
			//the new version that has multiple attachment points
			if(oXml.FindChildElement("PrimaryAttachmentID", false))
			{
				m_aryAttachmentPointIDs.Clear();

				string strAttachID;
				if(oXml.FindChildElement("PrimaryAttachmentID", false))
				{
					strAttachID = oXml.GetChildString("PrimaryAttachmentID");
					m_aryAttachmentPointIDs.Add(strAttachID);
				}

				if(oXml.FindChildElement("SecondaryAttachmentID", false))
				{
					strAttachID = oXml.GetChildString("SecondaryAttachmentID");
					m_aryAttachmentPointIDs.Add(strAttachID);
				}
			}

			if(oXml.FindChildElement("ForceType", false))
				m_eForceType = (VortexAnimatTools.DataObjects.Physical.RigidBodies.MultiSegmentSpring.enumForceTypes) Enum.Parse(typeof(VortexAnimatTools.DataObjects.Physical.RigidBodies.MultiSegmentSpring.enumForceTypes), oXml.GetChildString("ForceType"), true);

			oXml.OutOfElem(); //Outof RigidBody Element			
		}

		public override void SaveData(ref AnimatTools.DataObjects.Simulation dsSim, ref AnimatTools.DataObjects.Physical.PhysicalStructure doStructure, ref AnimatTools.Interfaces.StdXml oXml)
		{
			base.SaveData(ref dsSim, ref doStructure, ref oXml);

			oXml.IntoElem(); //Into RigidBody Element
 
			m_snNaturalLength.SaveData(ref oXml, "NaturalLength");
			m_snStiffness.SaveData(ref oXml, "Stiffness");
			m_snDamping.SaveData(ref oXml, "Damping");
			oXml.AddChildElement("ForceType", m_eForceType.ToString());

			oXml.OutOfElem(); //Outof RigidBody Element			
		}

		#endregion

		#endregion
	}
}
