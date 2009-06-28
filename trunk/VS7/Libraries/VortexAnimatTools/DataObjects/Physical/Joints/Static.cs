using System;
using Microsoft.DirectX;
using Microsoft.DirectX.Direct3D;

namespace VortexAnimatTools.DataObjects.Physical.Joints
{
	/// <summary>
	/// Summary description for Static.
	/// </summary>
	public class Static : VortexAnimatTools.DataObjects.Physical.Joints.Joint_DX
	{
		public Static(AnimatTools.Framework.DataObject doParent) : base(doParent)
		{
			Color  = System.Drawing.Color.PowderBlue;
		}

		#region Properties

		public override String ImageName {get{return "VortexAnimatTools.Graphics.Static_Treeview.gif";}}
		public override String ButtonImageName {get{return "VortexAnimatTools.Graphics.Static_Button.gif";}}
		public override String Type {get{return "Static";}}
		public override String BodyPartName {get{return "Static";}}
		public override System.Type PartType {get{return typeof(VortexAnimatTools.DataObjects.Physical.Joints.Static);}}


		public override Microsoft.DirectX.Vector3 DxLocation 
		{
			get
			{return base.DxLocation;
			}
			set
			{
				Microsoft.DirectX.Vector3 vVal = new Microsoft.DirectX.Vector3(0, 0, 0);

				//get the parents combined transformation matrix
				Matrix mtxParent = this.Parent_DX.CombinedTransformationMatrix;

				//get the inverse of the parent's combined transformation matrix
				Matrix mtxParentInv = Util_DX.CloneMatrix(mtxParent);
				mtxParentInv.Invert();

				//Create a mouse matrix
				Matrix mtxT = mtxParent * Matrix.Translation(vVal) * mtxParentInv;

				m_mtxTranslation = Matrix.Translation(vVal);
			}
		}

		public override Microsoft.DirectX.Vector3 DxRotation
		{
			get
			{
				return m_v3Rotation;
			}
			set{m_v3Rotation = new Vector3(0, 0, 0);}
		}

		#endregion

		#region Methods

		protected override void CreateBody()
		{
			if(m_d3dMesh != null)
			{
				m_d3dMesh.Dispose();
				m_d3dMesh = null;
			}

			if(m_mshSelectedBoundingBox != null)
			{
				this.m_mshSelectedBoundingBox.Dispose();
				this.m_mshSelectedBoundingBox = null;
			}

			if(m_mshCB != null)
			{
				this.m_mshCB.Dispose();
				this.m_mshCB = null;
			}

			m_d3dMesh = Mesh.Sphere(this.Device,m_fRadius,32,32);

			float fltScale = (this.MaxDimension * 0.15f);
			if(fltScale > 0.3f) fltScale = 0.3f;

			m_mshSelectedBoundingBox = Mesh.Box(Device, (float)((2*m_fRadius) + fltScale),(float)((2*m_fRadius) + fltScale), (float)((2*m_fRadius) + fltScale)); 
			RefreshSelectionBoxSizes();

			base.CreateBody();
		}

		public override AnimatTools.DataObjects.Physical.BodyPart CreateNewBodyPart(AnimatTools.Framework.DataObject doParent)
		{return new Static(doParent);}

		public override AnimatTools.Framework.DataObject Clone(AnimatTools.Framework.DataObject doParent, bool bCutData, AnimatTools.Framework.DataObject doRoot)
		{
			Static bnPart = new Static(doParent);
			bnPart.CloneInternal(this, bCutData, doRoot);
			if(doRoot != null && doRoot == this) bnPart.AfterClone(this, bCutData, doRoot, bnPart);
			return bnPart;
		}

		protected override void BuildProperties()
		{
			base.BuildProperties();

			//remove local location
			if(m_Properties.Properties.Contains("X") == true)
				m_Properties.Properties.Remove("X");
			if(m_Properties.Properties.Contains("Y") == true)
				m_Properties.Properties.Remove("Y");
			if(m_Properties.Properties.Contains("Z") == true)
				m_Properties.Properties.Remove("Z");

			//remove world location
			if(m_Properties.Properties.Contains("X") == true)
				m_Properties.Properties.Remove("X");
			if(m_Properties.Properties.Contains("Y") == true)
				m_Properties.Properties.Remove("Y");
			if(m_Properties.Properties.Contains("Z") == true)
				m_Properties.Properties.Remove("Z");
		
			//remove rotation
			if(m_Properties.Properties.Contains("X") == true)
				m_Properties.Properties.Remove("X");
			if(m_Properties.Properties.Contains("Y") == true)
				m_Properties.Properties.Remove("Y");
			if(m_Properties.Properties.Contains("Z") == true)
				m_Properties.Properties.Remove("Z");

			if(m_Properties.Properties.Contains("Enable Limits") == true)
				m_Properties.Properties.Remove("Enable Limits");
			if(m_Properties.Properties.Contains("Damping") == true)
				m_Properties.Properties.Remove("Damping");
			if(m_Properties.Properties.Contains("Restitution") == true)
				m_Properties.Properties.Remove("Restitution");
			if(m_Properties.Properties.Contains("Stiffness") == true)
				m_Properties.Properties.Remove("Stiffness");
		}		

		#endregion

	}
}
