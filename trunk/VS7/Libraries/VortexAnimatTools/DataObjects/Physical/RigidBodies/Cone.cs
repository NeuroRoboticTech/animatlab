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
	/// Summary description for Cone.
	/// </summary>
	public class Cone : VortexAnimatTools.DataObjects.Physical.RigidBodies.RigidBody_DX
	{
		#region Attributes
		
		/// <summary>
		/// The radius Of The cone at the base
		/// </summary>
		float m_fLowerRadius = 0.5f;
		/// <summary>
		/// The radius Of The cone at the tip
		/// </summary>
		float m_fUpperRadius = 0.0f;
		/// <summary>
		/// The height of the cone
		/// </summary>
		float m_fHeight = 1.0f;

		protected AnimatTools.Framework.ScaledNumber m_snUpperRadius;
		protected AnimatTools.Framework.ScaledNumber m_snLowerRadius;
		protected AnimatTools.Framework.ScaledNumber m_snHeight;

		protected DxConeSizeRef m_vrSize;

		#endregion

		#region Properties

		public override String ImageName {get{return "VortexAnimatTools.Graphics.Cone_Treeview.gif";}}
		public override String ButtonImageName {get{return "VortexAnimatTools.Graphics.Cone_Button.gif";}}
		public override String Type {get{return "Cone";}}
		public override String BodyPartName {get{return "Cone";}}
		public override System.Type PartType {get{return typeof(VortexAnimatTools.DataObjects.Physical.RigidBodies.Cone);}}

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
					this.ManualAddPropertyHistory("Size", new AnimatTools.Framework.Vec3d(null, m_fUpperRadius, m_fLowerRadius, m_fHeight), value, true);

					if(this.UpperRadius != value.X)
						this.UpperRadius = (float) value.X;

					if(this.LowerRadius != value.Y)
						this.LowerRadius = (float) value.Y;

					if(this.Height != value.Z)
						this.Height = (float) value.Z;
				}
			}
		}

		public double LowerRadius 
		{
			get{return m_fLowerRadius;}
			set 
			{
				if(value < 0)
					throw new System.Exception("The lower radius value of the cone can not be less than or equal to zero.");
				
				if(value == 0 && m_fUpperRadius == 0)
					throw new System.Exception("Both the lower and upper radius can not be zero.");

				SetSize((float) value, m_fUpperRadius, m_fHeight);
				base.CreateBody();
				InvalidateBodyView();
			}
		}

		public double UpperRadius 
		{
			get{return m_fUpperRadius;}
			set 
			{
				if(value < 0)
					throw new System.Exception("The upper radius value of the cone can not be less than or equal to zero.");
				
				if(value == 0 && m_fLowerRadius == 0)
					throw new System.Exception("Both the lower and upper radius can not be zero.");

				SetSize(m_fLowerRadius, (float) value, m_fHeight);
				base.CreateBody();
				InvalidateBodyView();
			}
		}

		public double Height 
		{
			get{return m_fHeight;}
			set 
			{
				if(value <= 0)
					throw new System.Exception("The height value of the cone can not be less than or equal to zero.");

				SetSize(m_fLowerRadius, m_fUpperRadius, (float) value);
				base.CreateBody();
				InvalidateBodyView();
			}
		}

		public AnimatTools.Framework.ScaledNumber UpperRadiusScaled
		{
			get
			{
				m_snUpperRadius.SetFromValue(this.UpperRadius*Util.Environment.DistanceUnitValue, (int) Util.Environment.DistanceUnits);
				return m_snUpperRadius;
			}
			set 
			{
				if(value != null)
					this.UpperRadius = (float) value.ActualValue/Util.Environment.DistanceUnitValue;
			}
		}

		public AnimatTools.Framework.ScaledNumber LowerRadiusScaled
		{
			get
			{
				m_snLowerRadius.SetFromValue(this.LowerRadius*Util.Environment.DistanceUnitValue, (int) Util.Environment.DistanceUnits);
				return m_snLowerRadius;
			}
			set 
			{
				if(value != null)
					this.LowerRadius = (float) value.ActualValue/Util.Environment.DistanceUnitValue;
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
				float fltMax = (float) this.UpperRadius;

				if(this.LowerRadius > fltMax)
					fltMax = (float) this.LowerRadius;

				if(this.Height > fltMax)
					fltMax = (float) this.Height;

				return fltMax;
			}
		}

		public override float MinDimension
		{
			get
			{
				float fltMin = (float) this.UpperRadius;

				if(this.LowerRadius > fltMin)
					fltMin = (float) this.LowerRadius;

				if(this.Height > fltMin)
					fltMin = (float) this.Height;

				return fltMin;
			}
		}

		#endregion

		#region Methods
		public Cone(AnimatTools.Framework.DataObject doParent) : base(doParent)
		{
			m_vrSize = new DxConeSizeRef(this);		

			m_snUpperRadius = new AnimatTools.Framework.ScaledNumber(this, "UpperRadiusScaled", "meters", "m");
			m_snLowerRadius = new AnimatTools.Framework.ScaledNumber(this, "LowerRadiusScaled", "meters", "m");
			m_snHeight = new AnimatTools.Framework.ScaledNumber(this, "HeightScaled", "meters", "m");

			m_fLowerRadius = 0.5f;
			m_fUpperRadius = 0;
			m_fHeight = 1;
		}

		/// <summary>
		/// Sets size of the Cone
		/// </summary>
		/// <param name="radius">radius of the base of the cone</param>
		/// <param name="height">the height of the cone</param>
		public void SetSize(float fltLowerRadius, float fltUpperRadius, float fltHeight)
		{
			this.CleanUp();
			m_fLowerRadius = fltLowerRadius; m_fUpperRadius = fltUpperRadius; m_fHeight = fltHeight;

			m_d3dMesh = Util_DX.CreateCylinder(this.Device, fltLowerRadius, fltUpperRadius, fltHeight, 32);	
			
			float fltScale = (this.MaxDimension * 0.15f);
			if(fltScale > 0.3f) fltScale = 0.3f;

			float fltMaxRadius = fltUpperRadius;
			if(fltLowerRadius > fltUpperRadius) fltMaxRadius = fltLowerRadius;

			m_mshSelectedBoundingBox = Mesh.Box(Device, (float)((2*fltMaxRadius) + fltScale),(float)((2*fltMaxRadius) + fltScale), (float)(fltHeight + fltScale)); 
			RefreshSelectionBoxSizes();
		}

		protected override void CreateReceptiveFields()
		{
			if(this.Device != null && !this.Device.Disposed)
			{
				float fltReceptiveFieldDistance = (float) m_snReceptiveFieldDistance.ActualValue / Util.Environment.DistanceUnitValue;

				int iHDivs = (int) ((m_fHeight/fltReceptiveFieldDistance) * 5);
				int iRUDivs = (int) ((m_fUpperRadius/fltReceptiveFieldDistance) * 5);
				int iRLDivs = (int) ((m_fLowerRadius/fltReceptiveFieldDistance) * 5);
				int iRDivs;
				
				if(iHDivs<10) iHDivs = 10; 
				if(iRUDivs<10) iRUDivs = 10; 
				if(iRLDivs<10) iRLDivs = 10; 

				if(iHDivs>50) iHDivs = 50; 
				if(iRUDivs>50) iRUDivs = 50; 
				if(iRLDivs>50) iRLDivs = 50; 
				
				if(iRLDivs > iRUDivs)
					iRDivs = iRLDivs;
				else
					iRDivs = iRUDivs;

				m_d3dReceptiveFieldMesh = Mesh.Cylinder(this.Device, m_fUpperRadius, m_fLowerRadius, m_fHeight, iHDivs, iRDivs);
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

			float fltR=0, fltH = m_fHeight;
			int iHDivs = (int) Math.Round(fltH/fltReceptiveFieldDistance);
			int iCDivs, iRDivs;

			if(iHDivs<=0) iHDivs = 1;

			float fltHDiv = fltH/iHDivs;
			float fltCDiv, fltRDiv;
			int iHDiv, iCDiv, iRDiv;
			float h, c, x, y;
			float half = 0.5f;

			//Find the equations for the line between the low and high radius
			h = m_fHeight*half; 
			float x1 = -h, x2 = h, y1=m_fUpperRadius, y2=m_fLowerRadius;
			float fltM = (y2-y1)/(x2-x1);
			float fltB = y1 - (fltM*x1);

			//Lets go through and create the cylinder portion.
			for(iHDiv=0; iHDiv<=iHDivs; iHDiv++)
			{
				h = (-m_fHeight*half) + (fltHDiv*iHDiv);

				fltR = fltM*h + fltB;
				iCDivs = (int) Math.Round((2*Math.PI*fltR)/fltReceptiveFieldDistance);
				if(iCDivs<4) iCDivs = 4;
				fltCDiv = (float) ((2*Math.PI)/iCDivs);

				for(iCDiv=0; iCDiv<iCDivs; iCDiv++)
				{
					c = fltCDiv*iCDiv;

					x = (float) (fltR * Math.Cos(c));
					y = (float) (fltR * Math.Sin(c));

					m_aryReceptiveFields.Add(new AnimatTools.Framework.Vec3d(null, x, y, h));
					//AddReceptiveField(x, y, h);
				}
			}

			//Now go through and add the end Math.PIeces of the cylinder.
			h = (m_fHeight*half);
			iRDivs = (int) (m_fLowerRadius/fltReceptiveFieldDistance);
			if(iHDivs<=0) iHDivs = 1;
			fltRDiv = m_fLowerRadius/iRDivs;

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
						//AddReceptiveField(x, y, h);
					}
				}
			}
			//AddReceptiveField(0, 0, h);
			m_aryReceptiveFields.Add(new AnimatTools.Framework.Vec3d(null, 0, 0, h));

			//Now go through and add the end Math.PIeces of the cylinder.
			h = (-m_fHeight*half);
			iRDivs = (int) (m_fUpperRadius/fltReceptiveFieldDistance);
			if(iHDivs<=0) iHDivs = 1;
			fltRDiv = m_fUpperRadius/iRDivs;

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
						//AddReceptiveField(x, y, h);
					}
				}
			}
			m_aryReceptiveFields.Add(new AnimatTools.Framework.Vec3d(null, 0, 0, h));
			//AddReceptiveField(0, 0, h);

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

