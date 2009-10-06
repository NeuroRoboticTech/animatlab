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

    Public Class Organism
        Inherits DataObjects.Physical.PhysicalStructure

#Region " Attributes "

        Protected m_frmBehaviorEditor As Forms.Behavior.Editor
        Protected m_nodeBehavioralSystem As TreeNode
        Protected m_aryBehavioralNodes As New Collections.AnimatSortedList(Me)
        Protected m_aryBehavioralLinks As New Collections.AnimatSortedList(Me)
        Protected m_aryNeuralModules As New Collections.SortedNeuralModules(Me)

#End Region

#Region " Properties "

        Public Overridable Property BehaviorEditor() As Forms.Behavior.Editor
            Get
                Return m_frmBehaviorEditor
            End Get
            Set(ByVal Value As Forms.Behavior.Editor)
                m_frmBehaviorEditor = Value
            End Set
        End Property

        Protected Overrides ReadOnly Property Structures(ByVal dsSim As AnimatTools.DataObjects.Simulation) As Collections.SortedStructures
            Get
                Return dsSim.Environment.Organisms
            End Get
        End Property

        Protected Overrides ReadOnly Property ParentTreeNode(ByVal dsSim As AnimatTools.DataObjects.Simulation) As TreeNode
            Get
                Return dsSim.Environment.OrganismsTreeNode
            End Get
        End Property

        Public Overridable ReadOnly Property BehavioralSystemFile() As String
            Get
                If Not Me.IsReference Then
                    Return Me.Name & ".absys"
                Else
                    Return Me.ReferenceName & ".absys"
                End If
            End Get
        End Property

        Public Overridable ReadOnly Property BehavioralEditorFile() As String
            Get
                If Not Me.IsReference Then
                    Return Me.Name & ".abef"
                Else
                    Return Me.ReferenceName & ".abef"
                End If
            End Get
        End Property

        Public Overridable ReadOnly Property BehavioralSystemTreeNode() As TreeNode
            Get
                Return m_nodeBehavioralSystem
            End Get
        End Property

        Public Overridable ReadOnly Property BehavioralNodes() As Collections.AnimatSortedList
            Get
                Return m_aryBehavioralNodes
            End Get
        End Property

        Public Overridable ReadOnly Property BehavioralLinks() As Collections.AnimatSortedList
            Get
                Return m_aryBehavioralLinks
            End Get
        End Property

        Public Overridable ReadOnly Property NeuralModules() As Collections.SortedNeuralModules
            Get
                Return m_aryNeuralModules
            End Get
        End Property

        Public Overrides ReadOnly Property ImageName() As String
            Get
                Return "AnimatTools.Organism.gif"
            End Get
        End Property

        <Browsable(False)> _
        Public Overrides ReadOnly Property RootForm() As System.Windows.Forms.Form
            Get
                If Not m_frmBehaviorEditor Is Nothing AndAlso Util.Application.ActiveMdiChild Is m_frmBehaviorEditor Then
                    Return m_frmBehaviorEditor
                ElseIf Not m_frmBodyEditor Is Nothing AndAlso Util.Application.ActiveMdiChild Is m_frmBodyEditor Then
                    Return m_frmBodyEditor
                Else
                    Return Util.Application
                End If
            End Get
        End Property


#End Region

