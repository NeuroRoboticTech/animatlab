// *****************************************************************************
// 
//  (c) Crownwood Consulting Limited 2002 
//  All rights reserved. The software and associated documentation 
//  supplied hereunder are the proprietary information of Crownwood Consulting 
//	Limited, Haxey, North Lincolnshire, England and are supplied subject to 
//	licence terms.
// 
//  Magic Version 1.7 	www.dotnetmagic.com
// *****************************************************************************

using System;
using System.Drawing;
using System.Windows.Forms;
using System.ComponentModel;
using Crownwood.Magic.Common;
using Crownwood.Magic.Controls;
using Crownwood.Magic.Collections;

namespace Crownwood.Magic.Docking
{
    [ToolboxItem(false)]
    public class WindowContent : Window
    {
        // Instance fields
        protected ContentCollection _contents;
        protected VisualStyle _style;

        public WindowContent(DockingManager manager, VisualStyle vs)
            : base(manager)
        {
            // Remember state
            _style = vs;
        
            // Create collection of window details
            _contents = new ContentCollection();

            // We want notification when contents are added/removed/cleared
            _contents.Clearing += new CollectionClear(OnContentsClearing);
            _contents.Inserted += new CollectionChange(OnContentInserted);
            _contents.Removing += new CollectionChange(OnContentRemoving);
            _contents.Removed += new CollectionChange(OnContentRemoved);
        }

        public ContentCollection Contents
        {
            get { return _contents; }
			
            set
            {
                _contents.Clear();
                _contents = value;
            }
        }

		public virtual void BringContentToFront(Content c) {}

        protected virtual void OnContentsClearing()
        {
					try
					{
            foreach(Content c in _contents)
            {
                // Inform content of new parent content window
                c.ParentWindowContent = null;

                // Unhook from property change notification
                c.PropertyChanged -= new Content.PropChangeHandler(OnContentChanged);
            }

            // Should we kill ourself?
            if (this.AutoDispose)
                Suicide();
					}
					catch(System.Exception ex)
					{MessageBox.Show(ex.Message);}
				}

        protected virtual void OnContentInserted(int index, object value)
        {
					try
					{
            Content content = value as Content;

            // Is this the first Content added?
            if (_contents.Count == 1)
            {
                // Use size of the Content to determine our size
                this.Size = content.DisplaySize;
            }

            // Inform content where it now resides
            content.ParentWindowContent = this;

            // Monitor changes in Content properties
            content.PropertyChanged += new Content.PropChangeHandler(OnContentChanged);
					}
					catch(System.Exception ex)
					{MessageBox.Show(ex.Message);}
				}

        protected virtual void OnContentRemoving(int index, object value)
        {
					try
					{
            Content content = value as Content;

            // Inform content of new parent content window
            content.ParentWindowContent = null;

            // Unhook from monitoring changes in Content properties
            content.PropertyChanged -= new Content.PropChangeHandler(OnContentChanged);
					}
					catch(System.Exception ex)
					{MessageBox.Show(ex.Message);}
				}

        protected virtual void OnContentRemoved(int index, object value)
        {
					try
					{
            // Removed the last entry?
            if (_contents.Count == 0)
            {
                // Should we kill ourself?
                if (this.AutoDispose)
                    Suicide();
            }
					}
					catch(System.Exception ex)
					{MessageBox.Show(ex.Message);}
				}

        protected virtual void OnContentChanged(Content obj, Content.Property prop) {}

        protected void Suicide()
        {
            // Are we inside a Zone object?
            if (this.ParentZone != null)
                this.ParentZone.Windows.Remove(this);

            // Remover monitoring of events
            _contents.Clearing -= new CollectionClear(OnContentsClearing);
            _contents.Inserted -= new CollectionChange(OnContentInserted);
            _contents.Removing -= new CollectionChange(OnContentRemoving);
            _contents.Removed -= new CollectionChange(OnContentRemoved);

						//DWC: I found that if you comment this line out so that it does not explictly call dispose it keeps a lot of
					  //eplosions from happening later on. The problem is that if AutoHide a docked form and then do a hide content
					  //on it then this dispose kills the form itself. When you go back to do a ShowContent the form is now already
					  //disposed and it blows up with an exception. I believe this is also related to the earlier problem involving
					  //autohidden windows and saving the configuration.
            //this.Dispose();
        }
    }
}