using System;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using Crownwood.Magic.Menus;

namespace Crownwood.Magic.Toolbars
{
	/// <summary>
	/// Summary description for ToolbarManager.
	/// </summary>

	[ProvideProperty( "ButtonManager", typeof(Component)) ]
	public class ButtonManager : Component, IExtenderProvider 
	{
		ToolBar   m_ToolBar    = null;
		Hashtable m_Dictionary = new Hashtable( );

		///<summary>
		/// Set each ToolBarButton to a specific MenuItem
		///</summary>

		public void SetButtonMenuItem( Component pComponent, MenuCommand pMenuItem )
		{
			if( ! m_Dictionary.Contains( pComponent ))
			{
				m_Dictionary.Add( pComponent, pMenuItem );
        
				if( m_ToolBar == null )
				{
					ToolBarButton pToolBarButton = pComponent as ToolBarButton;

					if( pToolBarButton != null )
					{
						m_ToolBar = pToolBarButton.Parent as ToolBar;

						if( m_ToolBar != null )
							m_ToolBar.ButtonClick += new ToolBarButtonClickEventHandler( Handle_ToolbarButtonClick );
					}
				}
			}
			else 
			{
				m_Dictionary[ pComponent ] = pMenuItem;
			}
		}

		///<summary>
		/// Return the MenuItem that is assigned to each ToolBarButton
		///</summary>

		public MenuCommand GetMenuItem( Component pComponent )
		{
			if( m_Dictionary.Contains( pComponent ))
				return (MenuCommand) m_Dictionary[ pComponent ];

			return null;
		}

		///<summary>
		/// Definition of the components that will be extended
		///</summary>

		public bool CanExtend( object pComponent )
		{
			return( pComponent is ToolBarButton );
		}

		private void Handle_ToolbarButtonClick( object pComponent, ToolBarButtonClickEventArgs e )
		{
			if( m_Dictionary.Contains( e.Button ))
			{
				MenuCommand pMenuItem = (MenuCommand) m_Dictionary[ e.Button ];

				if( pMenuItem != null )
					pMenuItem.PerformClick( );
			}
		}
	}

}
