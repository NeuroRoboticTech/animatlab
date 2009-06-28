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

Namespace Forms

    Public Class ProjectWorkspace
        Inherits AnimatForm 'System.Windows.Forms.Form

#Region " Windows Form Designer generated code "

        Public Sub New()
            MyBase.New()

            'This call is required by the Windows Form Designer.
            InitializeComponent()

            ''Add any initialization after the InitializeComponent() call
            'InitializeTreeView()
        End Sub

        'Form overrides dispose to clean up the component list.
        Protected Overloads Overrides Sub Dispose(ByVal disposing As Boolean)
            If disposing Then
                If Not (components Is Nothing) Then
                    components.Dispose()
                End If
            End If
            MyBase.Dispose(disposing)
        End Sub

        'Required by the Windows Form Designer
        Private components As System.ComponentModel.IContainer

        'NOTE: The following procedure is required by the Windows Form Designer
        'It can be modified using the Windows Form Designer.  
        'Do not modify it using the code editor.
        Friend WithEvents ctrlTreeView As System.Windows.Forms.TreeView
        Friend WithEvents ctrlSplitter As System.Windows.Forms.Splitter
        Friend WithEvents ctrlProperties As System.Windows.Forms.PropertyGrid
        <System.Diagnostics.DebuggerStepThrough()> Private Sub InitializeComponent()
            Me.ctrlTreeView = New System.Windows.Forms.TreeView
            Me.ctrlSplitter = New System.Windows.Forms.Splitter
            Me.ctrlProperties = New System.Windows.Forms.PropertyGrid
            Me.SuspendLayout()
            '
            'ctrlTreeView
            '
            Me.ctrlTreeView.Dock = System.Windows.Forms.DockStyle.Top
            Me.ctrlTreeView.FullRowSelect = True
            Me.ctrlTreeView.HideSelection = False
            Me.ctrlTreeView.ImageIndex = -1
            Me.ctrlTreeView.LabelEdit = True
            Me.ctrlTreeView.Location = New System.Drawing.Point(0, 0)
            Me.ctrlTreeView.Name = "ctrlTreeView"
            Me.ctrlTreeView.SelectedImageIndex = -1
            Me.ctrlTreeView.Size = New System.Drawing.Size(272, 168)
            Me.ctrlTreeView.Sorted = True
            Me.ctrlTreeView.TabIndex = 0
            '
            'ctrlSplitter
            '
            Me.ctrlSplitter.BackColor = System.Drawing.SystemColors.ControlDark
            Me.ctrlSplitter.Dock = System.Windows.Forms.DockStyle.Top
            Me.ctrlSplitter.Location = New System.Drawing.Point(0, 168)
            Me.ctrlSplitter.MinExtra = 100
            Me.ctrlSplitter.MinSize = 150
            Me.ctrlSplitter.Name = "ctrlSplitter"
            Me.ctrlSplitter.Size = New System.Drawing.Size(272, 8)
            Me.ctrlSplitter.TabIndex = 1
            Me.ctrlSplitter.TabStop = False
            '
            'ctrlProperties
            '
            Me.ctrlProperties.CommandsVisibleIfAvailable = True
            Me.ctrlProperties.Dock = System.Windows.Forms.DockStyle.Fill
            Me.ctrlProperties.LargeButtons = False
            Me.ctrlProperties.LineColor = System.Drawing.SystemColors.ScrollBar
            Me.ctrlProperties.Location = New System.Drawing.Point(0, 176)
            Me.ctrlProperties.Name = "ctrlProperties"
            Me.ctrlProperties.Size = New System.Drawing.Size(272, 198)
            Me.ctrlProperties.TabIndex = 2
            Me.ctrlProperties.Text = "PropertyGrid1"
            Me.ctrlProperties.ViewBackColor = System.Drawing.SystemColors.Window
            Me.ctrlProperties.ViewForeColor = System.Drawing.SystemColors.WindowText
            '
            'ProjectWorkspace
            '
            Me.AutoScaleBaseSize = New System.Drawing.Size(5, 13)
            Me.ClientSize = New System.Drawing.Size(272, 374)
            Me.Controls.Add(Me.ctrlProperties)
            Me.Controls.Add(Me.ctrlSplitter)
            Me.Controls.Add(Me.ctrlTreeView)
            Me.Name = "ProjectWorkspace"
            Me.Text = "Workspace"
            Me.ResumeLayout(False)

        End Sub

#End Region