#Region " Methods "

        Public Sub New(ByVal doParent As Framework.DataObject)
            MyBase.New(doParent)

            Dim nmModule As DataObjects.Behavior.NeuralModule
            Dim nmNewModule As DataObjects.Behavior.NeuralModule
            For Each deItem As DictionaryEntry In Util.Application.NeuralModules
                nmModule = DirectCast(deItem.Value, DataObjects.Behavior.NeuralModule)
                nmNewModule = DirectCast(nmModule.Clone(nmModule.Parent, False, Nothing), AnimatTools.DataObjects.Behavior.NeuralModule)
                nmNewModule.Organism = Me
                nmNewModule.Parent = Me
                m_aryNeuralModules.Add(nmModule.GetType().FullName, nmNewModule)
            Next

        End Sub

        Protected Overrides Sub BuildProperties()

            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Name", m_strName.GetType(), "Name", _
                                        "Organism Properties", "The name for this organism. ", m_strName))

            If m_bReference AndAlso Not Me.Reference Is Nothing Then
                m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Reference", GetType(String), "ReferenceName", _
                                            "Organism Properties", "The name of the organism that this one references. ", Me.ReferenceName, True))
            End If

            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Body Plan", Me.BodyPlanFile.GetType(), "BodyPlanFile", _
                                        "Organism Properties", "Specifies the body plan file.", Me.BodyPlanFile, True))

            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Behavioral System", Me.BehavioralSystemFile.GetType(), "BehavioralSystemFile", _
                                        "Organism Properties", "Specifies the behavioral system file.", Me.BehavioralSystemFile, True))

            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Behavioral Editor", Me.BehavioralEditorFile.GetType(), "BehavioralEditorFile", _
                                        "Organism Properties", "Specifies the behavioral editor file.", Me.BehavioralEditorFile, True))

            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Microcontrollers", m_aryMicrocontrollers.GetType(), "Microcontrollers", _
                                        "Organism Properties", "Edit the microcontroller list for robotics applications.", m_aryMicrocontrollers, _
                                        GetType(TypeHelpers.MicrocontrollerTypeEditor), GetType(TypeHelpers.MicrocontrollerTypeConverter)))

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
                                        "Organism Properties", "Pairs of body parts that should be excluded from collision detection between each other.", m_aryCollisionExclusionPairs, _
                                        GetType(TypeHelpers.CollisionPairsTypeEditor), GetType(TypeHelpers.CollisionPairsTypeConverter)))

            If Not Me.BodyEditor Is Nothing AndAlso Not Me.BodyEditor.BodyView Is Nothing Then
                m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("BackColor", Me.BackColor.GetType, "BackColor", _
                                        "Graphical Properties", "Sets the color used to draw the background for this item in the body editor window.", Me.BackColor))
            End If

        End Sub

