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
using System.Diagnostics;

namespace VortexAnimatTools.DataObjects.Physical.RigidBodies
{
	
	/// <summary>
	/// Base class for the primative body types
	/// </summary>
	
	public abstract class RigidBody_DX : AnimatTools.DataObjects.Physical.RigidBody
	{
		#region Attributes

		protected Microsoft.DirectX.Direct3D.Device m_d3dDevice;						//Device used by the mesh

		protected Microsoft.DirectX.Direct3D.Mesh m_d3dMesh;							//The actual mesh
		protected Microsoft.DirectX.Direct3D.Material m_d3dMaterial;					//The material of the object
		protected Microsoft.DirectX.Direct3D.Material m_d3dTransparentMaterial;			//Transparent version of the material
		protected Microsoft.DirectX.Direct3D.Texture m_d3dTexture;						//The texture of the body
		protected Microsoft.DirectX.Direct3D.Texture m_d3dAlphaTexture;					//Transparent version of the texture

		protected Microsoft.DirectX.Direct3D.Mesh m_d3dReceptiveFieldMesh;  //The mesh to use for displaying the receptive fields.
		protected Microsoft.DirectX.Direct3D.Mesh m_d3dReceptiveFieldPoint;  //The mesh to use for displaying the points of a receptive fields.
		protected Microsoft.DirectX.Direct3D.Material m_d3dRFPointMaterial;					//The material of the object
		protected Microsoft.DirectX.Direct3D.Material m_d3dSelRFPointMaterial;			//Transparent version of the material

		protected Microsoft.DirectX.Vector3 m_v3Rotation;								//Eular Angle of the rotation for the body
		protected DxLocationRef m_vrLocation;		/*remove this*/									
		protected DxRotationRef m_vrRotation;		/*remove this*/

		protected Microsoft.DirectX.Direct3D.IntersectInformation m_iiIntersectInfo;	//IntersectInformation after doing intersection test	

		protected Microsoft.DirectX.Vector3 m_v3FaceNormal;								//Normal of the last selected face
		protected Microsoft.DirectX.Vector3 m_v3SelectedFacePoint;						//Point on the last selected face

		protected Microsoft.DirectX.Vector3 m_v3Direction = new Vector3();				//Direction this body is facing
		
		protected Microsoft.DirectX.Matrix m_mtxOrientation = Matrix.Identity;			//Orientation matrix for this body
		protected Microsoft.DirectX.Matrix m_mtxTranslation = Matrix.Identity;			//Translation matrix for this body
				
		protected Mesh m_mshSelectedBoundingBox;										//The mesh for the selected bounding box
		protected Material m_matTransparentSelectedMaterial;							//The material for the selected bounding box
		protected Mesh m_mshCB;															//The Mesh for the center selection box
		protected Mesh m_mshLB;															//The Mesh for the lower selection circle
		protected Texture texCB = null;													//Texture for the center selection box
		protected Texture texLB = null;													//Texture for the lower selection circle
		protected Vector3 m_v3LB;														//Position of the lower selection circle

		protected bool m_bUBSelected = false;											//Flag for knowing if the center selection box is selected
		protected bool m_bLBSelected = false;											//Flag for knowing if the lower selection circle is selected
		protected bool m_bSelectionBoxSelected = false;									//Flag for knowing if the selectedboundingbox is selected
		protected float m_fltSelBoxSize = 0;											//Size of the selection box and circle

		protected Microsoft.DirectX.Vector3 m_v3SBBN;									//Normal of the side of the selected bounding box selected

		protected AnimatTools.Framework.ScaledNumber m_snXLocalLocation;				//Scaled version of X component of local location
		protected AnimatTools.Framework.ScaledNumber m_snYLocalLocation;				//Scaled version of Y component of local location
		protected AnimatTools.Framework.ScaledNumber m_snZLocalLocation;				//Scaled version of Z component of local location

		protected AnimatTools.Framework.ScaledNumber m_snXWorldLocation;				//Scaled version of X component of absolute location
		protected AnimatTools.Framework.ScaledNumber m_snYWorldLocation;				//Scaled version of Y component of absolute location
		protected AnimatTools.Framework.ScaledNumber m_snZWorldLocation;				//Scaled version of Z component of absolute location
	
		protected AnimatTools.Framework.ScaledNumber m_snXRotation;						//Scaled version of degrees around X axis
		protected AnimatTools.Framework.ScaledNumber m_snYRotation;						//Scaled version of degrees around Y axis
		protected AnimatTools.Framework.ScaledNumber m_snZRotation;						//Scaled version of degrees around Z axis

		private Mesh m_mshAxisCylinder;													//Cylinder part of Axis for orientation
		private Mesh m_mshAxisCone;														//Cone part of Axis for orientation (to make an arrow)
		private Mesh m_mshAxisLabel;													//Label for which axis it is
		private Material m_matXAxis;													//Material for the x axis
		private Material m_matYAxis;													//Material for the y axis
		private Material m_matZAxis;													//Material for the z axis

		private Texture texX = null;													//Texture for the x axis label
		private Texture texY = null;													//Texture for the y axis label
		private Texture texZ = null;													//Texture for the z axis label

		protected Vector3 m_v3Intersect = new Vector3();
		protected int m_iAlpha;
		
		#endregion

		#region Properties

		/// <summary>
		/// Retrieves or Sets the device object used by DirectX to draw the rigidbody
		/// </summary>
		public Microsoft.DirectX.Direct3D.Device Device 
		{
			get{return m_d3dDevice;}
			set
			{
				m_d3dDevice = value;

				//if this body has a joint set the device for the joint
				if(m_JointToParent != null)
				{
					VortexAnimatTools.DataObjects.Physical.Joints.Joint_DX dxJoint = (VortexAnimatTools.DataObjects.Physical.Joints.Joint_DX) m_JointToParent;
					dxJoint.Device = value;
				}

				//go through each child of this body and set the device for the child
				VortexAnimatTools.DataObjects.Physical.RigidBodies.RigidBody_DX dxChild;
				foreach(DictionaryEntry deItem in m_aryChildBodies)
				{	
					dxChild = (VortexAnimatTools.DataObjects.Physical.RigidBodies.RigidBody_DX) deItem.Value;
					dxChild.Device = value;
				}	
			}//end set
		}	
		
	
		#region Mesh Properties

		/// <summary>
		/// Returns the Mesh of the rigid body
		/// </summary>
		public Microsoft.DirectX.Direct3D.Mesh Mesh
		{
			get{return m_d3dMesh;}
		}
		

		/// <summary>
		/// Returns the intersection information for this body
		/// </summary>
		public Microsoft.DirectX.Direct3D.IntersectInformation IntersectInfo
		{
			get{return this.m_iiIntersectInfo;}
		}		
		

		/// <summary>
		/// Returns the distance of intersection with the ray
		/// </summary>
		public override float IntersectionDistance
		{
			get{return this.m_iiIntersectInfo.Dist;}
		}
				

		/// <summary>
		/// Gets Or Sets the color of this body
		/// </summary>
		public override System.Drawing.Color Color 
		{
			get
			{return m_d3dMaterial.Ambient;}
			set
			{
				int iAlpha = m_d3dMaterial.Ambient.A;
				m_d3dMaterial.Ambient = value;
				m_d3dMaterial.Diffuse = value;

				m_d3dTransparentMaterial.Ambient = Color.FromArgb(64, value);
				m_d3dTransparentMaterial.Diffuse = Color.FromArgb(64, value);
			}
		}


		/// <summary>
		/// Gets or Sets the Alpha value of this body
		/// </summary>
		public override int Alpha 
		{
			get
			{return m_d3dMaterial.Ambient.A;}
			set
			{
				m_iAlpha = value;
				m_d3dMaterial.Ambient = Color.FromArgb(value, m_d3dMaterial.Ambient);
				m_d3dMaterial.Diffuse = Color.FromArgb(value, m_d3dMaterial.Ambient);

				if(m_d3dDevice != null && m_strTexture != null && m_strTexture.Trim().Length > 0)
				{
					if(m_d3dTexture != null)
					{
						m_d3dTexture.Dispose();
						m_d3dTexture = null;
					}

					if(m_d3dAlphaTexture != null)
					{
						m_d3dAlphaTexture.Dispose();
						m_d3dAlphaTexture = null;
					}

					CreateBody();
				}						
			}
		}