#Region " Attributes "

        Protected m_imgManager As New AnimatTools.Framework.ImageManager

        Protected m_nodeToolViewers As TreeNode
        Protected m_nodeExternalStimuli As TreeNode

        Protected m_PropertyData As Crownwood.Magic.Controls.PropertyBag
        Protected m_PropertyArray() As Crownwood.Magic.Controls.PropertyBag

#End Region

#Region " Properties "

        Public Overrides ReadOnly Property IconName() As String
            Get
                Return "AnimatTools.ProjectWorkspace.gif"
            End Get
        End Property

        Public ReadOnly Property ImageManager() As AnimatTools.Framework.ImageManager
            Get
                Return m_imgManager
            End Get
        End Property

        Public ReadOnly Property TreeView() As System.Windows.Forms.TreeView
            Get
                Return ctrlTreeView
            End Get
        End Property

        Public ReadOnly Property ToolViewersTreeNode() As TreeNode
            Get
                Return m_nodeToolViewers
            End Get
        End Property

        Public ReadOnly Property StimuliTreeNode() As TreeNode
            Get
                Return m_nodeExternalStimuli
            End Get
        End Property

        Public ReadOnly Property PropertyGrid() As System.Windows.Forms.PropertyGrid
            Get
                Return Me.ctrlProperties
            End Get
        End Property

        Public Property PropertyData() As Crownwood.Magic.Controls.PropertyBag
            Get
                Return m_PropertyData
            End Get
            Set(ByVal Value As Crownwood.Magic.Controls.PropertyBag)

                Try
                    m_PropertyArray = Nothing
                    m_PropertyData = Value

                    If Not m_PropertyData Is Nothing Then
                        Me.ctrlProperties.SelectedObjects = Nothing
                        Me.ctrlProperties.SelectedObject = m_PropertyData
                    Else
                        Me.ctrlProperties.SelectedObjects = Nothing
                        Me.ctrlProperties.SelectedObject = Nothing
                    End If

                Catch ex As System.Exception
                    AnimatTools.Framework.Util.DisplayError(ex)
                End Try

            End Set
        End Property

        Public Property PropertyArray() As Crownwood.Magic.Controls.PropertyBag()
            Get
                Return m_PropertyArray
            End Get
            Set(ByVal Value As Crownwood.Magic.Controls.PropertyBag())

                Try
                    m_PropertyData = Nothing
                    m_PropertyArray = Value

                    If Not m_PropertyArray Is Nothing Then
                        Me.ctrlProperties.SelectedObject = Nothing
                        Me.ctrlProperties.SelectedObjects = m_PropertyArray
                    Else
                        Me.ctrlProperties.SelectedObject = Nothing
                        Me.ctrlProperties.SelectedObjects = Nothing
                    End If

                Catch ex As System.Exception
                    AnimatTools.Framework.Util.DisplayError(ex)
                End Try

            End Set
        End Property

#End Region

