using System.Windows.Forms;
using System.Text.RegularExpressions;
using System.ComponentModel;
using System.Drawing.Design;
using System.Windows.Forms.Design;
using AnimatTools.Framework;

namespace VortexAnimatTools.TypeHelpers
{
	/// <summary>
	/// Summary description for CalcMuscleEditor.
	/// </summary>
	public class CalcMuscleEditor : System.Drawing.Design.UITypeEditor
	{
		public CalcMuscleEditor()
		{
		}

		[System.Security.Permissions.PermissionSet(System.Security.Permissions.SecurityAction.Demand, Name="FullTrust")] 
		public override System.Drawing.Design.UITypeEditorEditStyle GetEditStyle(System.ComponentModel.ITypeDescriptorContext context)
		{
			return UITypeEditorEditStyle.Modal;
		}

		[System.Security.Permissions.PermissionSet(System.Security.Permissions.SecurityAction.Demand, Name="FullTrust")]
		public override object EditValue(System.ComponentModel.ITypeDescriptorContext context, System.IServiceProvider provider, object value)
		{
			try
			{
				if(context != null && context.Instance != null && context.Instance != null && provider != null && context.Instance is Crownwood.Magic.Controls.PropertyBag)
				{
					Crownwood.Magic.Controls.PropertyBag pbBag = (Crownwood.Magic.Controls.PropertyBag) context.Instance;

					if(pbBag.Tag != null && pbBag.Tag is VortexAnimatTools.DataObjects.Physical.RigidBodies.LinearHillMuscle)
					{
						VortexAnimatTools.DataObjects.Physical.RigidBodies.LinearHillMuscle doMuscle = (VortexAnimatTools.DataObjects.Physical.RigidBodies.LinearHillMuscle) pbBag.Tag;

						VortexAnimatTools.Forms.BodyPlan.CalculateMuscleStimulus frmCalc = new VortexAnimatTools.Forms.BodyPlan.CalculateMuscleStimulus();

						frmCalc.Muscle = doMuscle;
						frmCalc.ShowDialog();
					}
				}

			}
			catch(System.Exception ex)
			{Util.DisplayError(ex);}
			
			return null;																								 
		}

	}
}
