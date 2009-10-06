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

    Public Class PhysicalStructure
        Inherits DataObjects.DragObject

#Region " Attributes "

        Protected m_bReference As Boolean = False
        Protected m_strReferenceID As String = ""
        Protected m_strStructureType As String = "Basic"

        Protected m_fwPosition As New Framework.Vec3d(Me)
        Protected m_snXLocation As New ScaledNumber(Nothing, "XLocationScaled", "meters", "m")
        Protected m_snYLocation As New ScaledNumber(Nothing, "YLocationScaled", "meters", "m")
        Protected m_snZLocation As New ScaledNumber(Nothing, "ZLocationScaled", "meters", "m")

        Protected m_dbRoot As DataObjects.Physical.RigidBody

        Protected m_doReference As PhysicalStructure

        Protected m_frmBodyEditor As Forms.BodyPlan.Editor

        Protected m_wsStructureNode As TreeNode
        Protected m_wsBodyPlanNode As TreeNode
        Protected m_bpStructureNode As TreeNode

        Protected m_iNewBodyIndex As Integer = 0
        Protected m_iNewJointIndex As Integer = 0

        Protected m_aryCollisionExclusionPairs As New Collections.CollisionPairs(Me)
        Protected m_aryMicrocontrollers As New Collections.SortedMicrocontrollers(Me)

#End Region

#Region " Properties "

        <Browsable(False)> _
        Public Overrides ReadOnly Property RootForm() As System.Windows.Forms.Form
            Get
                If Not m_frmBodyEditor Is Nothing Then
                    Return m_frmBodyEditor
                Else
                    Return Util.Application
                End If
            End Get
        End Property

        Protected Overridable ReadOnly Property Structures(ByVal dsSim As AnimatTools.DataObjects.Simulation) As Collections.SortedStructures
            Get
                Return dsSim.Environment.Structures
            End Get
        End Property

        Protected Overridable ReadOnly Property ParentTreeNode(ByVal dsSim As AnimatTools.DataObjects.Simulation) As TreeNode
            Get
                Return dsSim.Environment.StructuresTreeNode
            End Get
        End Property

        Public Overridable Property NewBodyIndex() As Integer
            Get
                Return m_iNewBodyIndex
            End Get
            Set(ByVal Value As Integer)
                m_iNewBodyIndex = Value
            End Set
        End Property

        Public Overridable Property NewJointIndex() As Integer
            Get
                Return m_iNewJointIndex
            End Get
            Set(ByVal Value As Integer)
                m_iNewJointIndex = Value
            End Set
        End Property

        Public Overrides Property Name() As String
            Get
                Return m_strName
            End Get
            Set(ByVal Value As String)
                If Value.Trim.Length = 0 Then
                    Throw New System.Exception("You can not set the name for the organism to blank.")
                End If

                CheckForUniqueneName(Value)

                Dim strOldName As String = m_strName

                m_strName = Value.Trim

                If Not m_wsStructureNode Is Nothing Then
                    Dim bExpanded As Boolean = m_wsStructureNode.IsExpanded
                    m_wsStructureNode.Remove()
                    CreateWorkspaceTreeView(Util.Simulation, Util.Application.ProjectWorkspace)
                    Util.Application.ProjectWorkspace.ctrlTreeView.SelectedNode = m_wsStructureNode
                    If bExpanded Then m_wsStructureNode.Expand()
                End If

                RenameFiles(strOldName)

                RenameWindowTitles()

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

        <Browsable(False)> _
        Public Overrides Property Image() As System.Drawing.Image
            Get
                If m_Image Is Nothing Then
                    Dim myAssembly As System.Reflection.Assembly
                    myAssembly = System.Reflection.Assembly.Load("AnimatTools")
                    m_Image = ImageManager.LoadImage(myAssembly, Me.ImageName)
                End If

                Return m_Image
            End Get
            Set(ByVal Value As System.Drawing.Image)
                m_Image = Value
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
                m_DragImage = Value
            End Set
        End Property

        Public Overrides ReadOnly Property ImageName() As String
            Get
                Return "AnimatTools.Structure.gif"
            End Get
        End Property

        Public Overrides ReadOnly Property DragImageName() As String
            Get
                Return ImageName()
            End Get
        End Property

        Public Overridable Property Position() As Framework.Vec3d
            Get
                Return m_fwPosition
            End Get
            Set(ByVal Value As Framework.Vec3d)
                m_fwPosition = Value
            End Set
        End Property

        Public Overridable Property XLocation() As Double
            Get
                Return m_fwPosition.X
            End Get
            Set(ByVal Value As Double)
                m_fwPosition.X = Value
            End Set
        End Property

        Public Overridable Property YLocation() As Double
            Get
                Return m_fwPosition.Y
            End Get
            Set(ByVal Value As Double)
                m_fwPosition.Y = Value
            End Set
        End Property

        Public Overridable Property ZLocation() As Double
            Get
                Return m_fwPosition.Z
            End Get
            Set(ByVal Value As Double)
                m_fwPosition.Z = Value
            End Set
        End Property

        Public Overridable Property XLocationScaled() As ScaledNumber
            Get
                m_snXLocation.SetFromValue(Me.XLocation * Util.Environment.DistanceUnitValue, CInt(Util.Environment.DistanceUnits))
                Return m_snXLocation
            End Get
            Set(ByVal Value As ScaledNumber)
                If Not Value Is Nothing Then
                    Me.XLocation = CSng(Value.ActualValue / Util.Environment.DistanceUnitValue)
                End If
            End Set
        End Property

        Public Overridable Property YLocationScaled() As ScaledNumber
            Get
                m_snYLocation.SetFromValue(Me.YLocation * Util.Environment.DistanceUnitValue, CInt(Util.Environment.DistanceUnits))
                Return m_snYLocation
            End Get
            Set(ByVal Value As ScaledNumber)
                If Not Value Is Nothing Then
                    Me.YLocation = CSng(Value.ActualValue / Util.Environment.DistanceUnitValue)
                End If
            End Set
        End Property

        Public Overridable Property ZLocationScaled() As ScaledNumber
            Get
                m_snZLocation.SetFromValue(Me.ZLocation * Util.Environment.DistanceUnitValue, CInt(Util.Environment.DistanceUnits))
                Return m_snZLocation
            End Get
            Set(ByVal Value As ScaledNumber)
                If Not Value Is Nothing Then
                    Me.ZLocation = CSng(Value.ActualValue / Util.Environment.DistanceUnitValue)
                End If
            End Set
        End Property

        Public Overridable Property IsReference() As Boolean
            Get
                Return m_bReference
            End Get
            Set(ByVal Value As Boolean)
                m_bReference = Value
            End Set
        End Property

        Public Overridable Property Reference() As PhysicalStructure
            Get
                Return m_doReference
            End Get
            Set(ByVal Value As PhysicalStructure)
                m_doReference = Value
            End Set
        End Property

        Public Overridable ReadOnly Property ReferenceName() As String
            Get
                If Me.Reference Is Nothing Then
                    Throw New System.Exception("The physical structure '" & Me.Name & "' is a reference type but its reference param is null.")
                Else
                    Return Me.Reference.Name
                End If
            End Get
        End Property

        Public Overridable ReadOnly Property BodyPlanFile() As String
            Get
                If Not Me.IsReference Then
                    Return Me.Name & ".astl"
                Else
                    Return Me.ReferenceName & ".astl"
                End If
            End Get
        End Property

        Public Overridable ReadOnly Property BodyPlanEditorFile() As String
            Get
                If Not Me.IsReference Then
                    Return Me.Name & ".abpe"
                Else
                    Return Me.ReferenceName & ".abpe"
                End If
            End Get
        End Property

        Public Overridable Property RootBody() As DataObjects.Physical.RigidBody
            Get
                Return m_dbRoot
            End Get
            Set(ByVal Value As DataObjects.Physical.RigidBody)
                m_dbRoot = Value
            End Set
        End Property

        Public Overridable ReadOnly Property WorkspaceStructureNode() As TreeNode
            Get
                Return m_wsStructureNode
            End Get
        End Property

        Public Overridable ReadOnly Property WorkspaceBodyPlanNode() As TreeNode
            Get
                Return m_wsBodyPlanNode
            End Get
        End Property

        Public Overridable ReadOnly Property BodyPlanStructureNode() As TreeNode
            Get
                Return m_bpStructureNode
            End Get
        End Property

        Public Overrides ReadOnly Property DataColumnModuleName() As String
            Get
                Return ""
            End Get
        End Property

        Public Overrides ReadOnly Property DataColumnClassType() As String
            Get
                Return ""
            End Get
        End Property

        Public Overrides ReadOnly Property StructureID() As String
            Get
                Return Me.ID
            End Get
        End Property

        Public Overrides ReadOnly Property CanBeCharted() As Boolean
            Get
                Return False
            End Get
        End Property

        Public Overridable Property BodyEditor() As Forms.BodyPlan.Editor
            Get
                Return m_frmBodyEditor
            End Get
            Set(ByVal Value As Forms.BodyPlan.Editor)
                m_frmBodyEditor = Value
            End Set
        End Property

        Public Overridable Property CollisionExclusionPairs() As AnimatTools.Collections.CollisionPairs
            Get
                Return m_aryCollisionExclusionPairs
            End Get
            Set(ByVal Value As AnimatTools.Collections.CollisionPairs)
                If Value Is Nothing Then
                    Throw New System.Exception("The collision exclusion pair list can not be null.")
                End If

                'If Me.Name = "Crayfish" AndAlso Not Value.Count = 20 Then
                '    MessageBox.Show("Collision pairs messed up!!!")
                'End If

                m_aryCollisionExclusionPairs = Value
            End Set
        End Property

        Public Overridable Property Microcontrollers() As Collections.SortedMicrocontrollers
            Get
                Return m_aryMicrocontrollers
            End Get
            Set(ByVal Value As Collections.SortedMicrocontrollers)
                If Not Value Is Nothing Then
                    m_aryMicrocontrollers = Value
                End If
            End Set
        End Property

        <Browsable(False)> _
        Public Overridable Property BackColor() As Color
            Get
                If Not Me.BodyEditor Is Nothing AndAlso Not Me.BodyEditor.BodyView Is Nothing Then
                    Return Me.BodyEditor.BodyView.ViewBackColor
                Else
                    Return Color.Black
                End If
            End Get
            Set(ByVal Value As Color)
                If Not Me.BodyEditor Is Nothing AndAlso Not Me.BodyEditor.BodyView Is Nothing Then
                    Me.BodyEditor.BodyView.ViewBackColor = Value
                    Me.BodyEditor.BodyView.Invalidate()
                    Me.BodyEditor.BodyView.Refresh()
                End If
            End Set
        End Property

