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
	public class MuscleSpindle : AnimatTools.DataObjects.Behavior.Nodes.BodyPart
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
			get { return "MuscleSpindle"; }
		}

		[Browsable(false)]
		public override string ImageName
		{
			get { return "VortexAnimatTools.Graphics.MuscleSpindle.gif"; }
		}

		[Browsable(false)]
		public override string DiagramImageName
		{
			get { return "VortexAnimatTools.Graphics.MuscleSpindle.gif"; }
			set {}
		}

		public MuscleSpindle(AnimatTools.Framework.DataObject doParent) : base(doParent)
		{			 
			try
			{
				System.Reflection.Assembly myAssembly = System.Reflection.Assembly.Load("VortexAnimatTools");
 
				this.Size = new System.Drawing.SizeF(30, 150);
				this.Image = AnimatTools.Framework.ImageManager.LoadImage(ref myAssembly, "VortexAnimatTools.Graphics.IconSpindle.gif", false);
				this.DragImage = AnimatTools.Framework.ImageManager.LoadImage(ref myAssembly, "VortexAnimatTools.Graphics.DragSpindle.gif", false);
				this.Name = "Stretch Receptor";
				this.Description = "This node allows the user to collect propreoceptive feedback from muscle stretch receptors.";

				m_tpBodyPartType = typeof(VortexAnimatTools.DataObjects.Physical.RigidBodies.LinearHillStretchReceptor);
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
			MuscleSpindle bnMuscle = new MuscleSpindle(doParent);
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
			
			//Now lets add the property for the linked muscle.
      m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Spindle ID", typeof(AnimatTools.TypeHelpers.LinkedBodyPart), "LinkedPart", 
                                  "Spindle Properties", "Associates this muscle node to an ID of a muscle that exists within the body of the organism.", 
																	m_thLinkedPart, typeof(AnimatTools.TypeHelpers.DropDownListEditor), typeof(AnimatTools.TypeHelpers.LinkedBodyPartTypeConverter)));

			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Node Type", typeof(String), "TypeName", 
																	"Spindle Properties", "Returns the type of this spindle.", this.TypeName, true));

			m_Properties.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Description", m_strDescription.GetType(), "ToolTip", 
																	"Spindle Properties", "Sets the description for this spindle.", m_strToolTip, 
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

	}
}
