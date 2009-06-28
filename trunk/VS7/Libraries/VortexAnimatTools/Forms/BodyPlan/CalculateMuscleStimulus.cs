using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using AnimatTools.Framework;

namespace VortexAnimatTools.Forms.BodyPlan
{
	/// <summary>
	/// Summary description for CalculateMuscleStimulus.
	/// </summary>
	public class CalculateMuscleStimulus : System.Windows.Forms.Form
	{
		protected VortexAnimatTools.DataObjects.Physical.RigidBodies.LinearHillMuscle  m_doMuscle;
		private System.Windows.Forms.TextBox txtTension;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.TextBox txtLengthOffset;
		internal System.Windows.Forms.Button btnCalculate;
		private System.Windows.Forms.Label label3;
		private System.Windows.Forms.Label label4;
		private System.Windows.Forms.Label label5;
		private System.Windows.Forms.TextBox txtTl;
		private System.Windows.Forms.TextBox txtActivation;
		private System.Windows.Forms.TextBox txtVoltage;
		internal System.Windows.Forms.Button btnClose;
		private System.Windows.Forms.Label label6;
		private System.Windows.Forms.TextBox txtXdot;
		private System.Windows.Forms.Label label7;
		private System.Windows.Forms.TextBox txtTdot;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		public VortexAnimatTools.DataObjects.Physical.RigidBodies.LinearHillMuscle Muscle
		{
			get{return m_doMuscle;}
			set{m_doMuscle = value;}
		}

		public CalculateMuscleStimulus()
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
			this.btnCalculate = new System.Windows.Forms.Button();
			this.txtTension = new System.Windows.Forms.TextBox();
			this.label1 = new System.Windows.Forms.Label();
			this.label2 = new System.Windows.Forms.Label();
			this.txtLengthOffset = new System.Windows.Forms.TextBox();
			this.label3 = new System.Windows.Forms.Label();
			this.label4 = new System.Windows.Forms.Label();
			this.label5 = new System.Windows.Forms.Label();
			this.txtTl = new System.Windows.Forms.TextBox();
			this.txtActivation = new System.Windows.Forms.TextBox();
			this.txtVoltage = new System.Windows.Forms.TextBox();
			this.btnClose = new System.Windows.Forms.Button();
			this.label6 = new System.Windows.Forms.Label();
			this.txtXdot = new System.Windows.Forms.TextBox();
			this.label7 = new System.Windows.Forms.Label();
			this.txtTdot = new System.Windows.Forms.TextBox();
			this.SuspendLayout();
			// 
			// btnCalculate
			// 
			this.btnCalculate.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
			this.btnCalculate.Location = new System.Drawing.Point(8, 176);
			this.btnCalculate.Name = "btnCalculate";
			this.btnCalculate.Size = new System.Drawing.Size(64, 24);
			this.btnCalculate.TabIndex = 9;
			this.btnCalculate.Text = "Calculate";
			this.btnCalculate.Click += new System.EventHandler(this.btnCalculate_Click);
			// 
			// txtTension
			// 
			this.txtTension.Location = new System.Drawing.Point(8, 24);
			this.txtTension.Name = "txtTension";
			this.txtTension.Size = new System.Drawing.Size(136, 20);
			this.txtTension.TabIndex = 10;
			this.txtTension.Text = "10";
			// 
			// label1
			// 
			this.label1.Location = new System.Drawing.Point(8, 8);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(136, 16);
			this.label1.TabIndex = 11;
			this.label1.Text = "Desired Tension (N)";
			this.label1.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
			// 
			// label2
			// 
			this.label2.Location = new System.Drawing.Point(8, 48);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(136, 16);
			this.label2.TabIndex = 13;
			this.label2.Text = "Length Offset (m)";
			this.label2.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
			// 
			// txtLengthOffset
			// 
			this.txtLengthOffset.Location = new System.Drawing.Point(8, 64);
			this.txtLengthOffset.Name = "txtLengthOffset";
			this.txtLengthOffset.Size = new System.Drawing.Size(136, 20);
			this.txtLengthOffset.TabIndex = 12;
			this.txtLengthOffset.Text = "0";
			// 
			// label3
			// 
			this.label3.Location = new System.Drawing.Point(152, 8);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(136, 16);
			this.label3.TabIndex = 15;
			this.label3.Text = "Tension Length %";
			this.label3.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
			// 
			// label4
			// 
			this.label4.Location = new System.Drawing.Point(152, 48);
			this.label4.Name = "label4";
			this.label4.Size = new System.Drawing.Size(136, 16);
			this.label4.TabIndex = 17;
			this.label4.Text = "Activation Tension (N)";
			this.label4.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
			// 
			// label5
			// 
			this.label5.Location = new System.Drawing.Point(152, 88);
			this.label5.Name = "label5";
			this.label5.Size = new System.Drawing.Size(136, 16);
			this.label5.TabIndex = 19;
			this.label5.Text = "Voltage (mV)";
			this.label5.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
			// 
			// txtTl
			// 
			this.txtTl.Location = new System.Drawing.Point(152, 24);
			this.txtTl.Name = "txtTl";
			this.txtTl.ReadOnly = true;
			this.txtTl.Size = new System.Drawing.Size(136, 20);
			this.txtTl.TabIndex = 20;
			this.txtTl.Text = "";
			// 
			// txtActivation
			// 
			this.txtActivation.Location = new System.Drawing.Point(152, 64);
			this.txtActivation.Name = "txtActivation";
			this.txtActivation.ReadOnly = true;
			this.txtActivation.Size = new System.Drawing.Size(136, 20);
			this.txtActivation.TabIndex = 21;
			this.txtActivation.Text = "";
			// 
			// txtVoltage
			// 
			this.txtVoltage.Location = new System.Drawing.Point(152, 104);
			this.txtVoltage.Name = "txtVoltage";
			this.txtVoltage.ReadOnly = true;
			this.txtVoltage.Size = new System.Drawing.Size(136, 20);
			this.txtVoltage.TabIndex = 22;
			this.txtVoltage.Text = "";
			// 
			// btnClose
			// 
			this.btnClose.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
			this.btnClose.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this.btnClose.Location = new System.Drawing.Point(80, 176);
			this.btnClose.Name = "btnClose";
			this.btnClose.Size = new System.Drawing.Size(64, 24);
			this.btnClose.TabIndex = 23;
			this.btnClose.Text = "Close";
			// 
			// label6
			// 
			this.label6.Location = new System.Drawing.Point(8, 128);
			this.label6.Name = "label6";
			this.label6.Size = new System.Drawing.Size(136, 16);
			this.label6.TabIndex = 27;
			this.label6.Text = "Velocity (m/s)";
			this.label6.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
			// 
			// txtXdot
			// 
			this.txtXdot.Location = new System.Drawing.Point(8, 144);
			this.txtXdot.Name = "txtXdot";
			this.txtXdot.Size = new System.Drawing.Size(136, 20);
			this.txtXdot.TabIndex = 26;
			this.txtXdot.Text = "0";
			// 
			// label7
			// 
			this.label7.Location = new System.Drawing.Point(8, 88);
			this.label7.Name = "label7";
			this.label7.Size = new System.Drawing.Size(136, 16);
			this.label7.TabIndex = 25;
			this.label7.Text = "Tension Derivative (N/s)";
			this.label7.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
			// 
			// txtTdot
			// 
			this.txtTdot.Location = new System.Drawing.Point(8, 104);
			this.txtTdot.Name = "txtTdot";
			this.txtTdot.Size = new System.Drawing.Size(136, 20);
			this.txtTdot.TabIndex = 24;
			this.txtTdot.Text = "0";
			// 
			// CalculateMuscleStimulus
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(320, 206);
			this.Controls.Add(this.label6);
			this.Controls.Add(this.txtXdot);
			this.Controls.Add(this.label7);
			this.Controls.Add(this.txtTdot);
			this.Controls.Add(this.btnClose);
			this.Controls.Add(this.txtVoltage);
			this.Controls.Add(this.txtActivation);
			this.Controls.Add(this.txtTl);
			this.Controls.Add(this.label5);
			this.Controls.Add(this.label4);
			this.Controls.Add(this.label3);
			this.Controls.Add(this.label2);
			this.Controls.Add(this.txtLengthOffset);
			this.Controls.Add(this.label1);
			this.Controls.Add(this.txtTension);
			this.Controls.Add(this.btnCalculate);
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "CalculateMuscleStimulus";
			this.SizeGripStyle = System.Windows.Forms.SizeGripStyle.Hide;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
			this.Text = "Calc Stimulus";
			this.ResumeLayout(false);

		}
		#endregion

