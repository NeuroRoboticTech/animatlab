using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.IO;
using System.Xml;
using Crownwood.Magic.Controls;
using AnimatTools.Framework;
using Microsoft.DirectX;
using Microsoft.DirectX.Direct3D;
using VortexAnimatTools.DataObjects.Physical.PropertyHelpers;
using VortexAnimatTools.DataObjects;

namespace VortexAnimatTools.DataObjects.Physical.RigidBodies
{
	/// <summary>
	/// Summary description for Box.
	/// </summary>
	public class BodyMesh : VortexAnimatTools.DataObjects.Physical.RigidBodies.Box
	{

		public enum enumMeshTypes {Convex, Regular};

		#region Attributes
		
		protected Vec3d m_vLowerLeftExtent;
		protected Vec3d m_vUpperRightExtent;
		protected Vec3d m_vLoadSize;
		protected Vec3d m_vScale;

		protected enumMeshTypes m_eCollisionMeshType;
		protected string m_strMeshFile = "";
		protected string m_strCollisionMeshFile = "";

		protected Microsoft.DirectX.Direct3D.Mesh m_d3dCollisionMesh;
		protected Microsoft.DirectX.Direct3D.Material m_d3dCollisionMaterial;

		#endregion

		#region Properties

		public override String ImageName {get{return "VortexAnimatTools.Graphics.Box_Treeview.gif";}}
		public override String ButtonImageName {get{return "VortexAnimatTools.Graphics.Box_Button.gif";}}
		public override String Type {get{return "Mesh";}}
		public override String BodyPartName {get{return "Mesh";}}
		public override System.Type PartType {get{return typeof(VortexAnimatTools.DataObjects.Physical.RigidBodies.BodyMesh);}}

		[EditorAttribute(typeof(System.Windows.Forms.Design.FileNameEditor), typeof(System.Drawing.Design.UITypeEditor))]
		public string MeshFile 
		{
			get
			{return m_strMeshFile;}
			set
			{
				

				string strPath="", strFile="";

				if(value != null)
				{
					if(Util.IsFullPath(value))
					{
						if(Util.DetermineFilePath(value, ref strPath, ref strFile))
							value = strFile;
					}
					else
						strFile = value;
				}

				string strExtension = Util.GetFileExtension(strFile).Trim();
				if(strExtension.ToLower() != "obj")
					throw new System.Exception("Mesh files must be .obj files.");

				string strAseFile = strPath + strFile.Replace(strExtension, "ase");
				string strObjFile = strPath + strFile;
 
				if(!File.Exists(strObjFile))
					throw new System.Exception("The object file was not found!");

				if(!File.Exists(strAseFile))
					throw new System.Exception("Mesh files must have both a corresponding obj and ase version of the files. I am sorry for this inconvenience. This will change once the editor is converted to open scene graph.");

				m_strMeshFile = value;

				if(m_d3dMesh != null)
				{
					m_d3dMesh.Dispose();
					m_d3dMesh = null;
				}				

				CreateBody();
			}
		}

		[EditorAttribute(typeof(System.Windows.Forms.Design.FileNameEditor), typeof(System.Drawing.Design.UITypeEditor))]
		public string CollisionMeshFile 
		{
			get
			{return m_strCollisionMeshFile;}
			set
			{
				if(value != null)
				{
					string strPath="", strFile="";
					if(Util.DetermineFilePath(value, ref strPath, ref strFile))
						value = strFile;
				}

				m_strCollisionMeshFile = value;
			}
		}
		
		public Vec3d LowerLeftExtent 
		{
			get
			{return m_vLowerLeftExtent;}
			set
			{m_vLowerLeftExtent = value;}
		}
		
		public Vec3d UpperRightExtent 
		{
			get
			{return m_vUpperRightExtent;}
			set
			{m_vUpperRightExtent = value;}
		}
				
		public enumMeshTypes CollisionMeshType 
		{
			get
			{return m_eCollisionMeshType;}
			set
			{m_eCollisionMeshType = value;}
		}

		public Vec3d LoadSize 
		{get {return m_vLoadSize;}}

		#endregion

		#region Methods

