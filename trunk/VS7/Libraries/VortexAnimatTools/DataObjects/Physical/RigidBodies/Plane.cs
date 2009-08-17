using System;
using AnimatTools.Framework;
using VortexAnimatTools.DataObjects.Physical.PropertyHelpers;

namespace VortexAnimatTools.DataObjects.Physical.RigidBodies
{
	/// <summary>
	/// Summary description for Plane.
	/// </summary>
	public class Plane : VortexAnimatTools.DataObjects.Physical.RigidBodies.RigidBody_DX
	{

		#region Attributes

		protected string m_strHeightFieldImage = "";
		protected AnimatTools.Framework.Vec3d m_vMapLocation;
		protected AnimatTools.Framework.Vec3d m_vMapSize;
		protected float m_fltMapScale = 1;

		protected float m_fltHeight = 0;

		#endregion

		#region Properties

		public override String ImageName {get{return "VortexAnimatTools.Graphics.Plane_Treeview.gif";}}
		public override String Type {get{return "Plane";}}
		public override String BodyPartName {get{return "Plane";}}
		public override System.Type PartType {get{return typeof(VortexAnimatTools.DataObjects.Physical.RigidBodies.Plane);}}
		public override bool AllowUserAdd {get{return false;}}
		public override bool CanBeRootBody {get{return false;}}
		public override bool HasDynamics {get{return false;}}

		public override float YWorldLocation
		{
			get{return base.YWorldLocation;}
			set 
			{
				base.YWorldLocation = value; m_fltHeight=value;
				InvalidateBodyView();
			}
		}
		
		public string HeightFieldImage
		{
			get {return m_strHeightFieldImage;}
			set 
			{
				if(value != null)
				{
					string strPath = "", strFile = "";
					if(Util.DetermineFilePath(value, ref strPath, ref strFile))
						value = strFile;
				}

				m_strHeightFieldImage = value;
			}
		}

		public AnimatTools.Framework.Vec3d MapLocation
		{
			get{return m_vMapLocation;}
			set {m_vMapLocation=value;}
		}

		public AnimatTools.Framework.Vec3d MapSize
		{
			get{return m_vMapSize;}
			set 
			{
				if(value.X <= 0 || value.Y <= 0)
					throw new System.Exception("The map size must be greater than 0.");

				m_vMapSize = value;
			}
		}

		public float MapScale
		{
			get{return m_fltMapScale;}
			set 
			{
				if(value <=0)
					throw new System.Exception("The map scale must be greater than 0.");
				m_fltMapScale = value;
			}
		}

		public override float MaxDimension {get{return 0;}}
		public override float MinDimension {get{return 0;}}

		#endregion

		#region Methods

		public Plane(AnimatTools.Framework.DataObject doParent) : base(doParent)
		{
			m_vMapLocation = new AnimatTools.Framework.Vec3d(this, 0, 0, 0);
			m_vMapSize = new AnimatTools.Framework.Vec3d(this, 100, 100, 0);
		}

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

			if(m_mshLB != null)
			{
				this.m_mshLB.Dispose();
				this.m_mshLB = null;
			}

			m_d3dMesh = Util_DX.CreatePlane(this.Device, 5, 10, (float) this.YLocalLocation);

			base.CreateBody();
		}

		protected override void DrawReceptiveFields(AnimatTools.Forms.BodyPlan.Command.enumCommandMode cmCommand)
		{
			this.DrawRigidBody(cmCommand);
		}

		public override AnimatTools.DataObjects.Physical.BodyPart CreateNewBodyPart(AnimatTools.Framework.DataObject doParent)
		{return new Plane(doParent);}		

		protected override void CloneInternal(AnimatTools.Framework.DataObject doOriginal, bool bCutData, AnimatTools.Framework.DataObject doRoot)
		{
			base.CloneInternal (doOriginal, bCutData, doRoot);

			Plane doOrigBody = (Plane) doOriginal;
			m_fltHeight = doOrigBody.m_fltHeight;
			m_strHeightFieldImage = doOrigBody.m_strHeightFieldImage;
			m_vMapLocation = (AnimatTools.Framework.Vec3d) doOrigBody.m_vMapLocation.Clone(this, bCutData, doRoot);
			m_vMapSize  = (AnimatTools.Framework.Vec3d) doOrigBody.m_vMapSize.Clone(this, bCutData, doRoot);
			m_fltMapScale = doOrigBody.m_fltMapScale;
		}

