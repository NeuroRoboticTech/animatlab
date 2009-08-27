using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
//DirectX Library
using Microsoft.DirectX;
using Microsoft.DirectX.Direct3D;
using RigidBodies = VortexAnimatTools.DataObjects.Physical.RigidBodies;
using Joints = VortexAnimatTools.DataObjects.Physical.Joints;
using VortexAnimatTools;
using System.Diagnostics;
using AnimatTools.Framework;
using VortexAnimatTools.DataObjects;

namespace VortexAnimatTools.Forms.BodyPlan
{
	/// <summary>
	/// Summary description for BodyViewD3D.
	/// </summary>
	
	public class BodyViewD3D : AnimatTools.Forms.BodyPlan.BodyView
	{
		/// <summary>
		/// Required designer variable.
		/// </summary>
		
		#region Attributes
		private Device m_d3dDevice;
		private System.ComponentModel.Container m_Components = null;

		//axis meshes
		private Mesh m_mshAxisCylinder;
		private Mesh m_mshAxisCone;
		private Mesh m_mshAxisLabel;
		private Material m_matXAxis;
		private Material m_matYAxis;
		private Material m_matZAxis;

		//rendering to surface variables
		private Texture renderTexture = null;
		private Surface renderSurface = null;
		private RenderToSurface rts = null;
		private const int RenderSurfaceSize = 128;
			
		//button modifiers
		private bool m_bXDown;
		private bool m_bYDown;
		private bool m_bZDown;
		private bool m_bSDown;

		//rotation factor
		private float m_fltRotateX = 0.0f;
		private float m_fltRotateY = 0.0f;
		private float m_fltRotateZ = 0.0f;
		private Matrix m_mtxOrientation = Matrix.Identity;

		//pan factor
		private float m_fltPanX = 0.0f;
		private float m_fltPanY = 0.0f;

		//zoom factor
		private float m_fltZoom = -5.0f;

		//culling planes for camera
		private float m_fltNearPlane = 0.3f;
		private float m_fltFarPlane = 500;

		//previous mouse state
		private int m_iOldX;
		private int m_iOldY;

		//current mouse state
		private int m_iCurrentX = -1;
		private int m_iCurrentY = -1;

		private bool m_bReset = false;
		private bool m_bTransformWorld = false;
		private bool m_bMouseMove = false;

		private Texture texX = null;
		private Texture texY = null;
		private Texture texZ = null;

		//private Texture texTransBillBoard = null;
		//private GraphicsStream  gsStream = null;

		protected AnimatTools.DataObjects.Physical.PartPositionState m_psMovePositionState = null;

		PresentParameters m_ppPresentParams;

		#endregion 

		#region Properties
		/// <summary>
		/// Gets the currently selected RigidBody
		/// </summary>
		public Microsoft.DirectX.Direct3D.Device Device 
		{get{return m_d3dDevice;}}
		
		public float PanX 
		{
			get{return m_fltPanX;}
			set
			{
				m_fltPanX = value;
				if(this.Editor != null && this.Editor.PropertiesBar != null && this.Editor.PropertiesBar.SelectedPart != null)
					this.Editor.PropertiesBar.SelectedPart.RefreshSelectionBoxSizes();
			}
		}
		
		public float PanY 
		{
			get{return m_fltPanY;}
			set
			{
				m_fltPanY = value;
				if(this.Editor != null && this.Editor.PropertiesBar != null && this.Editor.PropertiesBar.SelectedPart != null)
					this.Editor.PropertiesBar.SelectedPart.RefreshSelectionBoxSizes();
			}
		}
		
		public float Zoom 
		{
			get{return m_fltZoom;}
			set
			{
				m_fltZoom = value;
				if(this.Editor != null && this.Editor.PropertiesBar != null && this.Editor.PropertiesBar.SelectedPart != null)
					this.Editor.PropertiesBar.SelectedPart.RefreshSelectionBoxSizes();
			}
		}
		
		public float NearPlane 
		{
			get{return m_fltNearPlane;}
			set
			{
				m_fltNearPlane = value;
			}
		}
		
		public float FarPlane 
		{
			get{return m_fltFarPlane;}
			set
			{
				m_fltFarPlane = value;
			}
		}

		public override Vec3d CameraPosition
		{get {return new AnimatTools.Framework.Vec3d(null, m_fltPanX, m_fltPanY, m_fltZoom);}}

		#endregion

		#region Methods

		public BodyViewD3D()
		{
			InitializeComponent();
			InitializeDevice();	
			this.SetStyle(ControlStyles.Opaque | ControlStyles.AllPaintingInWmPaint , true);

			m_mshAxisCylinder = Mesh.Cylinder(m_d3dDevice,0.1f, 0.1f, 2.0f, 16, 16);
			m_mshAxisCone = Mesh.Cylinder(m_d3dDevice,0.0f, 0.3f, 0.5f, 16, 16);
			m_matXAxis = new Material();
			m_matXAxis.Diffuse = Color.Red;
			m_matYAxis = new Material();
			m_matYAxis.Diffuse = Color.Green;
			m_matZAxis = new Material();
			m_matZAxis.Diffuse = Color.Blue;

			m_mshAxisLabel = Util_DX.CreateBillboard(m_d3dDevice, 0.5f, 0.5f);

			//this.m_fltRotateX = Geometry.DegreeToRadian(180);
			Util.Application.UnitsChanged += new AnimatTools.Forms.AnimatApplication.UnitsChangedEventHandler(Application_UnitsChanged);

			try
			{
				System.Reflection.Assembly myAssembly = System.Reflection.Assembly.Load("VortexAnimatTools");
				System.IO.Stream stream = myAssembly.GetManifestResourceStream("VortexAnimatTools.Graphics.X.png");
					
				//load texture from stream
				texX = TextureLoader.FromStream(Device,stream);
				//Debug.WriteLine("Loaded Texture X.png");

				stream = myAssembly.GetManifestResourceStream("VortexAnimatTools.Graphics.Y.png");
				texY = TextureLoader.FromStream(Device,stream);
				//Debug.WriteLine("Loaded Texture Y.png");

				stream = myAssembly.GetManifestResourceStream("VortexAnimatTools.Graphics.Z.png");
				texZ = TextureLoader.FromStream(Device,stream);
				//Debug.WriteLine("Loaded Texture Z.png");
			
			}
			catch(Exception ex)
			{
				Util.DisplayError(ex);
			}
		}

