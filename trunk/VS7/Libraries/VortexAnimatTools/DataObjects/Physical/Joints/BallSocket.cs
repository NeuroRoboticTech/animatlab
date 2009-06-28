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

namespace VortexAnimatTools.DataObjects.Physical.Joints
{
	/// <summary>
	/// Summary description for BallSocket.
	/// </summary>
	public class BallSocket : VortexAnimatTools.DataObjects.Physical.Joints.Joint_DX
	{
		public BallSocket(AnimatTools.Framework.DataObject doParent) : base(doParent)
		{
			Color  = System.Drawing.Color.Cyan;
			m_v3Rotation = new Vector3(0, 0, 1);
		}

		#region Attributes
		protected Mesh m_mshPole;
		protected bool m_bRBSelected = false;
		protected float m_fHalfAngle = Geometry.DegreeToRadian(45);

		protected Vector3 m_v3Axis = new Vector3(0,0,1);
		
		#endregion

		#region Properties

		public override String ImageName {get{return "VortexAnimatTools.Graphics.BallSocket_Treeview.gif";}}
		public override String ButtonImageName {get{return "VortexAnimatTools.Graphics.BallAndSocket_Button.gif";}}
		public override String Type {get{return "BallSocket";}}
		public override String BodyPartName {get{return "Ball-Socket";}}
		public override System.Type PartType {get{return typeof(VortexAnimatTools.DataObjects.Physical.Joints.BallSocket);}}
		public float HalfAngle
		{
			get
			{
				return Geometry.RadianToDegree(m_fHalfAngle); 
			}
			set
			{
				m_fHalfAngle = Geometry.DegreeToRadian(value);
				this.CreateBody();
			}
		}

		#endregion

