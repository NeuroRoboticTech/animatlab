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

namespace VortexAnimatTools.Collections
{
	/// <summary>
	/// Summary description for MuscleAttachments.
	/// </summary>
	public class MuscleAttachments : AnimatTools.Collections.AnimatCollectionBase
	{
		public MuscleAttachments(AnimatTools.Framework.DataObject doParent) : base(doParent)
		{
		}

		public VortexAnimatTools.DataObjects.Physical.RigidBodies.MuscleAttachment this[ int index ]  
		{
			get  
			{
				return( (VortexAnimatTools.DataObjects.Physical.RigidBodies.MuscleAttachment) List[index] );
			}
			set  
			{
				List[index] = value;
			}
		}

		public Crownwood.Magic.Controls.PropertyBag Properties
		{
			get
			{
				Crownwood.Magic.Controls.PropertyTable ptTable = new Crownwood.Magic.Controls.PropertyTable();

				ptTable.Tag = this;
				ptTable.GetValue+=new Crownwood.Magic.Controls.PropertySpecEventHandler(this.OnGetPropValue);
				
				int iIndex=0;
				foreach(VortexAnimatTools.DataObjects.Physical.RigidBodies.MuscleAttachment doAttach in this)
				{
					iIndex++;

					ptTable.Properties.Add(new Crownwood.Magic.Controls.PropertySpec("Attachment " + iIndex, typeof(string), (iIndex-1).ToString(), 
																	"Muscle Attachments", "One of the muscle attachment points.", 
																	doAttach.Name, true));
				}

				return ptTable;
			}
		}

		public int Add( VortexAnimatTools.DataObjects.Physical.RigidBodies.MuscleAttachment value )  
		{
			return( List.Add( value ) );
		}

		public int IndexOf( VortexAnimatTools.DataObjects.Physical.RigidBodies.MuscleAttachment value )  
		{
			return( List.IndexOf( value ) );
		}

		public void Insert( int index, VortexAnimatTools.DataObjects.Physical.RigidBodies.MuscleAttachment value )  
		{
			List.Insert( index, value );
		}

		public void Remove( VortexAnimatTools.DataObjects.Physical.RigidBodies.MuscleAttachment value )  
		{
			List.Remove( value );
		}

		public bool Contains( VortexAnimatTools.DataObjects.Physical.RigidBodies.MuscleAttachment value )  
		{
			// If value is not of type VortexAnimatTools.DataObjects.Physical.RigidBodies.MuscleAttachment, this will return false.
			return( List.Contains( value ) );
		}

		protected override void OnInsert( int index, Object value )  
		{
			if ( !Util.IsTypeOf(value.GetType(), typeof(VortexAnimatTools.DataObjects.Physical.RigidBodies.MuscleAttachment), false) )
				throw new ArgumentException( "value must be of type VortexAnimatTools.DataObjects.Physical.RigidBodies.MuscleAttachment.", "value" );
		}

		protected override void OnRemove( int index, Object value )  
		{
			if ( !Util.IsTypeOf(value.GetType(), typeof(VortexAnimatTools.DataObjects.Physical.RigidBodies.MuscleAttachment), false) )
				throw new ArgumentException( "value must be of type VortexAnimatTools.DataObjects.Physical.RigidBodies.MuscleAttachment.", "value" );
		}

		protected override void OnSet( int index, Object oldValue, Object newValue )  
		{
			if ( !Util.IsTypeOf(newValue.GetType(), typeof(VortexAnimatTools.DataObjects.Physical.RigidBodies.MuscleAttachment), false) )
				throw new ArgumentException( "newValue must be of type VortexAnimatTools.DataObjects.Physical.RigidBodies.MuscleAttachment.", "newValue" );
		}

		protected override void OnValidate( Object value )  
		{
			if ( !Util.IsTypeOf(value.GetType(), typeof(VortexAnimatTools.DataObjects.Physical.RigidBodies.MuscleAttachment), false) )
				throw new ArgumentException( "value must be of type VortexAnimatTools.DataObjects.Physical.RigidBodies.MuscleAttachment." );
		}

		protected void OnGetPropValue(object sender, PropertySpecEventArgs e)
		{
			try
			{
				int iIndex = int.Parse(e.Property.PropertyName);
				
				VortexAnimatTools.DataObjects.Physical.RigidBodies.MuscleAttachment doAttach = (VortexAnimatTools.DataObjects.Physical.RigidBodies.MuscleAttachment) List[iIndex];
				e.Value = doAttach.Name;
			}
			catch(Exception ex)
			{}
		}

		public override AnimatTools.Collections.AnimatCollectionBase Copy()
		{
			MuscleAttachments aryNewList = new MuscleAttachments(m_doParent);
			aryNewList.CopyInternal(this);
			return aryNewList;
		}

		public override AnimatTools.Collections.AnimatCollectionBase Clone(AnimatTools.Framework.DataObject doParent, bool bCutData, AnimatTools.Framework.DataObject doRoot)
		{
			MuscleAttachments aryNewList = new MuscleAttachments(doParent);
			aryNewList.CloneInternal(this, doParent, bCutData, doRoot);
			return aryNewList;
		}

	}
}
