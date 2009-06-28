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
	/// Summary description for Cylinder.
	/// </summary>
	public class Cylinder : VortexAnimatTools.DataObjects.Physical.RigidBodies.RigidBody_DX
	{
		#region Attributes

		protected float m_fRadius;
		protected float m_fHeight; //radius and height of the cylinder

		protected DxCylinderSizeRef m_vrSize;

		protected AnimatTools.Framework.ScaledNumber m_snRadius;
		protected AnimatTools.Framework.ScaledNumber m_snHeight;

		#endregion

		#region Properties

		public override String ImageName {get{return "VortexAnimatTools.Graphics.Cylinder_Treeview.gif";}}
		public override String ButtonImageName {get{return "VortexAnimatTools.Graphics.Cylinder_Button.gif";}}
		public override String Type {get{return "Cylinder";}}
		public override String BodyPartName {get{return "Cylinder";}}
		public override System.Type PartType {get{return typeof(VortexAnimatTools.DataObjects.Physical.RigidBodies.Cylinder);}}

		public override AnimatTools.Framework.Vec3d Size 
		{
			get
			{
				return m_vrSize;
			}
			set
			{
				if(value != null)
				{
					this.ManualAddPropertyHistory("Size", new AnimatTools.Framework.Vec3d(null, m_fRadius, m_fHeight, 0), value, true);

					if(this.Radius != value.X)
						this.Radius = (float) value.X;

					if(this.Height != value.Y)
						this.Height = (float) value.Y;
				}
			}
		}

		public double Radius 
		{
			get{return m_fRadius;}
			set 
			{
				if(value <= 0)
					throw new System.Exception("The radius of the cylinder can not be less than or equal to zero.");

				float fltdR = (float)(value - m_fRadius);
				SetSize((float) value, m_fHeight);
				base.CreateBody();
				//stays against parent
//				m_v3Location += (fltdR) * Direction;
//					
//				//if the body is a parent...move children
//				RigidBodies.RigidBody_DX dbChild;
//
//				foreach (DictionaryEntry deEntry in m_aryChildBodies)
//				{
//					dbChild = (RigidBodies.RigidBody_DX)deEntry.Value;
//					dbChild.AbsoluteLocation += (fltdR) * dbChild.Direction;
//				}

				InvalidateBodyView();
			}
		}

		public double Height 
		{
			get{return m_fHeight;}
			set 
			{
				if(value <= 0)
					throw new System.Exception("The height of the cylinder can not be less than or equal to zero.");

				float fltdR = (float)(value - m_fRadius);
				SetSize(m_fRadius, (float) value);
				base.CreateBody();
				//stays against parent
//				m_v3Location += (fltdR) * Direction;
//					
//				//if the body is a parent...move children
//				RigidBodies.RigidBody_DX dbChild;
//
//				foreach (DictionaryEntry deEntry in m_aryChildBodies)
//				{
//					dbChild = (RigidBodies.RigidBody_DX)deEntry.Value;
//					dbChild.AbsoluteLocation += (fltdR) * dbChild.Direction;
//				}

				InvalidateBodyView();
			}
		}

		public AnimatTools.Framework.ScaledNumber RadiusScaled
		{
			get
			{
				m_snRadius.SetFromValue(this.Radius*Util.Environment.DistanceUnitValue, (int) Util.Environment.DistanceUnits);
				return m_snRadius;
			}
			set 
			{
				if(value != null)
					this.Radius = (float) value.ActualValue/Util.Environment.DistanceUnitValue;
			}
		}

		public AnimatTools.Framework.ScaledNumber HeightScaled
		{
			get
			{
				m_snHeight.SetFromValue(this.Height*Util.Environment.DistanceUnitValue, (int) Util.Environment.DistanceUnits);
				return m_snHeight;
			}
			set 
			{
				if(value != null)
					this.Height = (float) value.ActualValue/Util.Environment.DistanceUnitValue;
			}
		}

		public override float MaxDimension
		{
			get
			{
				float fltMax = (float) this.Radius;

				if(this.Height > fltMax)
					fltMax = (float) this.Height;

				return fltMax;
			}
		}

		public override float MinDimension
		{
			get
			{
				float fltMin = (float) this.Radius;

				if(this.Height > fltMin)
					fltMin = (float) this.Height;

				return fltMin;
			}
		}

		#endregion

		#region Methods

		public Cylinder(AnimatTools.Framework.DataObject doParent) : base(doParent)
		{
			m_fRadius = 0.5f;
			m_fHeight = 1;

			m_snRadius = new AnimatTools.Framework.ScaledNumber(this, "RadiusScaled", "meters", "m");
			m_snHeight = new AnimatTools.Framework.ScaledNumber(this, "HeightScaled", "meters", "m");

			m_vrSize = new DxCylinderSizeRef(this);			
		}
		/// <summary>
		/// Set's the size of the cylinder
		/// </summary>
		/// <param name="radius">The radius of the cylinder</param>
		/// <param name="height">The height of the cylinder</param>
		public void SetSize(float radius, float height)
		{
			this.CleanUp();			

			m_fRadius = radius; m_fHeight = height;
			m_d3dMesh = Util_DX.CreateCylinder(this.Device, m_fRadius, m_fRadius, m_fHeight, 32);	
			
			float fltScale = (this.MaxDimension * 0.15f);
			if(fltScale > 0.3f) fltScale = 0.3f;

			m_mshSelectedBoundingBox = Mesh.Box(Device, (float)((2*m_fRadius) + fltScale),(float)((2*m_fRadius) + fltScale), (float)(m_fHeight + fltScale)); 
			RefreshSelectionBoxSizes();
		}

		protected override void CreateBody()
		{
			SetSize(m_fRadius,m_fHeight);
			base.CreateBody();
		}

		protected override void CreateReceptiveFields()
		{
			if(this.Device != null && !this.Device.Disposed)
			{
				float fltReceptiveFieldDistance = (float) m_snReceptiveFieldDistance.ActualValue / Util.Environment.DistanceUnitValue;

				int iHDivs = (int) ((m_fHeight/fltReceptiveFieldDistance) * 5);
				int iRDivs = (int) ((m_fRadius/fltReceptiveFieldDistance) * 5);
				
				if(iHDivs<10) iHDivs = 10; 
				if(iRDivs<10) iRDivs = 10; 

				if(iHDivs>50) iHDivs = 50; 
				if(iRDivs>50) iRDivs = 50; 

				m_d3dReceptiveFieldMesh = Mesh.Cylinder(this.Device, m_fRadius, m_fRadius, m_fHeight, iHDivs, iRDivs);
				m_d3dReceptiveFieldPoint = Mesh.Sphere(this.Device, 0.02f, 10, 5);

				Util_DX.ConvertPosOnlyVertexToPosNormCol(this.Device, ref m_d3dReceptiveFieldMesh);

				CreateReceptiveFieldVertices();
				m_vSelectedReceptiveField = (AnimatTools.Framework.Vec3d) m_aryReceptiveFields[0];

				Util_DX.ColorReceptiveField(m_d3dReceptiveFieldMesh, m_vSelectedReceptiveField, m_gnReceptiveFieldGain);
			}
			else
				CreateReceptiveFieldVertices();
		}
	
		protected override void CreateReceptiveFieldVertices()
		{
			m_aryReceptiveFields.Clear();

			//m_snReceptiveFieldDistance.ActualValue = 0.25f;
			float fltReceptiveFieldDistance = (float) m_snReceptiveFieldDistance.ActualValue / Util.Environment.DistanceUnitValue;

			float fltR = m_fRadius, fltH = m_fHeight, fltC = (float) (2*Math.PI*fltR);
			int iHDivs = (int) Math.Round(fltH/fltReceptiveFieldDistance);
			int iCDivs = (int) Math.Round(fltC/fltReceptiveFieldDistance);
			int iRDivs = (int) Math.Round(fltR/fltReceptiveFieldDistance);

			if(iHDivs<=0) iHDivs = 1;
			if(iCDivs<=0) iCDivs = 1;
			if(iRDivs<=0) iRDivs = 1;

			float fltHDiv = fltH/iHDivs;
			float fltCDiv = (float) (2*Math.PI)/iCDivs;
			float fltRDiv = fltR/iRDivs;
			int iHDiv, iCDiv, iRDiv;
			float h, c, x, y, h2;
			float half = 0.5f;

			//Lets go through and create the cylinder portion.
			for(iHDiv=0; iHDiv<=iHDivs; iHDiv++)
			{
				h = (-m_fHeight*half) + (fltHDiv*iHDiv);

				for(iCDiv=0; iCDiv<=iCDivs; iCDiv++)
				{
					c = fltCDiv*iCDiv;

					x = (float) (fltR * Math.Cos(c));
					y = (float) (fltR * Math.Sin(c));

					m_aryReceptiveFields.Add(new AnimatTools.Framework.Vec3d(null, x, y, h));
					//AddReceptiveField(x, y, h);
				}
			}

			//Now go through and add the end pieces of the cylinder.
			h = (-m_fHeight*half); h2 = (m_fHeight*half);
			for(iRDiv=0; iRDiv<iRDivs; iRDiv++)
			{
				fltR = fltRDiv*iRDiv;
				iCDivs = (int) Math.Round((2*Math.PI*fltR)/fltReceptiveFieldDistance);
				if(iCDivs == 0) iCDivs = 1;
				fltCDiv = (float) ((2*Math.PI)/iCDivs);

				if(iCDivs >= 3)
				{
					for(iCDiv=0; iCDiv<iCDivs; iCDiv++)
					{
						c = fltCDiv*iCDiv;

						x = (float) (fltR * Math.Cos(c));
						y = (float) (fltR * Math.Sin(c));

						m_aryReceptiveFields.Add(new AnimatTools.Framework.Vec3d(null, x, y, h));
						m_aryReceptiveFields.Add(new AnimatTools.Framework.Vec3d(null, x, y, h2));
						//AddReceptiveField(x, y, h);
						//AddReceptiveField(x, y, h2);
					}
				}
			}

			//Now add two points in the center of the end plates.
			//AddReceptiveField(0, 0, h);
			//AddReceptiveField(0, 0, h2);
			m_aryReceptiveFields.Add(new AnimatTools.Framework.Vec3d(null, 0, 0, h));
			m_aryReceptiveFields.Add(new AnimatTools.Framework.Vec3d(null, 0, 0, h2));

			SortReceptiveFields();
			VerifyReceptiveFielPairs();
		}

		public override void DrawBoundingBox()
		{
			base.DrawBoundingBox ();			

			//turn off z-buffer for drawing the upper and lower selection boxes, so they are always visible to the user
			Device.RenderState.ZBufferEnable = false;

			

//			tmp = m_v3Location - new Vector3(0,(float)(fltScale + this.Height)/2.0f,0);
//			tmp.TransformCoordinate(this.Device.Transform.World1);
//			m.M41 = tmp.X;
//			m.M42 = tmp.Y;
//			m.M43 = tmp.Z;
//			this.Device.Transform.World = m;
//			if(m_bUBSelected)
//				this.Device.Material = this.m_d3dSelectedMaterial;
//			else
//				this.Device.Material = this.m_d3dMaterial;
//			m_mshLB.DrawSubset(0);
			Device.RenderState.ZBufferEnable = true;	
		}

		public override void CalculateBodyExtent(ref float iMaxX, ref float iMinX, ref float iMaxY, ref float iMinY)
		{
			float fTmpMaxX = this.m_fRadius + this.XLocalLocation;
			float fTmpMinX = this.m_fRadius - this.XLocalLocation;
			float fTmpMaxY = this.m_fHeight/2 + this.YLocalLocation;
			float fTmpMinY = this.m_fHeight/2 - this.YLocalLocation;
			
			if(fTmpMaxX > iMaxX)
				iMaxX = fTmpMaxX;
			if(fTmpMinX < iMinX)
				iMinX = fTmpMinX;
			if(fTmpMaxY > iMaxY)
				iMaxY = fTmpMaxY;
			if(fTmpMinY < iMinY)
				iMinY = fTmpMinY;

		}

		protected override void BuildProperties()
		{
			base.BuildProperties();

//			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Size", this.Size.GetType(), "Size", 
//																	"Part Properties", "The size of the box.", this.Size, 
//																	"", typeof(AnimatTools.TypeHelpers.Vec3dTypeConverter)));
			
			Crownwood.Magic.Controls.PropertyBag pbNumberBag  = this.HeightScaled.Properties;
			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Height", pbNumberBag.GetType(), "HeightScaled", 
																	"Size", "Sets the height of the cylinder.", pbNumberBag,
																	"", typeof(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)));

			pbNumberBag  = this.RadiusScaled.Properties;
			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Radius", pbNumberBag.GetType(), "RadiusScaled", 
																	"Size", "Sets the radius of the cylinder.", pbNumberBag,
																	"", typeof(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)));
		}

		protected override void IncreaseSizeX(AnimatTools.Framework.MouseEventArgs AnimatMouseArgs)
		{
			double dblOldRadius = this.Radius;
			try
			{
				this.Radius = this.Radius +(AnimatMouseArgs.DeltaX * 0.005f) + (AnimatMouseArgs.DeltaY * 0.005f);
			}
			catch(Exception ex)
			{
				this.Radius = dblOldRadius;
			}
		}
		protected override void IncreaseSizeY(AnimatTools.Framework.MouseEventArgs AnimatMouseArgs)
		{
			double dblOldRadius = this.Radius;
			try
			{
				this.Radius = this.Radius +(AnimatMouseArgs.DeltaX * 0.005f) + (AnimatMouseArgs.DeltaY * 0.005f);
			}
			catch(Exception ex)
			{
				this.Radius = dblOldRadius;
			}
		}
		protected override void IncreaseSizeZ(AnimatTools.Framework.MouseEventArgs AnimatMouseArgs)
		{
			double dblOldHeight = this.Height;
			try
			{
				this.Height = this.Height +(AnimatMouseArgs.DeltaX * 0.005f) + (AnimatMouseArgs.DeltaY * 0.005f);
			}
			catch(Exception ex)
			{
				this.Height = dblOldHeight;
			}
		}

		public override AnimatTools.DataObjects.Physical.BodyPart CreateNewBodyPart(AnimatTools.Framework.DataObject doParent)
		{return new Cylinder(doParent);}

		protected override void CloneInternal(AnimatTools.Framework.DataObject doOriginal, bool bCutData, AnimatTools.Framework.DataObject doRoot)
		{
			base.CloneInternal (doOriginal, bCutData, doRoot);

			Cylinder doOrigBody = (Cylinder) doOriginal;

			m_fRadius = doOrigBody.m_fRadius;
			m_fHeight = doOrigBody.m_fHeight;

			m_snRadius = (AnimatTools.Framework.ScaledNumber) doOrigBody.m_snRadius.Clone(this, bCutData, doRoot);
			m_snHeight = (AnimatTools.Framework.ScaledNumber) doOrigBody.m_snHeight.Clone(this, bCutData, doRoot);
		}

		public override AnimatTools.Framework.DataObject Clone(AnimatTools.Framework.DataObject doParent, bool bCutData, AnimatTools.Framework.DataObject doRoot)
		{
			Cylinder bnPart = new Cylinder(doParent);
			bnPart.CloneInternal(this, bCutData, doRoot);
			if(doRoot != null && doRoot == this) bnPart.AfterClone(this, bCutData, doRoot, bnPart);
			return bnPart;
		}

		#region Loading and Saving Data

		public override void LoadData(ref AnimatTools.DataObjects.Simulation dsSim, ref AnimatTools.DataObjects.Physical.PhysicalStructure doStructure, ref AnimatTools.Interfaces.StdXml oXml)
		{
			base.LoadData (ref dsSim, ref doStructure, ref oXml);

			oXml.IntoElem();
					
			m_fRadius = oXml.GetChildFloat("Radius");
			m_fHeight = oXml.GetChildFloat("Height");

				
			oXml.OutOfElem(); //out of body			
		}

		public override void SaveData(ref AnimatTools.DataObjects.Simulation dsSim, ref AnimatTools.DataObjects.Physical.PhysicalStructure doStructure, ref AnimatTools.Interfaces.StdXml oXml)
		{
			base.SaveData (ref dsSim, ref doStructure, ref oXml);

			oXml.IntoElem();
					
			oXml.AddChildElement("Radius", m_fRadius);
			oXml.AddChildElement("Height", m_fHeight);

			oXml.AddChildElement("CollisionRadius", m_fRadius);
			oXml.AddChildElement("CollisionHeight", m_fHeight);
				
			oXml.OutOfElem(); //out of body			
		}

		#endregion

		#endregion

	}
}
