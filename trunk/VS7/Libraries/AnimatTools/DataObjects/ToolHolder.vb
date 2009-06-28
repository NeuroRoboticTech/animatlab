Imports System
Imports System.Drawing
Imports System.Collections
Imports System.ComponentModel
Imports System.Windows.Forms
Imports System.Diagnostics
Imports System.IO
Imports System.Xml
Imports Crownwood.Magic.Common
Imports Crownwood.Magic.Controls
Imports Crownwood.Magic.Docking
Imports Crownwood.Magic.Menus
Imports AnimatTools.Framework

Namespace DataObjects

    Public Class ToolHolder
        Inherits AnimatTools.DataObjects.DragObject

#Region " Attributes "

        Protected m_strOriginalName As String = ""
        Protected m_nodeViewer As TreeNode

        Protected m_strBaseAssemblyFile As String = ""
        Protected m_strBaseClassName As String = ""

#End Region

#Region " Properties "

        Public Overrides Property Name() As String
            Get
                Return m_strName
            End Get
            Set(ByVal Value As String)
                If Value.Trim.Length = 0 Then
                    Throw New System.Exception("You can not set the name for the tool viewer to blank.")
                End If

                If Not Util.Application.ToolHolders.FindName(Value.Trim, False) Is Nothing Then
                    Throw New System.Exception("There is already another tool viewer with the name '" & Value.Trim & "'.")
                End If

                'Dim strOldName As String = m_strName

                'Now add it back in the list with the new name
                m_strName = Value.Trim

                'RenameFiles(strOldName)

                Dim frmViewer As Forms.Tools.Viewer = FindViewerWindow()
                If Not frmViewer Is Nothing Then
                    frmViewer.Title = m_strName
                End If

                Me.CreateWorkspaceTreeView(Util.Simulation, Util.Application.ProjectWorkspace)
                If Not m_nodeViewer Is Nothing Then
                    Util.Application.ProjectWorkspace.TreeView.SelectedNode = m_nodeViewer
                End If

                'Util.Application.SaveProject(Util.Application.ProjectFile)

            End Set
        End Property

        Public Overridable Property BaseAssemblyFile() As String
            Get
                Return m_strBaseAssemblyFile
            End Get
            Set(ByVal Value As String)
                m_strBaseAssemblyFile = Value
            End Set
        End Property

        Public Overridable Property BaseClassName() As String
            Get
                Return m_strBaseClassName
            End Get
            Set(ByVal Value As String)
                m_strBaseClassName = Value
            End Set
        End Property

        Public Overridable ReadOnly Property ToolViewerFilename() As String
            Get
                Return Me.Name & ".atvf"
            End Get
        End Property

        Public Overridable ReadOnly Property ViewerTreeNode() As TreeNode
            Get
                Return m_nodeViewer
            End Get
        End Property

#Region " DragObject Properties "

        Public Overrides ReadOnly Property CanBeCharted() As Boolean
            Get
                Return False
            End Get
        End Property

        Public Overrides ReadOnly Property DataColumnClassType() As String
            Get
                Return "ToolHolder"
            End Get
        End Property

        Public Overrides ReadOnly Property DataColumnModuleName() As String
            Get
                Return "ToolHolder"
            End Get
        End Property

        Public Overrides Property DragImage() As System.Drawing.Image
            Get

            End Get
            Set(ByVal Value As System.Drawing.Image)

            End Set
        End Property

        Public Overrides ReadOnly Property DragImageName() As String
            Get
                Return ""
            End Get
        End Property

        Public Overrides Property Image() As System.Drawing.Image
            Get

            End Get
            Set(ByVal Value As System.Drawing.Image)

            End Set
        End Property

        Public Overrides ReadOnly Property ImageName() As String
            Get
                Return ""
            End Get
        End Property

        Public Overrides Property ItemName() As String
            Get
                Return Me.Name
            End Get
            Set(ByVal Value As String)
                Me.Name = Value
            End Set
        End Property

        Public Overrides ReadOnly Property StructureID() As String
            Get

            End Get
        End Property