#End Region

#Region " Methods "

        Public Sub New(ByVal doParent As Framework.DataObject)
            MyBase.New(doParent)
        End Sub

        Protected Overrides Sub BuildProperties()

            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Name", m_strName.GetType(), "Name", _
                                        "Structure Properties", "The name for this structure. ", m_strName))

            If m_bReference AndAlso Not Me.Reference Is Nothing Then
                m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Reference", GetType(String), "ReferenceName", _
                                            "Structure Properties", "The name of the structure that this one references. ", Me.ReferenceName, True))
            End If

            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Body Plan", Me.BodyPlanFile.GetType(), "BodyPlanFile", _
                                        "Structure Properties", "Sets the body plan file.", Me.BodyPlanFile, True))

            Dim pbNumberBag As Crownwood.Magic.Controls.PropertyBag = m_snXLocation.Properties
            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("X", pbNumberBag.GetType(), "XLocationScaled", _
                                        "Location", "Sets the x location for this structure in the virtual world.", pbNumberBag, _
                                        "", GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)))

            pbNumberBag = m_snYLocation.Properties
            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Y", pbNumberBag.GetType(), "YLocationScaled", _
                                        "Location", "Sets the y location for this structure in the virtual world.", pbNumberBag, _
                                        "", GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)))

            pbNumberBag = m_snZLocation.Properties
            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Z", pbNumberBag.GetType(), "ZLocationScaled", _
                                        "Location", "Sets the z location for this structure in the virtual world.", pbNumberBag, _
                                        "", GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)))

            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Collision Exclusions", m_aryCollisionExclusionPairs.GetType(), "CollisionExclusionPairs", _
                                        "Structure Properties", "Pairs of body parts that should be excluded from collision detection between each other.", m_aryCollisionExclusionPairs, _
                                        GetType(TypeHelpers.CollisionPairsTypeEditor), GetType(TypeHelpers.CollisionPairsTypeConverter)))

            If Not Me.BodyEditor Is Nothing AndAlso Not Me.BodyEditor.BodyView Is Nothing Then
                m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("BackColor", Me.BackColor.GetType, "BackColor", _
                                        "Graphical Properties", "Sets the color used to draw the background for this item in the body editor window.", Me.BackColor))
            End If

        End Sub

        Public Overrides Sub ClearIsDirty()
            MyBase.ClearIsDirty()
            m_fwPosition.ClearIsDirty()
            If Not m_dbRoot Is Nothing Then m_dbRoot.ClearIsDirty()
            If Not m_aryCollisionExclusionPairs Is Nothing Then m_aryCollisionExclusionPairs.ClearIsDirty()
        End Sub

        Public Overrides Sub AfterClone(ByVal doParent As AnimatTools.Framework.DataObject, ByVal bCutData As Boolean, _
                                        ByVal doOriginal As AnimatTools.Framework.DataObject, ByVal doClone As AnimatTools.Framework.DataObject)
            MyBase.AfterClone(doParent, bCutData, doOriginal, doClone)

            If Not m_dbRoot Is Nothing Then
                m_dbRoot.AfterClone(Me, bCutData, doOriginal, doClone)
            End If
        End Sub

        Protected Overrides Sub CloneInternal(ByVal doOriginal As AnimatTools.Framework.DataObject, ByVal bCutData As Boolean, _
                                            ByVal doRoot As AnimatTools.Framework.DataObject)
            MyBase.CloneInternal(doOriginal, bCutData, doRoot)

            Dim doOrig As AnimatTools.DataObjects.Physical.PhysicalStructure = DirectCast(doOriginal, PhysicalStructure)

            m_strName = doOrig.m_strName
            m_bReference = doOrig.m_bReference
            m_strReferenceID = doOrig.m_strReferenceID
            m_doReference = doOrig.m_doReference
            m_strStructureType = doOrig.m_strStructureType
            If Not doOrig.Image Is Nothing Then m_Image = DirectCast(doOrig.Image.Clone(), Image)
            If Not doOrig.m_DragImage Is Nothing Then m_DragImage = DirectCast(doOrig.m_DragImage.Clone(), Image)
            If Not doOrig.RootBody Is Nothing Then m_dbRoot = DirectCast(doOrig.RootBody.Clone(Me, bCutData, doRoot), RigidBody)
            m_fwPosition = DirectCast(doOrig.m_fwPosition.Clone(Me, bCutData, doRoot), Vec3d)
            m_frmBodyEditor = doOrig.m_frmBodyEditor
            m_iNewBodyIndex = doOrig.m_iNewBodyIndex
            m_iNewJointIndex = doOrig.m_iNewJointIndex

            m_aryCollisionExclusionPairs.Clear()


            Dim doOrigPair As CollisionPair
            Dim doOrigBody As AnimatTools.DataObjects.Physical.BodyPart
            Dim doFoundBody As AnimatTools.DataObjects.Physical.BodyPart
            For Each deItem As DictionaryEntry In doOrig.m_aryCollisionExclusionPairs
                doOrigPair = DirectCast(deItem.Value, CollisionPair)
                doOrigBody = Me.FindBodyPartByName(doOrigPair.Part1.Name)
                doFoundBody = Me.FindBodyPartByName(doOrigPair.Part2.Name)
                m_aryCollisionExclusionPairs.Add(New CollisionPair(Me, DirectCast(doOrigBody, RigidBody), DirectCast(doFoundBody, RigidBody)))
            Next

            m_aryMicrocontrollers = DirectCast(doOrig.m_aryMicrocontrollers.Clone(Me, bCutData, doRoot), AnimatTools.Collections.SortedMicrocontrollers)

        End Sub

        Public Overrides Function Clone(ByVal doParent As AnimatTools.Framework.DataObject, ByVal bCutData As Boolean, _
                                        ByVal doRoot As AnimatTools.Framework.DataObject) As AnimatTools.Framework.DataObject
            Dim doItem As New PhysicalStructure(doParent)
            doItem.CloneInternal(Me, bCutData, doRoot)
            If Not doRoot Is Nothing AndAlso doRoot Is Me Then doItem.AfterClone(Me, bCutData, doRoot, doItem)
            Return doItem
        End Function

        Public Overridable Function FindBodyPart(ByVal strID As String, Optional ByVal bThrowError As Boolean = True) As BodyPart
            Dim bpPart As BodyPart

            If Not m_dbRoot Is Nothing Then bpPart = m_dbRoot.FindBodyPart(strID)

            If bpPart Is Nothing AndAlso bThrowError Then
                Throw New System.Exception("The body part with ID '" & strID & "' could not be found in the organism '" & m_strName & "'.")
            End If

            Return bpPart
        End Function

        Public Overridable Function FindBodyPartByName(ByVal strName As String, Optional ByVal bThrowError As Boolean = True) As BodyPart
            Dim bpPart As BodyPart

            If Not m_dbRoot Is Nothing Then bpPart = m_dbRoot.FindBodyPartByName(strName)

            If bpPart Is Nothing AndAlso bThrowError Then
                Throw New System.Exception("The body part with name '" & strName & "' could not be found in the organism '" & m_strName & "'.")
            End If

            Return bpPart
        End Function

        Public Overridable Function FindBodyPartByCloneID(ByVal strID As String, Optional ByVal bThrowError As Boolean = True) As BodyPart
            Dim bpPart As BodyPart

            If Not m_dbRoot Is Nothing Then bpPart = m_dbRoot.FindBodyPartByCloneID(strID)

            If bpPart Is Nothing AndAlso bThrowError Then
                Throw New System.Exception("The body part with name '" & strID & "' could not be found in the organism '" & m_strName & "'.")
            End If

            Return bpPart
        End Function

        Public Overridable Sub AddRootBody(ByVal rbRootToAdd As AnimatTools.DataObjects.Physical.RigidBody)

            m_dbRoot = rbRootToAdd
            m_dbRoot.IsRoot = True

            If Not m_dbRoot Is Nothing AndAlso Not Me.BodyEditor Is Nothing AndAlso Not Me.BodyEditor.HierarchyBar Is Nothing Then
                m_dbRoot.CreateBodyPlanTreeView(Util.Simulation, Me, Nothing, Me.BodyEditor.HierarchyBar)
            End If

            If Not Me.BodyEditor Is Nothing Then
                Me.ManualAddHistory(New AnimatTools.Framework.UndoSystem.AddBodyPartEvent(Me.BodyEditor, Me, Nothing, m_dbRoot))
            End If
        End Sub

        Public Overridable Sub DeleteBodyPart(ByVal strID As String)
            Dim bpPart As AnimatTools.DataObjects.Physical.BodyPart = Me.FindBodyPart(strID)
            DeleteBodyPart(bpPart)
        End Sub

        Public Overridable Sub DeleteBodyPart(ByVal bpPart As AnimatTools.DataObjects.Physical.BodyPart)

            'If the object to delete is a joint, then switch to deleting the parent rigid body it is connected to.
            'You can never just delete a joint by itself. You must always delete the underlying body part to get rid of the joint.
            If TypeOf bpPart Is AnimatTools.DataObjects.Physical.Joint Then
                bpPart = DirectCast(bpPart.Parent, AnimatTools.DataObjects.Physical.BodyPart)
            End If

            Dim bpDeletedPart As AnimatTools.DataObjects.Physical.RigidBody = DirectCast(bpPart, AnimatTools.DataObjects.Physical.RigidBody)
            Dim bpParentPart As AnimatTools.DataObjects.Physical.RigidBody
            If Not bpDeletedPart.IsRoot Then
                bpParentPart = DirectCast(bpDeletedPart.Parent, AnimatTools.DataObjects.Physical.RigidBody)
            End If

            DeleteBodyPartInternal(bpPart)

            If Not Me.BodyEditor Is Nothing Then
                Me.ManualAddHistory(New AnimatTools.Framework.UndoSystem.DeleteBodyPartEvent(Me.BodyEditor, Me, bpParentPart, bpDeletedPart))
            End If
        End Sub

        Protected Overridable Sub DeleteBodyPartInternal(ByVal bpPart As AnimatTools.DataObjects.Physical.BodyPart)

            If Not bpPart.Parent Is Nothing Then
                If TypeOf bpPart Is AnimatTools.DataObjects.Physical.RigidBody Then

                    If bpPart Is m_dbRoot Then
                        m_dbRoot = Nothing
                    Else
                        Dim bpParent As RigidBody = DirectCast(bpPart.Parent, RigidBody)
                        If bpParent.ChildBodies.Contains(bpPart.ID) Then
                            bpParent.ChildBodies.Remove(bpPart.ID)
                        Else
                            Throw New System.Exception("You are attempting to delete an child item that is not in parents child collection.")
                        End If
                    End If
                ElseIf TypeOf bpPart Is AnimatTools.DataObjects.Physical.Joint Then
                    Dim bpParent As RigidBody = DirectCast(bpPart.Parent, RigidBody)
                    DeleteBodyPart(bpParent)
                End If

                bpPart.RemoveBodyPlanTreeView()
            End If

        End Sub

        Public Sub Draw(ByVal cmCommand As AnimatTools.Forms.BodyPlan.Command.enumCommandMode)

            'get a list of transparent objects and a list of non transparent objects
            Dim aryTransparent As New ArrayList
            Dim aryNonTransparent As New ArrayList

            m_dbRoot.GetTransparentBodies(aryTransparent, aryNonTransparent, cmCommand)

            Dim dbChild As AnimatTools.DataObjects.Physical.BodyPart

            'Draw all the non transparent objects first
            For i As Integer = 0 To aryNonTransparent.Count - 1
                dbChild = DirectCast(aryNonTransparent(i), AnimatTools.DataObjects.Physical.BodyPart)
                dbChild.Draw(cmCommand)
            Next i

            aryTransparent.Sort(New AnimatTools.Framework.BodyPartDistanceFromCameraComparer)


            'Now draw all the transparent objects
            For i As Integer = 0 To aryTransparent.Count - 1
                dbChild = DirectCast(aryTransparent(i), AnimatTools.DataObjects.Physical.BodyPart)
                dbChild.Draw(cmCommand)
            Next i

        End Sub

        Public Function TestIntersection(ByVal x As Integer, ByVal y As Integer, ByVal cmCommand As AnimatTools.Forms.BodyPlan.Command.enumCommandMode) As ArrayList
            Dim colSelected As New ArrayList

            m_dbRoot.TestIntersection(x, y, cmCommand, colSelected)

            Return colSelected

        End Function

        Public Sub GetStructureExent(ByRef maxX As Single, ByRef minX As Single, ByRef maxY As Single, ByRef minY As Single)
            If Not m_dbRoot Is Nothing Then
                m_dbRoot.GetBodyExtent(maxX, minX, maxY, minY)
            Else
                maxX = 0.5
                maxY = 0.5
            End If
        End Sub

        Public Overrides Sub FindChildrenOfType(ByVal tpTemplate As System.Type, ByRef colDataObjects As Collections.DataObjects)
            If Not m_dbRoot Is Nothing Then
                m_dbRoot.FindChildrenOfType(tpTemplate, colDataObjects)
            End If
        End Sub

        Public Overrides Sub SaveDataColumnToXml(ByRef oXml As AnimatTools.Interfaces.StdXml)

        End Sub

        Public Overrides Function FindDragObject(ByVal strStructureName As String, ByVal strDataItemID As String, Optional ByVal bThrowError As Boolean = True) As DataObjects.DragObject
            Dim oStructure As Object = Util.Environment.FindStructureFromAll(strStructureName, bThrowError)

            If Not oStructure Is Nothing Then
                Return DirectCast(oStructure, DataObjects.DragObject)
            End If

            Return Nothing
        End Function

        Public Overridable Function GetChildPartsList() As AnimatTools.Collections.SortedBodyParts
            Dim aryParts As New AnimatTools.Collections.SortedBodyParts(Nothing)
            If Not m_dbRoot Is Nothing Then
                m_dbRoot.GetChildPartsList(aryParts)
            End If

            Return aryParts
        End Function

        Public Overridable Function GetChildBodiesList() As AnimatTools.Collections.SortedRigidBodies
            Dim aryBodies As New AnimatTools.Collections.SortedRigidBodies(Nothing)
            If Not m_dbRoot Is Nothing Then
                m_dbRoot.GetChildBodiesList(aryBodies)
            End If

            Return aryBodies
        End Function

        Public Overridable Function GetChildJointsList() As AnimatTools.Collections.SortedJoints
            Dim aryBodies As New AnimatTools.Collections.SortedJoints(Nothing)
            If Not m_dbRoot Is Nothing Then
                m_dbRoot.GetChildJointsList(aryBodies)
            End If

            Return aryBodies
        End Function

        Public Overridable Function CreateJointTreeView(ByRef tvTree As TreeView, ByVal tnParent As TreeNode, _
                                                      ByVal thSelectedPart As TypeHelpers.LinkedBodyPart) As TreeNode

            Dim tnOrganism As TreeNode
            If Not tnParent Is Nothing Then
                tnOrganism = tnParent.Nodes.Add(Me.Name)
            Else
                tnOrganism = tvTree.Nodes.Add(Me.Name)
            End If
            tnOrganism.ForeColor = Color.Black

            If Not m_dbRoot Is Nothing Then
                m_dbRoot.CreateJointTreeView(tvTree, tnOrganism, thSelectedPart)
            End If

            Return tnOrganism
        End Function

        Public Overridable Function CreateRigidBodyTreeView(ByRef tvTree As TreeView, ByVal tnParent As TreeNode, _
                                                            ByVal thSelectedPart As TypeHelpers.LinkedBodyPart) As TreeNode

            Dim tnOrganism As TreeNode
            If Not tnParent Is Nothing Then
                tnOrganism = tnParent.Nodes.Add(Me.Name)
            Else
                tnOrganism = tvTree.Nodes.Add(Me.Name)
            End If
            tnOrganism.ForeColor = Color.Black

            If Not m_dbRoot Is Nothing Then
                m_dbRoot.CreateRigidBodyTreeView(tvTree, tnOrganism, thSelectedPart)
            End If

            Return tnOrganism
        End Function

