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
	public class Stomach : AnimatTools.DataObjects.Behavior.Nodes.BodyPart
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
					m_thLinkedPart = new AnimatTools.TypeHelpers.LinkedBodyPartList(m_ParentEditor.Organism, null, m_tpBodyPartType);
				}
			}
		}

		[Browsable(false)]
		public override string TypeName
		{
			get { return "Odor Sensor"; }
		}

		[Browsable(false)]
		public override string ImageName
		{
			get { return "VortexAnimatTools.Graphics.Stomach_Button.gif"; }
		}

		public Stomach(AnimatTools.Framework.DataObject doParent) : base(doParent)
		{			 
			try
			{
				m_thLinkedPart = new AnimatTools.TypeHelpers.LinkedBodyPartList(this);

				System.Reflection.Assembly myAssembly = System.Reflection.Assembly.Load("VortexAnimatTools");

				this.Size = new System.Drawing.SizeF(36, 40);
				this.Image = AnimatTools.Framework.ImageManager.LoadImage(ref myAssembly, "VortexAnimatTools.Graphics.Stomach_Button.gif", false);
				this.DragImage = AnimatTools.Framework.ImageManager.LoadImage(ref myAssembly, "VortexAnimatTools.Graphics.Stomach_Button.gif", false);
				this.Name = "Stomach";
				this.Description = "This node allows the user to collect interact with the Stomach object.";

				//Now lets try and find the image for this node. 
				this.ImageIndex = FindDiagramImageIndex(this.GetType().FullName);

				m_tpBodyPartType = typeof(VortexAnimatTools.DataObjects.Physical.RigidBodies.Stomach);
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
				diImage.Image = ImageManager.LoadImage(ref myAssembly, "VortexAnimatTools.Graphics.Stomach_Button.gif", true);
				diImage.ID = this.GetType().FullName;

				m_ParentEditor.DiagramImages.Add(diImage.ID, diImage);
				this.ImageIndex = diImage.DiagramIndex;
			}
		}

		public override AnimatTools.Framework.DataObject Clone(AnimatTools.Framework.DataObject doParent)
		{
			Stomach bnStomach = new Stomach(doParent);
			AnimatTools.DataObjects.Behavior.Data bnData = this;
			bnStomach.CloneInternal(bnData);
			return bnStomach;
		}

		protected override void BuildProperties()
		{
			base.BuildProperties();
			
			//Now lets add the property for the linked muscle.
			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Body ID", typeof(AnimatTools.TypeHelpers.LinkedBodyPartList), "LinkedPart", 
				"Body Properties", "Associates this body to an ID of a odor sensor that exists within the organism.", 
				m_thLinkedPart, typeof(AnimatTools.TypeHelpers.DropDownListEditor), typeof(AnimatTools.TypeHelpers.LinkedBodyPartTypeConverter)));

			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Node Type", typeof(String), "TypeName", 
				"Body Properties", "Returns the type of this item.", this.TypeName, true));

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

						m_thLinkedPart = new AnimatTools.TypeHelpers.LinkedBodyPartList(m_doOrganism, bpPart, m_tpBodyPartType);
						SetDataType();
					}
					else
						m_thLinkedPart = new AnimatTools.TypeHelpers.LinkedBodyPartList(m_doOrganism, null, m_tpBodyPartType);
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