		protected override void BuildProperties()
		{
			base.BuildProperties();

			//add property for the half angle
			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Cone Half Angle", typeof(float), "HalfAngle", 
						"Constraints", "Determines the range of movement for the ball and socket. Valid values are between 0 and 360.", HalfAngle));
		}

		protected override void CreateBody()
		{
			this.CleanUp();

			//used to make the selection box a little bigger than the joint
			float fltScale = (this.MaxDimension * 0.15f);
			if(fltScale > 0.3f) fltScale = 0.3f;

			//this.m_d3dMesh = Util_DX.CreateOpenCylinder(this.Device, 0.015f, this.Radius,0.06f, 16);
			this.m_d3dMesh = Util_DX.CreateConeLimit(this.Device, m_fHalfAngle, 0.06f, 16);
			this.m_mshPole = Mesh.Cylinder(Device, 0.0075f, 0.0075f, 0.09f, 16, 16);
			
			//Create the selected bounding box
			m_mshSelectedBoundingBox = Mesh.Box(Device, (float)(2 * Radius + fltScale),(float)(2 * Radius + fltScale), (float)(0.09f + fltScale)); 

			RefreshSelectionBoxSizes();

			base.CreateBody();
		}

		protected override void CleanUp()
		{
			SafeRelease(ref m_mshPole);
			
			base.CleanUp();
		}	

		public override void DrawParts()
		{
			if(CanDraw())
			{			
				Device.Transform.World = this.CombinedTransformationMatrix * Device.Transform.World1;
				m_d3dMesh.DrawSubset(0);
				Device.Transform.World = Matrix.Translation(0,0,0.045f) * this.CombinedTransformationMatrix * Device.Transform.World1;
				m_mshPole.DrawSubset(0);

				if(this.Selected)
				{
					DrawAxis();
					this.DrawBoundingBox();
				}
			}
		}

		public override void DrawBoundingBox()
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
				this.Device.Transform.World = Matrix.Translation(0,0,0.045f) * this.CombinedTransformationMatrix * this.Device.Transform.World1;  //moves the body to where it is in space
			
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
				

				tmp = Vector3.Empty;
				tmp.TransformCoordinate(Matrix.Translation(0,-Radius,0.06f) * this.CombinedTransformationMatrix * Device.Transform.World1);
		
				m = this.Device.Transform.View;
				m.Invert();
				m.M41 = tmp.X;
				m.M42 = tmp.Y;
				m.M43 = tmp.Z;
				this.Device.Transform.World = m;

				m_mshCB.DrawSubset(0);
				Device.SetTexture(0,null);
				this.Device.RenderState.ZBufferEnable = true;
			}			
		}

		public override void TestIntersection(int x, int y, AnimatTools.Forms.BodyPlan.Command.enumCommandMode cmCommand, ref System.Collections.ArrayList colSelected)
		{
			if(CanDraw() && cmCommand == AnimatTools.Forms.BodyPlan.Command.enumCommandMode.SelectJoints)
			{
				bool bBBSelected = false;
				
				Matrix mBBWorld =  Matrix.Translation(0,0,0.045f) * this.CombinedTransformationMatrix * this.Device.Transform.World1;  //moves the body to where it is in space
				Matrix mWorld = Matrix.Translation(0,0,0.045f) * this.CombinedTransformationMatrix * Device.Transform.World1;
		
				IntersectInformation iiTmp = new IntersectInformation();
				
				bBBSelected = Util_DX.TestIntersectWithMouse(m_mshSelectedBoundingBox,Device.Viewport,Device.Transform.Projection, Device.Transform.View, mBBWorld,x,y,out iiTmp) && this.m_bSelected;
				
				if(Util_DX.TestIntersectWithMouse(m_mshPole,Device.Viewport, Device.Transform.Projection, Device.Transform.View, mWorld, x, y, out iiTmp)
					|| bBBSelected)
					colSelected.Add(this);								
					
			}			
			base.TestIntersection (x, y, cmCommand, ref colSelected);
		}

		public override void UpdateWithMouse(AnimatTools.Framework.MouseEventArgs AnimatMouseArgs)
		{
			//If no button is pressed..nothing is selected
			if(AnimatMouseArgs.Button == System.Windows.Forms.MouseButtons.None)
			{
				m_bCBSelected = false;	
				m_bRBSelected = false;
				return;
			}

			IntersectInformation iiInfo = new IntersectInformation();
			Vector3 tmp = new Vector3();
			tmp.TransformCoordinate(CombinedTransformationMatrix * Device.Transform.World1);
		
			Matrix mWorld = this.Device.Transform.View;
			mWorld.Invert();
			mWorld.M41 = tmp.X;
			mWorld.M42 = tmp.Y;
			mWorld.M43 = tmp.Z;

			if(m_mshCB != null 
				&& !m_bRBSelected
				&& Util_DX.TestIntersectWithMouse(m_mshCB, Device.Viewport, Device.Transform.Projection, Device.Transform.View, mWorld,AnimatMouseArgs.X, AnimatMouseArgs.Y, out iiInfo) && (AnimatMouseArgs.Button == System.Windows.Forms.MouseButtons.Left || AnimatMouseArgs.Button == System.Windows.Forms.MouseButtons.Right))
				m_bCBSelected = true;

			tmp = new Vector3();
			tmp.TransformCoordinate(Matrix.Translation(0,-Radius,0.06f) * this.CombinedTransformationMatrix * Device.Transform.World1);
		
			mWorld = this.Device.Transform.View;
			mWorld.Invert();
			mWorld.M41 = tmp.X;
			mWorld.M42 = tmp.Y;
			mWorld.M43 = tmp.Z;

			if(m_mshCB != null 
				&& !m_bCBSelected
				&& Util_DX.TestIntersectWithMouse(m_mshCB, Device.Viewport, Device.Transform.Projection, Device.Transform.View, mWorld,AnimatMouseArgs.X, AnimatMouseArgs.Y, out iiInfo) && (AnimatMouseArgs.Button == System.Windows.Forms.MouseButtons.Left || AnimatMouseArgs.Button == System.Windows.Forms.MouseButtons.Right))
				m_bRBSelected = true;

			if(m_bCBSelected)
				ProcessCenterSquare(AnimatMouseArgs);	
			else if(m_bRBSelected)
				ProcessRadius(AnimatMouseArgs);
		}

		public void ProcessRadius(AnimatTools.Framework.MouseEventArgs AnimatMouseArgs)
		{
			this.Radius =  this.Radius + ((AnimatMouseArgs.DeltaX * AnimatMouseArgs.Scale));			
		}

		public override AnimatTools.DataObjects.Physical.BodyPart CreateNewBodyPart(AnimatTools.Framework.DataObject doParent)
		{return new BallSocket(doParent);}

		public override AnimatTools.Framework.DataObject Clone(AnimatTools.Framework.DataObject doParent, bool bCutData, AnimatTools.Framework.DataObject doRoot)
		{
			BallSocket bnPart = new BallSocket(doParent);
			bnPart.CloneInternal(this, bCutData, doRoot);
			if(doRoot != null && doRoot == this) bnPart.AfterClone(this, bCutData, doRoot, bnPart);
			return bnPart;
		}

		public override void LoadData(ref AnimatTools.DataObjects.Simulation dsSim, ref AnimatTools.DataObjects.Physical.PhysicalStructure doStructure, ref AnimatTools.Interfaces.StdXml oXml)
		{
			base.LoadData (ref dsSim, ref doStructure, ref oXml);

			oXml.IntoElem();
				m_fHalfAngle = oXml.GetChildFloat("ConstraintHalfAngle");	
				Vec3d v = Util.LoadVec3d(ref oXml,"ConstraintAxis",null);
//				this.Damping.ActualValue = oXml.GetChildFloat("Damping");
//				this.Stiffness.ActualValue = oXml.GetChildFloat("Stiffness");

				this.m_v3Rotation.X = (float)v.X;
				this.m_v3Rotation.Y = (float)v.Y;
				this.m_v3Rotation.Z = (float)v.Z;
			oXml.OutOfElem();
		}

		public override void SaveData(ref AnimatTools.DataObjects.Simulation dsSim, ref AnimatTools.DataObjects.Physical.PhysicalStructure doStructure, ref AnimatTools.Interfaces.StdXml oXml)
		{
			base.SaveData (ref dsSim, ref doStructure, ref oXml);
			Vector3 v3Rot = Util_DX.DecomposeXYZRotationMatrix(this.CombinedTransformationMatrix);

			Vector3 rot = m_v3Rotation;
			rot.Normalize();
			AnimatTools.Framework.Vec3d v = new Vec3d(null, rot.X, rot.Y, rot.Z);
			
			oXml.IntoElem();				
				//oXml.AddChildElement("Stiffness", this.Stiffness.ActualValue);
				//oXml.AddChildElement("Damping", this.Damping.ActualValue);
				oXml.AddChildElement("ConstraintHalfAngle",m_fHalfAngle);				

				m_v3Axis.TransformCoordinate(Matrix.RotationX(Geometry.DegreeToRadian(180)) * Matrix.RotationY(Geometry.DegreeToRadian(180)) * Matrix.RotationZ(Geometry.DegreeToRadian(180)) * Matrix.RotationX(v3Rot.X)* Matrix.RotationY(v3Rot.Y) * Matrix.RotationZ(v3Rot.Z));
				
				Vec3d vRotationAxis = new Vec3d(null,m_v3Axis.X, m_v3Axis.Y, m_v3Axis.Z);			
				Util.SaveVector(ref oXml, "ConstraintAxis", vRotationAxis);
			oXml.OutOfElem();
			
			m_v3Axis = new Vector3(0,0,1);			
		}

	}
}