		public void InitializeDevice()
		{
			m_ppPresentParams = new PresentParameters();
			m_ppPresentParams.Windowed = true;
			m_ppPresentParams.SwapEffect = SwapEffect.Discard;	
			m_ppPresentParams.EnableAutoDepthStencil = true;
			m_ppPresentParams.AutoDepthStencilFormat = DepthFormat.D16;

			//Caps hardware = Manager.GetDeviceCaps(0,DeviceType.Hardware);

			//Default to software processing

			//Use hardware if it's available
			//if(hardware.DeviceCaps.SupportsHardwareTransformAndLight)
			//	flags = CreateFlags.HardwareVertexProcessing;

			//Use pure if it's available
			//if(hardware.DeviceCaps.SupportsPureDevice)
			//	flags |= CreateFlags.PureDevice;

			m_d3dDevice = new Device(0, DeviceType.Hardware, this, CreateFlags.SoftwareVertexProcessing , m_ppPresentParams);

			m_d3dDevice.DeviceReset += new System.EventHandler(this.OnResetDevice);
			this.OnResetDevice(m_d3dDevice, null);
		}

		public void OnResetDevice(object sender, EventArgs e)
		{
			Device d3dDevice = (Device)sender;

			// Turn on the ZBuffer
			d3dDevice.RenderState.ZBufferEnable = true;
			
			d3dDevice.RenderState.CullMode = Cull.None;
			
			d3dDevice.RenderState.Lighting = true;		
	
			// Enable alpha blending.
			d3dDevice.RenderState.AlphaBlendEnable = true;
			
			// Set the source blend state.
			d3dDevice.RenderState.SourceBlend = Blend.SourceAlpha;
 
			// Set the destination blend state.
			d3dDevice.RenderState.DestinationBlend = Blend.InvSourceAlpha;

			d3dDevice.RenderState.AlphaTestEnable = true;
			d3dDevice.RenderState.Ambient = Color.FromArgb(255, 100, 100, 100);

			//set up the lights
			d3dDevice.Lights[0].Type = LightType.Directional;
			d3dDevice.Lights[0].Diffuse = Color.White;
			d3dDevice.Lights[0].Direction = new Vector3(0, 0, 1);
			d3dDevice.Lights[0].Enabled = true;

			d3dDevice.Lights[1].Type = LightType.Directional;
			d3dDevice.Lights[1].Diffuse = Color.White;
			d3dDevice.Lights[1].Direction = new Vector3(0, -1, 0);
			d3dDevice.Lights[1].Enabled = true;
			
			//the device was reset
			m_bReset = true;

			//create the render to surface object
			rts = new RenderToSurface(d3dDevice, RenderSurfaceSize, RenderSurfaceSize, Format.A8R8G8B8, true, DepthFormat.D16);

			//create render texture
			renderTexture = new Texture(d3dDevice, RenderSurfaceSize, RenderSurfaceSize, 1,Usage.RenderTarget, Format.A8R8G8B8, Pool.Default);

			//create the rendesurface texture
			renderSurface = renderTexture.GetSurfaceLevel(0);
			
			
		}

		private void SetupCamera()
		{
			m_d3dDevice.Transform.Projection = Matrix.PerspectiveFovLH((float)Geometry.DegreeToRadian(45), this.Width / this.Height, m_fltNearPlane, m_fltFarPlane);
			m_d3dDevice.Transform.View = Matrix.LookAtLH(new Vector3(m_fltPanX, m_fltPanY, m_fltZoom), new Vector3(m_fltPanX, m_fltPanY, 0), new Vector3(0,1,0));			
			m_d3dDevice.Transform.World1 = Matrix.RotationY(m_fltRotateX) * Matrix.RotationX(m_fltRotateY);
//			m_d3dDevice.Transform.World1 = m_mtxOrientation;
						
		}

		private void RenderIntoSurface()
		{
			//render to this surface
			Viewport view = new Viewport();
			view.Width = RenderSurfaceSize;
			view.Height = RenderSurfaceSize;
			view.MaxZ = 1.0f;

			rts.BeginScene(renderSurface, view);

			m_d3dDevice.Clear(ClearFlags.Target | ClearFlags.ZBuffer, Color.FromArgb(0, 0, 0, 0).ToArgb(), 1.0f, 0);
			m_d3dDevice.Transform.Projection = Matrix.PerspectiveFovLH((float)Geometry.DegreeToRadian(45), this.Width / this.Height, 1.0f, 100.0f);
			m_d3dDevice.Transform.View = Matrix.LookAtLH(new Vector3(0, 0, -7), new Vector3(0, 0, 0), new Vector3(0,1,0));			
			m_d3dDevice.Transform.World1 = Matrix.RotationY(m_fltRotateX) * Matrix.RotationX(m_fltRotateY);
//			m_d3dDevice.Transform.World1 = m_mtxOrientation;

			this.DrawAxis();
			rts.EndScene(Filter.None);
			
		}
		