#Region " Workspace TreeView "

        Public Overridable Sub CreateWorkspaceTreeView(ByVal dsSim As AnimatTools.DataObjects.Simulation, _
                                                       ByVal frmWorkspace As Forms.ProjectWorkspace)

            m_wsStructureNode = Me.ParentTreeNode(dsSim).Nodes.Add(Me.Name)
            m_wsStructureNode.ImageIndex = frmWorkspace.ImageManager.GetImageIndex(Me.ImageName)
            m_wsStructureNode.SelectedImageIndex = frmWorkspace.ImageManager.GetImageIndex(Me.ImageName)
            m_wsStructureNode.Tag = Me
            If Me.IsReference Then
                m_wsStructureNode.ForeColor = Color.LightSteelBlue
            End If

        End Sub

        Public Overridable Function WorkspaceTreeviewPopupMenu(ByRef tnSelectedNode As TreeNode, ByVal ptPoint As Point) As Boolean

            If tnSelectedNode Is m_wsStructureNode Then
                ' Create the menu items
                Dim mcDelete As New MenuCommand("Delete Structure", "DeleteStructure", Util.Application.SmallImages.ImageList, _
                                              Util.Application.SmallImages.GetImageIndex("AnimatTools.Delete.gif"), _
                                              New EventHandler(AddressOf Me.OnDeleteStructure))

                ' Create the popup menu object
                Dim popup As New PopupMenu

                If Not Me.IsReference Then
                    Dim mcClone As New MenuCommand("Clone Structure", "CloneStructure", _
                                                      New EventHandler(AddressOf Me.OnCloneStructure))
                    Dim mcCopy As New MenuCommand("Copy Reference", "CopyReference", _
                                                      New EventHandler(AddressOf Me.OnCopyReference))


                    ' Define the list of menu commands
                    popup.MenuCommands.AddRange(New MenuCommand() {mcDelete, mcClone, mcCopy})
                Else
                    popup.MenuCommands.Add(mcDelete)
                End If

                Dim mcSepExpand As MenuCommand = New MenuCommand("-")
                Dim mcExpandAll As New MenuCommand("Expand All", tnSelectedNode, _
                                                  New EventHandler(AddressOf Me.OnExpandAll))
                Dim mcCollapseAll As New MenuCommand("Collapse All", tnSelectedNode, _
                                                  New EventHandler(AddressOf Me.OnCollapseAll))
                popup.MenuCommands.AddRange(New MenuCommand() {mcSepExpand, mcExpandAll, mcCollapseAll})

                mcExpandAll.ImageList = Util.Application.SmallImages.ImageList
                mcExpandAll.ImageIndex = Util.Application.SmallImages.GetImageIndex("AnimatTools.Expand.gif")
                mcCollapseAll.ImageList = Util.Application.SmallImages.ImageList
                mcCollapseAll.ImageIndex = Util.Application.SmallImages.GetImageIndex("AnimatTools.Collapse.gif")

                ' Show it!
                Dim selected As MenuCommand = popup.TrackPopup(ptPoint)

                Return True
            ElseIf tnSelectedNode Is m_wsBodyPlanNode Then
                Return True
            End If

            Return False
        End Function

        Public Overridable Function WorkspaceTreeviewDoubleClick(ByRef tnSelectedNode As TreeNode) As Boolean

            If tnSelectedNode Is m_wsStructureNode Then
                Dim doRef As PhysicalStructure = Me
                If Me.IsReference Then
                    If Me.Reference Is Nothing Then
                        Throw New System.Exception("The physical structure '" & Me.Name & "' is a reference type but its reference param is null.")
                    Else
                        doRef = Me.Reference
                    End If
                End If

                Util.Application.EditBodyPlan(doRef)
                Return True
            End If

            Return False
        End Function