#End Region

#End Region

#Region " Methods "

        Public Sub New(ByVal doParent As Framework.DataObject)
            MyBase.New(doParent)
            m_strID = System.Guid.NewGuid().ToString()
        End Sub

        Protected Overrides Sub BuildProperties()

            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Name", m_strName.GetType(), "Name", _
                                        "Tool Viewer", "Sets the name of this Tool Viewer.", m_strName))

            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Filename", GetType(String), "ToolViewerFilename", _
                                        "Tool Viewer", "The filename for this viewer.", Me.ToolViewerFilename, True))

        End Sub

        Protected Overridable Function FindViewerWindow() As Forms.Tools.Viewer
            'Now lets see if there is a tool window open with this title.
            Dim frmTool As Forms.Tools.Viewer
            For Each oChild As Form In Util.Application.MdiChildren
                If TypeOf oChild Is Forms.Tools.Viewer Then
                    frmTool = DirectCast(oChild, Forms.Tools.Viewer)
                    If frmTool.ToolHolder Is Me Then
                        Return frmTool
                    End If
                End If
            Next
        End Function

        Public Overridable Sub CreateWorkspaceTreeView(ByVal dsSim As AnimatTools.DataObjects.Simulation, _
                                                       ByVal frmWorkspace As Forms.ProjectWorkspace)

            If Not m_nodeViewer Is Nothing Then
                m_nodeViewer.Remove()
            End If

            m_nodeViewer = frmWorkspace.ToolViewersTreeNode.Nodes.Add(Me.Name)

            Dim myAssembly As System.Reflection.Assembly
            myAssembly = System.Reflection.Assembly.Load("AnimatTools")
            frmWorkspace.ImageManager.AddImage(myAssembly, "AnimatTools.Wrench.gif")

            m_nodeViewer.ImageIndex = frmWorkspace.ImageManager.GetImageIndex("AnimatTools.Wrench.gif")
            m_nodeViewer.SelectedImageIndex = frmWorkspace.ImageManager.GetImageIndex("AnimatTools.Wrench.gif")
            m_nodeViewer.Tag = Me

        End Sub

        Public Overridable Function WorkspaceTreeviewPopupMenu(ByRef tnSelectedNode As TreeNode, ByVal ptPoint As Point) As Boolean

            If tnSelectedNode Is m_nodeViewer Then
                ' Create the menu items
                Dim mcDelete As New MenuCommand("Delete Viewer ", "DeleteViewer", Util.Application.SmallImages.ImageList, _
                                                  Util.Application.SmallImages.GetImageIndex("AnimatTools.Delete.gif"), _
                                                  New EventHandler(AddressOf Me.OnDeleteViewer))

                ' Create the popup menu object
                Dim popup As New PopupMenu
                popup.MenuCommands.Add(mcDelete)

                ' Show it!
                Dim selected As MenuCommand = popup.TrackPopup(ptPoint)

                Return True
            End If

            Return False
        End Function

        Public Overridable Function WorkspaceTreeviewDoubleClick(ByRef tnSelectedNode As TreeNode) As Boolean

            If tnSelectedNode Is Me.ViewerTreeNode Then
                Util.Application.DisplayToolViewer(Me)
                Return True
            End If

            Return False
        End Function

        Public Overridable Sub RenameFiles(ByVal strOriginalName As String)
            Dim strExtension As String, strNewFile As String

            If Util.Application.ProjectPath.Trim.Length > 0 AndAlso strOriginalName.Trim.Length > 0 Then
                Dim di As DirectoryInfo = New DirectoryInfo(Util.Application.ProjectPath)
                Dim fiFiles As FileInfo() = di.GetFiles(strOriginalName & ".*")

                For Each fiFile As FileInfo In fiFiles
                    strExtension = Util.GetFileExtension(fiFile.Name)
                    strNewFile = Util.GetFilePath(Util.Application.ProjectPath, (Me.Name & "." & strExtension))

                    fiFile.MoveTo(strNewFile)
                Next
            End If

        End Sub

        Public Overridable Sub RemoveFiles(ByVal strName As String)

            If Util.Application.ProjectPath.Trim.Length > 0 Then
                Dim di As DirectoryInfo = New DirectoryInfo(Util.Application.ProjectPath)
                Dim fiFiles As FileInfo() = di.GetFiles(strName & ".*")

                For Each fiFile As FileInfo In fiFiles
                    fiFile.Delete()
                Next
            End If

        End Sub

        Public Overrides Sub LoadData(ByRef oXml As Interfaces.StdXml)

            oXml.IntoElem()
            m_strName = oXml.GetChildString("Name")
            m_strOriginalName = m_strName
            m_strID = oXml.GetChildString("ID", m_strName)
            m_strBaseAssemblyFile = oXml.GetChildString("BaseAssemblyFile")
            m_strBaseClassName = oXml.GetChildString("BaseClassName")
            oXml.OutOfElem()

        End Sub

        Public Overrides Sub SaveData(ByRef oXml As Interfaces.StdXml)

            oXml.AddChildElement("ToolHolder")
            oXml.IntoElem()
            oXml.AddChildElement("ID", m_strID)
            oXml.AddChildElement("Name", m_strName)
            oXml.AddChildElement("BaseAssemblyFile", m_strBaseAssemblyFile)
            oXml.AddChildElement("BaseClassName", m_strBaseClassName)
            oXml.OutOfElem()

            If m_strName <> m_strOriginalName Then
                RemoveFiles(m_strOriginalName)
                m_strOriginalName = m_strName
            End If
        End Sub

        Public Overrides Function FindDragObject(ByVal strStructureName As String, ByVal strDataItemID As String, Optional ByVal bThrowError As Boolean = True) As DragObject

        End Function

        Public Overrides Sub SaveDataColumnToXml(ByRef oXml As Interfaces.StdXml)

        End Sub

        Public Overrides Function Clone(ByVal doParent As AnimatTools.Framework.DataObject, ByVal bCutData As Boolean, _
                                        ByVal doRoot As AnimatTools.Framework.DataObject) As AnimatTools.Framework.DataObject
            Dim doHolder As ToolHolder = New ToolHolder(doParent)

            doHolder.m_strName = m_strName
            doHolder.m_strOriginalName = m_strOriginalName
            doHolder.m_nodeViewer = m_nodeViewer
            doHolder.m_strBaseAssemblyFile = m_strBaseAssemblyFile
            doHolder.m_strBaseClassName = m_strBaseClassName

            If Not doRoot Is Nothing AndAlso doRoot Is Me Then doHolder.AfterClone(Me, bCutData, doRoot, doHolder)
            Return doHolder
        End Function

        Public Overridable Sub Delete()
            Me.OnDeleteViewer(Me, New System.EventArgs)
        End Sub

#End Region

#Region " Events "

        Protected Sub OnDeleteViewer(ByVal sender As Object, ByVal e As System.EventArgs)

            Try
                If MessageBox.Show("Are you certain that you want to permanently delete this " & _
                                    "tool viewer?", "Delete Viewer", MessageBoxButtons.YesNo) = DialogResult.Yes Then
                    Dim frmViewer As AnimatTools.Forms.Tools.Viewer = FindViewerWindow()

                    If Not frmViewer Is Nothing Then
                        frmViewer.IsDirty = False
                        frmViewer.Close()
                    End If

                    If Not m_nodeViewer Is Nothing Then m_nodeViewer.Remove()
                    Util.Application.ToolHolders.Remove(Me.ID)
                    Me.RemoveFiles(Me.Name)

                    Util.Application.SaveProject(Util.Application.ProjectFile)
                End If

            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try

        End Sub

#End Region


    End Class

End Namespace
