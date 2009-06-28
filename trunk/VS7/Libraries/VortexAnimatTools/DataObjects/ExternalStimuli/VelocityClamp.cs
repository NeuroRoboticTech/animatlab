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

namespace VortexAnimatTools.DataObjects.ExternalStimuli
{
	/// <summary>
	/// Summary description for VelocityClamp.
	/// </summary>
	public class VelocityClamp : AnimatTools.DataObjects.ExternalStimuli.BodyPartStimulus
	{
		#region Attributes

		protected AnimatTools.TypeHelpers.LinkedBodyPart m_thPrimaryAttachment;
		protected AnimatTools.TypeHelpers.LinkedBodyPart m_thSecondaryAttachment;
		protected System.Type m_tpBodyPartType;
		protected AnimatTools.Framework.ScaledNumber m_snVelocity;
		protected AnimatTools.Framework.ScaledNumber m_snMaxForce;

		protected float m_fltProportionalGain = 1;
		protected float m_fltDerivativeGain = 0.1f;
		protected float m_fltIntegratorGain = 0;
		protected float m_fltMaxIntegratorValue = 0.1f;

		#endregion

		#region Properties

		public override String ImageName {get{return "VortexAnimatTools.Graphics.VelocityClamp_Large.gif";}}
		public override String TypeName {get{return "Velocity Clamp";}}
		public override String Description {get{return "This Allows you to apply feedback regulated force to maintain a joint velocity.";}}
		public override String StimulusClassType {get{return "VelocityClamp";}}
		public override string WorkspaceNodeAssemblyName {get{return "VortexAnimatTools";}}
		public override string WorkspaceNodeImageName {get{return "VortexAnimatTools.Graphics.PositionClamp.gif";}}

		#region DragObject Properties

		public override bool CanBeCharted {get	{return true;}}
		public override string DragImageName {get {return this.ImageName;}}
		public override string DataColumnClassType {get {return "StimulusData";}}
		public override string StructureID
		{
			get
			{
				if(m_doStructure != null)
					return m_doStructure.ID;
				else
					return "";
			}
		}

		#endregion

		public virtual AnimatTools.TypeHelpers.LinkedBodyPart PrimaryAttachment
		{
			get{return m_thPrimaryAttachment;}
			set 
			{
				if(value != null && value.BodyPart != null &&
					m_thSecondaryAttachment != null && m_thSecondaryAttachment.BodyPart != null 
					&& m_thSecondaryAttachment.BodyPart.ID == value.BodyPart.ID)
					throw new System.Exception("You can not set the primary muscle attachment to be the same as the secondary attachment.");

				m_thPrimaryAttachment = value;
			}
		}

		public virtual AnimatTools.TypeHelpers.LinkedBodyPart SecondaryAttachment
		{
			get{return m_thSecondaryAttachment;}
			set 
			{
				if(value != null && value.BodyPart != null &&
					m_thPrimaryAttachment != null && m_thPrimaryAttachment.BodyPart != null 
					&& m_thPrimaryAttachment.BodyPart.ID == value.BodyPart.ID)
					throw new System.Exception("You can not set the secondary muscle attachment to be the same as the primary attachment.");

				m_thSecondaryAttachment = value;
			}
		}

		public override AnimatTools.DataObjects.Physical.PhysicalStructure PhysicalStructure
		{
			get
			{
				return m_doStructure;
			}
			set
			{
				m_thPrimaryAttachment = new AnimatTools.TypeHelpers.LinkedBodyPartList(value, null, m_tpBodyPartType);
				m_thSecondaryAttachment = new AnimatTools.TypeHelpers.LinkedBodyPartList(value, null, m_tpBodyPartType);
				m_doStructure = value;
			}
		}

		public override AnimatTools.DataObjects.Physical.BodyPart BodyPart
		{
			get
			{
				return m_doBodyPart;
			}
			set
			{
				m_doBodyPart = value;
				SetPositionUnits();
			}
		}

		public virtual AnimatTools.Framework.ScaledNumber Velocity
		{
			get{return m_snVelocity;}
			set 
			{
				m_snVelocity.CopyData(ref value);
			}
		}

		public virtual AnimatTools.Framework.ScaledNumber MaxForce
		{
			get{return m_snMaxForce;}
			set 
			{
				m_snMaxForce.CopyData(ref value);
			}
		}

		public virtual float ProportionalGain
		{
			get{return m_fltProportionalGain;}
			set 
			{
				if(value <= 0)
					throw new System.Exception("The gain must be greater than 0.");

				m_fltProportionalGain = value;
			}
		}

		public virtual float DerivativeGain
		{
			get{return m_fltDerivativeGain;}
			set 
			{
				if(value < 0)
					throw new System.Exception("The gain must be greater than 0.");

				m_fltDerivativeGain = value;
			}
		}

