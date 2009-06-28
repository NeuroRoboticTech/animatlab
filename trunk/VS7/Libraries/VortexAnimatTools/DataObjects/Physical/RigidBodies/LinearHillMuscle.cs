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
	public class LinearHillMuscle : VortexAnimatTools.DataObjects.Physical.RigidBodies.MuscleBase
	{
		#region Attributes

		protected AnimatTools.Framework.ScaledNumber m_snKse;
		protected AnimatTools.Framework.ScaledNumber m_snKpe;
		protected AnimatTools.Framework.ScaledNumber m_snB;

		protected AnimatTools.Framework.ScaledNumber m_snPeLengthPercentage;
		protected AnimatTools.Framework.ScaledNumber m_snMinPeLengthPercentage;

		protected AnimatTools.Framework.ScaledNumber m_snIbDischargeConstant;

		#endregion

		#region Properties

		public override String Type {get{return "LinearHillMuscle";}}
		public override String BodyPartName {get{return "Linear Hill Muscle";}}
		public override System.Type PartType {get{return typeof(VortexAnimatTools.DataObjects.Physical.RigidBodies.LinearHillMuscle);}}

		public virtual AnimatTools.Framework.ScaledNumber Kse
		{
			get{return m_snKse;}
			set 
			{
				if(value != null)
				{
					if(value.ActualValue <= 0)
						throw new System.Exception("The stiffness of the SE spring must be greater than zero.");

					m_snKse.CopyData(ref value);
				}
			}
		}

		public virtual AnimatTools.Framework.ScaledNumber Kpe
		{
			get{return m_snKpe;}
			set 
			{
				if(value != null)
				{
					if(value.ActualValue < 0)
						throw new System.Exception("The stiffness of the PE spring must be greater than or equal to zero.");

					m_snKpe.CopyData(ref value);
				}
			}
		}

		public virtual AnimatTools.Framework.ScaledNumber B
		{
			get{return m_snB;}
			set 
			{
				if(value != null)
				{
					if(value.ActualValue < 0)
						throw new System.Exception("The damping must be greater than zero.");

					m_snB.CopyData(ref value);
				}
			}
		}

		public virtual AnimatTools.Framework.ScaledNumber PeLengthPercentage
		{
			get{return m_snPeLengthPercentage;}
			set 
			{
				if(value != null)
				{
					if(value.ActualValue <= 0 || value.ActualValue >=100)
						throw new System.Exception("The Pe Length percentage must be between 0 and 100.");

					if(m_snMinPeLengthPercentage.ActualValue > value.ActualValue)
						throw new System.Exception("The Pe Length percentage can not be made lower than the minimum pe length percentage.");

					m_snPeLengthPercentage.CopyData(ref value);
				}
			}
		}

		public virtual AnimatTools.Framework.ScaledNumber MinPeLengthPercentage
		{
			get{return m_snMinPeLengthPercentage;}
			set 
			{
				if(value != null)
				{
					if(value.ActualValue <= 0 || value.ActualValue >=100)
						throw new System.Exception("The Pe Length percentage must be between 0 and 100.");

					if(m_snPeLengthPercentage.ActualValue < value.ActualValue)
						throw new System.Exception("The Min Pe Length percentage can not be made greater than the pe length percentage.");

					m_snMinPeLengthPercentage.CopyData(ref value);
				}
			}
		}

		public virtual AnimatTools.Framework.ScaledNumber IbDischargeConstant
		{
			get{return m_snIbDischargeConstant;}
			set 
			{
				if(value != null)
				{
					if(value.ActualValue < 0)
						throw new System.Exception("The Ib discharge rate must be greater than or equal to zero.");

					m_snIbDischargeConstant.CopyData(ref value);
				}
			}
		}

		#endregion

		#region Methods

		public LinearHillMuscle(AnimatTools.Framework.DataObject doParent) : base(doParent)
		{
			m_thDataTypes.DataTypes.Clear();

			m_thDataTypes.DataTypes.Add(new AnimatTools.DataObjects.DataType("Tension", "Tension", "Newtons", "N", 0, 1000));
			m_thDataTypes.DataTypes.Add(new AnimatTools.DataObjects.DataType("Tdot", "Change in Tension", "Newtons per second", "N/s", 0, 1000));
			m_thDataTypes.DataTypes.Add(new AnimatTools.DataObjects.DataType("MuscleLength", "Muscle Length", "Meters", "m", 0, 1));
			m_thDataTypes.DataTypes.Add(new AnimatTools.DataObjects.DataType("SeLength", "Se Length", "Meters", "m", 0, 1));
			m_thDataTypes.DataTypes.Add(new AnimatTools.DataObjects.DataType("PeLength", "Pe Length", "Meters", "m", 0, 1));
			m_thDataTypes.DataTypes.Add(new AnimatTools.DataObjects.DataType("A", "A", "Newtons", "N", 0, 1000));
			m_thDataTypes.DataTypes.Add(new AnimatTools.DataObjects.DataType("Activation", "Activation", "", "", 0, 1000));
			m_thDataTypes.DataTypes.Add(new AnimatTools.DataObjects.DataType("Vmuscle", "Muscle Velocity", "m/s", "m/s", -3, 3));
			m_thDataTypes.DataTypes.Add(new AnimatTools.DataObjects.DataType("Vse", "Se Velocity", "m/s", "m/s", -3, 3));
			m_thDataTypes.DataTypes.Add(new AnimatTools.DataObjects.DataType("Vpe", "Pe Velocity", "m/s", "m/s", -3, 3));
			m_thDataTypes.DataTypes.Add(new AnimatTools.DataObjects.DataType("AvgVmuscle", "Averaged Muscle Velocity", "m/s", "m/s", -3, 3));
			m_thDataTypes.DataTypes.Add(new AnimatTools.DataObjects.DataType("MembraneVoltage", "Membrane Voltage", "Volts", "V", -100, 100, ScaledNumber.enumNumericScale.milli, ScaledNumber.enumNumericScale.milli));
			m_thDataTypes.DataTypes.Add(new AnimatTools.DataObjects.DataType("Displacement", "Muscle Displacement", "Meters", "m", 0, 1));
			m_thDataTypes.DataTypes.Add(new AnimatTools.DataObjects.DataType("DisplacementRatio", "Muscle Displacement Ratio", "", "", -1, 1));
			m_thDataTypes.DataTypes.Add(new AnimatTools.DataObjects.DataType("Enable", "Enable", "", "", 0, 1));
			m_thDataTypes.DataTypes.Add(new AnimatTools.DataObjects.DataType("Ib", "Ib Discharge Rate", "Spikes/s", "Spikes/s", 0, 1000));
			m_thDataTypes.DataTypes.Add(new AnimatTools.DataObjects.DataType("Tl", "Tension-Length Percentage", "%", "%", 0, 1000));
			m_thDataTypes.ID = "Tension";

			m_thIncomingDataType = new AnimatTools.DataObjects.DataType("MembraneVoltage", "Membrane Voltage", "Volts", "V", -100, 100, ScaledNumber.enumNumericScale.milli, ScaledNumber.enumNumericScale.milli);

			m_StimTension = new AnimatTools.DataObjects.Gains.MuscleGains.StimulusTension(this);
			m_LengthTension = new AnimatTools.DataObjects.Gains.MuscleGains.LengthTension(this);
			m_LengthTension.ShowLcePercentage = false;

			m_snMaxTension = new AnimatTools.Framework.ScaledNumber(this, "MaxTension", 100, AnimatTools.Framework.ScaledNumber.enumNumericScale.None, "Newtons", "N");
			m_snMuscleLength = new AnimatTools.Framework.ScaledNumber(this, "MuscleLength", 0, AnimatTools.Framework.ScaledNumber.enumNumericScale.None, "Meters", "m");
			m_snKse = new AnimatTools.Framework.ScaledNumber(this, "Kse", 10, AnimatTools.Framework.ScaledNumber.enumNumericScale.None, "Newtons per meter", "N/m");
			m_snKpe = new AnimatTools.Framework.ScaledNumber(this, "Kpe", 1, AnimatTools.Framework.ScaledNumber.enumNumericScale.None, "Newtons per meter", "N/m");
			m_snB = new AnimatTools.Framework.ScaledNumber(this, "B", 1, AnimatTools.Framework.ScaledNumber.enumNumericScale.None, "Newton-seconds per meter", "Ns/m");

			m_snPeLengthPercentage = new AnimatTools.Framework.ScaledNumber(this, "PeLengthPercentage", 90, AnimatTools.Framework.ScaledNumber.enumNumericScale.None, "%", "%");
			m_snMinPeLengthPercentage = new AnimatTools.Framework.ScaledNumber(this, "MinPeLengthPercentage", 5, AnimatTools.Framework.ScaledNumber.enumNumericScale.None, "%", "%");

			m_snIbDischargeConstant = new AnimatTools.Framework.ScaledNumber(this, "Ib Discharge Constant", 100, AnimatTools.Framework.ScaledNumber.enumNumericScale.None, "Spikes/sN", "Spikes/sN");
		}


		protected override void BuildProperties()
		{
			base.BuildProperties();

			Crownwood.Magic.Controls.PropertyBag pbSubBag = m_StimTension.Properties;
			m_Properties.Properties.Add(new PropertySpec("Stimulus-Tension", pbSubBag.GetType(), 
																	"StimTension", "Muscle Properties", "Sets the stmilus-tension properties of the muscle.", pbSubBag,
																	typeof(AnimatTools.TypeHelpers.GainTypeEditor),  typeof(Crownwood.Magic.Controls.ExpandablePropBagConverter)));

			pbSubBag = m_LengthTension.Properties;
			m_Properties.Properties.Add(new PropertySpec("Length-Tension", pbSubBag.GetType(), 
																	"LengthTension", "Muscle Properties", "Sets the length-tension properties of the muscle.", pbSubBag,
																	typeof(AnimatTools.TypeHelpers.GainTypeEditor),  typeof(Crownwood.Magic.Controls.ExpandablePropBagConverter)));

			pbSubBag = m_snKse.Properties;
			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Kse", pbSubBag.GetType(), "Kse", 
																	"Muscle Properties", "Determines the stiffness of the SE spring element. This is the primarily the stiffness of the tendon.", pbSubBag,
																	"", typeof(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)));

			pbSubBag = m_snKpe.Properties;
			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Kpe", pbSubBag.GetType(), "Kpe", 
																	"Muscle Properties", "Determines the stiffness of the PE spring element. This controls force developed from passive stretch of the muscle.", pbSubBag,
																	"", typeof(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)));

			pbSubBag = m_snB.Properties;
			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("B", pbSubBag.GetType(), "B", 
										"Muscle Properties", "Determines the linear, viscous damping of this muscle. This model does NOT use a non-linear hill force-velocity curve.", pbSubBag,
										"", typeof(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)));

			pbSubBag = m_snPeLengthPercentage.Properties;
			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Pe Length Percentage", pbSubBag.GetType(), "PeLengthPercentage", 
																	"Muscle Properties", "The percentage of the resting length of the muscle that Pe section takes up.", pbSubBag,
																	"", typeof(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)));

			pbSubBag = m_snMinPeLengthPercentage.Properties;
			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Min Pe Length Percentage", pbSubBag.GetType(), "MinPeLengthPercentage", 
																	"Muscle Properties", "The minimum length, as a percentage of resting length, that the Pe section can go attain.", pbSubBag,
																	"", typeof(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)));

			pbSubBag = m_aryAttachmentPoints.Properties;
			m_Properties.Properties.Add(new PropertySpec("Calculate Stimulus", pbSubBag.GetType(), "", 
										"Muscle Properties", "Used to calculate the stimulus needed to develop a specific tension.", pbSubBag,
										typeof(VortexAnimatTools.TypeHelpers.CalcMuscleEditor),  typeof(Crownwood.Magic.Controls.ExpandablePropBagConverter)));

		
			pbSubBag = m_snIbDischargeConstant.Properties;
			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Ib Discharge Constant", pbSubBag.GetType(), "IbDischargeConstant", 
										"Muscle Properties", "Relates the muscle tension to the discharge rate of the type Ib fibers.", pbSubBag,
										"", typeof(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)));
		}

		public override AnimatTools.DataObjects.Physical.BodyPart CreateNewBodyPart(AnimatTools.Framework.DataObject doParent)
		{return new LinearHillMuscle(doParent);}

		protected override void CloneInternal(AnimatTools.Framework.DataObject doOriginal, bool bCutData, AnimatTools.Framework.DataObject doRoot)
		{
			base.CloneInternal (doOriginal, bCutData, doRoot);

			LinearHillMuscle doOrigBody = (LinearHillMuscle) doOriginal;

			m_aryAttachmentPoints = (VortexAnimatTools.Collections.MuscleAttachments) doOrigBody.m_aryAttachmentPoints.Copy();
			m_bEnabled = doOrigBody.m_bEnabled;
			m_StimTension = (AnimatTools.DataObjects.Gains.MuscleGains.StimulusTension) doOrigBody.m_StimTension.Clone(this, bCutData, doRoot);
			m_LengthTension = (AnimatTools.DataObjects.Gains.MuscleGains.LengthTension) doOrigBody.m_LengthTension.Clone(this, bCutData, doRoot);
			m_snMaxTension = (AnimatTools.Framework.ScaledNumber) doOrigBody.m_snMaxTension.Clone(this, bCutData, doRoot);
			m_snMuscleLength = (AnimatTools.Framework.ScaledNumber) doOrigBody.m_snMuscleLength.Clone(this, bCutData, doRoot);
			m_snKse = (AnimatTools.Framework.ScaledNumber) doOrigBody.m_snKse.Clone(this, bCutData, doRoot);
			m_snKpe = (AnimatTools.Framework.ScaledNumber) doOrigBody.m_snKpe.Clone(this, bCutData, doRoot);
			m_snB = (AnimatTools.Framework.ScaledNumber) doOrigBody.m_snB.Clone(this, bCutData, doRoot);
			m_snPeLengthPercentage = (AnimatTools.Framework.ScaledNumber) doOrigBody.m_snPeLengthPercentage.Clone(this, bCutData, doRoot);
			m_snMinPeLengthPercentage = (AnimatTools.Framework.ScaledNumber) doOrigBody.m_snMinPeLengthPercentage.Clone(this, bCutData, doRoot);
			m_snIbDischargeConstant = (AnimatTools.Framework.ScaledNumber) doOrigBody.m_snIbDischargeConstant.Clone(this, bCutData, doRoot);
		}

		public override AnimatTools.Framework.DataObject Clone(AnimatTools.Framework.DataObject doParent, bool bCutData, AnimatTools.Framework.DataObject doRoot)
		{
			LinearHillMuscle bnPart = new LinearHillMuscle(doParent);
			bnPart.CloneInternal(this, bCutData, doRoot);
			if(doRoot != null && doRoot == this) bnPart.AfterClone(this, bCutData, doRoot, bnPart);
			return bnPart;
		}

		#region Loading and Saving Data

		public override void LoadData(ref AnimatTools.DataObjects.Simulation dsSim, ref AnimatTools.DataObjects.Physical.PhysicalStructure doStructure, ref AnimatTools.Interfaces.StdXml oXml)
		{
			base.LoadData(ref dsSim, ref doStructure, ref oXml);

			oXml.IntoElem(); //'Into RigidBody Element

			m_StimTension.LoadData(ref oXml, "StimulusTension", "StimTension");
			m_LengthTension.LoadData(ref oXml, "LengthTension", "LengthTension");
			m_snKse.LoadData(ref oXml, "Kse");
			m_snKpe.LoadData(ref oXml, "Kpe");
			m_snB.LoadData(ref oXml, "B");

			if(oXml.FindChildElement("PeLength", false))
			{
				m_snPeLengthPercentage.LoadData(ref oXml, "PeLength");
				m_snMinPeLengthPercentage.LoadData(ref oXml, "MinPeLength");
			}

//			if(oXml.FindChildElement("IbDischarge", false))
//				m_snIbDischargeConstant.LoadData(ref oXml, "IbDischarge");

			oXml.OutOfElem(); //Outof RigidBody Element			
		}


		public override void SaveData(ref AnimatTools.DataObjects.Simulation dsSim, ref AnimatTools.DataObjects.Physical.PhysicalStructure doStructure, ref AnimatTools.Interfaces.StdXml oXml)
		{
			base.SaveData(ref dsSim, ref doStructure, ref oXml);

			oXml.IntoElem(); //Into RigidBody Element
 
			m_StimTension.SaveData(ref oXml, "StimulusTension");
			m_LengthTension.SaveData(ref oXml, "LengthTension");
			m_snKse.SaveData(ref oXml, "Kse");
			m_snKpe.SaveData(ref oXml, "Kpe");
			m_snB.SaveData(ref oXml, "B");
			m_snPeLengthPercentage.SaveData(ref oXml, "PeLength");
			m_snMinPeLengthPercentage.SaveData(ref oXml, "MinPeLength");
			m_snIbDischargeConstant.SaveData(ref oXml, "IbDischarge");

			oXml.OutOfElem(); //Outof RigidBody Element			
		}

		#endregion

		#endregion
	}
}
