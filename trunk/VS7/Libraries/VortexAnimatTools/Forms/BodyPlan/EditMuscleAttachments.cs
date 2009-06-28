using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using AnimatTools.Framework;

namespace VortexAnimatTools.Forms.BodyPlan
{
	/// <summary>
	/// Summary description for EditMuscleAttachments.
	/// </summary>
	public class EditMuscleAttachments : System.Windows.Forms.Form
	{
		private System.Windows.Forms.ListView lvMuscleAttachments;
		internal System.Windows.Forms.Button btnOk;
		internal System.Windows.Forms.Button btnCancel;
		internal System.Windows.Forms.Button btnDelete;
		internal System.Windows.Forms.Button btnAdd;
		protected bool m_bIsDirty = false;

		protected VortexAnimatTools.DataObjects.Physical.RigidBodies.MuscleBase  m_doMuscle;
		private System.Windows.Forms.Button btnUp;
		private System.Windows.Forms.ImageList ilImages;
		private System.Windows.Forms.Button btnDown;
		private System.Windows.Forms.ListView lvAttachments;
		private System.Windows.Forms.ColumnHeader colAttachment;
		private System.Windows.Forms.ColumnHeader colSelAttachments;
		private System.ComponentModel.IContainer components;

		public VortexAnimatTools.DataObjects.Physical.RigidBodies.MuscleBase Muscle
		{
			get{return m_doMuscle;}
			set{m_doMuscle = value;}
		}

		public EditMuscleAttachments()
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();