#End Region

#Region " BodyPlan TreeView "

        Public Overridable Sub CreateBodyPlanTreeView(ByVal dsSim As AnimatTools.DataObjects.Simulation, _
                                                       ByRef frmHierarchy As Forms.BodyPlan.Hierarchy)

            frmHierarchy.TreeView.Nodes.Clear()

            Dim myAssembly As System.Reflection.Assembly
            myAssembly = System.Reflection.Assembly.Load("AnimatTools")
            frmHierarchy.ImageManager.AddImage(myAssembly, Me.ImageName)

            m_bpStructureNode = frmHierarchy.TreeView.Nodes.Add(Me.Name)
            m_bpStructureNode.ImageIndex = frmHierarchy.ImageManager.GetImageIndex(Me.ImageName)
            m_bpStructureNode.SelectedImageIndex = frmHierarchy.ImageManager.GetImageIndex(Me.ImageName)
            m_bpStructureNode.Tag = Me

            If Not m_dbRoot Is Nothing Then
                m_dbRoot.CreateBodyPlanTreeView(dsSim, Me, Nothing, frmHierarchy)
            End If

            frmHierarchy.TreeView.ExpandAll()

        End Sub

        Public Overridable Function BodyPlanTreeviewPopupMenu(ByRef tnSelectedNode As TreeNode, ByVal ptPoint As Point) As Boolean

            If tnSelectedNode Is m_bpStructureNode Then
                ' Create the popup menu object
                Dim popup As New PopupMenu

                ' Create the menu items
                Dim mcExpandAll As New MenuCommand("Expand All", tnSelectedNode, _
                                                  New EventHandler(AddressOf Me.OnExpandAll))
                Dim mcCollapseAll As New MenuCommand("Collapse All", tnSelectedNode, _
                                                  New EventHandler(AddressOf Me.OnCollapseAll))
                popup.MenuCommands.AddRange(New MenuCommand() {mcExpandAll, mcCollapseAll})

                mcExpandAll.ImageList = Util.Application.SmallImages.ImageList
                mcExpandAll.ImageIndex = Util.Application.SmallImages.GetImageIndex("AnimatTools.Expand.gif")
                mcCollapseAll.ImageList = Util.Application.SmallImages.ImageList
                mcCollapseAll.ImageIndex = Util.Application.SmallImages.GetImageIndex("AnimatTools.Collapse.gif")

                ' Show it!
                Dim selected As MenuCommand = popup.TrackPopup(ptPoint)

                Return True
            ElseIf Not m_dbRoot Is Nothing Then
                'Lets check the rigidbody/joint objects below this one.
                Return m_dbRoot.BodyPlanTreeviewPopupMenu(tnSelectedNode, ptPoint)
            End If

            Return False
        End Function

        Public Overridable Function BodyPlanTreeviewDoubleClick(ByRef tnSelectedNode As TreeNode) As Boolean

            If tnSelectedNode Is m_wsStructureNode Then
                Return True
            End If

            Return False
        End Function

