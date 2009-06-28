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
Imports AnimatTools
Imports AnimatTools.Framework
Imports AnimatTools.DataObjects
Imports System.Drawing.Imaging

Namespace Forms.BodyPlan

    Public MustInherit Class BodyView
        Inherits AnimatForm

#Region " Windows Form Designer generated code "

        Public Sub New()
            MyBase.New()

            'This call is required by the Windows Form Designer.
            InitializeComponent()
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
        <System.Diagnostics.DebuggerStepThrough()> Private Sub InitializeComponent()
            components = New System.ComponentModel.Container
            Me.Title = "Body View"
        End Sub

#End Region

#Region " Attributes "

        Protected m_beEditor As AnimatTools.Forms.BodyPlan.Editor
        Protected m_clBackColor As Color = Color.Black

#End Region

#Region " Properties "

        'Public Overrides ReadOnly Property DockingHideTabsMode() As Crownwood.Magic.Controls.TabControl.HideTabsModes
        '    Get
        '        Return Crownwood.Magic.Controls.TabControl.HideTabsModes.HideAlways
        '    End Get
        'End Property

        Public Overridable Property Editor() As Forms.BodyPlan.Editor
            Get
                Return m_beEditor
            End Get
            Set(ByVal Value As Forms.BodyPlan.Editor)
                m_beEditor = Value
            End Set
        End Property

        Public MustOverride ReadOnly Property CameraPosition() As Vec3d

        Public Overridable Property ViewBackColor() As Color
            Get
                Return m_clBackColor
            End Get
            Set(ByVal Value As Color)
                m_clBackColor = Value
            End Set
        End Property

#End Region

#Region " Methods "

        Public Overrides Sub Initialize(ByRef frmApplication As AnimatApplication, _
                                        Optional ByVal frmMdiParent As MdiChild = Nothing, _
                                        Optional ByVal frmParent As AnimatForm = Nothing)
            Try

                MyBase.Initialize(frmApplication, frmMdiParent, frmParent)

                m_beEditor = DirectCast(frmMdiParent, AnimatTools.Forms.Bodyplan.Editor)

            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try

        End Sub

        Public Overrides Sub OnAfterMdiParentInitialized()
            If Not m_beEditor Is Nothing AndAlso Not m_beEditor.CommandBar Is Nothing Then
                AddHandler m_beEditor.CommandBar.CommandModeChanged, AddressOf Me.OnCommandModeChanged
            End If
        End Sub

        Protected Overridable Sub OnCommandModeChanged(ByVal eNewMode As Forms.BodyPlan.Command.enumCommandMode)

        End Sub

        Protected Overridable Sub CreatePopupMenu(ByVal ptScreen As Point)

            ' Create the popup menu object
            Dim popup As New PopupMenu

            ' Create the menu items
            Dim mcSelectBodies As New MenuCommand("Select Bodies", "SelectBodies", System.Windows.Forms.Shortcut.CtrlB, New EventHandler(AddressOf Me.OnSelectBodies))
            Dim mcSelectJoints As New MenuCommand("Select Joints", "SelectJoints", System.Windows.Forms.Shortcut.CtrlJ, New EventHandler(AddressOf Me.OnSelectJoints))
            Dim mcReceptiveFields As New MenuCommand("Select Receptive Fields", "SelectReceptiveFields", System.Windows.Forms.Shortcut.CtrlR, New EventHandler(AddressOf Me.OnSelectReceptiveFields))
            Dim mcAddBody As New MenuCommand("Add Body", "AddBody", System.Windows.Forms.Shortcut.CtrlA, New EventHandler(AddressOf Me.OnAddBody))
            Dim mcSepEditStart1 As MenuCommand = New MenuCommand("-")

            If Me.Editor.CommandBar.CommandMode = Command.enumCommandMode.SelectBodies Then
                mcSelectBodies.Checked = True
            ElseIf Me.Editor.CommandBar.CommandMode = Command.enumCommandMode.SelectJoints Then
                mcSelectJoints.Checked = True
            ElseIf Me.Editor.CommandBar.CommandMode = Command.enumCommandMode.SelectReceptiveFields Then
                mcReceptiveFields.Checked = True
            Else
                mcAddBody.Checked = True
            End If

            If TypeOf Me.Editor.PhysicalStructure Is DataObjects.Physical.Organism Then
                popup.MenuCommands.AddRange(New MenuCommand() {mcSelectBodies, mcSelectJoints, mcAddBody, mcReceptiveFields, mcSepEditStart1})
            Else
                popup.MenuCommands.AddRange(New MenuCommand() {mcSelectBodies, mcSelectJoints, mcAddBody, mcSepEditStart1})
            End If

            Dim mcCut As New MenuCommand("Cut", "Cut", m_beEditor.SmallImages.ImageList, _
                                         m_beEditor.SmallImages.GetImageIndex("AnimatTools.Cut.gif"), _
                                         Shortcut.CtrlX, New EventHandler(AddressOf Me.OnCut))
            Dim mcCopy As New MenuCommand("Copy", "Copy", m_beEditor.SmallImages.ImageList, _
                                            m_beEditor.SmallImages.GetImageIndex("AnimatTools.Copy.gif"), _
                                            Shortcut.CtrlC, New EventHandler(AddressOf Me.OnCopy))
            Dim mcPaste As New MenuCommand("Paste", "Paste", m_beEditor.SmallImages.ImageList, _
                                            m_beEditor.SmallImages.GetImageIndex("AnimatTools.Paste.gif"), _
                                            Shortcut.CtrlV, New EventHandler(AddressOf Me.OnPaste))
            Dim mcDelete As New MenuCommand("Delete", "Delete", m_beEditor.SmallImages.ImageList, _
                                            m_beEditor.SmallImages.GetImageIndex("AnimatTools.Delete.gif"), _
                                            Shortcut.Del, New EventHandler(AddressOf Me.OnDelete))

            If Me.Editor.PropertiesBar.SelectedParts.Count = 0 Then
                mcCut.Enabled = False
                mcCopy.Enabled = False
                mcDelete.Enabled = False
            End If

            mcPaste.Enabled = False
            Dim data As IDataObject = Clipboard.GetDataObject()
            If Not data Is Nothing AndAlso data.GetDataPresent("AnimatLab.Body.XMLFormat") Then
                Dim strXml As String = DirectCast(data.GetData("AnimatLab.Body.XMLFormat"), String)
                If strXml.Trim.Length > 0 Then
                    mcPaste.Enabled = True
                End If
            End If

            popup.MenuCommands.AddRange(New MenuCommand() {mcCut, mcCopy, mcPaste, mcDelete})

            'If a node is selected then show the Add Stimulus entry
            If Me.Editor.PropertiesBar.SelectedParts.Count = 1 AndAlso Not Me.Editor.PropertiesBar.SelectedPart Is Nothing Then
                If Me.Editor.PropertiesBar.SelectedPart.AllowStimulus Then
                    Dim mcSep3 As MenuCommand = New MenuCommand("-")
                    Dim mcAddStimulus As New MenuCommand("Add Stimulus", "AddStimulus", m_beEditor.SmallImages.ImageList, _
                                                      m_beEditor.SmallImages.GetImageIndex("AnimatTools.ExternalStimulus.gif"), _
                                                      New EventHandler(AddressOf Me.OnAddStimulus))

                    popup.MenuCommands.AddRange(New MenuCommand() {mcSep3, mcAddStimulus})
                End If
            End If

            Dim mcSepSelectStart As MenuCommand = New MenuCommand("-")
            Dim mcSelectByType As New MenuCommand("Select By Type", "SelectByType", m_beEditor.SmallImages.ImageList, _
                                            m_beEditor.SmallImages.GetImageIndex("AnimatTools.SelectByType.gif"), _
                                            New EventHandler(AddressOf Me.OnSelectByType))
            Dim mcRelabel As New MenuCommand("Relabel", "Relabel", m_beEditor.SmallImages.ImageList, _
                                              m_beEditor.SmallImages.GetImageIndex("AnimatTools.Relabel.gif"), _
                                              New EventHandler(AddressOf Me.OnRelabel))
            Dim mcRelabelSelected As New MenuCommand("Relabel Selected", "RelabelSelected", m_beEditor.SmallImages.ImageList, _
                                              m_beEditor.SmallImages.GetImageIndex("AnimatTools.RelabelSelected.gif"), _
                                              New EventHandler(AddressOf Me.OnRelabelSelected))

            If Me.Editor.PropertiesBar.SelectedParts.Count = 0 Then
                mcRelabelSelected.Enabled = False
            End If

            popup.MenuCommands.AddRange(New MenuCommand() {mcSepSelectStart, mcSelectByType, mcRelabel, mcRelabelSelected})

            ' Show it!
            Dim selected As MenuCommand = popup.TrackPopup(ptScreen)

            Me.Invalidate()
        End Sub