#Region " Methods "

        Public Overrides Sub Initialize(ByRef frmApplication As AnimatApplication, Optional ByVal frmMdiParent As MdiChild = Nothing, Optional ByVal frmParent As AnimatForm = Nothing)
            MyBase.Initialize(frmApplication, frmMdiParent, frmParent)

            ctrlTreeView.Nodes.Clear()
            m_imgManager.ImageList.ImageSize = New Size(25, 25)
            ctrlTreeView.ImageList = m_imgManager.ImageList

            If Not m_frmApplication.Simulation Is Nothing Then
                m_frmApplication.Simulation.CreateWorkspaceTreeView(Me)

                Dim myAssembly As System.Reflection.Assembly
                myAssembly = System.Reflection.Assembly.Load("AnimatTools")

                m_imgManager.AddImage(myAssembly, "AnimatTools.Toolbox.gif")
                m_imgManager.AddImage(myAssembly, "AnimatTools.ExternalStimulus.gif")

                m_nodeToolViewers = ctrlTreeView.Nodes.Add("Tool Viewers")
                m_nodeToolViewers.ImageIndex = m_imgManager.GetImageIndex("AnimatTools.Toolbox.gif")
                m_nodeToolViewers.SelectedImageIndex = m_imgManager.GetImageIndex("AnimatTools.Toolbox.gif")

                Dim doTool As DataObjects.ToolHolder
                For Each deEntry As DictionaryEntry In frmApplication.ToolHolders
                    doTool = DirectCast(deEntry.Value, DataObjects.ToolHolder)
                    doTool.CreateWorkspaceTreeView(frmApplication.Simulation, Me)
                Next

                m_nodeExternalStimuli = ctrlTreeView.Nodes.Add("Stimuli")
                m_nodeExternalStimuli.ImageIndex = m_imgManager.GetImageIndex("AnimatTools.ExternalStimulus.gif")
                m_nodeExternalStimuli.SelectedImageIndex = m_imgManager.GetImageIndex("AnimatTools.ExternalStimulus.gif")

                Dim doStimulus As DataObjects.ExternalStimuli.Stimulus
                For Each deEntry As DictionaryEntry In frmApplication.Stimuli
                    doStimulus = DirectCast(deEntry.Value, DataObjects.ExternalStimuli.Stimulus)
                    doStimulus.CreateWorkspaceTreeView(frmApplication.Simulation, Me)
                Next

            End If

            Util.Application.NewToolHolderIndex = Util.ExtractIDCount("DataTool", Util.Application.ToolHolders)

            ctrlTreeView.ExpandAll()
        End Sub

        Public Overrides Sub LoadData(ByRef oXml As AnimatTools.Interfaces.StdXml)
            MyBase.LoadData(oXml)

            oXml.IntoElem()
            Me.ctrlTreeView.Height = oXml.GetChildInt("TreeViewHeight", CInt(Me.Height * 0.6))
            oXml.OutOfElem()

            If Not m_frmApplication.Simulation Is Nothing Then
                m_frmApplication.Simulation.CreateWorkspaceTreeView(Me)
            End If

            ctrlTreeView.ExpandAll()
        End Sub

        Public Overrides Sub SaveData(ByRef oXml As AnimatTools.Interfaces.StdXml)
            MyBase.SaveData(oXml)

            oXml.IntoElem()
            oXml.AddChildElement("TreeViewHeight", Me.ctrlTreeView.Height)
            oXml.OutOfElem()
        End Sub

        Protected Overridable Sub CreateToolViewerPopupMenu(ByVal tnSelected As TreeNode, ByVal ptPoint As Point)

            Dim popup As New PopupMenu

            If Util.Application.ToolPlugins.Count > 0 Then
                Dim mcAddTool As New MenuCommand("Add Data Tool", "AddToolViewer")

                mcAddTool.ImageList = Util.Application.SmallImages.ImageList
                mcAddTool.ImageIndex = Util.Application.SmallImages.GetImageIndex("AnimatTools.AddDataTool.gif")

                Dim mcTool As MenuCommand
                For Each doTool As Forms.Tools.ToolForm In Util.Application.ToolPlugins
                    mcTool = New MenuCommand(doTool.Name, New EventHandler(AddressOf Me.OnAddToolViewer))
                    mcTool.Tag = doTool
                    mcAddTool.MenuCommands.Add(mcTool)
                Next

                popup.MenuCommands.Add(mcAddTool)
            End If

            Dim mcSepExpand As MenuCommand = New MenuCommand("-")
            Dim mcExpandAll As New MenuCommand("Expand All", tnSelected, _
                                              New EventHandler(AddressOf Me.OnExpandAll))
            Dim mcCollapseAll As New MenuCommand("Collapse All", tnSelected, _
                                              New EventHandler(AddressOf Me.OnCollapseAll))

            mcExpandAll.ImageList = Util.Application.SmallImages.ImageList
            mcExpandAll.ImageIndex = Util.Application.SmallImages.GetImageIndex("AnimatTools.Expand.gif")
            mcCollapseAll.ImageList = Util.Application.SmallImages.ImageList
            mcCollapseAll.ImageIndex = Util.Application.SmallImages.GetImageIndex("AnimatTools.Collapse.gif")

            popup.MenuCommands.AddRange(New MenuCommand() {mcSepExpand, mcExpandAll, mcCollapseAll})
            Dim selected As MenuCommand = popup.TrackPopup(ptPoint)

            Return

        End Sub

        Protected Overridable Sub CreateStimuliPopupMenu(ByVal tnSelected As TreeNode, ByVal ptPoint As Point)

            Dim popup As New PopupMenu

            Dim mcExpandAll As New MenuCommand("Expand All", tnSelected, _
                                              New EventHandler(AddressOf Me.OnExpandAll))
            Dim mcCollapseAll As New MenuCommand("Collapse All", tnSelected, _
                                              New EventHandler(AddressOf Me.OnCollapseAll))

            mcExpandAll.ImageList = Util.Application.SmallImages.ImageList
            mcExpandAll.ImageIndex = Util.Application.SmallImages.GetImageIndex("AnimatTools.Expand.gif")
            mcCollapseAll.ImageList = Util.Application.SmallImages.ImageList
            mcCollapseAll.ImageIndex = Util.Application.SmallImages.GetImageIndex("AnimatTools.Collapse.gif")

            popup.MenuCommands.AddRange(New MenuCommand() {mcExpandAll, mcCollapseAll})
            Dim selected As MenuCommand = popup.TrackPopup(ptPoint)

            Return

        End Sub

        Public Overridable Sub SelectDataObject(ByVal doData As AnimatTools.Framework.DataObject, Optional ByVal bThrowError As Boolean = True)

            Dim tnNode As TreeNode = FindTreeNodeForDataObject(doData)

            If tnNode Is Nothing Then
                If bThrowError Then
                    Throw New System.Exception("Unable to find a treenode that has a dataobject '" & doData.ClassName & "' tagged on it.")
                Else
                    Return
                End If
            End If

            ctrlTreeView.SelectedNode = tnNode
        End Sub

        Public Overridable Function FindTreeNodeForDataObject(ByVal doData As AnimatTools.Framework.DataObject) As TreeNode

            Dim tnFound As TreeNode
            For Each tnNode As TreeNode In ctrlTreeView.Nodes
                tnFound = FindTreeNodeForDataObject(tnNode, doData)
                If Not tnFound Is Nothing Then
                    Return tnFound
                End If
            Next

        End Function

        Protected Overridable Function FindTreeNodeForDataObject(ByVal tnParent As TreeNode, ByVal doData As AnimatTools.Framework.DataObject) As TreeNode
            If Not tnParent.Tag Is Nothing AndAlso tnParent.Tag Is doData Then
                Return tnParent
            End If

            Dim tnFound As TreeNode
            For Each tnNode As TreeNode In tnParent.Nodes
                tnFound = FindTreeNodeForDataObject(tnNode, doData)
                If Not tnFound Is Nothing Then
                    Return tnFound
                End If
            Next

            Return Nothing
        End Function