		protected float Ftl(float fltLceNorm, float fltTLwidth)
		{
			float fltTLc = (float) Math.Pow(fltTLwidth, 2);
			float fltTl = (float) (-(Math.Pow(fltLceNorm, 2)/fltTLc)  + 1);
			if(fltTl<0) fltTl = 0;
			return fltTl;
		}

		private void btnCalculate_Click(object sender, System.EventArgs e)
		{
			try
			{
				//Lets get the parent structure first.
				if(m_doMuscle == null)
					throw new System.Exception("The muscle is not defined.");

				AnimatTools.DataObjects.Physical.PhysicalStructure doStruct = m_doMuscle.ParentStructure;

				float fltT = float.Parse(txtTension.Text);
				float fltOffset = float.Parse(txtLengthOffset.Text);
				float fltTdot = float.Parse(txtTdot.Text);
				float fltXdot = float.Parse(txtXdot.Text);

				float fltLwidth = (float) m_doMuscle.LengthTension.Lwidth.ActualValue;

				float fltA1 = (float) m_doMuscle.StimTension.XOffset.ActualValue;
				float fltA2 = (float) m_doMuscle.StimTension.Amplitude.ActualValue;
				float fltA3 = (float) m_doMuscle.StimTension.Steepness.ActualValue;

				float fltKse = (float) m_doMuscle.Kse.ActualValue;
				float fltKpe = (float) m_doMuscle.Kpe.ActualValue;
				float fltB = (float) m_doMuscle.B.ActualValue;

				//Calculate tension length percentage
				float fltTl = Ftl(fltOffset, fltLwidth);

				//Calculate A Force needed
				float fltA = fltT - fltKpe*fltOffset + (fltKpe/fltKse)*fltT - fltB*(fltXdot - (fltTdot/fltKse));

				//Increase A to take Tension-length curve into account.
				fltA = fltA/fltTl;

				//Use A to calculate voltage required.
				float fltV = (float) (fltA1 - (1/fltA3)*Math.Log((fltA2-fltA)/fltA));
				
				//Change fltV to millivolts
				fltV *= 1000;	

				txtTl.Text = (fltTl*100).ToString();
				txtActivation.Text = fltA.ToString();
				txtVoltage.Text = fltV.ToString();
			}
			catch(System.Exception ex)
			{Util.DisplayError(ex);}		
		}


	}
}