		public virtual float IntegratorGain
		{
			get{return m_fltIntegratorGain;}
			set 
			{
				if(value < 0)
					throw new System.Exception("The gain must be greater than 0.");

				m_fltIntegratorGain = value;
			}
		}

		public virtual float MaxIntegratorValue
		{
			get{return m_fltMaxIntegratorValue;}
			set 
			{
				if(value < 0)
					throw new System.Exception("The gain must be greater than 0.");

				m_fltMaxIntegratorValue = value;
			}
		}

		#endregion

		#region Methods

		public VelocityClamp(AnimatTools.Framework.DataObject doParent) : base(doParent)
		{
			m_thPrimaryAttachment = new AnimatTools.TypeHelpers.LinkedBodyPartList(m_doStructure, null, m_tpBodyPartType);
			m_thSecondaryAttachment = new AnimatTools.TypeHelpers.LinkedBodyPartList(m_doStructure, null, m_tpBodyPartType);
			m_tpBodyPartType = typeof(VortexAnimatTools.DataObjects.Physical.RigidBodies.MuscleAttachment);
			m_snVelocity = new AnimatTools.Framework.ScaledNumber(this, "Velocity", 0, AnimatTools.Framework.ScaledNumber.enumNumericScale.None, "meters/second", "m/s");
			m_snMaxForce = new AnimatTools.Framework.ScaledNumber(this, "MaxForce", 1, AnimatTools.Framework.ScaledNumber.enumNumericScale.Kilo, "Newton", "N");

			m_thDataTypes.DataTypes.Add(new AnimatTools.DataObjects.DataType("Force", "Force", "Newtons", "N", 0, 1000));
			m_thDataTypes.DataTypes.Add(new AnimatTools.DataObjects.DataType("PError", "Proportional Error", "Meters", "m", 0, 1000));
			m_thDataTypes.DataTypes.Add(new AnimatTools.DataObjects.DataType("DError", "Derivative Error", "M/s", "m/s", 0, 1));
			m_thDataTypes.DataTypes.Add(new AnimatTools.DataObjects.DataType("IError", "Integrator Error", "m^2", "m^2", 0, 1000));
			m_thDataTypes.DataTypes.Add(new AnimatTools.DataObjects.DataType("Position", "Velocity", "m/s", "m/s", 0, 1000));
			m_thDataTypes.DataTypes.Add(new AnimatTools.DataObjects.DataType("Velocity", "Acceleration", "m/s^2", "m/s^2", 0, 1000));
			m_thDataTypes.ID = "Force";
		}