#End Region

#Region " Events "

        Protected Sub OnCut(ByVal sender As Object, ByVal e As System.EventArgs)
            Me.Editor.OnCut(sender, e)
        End Sub

        Protected Sub OnCopy(ByVal sender As Object, ByVal e As System.EventArgs)
            Me.Editor.OnCopy(sender, e)
        End Sub

        Protected Sub OnPaste(ByVal sender As Object, ByVal e As System.EventArgs)
            Me.Editor.OnPaste(sender, e)
        End Sub

        Protected Sub OnDelete(ByVal sender As Object, ByVal e As System.EventArgs)
            Me.Editor.OnDelete(sender, e)
        End Sub

        Protected Sub OnSelectBodies(ByVal sender As Object, ByVal e As System.EventArgs)
            Me.Editor.OnSelectBodies(sender, e)
        End Sub

        Protected Sub OnSelectJoints(ByVal sender As Object, ByVal e As System.EventArgs)
            Me.Editor.OnSelectJoints(sender, e)
        End Sub

        Protected Sub OnAddBody(ByVal sender As Object, ByVal e As System.EventArgs)
            Me.Editor.OnAddBody(sender, e)
        End Sub

        Protected Sub OnSelectReceptiveFields(ByVal sender As Object, ByVal e As System.EventArgs)
            Me.Editor.OnSelectReceptiveFields(sender, e)
        End Sub

        Protected Sub OnAddStimulus(ByVal sender As Object, ByVal e As System.EventArgs)

            Try
                If Me.Editor.PropertiesBar.SelectedParts.Count = 1 AndAlso Not Me.Editor.PropertiesBar.SelectedPart Is Nothing Then
                    Me.Editor.PropertiesBar.SelectedPart.SelectStimulusType()
                End If
            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try
        End Sub

        Protected Sub OnSelectByType(ByVal sender As Object, ByVal e As System.EventArgs)
            Me.Editor.OnSelectByType(sender, e)
        End Sub

        Protected Sub OnRelabel(ByVal sender As Object, ByVal e As System.EventArgs)
            Me.Editor.OnRelabel(sender, e)
        End Sub

        Protected Sub OnRelabelSelected(ByVal sender As Object, ByVal e As System.EventArgs)
            Me.Editor.OnRelabelSelected(sender, e)
        End Sub

#End Region

    End Class

End Namespace