		public override float Transparency
		{
			get
			{
				return 1 - (float)Math.Round((double)(Alpha / 255.0), 2);
			}
			set
			{
				Alpha = 255 - (int)(value * 255);
			}
		}


		public override AnimatTools.Framework.Vec3d Size 
		{get {return new AnimatTools.Framework.Vec3d(null, 0, 0, 0);} set{}}


		/// <summary>
		/// Gets the normal of the last face selected
		/// </summary>
		public Vector3 FaceNormal
		{
			get{return this.m_v3FaceNormal;}
		}
		
		
		/// <summary>
		/// Returns the coordinates of the last point selected
		/// </summary>
		public Vector3 SelectedFacePoint
		{
			get{return this.m_v3SelectedFacePoint;}
		}


		/// <summary>
		/// Gets or Sets the direction the body is facing
		/// </summary>
		public Vector3 Direction
		{
			get{return m_v3Direction;}
			set{m_v3Direction = value;}
		}


		/// <summary>
		/// Gets or Sets the texture of this body
		/// </summary>
		public override string Texture
		{
			get{return base.Texture;}
			set
			{
				if(value == null || value.ToUpper() == m_strTexture.ToUpper())
					return;

				base.Texture = value;

				if(m_d3dMesh != null)
				{
					m_d3dMesh.Dispose();
					m_d3dMesh = null;
				}

				if(m_d3dTexture != null)
				{
					m_d3dTexture.Dispose();
					m_d3dTexture = null;
				}

				if(m_d3dAlphaTexture != null)
				{
					m_d3dAlphaTexture.Dispose();
					m_d3dAlphaTexture = null;
				}

				CreateBody();
			}//end set
		}

		public virtual bool NeedToCreateBody
		{
			get
			{
				if(this.Device != null && (m_d3dMesh == null || m_d3dMesh.Disposed || m_mshSelectedBoundingBox == null || m_mshSelectedBoundingBox.Disposed 
					|| m_mshCB == null || m_mshCB.Disposed || m_mshLB == null || m_mshLB.Disposed || m_mshAxisCylinder == null || m_mshAxisCylinder.Disposed
					|| m_mshAxisCone == null || m_mshAxisCone.Disposed || m_mshAxisLabel == null || m_mshAxisLabel.Disposed))
					return true;
				else
					return false;
			}
		}

		public virtual bool NeedToCreateReceptiveFields
		{
			get
			{
				if(this.Device != null && (m_d3dReceptiveFieldMesh == null || m_d3dReceptiveFieldMesh.Disposed || m_d3dReceptiveFieldPoint == null || m_d3dReceptiveFieldPoint.Disposed))
					return true;
				else
					return false;
			}
		}

		#endregion	
		
		#region Matrix Properties	
	
		/// <summary>
		/// Gets or Sets the rotation matrix for this body
		/// </summary>
		public virtual Microsoft.DirectX.Matrix Orientation
		{
			get{return m_mtxOrientation;}
			set{m_mtxOrientation = value;}
		}

		public virtual Microsoft.DirectX.Matrix OrientationRight
		{
			get{
				//Get the Eular Angles (X,Y,Z) using left-handed rotations
				Vector3 v3Rot = Util_DX.DecomposeXYZRotationMatrix(m_mtxOrientation);				

				//Calculate a right-handed rotation matrix using the left-handed eular angles
				Matrix m = Util_DX.RotateRightX(v3Rot.X) *
					Util_DX.RotateRightY(v3Rot.Y) *
					Util_DX.RotateRightZ(v3Rot.Z);

				return m;
			}
			
		}
	
		/// <summary>
		/// Gets or Sets the translation matrix for this body
		/// </summary>
		public Microsoft.DirectX.Matrix Translation
		{
			get{return m_mtxTranslation;}
			set{m_mtxTranslation = value;}
		}
		
	
		/// <summary>
		/// Retuurns the rotation matrix * translation matrix of this body
		/// </summary>
		public Microsoft.DirectX.Matrix TransformationMatrix
		{
			get{return Orientation * Translation;}			
		}

		public Microsoft.DirectX.Matrix TransformationMatrixRight
		{
			get{return OrientationRight * Translation;}			
		}	
		
	
		public Microsoft.DirectX.Matrix CombinedTransformationMatrixRight
		{
			get
			{				
				if(!this.IsRoot)
				{
					RigidBodies.RigidBody_DX rbParent = (RigidBodies.RigidBody_DX)Parent;
					return TransformationMatrixRight * rbParent.CombinedTransformationMatrixRight ;
				}
				else
					return TransformationMatrixRight;	
			}
		}
		/// <summary>
		/// Returns the combined transformation matrix of this body and all the bodies back to the root
		/// </summary>		
		public Microsoft.DirectX.Matrix CombinedTransformationMatrix
		{
			get
			{				
				if(!this.IsRoot)
				{
					RigidBodies.RigidBody_DX rbParent = (RigidBodies.RigidBody_DX)Parent;
					return TransformationMatrix * rbParent.CombinedTransformationMatrix ;
				}
				else
					return TransformationMatrix;	
			}
		}

		#endregion		
	
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
		public virtual  Microsoft.DirectX.Vector3 AbsoluteLocation
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
				if(!this.IsRoot)
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
			}//end set
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

		#endregion	
		
		#region Scaled Local Location Properties
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

