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
	public class DxCylinderSizeRef : AnimatTools.Framework.Vec3d
	{
		#region Attributes
		
		protected VortexAnimatTools.DataObjects.Physical.RigidBodies.Cylinder m_doPart;

		#endregion

		#region Properties

		[Browsable(false)]
		public override double X 
		{
			get {return m_doPart.Radius;}
			set 
			{
				m_doPart.ManualAddPropertyHistory("Radius", m_doPart.Radius, value, false);

				m_doPart.Radius = (float) value;
				if(m_doParent != null) m_doParent.IsDirty = true;
			}
		}

		[Browsable(false)]
		public override double Y 
		{
			get{return m_doPart.Height;}
			set 
			{
				m_doPart.ManualAddPropertyHistory("Height", m_doPart.Height, value, false);

				m_doPart.Height = (float) value;
				if(m_doParent != null) m_doParent.IsDirty = true;
			}
		}

		[Browsable(false)]
		public override double Z 
		{
			get{return 0;}
			set {}
		}

		public double Radius 
		{
			get {return m_doPart.Radius;}
			set 
			{
				m_doPart.ManualAddPropertyHistory("Radius", m_doPart.Radius, value, false);

				m_doPart.Radius = (float) value;
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

		public DxCylinderSizeRef(AnimatTools.Framework.DataObject doParent) : base(doParent)
		{
			m_doPart = (VortexAnimatTools.DataObjects.Physical.RigidBodies.Cylinder) doParent;
		}

		protected override void BuildProperties()
		{
			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Radius", m_dblX.GetType(), "Radius", 
																	"Part Properties", "Radius of the cylinder. ", this.Radius));

			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Height", m_dblY.GetType(), "Height", 
																	"Part Properties", "Height of the cylinder. ", this.Height));
		}

		public override string ToString()
		{
			return "(" + this.X.ToString("F") + ", " + this.Y.ToString("F") + ")";
		}

		#endregion
	}
}
