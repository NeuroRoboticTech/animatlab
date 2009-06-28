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

namespace VortexAnimatTools.DataObjects.Physical.PropertyHelpers
{
	/// <summary>
	/// Summary description for DxVectorRef.
	/// </summary>
	public class DxHingeConstraintRef : AnimatTools.Framework.Vec3d
	{
		#region Attributes
		
		protected VortexAnimatTools.DataObjects.Physical.Joints.Hinge m_doPart;

		#endregion

		#region Properties

		[Browsable(false)]
		public override double X 
		{
			get {return m_doPart.MinRotation;}
			set 
			{
				m_doPart.ManualAddPropertyHistory("MinRotation", m_doPart.MinRotation, value, false);

				m_doPart.MinRotation = (float) value;
				if(m_doParent != null) m_doParent.IsDirty = true;
			}
		}

		[Browsable(false)]
		public override double Y 
		{
			get{return m_doPart.MaxRotation;}
			set 
			{
				m_doPart.ManualAddPropertyHistory("MaxRotation", m_doPart.MaxRotation, value, false);

				m_doPart.MaxRotation = (float) value;
				if(m_doParent != null) m_doParent.IsDirty = true;
			}
		}

		[Browsable(false)]
		public override double Z 
		{
			get{return 0;}
			set {}
		}

		public double Minimum
		{
			get {return m_doPart.MinRotation;}
			set 
			{
				m_doPart.ManualAddPropertyHistory("MinRotation", m_doPart.MinRotation, value, false);

				m_doPart.MinRotation = (float) value;
				if(m_doParent != null) m_doParent.IsDirty = true;
			}
		}

		public double Maximum
		{
			get{return m_doPart.MaxRotation;}
			set 
			{
				m_doPart.ManualAddPropertyHistory("MaxRotation", m_doPart.MaxRotation, value, false);

				m_doPart.MaxRotation = (float) value;
				if(m_doParent != null) m_doParent.IsDirty = true;
			}
		}

		#endregion

		#region Methods

		public DxHingeConstraintRef(AnimatTools.Framework.DataObject doParent) : base(doParent)
		{
			m_doPart = (VortexAnimatTools.DataObjects.Physical.Joints.Hinge) doParent;
		}

		protected override void BuildProperties()
		{
			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Minimum", m_dblX.GetType(), "Minimum", 
																	"Part Properties", "Minimum rotation for the hinge joint. ", this.Minimum));

			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Maximum", m_dblY.GetType(), "Maximum", 
																	"Part Properties", "Maximum rotation for the hinge joint. ", this.Maximum));
		}

		public override string ToString()
		{
			return "(" + this.X.ToString("F") + ", " + this.Y.ToString("F") + ")";
		}

		#endregion
	}
}