#End Region

#Region " Events "

        Private Sub ctrlTreeView_AfterSelect(ByVal sender As Object, ByVal e As System.Windows.Forms.TreeViewEventArgs) Handles ctrlTreeView.AfterSelect

            Try

                If Not e.Node Is Nothing AndAlso Not e.Node.Tag Is Nothing Then
                    If TypeOf e.Node.Tag Is Framework.DataObject Then
                        Dim doObject As Framework.DataObject = DirectCast(e.Node.Tag, Framework.DataObject)
                        Me.PropertyData = doObject.Properties
                    ElseIf TypeOf e.Node.Tag Is Forms.AnimatForm Then
                        Dim afForm As Forms.AnimatForm = DirectCast(e.Node.Tag, Forms.AnimatForm)
                        Me.PropertyData = afForm.Properties
                    Else
                        Me.PropertyData = Nothing
                    End If
                Else
                    Me.PropertyData = Nothing
                End If

            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try
        End Sub

        Private Sub ctrlTreeView_MouseDown(ByVal sender As Object, ByVal e As System.Windows.Forms.MouseEventArgs) Handles ctrlTreeView.MouseDown

            Try
                If e.Button = MouseButtons.Right Then
                    Dim ctl As Control = CType(sender, System.Windows.Forms.Control)
                    Dim ptPoint As Point = ctl.PointToScreen(New Point(e.X, e.Y))

                    Dim tnSelected As TreeNode = ctrlTreeView.GetNodeAt(e.X, e.Y)
                    If Not tnSelected Is Nothing Then
                        ctrlTreeView.SelectedNode = tnSelected
                    End If

                    If Not m_frmApplication.Simulation Is Nothing Then
                        If m_frmApplication.Simulation.WorkspaceTreeviewPopupMenu(tnSelected, ptPoint) Then
                            Return
                        End If
                    End If

                    Dim doHolder As DataObjects.ToolHolder
                    For Each deEntry As DictionaryEntry In Util.Application.ToolHolders
                        doHolder = DirectCast(deEntry.Value, DataObjects.ToolHolder)
                        If doHolder.WorkspaceTreeviewPopupMenu(tnSelected, ptPoint) Then
                            Return
                        End If
                    Next

                    Dim doStimulus As DataObjects.ExternalStimuli.Stimulus
                    For Each deEntry As DictionaryEntry In Util.Application.Stimuli
                        doStimulus = DirectCast(deEntry.Value, DataObjects.ExternalStimuli.Stimulus)
                        If doStimulus.WorkspaceTreeviewPopupMenu(tnSelected, ptPoint) Then
                            Return
                        End If
                    Next

                    If tnSelected Is m_nodeToolViewers Then
                        CreateToolViewerPopupMenu(tnSelected, ptPoint)
                        Return
                    End If

                    If tnSelected Is m_nodeExternalStimuli Then
                        CreateStimuliPopupMenu(tnSelected, ptPoint)
                        Return
                    End If

                End If
            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try

        End Sub

        Private Sub ctrlTreeView_DoubleClick(ByVal sender As Object, ByVal e As System.EventArgs) Handles ctrlTreeView.DoubleClick

            Try
                If Not m_frmApplication.Simulation Is Nothing AndAlso Not ctrlTreeView.SelectedNode Is Nothing Then
                    If m_frmApplication.Simulation.WorkspaceTreeviewDoubleClick(ctrlTreeView.SelectedNode) Then Return

                    'Now check the tool holders
                    Dim doTool As DataObjects.ToolHolder
                    For Each deEntry As DictionaryEntry In Util.Application.ToolHolders
                        doTool = DirectCast(deEntry.Value, DataObjects.ToolHolder)
                        If doTool.WorkspaceTreeviewDoubleClick(ctrlTreeView.SelectedNode) Then Return
                    Next

                End If

            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try

        End Sub

        Private Sub ctrlTreeView_BeforeLabelEdit(ByVal sender As Object, ByVal e As System.Windows.Forms.NodeLabelEditEventArgs) Handles ctrlTreeView.BeforeLabelEdit
            If Not e.Node.Tag Is Nothing AndAlso _
               (TypeOf e.Node.Tag Is DataObjects.ToolHolder OrElse _
               TypeOf e.Node.Tag Is DataObjects.ExternalStimuli.Stimulus OrElse _
               (TypeOf e.Node.Tag Is DataObjects.Physical.PhysicalStructure AndAlso _
               Not (TypeOf e.Node.Tag Is DataObjects.Physical.GroundSurface OrElse TypeOf e.Node.Tag Is DataObjects.Physical.WaterSurface))) Then
                e.CancelEdit = False
            Else
                e.CancelEdit = True
            End If
        End Sub

        Private Sub ctrlTreeView_AfterLabelEdit(ByVal sender As Object, ByVal e As System.Windows.Forms.NodeLabelEditEventArgs) Handles ctrlTreeView.AfterLabelEdit
            Try
                If Not e.Node.Tag Is Nothing Then
                    If Not e.Label Is Nothing AndAlso e.Label.Trim.Length > 0 Then
                        If TypeOf e.Node.Tag Is DataObjects.ToolHolder Then
                            Dim frmViewer As DataObjects.ToolHolder = DirectCast(e.Node.Tag, DataObjects.ToolHolder)
                            frmViewer.Name = e.Label.Trim
                        ElseIf TypeOf e.Node.Tag Is DataObjects.ExternalStimuli.Stimulus Then
                            Dim doStim As DataObjects.ExternalStimuli.Stimulus = DirectCast(e.Node.Tag, DataObjects.ExternalStimuli.Stimulus)
                            doStim.Name = e.Label.Trim
                        ElseIf TypeOf e.Node.Tag Is DataObjects.Physical.PhysicalStructure Then
                            Dim doStruct As DataObjects.Physical.PhysicalStructure = DirectCast(e.Node.Tag, DataObjects.Physical.PhysicalStructure)
                            doStruct.Name = e.Label.Trim
                        Else
                            e.CancelEdit = True
                        End If
                    Else
                        e.CancelEdit = True
                    End If
                End If
            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try
        End Sub

        Protected Overridable Sub OnAddToolViewer(ByVal sender As Object, ByVal e As System.EventArgs)

            Try
                If TypeOf sender Is MenuCommand Then
                    Dim mcCommand As MenuCommand = DirectCast(sender, MenuCommand)

                    If Not mcCommand.Tag Is Nothing AndAlso TypeOf mcCommand.Tag Is Tools.ToolForm Then
                        Dim doTool As Tools.ToolForm = DirectCast(mcCommand.Tag, Tools.ToolForm)
                        Util.Application.addnewTool(doTool)
                    End If
                End If

            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try

        End Sub

        Protected Overrides Sub OnResize(ByVal e As System.EventArgs)

            Try
                MyBase.OnResize(e)

                If Not Util.DisableDirtyFlags Then
                    If Not m_ctContent Is Nothing Then
                        If Not m_ctContent.AutoHidePanel Is Nothing Then
                            If Me.ctrlTreeView.Height > m_ctContent.AutoHidePanel.Size.Height Then
                                Me.ctrlTreeView.Height = CInt(m_ctContent.AutoHidePanel.Size.Height / 2)
                            End If
                        Else
                            If Me.ctrlTreeView.Height > Me.Height Then
                                Me.ctrlTreeView.Height = CInt(Me.Height / 2)
                            End If
                        End If
                    End If
                End If

            Catch ex As System.Exception
            End Try

        End Sub

#End Region

    End Class

End Namespace