		protected override void OnPaint(System.Windows.Forms.PaintEventArgs e)
		{
			try
			{
				RenderIntoSurface();

				m_d3dDevice.Clear(ClearFlags.Target | ClearFlags.ZBuffer, m_clBackColor , 1.0f, 0);
				SetupCamera();
						
				m_d3dDevice.BeginScene();	
		
				if(this.Editor != null && this.Editor.PhysicalStructure != null && m_d3dDevice != null)
					if(Editor.PhysicalStructure.RootBody != null)
					{
						RigidBodies.RigidBody_DX rbRoot = (RigidBodies.RigidBody_DX)Editor.PhysicalStructure.RootBody;					
						if(rbRoot.Device == null || m_bReset)
						{
							rbRoot.InitDevice(m_d3dDevice);	
							m_bReset = false;
							this.Invalidate();
						}
						else
							Editor.PhysicalStructure.Draw(Editor.CommandBar.CommandMode);
					}
			
				using (Sprite s = new Sprite(m_d3dDevice))
				{
					//save the renderTexture to a graphics stream
					//gsStream = TextureLoader.SaveToStream(ImageFileFormat.Dds, renderTexture);
					//load the renderTexture to a texture object with a color key for transparency
					//texTransBillBoard = TextureLoader.FromStream(Device,gsStream,0,0,1,Usage.None,Format.Unknown,Pool.Managed,Filter.None, Filter.None,Color.Black.ToArgb());
					
					//Draw the texture
					s.Begin(SpriteFlags.None);
					s.Draw(renderTexture, new Rectangle(0,0, RenderSurfaceSize, RenderSurfaceSize), new Vector3(), new Vector3(0,0,0.0f), Color.White);
					s.End();
										
				}
								
				m_d3dDevice.EndScene();
				m_d3dDevice.Present(this);
			}
			catch(Microsoft.DirectX.Direct3D.DeviceLostException)
			{
				try
				{
					m_d3dDevice.Reset(m_ppPresentParams);
					this.Invalidate();
				} 
				catch (DeviceLostException)
				{   
					// If it's still lost or lost again, just do nothing
				}

			}
			catch(Exception ex)
			{
				Util.Logger.LogMsg(AnimatTools.Interfaces.Logger.enumLogLevel.Error, Util.GetErrorDetails("", ex));

				if(Editor.PhysicalStructure.RootBody == null)
					return;
			}
		}

