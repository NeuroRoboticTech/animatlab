using System;
using System.Drawing;
using System.Windows.Forms;
using Microsoft.DirectX;
using Microsoft.DirectX.Direct3D;
using AnimatTools;
using AnimatTools.Framework;
using VortexAnimatTools.DataObjects.Physical.PropertyHelpers;

namespace VortexAnimatTools.DataObjects.Physical.Joints
{
	/// <summary>
	/// Summary description for Joint_DX.
	/// </summary>
	public abstract class Joint_DX : AnimatTools.DataObjects.Physical.Joint
	{

		#region Attributes
		protected Microsoft.DirectX.Direct3D.Device m_d3dDevice;   //device used by the mesh
		protected Microsoft.DirectX.Direct3D.Mesh m_d3dMesh;		// the actual mesh
		protected Microsoft.DirectX.Direct3D.Material m_d3dMaterial; //material of the object
		protected Microsoft.DirectX.Direct3D.Material m_d3dSelectedMaterial;
		protected Microsoft.DirectX.Direct3D.Material m_d3dTransparentMaterial;

		protected DxLocationRef m_vrLocation;
		protected DxRotationRef m_vrRotation;
		protected Microsoft.DirectX.Vector3 m_v3Rotation;
		
		protected Microsoft.DirectX.Direct3D.IntersectInformation m_iiIntersectInfo;

		protected float m_fRadius = 0.05f;
		protected AnimatTools.Framework.ScaledNumber m_snRadius;

		protected Mesh m_mshSelectedBoundingBox;
		protected Material m_matTransparentSelectedMaterial;

		protected Mesh m_mshCB;

		protected bool m_bCBSelected;
		protected float m_fltSelBoxSize = 0;

		protected Matrix m_mtxTranslation = Matrix.Identity;
		protected Matrix m_mtxOrientation = Matrix.Identity;
		
		protected AnimatTools.Framework.ScaledNumber m_snXLocalLocation;
		protected AnimatTools.Framework.ScaledNumber m_snYLocalLocation;
		protected AnimatTools.Framework.ScaledNumber m_snZLocalLocation;

		protected AnimatTools.Framework.ScaledNumber m_snXWorldLocation;
		protected AnimatTools.Framework.ScaledNumber m_snYWorldLocation;
		protected AnimatTools.Framework.ScaledNumber m_snZWorldLocation;

		protected AnimatTools.Framework.ScaledNumber m_snXRotation;
		protected AnimatTools.Framework.ScaledNumber m_snYRotation;
		protected AnimatTools.Framework.ScaledNumber m_snZRotation;

		private Mesh m_mshAxisCylinder;													//Cylinder part of Axis for orientation
		private Mesh m_mshAxisCone;														//Cone part of Axis for orientation (to make an arrow)
		private Mesh m_mshAxisLabel;													//Label for which axis it is
		private Material m_matXAxis;													//Material for the x axis
		private Material m_matYAxis;													//Material for the y axis
		private Material m_matZAxis;													//Material for the z axis

		private Texture texX = null;													//Texture for the x axis label
		private Texture texY = null;													//Texture for the y axis label
		private Texture texZ = null;													//Texture for the z axis label

		protected Texture texCB = null;

		#endregion

		#region Properties

		public virtual Microsoft.DirectX.Direct3D.Device Device {get{return m_d3dDevice;}set{m_d3dDevice = value;}}
		/// <summary>
		/// Retrieves or Sets the location of the body in world space
		/// </summary>
		
		
		/// <summary>
		/// Gets or Sets the rotation matrix for this body
		/// </summary>
		public virtual Microsoft.DirectX.Matrix Orientation
		{
			get{return m_mtxOrientation;}
			set{m_mtxOrientation = value;}
		}

		/// <summary>
		/// Gets or Sets the translation matrix for this joint
		/// </summary>
		public Microsoft.DirectX.Matrix Translation
		{
			get{return m_mtxTranslation;}
			set{m_mtxTranslation = value;}
		}
		
		/// <summary>
		/// Retuurns the rotation matrix * translation matrix of this joint
		/// </summary>
		public Microsoft.DirectX.Matrix TransformationMatrix
		{
			get
			{return Orientation * Translation;}			
		}
		
		/// <summary>
		/// Returns the combined transformation matrix of this joint and all the bodies back to the root
		/// </summary>		
		public Microsoft.DirectX.Matrix CombinedTransformationMatrix
		{
			get	{return TransformationMatrix * Parent_DX.CombinedTransformationMatrix;}
		}

		public Microsoft.DirectX.Direct3D.IntersectInformation IntersectInfo
		{
			get
			{
				return this.m_iiIntersectInfo;
			}
		}

		public override float IntersectionDistance
		{
			get
			{
				return this.m_iiIntersectInfo.Dist;
			}
		}

		#region Location Properties

		/// <summary>
		/// Gets or Sets the relative location of the body in reference to it's parent body
		/// </summary>
		public virtual Microsoft.DirectX.Vector3 RelativeLocation
		{
			get
			{
				Vector3 v3Loc = new Vector3(Translation.M41, Translation.M42, Translation.M43);
				return v3Loc;				
			}
			set
			{
				m_mtxTranslation.M41 = value.X;
				m_mtxTranslation.M42 = value.Y;
				m_mtxTranslation.M43 = value.Z;
			}
		}

		/// <summary>
		/// Gets the absolute location of this body in world space
		/// </summary>
		public virtual Microsoft.DirectX.Vector3 AbsoluteLocation
		{
			get
			{
				//decompose the matrix to get the translation matrix which is x = M41, y = M42, and z = M=43
				return new Vector3(CombinedTransformationMatrix.M41, CombinedTransformationMatrix.M42, CombinedTransformationMatrix.M43);							
			}
		}

		public virtual Microsoft.DirectX.Vector3 DxLocation 
		{
			get
			{
				if(!(this.Parent is AnimatTools.DataObjects.Physical.PhysicalStructure))
				{
					RigidBodies.RigidBody_DX parent = (RigidBodies.RigidBody_DX)Parent;
					Vector3 v3Local = new Vector3(m_mtxTranslation.M41, m_mtxTranslation.M42, m_mtxTranslation.M43);
					return v3Local + parent.DxLocation;
				}
				else
					return new Vector3();
			}
			set
			{
				//get the parents combined transformation matrix
				Matrix mtxParent = this.Parent_DX.CombinedTransformationMatrix;

				//get the inverse of the parent's combined transformation matrix
				Matrix mtxParentInv = Util_DX.CloneMatrix(mtxParent);
				mtxParentInv.Invert();

				//Create a mouse matrix
				Matrix mtxT = mtxParent * Matrix.Translation(value) * mtxParentInv;

				m_mtxTranslation = Matrix.Translation(value);
			}
		}
		
