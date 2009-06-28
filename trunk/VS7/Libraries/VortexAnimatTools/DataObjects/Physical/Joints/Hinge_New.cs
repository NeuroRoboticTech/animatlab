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
	/// Summary description for Hinge.
	/// </summary>
	public class Hinge : VortexAnimatTools.DataObjects.Physical.Joints.Joint_DX
	{
		#region Attributes

		protected float m_fltMinAngle = -1.57f;
		protected float m_fltMaxAngle =  1.57f;
		protected AnimatTools.Framework.ScaledNumber m_snMinAngle;
		protected AnimatTools.Framework.ScaledNumber m_snMaxAngle;
		protected bool m_bEnableMotor = false;
		protected AnimatTools.Framework.ScaledNumber m_snMaxTorque;
		protected AnimatTools.Framework.ScaledNumber m_snMaxVelocity;
		protected bool m_bServoMotor = false;
		protected float m_fltServoGain =  100;
	
		protected Microsoft.DirectX.Direct3D.Mesh m_mshPole;
		protected Microsoft.DirectX.Direct3D.Mesh m_mshBox;
		protected Microsoft.DirectX.Direct3D.Mesh m_mshConstraint;
				
		protected DxHingeConstraintRef m_vrConstraint;

		protected Mesh m_mshLCB;
		protected Mesh m_mshUCB;

		protected Vector3 m_v3Axis = new Vector3(0,0,1);
		protected bool m_bLCBSelected = false;
		protected bool m_bUCBSelected = false;

		protected Texture texConstraint = null;

		#endregion

		#region Properties

		public override String ImageName {get{return "VortexAnimatTools.Graphics.Hinge_Treeview.gif";}}
		public override String ButtonImageName {get{return "VortexAnimatTools.Graphics.Hinge_Button.gif";}}
		public override String Type {get{return "Hinge";}}
		public override String BodyPartName {get{return "Hinge";}}

		public AnimatTools.Framework.Vec3d Constraint 
		{
			get
			{
				return m_vrConstraint;
			}
			set
			{
				if(value != null)
				{
					if(this.MinRotation != value.X)
						this.MinRotation = (float) value.X;

					if(this.MaxRotation != value.Y)
						this.MaxRotation = (float) value.Y;
				}
			}
		}

		public float MinRotation
		{
			get{return Util.RadiansToDegrees(m_fltMinAngle);}
			set 
			{
//				if(value >180 || value<-180)
//					throw new System.Exception("The constraint rotation value must be between -180 and 180 degrees.");

				float fltVal = Util.DegreesToRadians(value);

				if(fltVal >= m_fltMaxAngle)
					throw new System.Exception("The minimum rotation angle constraint can not be greater than the maximum angle constraint.");

				m_fltMinAngle = fltVal;
				m_snMinAngle.SetFromValue(value, AnimatTools.Framework.ScaledNumber.enumNumericScale.None);
				InvalidateBodyView();
			}
		}

		public AnimatTools.Framework.ScaledNumber MinRotationScaled
		{
			get{return m_snMinAngle;}
			set 
			{
				if(value != null)
					this.MinRotation = (float) value.ActualValue;
			}
		}

		public float MaxRotation 
		{
			get{return Util.RadiansToDegrees(m_fltMaxAngle);}
			set 
			{
//				if(value >180 || value<-180)
//					throw new System.Exception("The constraint rotation value must be between -180 and 180 degrees.");

				float fltVal = Util.DegreesToRadians(value);

				if(fltVal <= m_fltMinAngle)
					throw new System.Exception("The maximum rotation angle constraint can not be less than the minimum angle constraint.");

				m_fltMaxAngle = fltVal;
				m_snMaxAngle.SetFromValue(value, AnimatTools.Framework.ScaledNumber.enumNumericScale.None);
				InvalidateBodyView();
			}
		}

		public AnimatTools.Framework.ScaledNumber MaxRotationScaled
		{
			get{return m_snMaxAngle;}
			set 
			{
				if(value != null)
					this.MaxRotation = (float) value.ActualValue;
			}
		}

		public bool EnableMotor 
		{
			get{return m_bEnableMotor;}
			set {m_bEnableMotor = value;}
		}

		public bool ServoMotor 
		{
			get{return m_bServoMotor;}
			set 
			{
				m_bServoMotor = value;

				if(m_bServoMotor)
					m_thIncomingDataType = new AnimatTools.DataObjects.DataType("Position", "Position", "rad", "rad", -3.142, 3.142, ScaledNumber.enumNumericScale.None, ScaledNumber.enumNumericScale.None);
				else
					m_thIncomingDataType = new AnimatTools.DataObjects.DataType("DesiredVelocity", "Desired Velocity", "m/s", "m/s", -5, 5, ScaledNumber.enumNumericScale.None, ScaledNumber.enumNumericScale.None);
			}
		}

		public float ServoGain 
		{
			get{return m_fltServoGain;}
			set {m_fltServoGain = value;}
		}

		public override string ScaleUnits
		{
			get
			{
				if(m_bServoMotor)
					return "rad";
				else
					return base.ScaleUnits;
			}
		}

		public override string InputStimulus
		{
			get
			{
				if(m_bServoMotor)
					return "Position";
				else
					return base.InputStimulus;
			}
		}

		public AnimatTools.Framework.ScaledNumber MaxTorque
		{
			get{return m_snMaxTorque;}
			set 
			{
				if(value != null)
				{
					if(value.ActualValue < 0)
						throw new System.Exception("The maximum motor torque must not be less than zero.");

					m_snMaxTorque.CopyData(ref value);
				}
			}
		}

		public AnimatTools.Framework.ScaledNumber MaxVelocity
		{
			get{return m_snMaxVelocity;}
			set 
			{
				if(value != null)
				{
					if(value.ActualValue < 0)
						throw new System.Exception("The maximum velocity value must not be less than zero.");

					m_snMaxVelocity.CopyData(ref value);
				}
			}
		}

		#endregion

		#region Methods

		public Hinge(AnimatTools.Framework.DataObject doParent) : base(doParent)
		{
			m_thDataTypes.DataTypes.Add(new AnimatTools.DataObjects.DataType("JointRotation", "Rotation", "Radians", "rad", -3.14, 3.14));
			m_thDataTypes.DataTypes.Add(new AnimatTools.DataObjects.DataType("JointActualVelocity", "Velocity", "m/s", "m/s", -5, 5));
			m_thDataTypes.DataTypes.Add(new AnimatTools.DataObjects.DataType("JointDesiredVelocity", "Desired Velocity", "m/s", "m/s", -5, 5));
			m_thDataTypes.DataTypes.Add(new AnimatTools.DataObjects.DataType("Enable", "Enable", "", "", 0, 1));
			m_thDataTypes.ID = "JointRotation";

			Color  = System.Drawing.Color.Magenta;

			m_snMinAngle = new AnimatTools.Framework.ScaledNumber(this, "MinRotationScaled", Util.RadiansToDegrees(m_fltMinAngle), AnimatTools.Framework.ScaledNumber.enumNumericScale.None, "Degrees", "Deg");
			m_snMaxAngle = new AnimatTools.Framework.ScaledNumber(this, "MaxRotationScaled", Util.RadiansToDegrees(m_fltMaxAngle), AnimatTools.Framework.ScaledNumber.enumNumericScale.None, "Degrees", "Deg");
			m_snMaxTorque = new AnimatTools.Framework.ScaledNumber(this, "MaxTorque", 100, AnimatTools.Framework.ScaledNumber.enumNumericScale.None, "Newton-Meters", "Nm");
			m_snMaxVelocity = new AnimatTools.Framework.ScaledNumber(this, "MaxVelocity", 100, AnimatTools.Framework.ScaledNumber.enumNumericScale.None, "rad/s", "rad/s");

			AddCompatibleStimulus(new AnimatTools.DataObjects.ExternalStimuli.MotorVelocity(null));
			AddCompatibleStimulus(new AnimatTools.DataObjects.ExternalStimuli.Enabler(null));
			AddCompatibleStimulus(new VortexAnimatTools.DataObjects.ExternalStimuli.PositionClamp(null));
		}

		protected override void CreateBody()
		{
			this.CleanUp();

			this.m_d3dMesh = Mesh.Sphere(this.Device,m_fRadius,32,32);
			this.m_mshPole = Mesh.Cylinder(this.Device,(0.5f * m_fRadius),(0.5f * m_fRadius), 4 * m_fRadius, 32, 32);
			this.m_mshBox = Mesh.Box(this.Device,2 * m_fRadius,(0.5f * m_fRadius), 4 * m_fRadius);
			this.m_mshConstraint = Mesh.Box(this.Device,2 * m_fRadius,(0.125f * m_fRadius), 4 * m_fRadius);

			this.m_mshSelectedBoundingBox = Mesh.Box(this.Device,5 * m_fRadius, 5 * m_fRadius, 5 * m_fRadius);
			RefreshSelectionBoxSizes();

			base.CreateBody();
			m_vrConstraint = new DxHingeConstraintRef(this);			
		}

		protected override void CleanUp()
		{
			SafeRelease(ref m_mshPole);
			SafeRelease(ref m_mshBox);
			SafeRelease(ref m_mshConstraint);
			SafeRelease(ref m_mshLCB);
			SafeRelease(ref m_mshUCB);
			
			base.CleanUp ();
		}


		public override void DrawParts()
		{
			DrawAxisOfRotation();
			DrawArc();
			if(m_bSelected)
			{
				base.DrawAxis();
				this.DrawBoundingBox();
			}
		}

		private void DrawAxisOfRotation()
		{
			//this.Device.Transform.World = Matrix.RotationY(Geometry.DegreeToRadian(90)) * mWorld;
			//this.Device.Transform.World = Matrix.RotationY(Geometry.DegreeToRadian(90)) * Matrix.RotationX(m_v3RotationAxis.Y) * Matrix.RotationY(m_v3RotationAxis.Z) * Matrix.RotationZ(m_v3RotationAxis.X) * this.Device.Transform.World;
			this.Device.Transform.World = Matrix.RotationY(Geometry.DegreeToRadian(180)) * this.CombinedTransformationMatrix * Device.Transform.World1;
			Matrix mWorld = this.Device.Transform.World;

			m_mshPole.DrawSubset(0);

			this.Device.Transform.World = Matrix.RotationY(Geometry.DegreeToRadian(180)) * Matrix.Translation( m_fRadius, 0,0) * Matrix.RotationAxis(m_v3Axis ,m_fltMaxAngle) * mWorld;
			
			Material mat = new Material();

			mat.Diffuse = Color.Maroon;
			mat.Ambient = Color.Maroon;
			Device.Material = mat;
			m_mshConstraint.DrawSubset(0);
			

			this.Device.Transform.World = Matrix.RotationY(Geometry.DegreeToRadian(180)) * Matrix.Translation( m_fRadius, 0,0) * Matrix.RotationAxis(m_v3Axis,m_fltMinAngle) * mWorld;
				
			mat.Diffuse = Color.Lavender;
			mat.Ambient = Color.Lavender;
			Device.Material = mat;
			m_mshConstraint.DrawSubset(0);
			
			Device.Material = this.m_d3dMaterial;
		}

		private void DrawArc()
		{
			//Create the vertices that will represent each axis
			CustomVertex.PositionColored[] vertArc = new CustomVertex.PositionColored[36];
			
			Vector3 v3Current = new Vector3();
			v3Current.TransformCoordinate(Matrix.Translation( m_fRadius, 0,0) * Matrix.RotationAxis(m_v3Axis ,m_fltMinAngle));
			float fltRotationAmount = (float)((Math.Abs(this.m_fltMinAngle) + this.m_fltMaxAngle)/vertArc.Length);
			int intInterpolateAmount = 100/ vertArc.Length;

			for(int i=0; i<vertArc.Length; i++)
			{
				if(i%2 == 0)
					vertArc[i].Position = new Vector3(v3Current.X, v3Current.Y, v3Current.Z + m_fRadius/2);
				else
					vertArc[i].Position = new Vector3(v3Current.X, v3Current.Y, v3Current.Z - m_fRadius/2);
				v3Current.TransformCoordinate(Matrix.RotationAxis(m_v3Axis, fltRotationAmount));
				vertArc[i].Color = Color.FromArgb(255, (int)(255 * (float)((i * intInterpolateAmount)/100.0)), 0).ToArgb();
				
			}
				vertArc[vertArc.Length -1].Position = new Vector3(v3Current.X, v3Current.Y, v3Current.Z - m_fRadius/2);
				vertArc[vertArc.Length -1].Color = Color.Purple.ToArgb();
				
			Device.Transform.World = Matrix.RotationY(Geometry.DegreeToRadian(180)) * this.CombinedTransformationMatrix * Device.Transform.World1;
			
			//tell the device what kind of vertex format we're using
			this.Device.VertexFormat = CustomVertex.PositionColored.Format;

			//draw the lines
			this.Device.RenderState.Lighting = false;
			this.Device.DrawUserPrimitives(PrimitiveType.TriangleStrip,vertArc.Length - 2,vertArc);
			this.Device.RenderState.Lighting = true;
		}
		

		public override void RefreshSelectionBoxSizes()
		{
			float fltSelBoxSize = m_fltSelBoxSize;

			base.RefreshSelectionBoxSizes();

			float fltDistance = this.Location.Distance(this.ParentStructure.BodyEditor.BodyView.CameraPosition);
			float fltScale = (fltDistance/5)*0.05f;

			if(fltSelBoxSize != fltScale || m_mshLCB == null || m_mshLCB.Disposed || m_mshUCB == null || m_mshUCB.Disposed)
			{
				this.m_mshLCB = Util_DX.CreateBillboard(Device, m_fltSelBoxSize, m_fltSelBoxSize);
				this.m_mshUCB = Util_DX.CreateBillboard(Device, m_fltSelBoxSize, m_fltSelBoxSize);
			}
		}

		protected override void BuildProperties()
		{
			base.BuildProperties();

			Crownwood.Magic.Controls.PropertyBag pbNumberBag  = this.MinRotationScaled.Properties;
			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Minimum", pbNumberBag.GetType(), "MinRotationScaled", 
																	"Constraints", "Sets the minimum angle rotation that is allowed for this joint in degrees.", pbNumberBag,
																	"", typeof(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)));

			pbNumberBag  = this.MaxRotationScaled.Properties;
			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Maximum", pbNumberBag.GetType(), "MaxRotationScaled", 
																	"Constraints", "Sets the maximum angle rotation that is allowed for this joint in degrees.", pbNumberBag,
																	"", typeof(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)));

			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Enable Motor", m_bEnableMotor.GetType(), "EnableMotor",
																	"Part Properties", "Sets whether the motor is enabled for this joint.", m_bEnableMotor));

			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Servo Motor", m_bServoMotor.GetType(), "ServoMotor",
																	"Part Properties", "Sets whether this is a servo or DC motor. If it is a servo then the Input specifies position, otherwise it specifies velocity.", m_bServoMotor));

			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Servo Gain", m_fltServoGain.GetType(), "ServoGain",
																	"Part Properties", "Sets the magnitude of the feedback gain for the servo motor.", m_fltServoGain));

			pbNumberBag  = this.MaxTorque.Properties;
			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Max Motor Torque", pbNumberBag.GetType(), "MaxTorque", 
																	"Part Properties", "Sets the maximum torque that this motor can apply to obtain a desired velocity of movement.", pbNumberBag,
																	"", typeof(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)));

			pbNumberBag  = this.MaxVelocity.Properties;
			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Max Velocity", pbNumberBag.GetType(), "MaxVelocity", 
																	"Part Properties", "Sets the maximum positive or negative velocity that the motor can move this part.", pbNumberBag,
																	"", typeof(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)));
		}

		public override void DrawBoundingBox()
		{
			base.DrawBoundingBox();		

			if(texConstraint == null)
			{
				try
				{
					System.Reflection.Assembly myAssembly = System.Reflection.Assembly.Load("VortexAnimatTools");
					System.IO.Stream stream = myAssembly.GetManifestResourceStream("VortexAnimatTools.Graphics.ikpoint.png");
					
					//load the texture for the selection boxes
					//texCB = TextureLoader.FromFile(Device,"VortexAnimatTools.ikpoint0.png");	

					//load texture from stream
					texConstraint = TextureLoader.FromStream(Device,stream);
					
				}
				catch(Exception ex)
				{
					Util.DisplayError(ex);
				}
			}
	
			this.Device.RenderState.ZBufferEnable = false;
			this.Device.Material = Util_DX.WhiteMaterial();

			Matrix mWorld = this.CombinedTransformationMatrix * Device.Transform.World1;
			
			//Matrix.Translation( m_fRadius, 0,0) *  * mWorld;
			Vector3 tmp =  new Vector3();
			tmp.TransformCoordinate(Matrix.Translation( -m_fRadius * 2, 0,0) * Matrix.RotationAxis(m_v3Axis ,-m_fltMinAngle) * mWorld);
			
			Matrix m = this.Device.Transform.View;
			m.Invert();

			m.M41 = tmp.X;
			m.M42 = tmp.Y;
			m.M43 = tmp.Z;

			this.Device.Transform.World = m;
			Device.SetTexture(0,texConstraint);
			this.m_mshLCB.DrawSubset(0);
			
			
			tmp =  new Vector3();
			tmp.TransformCoordinate(Matrix.Translation( -m_fRadius * 2, 0,0) * Matrix.RotationAxis(m_v3Axis ,-m_fltMaxAngle) * mWorld);
			m.M41 = tmp.X;
			m.M42 = tmp.Y;
			m.M43 = tmp.Z;

			this.Device.Transform.World = m;
			
			this.m_mshUCB.DrawSubset(0);
			Device.SetTexture(0,null);
			Device.RenderState.ZBufferEnable = true;	
		}

		public override void TestIntersection(int x, int y, AnimatTools.Forms.BodyPlan.Command.enumCommandMode cmCommand, ref System.Collections.ArrayList colSelected)
		{
			if(m_d3dMesh != null && cmCommand == AnimatTools.Forms.BodyPlan.Command.enumCommandMode.SelectJoints)
			{
				
				Matrix m_World = this.CombinedTransformationMatrix * Device.Transform.World1;
				Matrix mWorld = m_World;
		
				IntersectInformation iiTmp = new IntersectInformation();
				if(Util_DX.TestIntersectWithMouse(m_mshPole,Device.Viewport, Device.Transform.Projection, Device.Transform.View, mWorld, x, y, out iiTmp)|| (Util_DX.TestIntersectWithMouse(m_mshSelectedBoundingBox,Device.Viewport,Device.Transform.Projection, Device.Transform.View, mWorld,x,y,out iiTmp) && this.m_bSelected))				
					colSelected.Add(this);
				//else
				//	m_bSelected = false;	

				m_World = Matrix.Translation( m_fRadius, 0,0) * Matrix.RotationAxis(new Vector3(0,0,1),m_fltMaxAngle) * mWorld;
				
				if(Util_DX.TestIntersectWithMouse(m_mshConstraint,Device.Viewport, Device.Transform.Projection, Device.Transform.View, m_World, x, y, out iiTmp) || (Util_DX.TestIntersectWithMouse(m_mshSelectedBoundingBox,Device.Viewport,Device.Transform.Projection, Device.Transform.View, mWorld,x,y,out iiTmp) && this.m_bSelected))
					colSelected.Add(this);
				//else
				//	m_bSelected = false;	
				
				
				m_World = Matrix.Translation( m_fRadius, 0,0) * Matrix.RotationAxis(new Vector3(0,0,1),m_fltMinAngle) * mWorld;
				if(Util_DX.TestIntersectWithMouse(m_mshConstraint,Device.Viewport, Device.Transform.Projection, Device.Transform.View, m_World, x, y, out iiTmp) || (Util_DX.TestIntersectWithMouse(m_mshSelectedBoundingBox,Device.Viewport,Device.Transform.Projection, Device.Transform.View, mWorld,x,y,out iiTmp) && this.m_bSelected))
					colSelected.Add(this);
				//else
				//	m_bSelected = false;	
					
			}			
			base.TestIntersection (x, y, cmCommand, ref colSelected);
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
				m_bLCBSelected = false;
				m_bUCBSelected = false;
				return;
			}
			IntersectInformation iiInfo = new IntersectInformation();
			if(m_mshCB != null && 
				!this.m_bLCBSelected && !this.m_bUCBSelected &&
				Util_DX.TestIntersectWithMouse(m_mshCB,Device.Viewport, Device.Transform.Projection, Device.Transform.View, mWorld,AnimatMouseArgs.X, AnimatMouseArgs.Y, out iiInfo) && (AnimatMouseArgs.Button == System.Windows.Forms.MouseButtons.Left || AnimatMouseArgs.Button == System.Windows.Forms.MouseButtons.Right))
				m_bCBSelected = true;


			//set the location of the upper box
			tmp =  new Vector3();
			tmp.TransformCoordinate(Matrix.Translation( -m_fRadius * 2, 0,0) * Matrix.RotationAxis(m_v3Axis ,-m_fltMinAngle) * this.CombinedTransformationMatrix * Device.Transform.World1);
		
			mWorld = this.Device.Transform.View;
			mWorld.Invert();
			mWorld.M41 = tmp.X;
			mWorld.M42 = tmp.Y;
			mWorld.M43 = tmp.Z;
		
			//Did we click on the lower constraint box?
			if(m_mshLCB != null && 
				!this.m_bCBSelected && !this.m_bUCBSelected  &&
				Util_DX.TestIntersectWithMouse(m_mshLCB,Device.Viewport, Device.Transform.Projection, Device.Transform.View, mWorld,AnimatMouseArgs.X, AnimatMouseArgs.Y, out iiInfo) && (AnimatMouseArgs.Button == System.Windows.Forms.MouseButtons.Left))
				m_bLCBSelected = true;
			
			//set the location of the upper box
			tmp =  new Vector3();
			tmp.TransformCoordinate(Matrix.Translation( -m_fRadius * 2, 0,0) * Matrix.RotationAxis(m_v3Axis ,-m_fltMaxAngle) * this.CombinedTransformationMatrix * Device.Transform.World1);
		
			mWorld = this.Device.Transform.View;
			mWorld.Invert();
			mWorld.M41 = tmp.X;
			mWorld.M42 = tmp.Y;
			mWorld.M43 = tmp.Z;
		
			if(m_mshUCB != null 
				&& !this.m_bLCBSelected && !this.m_bCBSelected
				&& Util_DX.TestIntersectWithMouse(m_mshUCB,Device.Viewport, Device.Transform.Projection, Device.Transform.View, mWorld,AnimatMouseArgs.X, AnimatMouseArgs.Y, out iiInfo) && (AnimatMouseArgs.Button == System.Windows.Forms.MouseButtons.Left))
				m_bUCBSelected = true;

			if(m_bCBSelected)
				ProcessCenterSquare(AnimatMouseArgs);				
			else if(m_bLCBSelected && (AnimatMouseArgs.Button == MouseButtons.Left))
				ProcessLowerConstraint(AnimatMouseArgs);
			else if(m_bUCBSelected && (AnimatMouseArgs.Button == MouseButtons.Left))
				ProcessUpperConstraint(AnimatMouseArgs);
		}

		public void ProcessLowerConstraint(AnimatTools.Framework.MouseEventArgs AnimatMouseArgs)
		{
			this.m_fltMinAngle += ((AnimatMouseArgs.DeltaX * AnimatMouseArgs.Scale));
			if(m_fltMinAngle < -(float)Math.PI)
				m_fltMinAngle = -(float)Math.PI;
			if(m_fltMinAngle >0)
				m_fltMinAngle = 0;
			//this.InvalidateBodyView();
		}
		public void ProcessUpperConstraint(AnimatTools.Framework.MouseEventArgs AnimatMouseArgs)
		{
			this.m_fltMaxAngle += ((AnimatMouseArgs.DeltaX * AnimatMouseArgs.Scale));
			if(m_fltMaxAngle > (float)Math.PI)
				m_fltMaxAngle = (float)Math.PI;
			if(m_fltMaxAngle <0)
				m_fltMaxAngle = 0;
			//this.InvalidateBodyView();
		}

		public override AnimatTools.DataObjects.Physical.BodyPart CreateNewBodyPart(AnimatTools.Framework.DataObject doParent)
		{return new Hinge(doParent);}

		protected override void CloneInternal(AnimatTools.Framework.DataObject doOriginal)
		{
			base.CloneInternal (doOriginal);

			Hinge doOrigBody = (Hinge) doOriginal;

			m_fltMinAngle = doOrigBody.m_fltMinAngle;
			m_fltMaxAngle = doOrigBody.m_fltMaxAngle;
			m_snMinAngle = (AnimatTools.Framework.ScaledNumber) m_snMinAngle.Clone(this);
			m_snMaxAngle = (AnimatTools.Framework.ScaledNumber) m_snMaxAngle.Clone(this);
			m_bEnableMotor = doOrigBody.m_bEnableMotor;
			m_bServoMotor = doOrigBody.m_bServoMotor;
			m_fltServoGain = doOrigBody.ServoGain;
			m_snMaxTorque = (AnimatTools.Framework.ScaledNumber) m_snMaxTorque.Clone(this);
			m_snMaxVelocity = (AnimatTools.Framework.ScaledNumber) m_snMaxVelocity.Clone(this);
			m_v3Axis = new Vector3(doOrigBody.m_v3Axis.X, doOrigBody.m_v3Axis.Y, doOrigBody.m_v3Axis.Z);
		}

		public override AnimatTools.Framework.DataObject Clone(AnimatTools.Framework.DataObject doParent)
		{
			Hinge bnPart = new Hinge(doParent);
			bnPart.CloneInternal(this);
			return bnPart;
		}

		#region Loading and Saving Data

		public override void LoadData(ref AnimatTools.DataObjects.Simulation dsSim, ref AnimatTools.DataObjects.Physical.PhysicalStructure doStructure, ref AnimatTools.Interfaces.StdXml oXml)
		{
			base.LoadData (ref dsSim, ref doStructure, ref oXml);

			oXml.IntoElem();

			oXml.IntoChildElement("Constraint");
			m_fltMinAngle = oXml.GetAttribFloat("Low");
			m_fltMaxAngle = oXml.GetAttribFloat("High");
			oXml.OutOfElem(); //out of Constraint			

			m_snMinAngle.SetFromValue(Util.RadiansToDegrees(m_fltMinAngle), AnimatTools.Framework.ScaledNumber.enumNumericScale.None);
			m_snMaxAngle.SetFromValue(Util.RadiansToDegrees(m_fltMaxAngle), AnimatTools.Framework.ScaledNumber.enumNumericScale.None);

			m_bEnableMotor = oXml.GetChildBool("EnableMotor");
			m_bServoMotor = oXml.GetChildBool("ServoMotor", m_bServoMotor);
			m_fltServoGain = oXml.GetChildFloat("ServoGain", m_fltServoGain);

			//based on whether this is a servo motor or not the incoming data type will change.
			if(m_bServoMotor)
				m_thIncomingDataType = new AnimatTools.DataObjects.DataType("Position", "Position", "rad", "rad", -3.142, 3.142, ScaledNumber.enumNumericScale.None, ScaledNumber.enumNumericScale.None);
			else
				m_thIncomingDataType = new AnimatTools.DataObjects.DataType("DesiredVelocity", "Desired Velocity", "m/s", "m/s", -5, 5, ScaledNumber.enumNumericScale.None, ScaledNumber.enumNumericScale.None);

			if(oXml.FindChildElement("MaxTorque", false))
				m_snMaxTorque.LoadData(ref oXml, "MaxTorque");

			if(oXml.FindChildElement("MaxVelocity", false))
				m_snMaxTorque.LoadData(ref oXml, "MaxVelocity");

//			if(oXml.FindChildElement("RotationAxis", false))
//			{
//				Vec3d vTmp = Util.LoadVec3d(ref oXml, "RotationAxis", this);	
//				m_v3Axis = new Vector3((float)vTmp.X, (float)vTmp.Y, (float)vTmp.Z);
//			}

			
			m_v3Axis = new Vector3(0,0,1);
			oXml.OutOfElem(); //out of body	
		}

		public override void SaveData(ref AnimatTools.DataObjects.Simulation dsSim, ref AnimatTools.DataObjects.Physical.PhysicalStructure doStructure, ref AnimatTools.Interfaces.StdXml oXml)
		{
			base.SaveData (ref dsSim, ref doStructure, ref oXml);

			oXml.IntoElem();

			RigidBodies.RigidBody_DX rbParent = (RigidBodies.RigidBody_DX)this.Parent;
			Vector3 v3Rot = Util_DX.DecomposeXYZRotationMatrix(this.CombinedTransformationMatrix);;
			float x = Geometry.DegreeToRadian(180);

			if(!oXml.FindChildElement("Constraint", false))
				oXml.AddChildElement("Constraint");
			oXml.IntoElem();
			oXml.SetAttrib("Low",this.m_fltMinAngle);
			oXml.SetAttrib("High", this.m_fltMaxAngle);
			oXml.OutOfElem();

			oXml.AddChildElement("EnableMotor",this.m_bEnableMotor);
			oXml.AddChildElement("ServoMotor",this.m_bServoMotor);
			oXml.AddChildElement("ServoGain",this.m_fltServoGain);

			m_snMaxTorque.SaveData(ref oXml, "MaxTorque");
			m_snMaxVelocity.SaveData(ref oXml, "MaxVelocity");

//			if(v3Rot.Y >= 180)
//				m_v3Axis.TransformCoordinate(/*Matrix.RotationX(Geometry.DegreeToRadian(180))*  Matrix.RotationY(Geometry.DegreeToRadian(180)) * Matrix.RotationZ(Geometry.DegreeToRadian(180)) **/ Matrix.RotationX(v3Rot.X)* Matrix.RotationY(v3Rot.Y) * Matrix.RotationZ(v3Rot.Z));
//			else
				//m_v3Axis.TransformCoordinate(Matrix.RotationX(Geometry.DegreeToRadian(180)) * Matrix.RotationY(Geometry.DegreeToRadian(180)) * Matrix.RotationZ(Geometry.DegreeToRadian(180)) * Matrix.RotationX(v3Rot.X)* Matrix.RotationY(v3Rot.Y) * Matrix.RotationZ(v3Rot.Z));
			Matrix m = this.CombinedTransformationMatrix;
			m.M41 = 0;
			m.M42 = 0;
			m.M43 = 0;
			//Quaternion q = Quaternion.RotationYawPitchRoll(v3Rot.Y, v3Rot.X, v3Rot.Z);
			//m_v3Axis.TransformCoordinate(Matrix.RotationQuaternion(q));
			m_v3Axis.TransformCoordinate(m);


			//Vec3d vRotationAxis = new Vec3d(null,1, 0, 0);			
			Vec3d vRotationAxis = new Vec3d(null,m_v3Axis.X, m_v3Axis.Y, m_v3Axis.Z);			
			Util.SaveVector(ref oXml, "RotationAxis", vRotationAxis);
			
			oXml.OutOfElem();
			m_v3Axis = new Vector3(0,0,1);			
		}

		#endregion

		#endregion
	}
}
