using System;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using Crownwood.Magic.Menus;

namespace Crownwood.Magic.Toolbars
{
	/// <summary>
	/// Summary description for Toolbar.
	/// </summary>
	public class ToolbarControl : System.Windows.Forms.ToolBar
	{
		protected ButtonManager m_btnManager; 

		public ToolbarControl()
		{
			m_btnManager = new ButtonManager();
		}

		public ButtonManager ButtonManager
		{
			get { return m_btnManager; }
		}	
	}
}