#Region " Workspace TreeView "

        Public Overrides Sub CreateWorkspaceTreeView(ByVal dsSim As AnimatTools.DataObjects.Simulation, _
                                                     ByVal frmWorkspace As Forms.ProjectWorkspace)

            m_wsStructureNode = Me.ParentTreeNode(dsSim).Nodes.Add(Me.Name)
            m_wsStructureNode.ImageIndex = frmWorkspace.ImageManager.GetImageIndex(Me.ImageName)
            m_wsStructureNode.SelectedImageIndex = frmWorkspace.ImageManager.GetImageIndex(Me.ImageName)
            m_wsStructureNode.Tag = Me
            If Me.IsReference Then
                m_wsStructureNode.ForeColor = Color.LightSteelBlue
            Else
                m_wsBodyPlanNode = m_wsStructureNode.Nodes.Add("Body Plan")
                m_wsBodyPlanNode.ImageIndex = frmWorkspace.ImageManager.GetImageIndex("AnimatTools.Joint.gif")
                m_wsBodyPlanNode.SelectedImageIndex = frmWorkspace.ImageManager.GetImageIndex("AnimatTools.Joint.gif")

                m_nodeBehavioralSystem = m_wsStructureNode.Nodes.Add("Behavioral System")
                m_nodeBehavioralSystem.ImageIndex = frmWorkspace.ImageManager.GetImageIndex("AnimatTools.Neuron.gif")
                m_nodeBehavioralSystem.SelectedImageIndex = frmWorkspace.ImageManager.GetImageIndex("AnimatTools.Neuron.gif")
            End If

        End Sub

        Public Overrides Function WorkspaceTreeviewPopupMenu(ByRef tnSelectedNode As TreeNode, ByVal ptPoint As Point) As Boolean

            If tnSelectedNode Is m_wsStructureNode Then
                ' Create the menu items
                Dim mcDelete As New MenuCommand("Delete Organism ", "DeleteOrganism", Util.Application.SmallImages.ImageList, _
                                                  Util.Application.SmallImages.GetImageIndex("AnimatTools.Delete.gif"), _
                                                  New EventHandler(AddressOf Me.OnDeleteStructure))

                ' Create the popup menu object
                Dim popup As New PopupMenu

                If Not Me.IsReference Then
                    Dim mcClone As New MenuCommand("Clone Organism", "CloneOrganism", _
                                                      New EventHandler(AddressOf Me.OnCloneStructure))
                    Dim mcCopy As New MenuCommand("Copy Reference", "CopyReference", _
                                                      New EventHandler(AddressOf Me.OnCopyReference))
                    ' Define the list of menu commands
                    popup.MenuCommands.AddRange(New MenuCommand() {mcDelete, mcClone, mcCopy})
                Else
                    popup.MenuCommands.Add(mcDelete)
                End If

                If m_aryMicrocontrollers.Count > 0 Then
                    Dim mcRobot As New MenuCommand("Download to Robot", "DownloadToRobot", _
                                                      New EventHandler(AddressOf Me.OnDownloadToRobot))
                    popup.MenuCommands.Add(mcRobot)
                End If

                ' Show it!
                Dim selected As MenuCommand = popup.TrackPopup(ptPoint)

                Return True
            ElseIf tnSelectedNode Is m_wsBodyPlanNode OrElse tnSelectedNode Is m_nodeBehavioralSystem Then
                Return True
            End If

            Return False
        End Function

        Public Overrides Function WorkspaceTreeviewDoubleClick(ByRef tnSelectedNode As TreeNode) As Boolean

            If tnSelectedNode Is m_wsBodyPlanNode OrElse tnSelectedNode Is m_nodeBehavioralSystem Then
                Dim doRef As Organism = Me
                If Me.IsReference Then
                    If Me.Reference Is Nothing Then
                        Throw New System.Exception("The organism '" & Me.Name & "' is a reference type but its reference param is null.")
                    Else
                        doRef = DirectCast(Me.Reference, Organism)
                    End If
                End If

                If tnSelectedNode Is m_wsBodyPlanNode Then
                    Util.Application.EditBodyPlan(doRef)
                    Return True
                ElseIf tnSelectedNode Is m_nodeBehavioralSystem Then
                    Util.Application.EditBehavioralSystem(doRef)
                    Return True
                End If
            End If

            Return False
        End Function

#End Region

