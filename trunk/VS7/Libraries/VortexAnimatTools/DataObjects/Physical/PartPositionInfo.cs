using System;
using System.Drawing;
using System.Windows.Forms;
using System.Collections;
using Microsoft.DirectX;
using Microsoft.DirectX.Direct3D;
using AnimatTools;
using AnimatTools.Framework;
using VortexAnimatTools.DataObjects.Physical.PropertyHelpers;
using VortexAnimatTools.DataObjects;

namespace VortexAnimatTools.DataObjects.Physical
{
	/// <summary>
	/// Summary description for BodyPositionInfo.
	/// </summary>
	public class PartPositionInfo : AnimatTools.DataObjects.Physical.PartPositionInfo
	{

		#region Attributes

		public Microsoft.DirectX.Vector3 m_v3Rotation;
		public Microsoft.DirectX.Vector3 m_v3Direction;
		public AnimatTools.Framework.Vec3d m_vSize;
		public Microsoft.DirectX.Matrix m_mOrientation = Matrix.Identity;
		public Matrix m_mtxTranslation = Matrix.Identity;

		#endregion

		#region Methods

		public PartPositionInfo()
		{
		}

		public override bool HasChanged(AnimatTools.DataObjects.Physical.PartPositionInfo piEndSate)
		{
			VortexAnimatTools.DataObjects.Physical.PartPositionInfo piEnd = (VortexAnimatTools.DataObjects.Physical.PartPositionInfo) piEndSate;

			if(!Util_DX.MatricesEqual(m_mOrientation, piEnd.m_mOrientation))
				return true;

			if(!Util_DX.MatricesEqual(m_mtxTranslation, piEnd.m_mtxTranslation))
				return true;

			if(m_v3Rotation.X != piEnd.m_v3Rotation.X || m_v3Rotation.Y != piEnd.m_v3Rotation.Y || m_v3Rotation.Z != piEnd.m_v3Rotation.Z)
				return true;

			if(m_v3Direction.X != piEnd.m_v3Direction.X || m_v3Direction.Y != piEnd.m_v3Direction.Y || m_v3Direction.Z != piEnd.m_v3Direction.Z)
				return true;

			if(m_vSize.X != piEnd.m_vSize.X || m_vSize.Y != piEnd.m_vSize.Y || m_vSize.Z != piEnd.m_vSize.Z)
				return true;

			return false;
		}

		#endregion

	}
}
