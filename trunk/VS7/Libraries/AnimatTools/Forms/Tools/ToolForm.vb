Imports System
Imports System.Drawing
Imports System.Collections
Imports System.ComponentModel
Imports System.Windows.Forms
Imports System.Diagnostics
Imports Crownwood.Magic.Common
Imports Crownwood.Magic.Controls
Imports Crownwood.Magic.Docking
Imports Crownwood.Magic.Menus
Imports AnimatTools.Framework

Namespace Forms.Tools

    Public MustInherit Class ToolForm
        Inherits AnimatTools.Forms.AnimatForm

#Region " Windows Form Designer generated code "

        Public Sub New()
            MyBase.New()

            'This call is required by the Windows Form Designer.
            InitializeComponent()

            'Add any initialization after the InitializeComponent() call

        End Sub

        'Form overrides dispose to clean up the component list.
        Protected Overloads Overrides Sub Dispose(ByVal disposing As Boolean)
            Try
                If disposing Then
                    If Not (components Is Nothing) Then
                        components.Dispose()
                    End If
                End If
                MyBase.Dispose(disposing)
            Catch ex As System.Exception
                Dim i As Integer = 5
            End Try
        End Sub

        'Required by the Windows Form Designer
        Private components As System.ComponentModel.IContainer

        'NOTE: The following procedure is required by the Windows Form Designer
        'It can be modified using the Windows Form Designer.  
        'Do not modify it using the code editor.
        <System.Diagnostics.DebuggerStepThrough()> Private Sub InitializeComponent()
            components = New System.ComponentModel.Container
            Me.Text = "ToolForm"
            'Me.AllowDrop = True
        End Sub

#End Region

#Region " Attributes "

        Protected m_frmViewer As Forms.Tools.Viewer
        Protected m_doToolHolder As AnimatTools.DataObjects.ToolHolder
        Protected m_tnTreeNode As TreeNode

#End Region

#Region " Properties "

        Public MustOverride ReadOnly Property Image() As System.Drawing.Image
        Public MustOverride ReadOnly Property ImageName() As String
        Public MustOverride ReadOnly Property Description() As String

        Public Overrides Property Title() As String
            Get
                Return m_strTitle
            End Get
            Set(ByVal Value As String)
                m_strTitle = Value
                Me.Text = m_strTitle

                If Not m_tnTreeNode Is Nothing Then
                    m_tnTreeNode.Text = m_strTitle
                End If
            End Set
        End Property

        Public Overridable ReadOnly Property DragImage() As System.Drawing.Image
            Get
                Return Me.Image
            End Get
        End Property

        Public Overridable ReadOnly Property DragImageName() As String
            Get
                Return Me.ImageName
            End Get
        End Property

        Public Overridable Property Viewer() As Forms.Tools.Viewer
            Get
                Return m_frmViewer
            End Get
            Set(ByVal Value As Forms.Tools.Viewer)
                m_frmViewer = Value
            End Set
        End Property

        Public Overridable Property ToolHolder() As AnimatTools.DataObjects.ToolHolder
            Get
                Return m_doToolHolder
            End Get
            Set(ByVal Value As AnimatTools.DataObjects.ToolHolder)
                m_doToolHolder = Value
            End Set
        End Property

        Public Overridable Property TreeNode() As TreeNode
            Get
                Return m_tnTreeNode
            End Get
            Set(ByVal Value As TreeNode)
                m_tnTreeNode = Value
            End Set
        End Property

        <Browsable(False)> _
        Public Overrides ReadOnly Property RootForm() As System.Windows.Forms.Form
            Get
                If Not m_frmViewer Is Nothing Then
                    Return m_frmViewer
                Else
                    Return Util.Application
                End If
            End Get
        End Property

        Public MustOverride ReadOnly Property SimToolModuleName() As String
        Public MustOverride ReadOnly Property SimToolClassType() As String

#End Region