		private void DrawAxis()
		{
			//Draw X Axis			
			m_d3dDevice.Material = m_matXAxis;
			m_d3dDevice.Transform.World = Matrix.RotationY(Geometry.DegreeToRadian(-90)) * Matrix.Translation(1.0f, 0.0f, 0.0f) * m_d3dDevice.Transform.World1;				
			m_mshAxisCylinder.DrawSubset(0);
			m_d3dDevice.Transform.World = Matrix.RotationY(Geometry.DegreeToRadian(-90)) * Matrix.Translation(2.0f, 0.0f, 0.0f) * m_d3dDevice.Transform.World1;
			m_mshAxisCone.DrawSubset(0);
			
			//Draw Y Axis
			m_d3dDevice.Material = m_matYAxis;
			m_d3dDevice.Transform.World = Matrix.RotationX(Geometry.DegreeToRadian(-90)) * Matrix.Translation(0.0f, 1.0f, 0.0f) * m_d3dDevice.Transform.World1;
			m_mshAxisCylinder.DrawSubset(0);
			m_d3dDevice.Transform.World = Matrix.RotationX(Geometry.DegreeToRadian(90)) * Matrix.Translation(0.0f, 2.0f, 0.0f) * m_d3dDevice.Transform.World1;
			m_mshAxisCone.DrawSubset(0);
			
			//Draw Z Axis
			m_d3dDevice.Material = m_matZAxis;
			m_d3dDevice.Transform.World =  Matrix.Translation(0.0f, 0.0f, 1.0f) * m_d3dDevice.Transform.World1;
			m_mshAxisCylinder.DrawSubset(0);
			m_d3dDevice.Transform.World = Matrix.RotationY(Geometry.DegreeToRadian(180)) * Matrix.Translation(0.0f, 0.0f, 2.0f) * m_d3dDevice.Transform.World1;
			m_mshAxisCone.DrawSubset(0);

			//Draw X axis label
			m_d3dDevice.Transform.World = Matrix.Translation(2.5f, 0.0f, 0.0f) * m_d3dDevice.Transform.World1;
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
			m_d3dDevice.Transform.World = Matrix.Translation(0.0f, 2.5f, 0.0f) * m_d3dDevice.Transform.World1;
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
			m_d3dDevice.Transform.World = Matrix.Translation(0.0f, 0.0f, 2.5f) * m_d3dDevice.Transform.World1;
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

		protected override void Dispose (bool disposing)
		{
			try
			{
				if (disposing)
				{
					if (m_Components != null)
					{
						m_Components.Dispose();					
					}
					if(m_d3dDevice != null)
						m_d3dDevice.Dispose();

					if(Editor.PhysicalStructure != null && Editor.PhysicalStructure.RootBody != null)
					{
						RigidBodies.RigidBody_DX tmp = (RigidBodies.RigidBody_DX)Editor.PhysicalStructure.RootBody;
						
					}
					
				}
			}
			catch(Exception ex)
			{MessageBox.Show(ex.StackTrace);}
			
			base.Dispose(disposing);
		}
		
		public override void Initialize(ref AnimatTools.Forms.AnimatApplication frmApplication, AnimatTools.Forms.MdiChild frmMdiParent, AnimatTools.Forms.AnimatForm frmParent)
		{
			base.Initialize (ref frmApplication, frmMdiParent, frmParent);
			
			if(this.Editor != null && this.Editor.PhysicalStructure != null )
			{
				float fMinX = 0.0f, fMaxX = 0.0f, fMinY = 0.0f, fMaxY = 0.0f;
				Editor.PhysicalStructure.GetStructureExent(ref fMaxX,ref fMinX,ref fMaxY, ref fMinY);
				m_fltZoom = -10 * (fMaxX - fMinY) + (fMaxY - fMinY);					
			}
		}

		private void InitializeComponent()
		{
			// 
			// BodyViewD3D
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(492, 466);
			this.Name = "BodyViewD3D";
			this.KeyDown += new System.Windows.Forms.KeyEventHandler(this.BodyViewD3D_KeyDown);
			this.Resize += new System.EventHandler(this.BodyViewD3D_Resize);
			this.MouseDown += new System.Windows.Forms.MouseEventHandler(this.BodyViewD3D_MouseDown);
			this.MouseUp += new System.Windows.Forms.MouseEventHandler(this.BodyViewD3D_MouseUp);
			this.KeyUp += new System.Windows.Forms.KeyEventHandler(this.BodyViewD3D_KeyUp);
			this.Leave += new System.EventHandler(this.BodyViewD3D_Leave);
			this.MouseMove += new System.Windows.Forms.MouseEventHandler(this.BodyViewD3D_MouseMove);
			this.MouseWheel += new System.Windows.Forms.MouseEventHandler(this.BodyViewD3D_MouseWheel);

		}

		protected bool SelectionTest(int iX, int iY)
		{
			if(Editor.PhysicalStructure.RootBody != null)
			{
				System.Collections.ArrayList colSelected; //list of all elements selected
				//lets find which ones are selected
				colSelected = Editor.PhysicalStructure.TestIntersection(iX, iY, Editor.CommandBar.CommandMode);
				
				if(colSelected.Count == 0)
					return true;	
				else
					return false;
			}

			return true;
		}
		protected virtual void SelectPart(int iX, int iY)
		{
			if(Editor.PhysicalStructure.RootBody != null)
			{
				AnimatTools.DataObjects.Physical.BodyPart bpTmpSelected = null;
				System.Collections.ArrayList colSelected; //list of all elements selected
				bool bCurrentSelectedInList = false;
					
				//lets find which ones are selected
				colSelected = Editor.PhysicalStructure.TestIntersection(iX, iY, Editor.CommandBar.CommandMode);

				//find out which body is the closest one selected
				foreach (AnimatTools.DataObjects.Physical.BodyPart bpPart in colSelected)
				{
					if(this.Editor.PropertiesBar.SelectedPart != null && bpPart == this.Editor.PropertiesBar.SelectedPart)
						bCurrentSelectedInList = true;

					//got to have at least one body selected if we're in this loop
					if(bpTmpSelected == null)
						bpTmpSelected = bpPart;

					//if rbTmp is closer to the camera than the current selected one
					//change the current one to the selected one
					if(bpPart.IntersectionDistance < bpTmpSelected.IntersectionDistance)
						bpTmpSelected = bpPart;
				}

				if(Editor.CommandBar.CommandMode != AnimatTools.Forms.BodyPlan.Command.enumCommandMode.SelectReceptiveFields || this.Editor.PropertiesBar.SelectedPart == null 
					 || this.Editor.PropertiesBar.SelectedPart is AnimatTools.DataObjects.Physical.Joint)
					this.Editor.PropertiesBar.SelectPart(bpTmpSelected, this.IsCtrlKeyPressed);
				else
				{
					//If we are in receptive field mode and their is a selcted rigid body part then lets test to see if the user is attempting to 
					//select one of the receptive fields.
					if(bCurrentSelectedInList)
					{
						RigidBodies.RigidBody_DX rbPart = (RigidBodies.RigidBody_DX ) this.Editor.PropertiesBar.SelectedPart;
						Vec3d vVertex = new Vec3d(null, 0,0,0);

						if(rbPart.TestReceptiveFieldIntersection(iX, iY, ref vVertex))
							rbPart.SelectedReceptiveField = vVertex;
						else
							this.Editor.PropertiesBar.SelectPart(bpTmpSelected, this.IsCtrlKeyPressed);
					}
					else
						this.Editor.PropertiesBar.SelectPart(bpTmpSelected, this.IsCtrlKeyPressed);
				}
			}
		}

		protected virtual void SelectPart(AnimatTools.DataObjects.Physical.BodyPart bpPart)
		{
			if(bpPart != null)
				this.Editor.PropertiesBar.SelectPart(bpPart, this.IsCtrlKeyPressed);
		}

		protected virtual AnimatTools.DataObjects.Physical.BodyPart CreateNewBodyPart(AnimatTools.Framework.DataObject doParent, ref bool bNewJoint)
		{
			AnimatTools.DataObjects.Physical.RigidBody bpPart = null;
			bNewJoint = true;

			if(Editor.CommandBar.CommandMode == AnimatTools.Forms.BodyPlan.Command.enumCommandMode.AddBodies)
			{
				bpPart = (AnimatTools.DataObjects.Physical.RigidBody) Editor.CommandBar.SelectedBodyPartType.CreateNewBodyPart(doParent);

				Editor.PhysicalStructure.NewBodyIndex++;
				bpPart.Name = "Body_" + Editor.PhysicalStructure.NewBodyIndex.ToString();
			}
			else if(Editor.CommandBar.CommandMode == AnimatTools.Forms.BodyPlan.Command.enumCommandMode.PasteBodies)
			{
				IDataObject data = Clipboard.GetDataObject();

				if(data!= null && data.GetDataPresent("AnimatLab.Body.XMLFormat"))
				{
					string strXml = (string) data.GetData("AnimatLab.Body.XMLFormat");
					if(strXml != null && strXml.Trim().Length > 0 )
					{
						AnimatTools.Interfaces.StdXml oXml = new AnimatTools.Interfaces.StdXml();
						oXml.Deserialize(strXml);
						oXml.FindElement("CopyData");
						oXml.FindChildElement("RigidBody");

						AnimatTools.DataObjects.Physical.PhysicalStructure doStruct = this.Editor.PhysicalStructure;
						AnimatTools.DataObjects.Physical.PhysicalStructure doTempStruct = new AnimatTools.DataObjects.Physical.PhysicalStructure(null);
						AnimatTools.DataObjects.Simulation doSim = Util.Simulation;
						bpPart = (AnimatTools.DataObjects.Physical.RigidBody) doSim.CreateObject(ref oXml, "RigidBody", doParent);
						bpPart.LoadData(ref doSim, ref doTempStruct, ref oXml);

						//If the part we are pasting used to be a root object then we need to add a joint to it
						if(bpPart.JointToParent == null && bpPart.UsesAJoint)
						{
							//create the joint to the parent
							Joints.Joint_DX bpJointToParent = (Joints.Joint_DX) Editor.CommandBar.SelectedJointType.CreateNewBodyPart(bpPart);
					
							bpJointToParent.Device = m_d3dDevice;
							//set the joint's parent
							doStruct.NewJointIndex++;
							bpJointToParent.Name = "Joint_" + doStruct.NewJointIndex.ToString();

							//set the location of the joint
							bpJointToParent.DxLocation = new Vector3(0, 0, 0);

							//add the joint to the new part
							bpPart.JointToParent = bpJointToParent;                			
						}

					}
				}

				if(bpPart == null)
				{
					bpPart = (AnimatTools.DataObjects.Physical.RigidBody) Editor.CommandBar.SelectedBodyPartType.CreateNewBodyPart(doParent);
					Editor.PhysicalStructure.NewBodyIndex++;
					bpPart.Name = "Body_" + Editor.PhysicalStructure.NewBodyIndex.ToString();
				}

				if(bpPart.JointToParent != null && Editor.CommandBar.SelectedJointType != null && bpPart.JointToParent.Type == Editor.CommandBar.SelectedJointType.Type)
					bNewJoint = false;
				else
					bpPart.JointToParent = null;
			}
			else
				throw new System.Exception("Command mode must be either AddBodies or PasteBodies to create a new body part.");

			return bpPart;
		}

		protected virtual void AddBody(int iX, int iY)
		{
			AnimatTools.DataObjects.Physical.BodyPart bpNewPart=null;
			RigidBodies.RigidBody_DX rbNewPart=null;
			AnimatTools.DataObjects.Physical.PhysicalStructure doStruct = Editor.PhysicalStructure;
			bool bNewJoint = true;

			//add a root body
			if(Editor.PhysicalStructure.RootBody == null)
			{
				bpNewPart = CreateNewBodyPart(Editor.PhysicalStructure, ref bNewJoint);
				rbNewPart = (RigidBodies.RigidBody_DX) bpNewPart;

				if(!rbNewPart.CanBeRootBody)
					throw new System.Exception("You can not use a part type of '" + rbNewPart.BodyPartName + "' as the root body.");

				bpNewPart.Name = "Root";
				bpNewPart.Parent = doStruct;
	
				rbNewPart.Device = m_d3dDevice;
																			
				doStruct.AddRootBody( (RigidBodies.RigidBody_DX) bpNewPart);
			}
			//add a child body
			else if(Editor.PropertiesBar.SelectedPart != null)
			{
				//get the directx version of the selected part to add a child to
				RigidBodies.RigidBody_DX  rbParent = (RigidBodies.RigidBody_DX) Editor.PropertiesBar.SelectedPart;
				
				//the new bodypart
				bpNewPart = CreateNewBodyPart(rbParent, ref bNewJoint);
				
				//DX version of the new bodypart
				rbNewPart = (RigidBodies.RigidBody_DX)bpNewPart;

				//set the new part's device
				rbNewPart.Device = m_d3dDevice;

				rbNewPart.Direction = rbParent.FaceNormal;
				
				//Vector3 v3Translation = rbNewPart.FindPointOnSurface(rbParent.SelectedFacePoint, -rbParent.FaceNormal);			
				Vector3 v = Util_DX.FindIntersectionPoint(rbParent.Mesh, rbParent.IntersectInfo);
				rbNewPart.DxLocation = v + rbNewPart.FindPointOnSurface(new Vector3(), -rbParent.FaceNormal);
				rbNewPart.DxRotation = new Vector3(); //rbParent.DxRotation;				
				rbNewPart.OrientBody();

				if(rbNewPart.UsesAJoint && bNewJoint)
				{
					//create the joint to the parent
					Joints.Joint_DX bpJointToParent = (Joints.Joint_DX) Editor.CommandBar.SelectedJointType.CreateNewBodyPart(bpNewPart);
					
					bpJointToParent.Device = m_d3dDevice;
					//set the joint's parent
					doStruct.NewJointIndex++;
					bpJointToParent.Name = "Joint_" + doStruct.NewJointIndex.ToString();

					//set the location of the joint
					bpJointToParent.DxLocation = rbNewPart.FindPointOnSurface(new Vector3(),rbParent.FaceNormal);

					//add the joint to the new part
					rbNewPart.JointToParent = bpJointToParent;                			
				}

				//add the new part to the parent
				//DWC Added
				rbNewPart.BeforeAddBody();
				rbParent.AddChildBody(rbNewPart);				
				rbNewPart.AfterAddBody();

				if(rbNewPart.BodyPlanTreeNode != null)
					rbNewPart.BodyPlanTreeNode.ExpandAll();
			}

			//If we created a new part then lets select that part.
			if(bpNewPart != null)
				SelectPart(bpNewPart);

			if(this.Editor != null && this.Editor.CommandBar != null)
				this.Editor.CommandBar.CommandMode = AnimatTools.Forms.BodyPlan.Command.enumCommandMode.SelectBodies;

			this.Invalidate();
		}

		public override void SwapBodyPart(AnimatTools.DataObjects.Physical.BodyPart doExistingPart)
		{
		}

		#endregion

		#region Events

		#region Keyboard Events

		private void BodyViewD3D_KeyDown(object sender, System.Windows.Forms.KeyEventArgs e)
		{
			try
			{
				switch(e.KeyCode)
				{
					case Keys.X: 
						m_bXDown = true;
						break;
					case Keys.Y:
						m_bYDown = true;
						break;
					case Keys.Z:
						m_bZDown = true;
						break;
					case Keys.S:
						m_bSDown = true;
						break;
				}		
			}
			catch (Exception ex)
			{
				AnimatTools.Framework.Util.DisplayError(ex);
			}
		}

		private void BodyViewD3D_KeyUp(object sender, System.Windows.Forms.KeyEventArgs e)
		{			
			try
			{
				switch(e.KeyCode)
				{	
						//reset the transformation back to original
					case Keys.R: 
						m_fltPanX = 0.0f;
						m_fltPanY = 0.0f;
						m_fltRotateX = 0.0f;
						m_fltRotateY = 0.0f;
						m_fltRotateZ = 0.0f;
						m_mtxOrientation = Matrix.Identity;
						float fMinX = 0.0f, fMaxX = 0.0f, fMinY = 0.0f, fMaxY = 0.0f;
						Editor.PhysicalStructure.GetStructureExent(ref fMaxX,ref fMinX,ref fMaxY, ref fMinY);
						this.Zoom = -10 * (fMaxX - fMinY) + (fMaxY - fMinY);
						this.Invalidate();
						break;
					case Keys.X: 
						m_bXDown = false;
						break;
					case Keys.Y:
						m_bYDown = false;
						break;
					case Keys.Z:
						m_bZDown = false;
						break;
					case Keys.S:
						m_bSDown = false;
						break;
					case Keys.Escape:
						//If the user hits the escape key while moving the mouse we need to
						//reset the body part positin state
						if(m_bMouseMove && !m_bTransformWorld && m_psMovePositionState != null && this.Editor.PropertiesBar.SelectedPart != null)
						{
							this.Editor.PropertiesBar.SelectedPart.PartPositionState = m_psMovePositionState;
							this.Invalidate();
						}
						break;
				}
				
			}
			catch (Exception ex)
			{
				AnimatTools.Framework.Util.DisplayError(ex);
			}
		}

		public override void LoadData(ref AnimatTools.Interfaces.StdXml oXml)
		{
			m_fltPanX = oXml.GetChildFloat("PanX", m_fltPanX);
			m_fltPanY = oXml.GetChildFloat("PanY", m_fltPanY);
			m_fltZoom = oXml.GetChildFloat("Zoom", m_fltZoom);
			m_fltRotateX = oXml.GetChildFloat("RotateX", m_fltRotateX);
			m_fltRotateY = oXml.GetChildFloat("RotateY", m_fltRotateY);
			
			if(oXml.FindChildElement("RotateZ", false))
				m_fltRotateZ = oXml.GetChildFloat("RotateZ", m_fltRotateZ);
			else 
				m_fltRotateZ = 0;

			m_mtxOrientation = Matrix.RotationX(m_fltRotateX) * Matrix.RotationY(m_fltRotateY) * Matrix.RotationZ(m_fltRotateZ);

			if(oXml.FindChildElement("BackColor", false))
				m_clBackColor = Util.LoadColor(ref oXml, "BackColor");
		}

		public override void SaveData(ref AnimatTools.Interfaces.StdXml oXml)
		{
			Vector3 v3Rot = Util_DX.DecomposeXYZRotationMatrix(m_mtxOrientation);
			m_fltRotateX = v3Rot.X;
			m_fltRotateY = v3Rot.Y;
			m_fltRotateZ = v3Rot.Z;

			oXml.AddChildElement("PanX", m_fltPanX);
			oXml.AddChildElement("PanY", m_fltPanY);			
			oXml.AddChildElement("Zoom", m_fltZoom);
			oXml.AddChildElement("RotateX", m_fltRotateX);
			oXml.AddChildElement("RotateY", m_fltRotateY);
			oXml.AddChildElement("RotateZ", m_fltRotateZ);
			Util.SaveColor(ref oXml, "BackColor", m_clBackColor);
		}

		#endregion

		#region MouseEvents

		private void BodyViewD3D_MouseDown(object sender, System.Windows.Forms.MouseEventArgs e)
		{
			
			try
			{	
				//check to see if we select on dead space or an object
				m_bTransformWorld = this.SelectionTest(e.X,e.Y);
				
				//If we are in select receptive field mode you should never be able to transform the object using the mouse.
				if(this.Editor.CommandBar.CommandMode == AnimatTools.Forms.BodyPlan.Command.enumCommandMode.SelectReceptiveFields)
					m_bTransformWorld = true;

				//get the current location of the mouse
				this.m_iCurrentX = e.X;
				this.m_iCurrentY = e.Y;

				//If we use the right mouse button and we are not on a selection box then lets do a drop down menu
				if(e.Button == MouseButtons.Right && (Editor.PropertiesBar.SelectedPart == null || (Editor.PropertiesBar.SelectedPart != null && !Editor.PropertiesBar.SelectedPart.IsSelectionBoxSelected(new AnimatTools.Framework.MouseEventArgs(e, m_iOldX, m_iOldY, this.IsShiftKeyPressed, this.IsCtrlKeyPressed, m_bXDown, m_bYDown, m_bZDown, m_bSDown, m_beEditor.MouseSensitivity)))))
				{
					Control ctl = (System.Windows.Forms.Control) sender;
					Point ptScreen = ctl.PointToScreen(new Point(e.X, e.Y));
					CreatePopupMenu(ptScreen);
				}
			}
			catch (Exception ex)
			{
				AnimatTools.Framework.Util.DisplayError(ex);
			}
		}
				
		private void BodyViewD3D_MouseUp(object sender, System.Windows.Forms.MouseEventArgs e)
		{
			try
			{
				//if we're just clicking.
				if(e.Button == MouseButtons.Left && !this.m_bMouseMove)
				{	
					//if thre is a selected part test for selectionboxselected
					if(Editor.PropertiesBar.SelectedPart != null)
					{	
						// if the selection box is not selected then either select a part or add a part
						if(!Editor.PropertiesBar.SelectedPart.IsSelectionBoxSelected(new AnimatTools.Framework.MouseEventArgs(e,m_iOldX, m_iOldY, this.IsShiftKeyPressed, this.IsCtrlKeyPressed, m_bXDown, m_bYDown, m_bZDown, m_bSDown, m_beEditor.MouseSensitivity)))  
						{
							if(!Editor.CommandBar.InAddBodyMode )
								SelectPart(e.X, e.Y);
							else
							{
								SelectPart(e.X, e.Y);
								AddBody(e.X, e.Y);
							}
						}
					}
					else
					{
						//if no object is selected already try selectin or adding an object
						if(!Editor.CommandBar.InAddBodyMode )
							SelectPart(e.X, e.Y);
						else
						{
							SelectPart(e.X, e.Y);
							AddBody(e.X, e.Y);
						}
					}
				}
				
				//If we are finishing a mouse move which is not transforming the world, which means it is altering the position or rotation of a body part, 
				//then we need to save these changes out to the modification history.
				if(m_bMouseMove && !m_bTransformWorld && m_psMovePositionState != null && this.Editor.PropertiesBar.SelectedPart != null)
				{
					this.Editor.PropertiesBar.SelectedPart.IsDirty = true;
					AnimatTools.DataObjects.Physical.PartPositionState psEndState = this.Editor.PropertiesBar.SelectedPart.PartPositionState;

					//Only save off the history if the part has actually moved somewhere.
					if(m_psMovePositionState.HasChanged(psEndState))
						Util.ModificationHistory.AddHistoryEvent(new AnimatTools.Framework.UndoSystem.MoveBodyPartEvent(this.Editor, this.Editor.PhysicalStructure, 
																														(AnimatTools.DataObjects.Physical.BodyPart) this.Editor.PropertiesBar.SelectedPart, m_psMovePositionState, psEndState));
				}

			}
			catch(Exception ex)
			{
				AnimatTools.Framework.Util.DisplayError(ex);
			}
			try
			{
				if(Editor.PropertiesBar.SelectedPart != null)
					Editor.PropertiesBar.RefreshProperties();
			}
			catch(Exception ex)
			{
				AnimatTools.Framework.Util.DisplayError(ex);
			}
			finally
			{
				this.m_bTransformWorld = false;
				this.m_bMouseMove = false;
			}
		}

		private void BodyViewD3D_MouseMove(object sender, System.Windows.Forms.MouseEventArgs e)
		{
			//Debug.WriteLine("Mouse Wheel: " + e.Delta.ToString());
			try
			{	
				//create a custom mouseeventargs object
				AnimatTools.Framework.MouseEventArgs AnimatMouseArgs = new AnimatTools.Framework.MouseEventArgs(e,m_iOldX, m_iOldY, this.IsShiftKeyPressed, this.IsCtrlKeyPressed, m_bXDown, m_bYDown, m_bZDown, m_bSDown, m_beEditor.MouseSensitivity);
				
				//if the mousemove flag is false and the user isholding down the left mouse button do a test
				//for if the user is doing a selection or draging
				if (!m_bMouseMove && (e.Button == MouseButtons.Left || e.Button == MouseButtons.Right))  
				{
					//if the current mouse x,y position is within a one pixel range of the x,y location obtained by mousedown then this
					//not considered a mouse move.  
					if (m_iCurrentX <0 || m_iCurrentY <0 || (Math.Abs(e.X-m_iCurrentX) < 3 && Math.Abs(e.Y-m_iCurrentY) < 3))
					{
						m_bMouseMove = false;					
						return;
					}
					else
					{
						m_bMouseMove = true;

						//If we are moving the mouse and we are not transforming the world then we need to 
						//save off the current state of the selected body part.
						if(!m_bTransformWorld && this.Editor.PropertiesBar.SelectedPart != null)
							m_psMovePositionState = this.Editor.PropertiesBar.SelectedPart.PartPositionState;
					}
				}
				
				//if the mousemove flag is set and the transformworld flag is set then transform the world.
				if(e.Button == MouseButtons.Left && m_bMouseMove && m_bTransformWorld)
				{
				
					//pan in the x and y direction
					if(this.IsShiftKeyPressed)
					{
						this.PanX -= AnimatMouseArgs.DeltaX * AnimatMouseArgs.Scale;
						this.PanY += AnimatMouseArgs.DeltaY * AnimatMouseArgs.Scale;
			
						this.Invalidate();		
					}
						//zoom
					else if(this.IsCtrlKeyPressed)
					{
						this.Zoom -= AnimatMouseArgs.DeltaY * AnimatMouseArgs.Scale;
						this.Invalidate();
					}
						//rotate
					else
					{
						m_mtxOrientation = m_mtxOrientation * Matrix.RotationY(-AnimatMouseArgs.DeltaX * AnimatMouseArgs.Scale);
						m_fltRotateX -= AnimatMouseArgs.DeltaX * AnimatMouseArgs.Scale;
						m_fltRotateY += AnimatMouseArgs.DeltaY * AnimatMouseArgs.Scale;
						//m_mtxOrientation = m_mtxOrientation * Matrix.RotationX(-AnimatMouseArgs.DeltaY * AnimatMouseArgs.Scale);


						this.Invalidate();				
					}				
				}
				
				//if there is a selected object update it with the mouse
				if(Editor.PropertiesBar.SelectedPart != null && Editor.PropertiesBar.SelectedParts.Count == 1 && !this.m_bTransformWorld)
				{
					Editor.PropertiesBar.SelectedPart.UpdateWithMouse(AnimatMouseArgs);
					this.Invalidate();						
				}
				
				//set the current x,y values of the mouse to be old values;
				m_iOldX = e.X;
				m_iOldY = e.Y;
				
			}
			catch (Exception ex)
			{
				AnimatTools.Framework.Util.DisplayError(ex);
			}
		}

		private void BodyViewD3D_MouseWheel(object sender, System.Windows.Forms.MouseEventArgs e)
		{
			try
			{	
				if(e.Delta > 0)
					this.Zoom -= 0.5f;
				else
					this.Zoom += 0.5f;

				this.Invalidate();
			}
			catch (Exception ex)
			{
				AnimatTools.Framework.Util.DisplayError(ex);
			}
		}

		#endregion

		private void Application_UnitsChanged(AnimatTools.DataObjects.Physical.Environment.enumMassUnits ePrevMass, 
																					AnimatTools.DataObjects.Physical.Environment.enumMassUnits eNewMass, float fltMassChange, 
																					AnimatTools.DataObjects.Physical.Environment.enumDistanceUnits ePrevDistance, 
																					AnimatTools.DataObjects.Physical.Environment.enumDistanceUnits eNewDistance, float fltDistanceChange)
		{
			try
			{	
				if(this.Editor != null && this.Editor.PropertiesBar != null)
				{
					this.Editor.PropertiesBar.RefreshProperties();
					this.Invalidate();
				}
			}
			catch (Exception ex)
			{
				AnimatTools.Framework.Util.DisplayError(ex);
			}
		}

		private void BodyViewD3D_Resize(object sender, System.EventArgs e)
		{
			try
			{	
				if(this.Size == new Size(0,0))
					this.Size = new Size(500,500);
				else
				{
					if(this.Width > this.Height)
						this.Height = this.Width;
					else
						this.Width = this.Height;			

					this.Invalidate();
				}
			}
			catch (Exception ex)
			{
				AnimatTools.Framework.Util.DisplayError(ex);
			}
		}

		protected override void OnCommandModeChanged(AnimatTools.Forms.BodyPlan.Command.enumCommandMode eNewMode)
		{
			try
			{	
				this.Invalidate();
			}
			catch (Exception ex)
			{
				AnimatTools.Framework.Util.DisplayError(ex);
			}
		}

		private void BodyViewD3D_Leave(object sender, System.EventArgs e)
		{
			try
			{	
				//If we leave this form then we need to reset these variables so they are not set again next time we do stuff.
				m_iCurrentX = - 1;
				m_iCurrentY = -1;
				m_bMouseMove = false;
				m_bXDown = false;
				m_bYDown = false;
				m_bZDown = false;
				m_bSDown = false;
			}
			catch (Exception ex)
			{
				AnimatTools.Framework.Util.DisplayError(ex);
			}
		}

		public override void OnContentClosing(CancelEventArgs e)
		{
			try
			{	
				base.OnContentClosing (e);

				Util.Application.UnitsChanged -= new AnimatTools.Forms.AnimatApplication.UnitsChangedEventHandler(Application_UnitsChanged);
			}
			catch (Exception ex)
			{
				AnimatTools.Framework.Util.DisplayError(ex);
			}
		}

		#endregion

	}
}