		public BodyMesh(AnimatTools.Framework.DataObject doParent) : base(doParent)
		{
			m_vGraphicsBoxSize = new Vec3d(this, 0, 0, 0);
			m_vLowerLeftExtent = new Vec3d(this, 0, 0, 0);
			m_vUpperRightExtent = new Vec3d(this, 0, 0, 0);
			m_vScale = new Vec3d(this);
			m_vScale.X = m_vScale.Y = m_vScale.Z = 1;

			m_vrSize = new DxBoxSizeRef(this);
			m_eCollisionMeshType = enumMeshTypes.Convex;
		}

		public override void SetSize(float width, float height, float depth)
		{
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
			m_vGraphicsBoxSize.X = m_vLoadSize.X;
			m_vGraphicsBoxSize.Y = m_vLoadSize.Y;
			m_vGraphicsBoxSize.Z = m_vLoadSize.Z;

//			//first we need to figure out how much we need to unscale the mesh to get it back to its original state.
//			Vec3d vUnScale = new Vec3d(null, 1/m_vScale.X, 1/m_vScale.Y, 1/m_vScale.Z);
//
//
//			//Now lets figure out how much we need to rescale it for the new size value.
//			Vec3d vScale = Util_DX.FindScaleFactor(m_vGraphicsBoxSize, m_vLoadSize);
//			m_vScale = (Vec3d) vScale.Clone(this);
//			vScale.X *= vUnScale.X; vScale.Y *= vUnScale.Y; vScale.Z *= vUnScale.Z;
//
//			//Util_DX.ScaleMesh(m_d3dMesh, vScale);

			float fltScale = (this.MaxDimension * 0.15f);
			if(fltScale > 0.3f) fltScale = 0.3f;

			m_mshSelectedBoundingBox = Mesh.Box(Device, (float)(m_vGraphicsBoxSize.X + fltScale),(float)(m_vGraphicsBoxSize.Y + fltScale), (float)(m_vGraphicsBoxSize.Z + fltScale)); 

			fltScale = (this.MaxDimension * 0.1f);
			m_mshCB = Mesh.Box(Device, fltScale, fltScale, 0.001f);

			fltScale = (this.MaxDimension * 0.05f);
			m_mshLB = Mesh.Sphere(Device, fltScale, 16, 16);
		}
 
		protected override void CreateBody()
		{
			if(m_d3dMesh == null)
			{
				if(m_strMeshFile != null && m_strMeshFile.Trim().Length > 0)
				{
					Vector3 vLower = new Vector3(0,0,0), vUpper = new Vector3(0,0,0);

					OBJLoader loader = null;
					if(Util.IsFullPath(m_strMeshFile))					
						loader = new OBJLoader(m_strMeshFile, m_d3dDevice);						
					else
						loader = new OBJLoader(Util.Application.ProjectPath + m_strMeshFile, m_d3dDevice);
						

					if(loader.IsLoaded)
					{
						m_d3dMesh = loader.Mesh;
						m_d3dMaterial = loader.Material;
						this.Color = m_d3dMaterial.Diffuse;
						m_d3dMaterial.Ambient = Color.FromArgb(m_iAlpha, m_d3dMaterial.Ambient);
						m_d3dMaterial.Diffuse = Color.FromArgb(m_iAlpha, m_d3dMaterial.Ambient);

						//if(loader.Name != "" && (this.Name.ToLower() == "root" || this.Name.ToLower().StartsWith("body_")))
						//	this.Name = loader.Name;
						m_vLowerLeftExtent = new Vec3d(null, loader.LowerLeftExtent.X, loader.LowerLeftExtent.Y, loader.LowerLeftExtent.Z);
						m_vUpperRightExtent = new Vec3d(null, loader.UpperRightExtent.X, loader.UpperRightExtent.Y, loader.UpperRightExtent.Z);						
						
					}
					else
						throw new Exception("OBJ " + m_strMeshFile + " failed to load");
						
				}   
				else
				{
					m_d3dMesh = Util_DX.CreateBox(m_d3dDevice, 1, 1, 1);
					m_vLowerLeftExtent = new Vec3d(null, -1, -1, -1);
					m_vLowerLeftExtent = new Vec3d(null, 1, 1, 1);
				}

				//Util_DX.ScaleMesh(m_d3dMesh, m_vScale);
				m_vLoadSize = new Vec3d(this, Math.Abs(m_vUpperRightExtent.X-m_vLowerLeftExtent.X), Math.Abs(m_vUpperRightExtent.Y-m_vLowerLeftExtent.Y), Math.Abs(m_vUpperRightExtent.Z-m_vLowerLeftExtent.Z));

				if(m_vScale.X == 1 && m_vScale.Y == 1 && m_vScale.Z == 1)
					m_vGraphicsBoxSize = this.LoadSize;
			}

			SetSize((float) m_vGraphicsBoxSize.X, (float) m_vGraphicsBoxSize.Y, (float) m_vGraphicsBoxSize.Z);

			if(m_d3dCollisionMesh == null)
			{
				if(m_strCollisionMeshFile != null && m_strCollisionMeshFile.Trim().Length > 0)
				{
					OBJLoader loader = null;
					if(Util.IsFullPath(m_strCollisionMeshFile))					
						loader = new OBJLoader(m_strCollisionMeshFile, m_d3dDevice);						
					else
						loader = new OBJLoader(Util.Application.ProjectPath + m_strCollisionMeshFile, m_d3dDevice);
						
					if(loader.IsLoaded)
					{
						m_d3dCollisionMesh = loader.Mesh;
						m_d3dCollisionMaterial = loader.Material;
					}
					else
						throw new Exception("OBJ " + m_strMeshFile + " failed to load");						
				}
				else
				{
					m_d3dCollisionMesh = Util_DX.CreateBox(m_d3dDevice, 1, 1, 1);
				}
			}
						
			base.CreateBody();
		}

