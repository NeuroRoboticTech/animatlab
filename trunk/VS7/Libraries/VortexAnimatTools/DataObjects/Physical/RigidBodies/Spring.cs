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
	/// Summary description for Spring.
	/// </summary>
	public class Spring : VortexAnimatTools.DataObjects.Physical.RigidBodies.RigidBody_DX
	{
		#region Attributes

		protected AnimatTools.TypeHelpers.LinkedBodyPart m_thPrimaryAttachment;
		protected AnimatTools.TypeHelpers.LinkedBodyPart m_thSecondaryAttachment;
		protected System.Type m_tpBodyPartType;

		protected String m_strPrimaryAttachID = "";  //Only used during loading
		protected String m_strSecondaryAttachID = "";  //Only used during loading

		protected AnimatTools.Framework.ScaledNumber m_snNaturalLength;
		protected AnimatTools.Framework.ScaledNumber m_snStiffness;
		protected AnimatTools.Framework.ScaledNumber m_snDamping;
		protected AnimatTools.Framework.ScaledNumber m_snSpringLength;

		protected float m_fRadius = 0.05f;

		#endregion

		#region Properties

		public override String ImageName {get{return "VortexAnimatTools.Graphics.Spring_Treeview.gif";}}
		public override String ButtonImageName {get{return "VortexAnimatTools.Graphics.Spring_Button.gif";}}
		public override String Type {get{return "Spring";}}
		public override String BodyPartName {get{return "Spring";}}
		public override System.Type PartType {get{return typeof(VortexAnimatTools.DataObjects.Physical.RigidBodies.Spring);}}

		public override bool AllowUserAdd {get{return true;}}
		public override bool CanBeRootBody {get{return false;}}
		public override bool UsesAJoint {get{return false;}}
		public override bool AllowCollisions {get{return false;}}

		public virtual AnimatTools.TypeHelpers.LinkedBodyPart PrimaryAttachment
		{
			get{return m_thPrimaryAttachment;}
			set 
			{
				if(value != null && value.BodyPart != null &&
					m_thSecondaryAttachment != null && m_thSecondaryAttachment.BodyPart != null 
					&& m_thSecondaryAttachment.BodyPart.ID == value.BodyPart.ID)
					throw new System.Exception("You can not set the primary muscle attachment to be the same as the secondary attachment.");

				m_thPrimaryAttachment = value;
			}
		}

		public virtual AnimatTools.TypeHelpers.LinkedBodyPart SecondaryAttachment
		{
			get{return m_thSecondaryAttachment;}
			set 
			{
				if(value != null && value.BodyPart != null &&
					m_thPrimaryAttachment != null && m_thPrimaryAttachment.BodyPart != null 
					&& m_thPrimaryAttachment.BodyPart.ID == value.BodyPart.ID)
					throw new System.Exception("You can not set the secondary muscle attachment to be the same as the primary attachment.");

				m_thSecondaryAttachment = value;
			}
		}

		public virtual AnimatTools.Framework.ScaledNumber NaturalLength
		{
			get{return m_snNaturalLength;}
			set 
			{
				if(value != null)
				{
					if(value.ActualValue <= 0)
						throw new System.Exception("The natrual length must be greater than zero.");

					m_snNaturalLength.CopyData(ref value);
				}
			}
		}
 
		public virtual AnimatTools.Framework.ScaledNumber Stiffness
		{
			get{return m_snStiffness;}
			set 
			{
				if(value != null)
				{
					if(value.ActualValue <= 0)
						throw new System.Exception("The stiffness must be greater than zero.");

					m_snStiffness.CopyData(ref value);
				}
			}
		}
 
		public virtual AnimatTools.Framework.ScaledNumber Damping
		{
			get{return m_snDamping;}
			set 
			{
				if(value != null)
				{
					if(value.ActualValue < 0)
						throw new System.Exception("The damping must be greater than or equal to zero.");

					m_snDamping.CopyData(ref value);
				}
			}
		}

		public virtual AnimatTools.Framework.ScaledNumber SpringLength
		{get{return m_snSpringLength;}}

		public override bool Enabled
		{
			get
			{
				return base.Enabled;
			}
			set
			{
				base.Enabled = value;

				if(this.BodyPlanTreeNode != null)
				{
					if(this.Enabled)
						this.BodyPlanTreeNode.BackColor = Color.White;
					else
						this.BodyPlanTreeNode.BackColor = Color.Gray;
				}
			}
		}

		public override float MaxDimension {get{return 0;}}
		public override float MinDimension {get{return 0;}}

		#endregion

		#region Methods

		public Spring(AnimatTools.Framework.DataObject doParent) : base(doParent)
		{
			Color  = System.Drawing.Color.NavajoWhite;

			m_snNaturalLength = new AnimatTools.Framework.ScaledNumber(this, "NaturalLength", "meters", "m");
			m_snStiffness = new AnimatTools.Framework.ScaledNumber(this, "Stiffness", 50, AnimatTools.Framework.ScaledNumber.enumNumericScale.Kilo, "N/m", "N/m");
			m_snDamping = new AnimatTools.Framework.ScaledNumber(this, "Damping", 1, AnimatTools.Framework.ScaledNumber.enumNumericScale.Kilo, "g/s", "g/s");

			if(Util.Environment != null)
				m_snNaturalLength.SetFromValue(1, (int) Util.Environment.DisplayDistanceUnits);

			AnimatTools.DataObjects.Physical.PhysicalStructure doStruct = null;
			if(doParent != null && doParent is AnimatTools.DataObjects.Physical.BodyPart)
			{
				AnimatTools.DataObjects.Physical.BodyPart bpPart = (AnimatTools.DataObjects.Physical.BodyPart) doParent;
				doStruct = bpPart.ParentStructure;
			}

			m_tpBodyPartType = typeof(VortexAnimatTools.DataObjects.Physical.RigidBodies.MuscleAttachment);
			m_thPrimaryAttachment = new AnimatTools.TypeHelpers.LinkedBodyPartList(doStruct, null, m_tpBodyPartType);
			m_thSecondaryAttachment = new AnimatTools.TypeHelpers.LinkedBodyPartList(doStruct, null, m_tpBodyPartType);
			m_snSpringLength = new AnimatTools.Framework.ScaledNumber(this, "SpringLength", 0, AnimatTools.Framework.ScaledNumber.enumNumericScale.None, "Meters", "m");

			m_thDataTypes.DataTypes.Clear();
			m_thDataTypes.DataTypes.Add(new AnimatTools.DataObjects.DataType("SpringLength", "Spring Length", "Meters", "m", -10, 10));
			m_thDataTypes.DataTypes.Add(new AnimatTools.DataObjects.DataType("Displacement", "Displacement", "Meters", "m", -10, 10));
			m_thDataTypes.DataTypes.Add(new AnimatTools.DataObjects.DataType("Tension", "Tension", "Newtons", "N", -1000, 1000));
			m_thDataTypes.DataTypes.Add(new AnimatTools.DataObjects.DataType("Energy", "Energy", "Joules", "J", -10, 10));
			m_thDataTypes.DataTypes.Add(new AnimatTools.DataObjects.DataType("Enable", "Enable", "", "", 0, 1));
			m_thDataTypes.ID = "SpringLength";

			m_thIncomingDataType = new AnimatTools.DataObjects.DataType("Enabled", "Enabled", "", "", 0, 1, ScaledNumber.enumNumericScale.None, ScaledNumber.enumNumericScale.None);
		}

		public override void InitAfterAppStart()
		{
			base.InitAfterAppStart();
			AddCompatibleStimulusType("EnablerInput");
		}

		public override void AddChildBody(AnimatTools.DataObjects.Physical.RigidBody rbChildBody)
		{
			//If you try and add a body to a muscle attachment then you really need to add it to the attachment parent.
			if(this.Parent != null)
			{
				AnimatTools.DataObjects.Physical.RigidBody rbParent = (AnimatTools.DataObjects.Physical.RigidBody) this.Parent;
				
				rbChildBody.Parent = rbParent;
				rbParent.AddChildBody(rbChildBody);
			}
		}

		public override void Draw(AnimatTools.Forms.BodyPlan.Command.enumCommandMode cmCommand)
		{
			if(m_thPrimaryAttachment != null && m_thPrimaryAttachment.BodyPart != null &&
				m_thSecondaryAttachment != null && m_thSecondaryAttachment.BodyPart != null)
			{
				RigidBody_DX rbPrimary = (RigidBody_DX) m_thPrimaryAttachment.BodyPart;
				RigidBody_DX rbSecondary = (RigidBody_DX) m_thSecondaryAttachment.BodyPart;

				//Create the vertices that will represent each axis
				CustomVertex.PositionColored[] vertMuscle = new CustomVertex.PositionColored[2];

				Vector3 v3PrimLoc = rbPrimary.AbsoluteLocation;
				Vector3 v3SecLoc = rbSecondary.AbsoluteLocation;

				//calculate the muscle length;		
				float fltXd = v3PrimLoc.X - v3SecLoc.X;
				float fltYd = v3PrimLoc.Y - v3SecLoc.Y;
				float fltZd = v3PrimLoc.Z - v3SecLoc.Z;
				float fltLength = (float)Math.Sqrt( (fltXd*fltXd) + (fltYd*fltYd) + (fltZd*fltZd) ); 

				//define the muslce line
				vertMuscle[0].Position = v3PrimLoc;
				vertMuscle[1].Position = v3SecLoc;

				vertMuscle[0].Color = this.Color.ToArgb();
				vertMuscle[1].Color = this.Color.ToArgb();
				
				//tell the device what kind of vertex format we're using
				this.Device.VertexFormat = CustomVertex.PositionColored.Format;

				//draw the lines
				this.Device.RenderState.Lighting = false;
				this.Device.Transform.World = Device.Transform.World1;
				this.Device.DrawUserPrimitives(PrimitiveType.LineList, 1,vertMuscle);
				this.Device.RenderState.Lighting = true;

				m_snSpringLength.SetFromValue(fltLength * Util.Environment.DistanceUnitValue, (int) Util.Environment.DistanceUnits);
			}
		}

		public override Vector3 FindPointOnSurface(Vector3 v3Start, Vector3 v3Direction)
		{
			return new Vector3(0,0,0);
		}

		protected override void BuildProperties()
		{
			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Name", m_strName.GetType(), "Name", 
																	"Spring Properties", "The name of this item.", m_strName));

			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Color", this.Color.GetType(), "Color",
																	"Spring Properties", "Sets the color used to draw the muscle.", this.Color));
			
			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Description", m_strDescription.GetType(), "Description", 
				"Spring Properties", "Sets the description for this body part.", m_strDescription, 
				typeof(AnimatTools.TypeHelpers.MultiLineStringTypeEditor)));

			Crownwood.Magic.Controls.PropertyBag pbNumberBag  = this.NaturalLength.Properties;
			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Natural Length", pbNumberBag.GetType(), "NaturalLength", 
																	"Spring Properties", "Sets the natrual length of this spring.", pbNumberBag,
																	"", typeof(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)));

			pbNumberBag  = this.Stiffness.Properties;
			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Stiffness", pbNumberBag.GetType(), "Stiffness", 
																"Spring Properties", "Sets the stiffness of this spring.", pbNumberBag,
																"", typeof(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)));

			pbNumberBag  = this.Damping.Properties;
			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Damping", pbNumberBag.GetType(), "Damping", 
																	"Spring Properties", "Sets the damping of this spring.", pbNumberBag,
																	"", typeof(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)));

			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Primary Attachment", typeof(AnimatTools.TypeHelpers.LinkedBodyPart), "PrimaryAttachment", 
																	"Spring Properties", "The primary muscle attachment.", 
																	m_thPrimaryAttachment, typeof(AnimatTools.TypeHelpers.DropDownListEditor), 
																	typeof(AnimatTools.TypeHelpers.LinkedBodyPartTypeConverter)));

			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Secondary Attachment", typeof(AnimatTools.TypeHelpers.LinkedBodyPart), "SecondaryAttachment", 
																	"Spring Properties", "The secondary muscle attachment.", 
																	m_thSecondaryAttachment, typeof(AnimatTools.TypeHelpers.DropDownListEditor), 
																	typeof(AnimatTools.TypeHelpers.LinkedBodyPartTypeConverter)));
		
			pbNumberBag = m_snSpringLength.Properties;
			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Spring Length", pbNumberBag.GetType(), "SpringLength", 
																	"Spring Properties", "The current length of the spring between the two attachment points.", pbNumberBag,
																	"", typeof(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter), true));

			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Enabled", m_bEnabled.GetType(), "Enabled",
																	"Spring Properties", "Determines whether the spring will actually develop force.", m_bEnabled));
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

			m_d3dMesh = Mesh.Sphere(this.Device, m_fRadius, 32, 32);

			float fltScale = (this.MaxDimension * 0.15f);
			if(fltScale > 0.3f) fltScale = 0.3f;

			m_mshSelectedBoundingBox = Mesh.Box(Device, (float)((2*m_fRadius) + fltScale),(float)((2*m_fRadius) + fltScale), (float)((2*m_fRadius) + fltScale)); 
			RefreshSelectionBoxSizes();

			base.CreateBody();
		}

		public override AnimatTools.DataObjects.Physical.BodyPart CreateNewBodyPart(AnimatTools.Framework.DataObject doParent)
		{return new Spring(doParent);}

		public override void AfterClone(AnimatTools.Framework.DataObject doParent, bool bCutData, AnimatTools.Framework.DataObject doOriginal, AnimatTools.Framework.DataObject doClone)
		{
			base.AfterClone (doParent, bCutData, doOriginal, doClone);

			if(Util.IsTypeOf(doClone.GetType(), typeof(AnimatTools.DataObjects.Physical.BodyPart), false))
			{
				//This is the new cloned root object, not the original root object
				AnimatTools.DataObjects.Physical.BodyPart doPart = (AnimatTools.DataObjects.Physical.BodyPart) doClone;

				if(m_thPrimaryAttachment != null && m_thPrimaryAttachment.BodyPart != null)
				{
					AnimatTools.DataObjects.Physical.BodyPart doBase = doPart.FindBodyPartByCloneID(m_thPrimaryAttachment.BodyPart.ID);
					if(doBase != null)
						m_thPrimaryAttachment.BodyPart = (MuscleAttachment) doBase;
				}

				if(m_thSecondaryAttachment != null && m_thSecondaryAttachment.BodyPart != null)
				{
					AnimatTools.DataObjects.Physical.BodyPart doBase = doPart.FindBodyPartByCloneID(m_thSecondaryAttachment.BodyPart.ID);
					if(doBase != null)
						m_thSecondaryAttachment.BodyPart = (MuscleAttachment) doBase;
				}
			}
		}

		protected override void CloneInternal(AnimatTools.Framework.DataObject doOriginal, bool bCutData, AnimatTools.Framework.DataObject doRoot)
		{
			base.CloneInternal (doOriginal, bCutData, doRoot);

			Spring doOrigBody = (Spring) doOriginal;

			m_thPrimaryAttachment = (AnimatTools.TypeHelpers.LinkedBodyPart) doOrigBody.m_thPrimaryAttachment.Clone(this, bCutData, doRoot);
			m_thSecondaryAttachment = (AnimatTools.TypeHelpers.LinkedBodyPart) doOrigBody.m_thSecondaryAttachment.Clone(this, bCutData, doRoot);
	
			m_snNaturalLength = (AnimatTools.Framework.ScaledNumber) doOrigBody.m_snNaturalLength.Clone(this, bCutData, doRoot);
			m_snStiffness = (AnimatTools.Framework.ScaledNumber) doOrigBody.m_snStiffness.Clone(this, bCutData, doRoot);
			m_snDamping = (AnimatTools.Framework.ScaledNumber) doOrigBody.m_snDamping.Clone(this, bCutData, doRoot);
			m_fRadius = doOrigBody.m_fRadius;
			m_bEnabled = doOrigBody.m_bEnabled;
		}

		public override AnimatTools.Framework.DataObject Clone(AnimatTools.Framework.DataObject doParent, bool bCutData, AnimatTools.Framework.DataObject doRoot)
		{
			Spring bnPart = new Spring(doParent);
			bnPart.CloneInternal(this, bCutData, doRoot);
			if(doRoot != null && doRoot == this) bnPart.AfterClone(this, bCutData, doRoot, bnPart);
			return bnPart;
		}

		public override void UnitsChanged(AnimatTools.DataObjects.Physical.Environment.enumMassUnits ePrevMass, AnimatTools.DataObjects.Physical.Environment.enumMassUnits eNewMass, float fltMassChange, AnimatTools.DataObjects.Physical.Environment.enumDistanceUnits ePrevDistance, AnimatTools.DataObjects.Physical.Environment.enumDistanceUnits eNewDistance, float fltDistanceChange)
		{
			base.UnitsChanged (ePrevMass, eNewMass, fltMassChange, ePrevDistance, eNewDistance, fltDistanceChange);

			m_snNaturalLength.ActualValue = m_snNaturalLength.ActualValue * fltDistanceChange;
      m_snStiffness.ActualValue = m_snStiffness.ActualValue / fltDistanceChange;
      m_snDamping.ActualValue = m_snStiffness.ActualValue / fltMassChange;
		}

		#region Loading and Saving Data

		public override void LoadData(ref AnimatTools.DataObjects.Simulation dsSim, ref AnimatTools.DataObjects.Physical.PhysicalStructure doStructure, ref AnimatTools.Interfaces.StdXml oXml)
		{
			m_aryChildBodies.Clear();

			oXml.IntoElem(); //'Into RigidBody Element

			m_strID = oXml.GetChildString("ID");
			m_strName = oXml.GetChildString("Name", m_strID);
			m_strDescription = oXml.GetChildString("Description", "");

			if(m_strName.Trim().Length == 0)
				m_strName = m_strID;

			if(oXml.FindChildElement("Color", false))
			{
				System.Drawing.Color oColor = Util.LoadColor(ref oXml, "Color");
				this.Alpha = oColor.A;
				this.Color = oColor;			
			}


			if(oXml.FindChildElement("MuscleAttachments", false))
			{
				oXml.IntoElem();

				oXml.AddChildElement("Description", m_strDescription);

				if(oXml.NumberOfChildren() > 2)
					throw new System.Exception("The standard spring can not have more than 2 attachment points.");
				else if(oXml.NumberOfChildren() == 2)
				{
					oXml.FindChildByIndex(0);
					m_strPrimaryAttachID = oXml.GetChildString();
					oXml.FindChildByIndex(1);
					m_strSecondaryAttachID = oXml.GetChildString();
				}
				else if(oXml.NumberOfChildren() == 1)
				{
					oXml.FindChildByIndex(0);
					m_strPrimaryAttachID = oXml.GetChildString();
				}
				else
				{
					m_strPrimaryAttachID = "";
					m_strSecondaryAttachID = "";
				}

				oXml.OutOfElem();
			}
			else
			{
				if(oXml.FindChildElement("PrimaryAttachmentID", false))
					m_strPrimaryAttachID = oXml.GetChildString("PrimaryAttachmentID");

				if(oXml.FindChildElement("SecondaryAttachmentID", false))
					m_strSecondaryAttachID = oXml.GetChildString("SecondaryAttachmentID");
			}

			m_snNaturalLength.LoadData(ref oXml, "NaturalLength");  
			m_snStiffness.LoadData(ref oXml, "Stiffness");  
			m_snDamping.LoadData(ref oXml, "Damping");  

			m_bEnabled = oXml.GetChildBool("Enabled", m_bEnabled);
			m_bVisible = oXml.GetChildBool("IsVisible", m_bVisible);

			oXml.OutOfElem(); //Outof RigidBody Element			
		}

		public override void InitializeAfterLoad(ref AnimatTools.DataObjects.Simulation dsSim, ref AnimatTools.DataObjects.Physical.PhysicalStructure doStructure)
		{
			if(m_strPrimaryAttachID.Trim().Length > 0)
			{
				AnimatTools.DataObjects.Physical.BodyPart rbPart = (AnimatTools.DataObjects.Physical.BodyPart) doStructure.FindBodyPart(m_strPrimaryAttachID, true);
				m_thPrimaryAttachment = new AnimatTools.TypeHelpers.LinkedBodyPartList(doStructure, rbPart, m_tpBodyPartType);
			}

			if(m_strSecondaryAttachID.Trim().Length > 0)
			{
				AnimatTools.DataObjects.Physical.BodyPart rbPart = (AnimatTools.DataObjects.Physical.BodyPart) doStructure.FindBodyPart(m_strSecondaryAttachID, true);
				m_thSecondaryAttachment = new AnimatTools.TypeHelpers.LinkedBodyPartList(doStructure, rbPart, m_tpBodyPartType);
			}
		}

		public override void SaveData(ref AnimatTools.DataObjects.Simulation dsSim, ref AnimatTools.DataObjects.Physical.PhysicalStructure doStructure, ref AnimatTools.Interfaces.StdXml oXml)
		{
			oXml.AddChildElement(this.BodyPartType);

			oXml.IntoElem(); //Into RigidBody Element
 
			oXml.AddChildElement("Name", m_strName);
			oXml.AddChildElement("ID", m_strID);
			oXml.AddChildElement("Type", this.Type);
            oXml.AddChildElement("PartType", this.PartType.ToString());
			oXml.AddChildElement("ModuleName", this.ModuleName);

			Util.SaveColor(ref oXml, "Color", m_d3dMaterial.Diffuse);

			if(m_thPrimaryAttachment != null && m_thPrimaryAttachment.BodyPart != null)
				oXml.AddChildElement("PrimaryAttachmentID", m_thPrimaryAttachment.BodyPart.ID);

			if(m_thSecondaryAttachment != null && m_thSecondaryAttachment.BodyPart != null)
				oXml.AddChildElement("SecondaryAttachmentID", m_thSecondaryAttachment.BodyPart.ID);

			m_snNaturalLength.SaveData(ref oXml, "NaturalLength");  
			m_snStiffness.SaveData(ref oXml, "Stiffness");  
			m_snDamping.SaveData(ref oXml, "Damping");  

			oXml.AddChildElement("Enabled", m_bEnabled);
			oXml.AddChildElement("IsVisible", m_bVisible);

			oXml.OutOfElem(); //Outof RigidBody Element			
		}

		#endregion

		#endregion
	}
}
