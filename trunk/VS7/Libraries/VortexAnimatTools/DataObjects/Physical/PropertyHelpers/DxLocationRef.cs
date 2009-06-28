using System;

namespace VortexAnimatTools.DataObjects.Physical.PropertyHelpers
{
	/// <summary>
	/// Summary description for DxVectorRef.
	/// </summary>
	public class DxLocationRef : AnimatTools.Framework.Vec3d
	{
		#region Attributes
		
		protected AnimatTools.DataObjects.Physical.BodyPart m_doPart;

		#endregion

		#region Properties

		public override double X 
		{
			get {return m_doPart.XLocalLocation;}
			set 
			{
				m_doPart.ManualAddPropertyHistory("XLocalLocation", m_doPart.XLocalLocation, value, false);

				m_doPart.XLocalLocation = (float) value;
				if(m_doParent != null) m_doParent.IsDirty = true;
			}
		}

		public override double Y 
		{
			get{return m_doPart.YLocalLocation;}
			set 
			{
				m_doPart.ManualAddPropertyHistory("YLocalLocation", m_doPart.YLocalLocation, value, false);

				m_doPart.YLocalLocation = (float) value;
				if(m_doParent != null) m_doParent.IsDirty = true;
			}
		}

		public override double Z 
		{
			get{return m_doPart.ZLocalLocation;}
			set 
			{
				m_doPart.ManualAddPropertyHistory("ZLocalLocation", m_doPart.ZLocalLocation, value, false);

				m_doPart.ZLocalLocation = (float) value;
				if(m_doParent != null) m_doParent.IsDirty = true;
			}
		}

		#endregion

		#region Methods

		public DxLocationRef(AnimatTools.Framework.DataObject doParent) : base(doParent)
		{
			m_doPart = (AnimatTools.DataObjects.Physical.BodyPart) doParent;
		}

		#endregion
	}
}
