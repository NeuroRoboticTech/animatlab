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

Namespace DataObjects.Physical

    Public MustInherit Class BodyPart
        Inherits DataObjects.DragObject

#Region " Attributes "

        Protected m_strDescription As String = ""
        Protected m_bpBodyNode As TreeNode
        Protected m_ButtonImage As System.Drawing.Image
        Protected m_bSelected As Boolean

        Protected m_fDistFromCamera As Single = 0.0
        Protected m_fBoundingBoxVolume As Single = 0.0

#End Region

#Region " Properties "

        Public Overrides Property Name() As String
            Get
                Return m_strName
            End Get
            Set(ByVal Value As String)
                If Value Is Nothing OrElse Value.Trim.Length = 0 Then
                    Throw New System.Exception("The name property can not be blank.")
                End If

                'Dim doFind As AnimatTools.DataObjects.Physical.BodyPart = Me.ParentStructure.FindBodyPartByName(Value, False)

                'If Not doFind Is Nothing Then
                '    Throw New System.Exception("There is already a body part with the name '" & Value & "'")
                'End If

                m_strName = Value

                If Not Me.ParentStructure Is Nothing AndAlso Not Me.ParentStructure.BodyEditor Is Nothing AndAlso Not Me.ParentStructure.BodyEditor.HierarchyBar Is Nothing AndAlso Not Me.BodyPlanTreeNode Is Nothing Then
                    Me.BodyPlanTreeNode.Text = m_strName
                End If
            End Set
        End Property

        <Browsable(False)> _
        Public Overrides Property ItemName() As String
            Get
                Return Me.Name
            End Get
            Set(ByVal Value As String)
                Me.Name = Value
            End Set
        End Property

        Public Overridable ReadOnly Property BodyPlanTreeNode() As TreeNode
            Get
                Return m_bpBodyNode
            End Get
        End Property

        'Type tells what type of bodypart (hinge, box, etc..
        Public MustOverride ReadOnly Property Type() As String

        'BodyPartType tells if it is a rigidbody or a joint.
        Public MustOverride ReadOnly Property BodyPartType() As String

        'BodyPartName is a descriptive name used in the UI that tells the type of part.
        Public MustOverride ReadOnly Property BodyPartName() As String

        'AssemblyClass tells if it is a rigidbody or a joint.
        Public MustOverride ReadOnly Property PartType() As System.Type

        Public Overridable ReadOnly Property ParentStructure() As AnimatTools.DataObjects.Physical.PhysicalStructure
            Get
                Dim doParent As AnimatTools.DataObjects.Physical.PhysicalStructure
                Dim doTemp As AnimatTools.DataObjects.Physical.BodyPart

                If Not Me.Parent Is Nothing AndAlso (TypeOf Me.Parent Is Physical.BodyPart OrElse TypeOf Me.Parent Is Physical.PhysicalStructure) Then
                    If TypeOf Me.Parent Is Physical.BodyPart Then
                        doTemp = DirectCast(Me.Parent, Physical.BodyPart)
                        Return doTemp.ParentStructure
                    ElseIf TypeOf Me.Parent Is Physical.PhysicalStructure Then
                        doParent = DirectCast(Me.Parent, Physical.PhysicalStructure)
                        Return doParent
                    End If
                End If

                Return Nothing
            End Get
        End Property

        <Browsable(False)> _
        Public Overrides Property Image() As System.Drawing.Image
            Get
                If m_Image Is Nothing Then
                    Dim myAssembly As System.Reflection.Assembly
                    myAssembly = System.Reflection.Assembly.Load(Me.AssemblyModuleName)
                    m_Image = ImageManager.LoadImage(myAssembly, Me.ImageName)
                End If

                Return m_Image
            End Get
            Set(ByVal Value As System.Drawing.Image)
                If Not Value Is Nothing Then
                    m_Image = Value
                End If
            End Set
        End Property

        <Browsable(False)> _
        Public Overrides Property DragImage() As System.Drawing.Image
            Get
                If m_DragImage Is Nothing AndAlso Not m_Image Is Nothing Then
                    Return m_Image
                Else
                    Return m_DragImage
                End If
            End Get
            Set(ByVal Value As System.Drawing.Image)
                If Not Value Is Nothing Then
                    m_DragImage = Value
                End If
            End Set
        End Property

        <Browsable(False)> _
        Public Overrides ReadOnly Property DragImageName() As String
            Get
                Return ImageName()
            End Get
        End Property

        <Browsable(False)> _
        Public Overridable Property ButtonImage() As System.Drawing.Image
            Get
                If m_ButtonImage Is Nothing AndAlso Me.ButtonImageName.Trim.Length > 0 Then
                    Dim myAssembly As System.Reflection.Assembly
                    myAssembly = System.Reflection.Assembly.Load(Me.AssemblyModuleName)
                    m_ButtonImage = ImageManager.LoadImage(myAssembly, Me.ButtonImageName)
                End If

                Return m_ButtonImage
            End Get
            Set(ByVal Value As System.Drawing.Image)
                If Not Value Is Nothing Then
                    m_ButtonImage = Value
                End If
            End Set
        End Property

        <Browsable(False)> _
        Public Overridable ReadOnly Property ButtonImageName() As String
            Get
                Return ""
            End Get
        End Property

        Public Overrides ReadOnly Property StructureID() As String
            Get
                If Not Me.ParentStructure Is Nothing Then
                    Return Me.ParentStructure.ID
                End If

                Return ""
            End Get
        End Property

        <Browsable(False)> _
        Public Overrides ReadOnly Property CanBeCharted() As Boolean
            Get
                Return True
            End Get
        End Property

        <Browsable(False)> _
        Public Overridable ReadOnly Property AllowUserAdd() As Boolean
            Get
                Return True
            End Get
        End Property

        <Browsable(False)> _
        Public Overridable Property Selected() As Boolean
            Get
                Return m_bSelected
            End Get
            Set(ByVal Value As Boolean)
                m_bSelected = Value
            End Set
        End Property

        <Browsable(False)> _
        Public Overridable ReadOnly Property IntersectionDistance() As Single
            Get
                Return 0
            End Get
        End Property

        Public MustOverride Property XLocalLocation() As Single
        Public MustOverride Property YLocalLocation() As Single
        Public MustOverride Property ZLocalLocation() As Single

        Public MustOverride Property XLocalLocationScaled() As AnimatTools.Framework.ScaledNumber
        Public MustOverride Property YLocalLocationScaled() As AnimatTools.Framework.ScaledNumber
        Public MustOverride Property ZLocalLocationScaled() As AnimatTools.Framework.ScaledNumber

        Public MustOverride Property XWorldLocation() As Single
        Public MustOverride Property YWorldLocation() As Single
        Public MustOverride Property ZWorldLocation() As Single

        Public MustOverride Property XWorldLocationScaled() As AnimatTools.Framework.ScaledNumber
        Public MustOverride Property YWorldLocationScaled() As AnimatTools.Framework.ScaledNumber
        Public MustOverride Property ZWorldLocationScaled() As AnimatTools.Framework.ScaledNumber

        Public MustOverride Property XRotation() As Single
        Public MustOverride Property YRotation() As Single
        Public MustOverride Property ZRotation() As Single

        Public MustOverride Property XRotationScaled() As AnimatTools.Framework.ScaledNumber
        Public MustOverride Property YRotationScaled() As AnimatTools.Framework.ScaledNumber
        Public MustOverride Property ZRotationScaled() As AnimatTools.Framework.ScaledNumber

        Public MustOverride Property Location() As Vec3d
        Public MustOverride Property Rotation() As Vec3d
        Public MustOverride Property Size() As Vec3d
        Public MustOverride ReadOnly Property MaxDimension() As Single
        Public MustOverride ReadOnly Property MinDimension() As Single

        Public MustOverride Property PartPositionInfo() As PartPositionInfo
        Public MustOverride Property PartPositionState() As PartPositionState

        Public Overridable Property DistanceFromCamera() As Single
            Get
                Return Me.m_fDistFromCamera
            End Get
            Set(ByVal Value As Single)
                Me.m_fDistFromCamera = Value
            End Set
        End Property

        Public Overridable Property BoundingBoxVolume() As Single
            Get
                Return Me.m_fBoundingBoxVolume
            End Get
            Set(ByVal Value As Single)
                Me.m_fBoundingBoxVolume = Value
            End Set
        End Property

        Public Overridable ReadOnly Property HasDynamics() As Boolean
            Get
                Return True
            End Get
        End Property