#Region " Body Plan TreeView "

        Public Overrides Sub CreateBodyPlanTreeView(ByVal dsSim As AnimatTools.DataObjects.Simulation, _
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

            frmDataItem.ImageManager.AddImage(myAssembly, "AnimatTools.Neuron.gif")
            frmDataItem.ImageManager.AddImage(myAssembly, "AnimatTools.Joint.gif")
            frmDataItem.ImageManager.AddImage(myAssembly, "AnimatTools.DefaultObject.gif")
            frmDataItem.ImageManager.AddImage(myAssembly, "AnimatTools.DefaultLink.gif")

            Dim tnBodyplanNode As TreeNode
            tnBodyplanNode = tnNode.Nodes.Add("Body Plan")
            tnBodyplanNode.ImageIndex = frmDataItem.ImageManager.GetImageIndex("AnimatTools.Joint.gif")
            tnBodyplanNode.SelectedImageIndex = frmDataItem.ImageManager.GetImageIndex("AnimatTools.Joint.gif")

            If Not m_dbRoot Is Nothing Then
                m_dbRoot.CreateDataItemTreeView(frmDataItem, tnBodyplanNode, tpTemplatePartType)
            End If

            Dim tnBehavioralNode As TreeNode
            tnBehavioralNode = tnNode.Nodes.Add("Behavioral System")
            tnBehavioralNode.ImageIndex = frmDataItem.ImageManager.GetImageIndex("AnimatTools.Neuron.gif")
            tnBehavioralNode.SelectedImageIndex = frmDataItem.ImageManager.GetImageIndex("AnimatTools.Neuron.gif")

            Dim tnNodes As TreeNode
            tnNodes = tnBehavioralNode.Nodes.Add("Nodes")
            tnNodes.ImageIndex = frmDataItem.ImageManager.GetImageIndex("AnimatTools.DefaultObject.gif")
            tnNodes.SelectedImageIndex = frmDataItem.ImageManager.GetImageIndex("AnimatTools.DefaultObject.gif")

            Dim tnLinks As TreeNode
            tnLinks = tnBehavioralNode.Nodes.Add("Links")
            tnLinks.ImageIndex = frmDataItem.ImageManager.GetImageIndex("AnimatTools.DefaultLink.gif")
            tnLinks.SelectedImageIndex = frmDataItem.ImageManager.GetImageIndex("AnimatTools.DefaultLink.gif")

            Dim doData As DataObjects.Behavior.Data
            For Each deEntry As DictionaryEntry In m_aryBehavioralNodes
                doData = DirectCast(deEntry.Value, DataObjects.Behavior.Data)
                If doData.CanBeCharted Then
                    doData.CreateDataItemTreeView(frmDataItem, tnNodes, tpTemplatePartType)
                End If
            Next

            For Each deEntry As DictionaryEntry In m_aryBehavioralLinks
                doData = DirectCast(deEntry.Value, DataObjects.Behavior.Data)
                If doData.CanBeCharted Then
                    doData.CreateDataItemTreeView(frmDataItem, tnLinks, tpTemplatePartType)
                End If
            Next

        End Function

#End Region

#Region " Find Methods "

        Public Overridable Function FindBehavioralNode(ByVal strID As String, Optional ByVal bThrowError As Boolean = True) As AnimatTools.DataObjects.Behavior.Node
            Dim oNode As Object = m_aryBehavioralNodes(strID)
            If oNode Is Nothing Then
                If bThrowError Then Throw New System.Exception("No node was found with the following id. ID: " & strID)
            Else
                Return DirectCast(oNode, AnimatTools.DataObjects.Behavior.Node)
            End If
        End Function

        Public Overridable Function FindBehavioralNodeByName(ByVal strName As String, Optional ByVal bThrowError As Boolean = True) As AnimatTools.DataObjects.Behavior.Node

            Dim doNode As DataObjects.Behavior.Node
            For Each deEntry As DictionaryEntry In m_aryBehavioralNodes
                doNode = DirectCast(deEntry.Value, DataObjects.Behavior.Node)

                If doNode.Name = strName Then
                    Return doNode
                End If
            Next

            If doNode Is Nothing AndAlso bThrowError Then
                If bThrowError Then Throw New System.Exception("No node was found with the following name: " & strName)
            Else
                Return Nothing
            End If
        End Function

        Public Overridable Function FindBehavioralLink(ByVal strID As String, Optional ByVal bThrowError As Boolean = True) As AnimatTools.DataObjects.Behavior.Link
            Dim oLink As Object = m_aryBehavioralLinks(strID)
            If oLink Is Nothing Then
                If bThrowError Then Throw New System.Exception("No link was found with the following id. ID: " & strID)
            Else
                Return DirectCast(oLink, AnimatTools.DataObjects.Behavior.Link)
            End If
        End Function

        Public Overrides Sub FindChildrenOfType(ByVal tpTemplate As System.Type, ByRef colDataObjects As Collections.DataObjects)
            If Not m_dbRoot Is Nothing Then
                m_dbRoot.FindChildrenOfType(tpTemplate, colDataObjects)
            End If

            If tpTemplate Is Nothing OrElse Util.IsTypeOf(tpTemplate, GetType(AnimatTools.DataObjects.Behavior.Data), False) Then
                Dim doData As AnimatTools.DataObjects.Behavior.Data
                For Each deEntry As DictionaryEntry In Me.BehavioralNodes
                    doData = DirectCast(deEntry.Value, AnimatTools.DataObjects.Behavior.Data)
                    doData.FindChildrenOfType(tpTemplate, colDataObjects)
                Next

                For Each deEntry As DictionaryEntry In Me.BehavioralLinks
                    doData = DirectCast(deEntry.Value, AnimatTools.DataObjects.Behavior.Data)
                    doData.FindChildrenOfType(tpTemplate, colDataObjects)
                Next
            End If

        End Sub

#End Region

#Region " Load/Save Methods "

        Protected Overridable Sub LoadBehavioralSystem(ByVal dsSim As Simulation)
            Dim oXml As New AnimatTools.Interfaces.StdXml

            m_aryBehavioralNodes.Clear()
            m_aryBehavioralLinks.Clear()

            If System.IO.File.Exists(Util.GetFilePath(Util.Application.ProjectPath, Me.BehavioralEditorFile)) Then
                oXml.Load(Util.GetFilePath(Util.Application.ProjectPath, Me.BehavioralEditorFile))

                oXml.FindElement("Editor")
                oXml.IntoChildElement("Diagrams")

                Dim iCount As Integer = oXml.NumberOfChildren() - 1
                For iIndex As Integer = 0 To iCount
                    oXml.FindChildByIndex(iIndex)
                    LoadDiagram(dsSim, oXml)
                Next

                oXml.OutOfElem()  'Outof Diagrams Element

                LoadNeuralModules(dsSim, oXml)
            End If

            AnimatTools.Forms.Behavior.Diagram.InitializeDataAfterLoad(m_aryBehavioralNodes)
            AnimatTools.Forms.Behavior.Diagram.InitializeDataAfterLoad(m_aryBehavioralLinks)

        End Sub

        Protected Overridable Sub LoadDiagram(ByVal dsSim As Simulation, ByRef oXml As AnimatTools.Interfaces.StdXml)
            'Dim strDiagramName As String
            'Dim strNodeName As String

            Try

                Dim strAssemblyFile As String
                Dim strClassName As String

                oXml.IntoElem()

                'strDiagramName = oXml.GetChildString("PageName")

                'If strDiagramName = "Left Upper Leg" Then
                '    strDiagramName = "Left Upper Leg"
                'End If

                oXml.IntoChildElement("Nodes")
                Dim iCount As Integer = oXml.NumberOfChildren() - 1
                Dim bnNode As AnimatTools.DataObjects.Behavior.Node
                For iIndex As Integer = 0 To iCount
                    oXml.FindChildByIndex(iIndex)
                    oXml.IntoElem() 'Into Node element
                    strAssemblyFile = oXml.GetChildString("AssemblyFile")
                    strClassName = oXml.GetChildString("ClassName")
                    'strNodeName = oXml.GetChildString("Text")
                    oXml.OutOfElem() 'Outof Node element

                    If iIndex = 12 Then
                        iIndex = 12
                    End If

                    bnNode = DirectCast(Util.LoadClass(strAssemblyFile, strClassName, Me), AnimatTools.DataObjects.Behavior.Node)
                    bnNode.Organism = Me
                    bnNode.LoadData(oXml)
                    Me.BehavioralNodes.Add(bnNode.ID, bnNode)
                Next
                oXml.OutOfElem() 'Outof Nodes Element

                oXml.IntoChildElement("Links")
                iCount = oXml.NumberOfChildren() - 1
                Dim blLink As AnimatTools.DataObjects.Behavior.Link
                For iIndex As Integer = 0 To iCount
                    oXml.FindChildByIndex(iIndex)
                    oXml.IntoElem() 'Into Node element
                    strAssemblyFile = oXml.GetChildString("AssemblyFile")
                    strClassName = oXml.GetChildString("ClassName")
                    oXml.OutOfElem() 'Outof Node element

                    blLink = DirectCast(Util.LoadClass(strAssemblyFile, strClassName, Me), AnimatTools.DataObjects.Behavior.Link)
                    blLink.Organism = Me
                    blLink.LoadData(oXml)
                    Me.BehavioralLinks.Add(blLink.ID, blLink)
                Next
                oXml.OutOfElem() 'Outof Links Element

                oXml.IntoChildElement("Diagrams")
                Dim bdDiagram As AnimatTools.Forms.Behavior.Diagram
                iCount = oXml.NumberOfChildren() - 1
                For iIndex As Integer = 0 To iCount
                    oXml.FindChildByIndex(iIndex)
                    LoadDiagram(dsSim, oXml)
                Next
                oXml.OutOfElem() ' OutOf the Diagrams Element

                oXml.OutOfElem()  'Outof Diagram Element

            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)

                'If we get an error here then it most likely happened during one of our "modules"
                'So if we want to be able to continue loading then we need to jump out of the module in the xml file.
                oXml.OutOfElem() ' OutOf the sub node where the error will occur
                oXml.OutOfElem()  'Outof Diagram Element
            End Try

        End Sub

        Public Overridable Sub LoadNeuralModules(ByVal dsSim As Simulation, ByRef oXml As AnimatTools.Interfaces.StdXml)

            Try
                Dim strAssemblyFile As String
                Dim strClassName As String
                Dim oMod As Object

                m_aryNeuralModules.Clear()

                oXml.IntoChildElement("NeuralModules")
                Dim iCount As Integer = oXml.NumberOfChildren() - 1
                Dim nmModule As AnimatTools.DataObjects.Behavior.NeuralModule
                For iIndex As Integer = 0 To iCount
                    oXml.FindChildByIndex(iIndex)
                    oXml.IntoElem() 'Into Diagram element
                    strAssemblyFile = oXml.GetChildString("AssemblyFile")
                    strClassName = oXml.GetChildString("ClassName")
                    oXml.OutOfElem() 'Outof Diagram element

                    'If the module cannot be found then do not die because of this, just keep trying to go on.
                    oMod = Util.LoadClass(strAssemblyFile, strClassName, Me, False)
                    If Not oMod Is Nothing Then
                        nmModule = DirectCast(oMod, AnimatTools.DataObjects.Behavior.NeuralModule)
                        nmModule.Organism = Me
                        nmModule.LoadData(oXml)
                        m_aryNeuralModules.Add(nmModule.GetType().FullName, nmModule)
                    End If
                Next
                oXml.OutOfElem() 'Outof NeuralModules Element

                'Now lets go through and see if there are any neural modules found in module init that
                'we have not saved for this organism. If so then we need to add them so the user has
                'access to them.
                Dim nmNewModule As DataObjects.Behavior.NeuralModule
                For Each deItem As DictionaryEntry In Util.Application.NeuralModules
                    nmModule = DirectCast(deItem.Value, DataObjects.Behavior.NeuralModule)
                    If Not m_aryNeuralModules.Contains(nmModule.GetType.FullName) Then
                        nmNewModule = DirectCast(nmModule.Clone(nmModule.Parent, False, Nothing), AnimatTools.DataObjects.Behavior.NeuralModule)
                        nmNewModule.Organism = Me
                        nmNewModule.Parent = Me
                        m_aryNeuralModules.Add(nmModule.GetType().FullName, nmModule)
                    End If
                Next

            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try

        End Sub

        Public Overridable Overloads Sub LoadData(ByVal dsSim As Simulation, ByRef oXml As Interfaces.StdXml)

            Try
                MyBase.LoadData(dsSim, oXml)

                LoadBehavioralSystem(dsSim)
            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try

        End Sub

        Public Overridable Overloads Sub SaveData(ByVal dsSim As Simulation, ByRef oXml As Interfaces.StdXml)
            MyBase.SaveData(dsSim, oXml)

            oXml.IntoElem()
            If Util.ExportForStandAloneSim Then
                SaveNetworkFile(oXml)
            Else
                oXml.AddChildElement("BehavioralSystem", Me.BehavioralSystemFile)
            End If
            oXml.OutOfElem()

            If Not Util.ExportForStandAloneSim Then
                SaveNetworkFile(Me.BehavioralSystemFile)
            End If

        End Sub

        Protected Overridable Overloads Sub SaveNetworkFile(ByVal strFilename As String)

            Try
                Dim oXml As New AnimatTools.Interfaces.StdXml

                SaveNetworkFile(oXml)
                oXml.Save(Util.GetFilePath(Util.Application.ProjectPath, strFilename))

            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try
        End Sub

        Protected Overridable Overloads Sub SaveNetworkFile(ByRef oXml As AnimatTools.Interfaces.StdXml)

            Try

                If Util.ExportForStandAloneSim Then
                    oXml.AddChildElement("NervousSystem")
                    oXml.IntoElem()
                Else
                    oXml.AddElement("NervousSystem")
                End If

                oXml.AddChildElement("NeuralModules")
                oXml.IntoElem()
                Dim nmModule As DataObjects.Behavior.NeuralModule
                Dim nmPhysicsModule As DataObjects.Behavior.NeuralModule
                For Each deEntry As DictionaryEntry In m_aryNeuralModules
                    nmModule = DirectCast(deEntry.Value, DataObjects.Behavior.NeuralModule)

                    If Not nmModule.GetType() Is GetType(DataObjects.Behavior.PhysicsModule) Then
                        nmModule.SaveNetwork(oXml)
                    Else
                        nmPhysicsModule = nmModule
                    End If
                Next
                oXml.OutOfElem() 'Outof NeuralModules

                oXml.AddChildElement("Adapters")
                If Not nmPhysicsModule Is Nothing Then
                    nmPhysicsModule.SaveNetwork(oXml)
                End If

                If Util.ExportForStandAloneSim Then
                    oXml.OutOfElem() 'Outof Nervous System
                End If

            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try
        End Sub

        Public Overrides Sub CreateFiles()

            'Save the nervous system file
            Dim oXml As New Interfaces.StdXml

            If Util.Application.ProjectPath.Length > 0 Then
                If Not System.IO.File.Exists(Util.GetFilePath(Util.Application.ProjectPath, Me.BehavioralSystemFile)) Then
                    oXml.AddElement("NervousSystem")
                    oXml.AddChildElement("NeuralModules")
                    oXml.AddChildElement("Adapters")

                    oXml.Save(Util.GetFilePath(Util.Application.ProjectPath, Me.BehavioralSystemFile))
                End If

                If Not System.IO.File.Exists(Util.GetFilePath(Util.Application.ProjectPath, Me.BodyPlanFile)) Then
                    'Save the body plan file
                    oXml = New Interfaces.StdXml
                    oXml.AddElement("Structure")
                    oXml.Save(Util.GetFilePath(Util.Application.ProjectPath, Me.BodyPlanFile))
                End If
            End If

        End Sub