#End Region

#Region " Data Item TreeView "

        Public Overrides Function CreateDataItemTreeView(ByVal frmDataItem As Forms.Tools.SelectDataItem, ByVal tnParent As TreeNode, ByVal tpTemplatePartType As Type) As TreeNode

            Dim myAssembly As System.Reflection.Assembly
            myAssembly = System.Reflection.Assembly.Load(Me.AssemblyModuleName)
            frmDataItem.ImageManager.AddImage(myAssembly, Me.ImageName)

            Dim tnNode As TreeNode = frmDataItem.TreeView.Nodes.Add(Me.Name)
            tnNode.ImageIndex = frmDataItem.ImageManager.GetImageIndex(Me.ImageName)
            tnNode.SelectedImageIndex = frmDataItem.ImageManager.GetImageIndex(Me.ImageName)
            tnNode.Tag = Me

            If Not m_dbRoot Is Nothing Then
                m_dbRoot.CreateDataItemTreeView(frmDataItem, tnNode, tpTemplatePartType)
            End If

        End Function

#End Region

#Region " Load/Save Methods "

        Public Overridable Sub InitializeAfterLoad(ByRef dsSim As Simulation)
            If Not m_dbRoot Is Nothing Then
                m_dbRoot.InitializeAfterLoad(dsSim, Me)
            End If

            If Me.IsReference Then
                m_doReference = Util.Environment.FindStructureFromAll(m_strReferenceID)
                LoadBodyPlan(dsSim)
            End If

            Dim doController As DataObjects.Physical.Microcontroller
            For Each deEntry As DictionaryEntry In m_aryMicrocontrollers
                doController = DirectCast(deEntry.Value, DataObjects.Physical.Microcontroller)
                doController.InitializeAfterLoad(dsSim, Me)
            Next

        End Sub

        Protected Overridable Sub LoadBodyPlan(ByVal dsSim As Simulation)
            Dim oXml As New AnimatTools.Interfaces.StdXml

            Try
                m_aryCollisionExclusionPairs.Clear()

                If System.IO.File.Exists(Util.GetFilePath(Util.Application.ProjectPath, Me.BodyPlanFile)) Then
                    oXml.Load(Util.GetFilePath(Util.Application.ProjectPath, Me.BodyPlanFile))

                    oXml.FindElement("Structure")

                    If oXml.FindChildElement("RigidBody", False) OrElse oXml.FindChildElement("Body", False) Then
                        m_dbRoot = DirectCast(dsSim.CreateObject(oXml, "RigidBody", Me), DataObjects.Physical.RigidBody)
                        m_dbRoot.IsRoot = True
                        m_dbRoot.LoadData(dsSim, Me, oXml)
                    Else
                        m_dbRoot = Nothing
                    End If

                    'Load collision pairs
                    If oXml.FindChildElement("CollisionExclusionPairs", False) Then
                        oXml.IntoElem()

                        Dim iCount As Integer = oXml.NumberOfChildren() - 1
                        Dim doPair As AnimatTools.DataObjects.Physical.CollisionPair
                        For iIndex As Integer = 0 To iCount
                            oXml.FindChildByIndex(iIndex)
                            doPair = New AnimatTools.DataObjects.Physical.CollisionPair(Me)
                            doPair.LoadData(dsSim, oXml)

                            If Not doPair.Part1 Is Nothing AndAlso Not doPair.Part2 Is Nothing Then
                                m_aryCollisionExclusionPairs.Add(doPair)
                            End If
                        Next

                        oXml.OutOfElem()
                    End If
                End If

                'Load any microcontrollers if any exist.
                m_aryMicrocontrollers.Clear()
                If oXml.FindChildElement("Microcontrollers", False) Then
                    oXml.IntoElem()

                    Dim iCount As Integer = oXml.NumberOfChildren() - 1
                    Dim doController As AnimatTools.DataObjects.Physical.Microcontroller
                    Dim strAssemblyFile As String
                    Dim strClassName As String

                    For iIndex As Integer = 0 To iCount
                        oXml.FindChildByIndex(iIndex)

                        oXml.IntoElem()
                        strAssemblyFile = oXml.GetChildString("AssemblyFile", "animattools.dll")
                        strClassName = oXml.GetChildString("ClassName", "AnimatTools.DataObjects.Physical.Microcontroller")
                        oXml.OutOfElem()

                        doController = DirectCast(Util.LoadClass(strAssemblyFile, strClassName, Me), AnimatTools.DataObjects.Physical.Microcontroller)
                        doController.LoadData(dsSim, Me, oXml)

                        m_aryMicrocontrollers.Add(doController.ID, doController)
                    Next

                    oXml.OutOfElem()
                End If

                'Now lets find the index values used for adding new bodies and joints.
                Dim aryBodies As AnimatTools.Collections.SortedRigidBodies = Me.GetChildBodiesList()
                Dim aryJoints As AnimatTools.Collections.SortedJoints = Me.GetChildJointsList()

                m_iNewBodyIndex = Util.ExtractIDCount("Body", aryBodies)
                m_iNewJointIndex = Util.ExtractIDCount("Joint", aryJoints)
            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try

        End Sub

        Public Overridable Overloads Sub LoadData(ByVal dsSim As Simulation, ByRef oXml As Interfaces.StdXml)

            Try
                oXml.IntoElem()

                m_strName = oXml.GetChildString("Name")
                m_strID = oXml.GetChildString("ID", System.Guid.NewGuid().ToString())
                m_strStructureType = oXml.GetChildString("Type", m_strStructureType)
                m_bReference = oXml.GetChildBool("Reference")
                m_strReferenceID = oXml.GetChildString("ReferenceID", "")
                m_fwPosition = Util.LoadVec3d(oXml, "Position", Me)

                oXml.OutOfElem()

                If Not m_bReference Then
                    LoadBodyPlan(dsSim)
                End If
            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try

        End Sub

        Public Overridable Sub SaveBodyPlan(ByVal dsSim As Simulation)

            'Save the nervous system file
            Dim oXml As New Interfaces.StdXml
            If Util.Application.ProjectPath.Length > 0 Then
                'Save the body plan file
                oXml.AddElement("Structure")
                SaveBodyPlan(dsSim, oXml)
                oXml.OutOfElem()  'Out of structure
            End If

            oXml.Save(Util.GetFilePath(Util.Application.ProjectPath, Me.BodyPlanFile))
        End Sub

        Public Overridable Sub SaveBodyPlan(ByVal dsSim As Simulation, ByRef oXml As Interfaces.StdXml)

            Try
                If Not m_dbRoot Is Nothing Then
                    m_dbRoot.SaveData(Util.Simulation, Me, oXml)
                End If

                'Save collision pairs
                oXml.AddChildElement("CollisionExclusionPairs")
                oXml.IntoElem()
                For Each doPair As AnimatTools.DataObjects.Physical.CollisionPair In m_aryCollisionExclusionPairs
                    doPair.SaveData(dsSim, oXml)
                Next
                oXml.OutOfElem()  'Out of CollisionExclusionPairs

                If m_aryMicrocontrollers.Count > 0 Then
                    oXml.AddChildElement("Microcontrollers")
                    oXml.IntoElem()
                    Dim doController As DataObjects.Physical.Microcontroller
                    For Each deEntry As DictionaryEntry In m_aryMicrocontrollers
                        doController = DirectCast(deEntry.Value, DataObjects.Physical.Microcontroller)
                        doController.SaveData(Util.Simulation, Me, oXml)
                    Next
                    oXml.OutOfElem()
                End If

            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try

        End Sub

        Public Overridable Overloads Sub SaveData(ByVal dsSim As Simulation, ByRef oXml As Interfaces.StdXml)

            Try
                oXml.AddChildElement("Structure")
                oXml.IntoElem()

                oXml.AddChildElement("ID", m_strID)
                oXml.AddChildElement("Name", m_strName)
                oXml.AddChildElement("Type", m_strStructureType)
                oXml.AddChildElement("Reference", m_bReference)

                If m_bReference Then
                    If m_doReference Is Nothing Then
                        Throw New System.Exception("Error saving the physical structure '" & Me.Name & "' because it is a reference type but its reference param is null.")
                    Else
                        oXml.AddChildElement("ReferenceID", m_doReference.ID)
                    End If
                End If

                If Util.ExportForStandAloneSim Then
                    If Not m_bReference Then
                        SaveBodyPlan(dsSim, oXml)
                    End If
                Else
                    oXml.AddChildElement("BodyPlan", Me.BodyPlanFile)
                End If

                Util.SaveVector(oXml, "Position", m_fwPosition)

                oXml.OutOfElem()

                If Not m_bReference Then
                    If Not Util.ExportForStandAloneSim Then
                        SaveBodyPlan(dsSim)
                    End If
                End If

            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try

        End Sub

        Public Overridable Sub CreateFiles()

            'Save the nervous system file
            Dim oXml As New Interfaces.StdXml

            If Util.Application.ProjectPath.Length > 0 Then
                If Not System.IO.File.Exists(Util.GetFilePath(Util.Application.ProjectPath, Me.BodyPlanFile)) Then
                    'Save the body plan file
                    oXml = New Interfaces.StdXml
                    oXml.AddElement("Structure")
                    oXml.Save(Util.GetFilePath(Util.Application.ProjectPath, Me.BodyPlanFile))
                End If
            End If

        End Sub

        Public Overridable Sub RenameFiles(ByVal strOriginalName As String)
            Dim strExtension As String, strNewFile As String

            If Not Me.IsReference AndAlso Util.Application.ProjectPath.Trim.Length > 0 AndAlso strOriginalName.Trim.Length > 0 Then
                Dim di As DirectoryInfo = New DirectoryInfo(Util.Application.ProjectPath)
                Dim fiFiles As FileInfo() = di.GetFiles(strOriginalName & ".*")

                For Each fiFile As FileInfo In fiFiles
                    strExtension = Util.GetFileExtension(fiFile.Name)
                    strNewFile = Util.GetFilePath(Util.Application.ProjectPath, (Me.Name & "." & strExtension))

                    fiFile.MoveTo(strNewFile)
                Next
            End If

        End Sub

        Public Overridable Sub RemoveFiles()

            If Not Me.IsReference AndAlso Util.Application.ProjectPath.Trim.Length > 0 Then
                Dim di As DirectoryInfo = New DirectoryInfo(Util.Application.ProjectPath)
                Dim fiFiles As FileInfo() = di.GetFiles(Me.Name & ".*")

                For Each fiFile As FileInfo In fiFiles
                    fiFile.Delete()
                Next
            End If

        End Sub

