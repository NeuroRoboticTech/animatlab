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
	/// Summary description for OdorSensor.
	/// </summary>
	public class OdorSensor : VortexAnimatTools.DataObjects.Physical.RigidBodies.Sensor
	{
		#region Attributes

		protected AnimatTools.TypeHelpers.LinkedOdorType m_thOdorType;

		#endregion

		#region Properties

		public override String ImageName {get{return "VortexAnimatTools.Graphics.OdorSensor_TreeView.gif";}}
		public override String ButtonImageName {get{return "VortexAnimatTools.Graphics.OdorSensor_Button.gif";}}
		public override String Type {get{return "OdorSensor";}}
		public override String BodyPartName {get{return "Odor Sensor";}}
		public override System.Type PartType {get{return typeof(VortexAnimatTools.DataObjects.Physical.RigidBodies.OdorSensor);}}

		public virtual AnimatTools.TypeHelpers.LinkedOdorType OdorType
		{
			get{return m_thOdorType;}
			set 
			{
				m_thOdorType = value;
			}
		}

		#endregion

		#region Methods

		public OdorSensor(AnimatTools.Framework.DataObject doParent) : base(doParent)
		{
			m_thOdorType = new AnimatTools.TypeHelpers.LinkedOdorTypeList(null);

			m_thDataTypes.DataTypes.Add(new AnimatTools.DataObjects.DataType("OdorValue", "Odor Value", "", "", -1000, 1000));
			m_thDataTypes.ID = "OdorValue";
		}

		protected override void BuildProperties()
		{
			base.BuildProperties();

			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Odor Type", typeof(AnimatTools.TypeHelpers.LinkedOdorType), "OdorType", 
																	"Part Properties", "The odor type to detect.", 
																	m_thOdorType, typeof(AnimatTools.TypeHelpers.DropDownListEditor), 
																	typeof(AnimatTools.TypeHelpers.LinkedOdorTypeConverter)));
		}

		public override void SaveData(ref AnimatTools.DataObjects.Simulation dsSim, ref AnimatTools.DataObjects.Physical.PhysicalStructure doStructure, ref AnimatTools.Interfaces.StdXml oXml)
		{
			base.SaveData (ref dsSim, ref doStructure, ref oXml);

			oXml.IntoElem();
					
			if(m_thOdorType != null && m_thOdorType.OdorType != null)
				oXml.AddChildElement("OdorTypeID", m_thOdorType.OdorType.ID);

			oXml.OutOfElem(); //out of body			
		}

		public override void LoadData(ref AnimatTools.DataObjects.Simulation dsSim, ref AnimatTools.DataObjects.Physical.PhysicalStructure doStructure, ref AnimatTools.Interfaces.StdXml oXml)
		{
			base.LoadData(ref dsSim, ref doStructure, ref oXml);

			oXml.IntoElem(); //'Into RigidBody Element

			string strOdorTypeID = oXml.GetChildString("OdorTypeID", "");
			if(strOdorTypeID.Length > 0)
			{
				if(Util.Environment.OdorTypes.Contains(strOdorTypeID))
					m_thOdorType.OdorType = Util.Environment.OdorTypes.GetItem(strOdorTypeID);
			}
		
			oXml.OutOfElem(); //Outof RigidBody Element	
		}

		public override AnimatTools.DataObjects.Physical.BodyPart CreateNewBodyPart(AnimatTools.Framework.DataObject doParent)
		{return new OdorSensor(doParent);}		

		public override AnimatTools.Framework.DataObject Clone(AnimatTools.Framework.DataObject doParent, bool bCutData, AnimatTools.Framework.DataObject doRoot)
		{
			OdorSensor bnPart = new OdorSensor(doParent);
			bnPart.CloneInternal(this, bCutData, doRoot);
			bnPart.m_thOdorType = m_thOdorType;
			if(doRoot != null && doRoot == this) bnPart.AfterClone(this, bCutData, doRoot, bnPart);
			return bnPart;
		}

		#endregion

	}
}
