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
	public class DxPrismaticConstraintRef : AnimatTools.Framework.Vec3d
	{
		#region Attributes
		
		protected VortexAnimatTools.DataObjects.Physical.Joints.Prismatic m_doPart;

		#endregion

		#region Properties

		[Browsable(false)]
		public override double X 
		{
			get {return m_doPart.MinMovement;}
			set 
			{
				m_doPart.ManualAddPropertyHistory("MinMovement", m_doPart.MinMovement, value, false);

				m_doPart.MinMovement = (float) value;
				if(m_doParent != null) m_doParent.IsDirty = true;
			}
		}

		[Browsable(false)]
		public override double Y 
		{
			get{return m_doPart.MaxMovement;}
			set 
			{
				m_doPart.ManualAddPropertyHistory("MaxMovement", m_doPart.MaxMovement, value, false);

				m_doPart.MaxMovement = (float) value;
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
			get {return m_doPart.MinMovement;}
			set 
			{
				m_doPart.ManualAddPropertyHistory("MinMovement", m_doPart.MinMovement, value, false);

				m_doPart.MinMovement = (float) value;
				if(m_doParent != null) m_doParent.IsDirty = true;
			}
		}

		public double Maximum
		{
			get{return m_doPart.MaxMovement;}
			set 
			{
				m_doPart.ManualAddPropertyHistory("MaxMovement", m_doPart.MaxMovement, value, false);

				m_doPart.MaxMovement = (float) value;
				if(m_doParent != null) m_doParent.IsDirty = true;
			}
		}

		#endregion

		#region Methods

		public DxPrismaticConstraintRef(AnimatTools.Framework.DataObject doParent) : base(doParent)
		{
			m_doPart = (VortexAnimatTools.DataObjects.Physical.Joints.Prismatic) doParent;
		}

		protected override void BuildProperties()
		{
			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Minimum", m_dblX.GetType(), "Minimum", 
																	"Part Properties", "Minimum movement for the prismatic joint. ", this.Minimum));

			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Maximum", m_dblY.GetType(), "Maximum", 
																	"Part Properties", "Maximum movement for the prismatic joint. ", this.Maximum));
		}

		public override string ToString()
		{
			return "(" + this.X.ToString("F") + ", " + this.Y.ToString("F") + ")";
		}

		#endregion
	}
}