#End Region

        Protected Overridable Sub CheckForUniqueneName(ByVal strName As String)

            Dim doOrganism As DataObjects.Physical.PhysicalStructure
            If Not Me.Structures(Util.Application.Simulation) Is Nothing Then
                For Each deEntry As DictionaryEntry In Me.Structures(Util.Application.Simulation)
                    doOrganism = DirectCast(deEntry.Value, DataObjects.Physical.PhysicalStructure)

                    If Not doOrganism Is Me Then
                        If doOrganism.Name.Trim.ToUpper = strName.Trim.ToUpper Then
                            Throw New System.Exception("The name '" & doOrganism.Name & "' is already being used. Please choose a different name.")
                        End If
                    End If
                Next
            End If

        End Sub

        Protected Overridable Sub RenameWindowTitles()

            Try

                If Not Me.BodyEditor Is Nothing Then
                    Me.BodyEditor.Title = "Edit " & Me.Name

                    If Not Me.BodyPlanStructureNode Is Nothing Then
                        Me.BodyPlanStructureNode.Text = Me.Name
                    End If
                End If

            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try

        End Sub

        Public Overridable Sub ClearSelectedBodyParts()
            If Not m_dbRoot Is Nothing Then
                m_dbRoot.ClearSelectedBodyParts()
            End If
        End Sub

        Public Overrides Sub UnitsChanged(ByVal ePrevMass As AnimatTools.DataObjects.Physical.Environment.enumMassUnits, _
                                          ByVal eNewMass As AnimatTools.DataObjects.Physical.Environment.enumMassUnits, _
                                          ByVal fltMassChange As Single, _
                                          ByVal ePrevDistance As AnimatTools.DataObjects.Physical.Environment.enumDistanceUnits, _
                                          ByVal eNewDistance As AnimatTools.DataObjects.Physical.Environment.enumDistanceUnits, _
                                          ByVal fltDistanceChange As Single)
            If Not m_dbRoot Is Nothing Then
                m_dbRoot.UnitsChanged(ePrevMass, eNewMass, fltMassChange, ePrevDistance, eNewDistance, fltDistanceChange)
            End If
        End Sub

        Public Overridable Sub Delete()
            Me.OnDeleteStructure(Me, New System.EventArgs)
        End Sub