		public override AnimatTools.Framework.Vec3d Location 
		{
			get
			{
				return m_vrLocation;
			}
			set
			{
				if(value != null)
				{
					m_mtxTranslation.M41 = (float)value.X;
					m_mtxTranslation.M42 = (float)value.Y;
					m_mtxTranslation.M43 = (float)value.Z;

					InvalidateBodyView();
				}
			}
		}

		#region Local Location Properties

		public virtual Microsoft.DirectX.Vector3 LocalLocation
		{
			get
			{
				Vector3 v3Loc = new Vector3(Translation.M41, Translation.M42, Translation.M43);
				return v3Loc;				
			}
			set
			{
				m_mtxTranslation.M41 = value.X;
				m_mtxTranslation.M42 = value.Y;
				m_mtxTranslation.M43 = value.Z;
			}

		}

		/// <summary>
		/// Gets or Sets the X component of the this body's location
		/// </summary>
		public override float XLocalLocation
		{
			get{return m_mtxTranslation.M41;}
			set
			{			
				m_mtxTranslation.M41 = value;
				InvalidateBodyViewAndProperties();
			}
		}

		/// <summary>
		/// Gets or Sets the Y component of the this body's location
		/// </summary>
		public override float YLocalLocation 
		{
			get{return m_mtxTranslation.M42;}
			set 
			{
				m_mtxTranslation.M42 = value;
				InvalidateBodyViewAndProperties();
			}
		}

		/// <summary>
		/// Gets or Sets the Z component of the this body's location
		/// </summary>
		public override float ZLocalLocation 
		{
			get{return m_mtxTranslation.M43;}
			set 
			{
				m_mtxTranslation.M43 = value;
				InvalidateBodyViewAndProperties();
			}
		}

		public override AnimatTools.Framework.ScaledNumber XLocalLocationScaled
		{
			get
			{
				m_snXLocalLocation.SetFromValue(this.XLocalLocation*Util.Environment.DistanceUnitValue, (int) Util.Environment.DistanceUnits);
				return m_snXLocalLocation;
			}
			set 
			{
				if(value != null)
					this.XLocalLocation = (float) value.ActualValue/Util.Environment.DistanceUnitValue;
			}
		}

		public override AnimatTools.Framework.ScaledNumber YLocalLocationScaled
		{
			get
			{
				m_snYLocalLocation.SetFromValue(this.YLocalLocation*Util.Environment.DistanceUnitValue, (int) Util.Environment.DistanceUnits);
				return m_snYLocalLocation;
			}
			set 
			{
				if(value != null)
					this.YLocalLocation = (float) value.ActualValue/Util.Environment.DistanceUnitValue;
			}
		}

		public override AnimatTools.Framework.ScaledNumber ZLocalLocationScaled
		{
			get
			{
				m_snZLocalLocation.SetFromValue(this.ZLocalLocation*Util.Environment.DistanceUnitValue, (int) Util.Environment.DistanceUnits);
				return m_snZLocalLocation;
			}
			set 
			{
				if(value != null)
					this.ZLocalLocation = (float) value.ActualValue/Util.Environment.DistanceUnitValue;
			}
		}

		#endregion

		#region World Location Properties

		public Microsoft.DirectX.Vector3 Worldlocation
		{
			get
			{
				return this.AbsoluteLocation;
			}
			set
			{			
				if(Parent_DX != null)
				{
					//get the parents combined transformation matrix
					Matrix mtxParent = this.Parent_DX.CombinedTransformationMatrix;

					//get the inverse of the parent's combined transformation matrix
					Matrix mtxParentInv = Util_DX.CloneMatrix(mtxParent);
					mtxParentInv.Invert();

					//Create a mouse matrix
					Matrix mtxT = mtxParentInv * Matrix.Translation(value.X, value.Y, value.Z) * mtxParent;

					//Apply mtxT to the translation matrix		
					m_mtxTranslation = mtxT;
				}
				
				InvalidateBodyViewAndProperties();
			}
		}

		/// <summary>
		/// Gets or Sets the X component of the this body's location
		/// </summary>
		public override float XWorldLocation
		{
			get
			{
				//Matrix mtxT = GetBodyWorldMatrix();
				return this.CombinedTransformationMatrix.M41;
			}
			set
			{	
				Vector3 v3AbsLoc = new Vector3(value, YWorldLocation, ZWorldLocation);
				Vector3 v3RelLoc = new Vector3();

				Matrix invParent = Parent_DX.CombinedTransformationMatrix;
				invParent.Invert();
				v3RelLoc = Vector3.TransformCoordinate(v3AbsLoc, invParent);
				XLocalLocation = v3RelLoc.X;
				YLocalLocation = v3RelLoc.Y;
				ZLocalLocation = v3RelLoc.Z;

				InvalidateBodyViewAndProperties();
			}
		}

		/// <summary>
		/// Gets or Sets the Y component of the this body's location
		/// </summary>
		public override float YWorldLocation 
		{
			get
			{
				//Matrix mtxT = GetBodyWorldMatrix();
				return this.CombinedTransformationMatrix.M42;
			}
			set 
			{
				Vector3 v3AbsLoc = new Vector3(XWorldLocation,value, ZWorldLocation);
				Vector3 v3RelLoc = new Vector3();

				Matrix invParent = Parent_DX.CombinedTransformationMatrix;
				invParent.Invert();
				v3RelLoc = Vector3.TransformCoordinate(v3AbsLoc, invParent);
				XLocalLocation = v3RelLoc.X;
				YLocalLocation = v3RelLoc.Y;
				ZLocalLocation = v3RelLoc.Z;

				InvalidateBodyViewAndProperties();
			}
		}

		/// <summary>
		/// Gets or Sets the Z component of the this body's location
		/// </summary>
		public override float ZWorldLocation 
		{
			get
			{
				//				Matrix mtxT = GetBodyWorldMatrix();
				return this.CombinedTransformationMatrix.M43;
			}
			set 
			{
				Vector3 v3AbsLoc = new Vector3(XWorldLocation, YWorldLocation, value);
				Vector3 v3RelLoc = new Vector3();

				Matrix invParent = Parent_DX.CombinedTransformationMatrix;
				invParent.Invert();
				v3RelLoc = Vector3.TransformCoordinate(v3AbsLoc, invParent);
				XLocalLocation = v3RelLoc.X;
				YLocalLocation = v3RelLoc.Y;
				ZLocalLocation = v3RelLoc.Z;

				InvalidateBodyViewAndProperties();		
			}
		}