#End Region

        Protected Overrides Sub CloneInternal(ByVal doOriginal As AnimatTools.Framework.DataObject, ByVal bCutData As Boolean, _
                                            ByVal doRoot As AnimatTools.Framework.DataObject)
            MyBase.CloneInternal(doOriginal, bCutData, doRoot)

            Dim doOrganism As Organism = DirectCast(doOriginal, Organism)

            m_frmBehaviorEditor = doOrganism.m_frmBehaviorEditor
            m_aryBehavioralNodes = DirectCast(doOrganism.m_aryBehavioralNodes.Clone(), AnimatTools.Collections.AnimatSortedList)
            m_aryBehavioralLinks = DirectCast(doOrganism.m_aryBehavioralLinks.Clone(), AnimatTools.Collections.AnimatSortedList)
            m_aryNeuralModules = DirectCast(doOrganism.m_aryNeuralModules.Clone(Me, bCutData, doRoot), AnimatTools.Collections.SortedNeuralModules)

        End Sub

        Public Overrides Function Clone(ByVal doParent As AnimatTools.Framework.DataObject, ByVal bCutData As Boolean, _
                                        ByVal doRoot As AnimatTools.Framework.DataObject) As AnimatTools.Framework.DataObject
            Dim doItem As New Organism(doParent)
            doItem.CloneInternal(Me, bCutData, doRoot)
            If Not doRoot Is Nothing AndAlso doRoot Is Me Then doItem.AfterClone(Me, bCutData, doRoot, doItem)
            Return doItem
        End Function

        Public Overrides Sub UnitsChanged(ByVal ePrevMass As AnimatTools.DataObjects.Physical.Environment.enumMassUnits, _
                                          ByVal eNewMass As AnimatTools.DataObjects.Physical.Environment.enumMassUnits, _
                                          ByVal fltMassChange As Single, _
                                          ByVal ePrevDistance As AnimatTools.DataObjects.Physical.Environment.enumDistanceUnits, _
                                          ByVal eNewDistance As AnimatTools.DataObjects.Physical.Environment.enumDistanceUnits, _
                                          ByVal fltDistanceChange As Single)
            MyBase.UnitsChanged(ePrevMass, eNewMass, fltMassChange, ePrevDistance, eNewDistance, fltDistanceChange)

            Dim doData As Behavior.Data
            For Each deEntry As DictionaryEntry In m_aryBehavioralNodes
                doData = DirectCast(deEntry.Value, Behavior.Data)
                doData.UnitsChanged(ePrevMass, eNewMass, fltMassChange, ePrevDistance, eNewDistance, fltDistanceChange)
            Next

            For Each deEntry As DictionaryEntry In m_aryBehavioralLinks
                doData = DirectCast(deEntry.Value, Behavior.Data)
                doData.UnitsChanged(ePrevMass, eNewMass, fltMassChange, ePrevDistance, eNewDistance, fltDistanceChange)
            Next

        End Sub

        Public Overridable Sub AddContactAdapters(ByVal nmPhysicsModule As DataObjects.Behavior.NeuralModule, ByVal m_aryNodes As Collections.SortedNodes)
            If Not m_dbRoot Is Nothing Then
                m_dbRoot.AddContactAdapters(nmPhysicsModule, m_aryNodes)
            End If
        End Sub

        Protected Overrides Sub RenameWindowTitles()

            Try

                MyBase.RenameWindowTitles()

                If Not Me.BehaviorEditor Is Nothing Then
                    Me.BehaviorEditor.Title = "Edit " & Me.Name
                End If

            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try

        End Sub

