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
	/// Summary description for Sphere.
	/// </summary>
	public class Sphere : VortexAnimatTools.DataObjects.Physical.RigidBodies.RigidBody_DX
	{

		#region Attributes

		protected float m_fRadius =  0.1f; //radius of the sphere
		protected AnimatTools.Framework.ScaledNumber m_snRadius;
		
		#endregion

		#region Properties

		public override String ImageName {get{return "VortexAnimatTools.Graphics.Sphere_Treeview.gif";}}
		public override String ButtonImageName {get{return "VortexAnimatTools.Graphics.Sphere_Button.gif";}}
		public override String Type {get{return "Sphere";}}
		public override String BodyPartName {get{return "Sphere";}}
		public override System.Type PartType {get{return typeof(VortexAnimatTools.DataObjects.Physical.RigidBodies.Sphere);}}

		public override AnimatTools.Framework.Vec3d Size 
		{
			get
			{
				return new AnimatTools.Framework.Vec3d(null, this.Radius, 0, 0);
			}
			set
			{
				if(value != null)
				{
					this.ManualAddPropertyHistory("Size", new AnimatTools.Framework.Vec3d(null, m_fRadius, 0, 0), value, true);

					if(this.Radius != value.X)
						this.Radius = (float) value.X;
				}
			}
		}

		public double Radius 
		{
			get{return m_fRadius;}
			set 
			{
				if(value <= 0)
					throw new System.Exception("The radius of the sphere can not be less than or equal to zero.");

				float fltdR = (float)(value - m_fRadius);
				SetSize((float)value);
				base.CreateBody();
				
				//if the body is a parent...move children
				RigidBodies.RigidBody_DX dbChild;

				foreach (DictionaryEntry deEntry in m_aryChildBodies)
				{
					dbChild = (RigidBodies.RigidBody_DX)deEntry.Value;
					dbChild.RelativeLocation += (fltdR) * dbChild.Direction;
				}

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

		public override float MaxDimension {get{return (float) this.Radius;}}
		public override float MinDimension {get{return (float) this.Radius;}}

		#endregion

		#region Methods

		public Sphere(AnimatTools.Framework.DataObject doParent) : base(doParent)
		{
			m_fRadius = 0.5f;
			m_snRadius = new AnimatTools.Framework.ScaledNumber(this, "RadiusScaled", "meters", "m");
		}

		/// <summary>
		/// Set's the size of the sphere
		/// </summary>
		/// <param name="radius">The radius of the sphere</param>
		public void SetSize(float radius)
		{
			this.CleanUp();

			m_fRadius = radius;
			m_d3dMesh = Util_DX.CreateSphere(this.Device, m_fRadius, 32, 32);
			
			float fltScale = (this.MaxDimension * 0.2f);
			if(fltScale > 0.3f) fltScale = 0.3f;

			m_mshSelectedBoundingBox = Mesh.Box(Device, (float)((2*m_fRadius) + fltScale),(float)((2*m_fRadius) + fltScale), (float)((2*m_fRadius) + fltScale)); 
			RefreshSelectionBoxSizes();
		}

		protected override void CreateBody()
		{
			SetSize( m_fRadius);
			base.CreateBody ();
		}

		protected override void CreateReceptiveFields()
		{
			if(this.Device != null && !this.Device.Disposed)
			{
				float fltReceptiveFieldDistance = (float) m_snReceptiveFieldDistance.ActualValue / Util.Environment.DistanceUnitValue;

				int iRDivs = (int) ((m_fRadius/fltReceptiveFieldDistance) * 5);
				
				if(iRDivs<10) iRDivs = 10; 
				if(iRDivs>500) iRDivs = 500; 

				m_d3dReceptiveFieldMesh = Mesh.Sphere(this.Device, m_fRadius, iRDivs, iRDivs);
				m_d3dReceptiveFieldPoint = Mesh.Sphere(this.Device, 0.02f, 10, 5);

				Util_DX.ConvertPosOnlyVertexToPosNormCol(this.Device, ref m_d3dReceptiveFieldMesh);

				CreateReceptiveFieldVertices();
				m_vSelectedReceptiveField = (AnimatTools.Framework.Vec3d) m_aryReceptiveFields[0];

				Util_DX.ColorReceptiveField(m_d3dReceptiveFieldMesh, m_vSelectedReceptiveField, m_gnReceptiveFieldGain);
			}
			CreateReceptiveFieldVertices();
		}
	
		protected override void CreateReceptiveFieldVertices()
		{
			m_aryReceptiveFields.Clear();

			//m_snReceptiveFieldDistance.ActualValue = 0.25f;
			float fltReceptiveFieldDistance = (float) m_snReceptiveFieldDistance.ActualValue / Util.Environment.DistanceUnitValue;

			float fltR=0, fltH = m_fRadius*2;
			int iHDivs = (int) Math.Round(fltH/fltReceptiveFieldDistance), iCDivs=0;

			if(iHDivs<=3) iHDivs = 3;

			float fltHDiv = fltH/iHDivs;
			float fltCDiv=0;
			int iHDiv, iCDiv;
			float h, c, x, z;
			float half = 0.5f;

			for(iHDiv=0; iHDiv<=iHDivs; iHDiv++)
			{
				h = (-fltH*half) + (fltHDiv*iHDiv);

				fltR = (float) Math.Sqrt(Math.Pow(m_fRadius, 2) - h*h);
				iCDivs = (int) Math.Round((2*Math.PI*fltR)/fltReceptiveFieldDistance);
				if(iCDivs<3) iCDivs = 3;
				fltCDiv = (float) ((2*Math.PI)/iCDivs);

				for(iCDiv=0; iCDiv<=iCDivs; iCDiv++)
				{
					c = fltCDiv*iCDiv;

					x = (float) (fltR * Math.Cos(c));
					z = (float) (fltR * Math.Sin(c));

					m_aryReceptiveFields.Add(new AnimatTools.Framework.Vec3d(null, x, h, z));
					//AddReceptiveField(x, h, z);
				}
			}

			SortReceptiveFields();
			VerifyReceptiveFielPairs();
		}

		public override void CalculateBodyExtent(ref float iMaxX, ref float iMinX, ref float iMaxY, ref float iMinY)
		{
			float fTmpMaxX = this.m_fRadius + this.XLocalLocation;
			float fTmpMinX = this.m_fRadius - this.XLocalLocation;
			float fTmpMaxY = this.m_fRadius + this.YLocalLocation;
			float fTmpMinY = this.m_fRadius - this.YLocalLocation;
			
			if(fTmpMaxX > iMaxX)
				iMaxX = fTmpMaxX;
			if(fTmpMinX < iMinX)
				iMinX = fTmpMinX;
			if(fTmpMaxY > iMaxY)
				iMaxY = fTmpMaxY;
			if(fTmpMinY < iMinY)
				iMinY = fTmpMinY;
		}

		public override AnimatTools.DataObjects.Physical.BodyPart CreateNewBodyPart(AnimatTools.Framework.DataObject doParent)
		{return new Sphere(doParent);}		

		protected override void CloneInternal(AnimatTools.Framework.DataObject doOriginal, bool bCutData, AnimatTools.Framework.DataObject doRoot)
		{
			base.CloneInternal (doOriginal, bCutData, doRoot);

			Sphere doOrigBody = (Sphere) doOriginal;
			m_fRadius = doOrigBody.m_fRadius;
			m_snRadius = (AnimatTools.Framework.ScaledNumber) doOrigBody.m_snRadius.Clone(this, bCutData, doRoot);
		}

		public override AnimatTools.Framework.DataObject Clone(AnimatTools.Framework.DataObject doParent, bool bCutData, AnimatTools.Framework.DataObject doRoot)
		{
			Sphere bnPart = new Sphere(doParent);
			bnPart.CloneInternal(this, bCutData, doRoot);
			if(doRoot != null && doRoot == this) bnPart.AfterClone(this, bCutData, doRoot, bnPart);
			return bnPart;
		}

		protected override void BuildProperties()
		{
			base.BuildProperties();

//			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Radius", this.Radius.GetType(), "Radius",
//																 "Part Properties", "Sets the radius of the cylinder.", this.Radius));			
			
			Crownwood.Magic.Controls.PropertyBag pbNumberBag  = this.RadiusScaled.Properties;
			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Radius", pbNumberBag.GetType(), "RadiusScaled", 
																	"Size", "Sets the radius of the sphere.", pbNumberBag,
																	"", typeof(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)));
		}

		#region Mouse Manipulation Methods
		
		protected override void IncreaseSizeZ(AnimatTools.Framework.MouseEventArgs AnimatMouseArgs)
		{		
			double dblOldRadius = this.Radius;
			try
			{
				this.Radius = this.Radius + (-AnimatMouseArgs.DeltaX * 0.005f) + (AnimatMouseArgs.DeltaY * 0.005f);
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
				this.Radius = this.Radius + (-AnimatMouseArgs.DeltaX * 0.005f) + (AnimatMouseArgs.DeltaY * 0.005f);
			}
			catch(Exception ex)
			{
				this.Radius = dblOldRadius;
			}
		}
		
		protected override void IncreaseSizeX(AnimatTools.Framework.MouseEventArgs AnimatMouseArgs)
		{			
			double dblOldRadius = this.Radius;
			try
			{
				this.Radius = this.Radius + (-AnimatMouseArgs.DeltaX * 0.005f) + (AnimatMouseArgs.DeltaY * 0.005f);
			}
			catch(Exception ex)
			{
				this.Radius = dblOldRadius;
			}
		}

		#endregion

		#region Loading and Saving Data

		public override void LoadData(ref AnimatTools.DataObjects.Simulation dsSim, ref AnimatTools.DataObjects.Physical.PhysicalStructure doStructure, ref AnimatTools.Interfaces.StdXml oXml)
		{
			base.LoadData (ref dsSim, ref doStructure, ref oXml);

			oXml.IntoElem();
					
			m_fRadius = oXml.GetChildFloat("Radius");
				
			oXml.OutOfElem(); //out of body			
		}

		public override void SaveData(ref AnimatTools.DataObjects.Simulation dsSim, ref AnimatTools.DataObjects.Physical.PhysicalStructure doStructure, ref AnimatTools.Interfaces.StdXml oXml)
		{
			base.SaveData (ref dsSim, ref doStructure, ref oXml);

			oXml.IntoElem();
					
			oXml.AddChildElement("Radius", m_fRadius);
			oXml.AddChildElement("CollisionRadius", m_fRadius);
	
			oXml.OutOfElem(); //out of body			
		}

		#endregion

		#endregion

	}
}