		public override AnimatTools.Framework.ScaledNumber XWorldLocationScaled
		{
			get
			{
				m_snXWorldLocation.SetFromValue(this.XWorldLocation*Util.Environment.DistanceUnitValue, (int) Util.Environment.DistanceUnits);
				return m_snXWorldLocation;
			}
			set 
			{
				if(value != null)
					this.XWorldLocation = (float) value.ActualValue/Util.Environment.DistanceUnitValue;
			}
		}

		public override AnimatTools.Framework.ScaledNumber YWorldLocationScaled
		{
			get
			{
				m_snYWorldLocation.SetFromValue(this.YWorldLocation*Util.Environment.DistanceUnitValue, (int) Util.Environment.DistanceUnits);
				return m_snYWorldLocation;
			}
			set 
			{
				if(value != null)
					this.YWorldLocation = (float) value.ActualValue/Util.Environment.DistanceUnitValue;
			}
		}

		public override AnimatTools.Framework.ScaledNumber ZWorldLocationScaled
		{
			get
			{
				m_snZWorldLocation.SetFromValue(this.ZWorldLocation*Util.Environment.DistanceUnitValue, (int) Util.Environment.DistanceUnits);
				return m_snZWorldLocation;
			}
			set 
			{
				if(value != null)
					this.ZWorldLocation = (float) value.ActualValue/Util.Environment.DistanceUnitValue;
			}
		}

		#endregion

		#endregion

		#region Rotation Properties

		/// <summary>
		/// Gets or Sets the degrees of rotation around the X axis of this body
		/// </summary>
		public override float XRotation 
		{
			get{return Util.RadiansToDegrees(m_v3Rotation.X);}
			set 
			{
				if(value <-180 || value > 180)
					throw new System.Exception("The rotation angle must be between -180 and 180 degrees.");

				m_v3Rotation.X = Util.DegreesToRadians(value);
				m_mtxOrientation = Matrix.RotationX(m_v3Rotation.X) * Matrix.RotationY(m_v3Rotation.Y) * Matrix.RotationZ(m_v3Rotation.Z);
				InvalidateBodyView();
			}
		}

		/// <summary>
		/// Gets or Sets the degrees of rotation around the Y axis of this body
		/// </summary>
		public override float YRotation 
		{
			get{return Util.RadiansToDegrees(m_v3Rotation.Y);}
			set 
			{
				if(value <-180 || value > 180)
					throw new System.Exception("The rotation angle must be between -180 and 180 degrees.");

				m_v3Rotation.Y = Util.DegreesToRadians(value);				
				m_mtxOrientation = Matrix.RotationX(m_v3Rotation.X) * Matrix.RotationY(m_v3Rotation.Y) * Matrix.RotationZ(m_v3Rotation.Z);
				
				InvalidateBodyView();
			}
		}
		
		/// <summary>
		/// Gets or Sets the degrees of rotation around the Z axis of this body
		/// </summary>
		public override float ZRotation 
		{
			get{return Util.RadiansToDegrees(m_v3Rotation.Z);}
			set 
			{
				if(value <-180 || value > 180)
					throw new System.Exception("The rotation angle must be between -180 and 180 degrees.");

				m_v3Rotation.Z = Util.DegreesToRadians(value);
				m_mtxOrientation = Matrix.RotationX(m_v3Rotation.X) * Matrix.RotationY(m_v3Rotation.Y) * Matrix.RotationZ(m_v3Rotation.Z);
						
				InvalidateBodyView();
			}
		}

		public override AnimatTools.Framework.ScaledNumber XRotationScaled
		{
			get
			{
				m_snXRotation.SetFromValue(this.XRotation, 0);
				return m_snXRotation;
			}
			set 
			{
				if(value != null)
					this.XRotation = (float) value.ActualValue;
			}
		}

		public override AnimatTools.Framework.ScaledNumber YRotationScaled
		{
			get
			{
				m_snYRotation.SetFromValue(this.YRotation, 0);
				return m_snYRotation;
			}
			set 
			{
				if(value != null)
					this.YRotation = (float) value.ActualValue;
			}
		}

		public override AnimatTools.Framework.ScaledNumber ZRotationScaled
		{
			get
			{
				m_snZRotation.SetFromValue(this.ZRotation, 0);
				return m_snZRotation;
			}
			set 
			{
				if(value != null)
					this.ZRotation = (float) value.ActualValue;
			}
		}
		
		public virtual Microsoft.DirectX.Vector3 DxRotation
		{
			get
			{
				return m_v3Rotation;
			}
			set{m_v3Rotation = value;}
		}
		
		public override AnimatTools.Framework.Vec3d Rotation 
		{
			get
			{
				return m_vrRotation;
			}
			set
			{
				if(value != null)
				{
					m_v3Rotation.X = (float) Util.DegreesToRadians((float) value.X);
					m_v3Rotation.Y = (float) Util.DegreesToRadians((float) value.Y);
					m_v3Rotation.Z = (float) Util.DegreesToRadians((float) value.Z);

					m_mtxOrientation = Matrix.RotationX(m_v3Rotation.X) * Matrix.RotationY(m_v3Rotation.Y) * Matrix.RotationZ(m_v3Rotation.Z);
					
					InvalidateBodyView();
				}
			}
		}

		#endregion

