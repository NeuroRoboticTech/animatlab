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
	public class DxConeSizeRef : AnimatTools.Framework.Vec3d
	{
		#region Attributes
		
		protected VortexAnimatTools.DataObjects.Physical.RigidBodies.Cone m_doPart;

		#endregion

		#region Properties

		[Browsable(false)]
		public override double X 
		{
			get {return m_doPart.UpperRadius;}
			set 
			{
				m_doPart.ManualAddPropertyHistory("UpperRadius", m_doPart.UpperRadius, value, false);

				m_doPart.UpperRadius = (float) value;
				if(m_doParent != null) m_doParent.IsDirty = true;
			}
		}

		[Browsable(false)]
		public override double Y 
		{
			get{return m_doPart.LowerRadius;}
			set 
			{
				m_doPart.ManualAddPropertyHistory("LowerRadius", m_doPart.LowerRadius, value, false);

				m_doPart.LowerRadius = (float) value;
				if(m_doParent != null) m_doParent.IsDirty = true;
			}
		}

		[Browsable(false)]
		public override double Z 
		{
			get{return m_doPart.Height;}
			set 
			{
				m_doPart.ManualAddPropertyHistory("Height", m_doPart.Height, value, false);

				m_doPart.Height = (float) value;
				if(m_doParent != null) m_doParent.IsDirty = true;
			}
		}

		public double UpperRadius 
		{
			get {return m_doPart.UpperRadius;}
			set 
			{
				m_doPart.ManualAddPropertyHistory("UpperRadius", m_doPart.UpperRadius, value, false);

				m_doPart.UpperRadius = (float) value;
				if(m_doParent != null) m_doParent.IsDirty = true;
			}
		}

		public double LowerRadius 
		{
			get{return m_doPart.LowerRadius;}
			set 
			{
				m_doPart.ManualAddPropertyHistory("LowerRadius", m_doPart.LowerRadius, value, false);

				m_doPart.LowerRadius = (float) value;
				if(m_doParent != null) m_doParent.IsDirty = true;
			}
		}

		public double Height 
		{
			get{return m_doPart.Height;}
			set 
			{
				m_doPart.ManualAddPropertyHistory("Height", m_doPart.Height, value, false);

				m_doPart.Height = (float) value;
				if(m_doParent != null) m_doParent.IsDirty = true;
			}
		}

		#endregion

		#region Methods

		public DxConeSizeRef(AnimatTools.Framework.DataObject doParent) : base(doParent)
		{
			m_doPart = (VortexAnimatTools.DataObjects.Physical.RigidBodies.Cone) doParent;
		}

		protected override void BuildProperties()
		{
			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Upper Radius", m_dblX.GetType(), "UpperRadius", 
																	"Part Properties", "Upper radius of the cone. ", this.UpperRadius));

			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Lower Radius", m_dblZ.GetType(), "LowerRadius", 
																	"Part Properties", "Lower radius of the cone. ", this.LowerRadius));

			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Height", m_dblY.GetType(), "Height", 
																	"Part Properties", "Height of the cone. ", this.Height));
		}

		#endregion
	}
}
