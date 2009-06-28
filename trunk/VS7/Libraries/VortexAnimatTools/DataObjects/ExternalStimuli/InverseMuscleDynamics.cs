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
	/// Summary description for PositionClamp.
	/// </summary>
	public class InverseMuscleDynamics : AnimatTools.DataObjects.ExternalStimuli.NodeStimulus
	{
		#region Attributes
 
		protected AnimatTools.TypeHelpers.LinkedBodyPart m_thMuscle;
		protected System.Type m_tpBodyPartType;
		protected string m_strMuscleLengthData;
		protected AnimatTools.Framework.ScaledNumber m_snRestPotential;
		protected AnimatTools.Framework.ScaledNumber m_snConductance;

		#endregion

		#region Properties

		public override String ImageName {get{return "VortexAnimatTools.Graphics.InverseDynamics_Large.gif";}}
		public override String TypeName {get{return "Inverse Muscle Dynamics Current";}}
		public override String Description {get{return "This generates a stimulus current using the inverse dynamics of a muscle and a set predicted muscle length data. This is mainly for use in generating the gamma signal to drive a stretch receptor.";}}
		public override String StimulusModuleName {get{return "";}}
		public override String StimulusClassType {get{return "InverseMuscleCurrent";}}
		public override string WorkspaceNodeAssemblyName {get{return "VortexAnimatTools";}}
		public override string WorkspaceNodeImageName {get{return "VortexAnimatTools.Graphics.InverseDynamics.gif";}}

		public override AnimatTools.DataObjects.Physical.Organism Organism
		{
			get
			{
				return base.Organism;
			}
			set
			{
				base.Organism = value;
				m_thMuscle = new AnimatTools.TypeHelpers.LinkedBodyPartList(value, null, m_tpBodyPartType);
			}
		}

		public virtual AnimatTools.TypeHelpers.LinkedBodyPart Muscle
		{
			get{return m_thMuscle;}
			set {m_thMuscle = value;}
		}

		public virtual string MuscleLengthData
		{
			get{return m_strMuscleLengthData;}
			set 
			{
				if(value != null)
				{
					string strPath="", strFile="";
					if(Util.DetermineFilePath(value, ref strPath, ref strFile))
						value = strFile;
				}

				m_strMuscleLengthData = value;
			}
		}

		public virtual AnimatTools.Framework.ScaledNumber RestPotential
		{
			get{return m_snRestPotential;}
			set 
			{
				m_snRestPotential.CopyData(ref value);
			}
		}

		public virtual AnimatTools.Framework.ScaledNumber Conductance
		{
			get{return m_snConductance;}
			set 
			{
				m_snConductance.CopyData(ref value);
			}
		}


		#region DragObject Properties

		public override bool CanBeCharted {get	{return true;}}
		public override string DragImageName {get {return this.ImageName;}}
		public override string DataColumnClassType {get {return "StimulusData";}}
		public override string StructureID
		{
			get
			{
				if(m_doOrganism != null)
					return m_doOrganism.ID;
				else
					return "";
			}
		}

		#endregion

		#endregion

		#region Methods

		public InverseMuscleDynamics(AnimatTools.Framework.DataObject doParent) : base(doParent)
		{
			m_tpBodyPartType = typeof(VortexAnimatTools.DataObjects.Physical.RigidBodies.MuscleBase);
			m_thMuscle = new AnimatTools.TypeHelpers.LinkedBodyPartList(null, null, m_tpBodyPartType);
			m_strMuscleLengthData = "";
			m_snConductance = new AnimatTools.Framework.ScaledNumber(this, "Conductance", 100, AnimatTools.Framework.ScaledNumber.enumNumericScale.nano, "Siemens", "S");
			m_snRestPotential = new AnimatTools.Framework.ScaledNumber(this, "RestPotential", -100, AnimatTools.Framework.ScaledNumber.enumNumericScale.milli, "Volts", "V");

			m_thDataTypes.DataTypes.Add(new AnimatTools.DataObjects.DataType("A", "A", "Newtons", "N", 0, 1000));
			m_thDataTypes.DataTypes.Add(new AnimatTools.DataObjects.DataType("Vm", "Vm", "Volts", "V", -0.100, 0.100));
			m_thDataTypes.DataTypes.Add(new AnimatTools.DataObjects.DataType("Current", "Current", "Amps", "A", -100e-9, 100e-9));
			m_thDataTypes.ID = "A";
		}

		protected override void BuildProperties()
		{
			base.BuildProperties();

			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Muscle", typeof(AnimatTools.TypeHelpers.LinkedBodyPart), "Muscle", 
																	"Stimulus Properties", "The muscle that will be used to generate the inverse dynamics.", 
																	m_thMuscle, typeof(AnimatTools.TypeHelpers.DropDownListEditor), 
																	typeof(AnimatTools.TypeHelpers.LinkedBodyPartTypeConverter)));

			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Muscle Length Data", m_strMuscleLengthData.GetType(), "MuscleLengthData", 
										"Stimulus Properties", "Specifies the data file that has the muscle length prediction data used in the inverse dynamics calculations.", 
										m_strMuscleLengthData, typeof(System.Windows.Forms.Design.FileNameEditor)));

			Crownwood.Magic.Controls.PropertyBag pbNumberBag = m_snConductance.Properties;
			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Conductance", pbNumberBag.GetType(), "Conductance", 
										"Stimulus Properties", "The conductance that will be used to calculate the current to inject using the stimulus voltage.", pbNumberBag, 
										"", typeof(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)));


			pbNumberBag = m_snRestPotential.Properties;
			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Base Voltage", pbNumberBag.GetType(), "RestPotential", 
									"Stimulus Properties", "The resting potential of the neuron.", pbNumberBag, 
									"", typeof(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)));

			m_Properties.Properties.Remove("Start Time");
			m_Properties.Properties.Remove("End Time");
			m_Properties.Properties.Remove("Always Active");
		}

		protected override void CloneInternal(AnimatTools.Framework.DataObject doOriginal, bool bCutData, AnimatTools.Framework.DataObject doRoot)
		{
			base.CloneInternal (doOriginal, bCutData, doRoot);

			InverseMuscleDynamics doOrigBody = (InverseMuscleDynamics) doOriginal;

			m_thMuscle = (AnimatTools.TypeHelpers.LinkedBodyPart) doOrigBody.m_thMuscle.Clone(this, bCutData, doRoot);
			m_strMuscleLengthData = doOrigBody.m_strMuscleLengthData;
			m_snConductance = (AnimatTools.Framework.ScaledNumber) doOrigBody.m_snConductance.Clone(this, bCutData, doRoot);
			m_snRestPotential = (AnimatTools.Framework.ScaledNumber) doOrigBody.m_snRestPotential.Clone(this, bCutData, doRoot);
		}

		public override AnimatTools.Framework.DataObject Clone(AnimatTools.Framework.DataObject doParent, bool bCutData, AnimatTools.Framework.DataObject doRoot)
		{
			InverseMuscleDynamics bnPart = new InverseMuscleDynamics(doParent);
			bnPart.CloneInternal(this, bCutData, doRoot);
			if(doRoot != null && doRoot == this) bnPart.AfterClone(this, bCutData, doRoot, bnPart);
			return bnPart;
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
			if(m_doOrganism == null)
				throw new System.Exception("No organism was defined for the stimulus '" + m_strName + "'.");

			if(m_doNode == null)
				throw new System.Exception("No node was defined for the stimulus '" + m_strName + "'.");

			if(m_doNode != null && m_thMuscle != null && m_thMuscle.BodyPart != null && m_strMuscleLengthData != null && m_strMuscleLengthData.Length > 0)
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
			if(m_doOrganism == null)
				throw new System.Exception("No organism was defined for the stimulus '" + m_strName + "'.");

			if(m_doNode == null)
				throw new System.Exception("No node was defined for the stimulus '" + m_strName + "'.");
			
			//I have no idea why, but unders some conditions the object link we have to the node does not point to the 
			//actual node. We need to re-get the link to make sure that the node index is actually valid.
			m_doNode = m_doOrganism.FindBehavioralNode(m_doNode.ID, true);

			if(m_doNode != null && m_thMuscle != null && m_thMuscle.BodyPart != null && m_strMuscleLengthData != null && m_strMuscleLengthData.Length > 0)
			{
				oXml.AddChildElement("Stimulus");

				oXml.IntoElem();
				oXml.AddChildElement("ID", m_strID);
				oXml.AddChildElement("Name", m_strName);

				//The module name needs to be blank so it is created from the physics class factory.
				//But we still need to know which neural module to load the neuron from.
				oXml.AddChildElement("ModuleName", this.StimulusModuleName);
				oXml.AddChildElement("NeuralModuleName", m_doNode.DataColumnModuleName);
				oXml.AddChildElement("Type", this.StimulusClassType);

                oXml.AddChildElement("OrganismID", m_doOrganism.ID);
                oXml.AddChildElement("TargetNodeID", m_doNode.NodeIndex);

				oXml.AddChildElement("MuscleID", m_thMuscle.BodyPart.ID);
				oXml.AddChildElement("LengthData", m_strMuscleLengthData);
				oXml.AddChildElement("Conductance", m_snConductance.ActualValue);
				oXml.AddChildElement("RestPotential", m_snRestPotential.ActualValue);

				oXml.OutOfElem();
			}
		}

		public override void LoadData(ref AnimatTools.Interfaces.StdXml oXml)
		{
			base.LoadData(ref oXml);

		     oXml.IntoElem();

			if(oXml.FindChildElement("MuscleID", false))
			{
			  string strMuscleID = oXml.GetChildString("MuscleID");
		      if(m_doOrganism != null && strMuscleID.Trim().Length > 0)
				{
					AnimatTools.DataObjects.Physical.BodyPart rbPart = (AnimatTools.DataObjects.Physical.BodyPart) m_doOrganism.FindBodyPart(strMuscleID, false);
					if(rbPart != null)
						m_thMuscle = new AnimatTools.TypeHelpers.LinkedBodyPartList(m_doOrganism, rbPart, m_tpBodyPartType);
				}
			}
			else
				m_thMuscle = new AnimatTools.TypeHelpers.LinkedBodyPartList(m_doOrganism, null, m_tpBodyPartType);
			
			m_strMuscleLengthData = oXml.GetChildString("MuscleLengthData", m_strMuscleLengthData);

			if(oXml.FindChildElement("Conductance", false))
				m_snConductance.LoadData(ref oXml, "Conductance");

			if(oXml.FindChildElement("RestPotential", false))
				m_snRestPotential.LoadData(ref oXml, "RestPotential");

			oXml.OutOfElem();
		}

		public override void SaveData(ref AnimatTools.Interfaces.StdXml oXml)
		{
			base.SaveData(ref oXml);

			oXml.IntoElem();

			if(m_thMuscle != null && m_thMuscle.BodyPart != null)
				oXml.AddChildElement("MuscleID", m_thMuscle.BodyPart.ID);

			oXml.AddChildElement("MuscleLengthData", m_strMuscleLengthData);
			m_snConductance.SaveData(ref oXml, "Conductance");
			m_snRestPotential.SaveData(ref oXml, "RestPotential");

			oXml.OutOfElem();
		}

		#endregion

		#endregion
	}
}
