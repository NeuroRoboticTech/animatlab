using System.Windows.Forms;
using System.Text.RegularExpressions;
using System.ComponentModel;
using System.Drawing.Design;
using System.Windows.Forms.Design;
using AnimatTools.Framework;

namespace VortexAnimatTools.TypeHelpers
{
	/// <summary>
	/// Summary description for MuscleAttachmentsTypeEditor.
	/// </summary>
	public class MuscleAttachmentsTypeEditor : System.Drawing.Design.UITypeEditor
	{
		public MuscleAttachmentsTypeEditor()
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

					if(pbBag.Tag != null && pbBag.Tag is VortexAnimatTools.DataObjects.Physical.RigidBodies.MuscleBase)
					{
						VortexAnimatTools.DataObjects.Physical.RigidBodies.MuscleBase doMuscle = (VortexAnimatTools.DataObjects.Physical.RigidBodies.MuscleBase) pbBag.Tag;

						VortexAnimatTools.Forms.BodyPlan.EditMuscleAttachments frmAttachments = new VortexAnimatTools.Forms.BodyPlan.EditMuscleAttachments();

						frmAttachments.Muscle = doMuscle;
						frmAttachments.ShowDialog();
					}
				}

			}
			catch(System.Exception ex)
			{Util.DisplayError(ex);}
			
			return null;																								 
		}

	}
}