#End Region

#Region " Events "

        Protected Overrides Sub OnCloneStructure(ByVal sender As Object, ByVal e As System.EventArgs)

            Try
                Dim newOrganism As New DataObjects.Physical.Organism(Me)
                newOrganism.IsReference = False
                newOrganism = DirectCast(Me.Clone(Me.Parent, False, Nothing), DataObjects.Physical.Organism)
                Util.Simulation.Environment.Organisms.Add(newOrganism.ID, newOrganism)

                Util.Environment.NewOrganismCount = Util.Environment.NewOrganismCount + 1
                newOrganism.Name = "Organism_" & Util.Environment.NewOrganismCount
                'newOrganism.LoadBodyPlan(Util.Simulation)

                newOrganism.CreateWorkspaceTreeView(Util.Simulation, Util.Application.ProjectWorkspace)
                newOrganism.WorkspaceStructureNode.ExpandAll()
                Util.Application.ProjectWorkspace.TreeView.SelectedNode = newOrganism.WorkspaceStructureNode
                newOrganism.CreateFiles()

                Util.Application.SaveProject(Util.Application.ProjectFile)

                'If this is the first organism then lets set the camer to track it.
                If Util.Simulation.Environment.Organisms.Count = 1 Then
                    Util.Environment.Camera.AutoTrack(newOrganism)
                End If

            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try

        End Sub

        Protected Overrides Sub OnCopyReference(ByVal sender As Object, ByVal e As System.EventArgs)

            Try
                Dim newOrganism As New DataObjects.Physical.Organism(Me)
                newOrganism.IsReference = True
                newOrganism.Reference = Me
                newOrganism.Position = DirectCast(Me.Position.Clone(newOrganism, False, Nothing), Vec3d)

                Util.Environment.NewOrganismCount = Util.Environment.NewOrganismCount + 1
                newOrganism.Name = "Organism_" & Util.Environment.NewOrganismCount
                newOrganism.LoadBodyPlan(Util.Simulation)

                newOrganism.CreateWorkspaceTreeView(Util.Application.Simulation, Util.Application.ProjectWorkspace)
                Util.Application.ProjectWorkspace.TreeView.SelectedNode = newOrganism.WorkspaceStructureNode

                Util.Application.SaveProject(Util.Application.ProjectFile)

            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try

        End Sub

        Protected Overridable Sub OnDownloadToRobot(ByVal sender As Object, ByVal e As System.EventArgs)

            Try
                Util.Application.SaveRobotDownload(Me)
            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try

        End Sub

#End Region

    End Class

End Namespace

