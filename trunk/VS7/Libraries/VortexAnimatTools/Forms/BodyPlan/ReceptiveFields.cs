using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using AnimatTools.Framework;
using VortexAnimatTools.DataObjects;

namespace VortexAnimatTools.Forms.BodyPlan
{
	/// <summary>
	/// Summary description for ReceptiveFields.
	/// </summary>
	public class ReceptiveFields : AnimatTools.Forms.BodyPlan.ReceptiveFields
	{
		private AnimatTools.Forms.Gain.GainControl m_chartFieldGain;
		private AnimatTools.Forms.Gain.GainControl m_chartCurrentGain;

		private System.Windows.Forms.Panel pnlFields;
		private System.Windows.Forms.ComboBox cboNeurons;
		private System.Windows.Forms.TabControl tcGains;
		private System.Windows.Forms.TabPage tpFieldGain;
		private System.Windows.Forms.TabPage tpCurrentGain;
		private System.Windows.Forms.Button btnClearPairs;
		private System.Windows.Forms.Button btnRemovePair;
		private System.Windows.Forms.Button btnAddPair;
		private System.Windows.Forms.Label lblFieldAssociations;
		private System.Windows.Forms.ListView lvAssociationPairs;
		private System.Windows.Forms.Label lblVertices;
		private System.Windows.Forms.Label lblNeurons;
		private System.Windows.Forms.ComboBox cboVertices;
		private System.Windows.Forms.PropertyGrid grdFieldGainProps;
		private System.Windows.Forms.PropertyGrid grdCurrentGainProps;
		private System.Windows.Forms.Label lblFieldDistance;
		private System.Windows.Forms.TextBox txtFieldDistance;
		private System.Windows.Forms.ComboBox cboCurrentGainType;
		private System.Windows.Forms.ColumnHeader colVertex;
		private System.Windows.Forms.ColumnHeader colNeuron;
		private System.Windows.Forms.Button btnRefresh;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		public ReceptiveFields()
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
			System.Resources.ResourceManager resources = new System.Resources.ResourceManager(typeof(ReceptiveFields));
			this.pnlFields = new System.Windows.Forms.Panel();
			this.btnRefresh = new System.Windows.Forms.Button();
			this.txtFieldDistance = new System.Windows.Forms.TextBox();
			this.lblFieldDistance = new System.Windows.Forms.Label();
			this.cboNeurons = new System.Windows.Forms.ComboBox();
			this.tcGains = new System.Windows.Forms.TabControl();
			this.tpFieldGain = new System.Windows.Forms.TabPage();
			this.grdFieldGainProps = new System.Windows.Forms.PropertyGrid();
			this.tpCurrentGain = new System.Windows.Forms.TabPage();
			this.cboCurrentGainType = new System.Windows.Forms.ComboBox();
			this.grdCurrentGainProps = new System.Windows.Forms.PropertyGrid();
			this.btnClearPairs = new System.Windows.Forms.Button();
			this.btnRemovePair = new System.Windows.Forms.Button();
			this.btnAddPair = new System.Windows.Forms.Button();
			this.lblFieldAssociations = new System.Windows.Forms.Label();
			this.lvAssociationPairs = new System.Windows.Forms.ListView();
			this.colNeuron = new System.Windows.Forms.ColumnHeader();
			this.colVertex = new System.Windows.Forms.ColumnHeader();
			this.lblVertices = new System.Windows.Forms.Label();
			this.lblNeurons = new System.Windows.Forms.Label();
			this.cboVertices = new System.Windows.Forms.ComboBox();
			this.pnlFields.SuspendLayout();
			this.tcGains.SuspendLayout();
			this.tpFieldGain.SuspendLayout();
			this.tpCurrentGain.SuspendLayout();
			this.SuspendLayout();
			// 
			// pnlFields
			// 
			this.pnlFields.AutoScroll = true;
			this.pnlFields.Controls.Add(this.btnRefresh);
			this.pnlFields.Controls.Add(this.txtFieldDistance);
			this.pnlFields.Controls.Add(this.lblFieldDistance);
			this.pnlFields.Controls.Add(this.cboNeurons);
			this.pnlFields.Controls.Add(this.tcGains);
			this.pnlFields.Controls.Add(this.btnClearPairs);
			this.pnlFields.Controls.Add(this.btnRemovePair);
			this.pnlFields.Controls.Add(this.btnAddPair);
			this.pnlFields.Controls.Add(this.lblFieldAssociations);
			this.pnlFields.Controls.Add(this.lvAssociationPairs);
			this.pnlFields.Controls.Add(this.lblVertices);
			this.pnlFields.Controls.Add(this.lblNeurons);
			this.pnlFields.Controls.Add(this.cboVertices);
			this.pnlFields.Dock = System.Windows.Forms.DockStyle.Fill;
			this.pnlFields.Location = new System.Drawing.Point(0, 0);
			this.pnlFields.Name = "pnlFields";
			this.pnlFields.Size = new System.Drawing.Size(368, 446);
			this.pnlFields.TabIndex = 0;
			this.pnlFields.Visible = false;
			// 
			// btnRefresh
			// 
			this.btnRefresh.Image = ((System.Drawing.Image)(resources.GetObject("btnRefresh.Image")));
			this.btnRefresh.Location = new System.Drawing.Point(200, 264);
			this.btnRefresh.Name = "btnRefresh";
			this.btnRefresh.Size = new System.Drawing.Size(16, 23);
			this.btnRefresh.TabIndex = 44;
			this.btnRefresh.Click += new System.EventHandler(this.btnRefresh_Click);
			// 
			// txtFieldDistance
			// 
			this.txtFieldDistance.Location = new System.Drawing.Point(224, 267);
			this.txtFieldDistance.Name = "txtFieldDistance";
			this.txtFieldDistance.Size = new System.Drawing.Size(80, 20);
			this.txtFieldDistance.TabIndex = 43;
			this.txtFieldDistance.Text = "";
			this.txtFieldDistance.Validating += new System.ComponentModel.CancelEventHandler(this.txtFieldDistance_Validating);
			// 
			// lblFieldDistance
			// 
			this.lblFieldDistance.Location = new System.Drawing.Point(224, 256);
			this.lblFieldDistance.Name = "lblFieldDistance";
			this.lblFieldDistance.Size = new System.Drawing.Size(80, 16);
			this.lblFieldDistance.TabIndex = 42;
			this.lblFieldDistance.Text = "Field Distance";
			this.lblFieldDistance.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
			// 
			// cboNeurons
			// 
			this.cboNeurons.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.cboNeurons.Location = new System.Drawing.Point(8, 232);
			this.cboNeurons.Name = "cboNeurons";
			this.cboNeurons.Size = new System.Drawing.Size(176, 21);
			this.cboNeurons.Sorted = true;
			this.cboNeurons.TabIndex = 41;
			// 
			// tcGains
			// 
			this.tcGains.Controls.Add(this.tpFieldGain);
			this.tcGains.Controls.Add(this.tpCurrentGain);
			this.tcGains.Location = new System.Drawing.Point(8, 7);
			this.tcGains.Name = "tcGains";
			this.tcGains.SelectedIndex = 0;
			this.tcGains.Size = new System.Drawing.Size(352, 200);
			this.tcGains.TabIndex = 40;
			// 
			// tpFieldGain
			// 
			this.tpFieldGain.Controls.Add(this.grdFieldGainProps);
			this.tpFieldGain.Location = new System.Drawing.Point(4, 22);
			this.tpFieldGain.Name = "tpFieldGain";
			this.tpFieldGain.Size = new System.Drawing.Size(344, 174);
			this.tpFieldGain.TabIndex = 0;
			this.tpFieldGain.Text = "Field Gain";
			this.tpFieldGain.ToolTipText = "Receptive Field Gain Settings";
			// 
			// grdFieldGainProps
			// 
			this.grdFieldGainProps.CommandsVisibleIfAvailable = true;
			this.grdFieldGainProps.Font = new System.Drawing.Font("Tahoma", 11F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.World);
			this.grdFieldGainProps.HelpVisible = false;
			this.grdFieldGainProps.LargeButtons = false;
			this.grdFieldGainProps.LineColor = System.Drawing.SystemColors.ScrollBar;
			this.grdFieldGainProps.Location = new System.Drawing.Point(8, 8);
			this.grdFieldGainProps.Name = "grdFieldGainProps";
			this.grdFieldGainProps.Size = new System.Drawing.Size(112, 160);
			this.grdFieldGainProps.TabIndex = 3;
			this.grdFieldGainProps.Text = "grdFieldGainProps";
			this.grdFieldGainProps.ToolbarVisible = false;
			this.grdFieldGainProps.ViewBackColor = System.Drawing.SystemColors.Window;
			this.grdFieldGainProps.ViewForeColor = System.Drawing.SystemColors.WindowText;
			this.grdFieldGainProps.PropertyValueChanged += new System.Windows.Forms.PropertyValueChangedEventHandler(this.grdFieldGainProps_PropertyValueChanged);
			// 
			// tpCurrentGain
			// 
			this.tpCurrentGain.Controls.Add(this.cboCurrentGainType);
			this.tpCurrentGain.Controls.Add(this.grdCurrentGainProps);
			this.tpCurrentGain.Location = new System.Drawing.Point(4, 22);
			this.tpCurrentGain.Name = "tpCurrentGain";
			this.tpCurrentGain.Size = new System.Drawing.Size(344, 174);
			this.tpCurrentGain.TabIndex = 1;
			this.tpCurrentGain.Text = "Current Gain";
			this.tpCurrentGain.ToolTipText = "Receptive Field Current Gain Settings";
			this.tpCurrentGain.Visible = false;
			// 
			// cboCurrentGainType
			// 
			this.cboCurrentGainType.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.cboCurrentGainType.Location = new System.Drawing.Point(8, 8);
			this.cboCurrentGainType.Name = "cboCurrentGainType";
			this.cboCurrentGainType.Size = new System.Drawing.Size(112, 21);
			this.cboCurrentGainType.Sorted = true;
			this.cboCurrentGainType.TabIndex = 6;
			this.cboCurrentGainType.SelectedValueChanged += new System.EventHandler(this.cboCurrentGainType_SelectedValueChanged);
			// 
			// grdCurrentGainProps
			// 
			this.grdCurrentGainProps.CommandsVisibleIfAvailable = true;
			this.grdCurrentGainProps.HelpVisible = false;
			this.grdCurrentGainProps.LargeButtons = false;
			this.grdCurrentGainProps.LineColor = System.Drawing.SystemColors.ScrollBar;
			this.grdCurrentGainProps.Location = new System.Drawing.Point(8, 32);
			this.grdCurrentGainProps.Name = "grdCurrentGainProps";
			this.grdCurrentGainProps.Size = new System.Drawing.Size(112, 136);
			this.grdCurrentGainProps.TabIndex = 5;
			this.grdCurrentGainProps.Text = "grdCurrentGainProps";
			this.grdCurrentGainProps.ToolbarVisible = false;
			this.grdCurrentGainProps.ViewBackColor = System.Drawing.SystemColors.Window;
			this.grdCurrentGainProps.ViewForeColor = System.Drawing.SystemColors.WindowText;
			this.grdCurrentGainProps.PropertyValueChanged += new System.Windows.Forms.PropertyValueChangedEventHandler(this.grdCurrentGainProps_PropertyValueChanged);
			// 
			// btnClearPairs
			// 
			this.btnClearPairs.Location = new System.Drawing.Point(136, 262);
			this.btnClearPairs.Name = "btnClearPairs";
			this.btnClearPairs.Size = new System.Drawing.Size(56, 25);
			this.btnClearPairs.TabIndex = 39;
			this.btnClearPairs.Text = "Clear";
			this.btnClearPairs.Click += new System.EventHandler(this.btnClearPairs_Click);
			// 
			// btnRemovePair
			// 
			this.btnRemovePair.Location = new System.Drawing.Point(72, 263);
			this.btnRemovePair.Name = "btnRemovePair";
			this.btnRemovePair.Size = new System.Drawing.Size(56, 24);
			this.btnRemovePair.TabIndex = 38;
			this.btnRemovePair.Text = "Remove";
			this.btnRemovePair.Click += new System.EventHandler(this.btnRemovePair_Click);
			// 
			// btnAddPair
			// 
			this.btnAddPair.Location = new System.Drawing.Point(8, 263);
			this.btnAddPair.Name = "btnAddPair";
			this.btnAddPair.Size = new System.Drawing.Size(56, 24);
			this.btnAddPair.TabIndex = 37;
			this.btnAddPair.Text = "Add";
			this.btnAddPair.Click += new System.EventHandler(this.btnAddPair_Click);
			// 
			// lblFieldAssociations
			// 
			this.lblFieldAssociations.Location = new System.Drawing.Point(8, 295);
			this.lblFieldAssociations.Name = "lblFieldAssociations";
			this.lblFieldAssociations.Size = new System.Drawing.Size(344, 16);
			this.lblFieldAssociations.TabIndex = 36;
			this.lblFieldAssociations.Text = "Neuron\\Field Association Pairs";
			this.lblFieldAssociations.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
			// 
			// lvAssociationPairs
			// 
			this.lvAssociationPairs.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
																																												 this.colNeuron,
																																												 this.colVertex});
			this.lvAssociationPairs.FullRowSelect = true;
			this.lvAssociationPairs.GridLines = true;
			this.lvAssociationPairs.Location = new System.Drawing.Point(8, 311);
			this.lvAssociationPairs.MultiSelect = false;
			this.lvAssociationPairs.Name = "lvAssociationPairs";
			this.lvAssociationPairs.Size = new System.Drawing.Size(352, 128);
			this.lvAssociationPairs.Sorting = System.Windows.Forms.SortOrder.Ascending;
			this.lvAssociationPairs.TabIndex = 35;
			this.lvAssociationPairs.View = System.Windows.Forms.View.Details;
			this.lvAssociationPairs.SelectedIndexChanged += new System.EventHandler(this.lvAssociationPairs_SelectedIndexChanged);
			// 
			// colNeuron
			// 
			this.colNeuron.Text = "Neuron";
			this.colNeuron.Width = 100;
			// 
			// colVertex
			// 
			this.colVertex.Text = "Vertex";
			this.colVertex.Width = 150;
			// 
			// lblVertices
			// 
			this.lblVertices.Location = new System.Drawing.Point(184, 215);
			this.lblVertices.Name = "lblVertices";
			this.lblVertices.Size = new System.Drawing.Size(168, 16);
			this.lblVertices.TabIndex = 34;
			this.lblVertices.Text = "Vertices";
			this.lblVertices.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
			// 
			// lblNeurons
			// 
			this.lblNeurons.Location = new System.Drawing.Point(8, 215);
			this.lblNeurons.Name = "lblNeurons";
			this.lblNeurons.Size = new System.Drawing.Size(168, 16);
			this.lblNeurons.TabIndex = 33;
			this.lblNeurons.Text = "Neurons";
			this.lblNeurons.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
			// 
			// cboVertices
			// 
			this.cboVertices.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.cboVertices.Location = new System.Drawing.Point(184, 231);
			this.cboVertices.Name = "cboVertices";
			this.cboVertices.Size = new System.Drawing.Size(176, 21);
			this.cboVertices.TabIndex = 32;
			this.cboVertices.SelectedValueChanged += new System.EventHandler(this.cboVertices_SelectedValueChanged);
			// 
			// ReceptiveFields
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.AutoScroll = true;
			this.ClientSize = new System.Drawing.Size(368, 446);
			this.Controls.Add(this.pnlFields);
			this.Name = "ReceptiveFields";
			this.Resize += new System.EventHandler(this.ReceptiveFields_Resize);
			this.pnlFields.ResumeLayout(false);
			this.tcGains.ResumeLayout(false);
			this.tpFieldGain.ResumeLayout(false);
			this.tpCurrentGain.ResumeLayout(false);
			this.ResumeLayout(false);

		}
		#endregion

		public override bool MakeVisible
		{
			get{return pnlFields.Visible;}
			set{pnlFields.Visible = value;}
		}		

		public override void Initialize(ref AnimatTools.Forms.AnimatApplication frmApplication, AnimatTools.Forms.MdiChild frmMdiParent, AnimatTools.Forms.AnimatForm frmParent)
		{
			base.Initialize (ref frmApplication, frmMdiParent, frmParent);

      System.Reflection.Assembly oAssembly = System.Reflection.Assembly.LoadFrom(Util.GetFilePath(frmApplication.ApplicationDirectory, "LicensedAnimatTools.dll"));
      m_chartFieldGain = (AnimatTools.Forms.Gain.GainControl) oAssembly.CreateInstance("LicensedAnimatTools.Forms.Charts.GainControl");

			this.tpFieldGain.Controls.Add(this.m_chartFieldGain);
			this.m_chartFieldGain.Location = new System.Drawing.Point(128, 8);
			this.m_chartFieldGain.Name = "m_chartFieldGain";
			this.m_chartFieldGain.Size = new System.Drawing.Size(208, 160);
			this.m_chartFieldGain.TabIndex = 5;
			this.m_chartFieldGain.Text = "m_chartFieldGain";

			m_chartCurrentGain = (AnimatTools.Forms.Gain.GainControl) oAssembly.CreateInstance("LicensedAnimatTools.Forms.Charts.GainControl");

			this.tpCurrentGain.Controls.Add(this.m_chartCurrentGain);
			this.m_chartCurrentGain.Location = new System.Drawing.Point(128, 8);
			this.m_chartCurrentGain.Name = "m_chartCurrentGain";
			this.m_chartCurrentGain.Size = new System.Drawing.Size(208, 160);
			this.m_chartCurrentGain.TabIndex = 5;
			this.m_chartCurrentGain.Text = "m_chartCurrentGain";

			PopulateForm();
		}

		protected override void PopulateForm()
		{
			if(this.Editor != null && this.Editor.PhysicalStructure != null && 
				this.Editor.PhysicalStructure is AnimatTools.DataObjects.Physical.Organism && 
				m_doSelectedPart != null && m_doSelectedPart is AnimatTools.DataObjects.Physical.RigidBody)
			{
				AnimatTools.DataObjects.Physical.Organism doOrganism = (AnimatTools.DataObjects.Physical.Organism) this.Editor.PhysicalStructure;
				DataObjects.Physical.RigidBodies.RigidBody_DX rbPart = (DataObjects.Physical.RigidBodies.RigidBody_DX) m_doSelectedPart;
				
				//Fill in the gain types drop downs.
				if(cboCurrentGainType.Items.Count == 0)
					LoadGainTypes(cboCurrentGainType, rbPart.ReceptiveCurrentGain);

				m_chartFieldGain.Gain = rbPart.ReceptiveFieldGain;
				m_chartCurrentGain.Gain = rbPart.ReceptiveCurrentGain;

				m_chartFieldGain.MainTitle = "Field Gain";
				m_chartCurrentGain.MainTitle = "Current Gain";

				m_chartFieldGain.DrawGainChart(true);
				m_chartCurrentGain.DrawGainChart(true);

				AnimatTools.DataObjects.Behavior.Data doSelected = null;
				if(cboNeurons.SelectedItem != null)
					doSelected = (AnimatTools.DataObjects.Behavior.Data) cboNeurons.SelectedItem;

				cboNeurons.Items.Clear();
				foreach(DictionaryEntry deEntry in doOrganism.BehavioralNodes)
				{
					if(deEntry.Value is AnimatTools.DataObjects.Behavior.Nodes.Neuron)
					{
						cboNeurons.Items.Add(deEntry.Value);

						if(deEntry.Value == doSelected)
							cboNeurons.SelectedItem = deEntry.Value;
					}
				}

				if(doSelected == null && cboNeurons.Items.Count > 0)
					cboNeurons.SelectedIndex = 0;

				cboVertices.Items.Clear();
				foreach(Vec3d vPoint in rbPart.ReceptiveFields)
					cboVertices.Items.Add(vPoint);

				if(rbPart.SelectedReceptiveField != null)
					cboVertices.SelectedItem = rbPart.SelectedReceptiveField;


				if(rbPart.ReceptiveFieldGain != null)
					grdFieldGainProps.SelectedObject = rbPart.ReceptiveFieldGain.Properties;

				if(rbPart.ReceptiveCurrentGain != null)
					grdCurrentGainProps.SelectedObject = rbPart.ReceptiveCurrentGain.Properties;

				txtFieldDistance.Text = rbPart.ReceptiveFieldDistance.Text;

				ClearListView();

				AnimatTools.DataObjects.Physical.ReceptiveFieldPair doPair;
				ArrayList aryRemovePairs = new ArrayList();
				string [] aryItems = new string[2];
				foreach(DictionaryEntry deEntry in rbPart.ReceptiveFieldPairs)
				{	
					doPair = (AnimatTools.DataObjects.Physical.ReceptiveFieldPair) deEntry.Value;

					if(doPair.IsValidPair())
					{
						aryItems[0] = doPair.Neuron.Text;
						aryItems[1] = doPair.Vertex.ToString();

						ListViewItem liItem = new ListViewItem(aryItems);
						liItem.Tag = doPair;
						lvAssociationPairs.Items.Add(liItem);
					}
					else
					aryRemovePairs.Add(doPair);
				}

				foreach(AnimatTools.DataObjects.Physical.ReceptiveFieldPair doRemove in aryRemovePairs)
					rbPart.ReceptiveFieldPairs.Remove(doRemove.ID);
			}
		}

		protected void ClearListView()
		{
			lvAssociationPairs.Clear();

			this.lvAssociationPairs.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {this.colVertex, this.colNeuron});
		}

		protected void LoadGainTypes(ComboBox cboCombo, AnimatTools.DataObjects.Gain gnSelGain)
		{
			cboCombo.Items.Clear();
			foreach(AnimatTools.DataObjects.Gain gnGain in Util.Application.GainTypes)
			{
				if(gnGain.SelectableGain)
				{
					if(Util.IsTypeOf(gnGain.GetType(), gnSelGain.GetType(), false))
						cboCombo.Items.Add(gnSelGain);
					else
						cboCombo.Items.Add(gnGain);
				}
			}

			cboCombo.SelectedItem = gnSelGain;
		}

		public override void RefreshSelectedReceptiveField()
		{
			if(m_doSelectedPart != null && m_doSelectedPart is AnimatTools.DataObjects.Physical.RigidBody)
			{
				DataObjects.Physical.RigidBodies.RigidBody_DX rbPart = (DataObjects.Physical.RigidBodies.RigidBody_DX) m_doSelectedPart;
				if(rbPart.SelectedReceptiveField != null)
					cboVertices.SelectedItem = rbPart.SelectedReceptiveField;
			}
		}

		private void ReceptiveFields_Resize(object sender, System.EventArgs e)
		{
			try
			{
				int iWHalf = (this.Width - 15)/2;
				int iHHalf = (this.Height - lblVertices.Height - cboVertices.Height - btnAddPair.Height - 100)/2;

				tcGains.Width = this.Width - 10;

				m_chartCurrentGain.Width = tcGains.Width - grdCurrentGainProps.Width - 2;
//				grCurrentGain.Height = tcGains.Height - 5;
//				grdCurrentGainProps.Height = tcGains.Height - 5;
				m_chartFieldGain.Width = tcGains.Width - grdFieldGainProps.Width - 2;
//				grFieldGain.Height = tcGains.Height - 5;
//				grdFieldGainProps.Height = tcGains.Height - 5;

//				lblNeurons.Top = tcGains.Top + tcGains.Height + 5;
//				cboNeurons.Top = lblNeurons.Top + lblNeurons.Height + 5;
				lblNeurons.Width = iWHalf;
				cboNeurons.Width = iWHalf;

//				lblVertices.Top = tcGains.Top + tcGains.Height + 5;
//				cboVertices.Top = lblVertices.Top + lblVertices.Height + 5;
				lblVertices.Left = lblNeurons.Width + 10;
				cboVertices.Left = lblNeurons.Width + 10;
				lblVertices.Width = iWHalf;
				cboVertices.Width = iWHalf;

				lblFieldDistance.Top = txtFieldDistance.Top - lblFieldDistance.Height - 1;

//				btnAddPair.Top = cboNeurons.Top + cboNeurons.Height + 7;
//				btnRemovePair.Top = btnAddPair.Top;
//				btnClearPairs.Top = btnAddPair.Top;

//				lblFieldAssociations.Top = btnAddPair.Top + btnAddPair.Height + 5;
//				lvAssociationPairs.Top = lblFieldAssociations.Top + lblFieldAssociations.Height + 5;

				lblFieldAssociations.Width = this.Width - 10;
				lvAssociationPairs.Width = this.Width - 10;

				int iRemainder = this.Height - lvAssociationPairs.Top - 15;
				if(iRemainder < 30)
					iRemainder = 100;

				lvAssociationPairs.Height = iRemainder;
			}
			catch(System.Exception ex)
			{Util.DisplayError(ex);}			
		}

		private void grdCurrentGainProps_PropertyValueChanged(object s, System.Windows.Forms.PropertyValueChangedEventArgs e)
		{
			try
			{
				if(m_doSelectedPart != null && m_doSelectedPart is AnimatTools.DataObjects.Physical.RigidBody)
				{
					DataObjects.Physical.RigidBodies.RigidBody_DX rbPart = (DataObjects.Physical.RigidBodies.RigidBody_DX) m_doSelectedPart;
					m_chartCurrentGain.Gain = rbPart.ReceptiveCurrentGain;
					m_chartCurrentGain.DrawGainChart(true);
				}
			}
			catch(System.Exception ex)
			{Util.DisplayError(ex);}			
		}

		private void grdFieldGainProps_PropertyValueChanged(object s, System.Windows.Forms.PropertyValueChangedEventArgs e)
		{
			try
			{
				if(m_doSelectedPart != null && m_doSelectedPart is AnimatTools.DataObjects.Physical.RigidBody)
				{
					DataObjects.Physical.RigidBodies.RigidBody_DX rbPart = (DataObjects.Physical.RigidBodies.RigidBody_DX) m_doSelectedPart;
					m_chartFieldGain.Gain = rbPart.ReceptiveFieldGain;
					rbPart.ColorSelectedReceptiveField();
					m_chartFieldGain.DrawGainChart(true);
				}
			}
			catch(System.Exception ex)
			{Util.DisplayError(ex);}			
		}

		private void cboCurrentGainType_SelectedValueChanged(object sender, System.EventArgs e)
		{
			try
			{
				if(m_doSelectedPart != null && m_doSelectedPart is AnimatTools.DataObjects.Physical.RigidBody && cboCurrentGainType.SelectedItem != null)
				{
					DataObjects.Physical.RigidBodies.RigidBody_DX rbPart = (DataObjects.Physical.RigidBodies.RigidBody_DX) m_doSelectedPart;

					if(!Util.IsTypeOf(cboCurrentGainType.SelectedItem.GetType(), rbPart.ReceptiveCurrentGain.GetType(), false))
					{
						AnimatTools.DataObjects.Gain doGain = (AnimatTools.DataObjects.Gain) cboCurrentGainType.SelectedItem;
						AnimatTools.DataObjects.Gain doNewGain = (AnimatTools.DataObjects.Gain) doGain.Clone(rbPart, false, null);

						doNewGain.IndependentUnits = rbPart.ReceptiveCurrentGain.IndependentUnits;
						doNewGain.DependentUnits = rbPart.ReceptiveCurrentGain.DependentUnits;

						rbPart.ReceptiveCurrentGain = doNewGain;
						m_chartCurrentGain.DrawGainChart(true);
					}
				}
			}
			catch(System.Exception ex)
			{Util.DisplayError(ex);}	
		}

		private void txtFieldDistance_Validating(object sender, System.ComponentModel.CancelEventArgs e)
		{
			DataObjects.Physical.RigidBodies.RigidBody_DX rbPart = null;

			try
			{
				if(m_doSelectedPart != null && m_doSelectedPart is AnimatTools.DataObjects.Physical.RigidBody && cboCurrentGainType.SelectedItem != null)
				{
					rbPart = (DataObjects.Physical.RigidBodies.RigidBody_DX) m_doSelectedPart;

					if(txtFieldDistance.Text.Trim().Length == 0)
						throw new System.Exception("The field distance value can not be blank.");
					
					rbPart.ReceptiveFieldDistance = AnimatTools.Framework.ScaledNumber.Parse(txtFieldDistance.Text.Trim(), rbPart.ReceptiveFieldDistance.UnitsAbbreviation, rbPart.ReceptiveFieldDistance.Scale);
					txtFieldDistance.Text = rbPart.ReceptiveFieldDistance.Text;
					//rbPart.ReceptiveFieldPairs.Clear();
					//ClearListView();

					PopulateForm();
				}
			}
			catch(System.Exception ex)
			{
				Util.DisplayError(ex);
				e.Cancel = true;
				if(rbPart != null) 
					txtFieldDistance.Text = rbPart.ReceptiveFieldDistance.Text;
			}
		}

		private void cboVertices_SelectedValueChanged(object sender, System.EventArgs e)
		{
			try
			{
				if(m_doSelectedPart != null && m_doSelectedPart is AnimatTools.DataObjects.Physical.RigidBody && cboCurrentGainType.SelectedItem != null)
				{
					DataObjects.Physical.RigidBodies.RigidBody_DX rbPart = (DataObjects.Physical.RigidBodies.RigidBody_DX) m_doSelectedPart;

					AnimatTools.Framework.Vec3d vPoint = (AnimatTools.Framework.Vec3d) cboVertices.SelectedItem;
					if(!Util_DX.CompareVectors(rbPart.SelectedReceptiveField, vPoint))
						rbPart.SelectedReceptiveField = vPoint;
				}
			}
			catch(System.Exception ex)
			{Util.DisplayError(ex);}	
		}

		private void btnAddPair_Click(object sender, System.EventArgs e)
		{
			try
			{	
				if(m_doSelectedPart != null && m_doSelectedPart is AnimatTools.DataObjects.Physical.RigidBody && cboCurrentGainType.SelectedItem != null)
				{
					DataObjects.Physical.RigidBodies.RigidBody_DX rbPart = (DataObjects.Physical.RigidBodies.RigidBody_DX) m_doSelectedPart;

					if(cboNeurons.SelectedItem == null || cboVertices.SelectedItem == null)
						throw new System.Exception("You must select both a neuron and a vertex to add a receptive pair.");

					AnimatTools.DataObjects.Behavior.Nodes.Neuron doNeuron = (AnimatTools.DataObjects.Behavior.Nodes.Neuron) cboNeurons.SelectedItem;
					Vec3d vVertex = (Vec3d) cboVertices.SelectedItem;

					AnimatTools.DataObjects.Physical.ReceptiveFieldPair doPair = new AnimatTools.DataObjects.Physical.ReceptiveFieldPair(rbPart, vVertex, doNeuron);

					if(rbPart.ReceptiveFieldPairs.Contains(doPair.ID))
						throw new System.Exception("This rigid body already contains the pair associating neuron '" + doNeuron.Text + "' and vertex " + vVertex.ToString() + ".");

					rbPart.ReceptiveFieldPairs.Add(doPair.ID, doPair);

					string [] aryItems = new string[2];
					aryItems[0] = doNeuron.Text;
					aryItems[1] = vVertex.ToString();

					ListViewItem liItem = new ListViewItem(aryItems);
					liItem.Tag = doPair;
					lvAssociationPairs.Items.Add(liItem);
					liItem.Selected = true;
				}
			}
			catch(System.Exception ex)
			{Util.DisplayError(ex);}			
		}

		private void btnRemovePair_Click(object sender, System.EventArgs e)
		{
			try
			{
				if(m_doSelectedPart != null && m_doSelectedPart is AnimatTools.DataObjects.Physical.RigidBody && 
					cboCurrentGainType.SelectedItem != null && lvAssociationPairs.SelectedItems != null && lvAssociationPairs.SelectedItems.Count > 0)
				{
					DataObjects.Physical.RigidBodies.RigidBody_DX rbPart = (DataObjects.Physical.RigidBodies.RigidBody_DX) m_doSelectedPart;
					ListViewItem liItem = lvAssociationPairs.SelectedItems[0];

					if(liItem.Tag != null)
					{
						AnimatTools.DataObjects.Physical.ReceptiveFieldPair doPair = (AnimatTools.DataObjects.Physical.ReceptiveFieldPair) liItem.Tag;

						if(rbPart.ReceptiveFieldPairs.Contains(doPair.ID))
							rbPart.ReceptiveFieldPairs.Remove(doPair.ID);

						lvAssociationPairs.Items.Remove(liItem);
						
						cboVertices.SelectedItem = rbPart.ReceptiveFields[0];
					}
				}
			}
			catch(System.Exception ex)
			{Util.DisplayError(ex);}			
		}

		private void btnClearPairs_Click(object sender, System.EventArgs e)
		{
			try
			{
				if(m_doSelectedPart != null && m_doSelectedPart is AnimatTools.DataObjects.Physical.RigidBody)
				{
					DataObjects.Physical.RigidBodies.RigidBody_DX rbPart = (DataObjects.Physical.RigidBodies.RigidBody_DX) m_doSelectedPart;

					if(MessageBox.Show("This will remove all of the neuron/vertex pairs. Are you sure you want to proceed?", "Clear Pairs", MessageBoxButtons.YesNo) == DialogResult.Yes)
					{
						rbPart.ReceptiveFieldPairs.Clear();
						ClearListView();
					}
				}
			}
			catch(System.Exception ex)
			{Util.DisplayError(ex);}			
		}

		private void btnRefresh_Click(object sender, System.EventArgs e)
		{
			try
			{
				PopulateForm();
			}
			catch(System.Exception ex)
			{Util.DisplayError(ex);}			
		}

		private void lvAssociationPairs_SelectedIndexChanged(object sender, System.EventArgs e)
		{
			try
			{
				if(m_doSelectedPart != null && m_doSelectedPart is AnimatTools.DataObjects.Physical.RigidBody && 
					 cboCurrentGainType.SelectedItem != null && lvAssociationPairs.SelectedItems != null && lvAssociationPairs.SelectedItems.Count > 0)
				{
					ListViewItem liItem = lvAssociationPairs.SelectedItems[0];

					if(liItem.Tag != null)
					{
						AnimatTools.DataObjects.Physical.ReceptiveFieldPair doPair = (AnimatTools.DataObjects.Physical.ReceptiveFieldPair) liItem.Tag;
						cboNeurons.SelectedItem = doPair.Neuron;
						cboVertices.SelectedItem = doPair.Vertex;

						DataObjects.Physical.RigidBodies.RigidBody_DX rbPart = (DataObjects.Physical.RigidBodies.RigidBody_DX) m_doSelectedPart;

						if(!Util_DX.CompareVectors(rbPart.SelectedReceptiveField, doPair.Vertex))
							rbPart.SelectedReceptiveField = doPair.Vertex;
					}
				}
			}
			catch(System.Exception ex)
			{Util.DisplayError(ex);}			
		}

	}
}
