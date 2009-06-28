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
	public class DxBoxSizeRef : AnimatTools.Framework.Vec3d
	{
		#region Attributes
		
		protected VortexAnimatTools.DataObjects.Physical.RigidBodies.Box m_doPart;

		#endregion

		#region Properties

		[Browsable(false)]
		public override double X 
		{
			get {return m_doPart.Width;}
			set 
			{
				m_doPart.ManualAddPropertyHistory("Width", m_doPart.Width, value, false);

				m_doPart.Width = (float) value;
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
			get{return m_doPart.Length;}
			set 
			{
				m_doPart.ManualAddPropertyHistory("Length", m_doPart.Length, value, false);

				m_doPart.Length = (float) value;
				if(m_doParent != null) m_doParent.IsDirty = true;
			}
		}

		public double Width 
		{
			get {return m_doPart.Width;}
			set 
			{
				m_doPart.ManualAddPropertyHistory("Width", m_doPart.Width, value, false);

				m_doPart.Width = (float) value;
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

		public double Length 
		{
			get{return m_doPart.Length;}
			set 
			{
				m_doPart.ManualAddPropertyHistory("Length", m_doPart.Length, value, false);

				m_doPart.Length = (float) value;
				if(m_doParent != null) m_doParent.IsDirty = true;
			}
		}

		#endregion

		#region Methods

		public DxBoxSizeRef(AnimatTools.Framework.DataObject doParent) : base(doParent)
		{
			m_doPart = (VortexAnimatTools.DataObjects.Physical.RigidBodies.Box) doParent;
		}

		protected override void BuildProperties()
		{
			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Width", m_dblX.GetType(), "Width", 
																	"Part Properties", "Width of the box. ", this.Width));

			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Height", m_dblY.GetType(), "Height", 
																	"Part Properties", "Height of the box. ", this.Height));

			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Length", m_dblZ.GetType(), "Length", 
																	"Part Properties", "Length of the box. ", this.Length));
		}

		#endregion
	}
}
