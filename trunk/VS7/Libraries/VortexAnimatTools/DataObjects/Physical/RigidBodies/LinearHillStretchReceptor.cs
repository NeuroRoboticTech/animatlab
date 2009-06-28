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
	public class LinearHillStretchReceptor : VortexAnimatTools.DataObjects.Physical.RigidBodies.LinearHillMuscle
	{

		#region Attributes

		protected bool m_bApplyTension = false;
		protected AnimatTools.Framework.ScaledNumber m_snIaDischargeConstant;
		protected AnimatTools.Framework.ScaledNumber m_snIIDischargeConstant;

		#endregion

		#region Properties

		public override String ImageName {get{return "VortexAnimatTools.Graphics.MuscleSpindle_Treeview.gif";}}
		public override String ButtonImageName {get{return "VortexAnimatTools.Graphics.MuscleSpindle_Button.gif";}}
		public override String Type {get{return "LinearHillStretchReceptor";}}
		public override String BodyPartName {get{return "Linear Hill Stretch Receptor";}}
		public override System.Type PartType {get{return typeof(VortexAnimatTools.DataObjects.Physical.RigidBodies.LinearHillStretchReceptor);}}
		public override bool AllowUserAdd {get{return true;}}
		public override bool CanBeRootBody {get{return false;}}
		public override bool UsesAJoint {get{return false;}}
		public override bool AllowCollisions {get{return false;}}

		public virtual bool ApplyTension
		{
			get{return m_bApplyTension;}
			set{m_bApplyTension = value;}
		}

		public virtual AnimatTools.Framework.ScaledNumber IaDischargeConstant
		{
			get{return m_snIaDischargeConstant;}
			set 
			{
				if(value != null)
				{
					if(value.ActualValue < 0)
						throw new System.Exception("The Ia discharge rate must be greater than or equal to zero.");

					m_snIaDischargeConstant.CopyData(ref value);
				}
			}
		}

		public virtual AnimatTools.Framework.ScaledNumber IIDischargeConstant
		{
			get{return m_snIIDischargeConstant;}
			set 
			{
				if(value != null)
				{
					if(value.ActualValue < 0)
						throw new System.Exception("The II discharge rate must be greater than or equal to zero.");

					m_snIIDischargeConstant.CopyData(ref value);
				}
			}
		}

		#endregion

		#region Methods
		
		public LinearHillStretchReceptor(AnimatTools.Framework.DataObject doParent) : base(doParent)
		{
			m_thDataTypes.DataTypes.Add(new AnimatTools.DataObjects.DataType("Ia", "Ia Discharge Rate" , "Spikes/s", "Spikes/s", 0, 1000));
			m_thDataTypes.DataTypes.Add(new AnimatTools.DataObjects.DataType("II", "II Discharge Rate", "Spikes/s", "Spikes/s", 0, 1000));
			m_thDataTypes.ID = "Ia";

			m_snIaDischargeConstant = new AnimatTools.Framework.ScaledNumber(this, "Ia Discharge Constant", 100, AnimatTools.Framework.ScaledNumber.enumNumericScale.None, "Spikes/sm", "Spikes/sm");
			m_snIIDischargeConstant = new AnimatTools.Framework.ScaledNumber(this, "II Discharge Constant", 100, AnimatTools.Framework.ScaledNumber.enumNumericScale.None, "Spikes/sm", "Spikes/sm");
		}

		protected override void BuildProperties()
		{
			base.BuildProperties();

			Crownwood.Magic.Controls.PropertyBag pbSubBag = m_StimTension.Properties;

			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Apply Tension", m_bApplyTension.GetType(), "ApplyTension",
				"Muscle Properties", "Determines whether the stretch receptor actually applies tension.", m_bApplyTension));

			pbSubBag = m_snIaDischargeConstant.Properties;
			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Ia Discharge Constant", pbSubBag.GetType(), "IaDischargeConstant", 
				"Muscle Properties", "Relates the length of segments of the stretch receptor to the discharge rate of the type Ia fibers.", pbSubBag,
				"", typeof(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)));

			pbSubBag = m_snIIDischargeConstant.Properties;
			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("II Discharge Constant", pbSubBag.GetType(), "IIDischargeConstant", 
				"Muscle Properties", "Relates the length of segments of the stretch receptor to the discharge rate of the type II fibers.", pbSubBag,
				"", typeof(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)));
		}

		public override AnimatTools.DataObjects.Physical.BodyPart CreateNewBodyPart(AnimatTools.Framework.DataObject doParent)
		{return new LinearHillStretchReceptor(doParent);}		

		protected override void CloneInternal(AnimatTools.Framework.DataObject doOriginal, bool bCutData, AnimatTools.Framework.DataObject doRoot)
		{
			base.CloneInternal (doOriginal, bCutData, doRoot);

			LinearHillStretchReceptor doOrigBody = (LinearHillStretchReceptor) doOriginal;

			m_bApplyTension = doOrigBody.m_bApplyTension;
			m_snIaDischargeConstant = (AnimatTools.Framework.ScaledNumber) doOrigBody.m_snIaDischargeConstant.Clone(this, bCutData, doRoot);
			m_snIIDischargeConstant = (AnimatTools.Framework.ScaledNumber) doOrigBody.m_snIIDischargeConstant.Clone(this, bCutData, doRoot);
		}

		public override AnimatTools.Framework.DataObject Clone(AnimatTools.Framework.DataObject doParent, bool bCutData, AnimatTools.Framework.DataObject doRoot)
		{
			LinearHillStretchReceptor bnPart = new LinearHillStretchReceptor(doParent);
			bnPart.CloneInternal(this, bCutData, doRoot);
			if(doRoot != null && doRoot == this) bnPart.AfterClone(this, bCutData, doRoot, bnPart);
			return bnPart;
		}


		public override void LoadData(ref AnimatTools.DataObjects.Simulation dsSim, ref AnimatTools.DataObjects.Physical.PhysicalStructure doStructure, ref AnimatTools.Interfaces.StdXml oXml)
		{
			base.LoadData(ref dsSim, ref doStructure, ref oXml);

			oXml.IntoElem(); //'Into RigidBody Element

			m_bApplyTension = oXml.GetChildBool("ApplyTension", false);
			m_snIaDischargeConstant.LoadData(ref oXml, "IaDischarge");
			m_snIIDischargeConstant.LoadData(ref oXml, "IIDischarge");

			oXml.OutOfElem(); //Outof RigidBody Element			
		}

		public override void SaveData(ref AnimatTools.DataObjects.Simulation dsSim, ref AnimatTools.DataObjects.Physical.PhysicalStructure doStructure, ref AnimatTools.Interfaces.StdXml oXml)
		{
			base.SaveData(ref dsSim, ref doStructure, ref oXml);

			oXml.IntoElem(); //'Into RigidBody Element

			oXml.AddChildElement("ApplyTension", m_bApplyTension);
			m_snIaDischargeConstant.SaveData(ref oXml, "IaDischarge");
			m_snIIDischargeConstant.SaveData(ref oXml, "IIDischarge");

			oXml.OutOfElem(); //Outof RigidBody Element			
		}

		#endregion

	}
}