		protected override void BuildProperties()
		{
			base.BuildProperties();

			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Primary Attachment", typeof(AnimatTools.TypeHelpers.LinkedBodyPart), "PrimaryAttachment", 
																	"Clamp Properties", "The primary attachment.", 
																	m_thPrimaryAttachment, typeof(AnimatTools.TypeHelpers.DropDownListEditor), 
																	typeof(AnimatTools.TypeHelpers.LinkedBodyPartTypeConverter)));

			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Secondary Attachment", typeof(AnimatTools.TypeHelpers.LinkedBodyPart), "SecondaryAttachment", 
																	"Clamp Properties", "The secondary attachment.", 
																	m_thSecondaryAttachment, typeof(AnimatTools.TypeHelpers.DropDownListEditor), 
																	typeof(AnimatTools.TypeHelpers.LinkedBodyPartTypeConverter)));

			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Proportional Gain", m_fltProportionalGain.GetType(), "ProportionalGain",
																	"Clamp Properties", "The proportional feedback gain value.", m_fltProportionalGain));

			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Derivative Gain", m_fltDerivativeGain.GetType(), "DerivativeGain",
																	"Clamp Properties", "The derivative feedback gain value.", m_fltDerivativeGain));

			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Integrator Gain", m_fltIntegratorGain.GetType(), "IntegratorGain",
																	"Clamp Properties", "The integrator feedback gain value.", m_fltIntegratorGain));

			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Max Integrator Value", m_fltMaxIntegratorValue.GetType(), "MaxIntegratorValue",
																	"Clamp Properties", "The maximum value the intergrator can reach.", m_fltMaxIntegratorValue));

      Crownwood.Magic.Controls.PropertyBag pbNumberBag = m_snVelocity.Properties;
      m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Velocity", pbNumberBag.GetType(), "Velocity", 
                                  "Clamp Properties", "The velocity that should be maintained.", pbNumberBag, 
                                  "", typeof(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)));

			pbNumberBag = m_snMaxForce.Properties;
			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Max Force", pbNumberBag.GetType(), "MaxForce", 
																	"Clamp Properties", "The maximum force that can be applied by the clamp.", pbNumberBag, 
																	"", typeof(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)));
		}

		protected override void CloneInternal(AnimatTools.Framework.DataObject doOriginal, bool bCutData, AnimatTools.Framework.DataObject doRoot)
		{
			base.CloneInternal (doOriginal, bCutData, doRoot);

			VelocityClamp doOrigBody = (VelocityClamp) doOriginal;

			m_thPrimaryAttachment = (AnimatTools.TypeHelpers.LinkedBodyPart) doOrigBody.m_thPrimaryAttachment.Clone(this, bCutData, doRoot);
			m_thSecondaryAttachment = (AnimatTools.TypeHelpers.LinkedBodyPart) doOrigBody.m_thSecondaryAttachment.Clone(this, bCutData, doRoot);
			m_snVelocity = (AnimatTools.Framework.ScaledNumber) doOrigBody.m_snVelocity.Clone(this, bCutData, doRoot);
			m_snMaxForce = (AnimatTools.Framework.ScaledNumber) doOrigBody.m_snMaxForce.Clone(this, bCutData, doRoot);
			m_fltProportionalGain = doOrigBody.m_fltProportionalGain;
			m_fltDerivativeGain = doOrigBody.m_fltDerivativeGain;
			m_fltIntegratorGain = doOrigBody.m_fltIntegratorGain;
			m_fltMaxIntegratorValue = doOrigBody.m_fltMaxIntegratorValue;
		}

		public override AnimatTools.Framework.DataObject Clone(AnimatTools.Framework.DataObject doParent, bool bCutData, AnimatTools.Framework.DataObject doRoot)
		{
			VelocityClamp bnPart = new VelocityClamp(doParent);
			bnPart.CloneInternal(this, bCutData, doRoot);
			if(doRoot != null && doRoot == this) bnPart.AfterClone(this, bCutData, doRoot, bnPart);
			return bnPart;
		}

		protected void SetPositionUnits()
		{
			double dblActualValue = m_snVelocity.ActualValue;
			AnimatTools.Framework.ScaledNumber.enumNumericScale eScale =  m_snVelocity.Scale;

			if(m_doBodyPart != null && m_doBodyPart is AnimatTools.DataObjects.Physical.Joint)
			{	
				AnimatTools.DataObjects.Physical.Joint doJoint = (AnimatTools.DataObjects.Physical.Joint) m_doBodyPart;

				if(doJoint.UsesRadians) 
					m_snVelocity = new AnimatTools.Framework.ScaledNumber(this, "Velocity", 0, AnimatTools.Framework.ScaledNumber.enumNumericScale.None, "radians/sec", "rad/s");
				else
					m_snVelocity = new AnimatTools.Framework.ScaledNumber(this, "Velocity", 0, AnimatTools.Framework.ScaledNumber.enumNumericScale.None, "meters/sec", "m/s");
			}
			else
				m_snVelocity = new AnimatTools.Framework.ScaledNumber(this, "Velocity", 0, AnimatTools.Framework.ScaledNumber.enumNumericScale.None, "meters/sec", "m/s");

			m_snVelocity.SetFromValue(dblActualValue, eScale);
		}

		#region DragObject Methods

		public override void SaveDataColumnToXml(ref AnimatTools.Interfaces.StdXml oXml)
		{
			oXml.IntoElem();
			oXml.AddChildElement("StimulusID", this.ID);
			oXml.OutOfElem();
		}

		#endregion

		#region Loading and Saving Data

		public override string SaveStimulusToXml()
		{
			if(m_doStructure == null)
				throw new System.Exception("No structure was defined for the stimulus '" + m_strName + "'.");

			if(m_doBodyPart == null)
				throw new System.Exception("No bodypart was defined for the stimulus '" + m_strName + "'.");

			if(m_thPrimaryAttachment != null && m_thPrimaryAttachment.BodyPart != null && 
				m_thSecondaryAttachment != null && m_thSecondaryAttachment.BodyPart != null)
			{
				AnimatTools.Interfaces.StdXml oXml = new AnimatTools.Interfaces.StdXml(); 

				oXml.AddElement("Stimuli");
				SaveXml(ref oXml);

				return oXml.Serialize();
			}
			else
				return "";
		}

		public override void SaveXml(ref AnimatTools.Interfaces.StdXml oXml)
		{
			if(m_doStructure == null)
				throw new System.Exception("No structure was defined for the stimulus '" + m_strName + "'.");

			if(m_doBodyPart == null)
				throw new System.Exception("No bodypart was defined for the stimulus '" + m_strName + "'.");

			if(m_thPrimaryAttachment != null && m_thPrimaryAttachment.BodyPart != null && 
				m_thSecondaryAttachment != null && m_thSecondaryAttachment.BodyPart != null)
			{
				oXml.AddChildElement("Stimulus");

				oXml.IntoElem();
				oXml.AddChildElement("ID", m_strID);
				oXml.AddChildElement("Name", m_strName);
				oXml.AddChildElement("AlwaysActive", m_bAlwaysActive);

				oXml.AddChildElement("ModuleName", this.StimulusModuleName);
				oXml.AddChildElement("Type", this.StimulusClassType);

				oXml.AddChildElement("StartTime", m_snStartTime.ActualValue);
				oXml.AddChildElement("EndTime", m_snEndTime.ActualValue);

				oXml.AddChildElement("StructureID", m_doStructure.ID);
				oXml.AddChildElement("BodyID", m_doBodyPart.ID);

				oXml.AddChildElement("PrimaryAttachID", m_thPrimaryAttachment.BodyPart.ID);
				oXml.AddChildElement("SecondaryAttachID", m_thSecondaryAttachment.BodyPart.ID);
				oXml.AddChildElement("UsePosition", false);
				oXml.AddChildElement("Velocity", m_snVelocity.ActualValue);
				oXml.AddChildElement("MaxForce", m_snMaxForce.ActualValue);
				oXml.AddChildElement("PGain", m_fltProportionalGain);
				oXml.AddChildElement("DGain", m_fltDerivativeGain);
				oXml.AddChildElement("IGain", m_fltIntegratorGain);
				oXml.AddChildElement("MaxIVal", m_fltMaxIntegratorValue);

				oXml.OutOfElem();
			}
		}

		public override void LoadData(ref AnimatTools.Interfaces.StdXml oXml)
		{
			base.LoadData(ref oXml);

			oXml.IntoElem();


			if(oXml.FindChildElement("PrimaryAttachmentID", false))
			{
			  string strAttachID = oXml.GetChildString("PrimaryAttachmentID");
				if(m_doStructure != null && strAttachID.Trim().Length > 0)
				{
					AnimatTools.DataObjects.Physical.BodyPart rbPart = (AnimatTools.DataObjects.Physical.BodyPart) m_doStructure.FindBodyPart(strAttachID, false);
					if(rbPart != null)
						m_thPrimaryAttachment = new AnimatTools.TypeHelpers.LinkedBodyPartList(m_doStructure, rbPart, m_tpBodyPartType);
				}
			}
			else
				m_thPrimaryAttachment = new AnimatTools.TypeHelpers.LinkedBodyPartList(m_doStructure, null, m_tpBodyPartType);

			if(oXml.FindChildElement("SecondaryAttachmentID", false))
			{
				string strAttachID = oXml.GetChildString("SecondaryAttachmentID");
				if(m_doStructure != null && strAttachID.Trim().Length > 0)
				{
					AnimatTools.DataObjects.Physical.BodyPart rbPart = (AnimatTools.DataObjects.Physical.BodyPart) m_doStructure.FindBodyPart(strAttachID, false);
					if(rbPart != null)
						m_thSecondaryAttachment = new AnimatTools.TypeHelpers.LinkedBodyPartList(m_doStructure, rbPart, m_tpBodyPartType);
				}
			}
			else
				m_thSecondaryAttachment = new AnimatTools.TypeHelpers.LinkedBodyPartList(m_doStructure, null, m_tpBodyPartType);
			
			m_fltProportionalGain = oXml.GetChildFloat("PGain", m_fltProportionalGain);
			m_fltDerivativeGain = oXml.GetChildFloat("DGain", m_fltDerivativeGain);
			m_fltIntegratorGain = oXml.GetChildFloat("IGain", m_fltIntegratorGain);
			m_fltMaxIntegratorValue = oXml.GetChildFloat("MaxIValue", m_fltMaxIntegratorValue);
			m_snVelocity.LoadData(ref oXml, "Velocity");
			m_snMaxForce.LoadData(ref oXml, "MaxForce");

			SetPositionUnits();
			oXml.OutOfElem();
		}

		public override void SaveData(ref AnimatTools.Interfaces.StdXml oXml)
		{
			base.SaveData(ref oXml);

			oXml.IntoElem();

			if(m_thPrimaryAttachment != null && m_thPrimaryAttachment.BodyPart != null)
				oXml.AddChildElement("PrimaryAttachmentID", m_thPrimaryAttachment.BodyPart.ID);

			if(m_thSecondaryAttachment != null && m_thSecondaryAttachment.BodyPart != null)
				oXml.AddChildElement("SecondaryAttachmentID", m_thSecondaryAttachment.BodyPart.ID);

			oXml.AddChildElement("PGain", m_fltProportionalGain);
			oXml.AddChildElement("DGain", m_fltDerivativeGain);
			oXml.AddChildElement("IGain", m_fltIntegratorGain);
			oXml.AddChildElement("MaxIValue", m_fltMaxIntegratorValue);
			m_snVelocity.SaveData(ref oXml, "Velocity");
			m_snMaxForce.SaveData(ref oXml, "MaxForce");

			oXml.OutOfElem();
		}

		#endregion

		#endregion
	}
}