		public override AnimatTools.Framework.DataObject Clone(AnimatTools.Framework.DataObject doParent, bool bCutData, AnimatTools.Framework.DataObject doRoot)
		{
			Plane bnPart = new Plane(doParent);
			bnPart.CloneInternal(this, bCutData, doRoot);
			if(doRoot != null && doRoot == this) bnPart.AfterClone(this, bCutData, doRoot, bnPart);
			return bnPart;
		}

		protected override void BuildProperties()
		{
			base.BuildProperties();
			
			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Size", this.Size.GetType(), "Size", 
																	"Part Properties", "The size of the box.", this.Size, 
																	"", typeof(AnimatTools.TypeHelpers.Vec3dTypeConverter)));

			if(this.ParentStructure != null && this.ParentStructure is AnimatTools.DataObjects.Physical.GroundSurface)
			{
				m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Height Field Image", typeof(String), "HeightFieldImage",
																		"Height Field Properties", "Sets the bmp file that contains height field information for this surface.",
																		this.HeightFieldImage, typeof(System.Windows.Forms.Design.FileNameEditor)));

				bool bHeightField = true;
				if(m_strHeightFieldImage.Trim().Length > 0)
					bHeightField = false;

				m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Map Location", typeof(System.Drawing.PointF), "MapLocation", 
																		"Height Field Properties", "The starting location for the height field map.", this.MapLocation,
																		"", typeof(AnimatTools.TypeHelpers.Vec3dTypeConverter), bHeightField));

				m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Map Size", typeof(System.Drawing.PointF), "MapSize", 
																		"Height Field Properties", "The overall size of the height field map.", this.MapSize,
																		"", typeof(AnimatTools.TypeHelpers.Vec3dTypeConverter), bHeightField));

				m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Map Scale", m_fltMapScale.GetType(), "MapScale", 
																		"Height Field Properties", "The scale factor used to determine the height of the ground using the image file.",
																		this.MapSize, bHeightField));
			}
		}

		#region Loading and Saving Data

		public override void LoadData(ref AnimatTools.DataObjects.Simulation dsSim, ref AnimatTools.DataObjects.Physical.PhysicalStructure doStructure, ref AnimatTools.Interfaces.StdXml oXml)
		{
			base.LoadData (ref dsSim, ref doStructure, ref oXml);

			oXml.IntoElem();
			m_fltHeight = oXml.GetChildFloat("Height", m_fltHeight);
			m_strHeightFieldImage = oXml.GetChildString("HeightFieldImage", m_strHeightFieldImage);
	
			if(oXml.FindChildElement("MapLocation", false))
				m_vMapLocation = Util.LoadVec3d(ref oXml, "MapLocation", this);

			if(oXml.FindChildElement("MapSize", false))
				m_vMapSize = Util.LoadVec3d(ref oXml, "MapSize", this);

			m_fltMapScale = oXml.GetChildFloat("MapScale", m_fltMapScale);
			oXml.OutOfElem(); //out of body			
		}

		public override void SaveData(ref AnimatTools.DataObjects.Simulation dsSim, ref AnimatTools.DataObjects.Physical.PhysicalStructure doStructure, ref AnimatTools.Interfaces.StdXml oXml)
		{
			base.SaveData (ref dsSim, ref doStructure, ref oXml);

			oXml.IntoElem();
			oXml.AddChildElement("Height", m_fltHeight);
			oXml.AddChildElement("HeightFieldImage", m_strHeightFieldImage);
			Util.SaveVector(ref oXml, "MapLocation", m_vMapLocation);
			Util.SaveVector(ref oXml, "MapSize", m_vMapSize);
			oXml.AddChildElement("MapScale", m_fltMapScale);
			oXml.OutOfElem(); //out of body			

		}

		#endregion

		#endregion

	}
}