#End Region

#Region " Events "

        Protected Overridable Sub OnDeleteStructure(ByVal sender As Object, ByVal e As System.EventArgs)

            Try
                'Is this structure a reference for other structures?
                'If so then we can not delete it.
                Dim doStructure As DataObjects.Physical.PhysicalStructure
                If Not Me.Structures(Util.Application.Simulation) Is Nothing Then
                    For Each deEntry As DictionaryEntry In Me.Structures(Util.Application.Simulation)
                        doStructure = DirectCast(deEntry.Value, DataObjects.Physical.PhysicalStructure)
                        If Not doStructure Is Me AndAlso Not doStructure.Reference Is Nothing AndAlso doStructure.Reference Is Me Then
                            Throw New System.Exception("You can not delete a structure/organism that is being referenced. " & _
                                                        "Please remove all references to this item before attempting to delete it.")
                        End If
                    Next
                End If

                If MessageBox.Show("Are you certain that you want to permanently delete this " & _
                                    "structure\organism and all of its related files?", _
                                    "Delete Structure", MessageBoxButtons.YesNo) = DialogResult.Yes Then

                    'Lets see if there are any open windows for this organism/Structure
                    Dim frmBehavioral As Forms.Behavior.Editor
                    Dim frmBodyPlan As Forms.BodyPlan.Editor
                    For Each oChild As Form In Util.Application.MdiChildren
                        If TypeOf oChild Is Forms.Behavior.Editor Then
                            frmBehavioral = DirectCast(oChild, Forms.Behavior.Editor)
                            If frmBehavioral.Organism Is Me Then
                                frmBehavioral.Close()
                            End If
                        End If

                        If TypeOf oChild Is Forms.BodyPlan.Editor Then
                            frmBodyPlan = DirectCast(oChild, Forms.BodyPlan.Editor)
                            If frmBodyPlan.PhysicalStructure Is Me Then
                                frmBodyPlan.Close()
                            End If
                        End If
                    Next

                    'Now lets see if the camera is tracking this organism/structure. If it is then move it off it.
                    If Not Util.Environment.Camera.LinkedStructure Is Nothing AndAlso Not Util.Environment.Camera.LinkedStructure.PhysicalStructure Is Me Then
                        If Not Util.Environment.Camera.LinkedPart Is Nothing Then
                            Util.Environment.Camera.LinkedPart.BodyPart = Nothing
                        End If

                        Util.Environment.Camera.LinkedStructure.PhysicalStructure = Nothing
                        Util.Environment.Camera.TrackCamera = False
                    End If

                    If Not Me.Structures(Util.Application.Simulation) Is Nothing Then
                        Me.Structures(Util.Application.Simulation).Remove(Me.ID)
                    End If
                    Me.WorkspaceStructureNode.Remove()
                    m_wsStructureNode = Nothing
                    m_wsBodyPlanNode = Nothing
                    Me.RemoveFiles()

                    Util.Application.SaveProject(Util.Application.ProjectFile)
                End If

            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try

        End Sub

        Protected Overridable Sub OnCloneStructure(ByVal sender As Object, ByVal e As System.EventArgs)

            Try
                Dim newStructure As New DataObjects.Physical.PhysicalStructure(Me.Parent)
                newStructure.IsReference = False
                newStructure.Position = DirectCast(Me.Position.Clone(newStructure, False, Nothing), Vec3d)

                Util.Environment.NewStructureCount = Util.Environment.NewStructureCount + 1
                newStructure.Name = "Structure_" & Util.Environment.NewStructureCount
                newStructure.LoadBodyPlan(Util.Simulation)
                Util.Environment.Structures.Add(newStructure.ID, newStructure)

                newStructure.CreateWorkspaceTreeView(Util.Simulation, Util.Application.ProjectWorkspace)
                Util.Application.ProjectWorkspace.TreeView.SelectedNode = newStructure.WorkspaceStructureNode

                Util.Application.SaveProject(Util.Application.ProjectFile)

            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try

        End Sub

        Protected Overridable Sub OnCopyReference(ByVal sender As Object, ByVal e As System.EventArgs)

            Try
                Dim newStructure As New DataObjects.Physical.PhysicalStructure(Me.Parent)
                newStructure.IsReference = True
                newStructure.Reference = Me
                newStructure.Position = DirectCast(Me.Position.Clone(newStructure, False, Nothing), Vec3d)

                Util.Environment.NewStructureCount = Util.Environment.NewStructureCount + 1
                newStructure.Name = "Structure_" & Util.Environment.NewStructureCount
                newStructure.LoadBodyPlan(Util.Simulation)
                Util.Environment.Structures.Add(newStructure.ID, newStructure)

                newStructure.CreateWorkspaceTreeView(Util.Application.Simulation, Util.Application.ProjectWorkspace)
                Util.Application.ProjectWorkspace.TreeView.SelectedNode = newStructure.WorkspaceStructureNode

                Util.Application.SaveProject(Util.Application.ProjectFile)

            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try

        End Sub

#End Region

    End Class

End Namespace

