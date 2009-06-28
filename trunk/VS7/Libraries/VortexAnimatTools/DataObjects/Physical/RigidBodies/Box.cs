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
	/// Summary description for Box.
	/// </summary>
	public class Box : VortexAnimatTools.DataObjects.Physical.RigidBodies.RigidBody_DX
	{

		#region Attributes
		
		protected Vec3d m_vGraphicsBoxSize;
		protected DxBoxSizeRef m_vrSize;

		protected AnimatTools.Framework.ScaledNumber m_snWidth;
		protected AnimatTools.Framework.ScaledNumber m_snHeight;
		protected AnimatTools.Framework.ScaledNumber m_snLength;

		#endregion

		#region Properties

		public override String ImageName {get{return "VortexAnimatTools.Graphics.Box_Treeview.gif";}}
		public override String ButtonImageName {get{return "VortexAnimatTools.Graphics.Box_Button.gif";}}
		public override String Type 
		{
			get 
			{
				if(m_bContactSensor)
					return "BoxContactSensor";
				else
					return "Box";
			}
		}
		public override String BodyPartName {get{return "Box";}}
		public override System.Type PartType {get{return typeof(VortexAnimatTools.DataObjects.Physical.RigidBodies.Box);}}

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
					this.ManualAddPropertyHistory("Size", new AnimatTools.Framework.Vec3d(null, m_vGraphicsBoxSize.X, m_vGraphicsBoxSize.Y, m_vGraphicsBoxSize.Z), value, true);

					if(this.Width != value.X)
						this.Width = (float) value.X;

					if(this.Height != value.Y)
						this.Height = (float) value.Y;

					if(this.Length != value.Z)
						this.Length = (float) value.Z;
				}
			}
		}

		public double Width 
		{
			get{return m_vGraphicsBoxSize.X;}
			set 
			{
				if(value <= 0)
					throw new System.Exception("The width of the box can not be zero or less.");

				float fltdX = (float)(value - m_vGraphicsBoxSize.X);
				SetSize((float) value, (float) m_vGraphicsBoxSize.Y, (float) m_vGraphicsBoxSize.Z);
				base.CreateBody();
//				//stays against parent
//				if(Direction.X != 0)
//				{
//					//m_v3Location += (fltdX/2.0f) * Direction;
//					Vector3 v = (fltdX/2.0f) * Direction;
//					m_mtxTransformation.Multiply(Matrix.Translation(v));
//					m_v3Location += v;
//				}
//				
//				//if the body is a parent...move children
//				RigidBodies.RigidBody_DX dbChild;
//
//				foreach (DictionaryEntry deEntry in m_aryChildBodies)
//				{
//					dbChild = (RigidBodies.RigidBody_DX)deEntry.Value;
//					if(dbChild.Direction.X != 0)
//						dbChild.RelativeLocation += (fltdX/2.0f) * dbChild.Direction;						
//				}
				
				InvalidateBodyView();
				//Location = new Vector3((float)value/2.0f, Location.Y, Location.Z);
			}
		}

		public double Height 
		{
			get{return m_vGraphicsBoxSize.Y;}
			set 
			{
				if(value <= 0)
					throw new System.Exception("The height of the box can not be zero or less.");

				float fltdY = (float)(value - m_vGraphicsBoxSize.Y);

				SetSize((float) m_vGraphicsBoxSize.X, (float) value, (float) m_vGraphicsBoxSize.Z);
				base.CreateBody();
//				//stays against parent
//				if(Direction.Y != 0)
//				{
//					//m_v3Location += (fltdY/2.0f) * Direction;
//					Vector3 v = (fltdY/2.0f) * Direction;
//					m_mtxTransformation.Multiply(Matrix.Translation(v));
//					m_v3Location += v;
//				}
//				
//				//if the body is a parent...move children
//				RigidBodies.RigidBody_DX dbChild;
//
//				foreach (DictionaryEntry deEntry in m_aryChildBodies)
//				{
//					dbChild = (RigidBodies.RigidBody_DX)deEntry.Value;
//					if(dbChild.Direction.Y != 0)
//						dbChild.RelativeLocation += (fltdY/2.0f) * dbChild.Direction;						
//				}//end foreach

				InvalidateBodyView();
			}//end set
		}

		public double Length 
		{
			get{return m_vGraphicsBoxSize.Z;}
			set 
			{
				if(value <= 0)
					throw new System.Exception("The length of the box can not be zero or less.");

				float fltdZ = (float)(value - m_vGraphicsBoxSize.Z);

				SetSize((float) m_vGraphicsBoxSize.X, (float) m_vGraphicsBoxSize.Y, (float)value);
				base.CreateBody();
//				//stays against parent
//				if(Direction.Z != 0)
//				{
//					//m_v3Location += (fltdZ/2.0f) * Direction;
//					Vector3 v = (fltdZ/2.0f) * Direction;
//					m_mtxTransformation.Multiply(Matrix.Translation(v));
//					m_v3Location += v;
//				}
//				
//				//if the body is a parent...move children
//				RigidBodies.RigidBody_DX dbChild;
//
//				foreach (DictionaryEntry deEntry in m_aryChildBodies)
//				{
//					dbChild = (RigidBodies.RigidBody_DX)deEntry.Value;
//					if(dbChild.Direction.Z != 0)
//						dbChild.RelativeLocation += (fltdZ/2.0f) * dbChild.Direction;						
//				}//end foreach

				InvalidateBodyView();
			}//end set
		}

		public AnimatTools.Framework.ScaledNumber WidthScaled
		{
			get
			{
				m_snWidth.SetFromValue(this.Width*Util.Environment.DistanceUnitValue, (int) Util.Environment.DistanceUnits);
				return m_snWidth;
			}
			set 
			{
				if(value != null)
					this.Width = (float) value.ActualValue/Util.Environment.DistanceUnitValue;
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

		public AnimatTools.Framework.ScaledNumber LengthScaled
		{
			get
			{
				m_snLength.SetFromValue(this.Length*Util.Environment.DistanceUnitValue, (int) Util.Environment.DistanceUnits);
				return m_snLength;
			}
			set 
			{
				if(value != null)
					this.Length = (float) value.ActualValue/Util.Environment.DistanceUnitValue;
			}
		}

		public override float MaxDimension
		{
			get
			{
				float fltMax = (float) this.Length;

				if(this.Width > fltMax)
					fltMax = (float) this.Width;

				if(this.Height > fltMax)
					fltMax = (float) this.Height;

				return fltMax;
			}
		}

		public override float MinDimension
		{
			get
			{
				float fltMin = (float) this.Length;

				if(this.Width < fltMin)
					fltMin = (float) this.Width;

				if(this.Height < fltMin)
					fltMin = (float) this.Height;

				return fltMin;
			}
		}

		#endregion

		#region Methods

		public Box(AnimatTools.Framework.DataObject doParent) : base(doParent)
		{
			m_vGraphicsBoxSize = new Vec3d(this);
			m_vGraphicsBoxSize.X = m_vGraphicsBoxSize.Y = m_vGraphicsBoxSize.Z = 1;

			m_snWidth = new AnimatTools.Framework.ScaledNumber(this, "WidthScaled", "meters", "m");
			m_snHeight = new AnimatTools.Framework.ScaledNumber(this, "HeightScaled", "meters", "m");
			m_snLength = new AnimatTools.Framework.ScaledNumber(this, "LengthScaled", "meters", "m");

			m_vrSize = new DxBoxSizeRef(this);
		}

		public virtual void SetSize(float width, float height, float depth)
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

			if(m_mshLB != null)
			{
				this.m_mshLB.Dispose();
				this.m_mshLB = null;
			}

			m_vGraphicsBoxSize = new Vec3d(null);
			m_vGraphicsBoxSize.X = width;
			m_vGraphicsBoxSize.Y = height;
			m_vGraphicsBoxSize.Z = depth;

			m_d3dMesh = Util_DX.CreateBox(this.Device, width, height, depth);
			//m_d3dMesh = Mesh.Box(this.Device, width, height, depth);
			
			float fltScale = (this.MaxDimension * 0.15f);
			if(fltScale > 0.3f) fltScale = 0.3f;

			m_mshSelectedBoundingBox = Mesh.Box(Device, (float)(m_vGraphicsBoxSize.X + fltScale),(float)(m_vGraphicsBoxSize.Y + fltScale), (float)(m_vGraphicsBoxSize.Z + fltScale)); 
			RefreshSelectionBoxSizes();
		}

		
		protected override void IncreaseSizeZ(AnimatTools.Framework.MouseEventArgs AnimatMouseArgs)
		{		
			double dblOldLength = this.Length;
			try
			{
				this.Length = this.Length + (-AnimatMouseArgs.DeltaX * 0.005f) + (AnimatMouseArgs.DeltaY * 0.005f);
			}
			catch(Exception ex)
			{
				this.Length = dblOldLength;
			}
		}
		
		protected override void IncreaseSizeY(AnimatTools.Framework.MouseEventArgs AnimatMouseArgs)
		{		
			double dblOldHeight = this.Height;
			try
			{
				this.Height = this.Height + (AnimatMouseArgs.DeltaX * 0.005f) + (-AnimatMouseArgs.DeltaY * 0.005f);
			}
			catch(Exception ex)
			{
				this.Height = dblOldHeight;
			}
		}
		
		protected override void IncreaseSizeX(AnimatTools.Framework.MouseEventArgs AnimatMouseArgs)
		{			
			double dblOldWidth = this.Width;
			try
			{
				this.Width = this.Width + (AnimatMouseArgs.DeltaX * 0.005f) + (AnimatMouseArgs.DeltaY * 0.005f);
			}
			catch
			{
				this.Width = dblOldWidth;
			}
		}

		protected override void CreateBody()
		{
			SetSize((float) m_vGraphicsBoxSize.X, (float) m_vGraphicsBoxSize.Y, (float) m_vGraphicsBoxSize.Z);
			base.CreateBody();
		}

		protected override void CreateReceptiveFields()
		{
			if(this.Device != null && !this.Device.Disposed)
			{
				float fltReceptiveFieldDistance = (float) m_snReceptiveFieldDistance.ActualValue / Util.Environment.DistanceUnitValue;

				int iXDivs = (int) ((m_vGraphicsBoxSize.X/fltReceptiveFieldDistance) * 5);
				int iYDivs = (int) ((m_vGraphicsBoxSize.Y/fltReceptiveFieldDistance) * 5);
				int iZDivs = (int) ((m_vGraphicsBoxSize.Z/fltReceptiveFieldDistance) * 5);
				
				if(iXDivs<10) iXDivs = 10; 
				if(iYDivs<10) iYDivs = 10; 
				if(iZDivs<10) iZDivs = 10;

				if(iXDivs>20) iXDivs = 20; 
				if(iYDivs>20) iYDivs = 20; 
				if(iZDivs>20) iZDivs = 20;

				
				m_d3dReceptiveFieldMesh = Util_DX.CreateBox(this.Device, (float) m_vGraphicsBoxSize.X, (float) m_vGraphicsBoxSize.Y, (float) m_vGraphicsBoxSize.Z, iXDivs, iYDivs, iZDivs);
				m_d3dReceptiveFieldPoint = Mesh.Sphere(this.Device, 0.02f, 10, 5);

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

			float fltReceptiveFieldDistance = (float) m_snReceptiveFieldDistance.ActualValue / Util.Environment.DistanceUnitValue;

			int iXDivs = (int) Math.Round(this.Size.X/fltReceptiveFieldDistance);
			int iYDivs = (int) Math.Round(this.Size.Y/fltReceptiveFieldDistance);
			int iZDivs = (int) Math.Round(this.Size.Z/fltReceptiveFieldDistance);

			if(iXDivs<=0) iXDivs = 1;
			if(iYDivs<=0) iYDivs = 1;
			if(iZDivs<=0) iZDivs = 1;

			float fltW = (float) this.Size.X, fltH = (float) this.Size.Y, fltL = (float) this.Size.Z;
			float fltXDiv = (float) this.Size.X/iXDivs;
			float fltYDiv = (float) this.Size.Y/iYDivs;
			float fltZDiv = (float) this.Size.Z/iZDivs;
			int iXDiv, iYDiv, iZDiv;
			float x1, y1, z1, x2, y2, z2;
			float half = 0.5f;

			//Lets get the top and bottom panes of the box
			y1 = -fltH*half;	y2 = fltH*half;
			for(iXDiv=0; iXDiv<=iXDivs; iXDiv++)
			{
				x1 = (-fltW*half) + (fltXDiv*iXDiv);

				for(iZDiv=0; iZDiv<=iZDivs; iZDiv++)
				{
					z1 = (-fltL*half) + (fltZDiv*iZDiv);

					m_aryReceptiveFields.Add(new AnimatTools.Framework.Vec3d(null, x1, y1, z1));
					m_aryReceptiveFields.Add(new AnimatTools.Framework.Vec3d(null, x1, y2, z1));
					//AddReceptiveField(x1, y1, z1);
					//AddReceptiveField(x1, y2, z1);
				}
			}

			//Lets get the left and right panes of the box
			x1 = -fltW*half;	x2 = fltW*half;
			for(iYDiv=0; iYDiv<=iYDivs; iYDiv++)
			{
				y1 = (-fltH*half) + (fltYDiv*iYDiv);

				for(iZDiv=0; iZDiv<=iZDivs; iZDiv++)
				{
					z1 = (-fltL*half) + (fltZDiv*iZDiv);

					m_aryReceptiveFields.Add(new AnimatTools.Framework.Vec3d(null, x1, y1, z1));
					m_aryReceptiveFields.Add(new AnimatTools.Framework.Vec3d(null, x2, y1, z1));
					//AddReceptiveField(x1, y1, z1);
					//AddReceptiveField(x2, y1, z1);
				}
			}

			//Lets get the front and back panes of the box
			z1 = -fltL*half;	z2 = fltL*half;
			for(iXDiv=0; iXDiv<=iXDivs; iXDiv++)
			{
				x1 = (-fltW*half) + (fltXDiv*iXDiv);

				for(iYDiv=0; iYDiv<=iYDivs; iYDiv++)
				{
					y1 = (-fltH*half) + (fltYDiv*iYDiv);

					m_aryReceptiveFields.Add(new AnimatTools.Framework.Vec3d(null, x1, y1, z1));
					m_aryReceptiveFields.Add(new AnimatTools.Framework.Vec3d(null, x1, y1, z2));
					//AddReceptiveField(x1, y1, z1);
					//AddReceptiveField(x1, y1, z2);
				}
			}

			SortReceptiveFields();
			VerifyReceptiveFielPairs();
		}
				
		public override void CalculateBodyExtent(ref float iMaxX, ref float iMinX, ref float iMaxY, ref float iMinY)
		{
			float fTmpMaxX = (float)this.m_vGraphicsBoxSize.X/2 + this.XLocalLocation;
			float fTmpMinX = (float)this.m_vGraphicsBoxSize.X/2 - this.XLocalLocation;
			float fTmpMaxY = (float)this.m_vGraphicsBoxSize.Y/2 + this.YLocalLocation;
			float fTmpMinY = (float)this.m_vGraphicsBoxSize.Y/2 - this.YLocalLocation;
			
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
		{return new Box(doParent);}

		protected override void CloneInternal(AnimatTools.Framework.DataObject doOriginal, bool bCutData, AnimatTools.Framework.DataObject doRoot)
		{
			base.CloneInternal (doOriginal, bCutData, doRoot);

			Box doOrigBody = (Box) doOriginal;
			m_vGraphicsBoxSize = new Vec3d(this, doOrigBody.m_vGraphicsBoxSize.X, doOrigBody.m_vGraphicsBoxSize.Y, doOrigBody.m_vGraphicsBoxSize.Z);

			m_snHeight = (AnimatTools.Framework.ScaledNumber) doOrigBody.m_snHeight.Clone(this, bCutData, doRoot);
			m_snWidth = (AnimatTools.Framework.ScaledNumber) doOrigBody.m_snWidth.Clone(this, bCutData, doRoot);
			m_snLength = (AnimatTools.Framework.ScaledNumber) doOrigBody.m_snLength.Clone(this, bCutData, doRoot);
		}

		public override AnimatTools.Framework.DataObject Clone(AnimatTools.Framework.DataObject doParent, bool bCutData, AnimatTools.Framework.DataObject doRoot)
		{
			Box bnPart = new Box(doParent);
			bnPart.CloneInternal(this, bCutData, doRoot);
			if(doRoot != null && doRoot == this) bnPart.AfterClone(this, bCutData, doRoot, bnPart);
			return bnPart;
		}

		protected override void BuildProperties()
		{
			base.BuildProperties();
			
//      m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Size", this.Size.GetType(), "Size", 
//                                  "Part Properties", "The size of the box.", this.Size, 
//                                  "", typeof(AnimatTools.TypeHelpers.Vec3dTypeConverter)));
			
			Crownwood.Magic.Controls.PropertyBag pbNumberBag  = this.HeightScaled.Properties;
			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Height", pbNumberBag.GetType(), "HeightScaled", 
																	"Size", "Sets the height of the box.", pbNumberBag,
																	"", typeof(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)));

			pbNumberBag  = this.WidthScaled.Properties;
			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Width", pbNumberBag.GetType(), "WidthScaled", 
																	"Size", "Sets the width of the box.", pbNumberBag,
																	"", typeof(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)));

			pbNumberBag  = this.LengthScaled.Properties;
			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Length", pbNumberBag.GetType(), "LengthScaled", 
																	"Size", "Sets the length of the box.", pbNumberBag,
																	"", typeof(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)));
		}

		#region Loading and Saving Data

		public override void LoadData(ref AnimatTools.DataObjects.Simulation dsSim, ref AnimatTools.DataObjects.Physical.PhysicalStructure doStructure, ref AnimatTools.Interfaces.StdXml oXml)
		{
			base.LoadData (ref dsSim, ref doStructure, ref oXml);

			oXml.IntoElem();
			m_vGraphicsBoxSize = Util.LoadVec3d(ref oXml, "GraphicsBoxSize", this);	
			oXml.OutOfElem(); //out of body			

		}

		public override void SaveData(ref AnimatTools.DataObjects.Simulation dsSim, ref AnimatTools.DataObjects.Physical.PhysicalStructure doStructure, ref AnimatTools.Interfaces.StdXml oXml)
		{
			base.SaveData (ref dsSim, ref doStructure, ref oXml);

			oXml.IntoElem();
			Util.SaveVector(ref oXml, "CollisionBoxSize", new Vec3d(null, m_vGraphicsBoxSize.X, m_vGraphicsBoxSize.Y, m_vGraphicsBoxSize.Z));
			Util.SaveVector(ref oXml, "GraphicsBoxSize", new Vec3d(null, m_vGraphicsBoxSize.X, m_vGraphicsBoxSize.Y, m_vGraphicsBoxSize.Z));

			oXml.OutOfElem(); //out of body			

		}

		#endregion

		#endregion
	}
}
