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
	/// Summary description for Prismatic.
	/// </summary>
	public class Prismatic : VortexAnimatTools.DataObjects.Physical.Joints.Joint_DX
	{

		#region Attributes

		protected AnimatTools.Framework.ScaledNumber m_snMinMovement;
		protected AnimatTools.Framework.ScaledNumber m_snMaxMovement;
		protected AnimatTools.Framework.ScaledNumber m_snLength;
		protected float m_fltMinMovement = (float) -4;
		protected float m_fltMaxMovement = (float) 4;
		protected float m_fltLength = 5;
		protected bool m_bEnableMotor = false;
		protected AnimatTools.Framework.ScaledNumber m_snMaxForce;
		protected AnimatTools.Framework.ScaledNumber m_snMaxVelocity;
		protected bool m_bServoMotor = false;
		protected float m_fltServoGain =  100;

		protected DxPrismaticConstraintRef m_vrConstraint;

		protected Vector3 m_v3Axis = new Vector3(0,0,1);

		protected Microsoft.DirectX.Direct3D.Mesh m_mshConstraint;
		protected Mesh m_mshMin;
		protected Mesh m_mshMax;
		
		protected Mesh m_mshLCB;
		protected Mesh m_mshUCB;
		protected bool m_bMinSelected = false;
		protected bool m_bMaxSelected = false;

		#endregion

		#region Properties

		public override String ImageName {get{return "VortexAnimatTools.Graphics.Prismatic_Treeview.gif";}}
		public override String ButtonImageName {get{return "VortexAnimatTools.Graphics.Prismatic_Button.gif";}}
		public override String Type {get{return "Prismatic";}}
		public override String BodyPartName {get{return "Prismatic";}}
		public override System.Type PartType {get{return typeof(VortexAnimatTools.DataObjects.Physical.Joints.Prismatic);}}
		public override bool UsesRadians {get{return false;}}

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
					this.ManualAddPropertyHistory("Constraint", new AnimatTools.Framework.Vec3d(null, m_fltMinMovement, m_fltMaxMovement, 0), value, true);

					if(this.MinMovement != value.X)
						this.MinMovement = (float) value.X;

					if(this.MaxMovement != value.Y)
						this.MaxMovement = (float) value.Y;
				}
			}
		}

		public float MinMovement
		{
			get{return m_fltMinMovement;}
			set 
			{
				if(value > 0)
					value = -0.0001f;					
				
				m_fltMinMovement = value;				
				this.CreateBody();
			}
		}

		public float MaxMovement
		{
			get{return m_fltMaxMovement;}
			set 
			{
				if(value < 0)
					value = 0.0001f;				
				
				m_fltMaxMovement = value;				
				this.CreateBody();
			}
		}		

		public AnimatTools.Framework.ScaledNumber MinMovementScaled
		{
			get
			{
				m_snMinMovement.SetFromValue(this.MinMovement*Util.Environment.DistanceUnitValue, (int) Util.Environment.DistanceUnits);
				return m_snMinMovement;
			}
			set 
			{
				if(value != null)
					this.MinMovement = (float) value.ActualValue/Util.Environment.DistanceUnitValue;
			}
		}

		public AnimatTools.Framework.ScaledNumber MaxMovementScaled
		{
			get
			{
				m_snMaxMovement.SetFromValue(this.MaxMovement*Util.Environment.DistanceUnitValue, (int) Util.Environment.DistanceUnits);
				return m_snMaxMovement;
			}
			set 
			{
				if(value != null)
					this.MaxMovement = (float) value.ActualValue/Util.Environment.DistanceUnitValue;
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
					m_thIncomingDataType = new AnimatTools.DataObjects.DataType("Position", "Position", "m", "m", -10, 10, ScaledNumber.enumNumericScale.None, ScaledNumber.enumNumericScale.None);
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
					return "m";
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

		public AnimatTools.Framework.ScaledNumber MaxForce
		{
			get{return m_snMaxForce;}
			set 
			{
				if(value != null)
				{
					if(value.ActualValue < 0)
						throw new System.Exception("The maximum motor force must not be less than zero.");

					m_snMaxForce.CopyData(ref value);
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
						throw new System.Exception("The maximum velocity must not be less than zero.");

					m_snMaxVelocity.CopyData(ref value);
				}
			}
		}

		public override float MaxDimension
		{
			get
			{
				float fltMax = (float) this.Radius;

				return fltMax;
			}
		}

		public override float MinDimension
		{
			get
			{
				float fltMin = (float) this.Radius;

				

				return fltMin;
			}
		}


		public override bool NeedToCreateBody
		{
			get
			{
				if(this.Device != null && (m_mshSelectedBoundingBox == null || m_mshSelectedBoundingBox.Disposed || m_mshCB == null || m_mshCB.Disposed))
					return true;
				else
					return false;
			}
		}

		#endregion

		#region Methods

		public Prismatic(AnimatTools.Framework.DataObject doParent) : base(doParent)
		{
			m_thDataTypes.DataTypes.Add(new AnimatTools.DataObjects.DataType("JointPosition", "Position", "Meters", "m", -10, 10));
			m_thDataTypes.DataTypes.Add(new AnimatTools.DataObjects.DataType("JointActualVelocity", "Velocity", "m/s", "m/s", -5, 5));
			m_thDataTypes.DataTypes.Add(new AnimatTools.DataObjects.DataType("JointDesiredVelocity", "Desired Velocity", "m/s", "m/s", -5, 5));
			m_thDataTypes.DataTypes.Add(new AnimatTools.DataObjects.DataType("Enable", "Enable", "", "", 0, 1));
			m_thDataTypes.DataTypes.Add(new AnimatTools.DataObjects.DataType("BodyPositionX", "Position X Axis", "Meters", "m", -10, 10));
			m_thDataTypes.DataTypes.Add(new AnimatTools.DataObjects.DataType("BodyPositionY", "Position Y Axis", "Meters", "m", -10, 10));
			m_thDataTypes.DataTypes.Add(new AnimatTools.DataObjects.DataType("BodyPositionZ", "Position Z Axis", "Meters", "m", -10, 10));
			m_thDataTypes.ID = "JointPosition";

			Color  = System.Drawing.Color.YellowGreen;

			m_snMinMovement = new AnimatTools.Framework.ScaledNumber(this, "MinMovementScaled", "meters", "m");
			m_snMaxMovement = new AnimatTools.Framework.ScaledNumber(this, "MaxMovementScaled", "meters", "m");
			m_snLength = new AnimatTools.Framework.ScaledNumber(this, "LengthScaled", "meters", "m");
			m_snMaxForce = new AnimatTools.Framework.ScaledNumber(this, "MaxForce", 100, AnimatTools.Framework.ScaledNumber.enumNumericScale.None, "Newtons", "N");
			m_snMaxVelocity = new AnimatTools.Framework.ScaledNumber(this, "MaxVelocity", 100, AnimatTools.Framework.ScaledNumber.enumNumericScale.None, "m/s", "m/s");

			AddCompatibleStimulus(new AnimatTools.DataObjects.ExternalStimuli.MotorVelocity(null));
			AddCompatibleStimulus(new AnimatTools.DataObjects.ExternalStimuli.Enabler(null));
			AddCompatibleStimulus(new VortexAnimatTools.DataObjects.ExternalStimuli.PositionClamp(null));
			m_vrConstraint = new DxPrismaticConstraintRef(this);			
		}

		/// <summary>
		/// Creates all the bodies associated with this body part
		/// </summary>
		protected override void CreateBody()
		{		
			//dispose of any previous bodies
			this.CleanUp();

			//used to maked the selection box a little bigger than the joint
			float fltScale = (this.MaxDimension * 3.0f);
			if(fltScale > 0.3f) fltScale = 0.3f;

			//create the min and max movement cylinders
			//m_mshMin = Util_DX.CreateCylinder(this.Device, m_fRadius, m_fRadius, this.MinMovement, 16);
			//m_mshMax = Util_DX.CreateCylinder(this.Device, m_fRadius, m_fRadius, this.MaxMovement, 16);

			m_mshMin = Mesh.Cylinder(this.Device, m_fRadius, m_fRadius, Math.Abs(this.MinMovement), 16, 16);
			m_mshMax = Mesh.Cylinder(this.Device, m_fRadius, m_fRadius, this.MaxMovement, 16, 16);

			//create the box used as "stoppers"
			m_mshConstraint = Util_DX.CreateBox(this.Device,3 * m_fRadius, 3 * m_fRadius, 3 * m_fRadius);
			
			//Create the selected bounding box
			m_mshSelectedBoundingBox = Mesh.Box(Device, (float)((m_fRadius) + fltScale),(float)((m_fRadius) + fltScale), (float)((-1 * MinMovement) + MaxMovement + fltScale)); 

			RefreshSelectionBoxSizes();

			base.CreateBody();
		}

		#region Drawing Methods

		public override void DrawParts()
		{
			Matrix mWorld = this.Device.Transform.World;

			Material m = new Material();
			m.Diffuse = Color.Blue;
			Device.Material = m;

			this.Device.Transform.World = Matrix.Translation(0,0,-MaxMovement/2) * mWorld;			
			m_mshMax.DrawSubset(0);

			this.Device.Transform.World = Matrix.Translation(0, 0, -MaxMovement + m_fRadius) * mWorld;
			m_mshConstraint.DrawSubset(0);

			Device.Material = this.m_d3dMaterial;
			this.Device.Transform.World = Matrix.Translation(0,0,-MinMovement/2) * mWorld;
			m_mshMin.DrawSubset(0);

			this.Device.Transform.World = Matrix.Translation(0, 0,-MinMovement - m_fRadius) * mWorld;
			m_mshConstraint.DrawSubset(0);

			this.Device.Transform.World = mWorld;			
			
			if(m_bSelected)
			{	
				DrawAxis();
				DrawBoundingBox();			
				DrawSelectionBoxes();
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
				this.Device.Transform.World = Matrix.Translation(0, 0, -(MinMovement + MaxMovement)/2 ) * this.CombinedTransformationMatrix * this.Device.Transform.World1;  //moves the body to where it is in space
			
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


		protected virtual void DrawSelectionBoxes()
		{
			//Max Movement
			Vector3 tmp = new Vector3();
			tmp.TransformCoordinate(Matrix.Translation(0, 0, -MaxMovement + m_fRadius) * CombinedTransformationMatrix * Device.Transform.World1);
		
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

			//Min Movement
			tmp = new Vector3();
			tmp.TransformCoordinate(Matrix.Translation(0, 0, -MinMovement - m_fRadius) * CombinedTransformationMatrix * Device.Transform.World1);
		
			m = this.Device.Transform.View;
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

		#endregion

		protected override bool CanDraw()
		{
			if(this.Device != null && m_mshMin != null
				&& m_mshMax != null
				&& this.m_mshConstraint != null
				&& this.m_mshSelectedBoundingBox != null
				&& this.m_mshCB != null)
				return true;
			else
				return false;
		}

		protected override void CleanUp()
		{
			SafeRelease(ref m_mshConstraint);
			SafeRelease(ref m_mshMin);
			SafeRelease(ref m_mshMax);			

			base.CleanUp();
		}	

		public override void TestIntersection(int x, int y, AnimatTools.Forms.BodyPlan.Command.enumCommandMode cmCommand, ref System.Collections.ArrayList colSelected)
		{
			if(CanDraw() && cmCommand == AnimatTools.Forms.BodyPlan.Command.enumCommandMode.SelectJoints)
			{
				bool bBBSelected = false;
				
				Matrix mBBWorld =  Matrix.Translation(0, 0, -(MinMovement + MaxMovement)/2 ) * this.CombinedTransformationMatrix * this.Device.Transform.World1;  //moves the body to where it is in space
				Matrix mWorld = Matrix.Translation(0,0,-this.MinMovement/2) * this.CombinedTransformationMatrix * this.Device.Transform.World1;
		
				IntersectInformation iiTmp = new IntersectInformation();
				
				bBBSelected = Util_DX.TestIntersectWithMouse(m_mshSelectedBoundingBox,Device.Viewport,Device.Transform.Projection, Device.Transform.View, mBBWorld,x,y,out iiTmp) && this.m_bSelected;
				
				if(Util_DX.TestIntersectWithMouse(m_mshMin,Device.Viewport, Device.Transform.Projection, Device.Transform.View, mWorld, x, y, out iiTmp)
					|| bBBSelected)
					colSelected.Add(this);					
					
				mWorld = Matrix.Translation(0,0,-this.MaxMovement/2) * this.CombinedTransformationMatrix * this.Device.Transform.World1;
				if(Util_DX.TestIntersectWithMouse(m_mshMax,Device.Viewport, Device.Transform.Projection, Device.Transform.View, mWorld, x, y, out iiTmp)
					|| bBBSelected)
					colSelected.Add(this);

				mWorld = Matrix.Translation(0, 0, -MinMovement - m_fRadius) * this.CombinedTransformationMatrix * this.Device.Transform.World1;
				if(Util_DX.TestIntersectWithMouse(m_mshConstraint,Device.Viewport, Device.Transform.Projection, Device.Transform.View, mWorld, x, y, out iiTmp)
					|| bBBSelected)
					colSelected.Add(this);
				
				mWorld = Matrix.Translation(0, 0, -MaxMovement + m_fRadius) * this.CombinedTransformationMatrix * this.Device.Transform.World1;
				if(Util_DX.TestIntersectWithMouse(m_mshConstraint,Device.Viewport, Device.Transform.Projection, Device.Transform.View, mWorld, x, y, out iiTmp)
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
				m_bMinSelected = false;
				m_bMaxSelected = false;
				return;
			}

			//set the location for the max constraint box
			Vector3 tmp = new Vector3();
			tmp.TransformCoordinate(Matrix.Translation(0, 0, -MaxMovement + m_fRadius) * CombinedTransformationMatrix * Device.Transform.World1);
		
			Matrix m = this.Device.Transform.View;
			m.Invert();
			m.M41 = tmp.X;
			m.M42 = tmp.Y;
			m.M43 = tmp.Z;
			
			IntersectInformation iiInfo = new IntersectInformation();
			if(m_mshCB != null && Util_DX.TestIntersectWithMouse(m_mshCB,Device.Viewport, Device.Transform.Projection, Device.Transform.View, m,AnimatMouseArgs.X, AnimatMouseArgs.Y, out iiInfo) 
				&& !m_bCBSelected && !m_bMinSelected				
				&& (AnimatMouseArgs.Button == System.Windows.Forms.MouseButtons.Left || AnimatMouseArgs.Button == System.Windows.Forms.MouseButtons.Right))							
				m_bMaxSelected = true;

			//set the location for the min constraint box
			tmp = new Vector3();
			tmp.TransformCoordinate(Matrix.Translation(0, 0, -MinMovement - m_fRadius) * CombinedTransformationMatrix * Device.Transform.World1);
		
			m = this.Device.Transform.View;
			m.Invert();
			m.M41 = tmp.X;
			m.M42 = tmp.Y;
			m.M43 = tmp.Z;
			
			if(m_mshCB != null && Util_DX.TestIntersectWithMouse(m_mshCB,Device.Viewport, Device.Transform.Projection, Device.Transform.View, m,AnimatMouseArgs.X, AnimatMouseArgs.Y, out iiInfo) 
				&& !m_bCBSelected && !m_bMaxSelected
				&& (AnimatMouseArgs.Button == System.Windows.Forms.MouseButtons.Left || AnimatMouseArgs.Button == System.Windows.Forms.MouseButtons.Right))							
				m_bMinSelected = true;
			

			//set the location of the center box
			tmp = new Vector3();
			tmp.TransformCoordinate(CombinedTransformationMatrix * Device.Transform.World1);
		
			Matrix mWorld = this.Device.Transform.View;
			mWorld.Invert();
			mWorld.M41 = tmp.X;
			mWorld.M42 = tmp.Y;
			mWorld.M43 = tmp.Z;

			if(m_mshCB != null 
				&& !m_bMaxSelected && !m_bMinSelected
				&& Util_DX.TestIntersectWithMouse(m_mshCB, Device.Viewport, Device.Transform.Projection, Device.Transform.View, mWorld,AnimatMouseArgs.X, AnimatMouseArgs.Y, out iiInfo) && (AnimatMouseArgs.Button == System.Windows.Forms.MouseButtons.Left || AnimatMouseArgs.Button == System.Windows.Forms.MouseButtons.Right))
				m_bCBSelected = true;

			if(m_bCBSelected)
				ProcessCenterSquare(AnimatMouseArgs);	
			else if(m_bMaxSelected && (AnimatMouseArgs.Button == MouseButtons.Left))
				ProcessMaxSquare(AnimatMouseArgs);
			else if(m_bMinSelected && (AnimatMouseArgs.Button == MouseButtons.Left))
				ProcessMinSquare(AnimatMouseArgs);

		}

		public void ProcessMaxSquare(AnimatTools.Framework.MouseEventArgs AnimatMouseArgs)
		{
			this.MaxMovement =  this.MaxMovement + ((AnimatMouseArgs.DeltaX * AnimatMouseArgs.Scale));			
		}

		public void ProcessMinSquare(AnimatTools.Framework.MouseEventArgs AnimatMouseArgs)
		{
			this.MinMovement =  this.MinMovement + ((AnimatMouseArgs.DeltaX * AnimatMouseArgs.Scale));			
		}

		protected override void BuildProperties()
		{
			base.BuildProperties();

			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Enable Motor", m_bEnableMotor.GetType(), "EnableMotor",
																	"Part Properties", "Sets whether the motor is enabled for this joint.", m_bEnableMotor));

			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Servo Motor", m_bServoMotor.GetType(), "ServoMotor",
				"Part Properties", "Sets whether this is a servo or DC motor. If it is a servo then the Input specifies position, otherwise it specifies velocity.", m_bServoMotor));

			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Servo Gain", m_fltServoGain.GetType(), "ServoGain",
				"Part Properties", "Sets the magnitude of the feedback gain for the servo motor.", m_fltServoGain));

			Crownwood.Magic.Controls.PropertyBag pbNumberBag  = this.MaxForce.Properties;
			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Max Motor Force", pbNumberBag.GetType(), "MaxForce", 
																	"Part Properties", "Sets the maximum torque that this motor can apply to obtain a desired velocity of movement.", pbNumberBag,
																	"", typeof(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)));

			pbNumberBag  = this.MaxVelocity.Properties;
			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Max Velocity", pbNumberBag.GetType(), "MaxVelocity", 
																		"Constraint", "Sets the maximum positive or negative velocity that the motor can move this part.", pbNumberBag,
																		"", typeof(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)));

			pbNumberBag  = this.MinMovementScaled.Properties;
			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Minimum", pbNumberBag.GetType(), "MinMovementScaled", 
																	"Constraint", "Sets the minimum distance the body can slide on this prismatic joint.", pbNumberBag,
																	"", typeof(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)));

			pbNumberBag  = this.MaxMovementScaled.Properties;
			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Maximum", pbNumberBag.GetType(), "MaxMovementScaled", 
																	"Constraint", "Sets the maximum distance the body can slide on this prismatic joint.", pbNumberBag,
																	"", typeof(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)));			
			
		}

		public override AnimatTools.DataObjects.Physical.BodyPart CreateNewBodyPart(AnimatTools.Framework.DataObject doParent)
		{return new Prismatic(doParent);}

		protected override void CloneInternal(AnimatTools.Framework.DataObject doOriginal, bool bCutData, AnimatTools.Framework.DataObject doRoot)
		{
			base.CloneInternal (doOriginal, bCutData, doRoot);

			Prismatic doOrigBody = (Prismatic) doOriginal;

			m_fltMinMovement = doOrigBody.m_fltMinMovement;
			m_fltMaxMovement = doOrigBody.m_fltMaxMovement;
			m_fltLength = doOrigBody.m_fltLength;
			m_snMinMovement = (AnimatTools.Framework.ScaledNumber) doOrigBody.m_snMinMovement.Clone(this, bCutData, doRoot);
			m_snMaxMovement = (AnimatTools.Framework.ScaledNumber) doOrigBody.m_snMaxMovement.Clone(this, bCutData, doRoot);
			m_snLength = (AnimatTools.Framework.ScaledNumber) doOrigBody.m_snLength.Clone(this, bCutData, doRoot);
			m_snMaxForce = (AnimatTools.Framework.ScaledNumber) m_snMaxForce.Clone(this, bCutData, doRoot);
			m_snMaxVelocity = (AnimatTools.Framework.ScaledNumber) m_snMaxVelocity.Clone(this, bCutData, doRoot);
			m_bEnableMotor = doOrigBody.m_bEnableMotor;
			m_bServoMotor = doOrigBody.m_bServoMotor;
			m_fltServoGain = doOrigBody.ServoGain;
		}

		public override AnimatTools.Framework.DataObject Clone(AnimatTools.Framework.DataObject doParent, bool bCutData, AnimatTools.Framework.DataObject doRoot)
		{
			Prismatic bnPart = new Prismatic(doParent);
			bnPart.CloneInternal(this, bCutData, doRoot);
			if(doRoot != null && doRoot == this) bnPart.AfterClone(this, bCutData, doRoot, bnPart);
			return bnPart;
		}

		#region Loading and Saving Data

		public override void LoadData(ref AnimatTools.DataObjects.Simulation dsSim, ref AnimatTools.DataObjects.Physical.PhysicalStructure doStructure, ref AnimatTools.Interfaces.StdXml oXml)
		{
			base.LoadData (ref dsSim, ref doStructure, ref oXml);

			oXml.IntoElem();
		    
			if(oXml.FindChildElement("Constraint", false))
			{
				oXml.IntoChildElement("Constraint");
				m_fltMinMovement = oXml.GetAttribFloat("Low");
				m_fltMaxMovement = oXml.GetAttribFloat("High");
				oXml.OutOfElem(); //out of Constraint			
			}

			m_fltLength = oXml.GetChildFloat("Length", m_fltLength);
			m_bEnableMotor = oXml.GetChildBool("EnableMotor", false);
			m_bServoMotor = oXml.GetChildBool("ServoMotor", m_bServoMotor);
			m_fltServoGain = oXml.GetChildFloat("ServoGain", m_fltServoGain);

			if(oXml.FindChildElement("MaxForce", false))
				m_snMaxForce.LoadData(ref oXml, "MaxForce");

			if(oXml.FindChildElement("MaxVelocity", false))
				m_snMaxVelocity.LoadData(ref oXml, "MaxVelocity");

			oXml.OutOfElem(); //out of body			

		}

		public override void SaveData(ref AnimatTools.DataObjects.Simulation dsSim, ref AnimatTools.DataObjects.Physical.PhysicalStructure doStructure, ref AnimatTools.Interfaces.StdXml oXml)
		{
			base.SaveData (ref dsSim, ref doStructure, ref oXml);

			oXml.IntoElem();

			if(!oXml.FindChildElement("Constraint", false))
				oXml.AddChildElement("Constraint");
			oXml.IntoElem();
			oXml.SetAttrib("Low",this.m_fltMinMovement);
			oXml.SetAttrib("High", this.m_fltMaxMovement);
			oXml.OutOfElem();

			oXml.AddChildElement("Length", m_fltLength);
			oXml.AddChildElement("EnableMotor",this.m_bEnableMotor);
			oXml.AddChildElement("ServoMotor",this.m_bServoMotor);
			oXml.AddChildElement("ServoGain",this.m_fltServoGain);
			m_snMaxForce.SaveData(ref oXml, "MaxForce");
			m_snMaxVelocity.SaveData(ref oXml, "MaxVelocity");

			RigidBodies.RigidBody_DX rbParent = (RigidBodies.RigidBody_DX)this.Parent;
			Vector3 v3Rot = Util_DX.DecomposeXYZRotationMatrix(this.CombinedTransformationMatrix);;
			float x = Geometry.DegreeToRadian(180);
			m_v3Axis.TransformCoordinate(Matrix.RotationX(Geometry.DegreeToRadian(180)) * Matrix.RotationY(Geometry.DegreeToRadian(180)) * Matrix.RotationZ(Geometry.DegreeToRadian(180)) * Matrix.RotationX(v3Rot.X)* Matrix.RotationY(v3Rot.Y) * Matrix.RotationZ(v3Rot.Z));
			Vec3d vRotationAxis = new Vec3d(null,m_v3Axis.X, m_v3Axis.Y, m_v3Axis.Z);			
			Util.SaveVector(ref oXml, "RotationAxis", vRotationAxis);
			m_v3Axis = new Vector3(0,0,1);			

			oXml.OutOfElem();
					
		}

		#endregion

		#endregion
	}
}