		protected override void CreateReceptiveFields()
		{
			if(this.Device != null && !this.Device.Disposed)
			{
				m_d3dReceptiveFieldMesh = this.m_d3dCollisionMesh;
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

			if(m_d3dReceptiveFieldMesh != null)
			{
				GraphicsStream buffer = m_d3dReceptiveFieldMesh.LockVertexBuffer(LockFlags.ReadOnly);
				CustomVertex.PositionNormalTextured[] cvVerts = new CustomVertex.PositionNormalTextured[m_d3dReceptiveFieldMesh.NumberVertices];

				for(int i=0; i<m_d3dReceptiveFieldMesh.NumberVertices; i++)
				{
					buffer.Position = i * m_d3dReceptiveFieldMesh.NumberBytesPerVertex;
					cvVerts[i] = (CustomVertex.PositionNormalTextured)buffer.Read(typeof(CustomVertex.PositionNormalTextured));
					
					m_aryReceptiveFields.Add(new AnimatTools.Framework.Vec3d(null, cvVerts[i].X, cvVerts[i].Y, cvVerts[i].Z));
					//AddReceptiveField(cvVerts[i].X, cvVerts[i].Y, cvVerts[i].Z);
				}
				m_d3dReceptiveFieldMesh.UnlockVertexBuffer();
				
				SortReceptiveFields();
				//DumpReceptiveFields();
				VerifyReceptiveFielPairs();
			}
		}

		public override AnimatTools.DataObjects.Physical.BodyPart CreateNewBodyPart(AnimatTools.Framework.DataObject doParent)
		{return new BodyMesh(doParent);}

		protected override void CloneInternal(AnimatTools.Framework.DataObject doOriginal, bool bCutData, AnimatTools.Framework.DataObject doRoot)
		{
			base.CloneInternal (doOriginal, bCutData, doRoot);

			BodyMesh doOrigBody = (BodyMesh) doOriginal;

			m_vLowerLeftExtent = doOrigBody.m_vLowerLeftExtent;
			m_vUpperRightExtent = doOrigBody.m_vUpperRightExtent;
			m_vLoadSize = doOrigBody.m_vLoadSize;
			m_vScale = doOrigBody.m_vScale;
			m_eCollisionMeshType = doOrigBody.m_eCollisionMeshType;
			this.MeshFile = doOrigBody.m_strMeshFile;
			this.CollisionMeshFile = doOrigBody.m_strCollisionMeshFile;
		}

		public override AnimatTools.Framework.DataObject Clone(AnimatTools.Framework.DataObject doParent, bool bCutData, AnimatTools.Framework.DataObject doRoot)
		{
			BodyMesh bnPart = new BodyMesh(doParent);
			bnPart.CloneInternal(this, bCutData, doRoot);
			if(doRoot != null && doRoot == this) bnPart.AfterClone(this, bCutData, doRoot, bnPart);
			return bnPart;
		}

		protected override void BuildProperties()
		{
			base.BuildProperties();
			
			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Mesh File", m_strMeshFile.GetType(), "MeshFile", 
                                  "Part Properties", "Sets the mesh file to use for this body part.", 
                                  m_strMeshFile, typeof(System.Windows.Forms.Design.FileNameEditor)));

			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Collision Mesh File", m_strCollisionMeshFile.GetType(), "CollisionMeshFile", 
																	"Part Properties", "Sets the mesh file to use for the collision mesh of this body part.", 
																	m_strCollisionMeshFile, typeof(System.Windows.Forms.Design.FileNameEditor)));

			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Collision Mesh Type", m_eCollisionMeshType.GetType(), "CollisionMeshType", 
																	"Part Properties", "Determines if the collision mesh type is a convex mesh or a regular mesh.", m_eCollisionMeshType));

			m_Properties.Properties.Remove("Height");
			m_Properties.Properties.Remove("Width");
			m_Properties.Properties.Remove("Length");

			Crownwood.Magic.Controls.PropertyBag pbNumberBag  = this.HeightScaled.Properties;
			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Height", pbNumberBag.GetType(), "HeightScaled", 
																	"Size", "Sets the height of the box.", pbNumberBag,
																	"", typeof(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter), true));

			pbNumberBag  = this.WidthScaled.Properties;
			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Width", pbNumberBag.GetType(), "WidthScaled", 
																	"Size", "Sets the width of the box.", pbNumberBag,
																	"", typeof(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter), true));

			pbNumberBag  = this.LengthScaled.Properties;
			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Length", pbNumberBag.GetType(), "LengthScaled", 
																	"Size", "Sets the length of the box.", pbNumberBag,
																	"", typeof(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter), true));
		}

		#region Loading and Saving Data

		public override void LoadData(ref AnimatTools.DataObjects.Simulation dsSim, ref AnimatTools.DataObjects.Physical.PhysicalStructure doStructure, ref AnimatTools.Interfaces.StdXml oXml)
		{
			base.LoadData (ref dsSim, ref doStructure, ref oXml);

			oXml.IntoElem();
			m_vScale = Util.LoadVec3d(ref oXml, "Scale", null);
	        m_eCollisionMeshType = (enumMeshTypes) Enum.Parse(typeof(enumMeshTypes), oXml.GetChildString("CollisionMeshType"), true);
			m_strMeshFile = oXml.GetChildString("MeshFile");
			m_strCollisionMeshFile = oXml.GetChildString("CollisionMeshFile");
			oXml.OutOfElem(); //out of body			

			m_strMeshFile = Util.VerifyFilePath(m_strMeshFile);
			m_strCollisionMeshFile = Util.VerifyFilePath(m_strCollisionMeshFile);
		}

		public override void SaveData(ref AnimatTools.DataObjects.Simulation dsSim, ref AnimatTools.DataObjects.Physical.PhysicalStructure doStructure, ref AnimatTools.Interfaces.StdXml oXml)
		{
			base.SaveData (ref dsSim, ref doStructure, ref oXml);

			oXml.IntoElem();
			Util.SaveVector(ref oXml, "Scale", m_vScale);
			oXml.AddChildElement("MeshFile", m_strMeshFile);
			oXml.AddChildElement("CollisionMeshFile", m_strCollisionMeshFile);
			
			if(m_strCollisionMeshFile == null || m_strCollisionMeshFile.Trim().Length == 0)
				m_eCollisionMeshType = enumMeshTypes.Regular;

				oXml.AddChildElement("CollisionMeshType", m_eCollisionMeshType.ToString());

			oXml.OutOfElem(); //out of body			
		}

		#endregion

		#endregion
	}
}
