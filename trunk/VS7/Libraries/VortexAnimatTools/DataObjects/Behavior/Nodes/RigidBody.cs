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
using AnimatTools.DataObjects.Behavior;
using AnimatTools.DataObjects.Behavior.DiagramErrors;

namespace VortexAnimatTools.DataObjects.Behavior.Nodes
{
	/// <summary>
	/// Summary description for Muscle.
	/// </summary>
	public class RigidBody : AnimatTools.DataObjects.Behavior.Nodes.BodyPart
	{

		[Browsable(false)]
		public override AnimatTools.Forms.Behavior.Editor ParentEditor
		{
			get
			{ 
				return base.ParentEditor;
			}
			set
			{
				base.ParentEditor = value;

				if(m_ParentEditor != null && m_ParentEditor.Organism != null)
				{
					this.Organism = m_ParentEditor.Organism;
					m_thLinkedPart = new AnimatTools.TypeHelpers.LinkedBodyPartTree(m_ParentEditor.Organism, null, m_tpBodyPartType);
				}
			}
		}

		[Browsable(false)]
		public override string TypeName
		{
			get { return "Rigid Body"; }
		}

		[Browsable(false)]
		public override string ImageName
		{
			get { return "VortexAnimatTools.Graphics.Bone.gif"; }
		}

		[Browsable(false)]
		public override string DiagramImageName
		{
			get { return  "VortexAnimatTools.Graphics.BoneNodeImage.gif"; }
			set {}
		}

		public RigidBody(AnimatTools.Framework.DataObject doParent) : base(doParent)
		{			 
			try
			{
				m_thLinkedPart = new AnimatTools.TypeHelpers.LinkedBodyPartTree(this);

				System.Reflection.Assembly myAssembly = System.Reflection.Assembly.Load("VortexAnimatTools");

				this.Size = new System.Drawing.SizeF(50, 50);
				this.Image = AnimatTools.Framework.ImageManager.LoadImage(ref myAssembly, "VortexAnimatTools.Graphics.Bone.gif", false);
				this.DragImage = AnimatTools.Framework.ImageManager.LoadImage(ref myAssembly, "VortexAnimatTools.Graphics.DragBone.gif", false);
				this.Name = "Rigid Body";
				this.Description = "This node allows the user to collect data directly from a rigid body.";
				this.Shape = AnimatTools.DataObjects.Behavior.Node.enumShape.Hexagon;
				this.FillColor = Color.LightBlue;

				m_tpBodyPartType = typeof(AnimatTools.DataObjects.Physical.RigidBody);
			}
			catch(System.Exception ex)
			{
				AnimatTools.Framework.Util.DisplayError(ex);
			}
		}

		public override void AfterAddedToIconBand()
		{
			if(m_ParentEditor != null && !m_ParentEditor.DiagramImages.Contains(this.GetType().FullName))
			{
				AnimatTools.DataObjects.Behavior.DiagramImage diImage = new AnimatTools.DataObjects.Behavior.DiagramImage(m_ParentEditor.FormHelper);

				System.Reflection.Assembly myAssembly = System.Reflection.Assembly.Load("VortexAnimatTools");
				diImage.Image = ImageManager.LoadImage(ref myAssembly, this.DiagramImageName, true);
				diImage.ID = this.DiagramImageName;

				if(!m_ParentEditor.DiagramImages.Contains(diImage.ID))
					m_ParentEditor.DiagramImages.Add(diImage.ID, diImage);
			}
		}

		public override AnimatTools.Framework.DataObject Clone(AnimatTools.Framework.DataObject doParent, bool bCutData, AnimatTools.Framework.DataObject doRoot)
		{
			RigidBody bnRigidBody = new RigidBody(doParent);
			AnimatTools.DataObjects.Behavior.Data bnData = this;
			bnRigidBody.CloneInternal(bnData, bCutData, doRoot);
			if(doRoot != null && doRoot == this) bnRigidBody.AfterClone(this, bCutData, doRoot, bnRigidBody);
			return bnRigidBody;
		}