		public override AnimatTools.Framework.Vec3d Size 
		{
			get {return new AnimatTools.Framework.Vec3d(null, m_fRadius, 0, 0);} 
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

		public float Radius 
		{
			get{return m_fRadius;}
			set 
			{
				if(value <= 0)
					throw new System.Exception("The radius of the joint can not be less than or equal to zero.");
				
				m_fRadius = value;
				CreateBody();
				InvalidateBodyView();
			}
		}

		public AnimatTools.Framework.ScaledNumber RadiusScaled
		{
			get
			{
				m_snRadius.SetFromValue(this.Radius, (int) Util.Environment.DistanceUnits);
				return m_snRadius;
			}
			set 
			{
				if(value != null)
					this.Radius = (float) value.ActualValue;
			}
		}

		public override float MaxDimension {get{return this.Radius;}}
		public override float MinDimension {get{return this.Radius;}}

		public RigidBodies.RigidBody_DX Parent_DX
		{
			get
			{
				RigidBodies.RigidBody_DX rbParent = (RigidBodies.RigidBody_DX)this.Parent;
				return rbParent;
			}
		}

		public override AnimatTools.DataObjects.Physical.PartPositionInfo PartPositionInfo
		{
			get
			{
				VortexAnimatTools.DataObjects.Physical.PartPositionInfo biPosition = new VortexAnimatTools.DataObjects.Physical.PartPositionInfo();
				biPosition.m_strID = m_strID;
				biPosition.m_BodyType = this.GetType();

				biPosition.m_v3Rotation = new Microsoft.DirectX.Vector3(m_v3Rotation.X, m_v3Rotation.Y, m_v3Rotation.Z);
				biPosition.m_vSize = new AnimatTools.Framework.Vec3d(null, m_fRadius, 0, 0);

				biPosition.m_mtxTranslation = Util_DX.CloneMatrix(m_mtxTranslation);

				return biPosition;
			}
			set
			{
				if(value != null && value.m_strID == m_strID)
				{
					VortexAnimatTools.DataObjects.Physical.PartPositionInfo piPos = (VortexAnimatTools.DataObjects.Physical.PartPositionInfo) value;

					m_v3Rotation = new Microsoft.DirectX.Vector3(piPos.m_v3Rotation.X, piPos.m_v3Rotation.Y, piPos.m_v3Rotation.Z);
					m_fRadius = (float) piPos.m_vSize.X;
					m_mtxTranslation = Util_DX.CloneMatrix(piPos.m_mtxTranslation);
				}
			}
		}

		public System.Drawing.Color Color 
		{
			get
			{return m_d3dMaterial.Ambient;}
			set
			{
				m_d3dMaterial.Ambient = value;
				m_d3dMaterial.Diffuse = value;

				m_d3dTransparentMaterial.Ambient = Color.FromArgb(128,value);
				m_d3dTransparentMaterial.Diffuse = Color.FromArgb(128,value);

			}
		}

		public override System.Drawing.Image Image
		{   
			get
			{
				if(m_Image == null)
				{
					string [] aryImage = this.ImageName.Split('.');
					System.Reflection.Assembly myAssembly = System.Reflection.Assembly.Load(aryImage[0]);

					m_Image = ImageManager.LoadImage(ref myAssembly, this.ImageName, true);
				}

				return m_Image;
			}
		}

		public override System.Drawing.Image DragImage
		{   
			get
			{
				if(m_DragImage == null)
				{
					string [] aryImage = this.ImageName.Split('.');
					System.Reflection.Assembly myAssembly = System.Reflection.Assembly.Load(aryImage[0]);

					m_DragImage = ImageManager.LoadImage(ref myAssembly, this.DragImageName, true);
				}

				return m_DragImage;
			}
		}

		public override bool Selected
		{
			get{return base.Selected;}
			set
			{
				base.Selected = value;
				if(m_bSelected)
					RefreshSelectionBoxSizes();
			}
		}
		
		public virtual bool NeedToCreateBody
		{
			get
			{
				if(this.Device != null && (m_d3dMesh == null || m_d3dMesh.Disposed || m_mshSelectedBoundingBox == null || m_mshSelectedBoundingBox.Disposed || m_mshCB == null || m_mshCB.Disposed))
					return true;
				else
					return false;
			}
		}

		public override String DataColumnModuleName {get{return "";}}
		public override String DataColumnClassType {get{return "JointData";}}

		#endregion
		
		#region Methods

		public Joint_DX(AnimatTools.Framework.DataObject doParent) : base(doParent)
		{
			m_d3dSelectedMaterial = new Material();
			m_d3dSelectedMaterial.Diffuse = Color.Yellow;
			m_d3dSelectedMaterial.Ambient = Color.Yellow;

			this.m_matTransparentSelectedMaterial = new Material();
			this.m_matTransparentSelectedMaterial.Diffuse = Color.FromArgb(128,Color.FloralWhite);
			this.m_matTransparentSelectedMaterial.Ambient = Color.FromArgb(128,Color.FloralWhite);

			m_fRadius = 0.05f;

			m_vrLocation = new DxLocationRef(this);
			m_vrRotation = new DxRotationRef(this);

			m_snRadius = new AnimatTools.Framework.ScaledNumber(this, "RadiusScaled", "meters", "m");

			m_snXLocalLocation = new AnimatTools.Framework.ScaledNumber(this, "XLocalLocationScaled", "meters", "m");
			m_snYLocalLocation = new AnimatTools.Framework.ScaledNumber(this, "YLocalLocationScaled", "meters", "m");
			m_snZLocalLocation = new AnimatTools.Framework.ScaledNumber(this, "ZLocalLocationScaled", "meters", "m");

			m_snXWorldLocation = new AnimatTools.Framework.ScaledNumber(this, "XWorldLocationScaled", "meters", "m");
			m_snYWorldLocation = new AnimatTools.Framework.ScaledNumber(this, "YWorldLocationScaled", "meters", "m");
			m_snZWorldLocation = new AnimatTools.Framework.ScaledNumber(this, "ZWorldLocationScaled", "meters", "m");

			m_snXRotation = new AnimatTools.Framework.ScaledNumber(this, "XRotationScaled", "Degrees", "Deg");
			m_snYRotation = new AnimatTools.Framework.ScaledNumber(this, "YRotationScaled", "Degrees", "Deg");
			m_snZRotation = new AnimatTools.Framework.ScaledNumber(this, "ZRotationScaled", "Degrees", "Deg");
		}

		public virtual void DrawBoundingBox()
		{
			if(m_mshSelectedBoundingBox != null)
			{
				if(texCB == null)
				{
					try
					{
						System.Reflection.Assembly myAssembly = System.Reflection.Assembly.Load("VortexAnimatTools");
						System.IO.Stream stream = myAssembly.GetManifestResourceStream("VortexAnimatTools.Graphics.ikpoint0.png");
					
						//load the texture for the selection boxes
						//texCB = TextureLoader.FromFile(Device,"VortexAnimatTools.ikpoint0.png");	

						//load texture from stream
						texCB = TextureLoader.FromStream(Device,stream);
					
					}
					catch(Exception ex)
					{
						Util.DisplayError(ex);
					}
				}

				Matrix mWorld = this.Device.Transform.World;
				//set the material for the selection box
				Device.Material = this.m_matTransparentSelectedMaterial;

				//set the world matrix
				this.Device.Transform.World = this.CombinedTransformationMatrix * this.Device.Transform.World1;  //moves the body to where it is in space
			
				//draw the bounding box
				this.m_mshSelectedBoundingBox.DrawSubset(0);

				Vector3 tmp = new Vector3();
				tmp.TransformCoordinate(CombinedTransformationMatrix * Device.Transform.World1);
		
				Matrix m = this.Device.Transform.View;
				m.Invert();
				m.M41 = tmp.X;
				m.M42 = tmp.Y;
				m.M43 = tmp.Z;
				this.Device.Transform.World = m;
				this.Device.Material = Util_DX.WhiteMaterial();

				this.Device.RenderState.ZBufferEnable = false;
				Device.SetTexture(0,texCB);
				m_mshCB.DrawSubset(0);
				Device.SetTexture(0,null);
				this.Device.RenderState.ZBufferEnable = true;
			}			
		}

		public override void Draw(AnimatTools.Forms.BodyPlan.Command.enumCommandMode cmCommand)
		{
			if(this.NeedToCreateBody) //if the mesh is null create it  with the loaded information
				CreateBody();

			//If a mesh was created then draw it.
			//if(m_d3dMesh != null && this.Device != null)
			if(CanDraw())
			{
				
				this.Device.Transform.World = CombinedTransformationMatrix  * Device.Transform.World1;
				
				if(cmCommand != AnimatTools.Forms.BodyPlan.Command.enumCommandMode.SelectJoints)
				{
					this.Device.Material = this.m_d3dTransparentMaterial;
					m_bSelected = false;
				}
				else
					this.Device.Material = m_d3dMaterial;

				try
				{	
					DrawParts();					
				}
				catch(Exception ex)
				{
					try
					{
						if(m_d3dMesh.Disposed)
						{
							m_d3dMesh.Dispose();
							m_d3dMesh = null;
							this.Draw(cmCommand);
						}
					}
					catch(Exception ex2)
					{}
				}//end try catch 1
			}//end if(m_d3dMesh != null)					
		}//end Draw

		protected virtual bool CanDraw()
		{
			if(this.Device != null && m_d3dMesh != null)				
				return true;
			else
				return false;
		}

		public virtual void DrawParts()
		{
			m_d3dMesh.DrawSubset(0);  //draws the object
			if(m_bSelected)
			{
				DrawAxis();
				this.DrawBoundingBox();
			}
		}

		protected void DrawAxis()
		{
			Matrix mtxWorld = Matrix.Identity;

			mtxWorld = Translation * this.Parent_DX.CombinedTransformationMatrix;
			

			float fltScale = ((this.MinDimension + this.MaxDimension)/2 * 0.15f);
			if(fltScale > 0.1f) fltScale = 0.1f;

			//Draw X Axis			
			m_d3dDevice.Material = m_matXAxis;
			m_d3dDevice.Transform.World = Matrix.RotationY(Geometry.DegreeToRadian(-90)) * Matrix.Translation(10 * fltScale, 0.0f, 0.0f) * mtxWorld * Device.Transform.World1;				
			m_mshAxisCylinder.DrawSubset(0);
			m_d3dDevice.Transform.World = Matrix.RotationY(Geometry.DegreeToRadian(-90)) * Matrix.Translation(20 * fltScale, 0.0f, 0.0f) * mtxWorld * Device.Transform.World1;
			m_mshAxisCone.DrawSubset(0);
			
			//Draw Y Axis
			m_d3dDevice.Material = m_matYAxis;
			m_d3dDevice.Transform.World = Matrix.RotationX(Geometry.DegreeToRadian(-90)) * Matrix.Translation(0.0f, 10 * fltScale, 0.0f) * mtxWorld * Device.Transform.World1;
			m_mshAxisCylinder.DrawSubset(0);
			m_d3dDevice.Transform.World = Matrix.RotationX(Geometry.DegreeToRadian(90)) * Matrix.Translation(0.0f, 20 * fltScale, 0.0f) * mtxWorld * Device.Transform.World1;
			m_mshAxisCone.DrawSubset(0);
			
			//Draw Z Axis
			m_d3dDevice.Material = m_matZAxis;
			m_d3dDevice.Transform.World =  Matrix.Translation(0.0f, 0.0f, 10 * fltScale) * mtxWorld * Device.Transform.World1;
			m_mshAxisCylinder.DrawSubset(0);
			m_d3dDevice.Transform.World = Matrix.RotationY(Geometry.DegreeToRadian(180)) * Matrix.Translation(0.0f, 0.0f, 20 * fltScale) * mtxWorld * Device.Transform.World1;
			m_mshAxisCone.DrawSubset(0);

			//Draw X axis label
			m_d3dDevice.Transform.World = Matrix.Translation(25 * fltScale, 0.0f, 0.0f) * mtxWorld * Device.Transform.World1;
			Vector3 tmp = new Vector3();
			tmp.TransformCoordinate(m_d3dDevice.Transform.World);
		
			Matrix m =m_d3dDevice.Transform.View;
			m.Invert();
			m.M41 = tmp.X;
			m.M42 = tmp.Y;
			m.M43 = tmp.Z;

			m_d3dDevice.Transform.World = m;

			Device.Material = Util_DX.WhiteMaterial();
			Device.SetTexture(0,texX);
			m_mshAxisLabel.DrawSubset(0);
			Device.SetTexture(0,null);

			//Draw Y Axis Label
			m_d3dDevice.Transform.World = Matrix.Translation(0.0f, 25 * fltScale, 0.0f) * mtxWorld * Device.Transform.World1;
			tmp = new Vector3();
			tmp.TransformCoordinate(m_d3dDevice.Transform.World);
		
			m =m_d3dDevice.Transform.View;
			m.Invert();
			m.M41 = tmp.X;
			m.M42 = tmp.Y;
			m.M43 = tmp.Z;

			m_d3dDevice.Transform.World = m;

			Device.Material = Util_DX.WhiteMaterial();
			Device.SetTexture(0,texY);
			m_mshAxisLabel.DrawSubset(0);
			Device.SetTexture(0,null);

			//Draw Z Axis Label
			m_d3dDevice.Transform.World = Matrix.Translation(0.0f, 0.0f, 25 * fltScale) * mtxWorld * Device.Transform.World1;
			tmp = new Vector3();
			tmp.TransformCoordinate(m_d3dDevice.Transform.World);
		
			m =m_d3dDevice.Transform.View;
			m.Invert();
			m.M41 = tmp.X;
			m.M42 = tmp.Y;
			m.M43 = tmp.Z;

			m_d3dDevice.Transform.World = m;

			Device.Material = Util_DX.WhiteMaterial();
			Device.SetTexture(0,texZ);
			m_mshAxisLabel.DrawSubset(0);
			Device.SetTexture(0,null);

		}
	
		public override void TestIntersection(int x, int y, AnimatTools.Forms.BodyPlan.Command.enumCommandMode cmCommand, ref System.Collections.ArrayList colSelected)
		{
			if(cmCommand == AnimatTools.Forms.BodyPlan.Command.enumCommandMode.SelectJoints)
			{
				Matrix mWorld = this.CombinedTransformationMatrix * Device.Transform.World1;
				IntersectInformation iiTmp = new IntersectInformation();
				if(m_d3dMesh != null && Util_DX.TestIntersectWithMouse(m_d3dMesh, Device.Viewport, Device.Transform.Projection, Device.Transform.View, mWorld, x, y, out iiTmp))
					colSelected.Add(this);

				if(m_mshSelectedBoundingBox != null && Util_DX.TestIntersectWithMouse(m_mshSelectedBoundingBox, Device.Viewport, Device.Transform.Projection, Device.Transform.View, mWorld, x, y,out iiTmp) && this.m_bSelected)
					colSelected.Add(this);
			}

			base.TestIntersection (x, y, cmCommand, ref colSelected);
		}

		protected void DoMouseTranslation(AnimatTools.Framework.MouseEventArgs AnimatMouseArgs)
		{
			//get the parents combined transformation matrix
			Matrix mtxParent = this.Parent_DX.CombinedTransformationMatrix;

			//get the inverse of the parent's combined transformation matrix
			Matrix mtxParentInv = Util_DX.CloneMatrix(mtxParent);
			mtxParentInv.Invert();

			if(AnimatMouseArgs.XKey)
			{
				//get the amount to move
				float fX = (AnimatMouseArgs.DeltaX * AnimatMouseArgs.Scale) + (AnimatMouseArgs.DeltaY * -AnimatMouseArgs.Scale);						
                        
				//Create a mouse matrix
				Matrix mtxMouse = mtxParent * Matrix.Translation(fX,0,0) * mtxParentInv;

				//Apply mtxMouse to mouse translate matrix			
				m_mtxTranslation.Multiply(mtxMouse);				
			}					
			else if(AnimatMouseArgs.YKey)
			{	
				//get the amoount to move
				float fY = (AnimatMouseArgs.DeltaX * AnimatMouseArgs.Scale) + (AnimatMouseArgs.DeltaY * -AnimatMouseArgs.Scale);
						
				//Create a mouse matrix
				Matrix mtxMouse = mtxParent * Matrix.Translation(0,fY,0) * mtxParentInv;

				//Apply mtxMouse to mouse translate matrix			
				m_mtxTranslation.Multiply(mtxMouse);
			}
			else if(AnimatMouseArgs.ZKey)
			{
				//get the amount to move
				float fZ = (AnimatMouseArgs.DeltaX * AnimatMouseArgs.Scale) + (AnimatMouseArgs.DeltaY * -AnimatMouseArgs.Scale);
						
				//Create a mouse matrix
				Matrix mtxMouse = mtxParent * Matrix.Translation(0,0,fZ) * mtxParentInv;

				//Apply mtxMouse to mouse translate matrix			
				m_mtxTranslation.Multiply(mtxMouse);
			}			
			else if(AnimatMouseArgs.SKey)
			{
				//get the amount to move
				float fZ = AnimatMouseArgs.DeltaX * (AnimatMouseArgs.Scale * 2);
				float fY = AnimatMouseArgs.DeltaY * (AnimatMouseArgs.Scale * 2);

				//Create a mouse matrix
				Matrix mtxMouse = mtxParent * Matrix.Translation(0,-fY,fZ) * mtxParentInv;

				//Apply mtxMouse to mouse translate matrix			
				m_mtxTranslation.Multiply(mtxMouse);
			}
			else
			{
				//get the amount to move
				float fX = AnimatMouseArgs.DeltaX * (AnimatMouseArgs.Scale * 2);
				float fY = AnimatMouseArgs.DeltaY * (AnimatMouseArgs.Scale * 2);

				//Create a mouse matrix
				Matrix mtxMouse = mtxParent * Matrix.Translation(fX,-fY,0) * mtxParentInv;
								
				//Apply mtxMouse to mouse translate matrix			
				m_mtxTranslation.Multiply(mtxMouse);
			}
		}
		protected void DoMouseRotation(AnimatTools.Framework.MouseEventArgs AnimatMouseArgs)
		{
			if(AnimatMouseArgs.XKey)
			{
				float fX = (AnimatMouseArgs.DeltaX * AnimatMouseArgs.Scale) + (AnimatMouseArgs.DeltaY * -AnimatMouseArgs.Scale);

				m_mtxOrientation.Multiply(Matrix.RotationX(fX));
					
				m_v3Rotation = Util_DX.DecomposeXYZRotationMatrix(m_mtxOrientation);											
			}								
			else if(AnimatMouseArgs.YKey)
			{
				float fY = (AnimatMouseArgs.DeltaX * AnimatMouseArgs.Scale) + (AnimatMouseArgs.DeltaY * -AnimatMouseArgs.Scale);
	
				m_mtxOrientation.Multiply(Matrix.RotationY(fY));
					
				m_v3Rotation = Util_DX.DecomposeXYZRotationMatrix(m_mtxOrientation);				
			}
			else if(AnimatMouseArgs.ZKey)
			{					
				float fZ = (AnimatMouseArgs.DeltaX * AnimatMouseArgs.Scale) + (AnimatMouseArgs.DeltaY * -AnimatMouseArgs.Scale);
						
				m_mtxOrientation.Multiply(Matrix.RotationZ(fZ));
				
				m_v3Rotation = Util_DX.DecomposeXYZRotationMatrix(m_mtxOrientation);
			}			
			else if(AnimatMouseArgs.SKey)
			{					
				float fX = (AnimatMouseArgs.DeltaX * -AnimatMouseArgs.Scale * 2);
				float fZ = (AnimatMouseArgs.DeltaY * -AnimatMouseArgs.Scale * 2);

				m_mtxOrientation.Multiply(Matrix.RotationY(fZ)* Matrix.RotationZ(fX));
					
				m_v3Rotation = Util_DX.DecomposeXYZRotationMatrix(m_mtxOrientation);
			}
			else
			{
				float fX = (AnimatMouseArgs.DeltaX * -AnimatMouseArgs.Scale * 2);
				float fY = (AnimatMouseArgs.DeltaY * -AnimatMouseArgs.Scale * 2);
					
				m_mtxOrientation.Multiply(Matrix.RotationX(fY) * Matrix.RotationY(fX));
					
				m_v3Rotation = Util_DX.DecomposeXYZRotationMatrix(m_mtxOrientation);
			}
		}
		
		protected void ProcessCenterSquare(AnimatTools.Framework.MouseEventArgs AnimatMouseArgs)
		{
			if(AnimatMouseArgs.Button == System.Windows.Forms.MouseButtons.Left)
				this.DoMouseTranslation(AnimatMouseArgs);
			else if (AnimatMouseArgs.Button == MouseButtons.Right)
				this.DoMouseRotation(AnimatMouseArgs);				
		}		

		public override bool IsSelectionBoxSelected(AnimatTools.Framework.MouseEventArgs AnimatMouseArgs)
		{
			if(this.m_bCBSelected)
				return true;
			else
			{
				//set the location of the upper box
				Vector3 tmp = new Vector3();
				tmp.TransformCoordinate(CombinedTransformationMatrix * Device.Transform.World1);
		
				Matrix mWorld = this.Device.Transform.View;
				mWorld.Invert();
				mWorld.M41 = tmp.X;
				mWorld.M42 = tmp.Y;
				mWorld.M43 = tmp.Z;
		
				return m_mshCB != null && Util_DX.TestIntersectWithMouse(m_mshCB,Device.Viewport, Device.Transform.Projection, Device.Transform.View, mWorld,AnimatMouseArgs.X, AnimatMouseArgs.Y, out this.m_iiIntersectInfo);
				
			}
		}

		public override void RefreshSelectionBoxSizes()
		{
			float fltDistance = this.Location.Distance(this.ParentStructure.BodyEditor.BodyView.CameraPosition);
			float fltScale = (fltDistance/5)*0.05f;

			if(m_fltSelBoxSize != fltScale || m_mshCB == null || m_mshCB.Disposed)
			{
				m_fltSelBoxSize = fltScale;
				m_mshCB = Util_DX.CreateBillboard(Device,m_fltSelBoxSize, m_fltSelBoxSize);
			}
		}

		protected virtual void CreateBody()
		{
			float fltScale = ((this.MinDimension + this.MaxDimension)/2 * 0.15f);
			if(fltScale > 0.1f) fltScale = 0.1f;			

			m_mshAxisCylinder = Mesh.Cylinder(Device,fltScale, fltScale, 20 * fltScale, 16, 16);
			m_mshAxisCone = Mesh.Cylinder(Device,0.0f, 3 * fltScale, 5 * fltScale, 16, 16);
			m_mshAxisLabel = Util_DX.CreateBillboard(Device,5 * fltScale, 5 * fltScale);

			m_matXAxis = new Material();
			m_matXAxis.Diffuse = Color.Red;
			m_matYAxis = new Material();
			m_matYAxis.Diffuse = Color.Green;
			m_matZAxis = new Material();
			m_matZAxis.Diffuse = Color.Blue;

			LoadTextures();
		}

		protected void  LoadTextures()
		{
			if(texCB == null)
			{
				//load center selectionbox graphic
				System.Reflection.Assembly myAssembly = System.Reflection.Assembly.Load("VortexAnimatTools");
				
				texCB = LoadTextureFromAssembly("ikpoint0.png", ref myAssembly);
				texX = LoadTextureFromAssembly("X.png", ref myAssembly);
				texY = LoadTextureFromAssembly("Y.png", ref myAssembly);
				texZ = LoadTextureFromAssembly("Z.png", ref myAssembly);
			}			
		}

		protected Texture LoadTextureFromAssembly(string fileName, ref System.Reflection.Assembly assembly)
		{
			try
			{
				Texture tex = null;
				System.IO.Stream stream = assembly.GetManifestResourceStream("VortexAnimatTools.Graphics." + fileName);

				//load texture from stream
				tex = TextureLoader.FromStream(Device,stream);

				return tex;

			}
			catch(Exception ex)
			{
				Util.DisplayError(ex);
			}

			return null;

		}


		protected virtual void CleanUp()
		{
			SafeRelease(ref m_d3dMesh);
			SafeRelease(ref m_mshSelectedBoundingBox);
			SafeRelease(ref m_mshAxisCone);
			SafeRelease(ref m_mshAxisCylinder);
			SafeRelease(ref m_mshAxisLabel);
			SafeRelease(ref m_mshCB);
			
			System.GC.Collect();
		}

		protected void SafeRelease(ref Mesh mesh)
		{
			if(mesh != null)
			{
				mesh.Dispose();
				mesh = null;
			}
		}
		protected virtual void InvalidateBodyView()
		{
			AnimatTools.DataObjects.Physical.PhysicalStructure doStructure = this.ParentStructure;
			if(doStructure != null && doStructure.BodyEditor != null && doStructure.BodyEditor.BodyView != null)
				this.ParentStructure.BodyEditor.BodyView.Invalidate();
		}

		protected virtual void InvalidateBodyViewAndProperties()
		{
			InvalidateBodyView();
			if(this.ParentStructure != null && this.ParentStructure.BodyEditor != null && this.ParentStructure.BodyEditor.PropertiesBar != null)
				this.ParentStructure.BodyEditor.PropertiesBar.RefreshProperties();
		}


		protected override void Properties_SetValue(object sender, Crownwood.Magic.Controls.PropertySpecEventArgs e)
		{
			base.Properties_SetValue(sender, e);
			InvalidateBodyView();
		}

		public override void BeforeSelected()
		{
			//This code was added in because if you undo something that was done to an object and then the window is closed, we reopen then window, but
			//the device is no longer correct. so we need to reset it in this case.
			if(this.ParentStructure != null && this.ParentStructure.BodyEditor != null && this.ParentStructure.BodyEditor.BodyView != null)
			{
				VortexAnimatTools.Forms.BodyPlan.BodyViewD3D frmView = (VortexAnimatTools.Forms.BodyPlan.BodyViewD3D) this.ParentStructure.BodyEditor.BodyView;

				if(this.Device != frmView.Device)
					this.Device = frmView.Device;
			}
		}

		public override void SaveDataColumnToXml(ref AnimatTools.Interfaces.StdXml oXml)
		{
			oXml.IntoElem();
			oXml.AddChildElement("StructureID", this.StructureID);
			oXml.AddChildElement("JointID", this.ID);
			oXml.OutOfElem();
		}


		public override void UpdateWithMouse(AnimatTools.Framework.MouseEventArgs AnimatMouseArgs)
		{
			//set the location of the upper box
			Vector3 tmp = new Vector3();
			tmp.TransformCoordinate(CombinedTransformationMatrix * Device.Transform.World1);
		
			Matrix mWorld = this.Device.Transform.View;
			mWorld.Invert();
			mWorld.M41 = tmp.X;
			mWorld.M42 = tmp.Y;
			mWorld.M43 = tmp.Z;
		
			if(AnimatMouseArgs.Button == System.Windows.Forms.MouseButtons.None)
			{
				m_bCBSelected = false;
				return;
			}

			IntersectInformation iiInfo = new IntersectInformation();
			if(m_mshCB != null && Util_DX.TestIntersectWithMouse(m_mshCB, Device.Viewport, Device.Transform.Projection, Device.Transform.View, mWorld,AnimatMouseArgs.X, AnimatMouseArgs.Y, out iiInfo) && (AnimatMouseArgs.Button == System.Windows.Forms.MouseButtons.Left || AnimatMouseArgs.Button == System.Windows.Forms.MouseButtons.Right))
				m_bCBSelected = true;

			if(m_bCBSelected)
				ProcessCenterSquare(AnimatMouseArgs);				
		}

		protected override void CloneInternal(AnimatTools.Framework.DataObject doOriginal, bool bCutData, AnimatTools.Framework.DataObject doRoot)
		{
			base.CloneInternal (doOriginal, bCutData, doRoot);
			Joint_Clone(doOriginal, bCutData, doRoot);
		}

		protected void Joint_Clone(AnimatTools.Framework.DataObject doOriginal, bool bCutData, AnimatTools.Framework.DataObject doRoot)
		{
			Joint_DX doOrigBody = (Joint_DX) doOriginal;

			m_d3dDevice = doOrigBody.m_d3dDevice;
			m_v3Rotation = new Microsoft.DirectX.Vector3(doOrigBody.m_v3Rotation.X, doOrigBody.m_v3Rotation.Y, doOrigBody.m_v3Rotation.Z);
			
			m_mtxOrientation = Util_DX.CloneMatrix(doOrigBody.m_mtxOrientation);
			m_mtxTranslation = Util_DX.CloneMatrix(doOrigBody.m_mtxTranslation);

			m_d3dMaterial.Ambient = doOrigBody.m_d3dMaterial.Ambient;
			m_d3dMaterial.Diffuse = doOrigBody.m_d3dMaterial.Diffuse;
			m_d3dTransparentMaterial.Ambient = doOrigBody.m_d3dTransparentMaterial.Ambient;
			m_d3dTransparentMaterial.Diffuse = doOrigBody.m_d3dTransparentMaterial.Diffuse;
			m_d3dSelectedMaterial.Ambient = doOrigBody.m_d3dSelectedMaterial.Ambient;
			m_d3dSelectedMaterial.Diffuse = doOrigBody.m_d3dSelectedMaterial.Diffuse;

			m_fRadius = doOrigBody.m_fRadius;
			m_snRadius = (AnimatTools.Framework.ScaledNumber) doOrigBody.m_snRadius.Clone(this, bCutData, doRoot);

			m_snXLocalLocation = (AnimatTools.Framework.ScaledNumber) doOrigBody.m_snXLocalLocation.Clone(this, bCutData, doRoot);
			m_snYLocalLocation = (AnimatTools.Framework.ScaledNumber) doOrigBody.m_snYLocalLocation.Clone(this, bCutData, doRoot);
			m_snZLocalLocation = (AnimatTools.Framework.ScaledNumber) doOrigBody.m_snZLocalLocation.Clone(this, bCutData, doRoot);
			
			m_snXWorldLocation = (AnimatTools.Framework.ScaledNumber) doOrigBody.m_snXWorldLocation.Clone(this, bCutData, doRoot);
			m_snYWorldLocation = (AnimatTools.Framework.ScaledNumber) doOrigBody.m_snYWorldLocation.Clone(this, bCutData, doRoot);
			m_snZWorldLocation = (AnimatTools.Framework.ScaledNumber) doOrigBody.m_snZWorldLocation.Clone(this, bCutData, doRoot);

			m_snXRotation = (AnimatTools.Framework.ScaledNumber) doOrigBody.m_snXRotation.Clone(this, bCutData, doRoot);
			m_snYRotation = (AnimatTools.Framework.ScaledNumber) doOrigBody.m_snYRotation.Clone(this, bCutData, doRoot);
			m_snZRotation = (AnimatTools.Framework.ScaledNumber) doOrigBody.m_snZRotation.Clone(this, bCutData, doRoot);
		}

		public override void SwapBodyPartCopy(AnimatTools.DataObjects.Physical.BodyPart doOriginal)
		{
			base.SwapBodyPartCopy(doOriginal);
			Joint_Clone(doOriginal, false, null);
		}

		protected override void BuildProperties()
		{
			base.BuildProperties();

//			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Radius", typeof(float), "Radius",
//																	"Part Properties", "Sets the radius for this joint.", this.Radius));			
			Crownwood.Magic.Controls.PropertyBag pbNumberBag  = this.RadiusScaled.Properties;
			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Radius", pbNumberBag.GetType(), "RadiusScaled", 
																	"Size", "Sets the radius of the sphere.", pbNumberBag,
																	"", typeof(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)));
		}		