#End Region

#Region " Methods "

        Public Sub New(ByVal doParent As Framework.DataObject)
            MyBase.New(doParent)
            m_strDescription = ""
        End Sub

        Public Overrides Sub ClearIsDirty()
            MyBase.ClearIsDirty()
            If Not m_thDataTypes Is Nothing Then m_thDataTypes.ClearIsDirty()
        End Sub

        <Browsable(False)> _
        Public Overridable Property Description() As String
            Get
                Return m_strDescription
            End Get
            Set(ByVal Value As String)
                m_strDescription = Value
            End Set
        End Property

        Public Overridable Function FindBodyPart(ByVal strID As String) As BodyPart
        End Function

        Public Overridable Function FindBodyPartByName(ByVal strName As String) As BodyPart
        End Function

        Public Overridable Function FindBodyPartByCloneID(ByVal strId As String) As BodyPart
        End Function

        Public Overrides Function FindDragObject(ByVal strStructureName As String, ByVal strDataItemID As String, Optional ByVal bThrowError As Boolean = True) As DataObjects.DragObject

            Dim oStructure As Object = Util.Environment.FindStructureFromAll(strStructureName, bThrowError)
            If oStructure Is Nothing Then Return Nothing

            Dim doStructure As AnimatTools.DataObjects.Physical.PhysicalStructure = DirectCast(oStructure, AnimatTools.DataObjects.Physical.PhysicalStructure)
            Dim doPart As AnimatTools.DataObjects.Physical.BodyPart

            If Not doStructure Is Nothing Then
                doPart = doStructure.FindBodyPart(strDataItemID, False)
                If doPart Is Nothing AndAlso bThrowError Then
                    Throw New System.Exception("The drag object with id '" & strDataItemID & "' was not found.")
                End If
            End If

            Return doPart

        End Function

        Public Overridable Function CreateJointTreeView(ByRef tvTree As TreeView, ByVal tnParent As TreeNode, _
                                                        ByVal thSelectedPart As TypeHelpers.LinkedBodyPart) As TreeNode
        End Function

        Public Overridable Function CreateRigidBodyTreeView(ByRef tvTree As TreeView, ByVal tnParent As TreeNode, _
                                                           ByVal thSelectedPart As TypeHelpers.LinkedBodyPart) As TreeNode
        End Function

        Public Overridable Sub CreateBodyPlanTreeView(ByVal dsSim As AnimatTools.DataObjects.Simulation, _
                                                      ByVal doStructure As DataObjects.Physical.PhysicalStructure, _
                                                      ByVal doParentNode As DataObjects.Physical.BodyPart, _
                                                      ByVal frmHierarchy As Forms.BodyPlan.Hierarchy)

            If Not doParentNode Is Nothing Then
                m_bpBodyNode = doParentNode.BodyPlanTreeNode.Nodes.Add(Me.Name)
            Else
                m_bpBodyNode = doStructure.BodyPlanStructureNode.Nodes.Add(Me.Name)
            End If

            frmHierarchy.ImageManager.AddImage(Me.ImageName, Me.Image)

            m_bpBodyNode.ImageIndex = frmHierarchy.ImageManager.GetImageIndex(Me.ImageName)
            m_bpBodyNode.SelectedImageIndex = frmHierarchy.ImageManager.GetImageIndex(Me.ImageName)
            m_bpBodyNode.Tag = Me

            If Me.Enabled Then
                m_bpBodyNode.BackColor = Color.White
            Else
                m_bpBodyNode.BackColor = Color.Gray
            End If

        End Sub

        Public Overridable Sub RemoveBodyPlanTreeView()

            If Not m_bpBodyNode Is Nothing Then
                m_bpBodyNode.Remove()
                m_bpBodyNode = Nothing
            End If

        End Sub

        'Public MustOverride Function GetChildBodiesList() As AnimatTools.Collections.RigidBodies
        'Public MustOverride Function GetChildJointsList() As AnimatTools.Collections.Joints

        Public Overridable Sub DeleteBodyPart(Optional ByVal bAskToDelete As Boolean = True)

            If bAskToDelete Then
                If MessageBox.Show("Are you certain that you want to permanently delete this " & _
                                    "body part and all its children?", "Delete Body Part", MessageBoxButtons.YesNo) <> DialogResult.Yes Then
                    Return
                End If
            End If

            If Not Me.ParentStructure Is Nothing Then
                Me.ParentStructure.DeleteBodyPart(Me)

                If Not Me.ParentStructure.BodyEditor Is Nothing AndAlso Not Me.ParentStructure.BodyEditor.HierarchyBar Is Nothing Then
                    Me.ParentStructure.BodyEditor.HierarchyBar.TreeView.SelectedNode = Me.ParentStructure.BodyPlanStructureNode
                End If
            End If

        End Sub

        Public Overridable Sub CopyBodyPart(ByVal bCutData As Boolean)

            Dim doClone As AnimatTools.DataObjects.Physical.RigidBody = DirectCast(Me.Clone(Me.Parent, bCutData, Me), AnimatTools.DataObjects.Physical.RigidBody)

            'Now lets save the xml for this cloned object.
            Dim oXml As New AnimatTools.Interfaces.StdXml

            oXml.AddElement("CopyData")

            doClone.SaveData(Util.Simulation, Me.ParentStructure, oXml)

            Dim strXml As String = oXml.Serialize()

            Dim data As New System.Windows.Forms.DataObject
            data.SetData("AnimatLab.Body.XMLFormat", strXml)
            Clipboard.SetDataObject(data, True)

        End Sub

        Public Overridable Function BodyPlanTreeviewPopupMenu(ByRef tnSelectedNode As TreeNode, ByVal ptPoint As Point) As Boolean

            If tnSelectedNode Is m_bpBodyNode Then
                ' Create the popup menu object
                Dim popup As New PopupMenu

                ' Create the menu items

                If Me.AllowStimulus AndAlso Me.CompatibleStimuli.Count > 0 Then
                    ' Create the menu items
                    Dim mcAddStimulus As New MenuCommand("Add Stimulus", "AddStimulus", Util.Application.SmallImages.ImageList, _
                                                         Util.Application.SmallImages.GetImageIndex("AnimatTools.AddStimulus.gif"), _
                                                         New EventHandler(AddressOf Me.OnAddStimulus))
                    popup.MenuCommands.Add(mcAddStimulus)
                End If

                Dim mcSwapPart As New MenuCommand("Swap Part", "SwapPart", Util.Application.SmallImages.ImageList, _
                                             Util.Application.SmallImages.GetImageIndex("AnimatTools.Swap.gif"), _
                                             New EventHandler(AddressOf Me.OnSwapBodyPart))
                popup.MenuCommands.Add(mcSwapPart)

                Dim mcCut As New MenuCommand("Cut", "Cut", Util.Application.SmallImages.ImageList, _
                                             Util.Application.SmallImages.GetImageIndex("AnimatTools.Cut.gif"), _
                                             Shortcut.CtrlX, New EventHandler(AddressOf Me.OnCutBodyPart))
                Dim mcCopy As New MenuCommand("Copy", "Copy", Util.Application.SmallImages.ImageList, _
                                                Util.Application.SmallImages.GetImageIndex("AnimatTools.Copy.gif"), _
                                                Shortcut.CtrlC, New EventHandler(AddressOf Me.OnCopyBodyPart))
                Dim mcDelete As New MenuCommand("Delete", "Delete", Util.Application.SmallImages.ImageList, _
                                                     Util.Application.SmallImages.GetImageIndex("AnimatTools.Delete.gif"), _
                                                     New EventHandler(AddressOf Me.OnDeleteBodyPart))
                popup.MenuCommands.AddRange(New MenuCommand() {mcCut, mcCopy, mcDelete})

                If Not Me.ParentStructure Is Nothing AndAlso Not Me.ParentStructure.BodyEditor Is Nothing Then
                    Dim mcRelabel As New MenuCommand("Relabel Children", "RelabelChildren", Me.ParentStructure.BodyEditor.SmallImages.ImageList, _
                                                      Me.ParentStructure.BodyEditor.SmallImages.GetImageIndex("AnimatTools.Relabel.gif"), _
                                                      New EventHandler(AddressOf Me.OnRelabelChildren))
                    popup.MenuCommands.Add(mcRelabel)
                End If


                Dim mcSepExpand As MenuCommand = New MenuCommand("-")
                Dim mcExpandAll As New MenuCommand("Expand All", tnSelectedNode, _
                                                  New EventHandler(AddressOf Me.OnExpandAll))
                Dim mcCollapseAll As New MenuCommand("Collapse All", tnSelectedNode, _
                                                  New EventHandler(AddressOf Me.OnCollapseAll))

                mcExpandAll.ImageList = Util.Application.SmallImages.ImageList
                mcExpandAll.ImageIndex = Util.Application.SmallImages.GetImageIndex("AnimatTools.Expand.gif")
                mcCollapseAll.ImageList = Util.Application.SmallImages.ImageList
                mcCollapseAll.ImageIndex = Util.Application.SmallImages.GetImageIndex("AnimatTools.Collapse.gif")

                popup.MenuCommands.AddRange(New MenuCommand() {mcSepExpand, mcExpandAll, mcCollapseAll})

                ' Show it!
                Dim selected As MenuCommand = popup.TrackPopup(ptPoint)

                Return True
            End If

            Return False
        End Function

        Public Overrides Sub SelectStimulusType()
            Dim frmStimulusType As New Forms.ExternalStimuli.SelectStimulusType
            frmStimulusType.CompatibleStimuli = Me.CompatibleStimuli

            If frmStimulusType.ShowDialog(Util.Application) = DialogResult.OK Then
                Dim doStimulus As DataObjects.ExternalStimuli.BodyPartStimulus = DirectCast(frmStimulusType.SelectedStimulus.Clone(Util.Application.FormHelper, False, Nothing), DataObjects.ExternalStimuli.BodyPartStimulus)
                doStimulus.PhysicalStructure = Me.ParentStructure
                doStimulus.BodyPart = Me

                Util.Application.NewStimuliIndex = Util.Application.NewStimuliIndex + 1
                doStimulus.Name = "Stimulus_" & Util.Application.NewStimuliIndex

                Util.Application.ProjectStimuli.Add(doStimulus.ID, doStimulus)
                doStimulus.CreateWorkspaceTreeView(Util.Simulation, Util.Application.ProjectWorkspace)
            End If
        End Sub

        Public Overridable Sub InitializeAfterLoad(ByRef dsSim As Simulation, ByRef doStructure As DataObjects.Physical.PhysicalStructure)
        End Sub

        Public MustOverride Function CreateNewBodyPart(ByVal doParent As AnimatTools.Framework.DataObject) As AnimatTools.DataObjects.Physical.BodyPart
        Public MustOverride Sub RenameBodyParts(ByVal doStructure As AnimatTools.DataObjects.Physical.PhysicalStructure)
        Public MustOverride Sub ClearSelectedBodyParts()
        Public MustOverride Sub RefreshSelectionBoxSizes()

        Protected Overrides Sub BuildProperties()
            MyBase.BuildProperties()

            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Name", m_strID.GetType(), "Name", _
                                        "Part Properties", "The name of this item.", m_strName))

            Dim pbNumberBag As Crownwood.Magic.Controls.PropertyBag = Me.XRotationScaled.Properties
            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("X", pbNumberBag.GetType(), "XRotationScaled", _
                                        "Rotation", "Sets the x rotation of this body part.", pbNumberBag, _
                                        "", GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)))

            pbNumberBag = Me.YRotationScaled.Properties
            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Y", pbNumberBag.GetType(), "YRotationScaled", _
                                        "Rotation", "Sets the y rotation of this body part.", pbNumberBag, _
                                        "", GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)))

            pbNumberBag = Me.ZRotationScaled.Properties
            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Z", pbNumberBag.GetType(), "ZRotationScaled", _
                                        "Rotation", "Sets the z rotation of this body part.", pbNumberBag, _
                                        "", GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)))

            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Description", m_strDescription.GetType(), "Description", _
                                        "Part Properties", "Sets the description for this body part.", m_strDescription, _
                                        GetType(AnimatTools.TypeHelpers.MultiLineStringTypeEditor)))

            'm_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Rotation", Me.Rotation.GetType(), "Rotation", _
            '                            "Part Properties", "3D position of this structure in the environment.", Me.Rotation, _
            '                            "", GetType(TypeHelpers.Vec3dTypeConverter)))

        End Sub

        Public Overridable Overloads Sub LoadData(ByRef dsSim As Simulation, ByRef doStructure As DataObjects.Physical.PhysicalStructure, ByRef oXml As Interfaces.StdXml)

            oXml.IntoElem() 'Into BodyPart Element
            m_strID = oXml.GetChildString("ID")
            m_strName = oXml.GetChildString("Name", m_strID)

            If m_strID.Trim.Length = 0 Then
                m_strID = System.Guid.NewGuid().ToString()
            End If

            If m_strName.Trim.Length = 0 Then
                m_strName = m_strID
            End If

            m_strDescription = oXml.GetChildString("Description", "")

            oXml.OutOfElem() 'Outof BodyPart Element

        End Sub

        Public Overridable Overloads Sub SaveData(ByRef dsSim As Simulation, ByRef doStructure As DataObjects.Physical.PhysicalStructure, ByRef oXml As Interfaces.StdXml)

            oXml.AddChildElement(Me.BodyPartType)

            oXml.IntoElem() 'Into Child Elemement
            oXml.AddChildElement("Name", m_strName)
            oXml.AddChildElement("ID", m_strID)
            oXml.AddChildElement("Type", Me.Type)
            oXml.AddChildElement("PartType", Me.PartType.ToString)
            oXml.AddChildElement("Description", m_strDescription)

            If Me.ModuleName.Length > 0 Then
                oXml.AddChildElement("ModuleName", Me.ModuleName)
            End If
            oXml.OutOfElem() 'Outof BodyPart Element

        End Sub

        Public Overridable Sub UpdateWithMouse(ByVal AnimatMouseArgs As Framework.MouseEventArgs)

        End Sub

        Public Overrides Function ToString() As String
            Return Me.Name
        End Function

        Public Overridable Function IsSelectionBoxSelected(ByVal AnimatMouseArgs As Framework.MouseEventArgs) As Boolean

        End Function

        Public Overrides Function EnsureParentMdiShown() As AnimatTools.Forms.MdiChild

            If Not Me.ParentStructure Is Nothing AndAlso Me.ParentStructure.BodyEditor Is Nothing Then
                Return Util.Application.EditBodyPlan(Me.ParentStructure)
            End If

        End Function

        Public Overridable Sub BeforeSelected()
        End Sub

        Public Overridable Sub AfterSelected()
        End Sub

        Public Overridable Sub BeforeAddBody()
        End Sub

        Public Overridable Sub AfterAddBody()
            'The newly added part may be a copy of an entire heirarchy of parts. If htis is the
            'case then it may have things in it like springs and muscles that will need to be re-initialized.
            Me.InitializeAfterLoad(Util.Simulation, Me.ParentStructure)
        End Sub

        Public MustOverride Sub Draw(ByVal cmCommand As AnimatTools.Forms.BodyPlan.Command.enumCommandMode)
        Public MustOverride Function SwapBodyPartList() As AnimatTools.Collections.BodyParts
        Public MustOverride Sub SwapBodyPartCopy(ByVal doOriginal As AnimatTools.DataObjects.Physical.BodyPart)


