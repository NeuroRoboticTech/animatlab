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
	public class Muscle : AnimatTools.DataObjects.Behavior.Nodes.BodyPart
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
			get { return "Muscle"; }
		}

		[Browsable(false)]
		public override string ImageName
		{
			get { return "VortexAnimatTools.Graphics.ArmMuscle.gif"; }
		}

		[Browsable(false)]
		public override string DiagramImageName
		{
			get { return "VortexAnimatTools.Graphics.Muscle.gif"; }
			set {}
		}

		public Muscle(AnimatTools.Framework.DataObject doParent) : base(doParent)
		{			 
			try 
			{
				System.Reflection.Assembly myAssembly = System.Reflection.Assembly.Load("VortexAnimatTools");

				this.Size = new System.Drawing.SizeF(30, 150);
				this.Image = AnimatTools.Framework.ImageManager.LoadImage(ref myAssembly, "VortexAnimatTools.Graphics.ArmMuscle.gif", false);
				this.DragImage = AnimatTools.Framework.ImageManager.LoadImage(ref myAssembly, "VortexAnimatTools.Graphics.DragMuscle.gif", false);
				this.Name = "Muscle";
				this.Description = "This node allows the user to connect neural output directly to a physical muscle.";

				m_tpBodyPartType = typeof(VortexAnimatTools.DataObjects.Physical.RigidBodies.MuscleBase);
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
			Muscle bnMuscle = new Muscle(doParent);
			AnimatTools.DataObjects.Behavior.Data bnData = this;
			bnMuscle.CloneInternal(bnData, bCutData, doRoot);
			if(doRoot != null && doRoot == this) bnMuscle.AfterClone(this, bCutData, doRoot, bnMuscle);
			return bnMuscle;
		}
		/*
		public override void OwnerDraw(object sender, RectangleF szRect, Graphics eGraphics)
		{
			base.OwnerDraw (sender, szRect, eGraphics);

			eGraphics.DrawImage(m_MuscleImage, szRect);
		}
		*/

		protected override void BuildProperties()
		{
			base.BuildProperties();
			
      //Lets remove any properties that should not be displayed for a muscle node
//      m_Properties.Properties.Remove("AutoSize");
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
      m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Muscle ID", typeof(AnimatTools.TypeHelpers.LinkedBodyPart), "LinkedPart", 
                                  "Muscle Properties", "Associates this muscle node to an ID of a muscle that exists within the body of the organism.", 
																	m_thLinkedPart, typeof(AnimatTools.TypeHelpers.DropDownListEditor), typeof(AnimatTools.TypeHelpers.LinkedBodyPartTypeConverter)));

			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Node Type", typeof(String), "TypeName", 
																	"Muscle Properties", "Returns the type of this neuron.", this.TypeName, true));

			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Description", m_strDescription.GetType(), "ToolTip", 
																	"Muscle Properties", "Sets the description for this neuron.", m_strToolTip, 
																	 typeof(AnimatTools.TypeHelpers.MultiLineStringTypeEditor)));

		}

		public override void InitializeAfterLoad(int iAttempt)
		{
			try
			{
				base.InitializeAfterLoad(iAttempt);

				if(m_bInitialized)
				{
					AnimatTools.DataObjects.Physical.BodyPart bpPart = null;
					if(m_strLinkedBodyPartID.Length > 0)
						bpPart = m_doOrganism.FindBodyPart(m_strLinkedBodyPartID, false);

					m_thLinkedPart = new AnimatTools.TypeHelpers.LinkedBodyPartList(m_doOrganism, bpPart, m_tpBodyPartType);
					SetDataType();
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
				if(!m_ParentEditor.ErrorsBar.Errors.Contains(DataError.GenerateID(this, DiagramError.enumErrorTypes.MuscleNotSet)))
				{
					DataError deError = new DataError(this, DiagramError.enumErrorLevel.Error, DiagramError.enumErrorTypes.MuscleNotSet, 
						"The reference for the " + this.TypeName + " '" + this.Text + "' is not set.");
					m_ParentEditor.ErrorsBar.Errors.Add(deError.ID, deError);
				}
			}
			else
			{
				if(m_ParentEditor.ErrorsBar.Errors.Contains(DataError.GenerateID(this, DiagramError.enumErrorTypes.MuscleNotSet)))
					m_ParentEditor.ErrorsBar.Errors.Remove(DataError.GenerateID(this, DiagramError.enumErrorTypes.MuscleNotSet));

				//We need to check and see if there are any other muscle types with this same linked muscle body part.
				//There should only be one muscle behavioral node with this muscle ID
				if(this.Organism != null && this.Organism.BehavioralNodes != null)
				{
					VortexAnimatTools.DataObjects.Behavior.Nodes.Muscle doMuscle;
					bool bMatchFound = false;
					foreach (DictionaryEntry deEntry in this.Organism.BehavioralNodes)
					{
						if(deEntry.Value != null && Util.IsTypeOf(deEntry.Value.GetType(), typeof(VortexAnimatTools.DataObjects.Behavior.Nodes.Muscle), false))
						{
							doMuscle = (VortexAnimatTools.DataObjects.Behavior.Nodes.Muscle) deEntry.Value;

							if(doMuscle.LinkedPart != null && doMuscle.LinkedPart.BodyPart != null && doMuscle.LinkedPart.BodyPart.ID == m_thLinkedPart.BodyPart.ID && doMuscle != this)
								bMatchFound = true;
						}
					}

					if(bMatchFound)
					{
						if(!m_ParentEditor.ErrorsBar.Errors.Contains(DataError.GenerateID(this, DiagramError.enumErrorTypes.MuscleIDDuplicate)))
						{
							DataError deError = new DataError(this, DiagramError.enumErrorLevel.Error, DiagramError.enumErrorTypes.MuscleIDDuplicate, 
								"The body part id for the " + this.TypeName + " '" + this.Text + "' is used by more than one muscle reference. This can cause interference in setting the membrane voltage for the muscle. Instead use one motor neuron and have it do any integration of different inputs.");
							m_ParentEditor.ErrorsBar.Errors.Add(deError.ID, deError);
						}
					}
					else
					{
						if(m_ParentEditor.ErrorsBar.Errors.Contains(DataError.GenerateID(this, DiagramError.enumErrorTypes.MuscleIDDuplicate)))
							m_ParentEditor.ErrorsBar.Errors.Remove(DataError.GenerateID(this, DiagramError.enumErrorTypes.MuscleIDDuplicate));
					}
				}
			}

		}

	}
}