		#region Loading and Saving Data
		
		public override void LoadData(ref AnimatTools.DataObjects.Simulation dsSim, ref AnimatTools.DataObjects.Physical.PhysicalStructure doStructure, ref AnimatTools.Interfaces.StdXml oXml)
		{
			base.LoadData (ref dsSim, ref doStructure, ref oXml);
		
			oXml.IntoElem();
			m_fRadius = oXml.GetChildFloat("Radius", m_fRadius);

			if(oXml.FindChildElement("OrientationMatrix", false))
			{
				m_mtxOrientation = Util_DX.LoadMatrix(ref oXml, "OrientationMatrix");
				m_v3Rotation = Util_DX.DecomposeXYZRotationMatrix(Orientation);
				//this.Rotation = new Vec3d(null,Geometry.RadianToDegree(m_v3Rotation.X),Geometry.RadianToDegree(m_v3Rotation.Y),Geometry.RadianToDegree(m_v3Rotation.Z));
			}

			if(oXml.FindChildElement("TranslationMatrix",false))
				m_mtxTranslation = Util_DX.LoadMatrix(ref oXml,"TranslationMatrix");				

			oXml.OutOfElem();	
			
		}
		
		public override void SaveData(ref AnimatTools.DataObjects.Simulation dsSim, ref AnimatTools.DataObjects.Physical.PhysicalStructure doStructure, ref AnimatTools.Interfaces.StdXml oXml)
		{
			base.SaveData (ref dsSim, ref doStructure, ref oXml);
		
			oXml.IntoElem();

				RigidBodies.RigidBody_DX rbParent = (RigidBodies.RigidBody_DX)Parent;

				Vector3 v3ParentAbsLoc= new Vector3(rbParent.CombinedTransformationMatrix.M41, rbParent.CombinedTransformationMatrix.M42, rbParent.CombinedTransformationMatrix.M43);
				Vector3 v3AbsLoc = new Vector3(this.CombinedTransformationMatrix.M41, this.CombinedTransformationMatrix.M42, this.CombinedTransformationMatrix.M43);
				Vector3 v3Loc = v3AbsLoc - v3ParentAbsLoc;
				
				Vector3 v3Rot = Util_DX.DecomposeXYZRotationMatrix(this.CombinedTransformationMatrix);

				oXml.AddChildElement("Radius", m_fRadius);
				
				Util.SaveVector(ref oXml, "RelativePosition", new Vec3d(null, v3Loc.X, v3Loc.Y, v3Loc.Z));
				Util.SaveVector(ref oXml, "Rotation", new Vec3d(null, -v3Rot.X, v3Rot.Y, -v3Rot.Z));
				
				Util_DX.SaveMatrix(ref oXml, "OrientationMatrix", Orientation);
				Util_DX.SaveMatrix(ref oXml, "TranslationMatrix", Translation);
								

			oXml.OutOfElem();	
			
		}

		#endregion

		#endregion

	}
}