		public virtual Microsoft.DirectX.Vector3 Worldlocation
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
				if(!this.IsRoot)
				{
					Matrix invParent = Parent_DX.CombinedTransformationMatrix;
					invParent.Invert();
					v3RelLoc = Vector3.TransformCoordinate(v3AbsLoc, invParent);
					XLocalLocation = v3RelLoc.X;
					YLocalLocation = v3RelLoc.Y;
					ZLocalLocation = v3RelLoc.Z;
				}				
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
				if(!this.IsRoot)
				{
					Matrix invParent = Parent_DX.CombinedTransformationMatrix;
					invParent.Invert();
					v3RelLoc = Vector3.TransformCoordinate(v3AbsLoc, invParent);
					XLocalLocation = v3RelLoc.X;
					YLocalLocation = v3RelLoc.Y;
					ZLocalLocation = v3RelLoc.Z;
				}				
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
				if(!this.IsRoot)
				{
					Matrix invParent = Parent_DX.CombinedTransformationMatrix;
					invParent.Invert();
					v3RelLoc = Vector3.TransformCoordinate(v3AbsLoc, invParent);
					XLocalLocation = v3RelLoc.X;
					YLocalLocation = v3RelLoc.Y;
					ZLocalLocation = v3RelLoc.Z;
				}				
				InvalidateBodyViewAndProperties();		
			}
		}

		#endregion

		#region Scaled World Location Properties
		
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
		/// Gets a Vector3 representation of the rotation of this body.
		/// Sets a Vector3 representation of the rotation of this body and sets the Orientation matrix
		/// </summary>
		public virtual Microsoft.DirectX.Vector3 DxRotation 
		{
			get{return m_v3Rotation;}
			set
			{		
				m_mtxOrientation = Matrix.RotationX(value.X) * Matrix.RotationY(value.Y) * Matrix.RotationZ(value.Z);
				m_v3Rotation = value;
			}
		}


		public override AnimatTools.Framework.Vec3d Rotation 
		{
			get{return m_vrRotation;}
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

	
		#region Scaled Rotation Properties
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

		#endregion
		
		
		#endregion

		public override void GetBodyExtent(ref float iMaxX, ref float iMinX, ref float iMaxY, ref float iMinY)
		{
			base.GetBodyExtent (ref iMaxX, ref iMinX, ref iMaxY, ref iMinY);
			CalculateBodyExtent(ref iMaxX, ref iMinX, ref iMaxY, ref iMinY);
		}


		/// <summary>
		/// Gets a RigidBody_DX version of this body's parent. Returns null if this body's parent is not a bodypart object
		/// </summary>
		public RigidBodies.RigidBody_DX Parent_DX
		{
			get
			{
				if(!this.IsRoot)
				{
					RigidBodies.RigidBody_DX rbParent = (RigidBodies.RigidBody_DX)this.Parent;
					return rbParent;
				}
				
				return null;
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
				biPosition.m_v3Direction = new Microsoft.DirectX.Vector3(m_v3Direction.X, m_v3Direction.Y, m_v3Direction.Z);
				biPosition.m_vSize = (AnimatTools.Framework.Vec3d) this.Size.Clone(this.Size.Parent, false, null);

				biPosition.m_mOrientation = Util_DX.CloneMatrix(m_mtxOrientation);
				biPosition.m_mtxTranslation = Util_DX.CloneMatrix(m_mtxTranslation);

				return biPosition;
			}
			set
			{
				if(value != null && value.m_strID == m_strID)
				{
					VortexAnimatTools.DataObjects.Physical.PartPositionInfo piPos = (VortexAnimatTools.DataObjects.Physical.PartPositionInfo) value;

					m_v3Rotation = new Microsoft.DirectX.Vector3(piPos.m_v3Rotation.X, piPos.m_v3Rotation.Y, piPos.m_v3Rotation.Z);
					m_v3Direction = new Microsoft.DirectX.Vector3(piPos.m_v3Direction.X, piPos.m_v3Direction.Y, piPos.m_v3Direction.Z);
					this.Size = (AnimatTools.Framework.Vec3d) piPos.m_vSize.Clone(this, false, null);
			
					m_mtxOrientation = Util_DX.CloneMatrix(piPos.m_mOrientation);
					m_mtxTranslation = Util_DX.CloneMatrix(piPos.m_mtxTranslation);
				}
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
		
		
		/// <summary>
		/// Gets or Sets if this body is selected 
		/// </summary>
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

		public override Vec3d SelectedReceptiveField
		{
			get
			{
				return base.SelectedReceptiveField;
			}
			set
			{
				m_vSelectedReceptiveField = value;
				if(m_d3dReceptiveFieldMesh != null)
				{
					Util_DX.ColorReceptiveField(m_d3dReceptiveFieldMesh, m_vSelectedReceptiveField, m_gnReceptiveFieldGain);
					InvalidateBodyView();
				}

				AnimatTools.DataObjects.Physical.PhysicalStructure doStruct = this.ParentStructure;
				if(doStruct != null && doStruct.BodyEditor != null && doStruct.BodyEditor.ReceptiveFieldsBar != null)
					doStruct.BodyEditor.ReceptiveFieldsBar.RefreshSelectedReceptiveField();
			}
		}

		public override ArrayList ReceptiveFields
		{
			get
			{
				if(m_aryReceptiveFields.Count == 0)
					CreateReceptiveFields();

				return m_aryReceptiveFields;
			}
		}

		public override ScaledNumber ReceptiveFieldDistance
		{
			get
			{
				return base.ReceptiveFieldDistance;
			}
			set
			{
				base.ReceptiveFieldDistance = value;

				CleanUp();
				InvalidateBodyView();
			}
		}

		public override String DataColumnModuleName {get{return "";}}

		public override String DataColumnClassType {get{return "RigidBodyData";}}

		#endregion 

		#region Methods

		public RigidBody_DX(AnimatTools.Framework.DataObject doParent) : base(doParent)
		{
			m_vrLocation = new DxLocationRef(this);
			m_vrRotation = new DxRotationRef(this);

			m_snXLocalLocation = new AnimatTools.Framework.ScaledNumber(this, "XLocalLocationScaled", "meters", "m");
			m_snYLocalLocation = new AnimatTools.Framework.ScaledNumber(this, "YLocalLocationScaled", "meters", "m");
			m_snZLocalLocation = new AnimatTools.Framework.ScaledNumber(this, "ZLocalLocationScaled", "meters", "m");

			m_snXWorldLocation = new AnimatTools.Framework.ScaledNumber(this, "XWorldLocationScaled", "meters", "m");
			m_snYWorldLocation = new AnimatTools.Framework.ScaledNumber(this, "YWorldLocationScaled", "meters", "m");
			m_snZWorldLocation = new AnimatTools.Framework.ScaledNumber(this, "ZWorldLocationScaled", "meters", "m");

			m_snXRotation = new AnimatTools.Framework.ScaledNumber(this, "XRotationScaled", "Degrees", "Deg");
			m_snYRotation = new AnimatTools.Framework.ScaledNumber(this, "YRotationScaled", "Degrees", "Deg");
			m_snZRotation = new AnimatTools.Framework.ScaledNumber(this, "ZRotationScaled", "Degrees", "Deg");

			this.m_matTransparentSelectedMaterial = new Material();
			this.m_matTransparentSelectedMaterial.Diffuse = Color.FromArgb(128,Color.FloralWhite);
			this.m_matTransparentSelectedMaterial.Ambient = Color.FromArgb(128,Color.FloralWhite);

			Alpha = 255;
			Color = Color.Red;			

			m_d3dRFPointMaterial = new Material();
			m_d3dRFPointMaterial.Ambient = Color.Green;
			m_d3dRFPointMaterial.Diffuse = Color.Green;

			m_d3dSelRFPointMaterial = new Material();
			m_d3dSelRFPointMaterial.Ambient = Color.DarkRed;
			m_d3dSelRFPointMaterial.Diffuse = Color.DarkRed;			
		}		
		
		#region Drawing Methods
				
		/// <summary>
		/// Draws the rigid body
		/// </summary>
		//		public override void DrawRigidBody(AnimatTools.Forms.BodyPlan.Command.enumCommandMode cmCommand)
		//		{
		//			if(cmCommand == AnimatTools.Forms.BodyPlan.Command.enumCommandMode.SelectReceptiveFields && this.Selected)
		//				DrawReceptiveFields(cmCommand);
		//			else
		//				DrawRigidBodyNormal(cmCommand);
		//
		//			base.DrawRigidBody(cmCommand); //draw the children
		//		}

		protected virtual void DrawReceptiveFields(AnimatTools.Forms.BodyPlan.Command.enumCommandMode cmCommand)
		{
			try
			{
				if(this.NeedToCreateReceptiveFields) //if the mesh is null create it  with the loaded information
					CreateReceptiveFields();

				//If a mesh was created then draw it.
				if(m_d3dReceptiveFieldMesh != null && m_d3dReceptiveFieldPoint != null && this.Device != null)
				{
					this.Device.Transform.World = CombinedTransformationMatrix  * Device.Transform.World1;
					m_d3dReceptiveFieldMesh.DrawSubset(0);

					foreach(Vec3d vPoint in m_aryReceptiveFields)
					{
						this.Device.Transform.World = Matrix.Translation((float) vPoint.X, (float) vPoint.Y, (float) vPoint.Z) * CombinedTransformationMatrix  * Device.Transform.World1;

						if(Util_DX.CompareVectors(vPoint, m_vSelectedReceptiveField))
							this.Device.Material = m_d3dSelRFPointMaterial;
						else
							this.Device.Material = m_d3dRFPointMaterial;

						m_d3dReceptiveFieldPoint.DrawSubset(0);
					}
				}
			}
			catch(Exception ex2)
			{
				Debug.WriteLine(ex2.Message);
			}
		}

		protected override void CalculateDistanceFromCamera()
		{
			Vector3 v3Tmp = this.Worldlocation;

			v3Tmp.TransformCoordinate(CombinedTransformationMatrix  * Device.Transform.World1);

			Vec3d dvTmp = new Vec3d(null, v3Tmp.X, v3Tmp.Y, v3Tmp.Z);
			this.m_fDistFromCamera = dvTmp.Distance(this.ParentStructure.BodyEditor.BodyView.CameraPosition);
			
		}

		public override void Draw(AnimatTools.Forms.BodyPlan.Command.enumCommandMode cmCommand)
		{
			if(cmCommand == AnimatTools.Forms.BodyPlan.Command.enumCommandMode.SelectReceptiveFields && this.Selected)
				DrawReceptiveFields(cmCommand);
			else
				DrawParts(cmCommand);
			//				DrawRigidBodyNormal(cmCommand);

			base.Draw(cmCommand); //draw the children
			
		}
		protected void DrawParts(AnimatTools.Forms.BodyPlan.Command.enumCommandMode cmCommand)
		{
			DrawRigidBody(cmCommand);
			//VortexAnimatTools.Forms.BodyPlan.BodyViewD3D frm = (VortexAnimatTools.Forms.BodyPlan.BodyViewD3D)this.ParentStructure.BodyEditor.BodyView;
			//frm.RenderIntoSurface();
			if(m_bSelected)
			{
				this.DrawBoundingBox();
				DrawCenterSelectionBox();

				//				if(!this.IsRoot)
				//					DrawLowerSelectionBox();					
			}
		}


		protected virtual void DrawRigidBody(AnimatTools.Forms.BodyPlan.Command.enumCommandMode cmCommand)
		{
			if(this.NeedToCreateBody) //if the mesh is null create it  with the loaded information
				CreateBody();

			//If a mesh was created then draw it.
			if(m_d3dMesh != null && this.Device != null)
			{
				this.Device.Transform.World = CombinedTransformationMatrix  * Device.Transform.World1;
				
				//if we are in SelectJoint mode set the transparent material and set selected to false
				if(cmCommand == AnimatTools.Forms.BodyPlan.Command.enumCommandMode.SelectJoints || cmCommand == AnimatTools.Forms.BodyPlan.Command.enumCommandMode.SelectReceptiveFields)
				{
					this.Device.Material = m_d3dTransparentMaterial;
					m_bSelected = false;

					if(m_d3dTexture != null)
						m_d3dDevice.SetTexture(0, m_d3dAlphaTexture);
				}
					//otherwise set the material to default material
				else
				{
					this.Device.Material = m_d3dMaterial;

					if(m_d3dTexture != null)
						m_d3dDevice.SetTexture(0, m_d3dTexture);
				}

				//try to draw the mesh
				try
				{
					m_d3dMesh.DrawSubset(0);  //draws the object	
					m_d3dDevice.SetTexture(0, null);
										
				}
					//if we fail..see if the mesh has been disposed..if it has...dispose of the mesh and recreate it
				catch(Exception ex)
				{
					try
					{
						if(m_d3dMesh.Disposed)
						{
							m_d3dMesh.Dispose();
							m_d3dMesh = null;
							this.DrawRigidBody(cmCommand);
						}
					}
					catch(Exception ex2)
					{}
				}
			}
		}
		
		public virtual void ColorSelectedReceptiveField()
		{
			if(m_d3dReceptiveFieldMesh != null && m_vSelectedReceptiveField != null && m_gnReceptiveFieldGain != null)
			{
				Util_DX.ColorReceptiveField(m_d3dReceptiveFieldMesh, m_vSelectedReceptiveField, m_gnReceptiveFieldGain);
				InvalidateBodyView();
			}
		}

		public virtual void DrawBoundingBox()
		{
			if(texCB == null || texLB == null)
				LoadTextures();

			this.DrawAxis();

			//set the material for the selection box
			Device.Material = this.m_matTransparentSelectedMaterial;

			//set the world matrix
			this.Device.Transform.World = CombinedTransformationMatrix  * Device.Transform.World1; //moves the body to where it is in space
			
			//draw the bounding box
			this.m_mshSelectedBoundingBox.DrawSubset(0);							
		}
		

		protected void DrawCenterSelectionBox()
		{
			//set the location of the center selection box
			Vector3 tmp = new Vector3();
			tmp.TransformCoordinate(CombinedTransformationMatrix * Device.Transform.World1);
		
			//get the current view matrix and invert it
			Matrix m = this.Device.Transform.View;
			m.Invert();

			//set the location of the inverted view matrix to the location of the selection box
			m.M41 = tmp.X;
			m.M42 = tmp.Y;
			m.M43 = tmp.Z;

			//set the device world matrix
			this.Device.Transform.World = m;

			//set a white material
			this.Device.Material = Util_DX.WhiteMaterial();

			//disable the z buffer
			this.Device.RenderState.ZBufferEnable = false;
			
			//set the texture of the selection box
			Device.SetTexture(0,texCB);

			//draw the selection box
			m_mshCB.DrawSubset(0);

			//set the texture back to null
			Device.SetTexture(0,null);

			//renable the z buffer
			this.Device.RenderState.ZBufferEnable = true;

		}

		//TODO:  Find what causes the graphics to freeze in this code
		protected void DrawLowerSelectionBox()
		{
			RigidBodies.RigidBody_DX rbParent = (RigidBodies.RigidBody_DX)this.Parent;
			//			Vector3 v3D = rbParent.RelativeLocation - this.RelativeLocation;
			//			v3D.Normalize();

			IntersectInformation iiInfo = new IntersectInformation();
			Vector3 v = new Vector3();
			v.TransformCoordinate(this.CombinedTransformationMatrix);
			this.m_mshSelectedBoundingBox.Intersect(rbParent.RelativeLocation, Direction, out iiInfo);


			m_v3LB = Util_DX.FindIntersectionPoint(this.m_mshSelectedBoundingBox,iiInfo);
			//m_v3LB = rbParent.FindPointOnSurface(new Vector3(), -Direction);
			m_v3LB.TransformCoordinate(CombinedTransformationMatrix * Device.Transform.World1);
			Matrix m = this.Device.Transform.View;
			m.Invert();
			m.M41 = m_v3LB.X;
			m.M42 = m_v3LB.Y;
			m.M43 = m_v3LB.Z;
			this.Device.Transform.World = m;
			this.Device.Material = Util_DX.WhiteMaterial();
			this.Device.RenderState.ZBufferEnable = false;
			Device.SetTexture(0,texLB);
			m_mshLB.DrawSubset(0);
			Device.SetTexture(0,null);
			this.Device.RenderState.ZBufferEnable = true;			
		}

	
		private void DrawAxis()
		{
			Matrix mtxWorld = Matrix.Identity;

			if(!this.IsRoot)
				mtxWorld = Translation * this.Parent_DX.CombinedTransformationMatrix;
			else
				mtxWorld = Translation * this.CombinedTransformationMatrix;

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
	
		
		#endregion
		
		#region Selection Methods
		
		/// <summary>
		/// Test for intersection with the mesh
		/// </summary>
		/// <param name="device">The Direct3D device associated with the mesh</param>
		/// <param name="x">X coordinate of the mouse</param>
		/// <param name="y">Y coordinate of the mouse</param>
		/// <param name="cmCommand">Which mode we are in: SelectBodies, SelectJoints, or AddBodies</param>
		/// <param name="colSelected">A collection of meshes that intersected with the ray from the mouse</param>
		public override void TestIntersection(int x, int y, AnimatTools.Forms.BodyPlan.Command.enumCommandMode cmCommand, ref System.Collections.ArrayList colSelected)
		{
			//If we're not in the right mode the skip this computation
			if(m_d3dMesh != null && cmCommand != AnimatTools.Forms.BodyPlan.Command.enumCommandMode.SelectJoints)
			{
				//matrix transormation of the world
				Matrix mWorld = CombinedTransformationMatrix  * Device.Transform.World1;
						
				//a temporary intersect information for the selected bounding box
				IntersectInformation iiTmp = new IntersectInformation();
				
				//shoot the ray and test for intersection
				if(Util_DX.TestIntersectWithMouse(m_d3dMesh,Device.Viewport, Device.Transform.Projection, Device.Transform.View, mWorld, x, y, out this.m_iiIntersectInfo))
				{
					//if there was a sucessfull intersection then add this mesh to the selected collection
					colSelected.Add(this);

					//Vertices that represent the intersected face
					Vector3[] vertsFace = Util_DX.FindIntersectionFace(m_d3dMesh, m_iiIntersectInfo);					

					//the normal of the selected face
					this.m_v3FaceNormal =  Util_DX.CalculateNormal(vertsFace);
					
					//m_v3SelectedFacePoint = this.RayIntersectWithPlane(new Vector3(),FaceNormal,vertsFace);
					m_v3SelectedFacePoint = Util_DX.FindIntersectionPoint(this.Mesh,this.IntersectInfo);
										
					//Vector3 v = m_v3SelectedFacePoint - m_v3Location;
					//Debug.WriteLine("Relative Point: (" + v.X + ", " + v.Y + ", " + v.Z + ")");
					
				}
				else if((this.m_mshSelectedBoundingBox != null && this.m_bSelected 
					&& Util_DX.TestIntersectWithMouse(m_mshSelectedBoundingBox,Device.Viewport, Device.Transform.Projection, Device.Transform.View, mWorld, x, y, out iiTmp)))
				{
					colSelected.Add(this);					
				}
				//	else 
				//		m_bSelected = false;	
			}			
			base.TestIntersection (x, y, cmCommand, ref colSelected);

		}		

		public virtual bool  TestReceptiveFieldIntersection(int x, int y, ref Vec3d vVertex)
		{
			if(m_d3dReceptiveFieldPoint != null)
			{
				//a temporary intersect information for the selected bounding box
				IntersectInformation iiTmp = new IntersectInformation();
				Vec3d vSelVertex = new Vec3d(null, 0, 0, 0);
				float fltSelDistance = -1;

				//First find all of the vertex points that are intersected.
				foreach(Vec3d vPoint in m_aryReceptiveFields)
				{
					Matrix mWorld = Matrix.Translation((float) vPoint.X, (float) vPoint.Y, (float) vPoint.Z) * CombinedTransformationMatrix  * Device.Transform.World1;

					if(Util_DX.TestIntersectWithMouse(m_d3dReceptiveFieldPoint,Device.Viewport, Device.Transform.Projection, Device.Transform.View, mWorld, x, y, out iiTmp))
					{
						if(fltSelDistance < 0 || iiTmp.Dist < fltSelDistance)
						{
							fltSelDistance = iiTmp.Dist;
							vSelVertex = vPoint;
						}
					}
				}

				if(fltSelDistance > 0)
				{
					vVertex = vSelVertex;
					return true;
				}
			}

			return false;
		}
		 
		public override bool IsSelectionBoxSelected(AnimatTools.Framework.MouseEventArgs AnimatMouseArgs)
		{
			if(this.m_mshCB == null && this.m_mshLB == null)
				return false;
			else if (this.m_bUBSelected || this.m_bLBSelected)
				return true;			
			else
			{
				IntersectInformation iiInfo = new IntersectInformation();				
				
				//matrix transormation of the world
				Vector3 tmp = new Vector3();
				tmp.TransformCoordinate(CombinedTransformationMatrix * Device.Transform.World1);
		
				Matrix mWorld = this.Device.Transform.View;
				mWorld.Invert();
				mWorld.M41 = tmp.X;
				mWorld.M42 = tmp.Y;
				mWorld.M43 = tmp.Z;
		
				if(Util_DX.TestIntersectWithMouse(m_mshCB,Device.Viewport, Device.Transform.Projection, Device.Transform.View, mWorld,AnimatMouseArgs.X, AnimatMouseArgs.Y, out iiInfo))
					return true;
				
				mWorld.M41 = m_v3LB.X;
				mWorld.M42 = m_v3LB.Y;
				mWorld.M43 = m_v3LB.Z;

				if(Util_DX.TestIntersectWithMouse(m_mshLB,Device.Viewport, Device.Transform.Projection, Device.Transform.View, mWorld,AnimatMouseArgs.X, AnimatMouseArgs.Y, out iiInfo))
					return true;				

				return false;
			}
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
		

		#endregion
		
		#region Mouse Manipulation Methods		
		
		public override void UpdateWithMouse(AnimatTools.Framework.MouseEventArgs AnimatMouseArgs)
		{
			IntersectInformation iiInfo = new IntersectInformation();

			//matrix transormation of the world
			Vector3 tmp = new Vector3();
			tmp.TransformCoordinate(CombinedTransformationMatrix * this.Device.Transform.World1);
		
			Matrix mWorld = this.Device.Transform.View;
			mWorld.Invert();
			mWorld.M41 = tmp.X;
			mWorld.M42 = tmp.Y;
			mWorld.M43 = tmp.Z;
		
			
			if(AnimatMouseArgs.Button == System.Windows.Forms.MouseButtons.None)
			{
				m_bUBSelected = false;
				m_bLBSelected = false;
				m_bSelectionBoxSelected = false;
				return;
			}


			//Did we click on the middle selection box?
			if(m_mshCB != null && Util_DX.TestIntersectWithMouse(m_mshCB,Device.Viewport, Device.Transform.Projection, Device.Transform.View, mWorld,AnimatMouseArgs.X, AnimatMouseArgs.Y, out iiInfo))
				m_bUBSelected = true;
			
			
			mWorld.M41 = m_v3LB.X;
			mWorld.M42 = m_v3LB.Y;
			mWorld.M43 = m_v3LB.Z;

			//Did we click on the lower selection box?
			if(m_mshLB != null && Util_DX.TestIntersectWithMouse(m_mshLB,Device.Viewport, Device.Transform.Projection, Device.Transform.View, mWorld,AnimatMouseArgs.X, AnimatMouseArgs.Y, out iiInfo))
				m_bLBSelected = true;
				
			if(m_bUBSelected && !m_bSelectionBoxSelected)
				ProcessCenterSquare(AnimatMouseArgs);
			else if(m_bLBSelected && !m_bSelectionBoxSelected)
				ProcessLowerSquare(AnimatMouseArgs);
			else
			{			
				mWorld = CombinedTransformationMatrix  * this.Device.Transform.World1;
				
				if(!m_bSelectionBoxSelected && this.m_mshSelectedBoundingBox != null && Util_DX.TestIntersectWithMouse(this.m_mshSelectedBoundingBox,Device.Viewport, Device.Transform.Projection, Device.Transform.View, mWorld,AnimatMouseArgs.X, AnimatMouseArgs.Y, out iiInfo))
				{
					m_bSelectionBoxSelected = true;
					Vector3[] vertsFace = Util_DX.FindIntersectionFace(this.m_mshSelectedBoundingBox, iiInfo);					
					//the normal of the selected face
					this.m_v3SBBN =  Util_DX.CalculateNormal(vertsFace);
				}

				if(m_bSelectionBoxSelected)
					ProcessSelectedBoundingBox(AnimatMouseArgs, this.m_v3SBBN);				
			}
			
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

			Direction = FindDirection();			
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
			{				
				if(!this.IsRoot)
					this.DoMouseTranslation(AnimatMouseArgs);
			}
			else if (AnimatMouseArgs.Button == MouseButtons.Right)
			{				
				this.DoMouseRotation(AnimatMouseArgs);	
			}
		}


		protected void ProcessLowerSquare(AnimatTools.Framework.MouseEventArgs AnimatMouseArgs)
		{
			RigidBodies.RigidBody_DX rbParent = this.Parent_DX;

			//						Vector3 v3Tmp = this.LocalLocation;
			//						//allow for normal movement with mouse
			//						DoMouseTranslation(AnimatMouseArgs);
			//			
			//						//now make sure the final position of the movement puts the object on the surface of the parent
			//			
			//						//shoot a ray from the center of this body to to the surface of the parent.  
			//						//Direction of the ray is -this.Direction
			//						IntersectInformation iiTmp = new IntersectInformation();
			//						if(rbParent.Mesh.Intersect(this.LocalLocation,-Direction,out iiTmp))
			//						{
			//							//find the point of intersection
			//							Vector3 m_v3Intersect = Util_DX.FindIntersectionPoint(rbParent.Mesh,iiTmp);
			//							this.LocalLocation = m_v3Intersect + this.FindPointOnSurface(new Vector3(),-Direction);
			//						}
			//						else
			//						{
			//							//MessageBox.Show("There was NOT an intersection");
			////							System.Windows.Forms.MouseEventArgs MouseArgs = new System.Windows.Forms.MouseEventArgs(AnimatMouseArgs.Button, AnimatMouseArgs.Clicks, -AnimatMouseArgs.X, -AnimatMouseArgs.Y, AnimatMouseArgs.Delta);
			////							AnimatTools.Framework.MouseEventArgs ArgsInv = new AnimatTools.Framework.MouseEventArgs(MouseArgs,AnimatMouseArgs.OldX, AnimatMouseArgs.OldY, AnimatMouseArgs.Shift, AnimatMouseArgs.Control, AnimatMouseArgs.XKey, AnimatMouseArgs.YKey, AnimatMouseArgs.ZKey, AnimatMouseArgs.SKey, AnimatMouseArgs.Scale);
			////							DoMouseTranslation(ArgsInv);
			//							this.LocalLocation = v3Tmp;
			//						}

			//			//Get the spherical coordinates for the current location
			//			Vector3 v3Spherical = Util_DX.Cartesian_To_Spherical(this.LocalLocation);
			//
			//			//change the r and theta values according to the mouse
			//			v3Spherical.X = 1.5f;
			//			v3Spherical.Z += AnimatMouseArgs.DeltaX * (AnimatMouseArgs.Scale * 2);
			//			v3Spherical.Y += AnimatMouseArgs.DeltaY * (AnimatMouseArgs.Scale * 2);
			//		
			//			
			//			//Convert the sphereical cooridnate back to cartesian
			//			Vector3 v3TmpLoc = Util_DX.Spherical_To_Cartesian(v3Spherical);
			//			this.LocalLocation = v3TmpLoc;
			//			Direction = FindDirection();
			//			
			//			//shoot a ray from the center of this body to to the surface of the parent.  
			//			//Direction of the ray is -this.Direction
			//			IntersectInformation iiTmp = new IntersectInformation();
			//			if(rbParent.Mesh.Intersect(Vector3.Empty, v3TmpLoc ,out iiTmp))
			//			{
			//				//find the point of intersection
			//				Vector3 m_v3Intersect = Util_DX.FindIntersectionPoint(rbParent.Mesh,iiTmp);
			//				this.LocalLocation = m_v3Intersect + this.FindPointOnSurface(new Vector3(),-Direction);
			//				this.OrientBody();
			//			}
			//			else
			//				MessageBox.Show("Doh");
			//
			//			this.LocalLocation = Util_DX.Spherical_To_Cartesian(v3Spherical);

			Vector3 v3Tmp;

			if(m_v3Intersect != Vector3.Empty)
				v3Tmp = m_v3Intersect;
			else
				v3Tmp = this.LocalLocation;
			
			float fX = (AnimatMouseArgs.DeltaX * -AnimatMouseArgs.Scale * 2);
			float fY = (AnimatMouseArgs.DeltaY * -AnimatMouseArgs.Scale * 2);

			v3Tmp.TransformCoordinate(Matrix.RotationX(fX) * Matrix.RotationZ(fY));

			this.LocalLocation = v3Tmp;
			Direction = FindDirection();

			IntersectInformation iiTmp = new IntersectInformation();
			if(rbParent.Mesh.Intersect(Vector3.Empty, this.LocalLocation ,out iiTmp))
			{
				//find the point of intersection
				m_v3Intersect = Util_DX.FindIntersectionPoint(rbParent.Mesh,iiTmp);
				this.LocalLocation = m_v3Intersect + this.FindPointOnSurface(new Vector3(),-Direction);
				//this.LocalLocation = m_v3Intersect;
				this.OrientBody();
			}
			//			else
			//				MessageBox.Show("Damnit");
			
		}
		
	
		protected void ProcessSelectedBoundingBox(AnimatTools.Framework.MouseEventArgs AnimatMouseArgs, Vector3 normal)
		{
			if(this.m_v3SBBN == new Vector3(0,0,-1) || this.m_v3SBBN == new Vector3(0,0,1))
				IncreaseSizeZ(AnimatMouseArgs);
			else if(this.m_v3SBBN == new Vector3(0,-1,0) || this.m_v3SBBN == new Vector3(0,1,0))
				IncreaseSizeY(AnimatMouseArgs);
			else if(this.m_v3SBBN == new Vector3(-1,0,0) || this.m_v3SBBN == new Vector3(1,0,0))
				IncreaseSizeX(AnimatMouseArgs);
		}

		
		protected virtual void IncreaseSizeZ(AnimatTools.Framework.MouseEventArgs AnimatMouseArgs)
		{

		}
		

		protected virtual void IncreaseSizeY(AnimatTools.Framework.MouseEventArgs AnimatMouseArgs)
		{

		}
		

		protected virtual void IncreaseSizeX(AnimatTools.Framework.MouseEventArgs AnimatMouseArgs)
		{

		}

		
		#endregion

		#region Loading and Saving Data

		public override void SaveDataColumnToXml(ref AnimatTools.Interfaces.StdXml oXml)
		{
			oXml.IntoElem();
			oXml.AddChildElement("StructureID", this.StructureID);
			oXml.AddChildElement("RigidBodyID", this.ID);
			oXml.OutOfElem();		
		}

		
		public override void LoadData(ref AnimatTools.DataObjects.Simulation dsSim, ref AnimatTools.DataObjects.Physical.PhysicalStructure doStructure, ref AnimatTools.Interfaces.StdXml oXml)
		{
			base.LoadData (ref dsSim, ref doStructure, ref oXml);
			oXml.IntoElem();

			//get the color of the body
			if(oXml.FindChildElement("Color"))
			{
				System.Drawing.Color oColor = Util.LoadColor(ref oXml, "Color");

				this.Alpha = oColor.A;
				this.Color = oColor;			
			}

			if(oXml.FindChildElement("Direction",false))
			{
				this.m_v3Direction = new Vector3();
				Vec3d vDirection = Util.LoadVec3d(ref oXml, "Direction", this);
				m_v3Direction.X = (float)vDirection.X;
				m_v3Direction.Y = (float)vDirection.Y;
				m_v3Direction.Z = (float)vDirection.Z;
			}

			if(oXml.FindChildElement("OrientationMatrix", false))
			{
				m_mtxOrientation = Util_DX.LoadMatrix(ref oXml, "OrientationMatrix");
				m_v3Rotation = Util_DX.DecomposeXYZRotationMatrix(Orientation);
			}

			if(oXml.FindChildElement("TranslationMatrix",false))
				m_mtxTranslation = Util_DX.LoadMatrix(ref oXml,"TranslationMatrix");				

			oXml.OutOfElem();	
		
		}

		
		protected virtual void SaveLocation(ref AnimatTools.DataObjects.Simulation dsSim, ref AnimatTools.DataObjects.Physical.PhysicalStructure doStructure, ref AnimatTools.Interfaces.StdXml oXml)
		{
			Vector3 v3Loc = new Vector3();  //default to being the root which is always at (0,0,0)

			//if this is not the root save the location of the body
			//if(!this.IsRoot && this.Parent != null && Util.IsTypeOf(this.Parent.GetType(), typeof(RigidBodies.RigidBody_DX), false))
			if(this.Parent_DX != null)
				v3Loc = this.AbsoluteLocation - this.Parent_DX.AbsoluteLocation;
			
			//Save the Simulator location of this body which is v3Loc
			Util.SaveVector(ref oXml, "RelativePosition", new Vec3d(null, v3Loc.X, v3Loc.Y, v3Loc.Z));
			Util.SaveVector(ref oXml, "LocalPosition", new Vec3d(null, this.LocalLocation.X, this.LocalLocation.Y, this.LocalLocation.Z));
		}
		/// <summary>
		/// returns point ready for simulation
		/// </summary>
		/// <param name="current"></param>
		/// <returns></returns>
		protected Vector3 CenterOfMassCalculation(Vector3 current)
		{
			if(this.IsRoot)
				return current;

			RigidBodies.RigidBody_DX rbParent = (RigidBodies.RigidBody_DX)this.Parent;

			Vector3 v3ParentRotation = Util_DX.DecomposeXYZRotationMatrix(rbParent.CombinedTransformationMatrix);

			Matrix rotation = Matrix.RotationX(v3ParentRotation.X) * Matrix.RotationY(v3ParentRotation.Y) * Matrix.RotationZ(v3ParentRotation.Z);
			Matrix rotationInverse = rotation;
			rotationInverse.Invert();
			
			//get the absolute location of the parent and of this body
			Vector3 v3ParentAbsLoc= new Vector3(rbParent.CombinedTransformationMatrix.M41, rbParent.CombinedTransformationMatrix.M42, rbParent.CombinedTransformationMatrix.M43);
						
				
			//subtract the two vectors to get the relative location of this body
			Vector3 v3Loc = current - v3ParentAbsLoc;		
		
			v3Loc.TransformCoordinate(rotationInverse);		
			
			return current;

		}

		protected virtual void SaveRotation(ref AnimatTools.DataObjects.Simulation dsSim, ref AnimatTools.DataObjects.Physical.PhysicalStructure doStructure, ref AnimatTools.Interfaces.StdXml oXml)
		{	
			//The rotation vector to be saved
			Vector3 v3Rot = Vector3.Empty;
			String name = this.Name;
            
			//if this is not the root then calculate the rotation of the body using the combined transformation matrix
			if(!this.IsRoot && this.Parent != null && Util.IsTypeOf(this.Parent.GetType(), typeof(RigidBodies.RigidBody_DX), false))
			{				
				//Get the Eular Angles (X,Y,Z) using left-handed rotations
				v3Rot = Util_DX.DecomposeXYZRotationMatrix(CombinedTransformationMatrix);				

				//Calculate a right-handed rotation matrix using the left-handed eular angles
				Matrix m = Util_DX.RotateRightX(v3Rot.X) *
					Util_DX.RotateRightY(v3Rot.Y) *
					Util_DX.RotateRightZ(v3Rot.Z);

				//Now get the Eular Angles (X,Y,Z) using the right-handed rotations
				v3Rot = Util_DX.DecomposeXYZRotationMatrix(m);				
			}
			else
			{
				//Calculate a right-handed rotation matrix using the left-handed eular angles
				Matrix m = Util_DX.RotateRightX(Geometry.DegreeToRadian((float) this.Rotation.X)) *
					Util_DX.RotateRightY(Geometry.DegreeToRadian((float) this.Rotation.Y)) *
					Util_DX.RotateRightZ(Geometry.DegreeToRadian((float) this.Rotation.Z));
				
				//Now get the Eular Angles (X,Y,Z) using the right-handed rotations
				v3Rot = Util_DX.DecomposeXYZRotationMatrix(m);				
			}
					
			//save out the simulation rotation
			Util.SaveVector(ref oXml, "Rotation", new Vec3d(null, v3Rot.X, v3Rot.Y, v3Rot.Z));

			//save out the editor rotation
			Util.SaveVector(ref oXml, "LocalRotation", new Vec3d(null, Geometry.DegreeToRadian((float) this.Rotation.X), Geometry.DegreeToRadian((float) this.Rotation.Y), Geometry.DegreeToRadian((float) this.Rotation.Z)));
		}

		protected virtual void SaveCenterOfMass(ref AnimatTools.DataObjects.Simulation dsSim, ref AnimatTools.DataObjects.Physical.PhysicalStructure doStructure, ref AnimatTools.Interfaces.StdXml oXml)
		{	
			Vector3 point = new Vector3((float) m_snXCenterOfMass.ActualValue, (float) m_snYCenterOfMass.ActualValue, (float) m_snZCenterOfMass.ActualValue);
			point = this.CenterOfMassCalculation(point);

			Util.SaveVector(ref oXml, "CenterOfMass", new Vec3d(null, point.X, point.Y, point.Z));
		}
		
		public override void SaveData(ref AnimatTools.DataObjects.Simulation dsSim, ref AnimatTools.DataObjects.Physical.PhysicalStructure doStructure, ref AnimatTools.Interfaces.StdXml oXml)
		{
			//call the parents SaveData
			base.SaveData (ref dsSim, ref doStructure, ref oXml);
		
			oXml.IntoElem();  //step into element

			SaveCenterOfMass(ref dsSim, ref doStructure, ref oXml);
			
			SaveLocation(ref dsSim, ref doStructure, ref oXml);  //Save the location of this body
			
			SaveRotation(ref dsSim, ref doStructure, ref oXml); //Save the rotation of this body

			Util.SaveVector(ref oXml, "Direction", new Vec3d(null, this.Direction.X, this.Direction.Y, this.Direction.Z));
			
			Util.SaveColor(ref oXml, "Color", m_d3dMaterial.Diffuse); //save the color of this body

			Util_DX.SaveMatrix(ref oXml, "OrientationMatrix", Orientation);
			Util_DX.SaveMatrix(ref oXml, "TranslationMatrix", Translation);
			Util_DX.SaveMatrix(ref oXml, "CombinedTransformationMatrix", this.CombinedTransformationMatrix);

			oXml.OutOfElem(); //step out of the element
		}

		
		#endregion

		#region Utility Methods

		/// <summary>
		/// Releases any unmanaged resources
		/// </summary>
		protected virtual void CleanUp()
		{
			SafeRelease(ref m_d3dMesh);
			SafeRelease(ref m_mshSelectedBoundingBox);
			SafeRelease(ref m_d3dReceptiveFieldMesh);
			SafeRelease(ref m_d3dReceptiveFieldPoint);
			SafeRelease(ref m_mshCB);
			SafeRelease(ref m_mshLB);
			SafeRelease(ref m_mshAxisCylinder);
			SafeRelease(ref m_mshAxisCone);
			SafeRelease(ref m_mshAxisLabel);	

			m_aryReceptiveFields.Clear();

			System.GC.Collect();
					
		}

		/// <summary>
		/// Releases an unmanaged resources of a mesh
		/// </summary>
		/// <param name="mesh"></param>
		protected void SafeRelease(ref Mesh mesh)
		{
			if(mesh != null)
			{
				mesh.Dispose();
				mesh = null;				
			}
		}

		public override void RefreshSelectionBoxSizes()
		{
			float fltDistance = this.Location.Distance(this.ParentStructure.BodyEditor.BodyView.CameraPosition);
			float fltScale = (fltDistance/5)*0.05f;

			if(m_fltSelBoxSize != fltScale || m_mshCB == null || m_mshLB == null || m_mshCB.Disposed || m_mshLB.Disposed)
			{
				m_fltSelBoxSize = fltScale;				
				m_mshCB = Util_DX.CreateBillboard(Device,m_fltSelBoxSize, m_fltSelBoxSize);
				m_mshLB = Util_DX.CreateBillboard(Device,m_fltSelBoxSize, m_fltSelBoxSize);
			}
		}
		
				
		/// <summary>
		/// Finds a point on the surface of the mesh from intersection test
		/// </summary>
		/// <param name="d3dDevice">Direct3D device</param>
		/// <param name="v3Start">Starting location of the ray to test for intersection with surface</param>
		/// <param name="v3Direction">Direction of the ray to test for intersection with surface</param>
		/// <returns>Point on the surface of the mesh from the orgin of mesh in the direction of v3Direction</returns>
		public virtual Vector3 FindPointOnSurface(Vector3 v3Start, Vector3 v3Direction)
		{
			//IntersectInformation of the intersection test
			IntersectInformation iiII = new IntersectInformation();

			//if the mesh hasn't been created..create it
			if(m_d3dMesh == null)
				this.CreateBody();

			//test for intersection from the orgin to the surface in the v3Direction direction
			m_d3dMesh.Intersect(new Vector3(), v3Direction, out iiII);

			//get the face that was intersected
			Vector3[] v3Verts = Util_DX.FindIntersectionFace(m_d3dMesh, iiII);

			//get the normal of said face
			Vector3 v3N = Util_DX.CalculateNormal(v3Verts);
			
			//set //the direction of the body which is opposite the surface normal
			//m_v3Direction = -v3N;

			//this point will be negative..need to flip it
			return -1 * RayIntersectWithPlane(v3Start,v3N ,v3Verts);

		}

		protected Vector3 FindDirection()
		{
			Vector3 v3DirToParent = -this.LocalLocation;
			v3DirToParent.Normalize();

			//a temporary intersect information
			IntersectInformation iiInfo = new IntersectInformation();

			//check for intersection with parent
			this.Parent_DX.Mesh.Intersect(new Vector3(), v3DirToParent, out iiInfo);

			Vector3[] vertsFace = Util_DX.FindIntersectionFace(this.Parent_DX.Mesh, iiInfo);				
			Vector3 v3Norm = Util_DX.CalculateNormal(vertsFace);
			return -v3Norm;
		}
		/// <summary>
		/// Finds a point on a plane that a ray intersects with
		/// </summary>
		/// <param name="v3StartPosition">Starting position of the ray</param>
		/// <param name="v3Direction">Direction of the ray</param>
		/// <param name="vertsFace">Face of the mesh to test intersection with</param>
		/// <returns>Vector3: A point on the plane that was intersected</returns>
		private Vector3 RayIntersectWithPlane(Vector3 v3StartPosition, Vector3 v3Direction, Vector3[] vertsFace)
		{
			//representation of a the plane that was selected
			//P dot N = fltD
			//where P = point on the plane and N = normal of said plane
			float fltD = Vector3.Dot(vertsFace[0], v3Direction);
					
			//Starting point of the ray to test for intersection with the plane
			Vector3 v3P0 = v3StartPosition;

			//Direction of the ray to test for intersection with the plane
			Vector3 v3D = v3Direction;
					
			//distance which p0 intersects with the plane
			float t = 0;

			float fltDDotN = Vector3.Dot(v3D,v3Direction);

			//if v3D dot FaceNormal == 0 then there is no intersection so return zero vector
			if(fltDDotN != 0)
			{	
				//calculate the intersection distance
				t= (fltD - (Vector3.Dot(v3P0,v3Direction))/Vector3.Dot(v3D,v3Direction));

				//extend the normal to reach the point on the plane of intersection
				return  t * v3Direction;					
				
			}
			else
				return new Vector3();
		}

				
		public void InitDevice(Device d3dDevice)
		{
			this.Device = d3dDevice;
			this.Device.DeviceLost += new System.EventHandler(this.OnDeviceLost); 
			if(m_JointToParent != null)
			{
				VortexAnimatTools.DataObjects.Physical.Joints.Joint_DX jntJointToParent = (VortexAnimatTools.DataObjects.Physical.Joints.Joint_DX)m_JointToParent;
				jntJointToParent.Device = d3dDevice;
			}

			RigidBodies.RigidBody_DX dbChild = null;
			foreach (DictionaryEntry deEntry in m_aryChildBodies)
			{
				dbChild = (RigidBodies.RigidBody_DX)deEntry.Value;
				dbChild.InitDevice(d3dDevice);
			}		
		}		
		
		
		protected void  LoadTextures()
		{
			if(texCB == null)
			{
				//load center selectionbox graphic
				System.Reflection.Assembly myAssembly = System.Reflection.Assembly.Load("VortexAnimatTools");
				
				texCB = LoadTextureFromAssembly("ikpoint0.png", ref myAssembly);
				texLB = LoadTextureFromAssembly("ikpoint.png", ref myAssembly);
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

		protected virtual void CreateBody()
		{
			if((m_strTexture.Trim().Length > 0 && m_d3dTexture == null) || (m_d3dTexture != null && m_d3dTexture.Disposed) )
			{
				LoadTextureImage(m_strTexture);

				if(m_imgTexture != null)
				{
					m_d3dTexture = new Texture(m_d3dDevice, m_imgTexture, 0, Pool.Managed);
					m_d3dAlphaTexture = new Texture(m_d3dDevice, m_imgAlphaTexture, 0, Pool.Managed);
				}				
			}

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

		protected virtual void CreateReceptiveFields()
		{
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
		
		protected override void BuildProperties()
		{
			base.BuildProperties();

			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Color", this.Color.GetType(), "Color",
				"Part Properties", "Sets the color used to draw the body.", this.Color));
			
		}
		
		public void OrientBody()
		{
			Vector3 v3InitDir = new Vector3(0,0,1);
			
			float fltAngle = (float)Math.Acos(Vector3.Dot(v3InitDir,Direction));

			Vector3 v3Axis = Vector3.Cross(v3InitDir, Direction);

			m_mtxOrientation.RotateAxis(v3Axis,fltAngle);
			
			m_v3Rotation = Util_DX.DecomposeXYZRotationMatrix(m_mtxOrientation);
			this.Rotation = new Vec3d(null,Geometry.RadianToDegree(m_v3Rotation.X),Geometry.RadianToDegree(m_v3Rotation.Y),Geometry.RadianToDegree(m_v3Rotation.Z));
		}

		#region Receptive Field Methods

		protected virtual void CreateReceptiveFieldVertices()
		{
		}

		#endregion

		protected override void CloneInternal(AnimatTools.Framework.DataObject doOriginal, bool bCutData, AnimatTools.Framework.DataObject doRoot)
		{
			base.CloneInternal (doOriginal, bCutData, doRoot);

			RigidBody_Clone(doOriginal, bCutData, doRoot);
		}

		protected void RigidBody_Clone(AnimatTools.Framework.DataObject doOriginal, bool bCutData, AnimatTools.Framework.DataObject doRoot)
		{
			RigidBody_DX doOrigBody = (RigidBody_DX) doOriginal;

			m_d3dDevice = doOrigBody.m_d3dDevice;
			m_v3Rotation = new Microsoft.DirectX.Vector3(doOrigBody.m_v3Rotation.X, doOrigBody.m_v3Rotation.Y, doOrigBody.m_v3Rotation.Z);
			m_v3Direction = new Microsoft.DirectX.Vector3(doOrigBody.m_v3Direction.X, doOrigBody.m_v3Direction.Y, doOrigBody.m_v3Direction.Z);
			
			m_mtxOrientation = Util_DX.CloneMatrix(doOrigBody.m_mtxOrientation);
			m_mtxTranslation = Util_DX.CloneMatrix(doOrigBody.m_mtxTranslation);

			m_d3dMaterial.Ambient = doOrigBody.m_d3dMaterial.Ambient;
			m_d3dMaterial.Diffuse = doOrigBody.m_d3dMaterial.Diffuse;
			m_d3dTransparentMaterial.Ambient = doOrigBody.m_d3dTransparentMaterial.Ambient;
			m_d3dTransparentMaterial.Diffuse = doOrigBody.m_d3dTransparentMaterial.Diffuse;
			
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
			RigidBody_Clone(doOriginal, false, null);
		}

		#endregion
	
		#endregion

		#region Events
		private void OnDeviceLost(object sender, EventArgs e )
		{
			if(m_d3dMesh != null)
			{
				m_d3dMesh.Dispose();
				m_d3dMesh = null;
			}
		}
		#endregion
		
	}
}
