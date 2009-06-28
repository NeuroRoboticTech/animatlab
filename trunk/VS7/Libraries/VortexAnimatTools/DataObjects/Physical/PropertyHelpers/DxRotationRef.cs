using System;

namespace VortexAnimatTools.DataObjects.Physical.PropertyHelpers
{
	/// <summary>
	/// Summary description for DxVectorRef.
	/// </summary>
	public class DxRotationRef : AnimatTools.Framework.Vec3d
	{
		#region Attributes
		
		protected AnimatTools.DataObjects.Physical.BodyPart m_doPart;

		#endregion

		#region Properties

		public override double X 
		{
			get {return m_doPart.XRotation;}
			set 
			{
				m_doPart.ManualAddPropertyHistory("XRotation", m_doPart.XRotation, value, false);

				m_doPart.XRotation = (float) value;
				if(m_doParent != null) m_doParent.IsDirty = true;
			}
		}

		public override double Y 
		{
			get{return m_doPart.YRotation;}
			set 
			{
				m_doPart.ManualAddPropertyHistory("YRotation", m_doPart.YRotation, value, false);

				m_doPart.YRotation = (float) value;
				if(m_doParent != null) m_doParent.IsDirty = true;
			}
		}

		public override double Z 
		{
			get{return m_doPart.ZRotation;}
			set 
			{
				m_doPart.ManualAddPropertyHistory("ZRotation", m_doPart.ZRotation, value, false);

				m_doPart.ZRotation = (float) value;
				if(m_doParent != null) m_doParent.IsDirty = true;
			}
		}

		#endregion

		#region Methods

		public DxRotationRef(AnimatTools.Framework.DataObject doParent) : base(doParent)
		{
			m_doPart = (AnimatTools.DataObjects.Physical.BodyPart) doParent;
		}

		#endregion
	}
}