			//
			// TODO: Add any constructor code after InitializeComponent call
			//
		}

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				if(components != null)
				{
					components.Dispose();
				}
			}
			base.Dispose( disposing );
		}

		#region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			this.components = new System.ComponentModel.Container();
			System.Resources.ResourceManager resources = new System.Resources.ResourceManager(typeof(EditMuscleAttachments));
			this.lvMuscleAttachments = new System.Windows.Forms.ListView();
			this.colSelAttachments = new System.Windows.Forms.ColumnHeader();
			this.btnOk = new System.Windows.Forms.Button();
			this.btnCancel = new System.Windows.Forms.Button();
			this.btnDelete = new System.Windows.Forms.Button();
			this.btnAdd = new System.Windows.Forms.Button();
			this.btnUp = new System.Windows.Forms.Button();
			this.ilImages = new System.Windows.Forms.ImageList(this.components);
			this.btnDown = new System.Windows.Forms.Button();
			this.lvAttachments = new System.Windows.Forms.ListView();
			this.colAttachment = new System.Windows.Forms.ColumnHeader();
			this.SuspendLayout();
			// 
			// lvMuscleAttachments
			// 
			this.lvMuscleAttachments.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
				| System.Windows.Forms.AnchorStyles.Right)));
			this.lvMuscleAttachments.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
																								  this.colSelAttachments});
			this.lvMuscleAttachments.FullRowSelect = true;
			this.lvMuscleAttachments.GridLines = true;
			this.lvMuscleAttachments.HideSelection = false;
			this.lvMuscleAttachments.Location = new System.Drawing.Point(184, 16);
			this.lvMuscleAttachments.Name = "lvMuscleAttachments";
			this.lvMuscleAttachments.Size = new System.Drawing.Size(200, 240);
			this.lvMuscleAttachments.TabIndex = 0;
			this.lvMuscleAttachments.View = System.Windows.Forms.View.Details;
			// 
			// colSelAttachments
			// 
			this.colSelAttachments.Text = "Selected Attachments";
			this.colSelAttachments.Width = 170;
			// 
			// btnOk
			// 
			this.btnOk.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
			this.btnOk.Location = new System.Drawing.Point(8, 232);
			this.btnOk.Name = "btnOk";
			this.btnOk.Size = new System.Drawing.Size(64, 24);
			this.btnOk.TabIndex = 6;
			this.btnOk.Text = "Ok";
			this.btnOk.Click += new System.EventHandler(this.btnOk_Click);
			// 
			// btnCancel
			// 
			this.btnCancel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
			this.btnCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this.btnCancel.Location = new System.Drawing.Point(80, 232);
			this.btnCancel.Name = "btnCancel";
			this.btnCancel.Size = new System.Drawing.Size(64, 24);
			this.btnCancel.TabIndex = 7;
			this.btnCancel.Text = "Cancel";
			// 
			// btnDelete
			// 
			this.btnDelete.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
			this.btnDelete.Location = new System.Drawing.Point(80, 200);
			this.btnDelete.Name = "btnDelete";
			this.btnDelete.Size = new System.Drawing.Size(64, 24);
			this.btnDelete.TabIndex = 13;
			this.btnDelete.Text = "Remove";
			this.btnDelete.Click += new System.EventHandler(this.btnDelete_Click);
			// 
			// btnAdd
			// 
			this.btnAdd.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
			this.btnAdd.Location = new System.Drawing.Point(8, 200);
			this.btnAdd.Name = "btnAdd";
			this.btnAdd.Size = new System.Drawing.Size(64, 24);
			this.btnAdd.TabIndex = 12;
			this.btnAdd.Text = "Add";
			this.btnAdd.Click += new System.EventHandler(this.btnAdd_Click);
			// 
			// btnUp
			// 
			this.btnUp.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
			this.btnUp.ImageIndex = 1;
			this.btnUp.ImageList = this.ilImages;
			this.btnUp.Location = new System.Drawing.Point(152, 200);
			this.btnUp.Name = "btnUp";
			this.btnUp.Size = new System.Drawing.Size(24, 24);
			this.btnUp.TabIndex = 14;
			this.btnUp.Click += new System.EventHandler(this.btnUp_Click);
			// 
			// ilImages
			// 
			this.ilImages.ImageSize = new System.Drawing.Size(9, 9);
			this.ilImages.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("ilImages.ImageStream")));
			this.ilImages.TransparentColor = System.Drawing.Color.Transparent;
			// 
			// btnDown
			// 
			this.btnDown.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
			this.btnDown.ImageIndex = 0;
			this.btnDown.ImageList = this.ilImages;
			this.btnDown.Location = new System.Drawing.Point(152, 232);
			this.btnDown.Name = "btnDown";
			this.btnDown.Size = new System.Drawing.Size(24, 24);
			this.btnDown.TabIndex = 15;
			this.btnDown.Click += new System.EventHandler(this.btnDown_Click);
			// 
			// lvAttachments
			// 
			this.lvAttachments.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
				| System.Windows.Forms.AnchorStyles.Left) 
				| System.Windows.Forms.AnchorStyles.Right)));
			this.lvAttachments.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
																							this.colAttachment});
			this.lvAttachments.FullRowSelect = true;
			this.lvAttachments.GridLines = true;
			this.lvAttachments.HideSelection = false;
			this.lvAttachments.Location = new System.Drawing.Point(8, 16);
			this.lvAttachments.Name = "lvAttachments";
			this.lvAttachments.Size = new System.Drawing.Size(168, 176);
			this.lvAttachments.Sorting = System.Windows.Forms.SortOrder.Ascending;
			this.lvAttachments.TabIndex = 16;
			this.lvAttachments.View = System.Windows.Forms.View.Details;
			// 
			// colAttachment
			// 
			this.colAttachment.Text = "Available Attachments";
			this.colAttachment.Width = 160;
			// 
			// EditMuscleAttachments
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(392, 266);
			this.Controls.Add(this.lvAttachments);
			this.Controls.Add(this.btnDown);
			this.Controls.Add(this.btnUp);
			this.Controls.Add(this.btnDelete);
			this.Controls.Add(this.btnAdd);
			this.Controls.Add(this.btnCancel);
			this.Controls.Add(this.btnOk);
			this.Controls.Add(this.lvMuscleAttachments);
			this.Name = "EditMuscleAttachments";
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
			this.Text = "Edit Attachments";
			this.Load += new System.EventHandler(this.EditMuscleAttachments_Load);
			this.ResumeLayout(false);

		}
		#endregion

		protected ListViewItem FindListItem(ListView lView, string strID, bool bThrowError)
		{
			foreach(ListViewItem liItem in lView.Items)
			{
				if(liItem.Tag != null && Util.IsTypeOf(liItem.Tag.GetType(), typeof(AnimatTools.Framework.DataObject), true))
				{
					AnimatTools.Framework.DataObject doObject = (AnimatTools.Framework.DataObject) liItem.Tag;
					if(doObject.ID == strID)
						return liItem;
				}
			}

			if(bThrowError)
				throw new System.Exception("No listview item with ID '" + strID +"' was found");

			return null;
		}

		private void EditMuscleAttachments_Load(object sender, System.EventArgs e)
		{
			try
			{
				//Lets get the parent structure first.
				if(m_doMuscle == null)
					throw new System.Exception("The muscle is not defined.");

				AnimatTools.DataObjects.Physical.PhysicalStructure doStruct = m_doMuscle.ParentStructure;

				if(doStruct == null)
					throw new System.Exception("The parent structure is not defined.");

				//now lets populate the drop down box with all of the muscle attachments.

				AnimatTools.Collections.DataObjects aryAttachments = new AnimatTools.Collections.DataObjects(null);
				doStruct.FindChildrenOfType(typeof(VortexAnimatTools.DataObjects.Physical.RigidBodies.MuscleAttachment), ref aryAttachments);
				
				//Now lets populate the list view with the current muscle attachments.
				foreach(AnimatTools.Framework.DataObject doAttach in aryAttachments)
				{
					ListViewItem liItem = new ListViewItem(doAttach.ToString());
					liItem.Tag = doAttach;
					lvAttachments.Items.Add(liItem);
				}

				//Now lets populate the list box with the current muscle attachments.
				ListViewItem liFindItem;
				foreach(VortexAnimatTools.DataObjects.Physical.RigidBodies.MuscleAttachment doAttach in m_doMuscle.AttachmentPoints)
				{
					ListViewItem liItem = new ListViewItem(doAttach.ToString());
					liItem.Tag = doAttach;
					lvMuscleAttachments.Items.Add(liItem);

					liFindItem = FindListItem(lvAttachments, doAttach.ID, false);
					if(liFindItem != null)
						lvAttachments.Items.Remove(liFindItem);
				}
				

			}
			catch(System.Exception ex)
			{Util.DisplayError(ex);}
		}

		private void btnUp_Click(object sender, System.EventArgs e)
		{
			try
			{
				if(lvMuscleAttachments.SelectedItems.Count == 0)
					throw new System.Exception("Please select an item to move down.");
				if(lvMuscleAttachments.SelectedItems.Count > 1)
					throw new System.Exception("You can only move one item down at a time.");

				ListViewItem liItem = lvMuscleAttachments.SelectedItems[0];
				int iIndex = liItem.Index;

				if(iIndex > 0)
				{
					lvMuscleAttachments.Items.Remove(liItem);
					lvMuscleAttachments.Items.Insert(iIndex-1, liItem);
				}

				m_bIsDirty = true;
			}
			catch(System.Exception ex)
			{Util.DisplayError(ex);}		
		}

		private void btnDown_Click(object sender, System.EventArgs e)
		{
			try
			{
				if(lvMuscleAttachments.SelectedItems.Count == 0)
					throw new System.Exception("Please select an item to move down.");
				if(lvMuscleAttachments.SelectedItems.Count > 1)
					throw new System.Exception("You can only move one item down at a time.");

				ListViewItem liItem = lvMuscleAttachments.SelectedItems[0];
				int iIndex = liItem.Index;

				if(iIndex < (lvMuscleAttachments.Items.Count-1))
				{
					lvMuscleAttachments.Items.Remove(liItem);
					lvMuscleAttachments.Items.Insert(iIndex+1, liItem);
				}

				m_bIsDirty = true;
			}
			catch(System.Exception ex)
			{Util.DisplayError(ex);}		
		}

		private void btnAdd_Click(object sender, System.EventArgs e)
		{
			try
			{
				if(lvAttachments.SelectedItems.Count == 0)
					throw new System.Exception("you must select a muscle attachment to add.");

				ArrayList aryItems = new ArrayList();

				foreach(ListViewItem liItem in lvAttachments.SelectedItems)
					AddAttachment(liItem, aryItems);

				//now remove all the selected attachments that were added
				foreach(ListViewItem liItem in aryItems)
					lvAttachments.Items.Remove(liItem);

				m_bIsDirty = true;
			}
			catch(System.Exception ex)
			{Util.DisplayError(ex);}		
		}

		protected void AddAttachment(ListViewItem liSelItem, ArrayList aryItems)
		{
			try
			{
				//Now lets make sure it is not already in the list.
				foreach(ListViewItem liTempItem in lvMuscleAttachments.Items)
				{
					if(liTempItem.Tag == liSelItem.Tag)
					{
						VortexAnimatTools.DataObjects.Physical.RigidBodies.MuscleAttachment doAttach1 = (VortexAnimatTools.DataObjects.Physical.RigidBodies.MuscleAttachment) liTempItem.Tag;
						throw new System.Exception("The muscle attachment '" + doAttach1.Name + "' is already in the list.");
					}
				}

				VortexAnimatTools.DataObjects.Physical.RigidBodies.MuscleAttachment doAttach = (VortexAnimatTools.DataObjects.Physical.RigidBodies.MuscleAttachment) liSelItem.Tag;
					
				ListViewItem liItem = new ListViewItem(doAttach.ToString());
				liItem.Tag = doAttach;
				lvMuscleAttachments.Items.Add(liItem);

				aryItems.Add(liSelItem);
			}
			catch(System.Exception ex)
			{Util.DisplayError(ex);}		
		}

		private void btnDelete_Click(object sender, System.EventArgs e)
		{
			try
			{
				ArrayList aryList = new ArrayList();
				foreach(ListViewItem liItem in lvMuscleAttachments.SelectedItems)
					aryList.Add(liItem);

				foreach(ListViewItem liItem in aryList)
				{
					lvMuscleAttachments.Items.Remove(liItem);
					lvAttachments.Items.Add(liItem);
				}

				m_bIsDirty = true;
			}
			catch(System.Exception ex)
			{Util.DisplayError(ex);}		
		}

		private void btnOk_Click(object sender, System.EventArgs e)
		{
			try
			{
				if(m_bIsDirty)
				{
					VortexAnimatTools.Collections.MuscleAttachments aryOldPoints = (VortexAnimatTools.Collections.MuscleAttachments) m_doMuscle.AttachmentPoints.Clone(m_doMuscle.AttachmentPoints.Parent, false, null);

					m_doMuscle.AttachmentPoints.Clear();

					foreach(ListViewItem liItem in lvMuscleAttachments.Items)
						m_doMuscle.AttachmentPoints.Add((VortexAnimatTools.DataObjects.Physical.RigidBodies.MuscleAttachment) liItem.Tag);

					VortexAnimatTools.Collections.MuscleAttachments aryNewPoints = (VortexAnimatTools.Collections.MuscleAttachments) m_doMuscle.AttachmentPoints.Clone(m_doMuscle.AttachmentPoints.Parent, false, null);

					this.DialogResult = System.Windows.Forms.DialogResult.OK;

					//If this is the first time we are setting the attachment points for the muscles then lets setup the length properties of the muscle.
					if(aryOldPoints.Count <= 1 && aryNewPoints.Count >= 2)
					{
						AnimatTools.Framework.ScaledNumber snLength = (AnimatTools.Framework.ScaledNumber) m_doMuscle.LengthTension.RestingLength.Clone(m_doMuscle.LengthTension.RestingLength.Parent, false, null);
						AnimatTools.Framework.ScaledNumber snLwidth = (AnimatTools.Framework.ScaledNumber) m_doMuscle.LengthTension.Lwidth.Clone(m_doMuscle.LengthTension.Lwidth.Parent, false, null);

						m_doMuscle.LengthTension.RestingLength.SetFromValue(m_doMuscle.CalculateMuscleLength(), (int) Util.Environment.DisplayDistanceUnits);
						m_doMuscle.LengthTension.Lwidth.SetFromValue((m_doMuscle.LengthTension.RestingLength.ActualValue*0.3f), (int) Util.Environment.DisplayDistanceUnits);
						m_doMuscle.LengthTension.LowerLimit.SetFromValue(m_doMuscle.LengthTension.RestingLength.ActualValue*0.9);
						m_doMuscle.LengthTension.UpperLimit.SetFromValue(m_doMuscle.LengthTension.RestingLength.ActualValue*1.1);

						Util.ModificationHistory.BeginHistoryGroup();
						m_doMuscle.ManualAddPropertyHistory("AttachmentPoints", aryOldPoints, aryNewPoints, true);
						m_doMuscle.LengthTension.ManualAddPropertyHistory("RestingLength", snLength, m_doMuscle.LengthTension.RestingLength.Clone(m_doMuscle.LengthTension.RestingLength.Parent, false, null), true);
						m_doMuscle.LengthTension.ManualAddPropertyHistory("Lwidth", snLwidth, m_doMuscle.LengthTension.Lwidth.Clone(m_doMuscle.LengthTension.Lwidth.Parent, false, null), true);
						Util.ModificationHistory.CommitHistoryGroup();
					}
					else
						m_doMuscle.ManualAddPropertyHistory("AttachmentPoints", aryOldPoints, aryNewPoints, true);

				}
				else
					this.DialogResult = System.Windows.Forms.DialogResult.Cancel;

				this.Close();
			}
			catch(System.Exception ex)
			{Util.DisplayError(ex);}		
		}

	}
}