#End Region

#Region " Events "

        Protected Overridable Sub OnAddStimulus(ByVal sender As Object, ByVal e As System.EventArgs)

            Try

                Me.SelectStimulusType()

            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try

        End Sub

        'Protected Overridable Sub OnAddBodyPart(ByVal sender As Object, ByVal e As System.EventArgs)
        'End Sub

        Protected Overridable Sub OnRelabelChildren(ByVal sender As Object, ByVal e As System.EventArgs)
            Try
                Dim frmRelabel As New AnimatTools.Forms.BodyPlan.Relabel

                frmRelabel.PhysicalStructure = Me.ParentStructure
                frmRelabel.RootNode = Me
                If frmRelabel.ShowDialog = DialogResult.OK Then
                    Util.Relable(frmRelabel.Items, frmRelabel.txtMatch.Text, frmRelabel.txtReplace.Text)
                End If

            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try

        End Sub

        Protected Overridable Sub OnSwapBodyPart(ByVal sender As Object, ByVal e As System.EventArgs)

            Try
                Me.ParentStructure.BodyEditor.BodyView.SwapBodyPart(Me)
            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try

        End Sub

        Protected Overridable Sub OnCopyBodyPart(ByVal sender As Object, ByVal e As System.EventArgs)

            Try
                Me.CopyBodyPart(False)
            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try

        End Sub

        Protected Overridable Sub OnCutBodyPart(ByVal sender As Object, ByVal e As System.EventArgs)

            Try
                Me.DeleteBodyPart(False)
            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try

        End Sub

        Protected Overridable Sub OnDeleteBodyPart(ByVal sender As Object, ByVal e As System.EventArgs)

            Try
                Me.DeleteBodyPart(True)
            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try

        End Sub

#End Region

    End Class

End Namespace