//		protected override void IncreaseSizeX(AnimatTools.Framework.MouseEventArgs AnimatMouseArgs)
//		{
//			double dblOldRadius = this.Radius;
//			try
//			{
//				this.Radius = this.Radius +(AnimatMouseArgs.DeltaX * 0.005f) + (AnimatMouseArgs.DeltaY * 0.005f);
//			}
//			catch(Exception ex)
//			{
//				this.Radius = dblOldRadius;
//			}
//		}
//		protected override void IncreaseSizeY(AnimatTools.Framework.MouseEventArgs AnimatMouseArgs)
//		{
//			double dblOldRadius = this.Radius;
//			try
//			{
//				this.Radius = this.Radius +(AnimatMouseArgs.DeltaX * 0.005f) + (AnimatMouseArgs.DeltaY * 0.005f);
//			}
//			catch(Exception ex)
//			{
//				this.Radius = dblOldRadius;
//			}
//		}
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


		public override Vector3 FindPointOnSurface(Vector3 v3Start, Vector3 v3Direction)
		{
			this.UpperRadius = 0.5f;
			Vector3 v3Tmp =  base.FindPointOnSurface (v3Start, v3Direction);
			this.UpperRadius = 0.0f;
			return v3Tmp;
		}

		protected override void CreateBody()
		{
			SetSize(m_fLowerRadius, m_fUpperRadius, m_fHeight);
			base.CreateBody ();
		}

		public override AnimatTools.DataObjects.Physical.BodyPart CreateNewBodyPart(AnimatTools.Framework.DataObject doParent)
		{return new Cone(doParent);}

		protected override void CloneInternal(AnimatTools.Framework.DataObject doOriginal, bool bCutData, AnimatTools.Framework.DataObject doRoot)
		{
			base.CloneInternal (doOriginal, bCutData, doRoot);

			Cone doOrigBody = (Cone) doOriginal;

			m_fLowerRadius = doOrigBody.m_fLowerRadius;
			m_fUpperRadius = doOrigBody.m_fUpperRadius;
			m_fHeight = doOrigBody.m_fHeight;

			m_snUpperRadius = (AnimatTools.Framework.ScaledNumber) doOrigBody.m_snUpperRadius.Clone(this, bCutData, doRoot);
			m_snLowerRadius = (AnimatTools.Framework.ScaledNumber) doOrigBody.m_snLowerRadius.Clone(this, bCutData, doRoot);
			m_snHeight = (AnimatTools.Framework.ScaledNumber) doOrigBody.m_snHeight.Clone(this, bCutData, doRoot);
		}

		public override AnimatTools.Framework.DataObject Clone(AnimatTools.Framework.DataObject doParent, bool bCutData, AnimatTools.Framework.DataObject doRoot)
		{
			Cone bnPart = new Cone(doParent);
			bnPart.CloneInternal(this, bCutData, doRoot);
			if(doRoot != null && doRoot == this) bnPart.AfterClone(this, bCutData, doRoot, bnPart);
			return bnPart;
		}

		protected override void BuildProperties()
		{
			base.BuildProperties();

//			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Size", this.Size.GetType(), "Size", 
//																	"Part Properties", "The size of the cone.", this.Size, 
//																	"", typeof(AnimatTools.TypeHelpers.Vec3dTypeConverter)));
			
      Crownwood.Magic.Controls.PropertyBag pbNumberBag  = this.HeightScaled.Properties;
      m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Height", pbNumberBag.GetType(), "HeightScaled", 
                                  "Size", "Sets the height of the cone.", pbNumberBag,
                                  "", typeof(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)));

			pbNumberBag  = this.LowerRadiusScaled.Properties;
			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Lower Radius", pbNumberBag.GetType(), "LowerRadiusScaled", 
																	"Size", "Sets the lower radius of the cone.", pbNumberBag,
																	"", typeof(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)));

			pbNumberBag  = this.UpperRadiusScaled.Properties;
			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Upper Radius", pbNumberBag.GetType(), "UpperRadiusScaled", 
																	"Size", "Sets the upper radius of the cone.", pbNumberBag,
																	"", typeof(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)));
		}

		public override void CalculateBodyExtent(ref float iMaxX, ref float iMinX, ref float iMaxY, ref float iMinY)
		{
			float fTmpMaxX = this.m_fLowerRadius + this.XLocalLocation;
			float fTmpMinX = this.m_fLowerRadius - this.XLocalLocation;
			float fTmpMaxY = this.m_fHeight;
			float fTmpMinY = this.m_fHeight;
			
			if(fTmpMaxX > iMaxX)
				iMaxX = fTmpMaxX;
			if(fTmpMinX < iMinX)
				iMinX = fTmpMinX;
			if(fTmpMaxY > iMaxY)
				iMaxY = fTmpMaxY;
			if(fTmpMinY < iMinY)
				iMinY = fTmpMinY;

		}

		#region Loading and Saving Data

		public override void LoadData(ref AnimatTools.DataObjects.Simulation dsSim, ref AnimatTools.DataObjects.Physical.PhysicalStructure doStructure, ref AnimatTools.Interfaces.StdXml oXml)
		{
			base.LoadData (ref dsSim, ref doStructure, ref oXml);

			oXml.IntoElem();
					
			m_fLowerRadius = oXml.GetChildFloat("LowerRadius");
			m_fUpperRadius = oXml.GetChildFloat("UpperRadius");
			m_fHeight = oXml.GetChildFloat("Height");
			m_snHeight.ActualValue = m_fHeight;

			oXml.OutOfElem(); //out of body			
		}

		public override void SaveData(ref AnimatTools.DataObjects.Simulation dsSim, ref AnimatTools.DataObjects.Physical.PhysicalStructure doStructure, ref AnimatTools.Interfaces.StdXml oXml)
		{
			base.SaveData (ref dsSim, ref doStructure, ref oXml);

			oXml.IntoElem();
					
			oXml.AddChildElement("LowerRadius", m_fLowerRadius);
			oXml.AddChildElement("UpperRadius", m_fUpperRadius);
			oXml.AddChildElement("Height", m_fHeight);
			oXml.AddChildElement("CollisionLowerRadius", m_fLowerRadius);
			oXml.AddChildElement("CollisionUpperRadius", m_fUpperRadius);
			oXml.AddChildElement("CollisionHeight", m_fHeight);

			oXml.OutOfElem(); //out of body			

		}

		#endregion

		#endregion

	}
}