		protected override void BuildProperties()
		{
			base.BuildProperties();
			
			//Lets remove any properties that should not be displayed for a muscle node
//			m_Properties.Properties.Remove("AutoSize");
//			m_Properties.Properties.Remove("Back Mode");
//			m_Properties.Properties.Remove("Dash Style");
//			m_Properties.Properties.Remove("Draw Color");
//			m_Properties.Properties.Remove("Draw Width");
//			m_Properties.Properties.Remove("Fill Color");
//			m_Properties.Properties.Remove("Gradient");
//			m_Properties.Properties.Remove("Gradient Color");
//			m_Properties.Properties.Remove("Gradient Mode");
//			m_Properties.Properties.Remove("Shadow Color");
//			m_Properties.Properties.Remove("Shadow Size");
//			m_Properties.Properties.Remove("Shadow Style");
//			m_Properties.Properties.Remove("Shape");
//			m_Properties.Properties.Remove("Shape Orientation");
//			m_Properties.Properties.Remove("Transparent");
//			m_Properties.Properties.Remove("Trimming");
//			m_Properties.Properties.Remove("Node Type");
//			m_Properties.Properties.Remove("Description");

			//Now lets add the property for the linked muscle.
			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Body ID", typeof(AnimatTools.TypeHelpers.LinkedBodyPartTree), "LinkedPart", 
				"Body Properties", "Associates this body to an ID of a rigid body that exists within the organism.", 
				m_thLinkedPart, typeof(AnimatTools.TypeHelpers.DropDownTreeEditor), typeof(AnimatTools.TypeHelpers.LinkedBodyPartTypeConverter)));

			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Node Type", typeof(String), "TypeName", 
				"Body Properties", "Returns the type of this neuron.", this.TypeName, true));

			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Description", m_strDescription.GetType(), "ToolTip", 
				"Body Properties", "Sets the description for this neuron.", m_strToolTip, 
				typeof(AnimatTools.TypeHelpers.MultiLineStringTypeEditor)));
		}

		public override void InitializeAfterLoad(int iAttempt)
		{
			try
			{
				base.InitializeAfterLoad(iAttempt);

				if(m_bInitialized)
				{
					if(m_strLinkedBodyPartID.Length > 0)
					{
						AnimatTools.DataObjects.Physical.BodyPart bpPart = m_doOrganism.FindBodyPart(m_strLinkedBodyPartID, false);

						m_thLinkedPart = new AnimatTools.TypeHelpers.LinkedBodyPartTree(m_doOrganism, bpPart, m_tpBodyPartType);
						SetDataType();
					}
					else
						m_thLinkedPart = new AnimatTools.TypeHelpers.LinkedBodyPartTree(m_doOrganism, null, m_tpBodyPartType);
				}
			}
			catch(System.Exception ex)
			{
				m_bInitialized = false;
				if(iAttempt == 1)
					AnimatTools.Framework.Util.DisplayError(ex);
			}
		}

		public override void CheckForErrors()
		{
			base.CheckForErrors();

			if(m_ParentEditor == null || m_ParentEditor.ErrorsBar == null)
				return;

			if(m_thLinkedPart == null || m_thLinkedPart.BodyPart == null)
			{
				if(!m_ParentEditor.ErrorsBar.Errors.Contains(DataError.GenerateID(this, DiagramError.enumErrorTypes.RigidBodyNotSet)))
				{
					DataError deError = new DataError(this, DiagramError.enumErrorLevel.Error, DiagramError.enumErrorTypes.RigidBodyNotSet, 
						"The reference for the rigid body '" + this.Text + "' is not set.");
					m_ParentEditor.ErrorsBar.Errors.Add(deError.ID, deError);
				}
			}
			else
			{
				if(m_ParentEditor.ErrorsBar.Errors.Contains(DataError.GenerateID(this, DiagramError.enumErrorTypes.RigidBodyNotSet)))
					m_ParentEditor.ErrorsBar.Errors.Remove(DataError.GenerateID(this, DiagramError.enumErrorTypes.RigidBodyNotSet));
			}

		}

	}
}