#Region " Methods "

        Public Overrides Sub Initialize(ByRef frmApplication As AnimatApplication, _
                                        Optional ByVal frmMdiParent As MdiChild = Nothing, _
                                        Optional ByVal frmParent As AnimatForm = Nothing)
            MyBase.Initialize(frmApplication, frmMdiParent, frmParent)

            If Not frmMdiParent Is Nothing AndAlso TypeOf frmMdiParent Is Forms.Tools.Viewer Then
                m_frmViewer = DirectCast(frmMdiParent, Forms.Tools.Viewer)
            End If
        End Sub

        Public MustOverride Function Clone() As AnimatTools.Forms.Tools.ToolForm

        Public Overridable Sub AfterDropped()
        End Sub

        Protected Sub OnDragItemEntered(ByVal sender As Object, ByVal e As System.Windows.Forms.DragEventArgs)
            Try
                If e.Data.GetDataPresent(GetType(Crownwood.Magic.Controls.PanelIcon)) OrElse _
                   e.Data.GetDataPresent(GetType(Framework.DataDragHelper)) Then
                    e.Effect = DragDropEffects.Copy
                    Me.Cursor = Cursors.Arrow
                Else
                    e.Effect = DragDropEffects.None
                    Me.Cursor = Cursors.Default
                End If

            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try
        End Sub

        Protected Sub OnDragItemLeave(ByVal sender As Object, ByVal e As System.EventArgs)
            Me.Cursor = Cursors.Default
        End Sub

        Public Sub AddTool(ByVal frmOrigTool As Forms.Tools.ToolForm)

            m_frmViewer.ToolCount = m_frmViewer.ToolCount + 1
            Dim strTitle As String = "Tool_" & m_frmViewer.ToolCount

            Dim frmTool As Forms.Tools.ToolForm = frmOrigTool.Clone()

            'If this is the tab filler page then just add it straight to the form.
            If m_frmViewer.TabFiller.TabPages(0).Control Is Me Then
                Util.Application.AddDockingForm(m_frmViewer.DockManager, frmTool, m_frmViewer, strTitle)
            Else
                'If it is not the tab filler then add it specifically to that window.
                Util.Application.AddDockingForm(m_frmViewer.DockManager, frmTool, m_frmViewer, strTitle, strTitle, , , , Me.Content.ParentWindowContent)
            End If

            frmTool.AfterDropped()
            frmTool.CreateTreeView()

        End Sub

        Protected Sub OnDragItemDropped(ByVal sender As Object, ByVal e As System.Windows.Forms.DragEventArgs)

            Try

                'Check if it is a behavioral node, the check if it is a behavioral connector
                If (e.Data.GetDataPresent(GetType(Crownwood.Magic.Controls.PanelIcon))) Then
                    Dim pnlIcon As Crownwood.Magic.Controls.PanelIcon = DirectCast(e.Data.GetData(GetType(Crownwood.Magic.Controls.PanelIcon)), Crownwood.Magic.Controls.PanelIcon)

                    If Not pnlIcon.Data Is Nothing AndAlso TypeOf pnlIcon.Data Is Forms.Tools.ToolForm Then
                        AddTool(DirectCast(pnlIcon.Data, Forms.Tools.ToolForm))
                    End If

                    'Debug.WriteLine("Finishing DragDrop")
                    pnlIcon.DraggingIcon = False
                    m_frmViewer.IsDirty = True

                ElseIf (e.Data.GetDataPresent(GetType(Framework.DataDragHelper))) Then
                    Dim doDrag As Framework.DataDragHelper = DirectCast(e.Data.GetData(GetType(Framework.DataDragHelper)), Framework.DataDragHelper)
                    DroppedDragData(doDrag)
                End If

            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try
        End Sub

        Protected Overridable Sub DroppedDragData(ByVal doDrag As Framework.DataDragHelper)
        End Sub

#Region " TreeView Methods "

        Public Overridable Sub CreateTreeView()

            If Not m_frmViewer Is Nothing AndAlso Not m_frmViewer.HierarchyBar Is Nothing Then
                m_frmViewer.HierarchyBar.ImageManager.AddImage(Me.ImageName, Me.Image)
                m_tnTreeNode = m_frmViewer.HierarchyBar.RootNode.Nodes.Add(Me.Title)
                m_tnTreeNode.ImageIndex = m_frmViewer.HierarchyBar.ImageManager.GetImageIndex(Me.ImageName)
                m_tnTreeNode.SelectedImageIndex = m_frmViewer.HierarchyBar.ImageManager.GetImageIndex(Me.ImageName)
                m_tnTreeNode.Tag = Me
            End If

        End Sub

        Public Overridable Function TreeviewPopupMenu(ByRef tnSelectedNode As TreeNode, ByVal ptPoint As Point) As Boolean

            If tnSelectedNode Is m_tnTreeNode Then
                Return True
            End If

            Return False
        End Function

#End Region

        Public MustOverride Function SaveChartToXml() As String
        Public MustOverride Sub SaveXml(ByRef oXml As Interfaces.StdXml)

        Public Overrides Sub LoadData(ByRef oXml As Interfaces.StdXml)
            MyBase.LoadData(oXml)

            CreateTreeView()
        End Sub

        'Public Overrides Sub SaveData(ByRef oXml As Interfaces.StdXml)
        '    MyBase.SaveData(oXml)

        '    oXml.IntoElem()

        '    oXml.OutOfElem()

        'End Sub

#End Region

    End Class

End Namespace
