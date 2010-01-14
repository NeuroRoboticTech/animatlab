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

    Public Class Environment
        Inherits Framework.DataObject

#Region " Enums "

        Public Enum enumDistanceUnits
            Kilometers = 3
            Centameters = 2
            Decameters = 1
            Meters = 0
            Decimeters = -1
            Centimeters = -2
            Millimeters = -3
        End Enum

        Public Enum enumMassUnits
            Kilograms = 3
            Centagrams = 2
            Decagrams = 1
            Grams = 0
            Decigrams = -1
            Centigrams = -2
            Milligrams = -3
        End Enum

#End Region

#Region " Attributes "

        Protected m_snPhysicsTimeStep As ScaledNumber
        Protected m_snGravity As ScaledNumber
        Protected m_snMaxSurfaceFriction As ScaledNumber
        Protected m_snFluidDensity As ScaledNumber

        Protected m_snMouseSpringStiffness As ScaledNumber
        Protected m_snMouseSpringDamping As ScaledNumber

        Protected m_snLinearCompliance As ScaledNumber
        Protected m_snLinearDamping As ScaledNumber
        Protected m_snAngularCompliance As ScaledNumber
        Protected m_snAngularDamping As ScaledNumber
        Protected m_snLinearKineticLoss As ScaledNumber
        Protected m_snAngularKineticLoss As ScaledNumber

        Protected m_snMaxHydroForce As ScaledNumber
        Protected m_snMaxHydroTorque As ScaledNumber

        Protected m_eDistanceUnits As enumDistanceUnits = enumDistanceUnits.Decimeters
        Protected m_eMassUnits As enumMassUnits = enumMassUnits.Grams
        Protected m_bSimulateHydrodynamics As Boolean = False
        Protected m_fltPlaybackRate As Single = 1
        Protected m_bUseAlphaBlending As Boolean = False

        Protected m_iNewOrganismCount As Integer
        Protected m_iNewStructureCount As Integer

        Protected m_aryOrganisms As New Collections.SortedStructures(Me)
        Protected m_aryStructures As New Collections.SortedStructures(Me)
        Protected m_aryOdorTypes As New Collections.SortedOdorTypes(Me)

        Protected m_doGround As DataObjects.Physical.GroundSurface
        Protected m_doWater As DataObjects.Physical.WaterSurface

        Protected m_doCamera As New DataObjects.Physical.Camera(Me)

        Protected m_nodeOrganisms As TreeNode
        Protected m_wsStructureNodes As TreeNode
        Protected m_nodeEnvironment As TreeNode

        Protected m_bAutoGenerateRandomSeed As Boolean = True
        Protected m_iManualRandomSeed As Integer = 12345

#End Region

#Region " Properties "


        Public Property PhysicsTimeStep() As ScaledNumber
            Get
                Return m_snPhysicsTimeStep
            End Get
            Set(ByVal Value As ScaledNumber)
                If Value.ActualValue <= 0 Then
                    Throw New System.Exception("You can not set the physics time step value to be less than or equal to zero!")
                End If

                m_snPhysicsTimeStep.CopyData(Value)
            End Set
        End Property

        Public Property Gravity() As ScaledNumber
            Get
                Return m_snGravity
            End Get
            Set(ByVal Value As ScaledNumber)
                m_snGravity.CopyData(Value)
            End Set
        End Property

        Public Property MaxSurfaceFriction() As ScaledNumber
            Get
                Return m_snMaxSurfaceFriction
            End Get
            Set(ByVal Value As ScaledNumber)
                If Value.ActualValue < 0 Then
                    Throw New System.Exception("The surface friction must be greater than or equal to 0.")
                End If

                m_snMaxSurfaceFriction.CopyData(Value)
            End Set
        End Property

        Public Overridable Property PlaybackRate() As Single
            Get
                Return m_fltPlaybackRate
            End Get
            Set(ByVal Value As Single)
                If Value < 0 Then
                    Throw New System.Exception("The playback rate must be greater than zero.")
                End If

                m_fltPlaybackRate = Value

                'If the simulation is active then lets update the playbackrate
                If Not Util.Application.SimulationView Is Nothing AndAlso Not Util.Application.SimulationView.Simulator Is Nothing _
                   AndAlso Util.Application.SimulationView.Simulator.Loaded Then
                    Util.Application.SimulationView.Simulator.PlaybackRate = Value
                End If
            End Set
        End Property

        Public Overridable Property UseAlphaBlending() As Boolean
            Get
                Return m_bUseAlphaBlending
            End Get
            Set(ByVal Value As Boolean)
                m_bUseAlphaBlending = Value

                'If the simulation is active then lets update the playbackrate
                If Not Util.Application.SimulationView Is Nothing AndAlso Not Util.Application.SimulationView.Simulator Is Nothing _
                   AndAlso Util.Application.SimulationView.Simulator.Loaded Then
                    Util.Application.SimulationView.Simulator.UseAlphaBlending = Value
                End If
            End Set
        End Property

        Public Property SimulateHydrodynamics() As Boolean
            Get
                Return m_bSimulateHydrodynamics
            End Get
            Set(ByVal Value As Boolean)
                m_bSimulateHydrodynamics = Value
            End Set
        End Property

        Public Property FluidDensity() As ScaledNumber
            Get
                Return m_snFluidDensity
            End Get
            Set(ByVal Value As ScaledNumber)
                If Value.ActualValue < 0 Then
                    Throw New System.Exception("You can not set the fluid density value to be less than zero!")
                End If

                m_snFluidDensity.CopyData(Value)
            End Set
        End Property

        Public Property MouseSpringStiffness() As ScaledNumber
            Get
                Return m_snMouseSpringStiffness
            End Get
            Set(ByVal Value As ScaledNumber)
                If Value.ActualValue < 0 Then
                    Throw New System.Exception("You can not set the mouse spring stiffness to be less than zero!")
                End If

                m_snMouseSpringStiffness.CopyData(Value)
            End Set
        End Property

        Public Property MouseSpringDamping() As ScaledNumber
            Get
                Return m_snMouseSpringDamping
            End Get
            Set(ByVal Value As ScaledNumber)
                If Value.ActualValue < 0 Then
                    Throw New System.Exception("You can not set the mouse spring stiffness to be less than zero!")
                End If

                m_snMouseSpringDamping.CopyData(Value)
            End Set
        End Property

        Public Property LinearCompliance() As ScaledNumber
            Get
                Return m_snLinearCompliance
            End Get
            Set(ByVal Value As ScaledNumber)
                If Value.ActualValue < 0 Then
                    Throw New System.Exception("You can not set the linear compliance to be less than zero!")
                End If

                m_snLinearCompliance.CopyData(Value)
            End Set
        End Property

        Public Property LinearDamping() As ScaledNumber
            Get
                Return m_snLinearDamping
            End Get
            Set(ByVal Value As ScaledNumber)
                If Value.ActualValue < 0 Then
                    Throw New System.Exception("You can not set the linear damping to be less than zero!")
                End If

                m_snLinearDamping.CopyData(Value)
            End Set
        End Property

        Public Property AngularCompliance() As ScaledNumber
            Get
                Return m_snAngularCompliance
            End Get
            Set(ByVal Value As ScaledNumber)
                If Value.ActualValue < 0 Then
                    Throw New System.Exception("You can not set the angular compliance to be less than zero!")
                End If

                m_snAngularCompliance.CopyData(Value)
            End Set
        End Property

        Public Property AngularDamping() As ScaledNumber
            Get
                Return m_snAngularDamping
            End Get
            Set(ByVal Value As ScaledNumber)
                If Value.ActualValue < 0 Then
                    Throw New System.Exception("You can not set the angular damping to be less than zero!")
                End If

                m_snAngularDamping.CopyData(Value)
            End Set
        End Property

        Public Property LinearKineticLoss() As ScaledNumber
            Get
                Return m_snLinearKineticLoss
            End Get
            Set(ByVal Value As ScaledNumber)
                If Value.ActualValue < 0 Then
                    Throw New System.Exception("You can not set the linear kinetic loss to be less than zero!")
                End If

                m_snLinearKineticLoss.CopyData(Value)
            End Set
        End Property

        Public Property AngularKineticLoss() As ScaledNumber
            Get
                Return m_snAngularKineticLoss
            End Get
            Set(ByVal Value As ScaledNumber)
                If Value.ActualValue < 0 Then
                    Throw New System.Exception("You can not set the angular kinetic loss to be less than zero!")
                End If

                m_snAngularKineticLoss.CopyData(Value)
            End Set
        End Property

        Public Property MaxHydroForce() As ScaledNumber
            Get
                Return m_snMaxHydroForce
            End Get
            Set(ByVal Value As ScaledNumber)
                If Value.ActualValue < 0 Then
                    Throw New System.Exception("You can not set the maximum hyrdrodynamic force to be less than  zero!")
                End If

                m_snMaxHydroForce.CopyData(Value)
            End Set
        End Property

        Public Property MaxHydroTorque() As ScaledNumber
            Get
                Return m_snMaxHydroTorque
            End Get
            Set(ByVal Value As ScaledNumber)
                If Value.ActualValue < 0 Then
                    Throw New System.Exception("You can not set the maximum hyrdrodynamic torque to be less than  zero!")
                End If

                m_snMaxHydroTorque.CopyData(Value)
            End Set
        End Property

        Public Property DistanceUnits() As enumDistanceUnits
            Get
                Return m_eDistanceUnits
            End Get
            Set(ByVal Value As enumDistanceUnits)
                m_eDistanceUnits = Value
            End Set
        End Property

        Public ReadOnly Property DistanceUnitValue() As Single
            Get
                Return CSng(Math.Pow(10, CInt(m_eDistanceUnits)))
            End Get
        End Property

        Public ReadOnly Property DistanceUnitAbbreviation(ByVal eUnits As Environment.enumDistanceUnits) As String
            Get
                Select Case eUnits
                    Case enumDistanceUnits.Kilometers
                        Return "Km"
                    Case enumDistanceUnits.Centameters
                        Return "Cm"
                    Case enumDistanceUnits.Decameters
                        Return "Dm"
                    Case enumDistanceUnits.Meters
                        Return "m"
                    Case enumDistanceUnits.Decimeters
                        Return "dm"
                    Case enumDistanceUnits.Centimeters
                        Return "cm"
                    Case enumDistanceUnits.Millimeters
                        Return "mm"
                End Select

                Return "m"
            End Get
        End Property

        Public ReadOnly Property DisplayDistanceUnits() As enumDistanceUnits
            Get
                Return DisplayDistanceUnits(Me.DistanceUnits)
            End Get
        End Property

        Public ReadOnly Property DisplayDistanceUnits(ByVal eUnits As enumDistanceUnits) As enumDistanceUnits
            Get
                Select Case eUnits
                    Case enumDistanceUnits.Kilometers
                        Return enumDistanceUnits.Kilometers
                    Case enumDistanceUnits.Centameters
                        Return enumDistanceUnits.Centameters
                    Case enumDistanceUnits.Decameters
                        Return enumDistanceUnits.Meters
                    Case enumDistanceUnits.Meters
                        Return enumDistanceUnits.Meters
                    Case enumDistanceUnits.Decimeters
                        Return enumDistanceUnits.Centimeters
                    Case enumDistanceUnits.Centimeters
                        Return enumDistanceUnits.Centimeters
                    Case enumDistanceUnits.Millimeters
                        Return enumDistanceUnits.Millimeters
                End Select

                Return enumDistanceUnits.Meters
            End Get
        End Property

        Public ReadOnly Property DisplayDistanceUnitValue() As Single
            Get
                Return Me.DisplayDistanceUnitValue(Me.DistanceUnits)
            End Get
        End Property

        Public ReadOnly Property DisplayDistanceUnitValue(ByVal eUnits As enumDistanceUnits) As Single
            Get
                Return CSng(Math.Pow(10, CInt(Me.DisplayDistanceUnits(eUnits))))
            End Get
        End Property

        Public Property MassUnits() As enumMassUnits
            Get
                Return m_eMassUnits
            End Get
            Set(ByVal Value As enumMassUnits)
                m_eMassUnits = Value
            End Set
        End Property

        Public ReadOnly Property MassUnitValue() As Single
            Get
                Return CSng(Math.Pow(10, CInt(m_eMassUnits)))
            End Get
        End Property

        Public ReadOnly Property MassUnitAbbreviation() As String
            Get
                Select Case m_eMassUnits
                    Case enumMassUnits.Kilograms
                        Return "Kg"
                    Case enumMassUnits.Centagrams
                        Return "Cg"
                    Case enumMassUnits.Grams
                        Return "g"
                    Case enumMassUnits.Centigrams
                        Return "cg"
                    Case enumMassUnits.Milligrams
                        Return "mg"
                End Select

                Return "g"
            End Get
        End Property

        'This calculates the default density. We will be using the density of water as the 
        'default setting, but we must set it up appropriately.
        Public Overridable ReadOnly Property DefaultDensity() As ScaledNumber
            Get

                Dim fltValue As Double = Me.MassUnitValue
                Dim eSCale As ScaledNumber.enumNumericScale = CType(Util.Environment.MassUnits, ScaledNumber.enumNumericScale)
                Dim strUnits As String = "g/" & Util.Environment.DistanceUnitAbbreviation(Me.DisplayDistanceUnits) & "^3"

                Return New ScaledNumber(Me, "Density", fltValue, eSCale, strUnits, strUnits)
            End Get
        End Property

        Public Overridable ReadOnly Property Organisms() As Collections.SortedStructures
            Get
                Return m_aryOrganisms
            End Get
        End Property

        Public Overridable ReadOnly Property Structures() As Collections.SortedStructures
            Get
                Return m_aryStructures
            End Get
        End Property

        Public Overridable ReadOnly Property OdorTypes() As Collections.SortedOdorTypes
            Get
                Return m_aryOdorTypes
            End Get
        End Property

        Public Overridable ReadOnly Property OrganismsTreeNode() As TreeNode
            Get
                Return m_nodeOrganisms
            End Get
        End Property

        Public Overridable ReadOnly Property EnvironmentTreeNode() As TreeNode
            Get
                Return m_nodeEnvironment
            End Get
        End Property

        Public Overridable ReadOnly Property StructuresTreeNode() As TreeNode
            Get
                Return m_wsStructureNodes
            End Get
        End Property

        Public Overridable Property NewOrganismCount() As Integer
            Get
                Return m_iNewOrganismCount
            End Get
            Set(ByVal Value As Integer)
                m_iNewOrganismCount = Value
            End Set
        End Property

        Public Overridable Property NewStructureCount() As Integer
            Get
                Return m_iNewStructureCount
            End Get
            Set(ByVal Value As Integer)
                m_iNewStructureCount = Value
            End Set
        End Property

        Public Overridable Property GroundSurface() As DataObjects.Physical.PhysicalStructure
            Get
                Return m_doGround
            End Get
            Set(ByVal Value As DataObjects.Physical.PhysicalStructure)
                m_doGround = DirectCast(Value, DataObjects.Physical.GroundSurface)
            End Set
        End Property

        Public Overridable Property WaterSurface() As DataObjects.Physical.PhysicalStructure
            Get
                Return m_doWater
            End Get
            Set(ByVal Value As DataObjects.Physical.PhysicalStructure)
                m_doWater = DirectCast(Value, DataObjects.Physical.WaterSurface)
            End Set
        End Property

        Public Overridable ReadOnly Property Camera() As DataObjects.Physical.Camera
            Get
                Return m_doCamera
            End Get
        End Property

        Public Overridable Property AutoGenerateRandomSeed() As Boolean
            Get
                Return m_bAutoGenerateRandomSeed
            End Get
            Set(ByVal Value As Boolean)
                m_bAutoGenerateRandomSeed = Value

                'Reselect this node to see if we need to display the manual seed or not.
                Util.Application.ProjectWorkspace.PropertyData = Me.Properties
            End Set
        End Property

        Public Overridable Property ManualRandomSeed() As Integer
            Get
                Return m_iManualRandomSeed
            End Get
            Set(ByVal Value As Integer)
                m_iManualRandomSeed = Value
            End Set
        End Property

#End Region

#Region " Methods "

        Public Sub New(ByVal doParent As Framework.DataObject)
            MyBase.New(doParent)

            m_snPhysicsTimeStep = New AnimatTools.Framework.ScaledNumber(Me, "PhysicsTimeStep", 1, AnimatTools.Framework.ScaledNumber.enumNumericScale.milli, "seconds", "s")
            m_snGravity = New AnimatTools.Framework.ScaledNumber(Me, "Gravity", -9.81, AnimatTools.Framework.ScaledNumber.enumNumericScale.None, "m/s^2", "m/s^2")
            m_snMaxSurfaceFriction = New AnimatTools.Framework.ScaledNumber(Me, "MaxSurfaceFriction", 15, AnimatTools.Framework.ScaledNumber.enumNumericScale.Kilo, "Newtons", "N")

            If Not Util.Environment Is Nothing Then
                m_snFluidDensity = Util.Environment.DefaultDensity
            Else
                m_snFluidDensity = New ScaledNumber(Me, "FluidDensity", 1000, ScaledNumber.enumNumericScale.Kilo, "g/m^2", "g/m^2")
            End If

            m_snMouseSpringStiffness = New AnimatTools.Framework.ScaledNumber(Me, "MouseSpringStiffness", 300, ScaledNumber.enumNumericScale.None, "N/m", "N/m")
            m_snMouseSpringDamping = New AnimatTools.Framework.ScaledNumber(Me, "MouseSpringDamping", 100, ScaledNumber.enumNumericScale.Kilo, "g/s", "g/s")

            m_snLinearCompliance = New ScaledNumber(Me, "LinearCompliance", 0.1, ScaledNumber.enumNumericScale.micro, "m/N", "m/N")
            m_snLinearDamping = New ScaledNumber(Me, "LinearDamping", 200, ScaledNumber.enumNumericScale.Kilo, "g/s", "g/s")

            m_snAngularCompliance = New ScaledNumber(Me, "AngularCompliance", 0.1, ScaledNumber.enumNumericScale.micro, "m/N", "m/N")
            m_snAngularDamping = New ScaledNumber(Me, "AngularDamping", 50, ScaledNumber.enumNumericScale.Kilo, "g/s", "g/s")

            m_snLinearKineticLoss = New ScaledNumber(Me, "LinearKineticLoss", 1, ScaledNumber.enumNumericScale.micro, "g/s", "g/s")
            m_snAngularKineticLoss = New ScaledNumber(Me, "AngularKineticLoss", 1, ScaledNumber.enumNumericScale.micro, "g/s", "g/s")

            m_snMaxHydroForce = New ScaledNumber(Me, "MaxHydroForce", 100, ScaledNumber.enumNumericScale.None, "Newtons", "N")
            m_snMaxHydroTorque = New ScaledNumber(Me, "MaxHydroTorque", 100, ScaledNumber.enumNumericScale.None, "Newton-Meters", "Nm")

        End Sub

        Public Overridable Sub AddOrganism()
            Me.OnNewOrganism(Me, New System.EventArgs)
        End Sub

        Public Overridable Sub AddStructure()
            Me.OnNewStructure(Me, New System.EventArgs)
        End Sub

        Public Overridable Sub AddGround()
            Me.OnAddGround(Me, New System.EventArgs)
        End Sub

        Public Overridable Sub AddWater()
            Me.OnAddWater(Me, New System.EventArgs)
        End Sub

#Region " Treeview/Menu Methods "

        Public Overridable Sub CreateWorkspaceTreeView(ByVal dsSim As AnimatTools.DataObjects.Simulation, _
                                                       ByVal frmWorkspace As Forms.ProjectWorkspace)

            Dim myAssembly As System.Reflection.Assembly
            myAssembly = System.Reflection.Assembly.Load("AnimatTools")

            frmWorkspace.ImageManager.AddImage(myAssembly, "AnimatTools.Organisms.gif")
            frmWorkspace.ImageManager.AddImage(myAssembly, "AnimatTools.Organism.gif")
            frmWorkspace.ImageManager.AddImage(myAssembly, "AnimatTools.Environment.gif")
            frmWorkspace.ImageManager.AddImage(myAssembly, "AnimatTools.Structures.gif")
            frmWorkspace.ImageManager.AddImage(myAssembly, "AnimatTools.Structure.gif")
            frmWorkspace.ImageManager.AddImage(myAssembly, "AnimatTools.Neuron.gif")
            frmWorkspace.ImageManager.AddImage(myAssembly, "AnimatTools.Joint.gif")
            frmWorkspace.ImageManager.AddImage(myAssembly, "AnimatTools.Ground.gif")
            frmWorkspace.ImageManager.AddImage(myAssembly, "AnimatTools.Water.gif")
            frmWorkspace.ImageManager.AddImage(myAssembly, "AnimatTools.CameraTreeView.gif")

            frmWorkspace.ImageManager.ImageList.ImageSize = New Size(25, 25)
            frmWorkspace.TreeView.ImageList = frmWorkspace.ImageManager.ImageList

            m_nodeEnvironment = Util.Simulation.SimulationTreeNode.Nodes.Add("Environment")
            m_nodeEnvironment.ImageIndex = frmWorkspace.ImageManager.GetImageIndex("AnimatTools.Environment.gif")
            m_nodeEnvironment.SelectedImageIndex = frmWorkspace.ImageManager.GetImageIndex("AnimatTools.Environment.gif")
            m_nodeEnvironment.Tag = Me

            m_nodeOrganisms = m_nodeEnvironment.Nodes.Add("Organisms")
            m_nodeOrganisms.ImageIndex = frmWorkspace.ImageManager.GetImageIndex("AnimatTools.Organisms.gif")
            m_nodeOrganisms.SelectedImageIndex = frmWorkspace.ImageManager.GetImageIndex("AnimatTools.Organisms.gif")

            m_wsStructureNodes = m_nodeEnvironment.Nodes.Add("Structures")
            m_wsStructureNodes.ImageIndex = frmWorkspace.ImageManager.GetImageIndex("AnimatTools.Structures.gif")
            m_wsStructureNodes.SelectedImageIndex = frmWorkspace.ImageManager.GetImageIndex("AnimatTools.Structures.gif")

            Dim doOrganism As DataObjects.Physical.Organism
            For Each deEntry As DictionaryEntry In m_aryOrganisms
                doOrganism = DirectCast(deEntry.Value, DataObjects.Physical.Organism)
                doOrganism.CreateWorkspaceTreeView(dsSim, frmWorkspace)
            Next

            Dim doStructure As DataObjects.Physical.PhysicalStructure
            For Each deEntry As DictionaryEntry In m_aryStructures
                doStructure = DirectCast(deEntry.Value, DataObjects.Physical.PhysicalStructure)
                doStructure.CreateWorkspaceTreeView(dsSim, frmWorkspace)
            Next

            If Not m_doGround Is Nothing Then
                m_doGround.CreateWorkspaceTreeView(dsSim, frmWorkspace)
            End If

            If Not m_doWater Is Nothing Then
                m_doWater.CreateWorkspaceTreeView(dsSim, frmWorkspace)
            End If

            m_doCamera.CreateWorkspaceTreeView(dsSim, frmWorkspace)

            m_iNewOrganismCount = Util.ExtractIDCount("Organism", m_aryOrganisms)
            m_iNewStructureCount = Util.ExtractIDCount("Structure", m_aryStructures)

        End Sub

        Public Overridable Function WorkspaceTreeviewPopupMenu(ByRef tnSelectedNode As TreeNode, ByVal ptPoint As Point) As Boolean

            If tnSelectedNode Is m_nodeOrganisms Then
                PopupOrganismMenu(ptPoint)
                Return True
            End If

            If tnSelectedNode Is m_wsStructureNodes Then
                PopupStructuresMenu(ptPoint)
                Return True
            End If

            If Not m_doGround Is Nothing AndAlso tnSelectedNode Is m_doGround.WorkspaceStructureNode Then
                Return m_doGround.WorkspaceTreeviewPopupMenu(tnSelectedNode, ptPoint)
            End If

            If Not m_doWater Is Nothing AndAlso tnSelectedNode Is m_doWater.WorkspaceStructureNode Then
                Return m_doWater.WorkspaceTreeviewPopupMenu(tnSelectedNode, ptPoint)
            End If

            Dim doOrganism As DataObjects.Physical.Organism
            For Each deEntry As DictionaryEntry In m_aryOrganisms
                doOrganism = DirectCast(deEntry.Value, DataObjects.Physical.Organism)
                If doOrganism.WorkspaceTreeviewPopupMenu(tnSelectedNode, ptPoint) Then Return True
            Next

            Dim doStructure As DataObjects.Physical.PhysicalStructure
            For Each deEntry As DictionaryEntry In m_aryStructures
                doStructure = DirectCast(deEntry.Value, DataObjects.Physical.PhysicalStructure)
                If doStructure.WorkspaceTreeviewPopupMenu(tnSelectedNode, ptPoint) Then Return True
            Next

            If tnSelectedNode Is m_nodeEnvironment Then
                Dim popup As New PopupMenu
                Dim mcExpandAll As New MenuCommand("Expand All", tnSelectedNode, _
                                                  New EventHandler(AddressOf Me.OnExpandAll))
                Dim mcCollapseAll As New MenuCommand("Collapse All", tnSelectedNode, _
                                                  New EventHandler(AddressOf Me.OnCollapseAll))

                mcExpandAll.ImageList = Util.Application.SmallImages.ImageList
                mcExpandAll.ImageIndex = Util.Application.SmallImages.GetImageIndex("AnimatTools.Expand.gif")
                mcCollapseAll.ImageList = Util.Application.SmallImages.ImageList
                mcCollapseAll.ImageIndex = Util.Application.SmallImages.GetImageIndex("AnimatTools.Collapse.gif")

                popup.MenuCommands.AddRange(New MenuCommand() {mcExpandAll, mcCollapseAll})
                Dim selected As MenuCommand = popup.TrackPopup(ptPoint)
            End If

            Return False
        End Function

        Protected Overridable Sub PopupOrganismMenu(ByVal ptPoint As Point)

            ' Create the menu items
            Dim mcInsert As New MenuCommand("New Organism", "NewOrganism", Util.Application.SmallImages.ImageList, _
                                              Util.Application.SmallImages.GetImageIndex("AnimatTools.AddOrganism.gif"), _
                                              New EventHandler(AddressOf Me.OnNewOrganism))

            Dim mcSepExpand As MenuCommand = New MenuCommand("-")
            Dim mcExpandAll As New MenuCommand("Expand All", m_nodeOrganisms, _
                                              New EventHandler(AddressOf Me.OnExpandAll))
            Dim mcCollapseAll As New MenuCommand("Collapse All", m_nodeOrganisms, _
                                                New EventHandler(AddressOf Me.OnCollapseAll))

            mcExpandAll.ImageList = Util.Application.SmallImages.ImageList
            mcExpandAll.ImageIndex = Util.Application.SmallImages.GetImageIndex("AnimatTools.Expand.gif")
            mcCollapseAll.ImageList = Util.Application.SmallImages.ImageList
            mcCollapseAll.ImageIndex = Util.Application.SmallImages.GetImageIndex("AnimatTools.Collapse.gif")

            ' Create the popup menu object
            Dim popup As New PopupMenu

            ' Define the list of menu commands
            popup.MenuCommands.Add(mcInsert)
            popup.MenuCommands.AddRange(New MenuCommand() {mcSepExpand, mcExpandAll, mcCollapseAll})

            ' Show it!
            Dim selected As MenuCommand = popup.TrackPopup(ptPoint)

        End Sub

        Protected Overridable Sub PopupStructuresMenu(ByVal ptPoint As Point)

            ' Create the menu items
            Dim mcInsert As New MenuCommand("New Structure", "NewStructure", Util.Application.SmallImages.ImageList, _
                                              Util.Application.SmallImages.GetImageIndex("AnimatTools.AddStructure.gif"), _
                                              New EventHandler(AddressOf Me.OnNewStructure))

            Dim mcSepExpand As MenuCommand = New MenuCommand("-")
            Dim mcExpandAll As New MenuCommand("Expand All", m_wsStructureNodes, _
                                              New EventHandler(AddressOf Me.OnExpandAll))
            Dim mcCollapseAll As New MenuCommand("Collapse All", m_wsStructureNodes, _
                                                New EventHandler(AddressOf Me.OnCollapseAll))

            mcExpandAll.ImageList = Util.Application.SmallImages.ImageList
            mcExpandAll.ImageIndex = Util.Application.SmallImages.GetImageIndex("AnimatTools.Expand.gif")
            mcCollapseAll.ImageList = Util.Application.SmallImages.ImageList
            mcCollapseAll.ImageIndex = Util.Application.SmallImages.GetImageIndex("AnimatTools.Collapse.gif")

            ' Create the popup menu object
            Dim popup As New PopupMenu

            ' Define the list of menu commands
            popup.MenuCommands.Add(mcInsert)

            If m_doGround Is Nothing Then
                Dim mcGround As New MenuCommand("Add Ground Surface", "AddGroundSurface", Util.Application.SmallImages.ImageList, _
                                              Util.Application.SmallImages.GetImageIndex("AnimatTools.AddGround.gif"), _
                                              New EventHandler(AddressOf Me.OnAddGround))
                popup.MenuCommands.Add(mcGround)
            End If

            If m_doWater Is Nothing Then
                Dim mcWater As New MenuCommand("Add Water Surface", "AddWaterSurface", Util.Application.SmallImages.ImageList, _
                                              Util.Application.SmallImages.GetImageIndex("AnimatTools.AddWater.gif"), _
                                              New EventHandler(AddressOf Me.OnAddWater))
                popup.MenuCommands.Add(mcWater)
            End If

            popup.MenuCommands.AddRange(New MenuCommand() {mcSepExpand, mcExpandAll, mcCollapseAll})

            ' Show it!
            Dim selected As MenuCommand = popup.TrackPopup(ptPoint)

        End Sub

        Public Overridable Function WorkspaceTreeviewDoubleClick(ByRef tnSelectedNode As TreeNode) As Boolean

            If Not m_doGround Is Nothing AndAlso tnSelectedNode Is m_doGround.WorkspaceStructureNode Then
                Return m_doGround.WorkspaceTreeviewDoubleClick(tnSelectedNode)
            End If

            If Not m_doWater Is Nothing AndAlso tnSelectedNode Is m_doWater.WorkspaceStructureNode Then
                Return m_doWater.WorkspaceTreeviewDoubleClick(tnSelectedNode)
            End If

            Dim doOrganism As DataObjects.Physical.Organism
            For Each deEntry As DictionaryEntry In m_aryOrganisms
                doOrganism = DirectCast(deEntry.Value, DataObjects.Physical.Organism)
                If doOrganism.WorkspaceTreeviewDoubleClick(tnSelectedNode) Then Return True
            Next

            Dim doStructure As DataObjects.Physical.PhysicalStructure
            For Each deEntry As DictionaryEntry In m_aryStructures
                doStructure = DirectCast(deEntry.Value, DataObjects.Physical.PhysicalStructure)
                If doStructure.WorkspaceTreeviewDoubleClick(tnSelectedNode) Then Return True
            Next

            Return False

        End Function

#End Region

#Region " Find Methods "

        Public Overridable Function FindOrganism(ByVal strID As String, Optional ByVal bThrowError As Boolean = True) As DataObjects.Physical.PhysicalStructure
            Dim doStructure As DataObjects.Physical.PhysicalStructure

            If Me.Organisms.Contains(strID) Then
                doStructure = Me.Organisms(strID)
            ElseIf bThrowError Then
                Throw New System.Exception("No organism with the id '" & strID & "' was found.")
            End If

            Return doStructure
        End Function

        Public Overridable Function FindStructure(ByVal strID As String, Optional ByVal bThrowError As Boolean = True) As DataObjects.Physical.PhysicalStructure
            Dim doStructure As DataObjects.Physical.PhysicalStructure

            If Me.Structures.Contains(strID) Then
                doStructure = Me.Structures(strID)
            End If

            If strID.Trim.ToUpper = "GROUND" Then
                Return m_doGround
            End If

            If strID.Trim.ToUpper = "WATER" Then
                Return m_doWater
            End If

            If bThrowError Then
                Throw New System.Exception("No structure with the id '" & strID & "' was found.")
            End If

            Return doStructure
        End Function

        Public Overridable Function FindStructureFromAll(ByVal strID As String, Optional ByVal bThrowError As Boolean = True) As DataObjects.Physical.PhysicalStructure
            Dim doStructure As DataObjects.Physical.PhysicalStructure

            doStructure = FindStructure(strID, False)
            If Not doStructure Is Nothing Then Return doStructure

            doStructure = FindOrganism(strID, False)
            If Not doStructure Is Nothing Then Return doStructure

            If bThrowError Then
                Throw New System.Exception("No structure with the id '" & strID & "' was found.")
            End If

            Return doStructure
        End Function

        Public Overridable Function FindOrganismByName(ByVal strName As String, Optional ByVal bThrowError As Boolean = True) As DataObjects.Physical.PhysicalStructure
            Dim doStructure As DataObjects.Physical.PhysicalStructure

            For Each deEntry As DictionaryEntry In m_aryOrganisms
                doStructure = DirectCast(deEntry.Value, PhysicalStructure)
                If doStructure.Name = strName Then
                    Return doStructure
                End If
            Next

            If bThrowError Then
                Throw New System.Exception("No organism with the name '" & strName & "' was found.")
            End If

            Return doStructure
        End Function

        Public Overridable Function FindStructureByName(ByVal strName As String, Optional ByVal bThrowError As Boolean = True) As DataObjects.Physical.PhysicalStructure
            Dim doStructure As DataObjects.Physical.PhysicalStructure

            For Each deEntry As DictionaryEntry In m_aryStructures
                doStructure = DirectCast(deEntry.Value, PhysicalStructure)
                If doStructure.Name = strName Then
                    Return doStructure
                End If
            Next

            If Not m_doGround Is Nothing AndAlso m_doGround.Name = strName Then
                Return m_doGround
            End If

            If Not m_doWater Is Nothing AndAlso m_doWater.Name = strName Then
                Return m_doWater
            End If

            If bThrowError Then
                Throw New System.Exception("No structure with the name '" & strName & "' was found.")
            End If

            Return doStructure
        End Function

        Public Overridable Function FindStructureFromAllByName(ByVal strName As String, Optional ByVal bThrowError As Boolean = True) As DataObjects.Physical.PhysicalStructure
            Dim doStructure As DataObjects.Physical.PhysicalStructure

            doStructure = FindOrganismByName(strName, False)
            If Not doStructure Is Nothing Then Return doStructure

            doStructure = FindStructureByName(strName, False)
            If Not doStructure Is Nothing Then Return doStructure

            If bThrowError Then
                Throw New System.Exception("No structure with the name '" & strName & "' was found.")
            End If

            Return doStructure
        End Function

#End Region

#Region " DataObject Methods "

        Protected Overrides Sub BuildProperties()

            Dim pbNumberBag As Crownwood.Magic.Controls.PropertyBag = m_snPhysicsTimeStep.Properties
            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Physics Time Step", pbNumberBag.GetType(), "PhysicsTimeStep", _
                                        "Settings", "This is the increment that is taken between each time step of the physics simulator. ", pbNumberBag, _
                                        "", GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)))

            pbNumberBag = m_snGravity.Properties
            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Gravity", pbNumberBag.GetType(), "Gravity", _
                                        "Settings", "Sets the gravity for the simulation. This is applied along the y axis. Gravity is always specified in " & _
                                        "meters per second squared. regardless of the distance units specified.", pbNumberBag, _
                                        "", GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)))

            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("ID", Me.ID.GetType(), "ID", _
                                        "Settings", "ID", Me.ID, True))

            pbNumberBag = m_snMaxSurfaceFriction.Properties
            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Max Surface Friction", pbNumberBag.GetType(), "MaxSurfaceFriction", _
                                        "Settings", "Sets the surface friction for the simulation.", pbNumberBag, _
                                        "", GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)))

            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("AutoGenerate Random Seed", m_bAutoGenerateRandomSeed.GetType(), "AutoGenerateRandomSeed", _
                                        "Settings", "If this is true then the random number generator is automatically seeded at the beginning of a simulation to ensure " & _
                                        "different numbers are generated each run. If it is false then the seed specified in the Manual Random Seed property is used.", m_bAutoGenerateRandomSeed))

            If Not m_bAutoGenerateRandomSeed Then
                m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Manual Random Seed", m_iManualRandomSeed.GetType(), "ManualRandomSeed", _
                                            "Settings", "Allows the user to manual set the random number seed to use for the random number generator.", m_iManualRandomSeed))
            End If

            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("SimulateHydrodynamics", m_bSimulateHydrodynamics.GetType(), "SimulateHydrodynamics", _
                                        "Hydrodynamics", "Determines whether hydrodynamic effects such as buoyancy and drag act upon the bodies in the simulation. " & _
                                        "If this is turned off then the simulation will run slightly faster.", m_bSimulateHydrodynamics))

            pbNumberBag = m_snFluidDensity.Properties
            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Fluid Density", pbNumberBag.GetType(), "FluidDensity", _
                                        "Hydrodynamics", "The density of the fluid medium. This is only used if hydrodynamics are being simulated.", pbNumberBag, _
                                        "", GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)))

            pbNumberBag = m_snMaxHydroForce.Properties
            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Max Hydro Force", pbNumberBag.GetType(), "MaxHydroForce", _
                                        "Hydrodynamics", "The maximum hydrodynamic force that can be exerted.", pbNumberBag, _
                                        "", GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)))

            pbNumberBag = m_snMaxHydroTorque.Properties
            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Max Hydro Torque", pbNumberBag.GetType(), "MaxHydroTorque", _
                                        "Hydrodynamics", "The maximum hydrodynamic torque that can be exerted.", pbNumberBag, _
                                        "", GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)))

            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("DistanceUnits", GetType(String), "DistanceUnits", _
                                        "Units", "Determines the distance unit measurements used within the configuration files.", _
                                        m_eDistanceUnits, GetType(AnimatTools.TypeHelpers.UnitsTypeEditor), GetType(AnimatTools.TypeHelpers.UnitsTypeConverter)))

            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("MassUnits", GetType(String), "MassUnits", _
                                        "Units", "Determines the mass unit measurements used within the configuration files.", _
                                        m_eMassUnits, GetType(AnimatTools.TypeHelpers.UnitsTypeEditor), GetType(AnimatTools.TypeHelpers.UnitsTypeConverter)))

            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Alpha Blending", m_bUseAlphaBlending.GetType(), "UseAlphaBlending", _
                                        "Settings", "If this is true then it uses the transparency settings of the colors of bodies in the display.", m_bUseAlphaBlending))

            pbNumberBag = m_snMouseSpringStiffness.Properties
            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Mouse Spring Stiffness", pbNumberBag.GetType(), "MouseSpringStiffness", _
                                        "Mouse Spring Settings", "Sets the stiffness of the spring used when applying forces using the mouse during a simulation.", pbNumberBag, _
                                        "", GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)))

            pbNumberBag = m_snMouseSpringDamping.Properties
            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Mouse Spring Damping", pbNumberBag.GetType(), "MouseSpringDamping", _
                                        "Mouse Spring Settings", "Sets the damping of the spring used when applying forces using the mouse during a simulation.", pbNumberBag, _
                                        "", GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)))

            pbNumberBag = m_snLinearCompliance.Properties
            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Linear Compliance", pbNumberBag.GetType(), "LinearCompliance", _
                                        "World Stability", "The compliance value of the spring used in linear collisions within the simulator.", pbNumberBag, _
                                        "", GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)))

            pbNumberBag = m_snLinearDamping.Properties
            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Linear Damping", pbNumberBag.GetType(), "LinearDamping", _
                                        "World Stability", "The damping value of the spring used in linear collisions within the simulator.", pbNumberBag, _
                                        "", GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)))

            pbNumberBag = m_snAngularCompliance.Properties
            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Angular Compliance", pbNumberBag.GetType(), "AngularCompliance", _
                                        "World Stability", "The compliance value of the spring used in angular collisions within the simulator.", pbNumberBag, _
                                        "", GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)))

            pbNumberBag = m_snAngularDamping.Properties
            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Angular Damping", pbNumberBag.GetType(), "AngularDamping", _
                                        "World Stability", "The damping value of the spring used in angular collisions within the simulator.", pbNumberBag, _
                                        "", GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)))

            pbNumberBag = m_snLinearKineticLoss.Properties
            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Linear Kinetic Loss", pbNumberBag.GetType(), "LinearKineticLoss", _
                                        "World Stability", "The amount of kinetic loss for linear collisions within the simulator.", pbNumberBag, _
                                        "", GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)))

            pbNumberBag = m_snAngularKineticLoss.Properties
            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Angular Kinetic Loss", pbNumberBag.GetType(), "AngularKineticLoss", _
                                        "World Stability", "The amount of kinetic loss for angular collisions within the simulator.", pbNumberBag, _
                                        "", GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)))

        End Sub

        Public Overrides Sub ClearIsDirty()
            MyBase.ClearIsDirty()
            m_aryOrganisms.ClearIsDirty()
            m_aryStructures.ClearIsDirty()
            m_aryOdorTypes.ClearIsDirty()
            If Not m_doGround Is Nothing Then m_doGround.ClearIsDirty()
            If Not m_doWater Is Nothing Then m_doWater.ClearIsDirty()

            If Not m_snPhysicsTimeStep Is Nothing Then m_snPhysicsTimeStep.ClearIsDirty()
            If Not m_snGravity Is Nothing Then m_snGravity.ClearIsDirty()
            If Not m_snMaxSurfaceFriction Is Nothing Then m_snMaxSurfaceFriction.ClearIsDirty()
            If Not m_snFluidDensity Is Nothing Then m_snFluidDensity.ClearIsDirty()
            If Not m_snMouseSpringStiffness Is Nothing Then m_snMouseSpringStiffness.ClearIsDirty()
            If Not m_snMouseSpringDamping Is Nothing Then m_snMouseSpringDamping.ClearIsDirty()

            If Not m_snLinearCompliance Is Nothing Then m_snLinearCompliance.ClearIsDirty()
            If Not m_snLinearDamping Is Nothing Then m_snLinearDamping.ClearIsDirty()
            If Not m_snAngularCompliance Is Nothing Then m_snAngularCompliance.ClearIsDirty()
            If Not m_snAngularDamping Is Nothing Then m_snAngularDamping.ClearIsDirty()
            If Not m_snLinearKineticLoss Is Nothing Then m_snLinearKineticLoss.ClearIsDirty()
            If Not m_snAngularKineticLoss Is Nothing Then m_snAngularKineticLoss.ClearIsDirty()
            If Not m_snMaxHydroForce Is Nothing Then m_snMaxHydroForce.ClearIsDirty()
            If Not m_snMaxHydroTorque Is Nothing Then m_snMaxHydroTorque.ClearIsDirty()

        End Sub

        Protected Overrides Sub CloneInternal(ByVal doOriginal As AnimatTools.Framework.DataObject, ByVal bCutData As Boolean, _
                                            ByVal doRoot As AnimatTools.Framework.DataObject)
            MyBase.CloneInternal(doOriginal, bCutData, doRoot)

            Dim doOrig As AnimatTools.DataObjects.Physical.Environment = DirectCast(doOriginal, Environment)

            m_snPhysicsTimeStep = DirectCast(doOrig.m_snPhysicsTimeStep.Clone(Me, bCutData, doRoot), ScaledNumber)
            m_snGravity = DirectCast(doOrig.m_snGravity.Clone(Me, bCutData, doRoot), ScaledNumber)
            m_snMaxSurfaceFriction = DirectCast(doOrig.m_snMaxSurfaceFriction.Clone(Me, bCutData, doRoot), ScaledNumber)
            m_snFluidDensity = DirectCast(doOrig.m_snFluidDensity.Clone(Me, bCutData, doRoot), ScaledNumber)
            m_snMouseSpringStiffness = DirectCast(doOrig.m_snMouseSpringStiffness.Clone(Me, bCutData, doRoot), ScaledNumber)
            m_snMouseSpringDamping = DirectCast(doOrig.m_snMouseSpringDamping.Clone(Me, bCutData, doRoot), ScaledNumber)

            m_snLinearCompliance = DirectCast(doOrig.m_snLinearCompliance.Clone(Me, bCutData, doRoot), ScaledNumber)
            m_snLinearDamping = DirectCast(doOrig.m_snLinearDamping.Clone(Me, bCutData, doRoot), ScaledNumber)
            m_snAngularCompliance = DirectCast(doOrig.m_snAngularCompliance.Clone(Me, bCutData, doRoot), ScaledNumber)
            m_snAngularDamping = DirectCast(doOrig.m_snAngularDamping.Clone(Me, bCutData, doRoot), ScaledNumber)
            m_snLinearKineticLoss = DirectCast(doOrig.m_snLinearKineticLoss.Clone(Me, bCutData, doRoot), ScaledNumber)
            m_snAngularKineticLoss = DirectCast(doOrig.m_snAngularKineticLoss.Clone(Me, bCutData, doRoot), ScaledNumber)

            m_snMaxHydroForce = DirectCast(doOrig.m_snMaxHydroForce.Clone(Me, bCutData, doRoot), ScaledNumber)
            m_snMaxHydroTorque = DirectCast(doOrig.m_snMaxHydroTorque.Clone(Me, bCutData, doRoot), ScaledNumber)

            m_eDistanceUnits = doOrig.m_eDistanceUnits
            m_eMassUnits = doOrig.m_eMassUnits
            m_bSimulateHydrodynamics = doOrig.m_bSimulateHydrodynamics
            m_fltPlaybackRate = doOrig.m_fltPlaybackRate
            m_bUseAlphaBlending = doOrig.m_bUseAlphaBlending

            m_iNewOrganismCount = 0
            m_iNewStructureCount = 0

            'm_aryOrganisms = doOrig.m_aryOrganisms.Clone
            'm_aryStructures = doOrig.m_aryStructures.Clone

            m_doGround = DirectCast(doOrig.m_doGround.Clone(Me, bCutData, doRoot), GroundSurface)
            m_doWater = DirectCast(doOrig.m_doWater.Clone(Me, bCutData, doRoot), WaterSurface)

            m_doCamera = DirectCast(doOrig.m_doCamera.Clone(Me, bCutData, doRoot), Camera)

        End Sub

        Public Overrides Function Clone(ByVal doParent As AnimatTools.Framework.DataObject, ByVal bCutData As Boolean, _
                                        ByVal doRoot As AnimatTools.Framework.DataObject) As AnimatTools.Framework.DataObject
            Dim doItem As New Environment(doParent)
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

            Dim iDistDiff As Integer = CInt(Me.DisplayDistanceUnits) - CInt(Util.Environment.DisplayDistanceUnits(ePrevDistance))
            Dim fltDensityDistChange As Single = CSng(10 ^ iDistDiff)

            Dim fltValue As Double = (m_snFluidDensity.ActualValue / Math.Pow(10, CInt(ePrevMass))) * (Math.Pow(fltDensityDistChange, 3) / fltMassChange)
            Dim eSCale As ScaledNumber.enumNumericScale = CType(Util.Environment.MassUnits, ScaledNumber.enumNumericScale)
            Dim strUnits As String = "g/" & Util.Environment.DistanceUnitAbbreviation(Me.DisplayDistanceUnits) & "^3"
            m_snFluidDensity = New ScaledNumber(Me, "FluidDensity", fltValue, eSCale, strUnits, strUnits)

            m_snMouseSpringStiffness.ActualValue = m_snMouseSpringStiffness.ActualValue / fltDistanceChange
            m_snMouseSpringDamping.ActualValue = m_snMouseSpringStiffness.ActualValue / fltMassChange

            m_snLinearCompliance.ActualValue = m_snLinearCompliance.ActualValue * fltDistanceChange
            m_snLinearDamping.ActualValue = m_snLinearDamping.ActualValue / fltDistanceChange

            m_snAngularCompliance.ActualValue = m_snAngularCompliance.ActualValue * fltDistanceChange
            m_snAngularDamping.ActualValue = m_snAngularDamping.ActualValue / fltDistanceChange

            'm_snAngularCompliance.ActualValue = m_snAngularCompliance.ActualValue * fltDistanceChange
            'm_snAngularDamping.ActualValue = m_snAngularDamping.ActualValue / fltMassChange

            If Not m_doGround Is Nothing Then
                m_doGround.UnitsChanged(ePrevMass, eNewMass, fltMassChange, ePrevDistance, eNewDistance, fltDistanceChange)
            End If
            If Not m_doWater Is Nothing Then
                m_doWater.UnitsChanged(ePrevMass, eNewMass, fltMassChange, ePrevDistance, eNewDistance, fltDistanceChange)
            End If
            If Not m_doCamera Is Nothing Then
                m_doCamera.UnitsChanged(ePrevMass, eNewMass, fltMassChange, ePrevDistance, eNewDistance, fltDistanceChange)
            End If

            Dim doStruct As PhysicalStructure
            For Each deEntry As DictionaryEntry In m_aryOrganisms
                doStruct = DirectCast(deEntry.Value, PhysicalStructure)
                doStruct.UnitsChanged(ePrevMass, eNewMass, fltMassChange, ePrevDistance, eNewDistance, fltDistanceChange)
            Next

            For Each deEntry As DictionaryEntry In m_aryStructures
                doStruct = DirectCast(deEntry.Value, PhysicalStructure)
                doStruct.UnitsChanged(ePrevMass, eNewMass, fltMassChange, ePrevDistance, eNewDistance, fltDistanceChange)
            Next

        End Sub

        Public Overridable Overloads Sub LoadData(ByRef frmApplication As Forms.AnimatApplication, ByRef oXml As Interfaces.StdXml)

            oXml.IntoChildElement("Environment") 'Into Environment Element

            If ScaledNumber.IsValidXml(oXml, "PhysicsTimeStep") Then
                m_snPhysicsTimeStep.LoadData(oXml, "PhysicsTimeStep")
                m_snFluidDensity.LoadData(oXml, "FluidDensity")
                m_snMaxSurfaceFriction.LoadData(oXml, "MaxSurfaceFriction")
                m_snGravity.LoadData(oXml, "Gravity")
            Else
                m_snPhysicsTimeStep.ActualValue = oXml.GetChildDouble("PhysicsTimeStep", m_snPhysicsTimeStep.ActualValue)
                m_snFluidDensity.ActualValue = oXml.GetChildDouble("FluidDensity", m_snFluidDensity.ActualValue)
                m_snMaxSurfaceFriction.ActualValue = oXml.GetChildDouble("SurfaceFriction", m_snMaxSurfaceFriction.ActualValue)
                m_snGravity.ActualValue = oXml.GetChildDouble("Gravity", m_snGravity.ActualValue)
            End If

            If oXml.FindChildElement("MouseSpringStiffness", False) Then
                m_snMouseSpringStiffness.LoadData(oXml, "MouseSpringStiffness")
                m_snMouseSpringDamping.LoadData(oXml, "MouseSpringDamping")
            End If

            If oXml.FindChildElement("LinearCompliance", False) Then
                m_snLinearCompliance.LoadData(oXml, "LinearCompliance")
                m_snLinearDamping.LoadData(oXml, "LinearDamping")
                m_snAngularCompliance.LoadData(oXml, "AngularCompliance")
                m_snAngularDamping.LoadData(oXml, "AngularDamping")
                m_snLinearKineticLoss.LoadData(oXml, "LinearKineticLoss")
                m_snAngularKineticLoss.LoadData(oXml, "AngularKineticLoss")
            End If

            Me.SimulateHydrodynamics = oXml.GetChildBool("SimulateHydrodynamics", m_bSimulateHydrodynamics)
            Me.PlaybackRate = oXml.GetChildFloat("PlaybackRate", m_fltPlaybackRate)
            Me.UseAlphaBlending = oXml.GetChildBool("UseAlphaBlending", m_bUseAlphaBlending)

            If oXml.FindChildElement("AutoGenerateRandomSeed", False) Then
                m_bAutoGenerateRandomSeed = oXml.GetChildBool("AutoGenerateRandomSeed")
                m_iManualRandomSeed = oXml.GetChildInt("ManualRandomSeed")
            End If

            If oXml.FindChildElement("MaxHydroForce", False) Then
                m_snMaxHydroForce.LoadData(oXml, "MaxHydroForce")
                m_snMaxHydroTorque.LoadData(oXml, "MaxHydroTorque")
            End If

            m_eMassUnits = DirectCast([Enum].Parse(GetType(enumMassUnits), oXml.GetChildString("MassUnits"), True), enumMassUnits)
            m_eDistanceUnits = DirectCast([Enum].Parse(GetType(enumDistanceUnits), oXml.GetChildString("DistanceUnits"), True), enumDistanceUnits)

            'Odor types must be loaded before structures
            Dim iCount As Integer
            If oXml.FindChildElement("OdorTypes", False) Then
                oXml.IntoChildElement("OdorTypes") 'Into Structures Element
                iCount = oXml.NumberOfChildren() - 1

                Dim newOdorType As OdorType
                For iIndex As Integer = 0 To iCount
                    oXml.FindChildByIndex(iIndex)

                    newOdorType = New DataObjects.Physical.OdorType(Me)
                    newOdorType.LoadData(frmApplication.Simulation, oXml)

                    m_aryOdorTypes.Add(newOdorType.ID, newOdorType)
                Next
                oXml.OutOfElem() 'Outof Structures Element
            End If

            If oXml.FindChildElement("GroundSurface", False) Then
                m_doGround = New DataObjects.Physical.GroundSurface(Me)
                m_doGround.LoadData(frmApplication.Simulation, oXml)
            End If

            If oXml.FindChildElement("WaterSurface", False) Then
                m_doWater = New DataObjects.Physical.WaterSurface(Me)
                m_doWater.LoadData(frmApplication.Simulation, oXml)
            End If

            Dim newOrganism As DataObjects.Physical.Organism
            oXml.IntoChildElement("Organisms") 'Into Organisms Element
            iCount = oXml.NumberOfChildren() - 1

            For iIndex As Integer = 0 To iCount
                oXml.FindChildByIndex(iIndex)

                newOrganism = New DataObjects.Physical.Organism(Me)
                newOrganism.LoadData(frmApplication.Simulation, oXml)

                m_aryOrganisms.Add(newOrganism.ID, newOrganism)
            Next
            oXml.OutOfElem() 'Outof Organisms Element

            Dim newStructure As DataObjects.Physical.PhysicalStructure
            oXml.IntoChildElement("Structures") 'Into Structures Element
            iCount = oXml.NumberOfChildren() - 1

            For iIndex As Integer = 0 To iCount
                oXml.FindChildByIndex(iIndex)

                newStructure = New DataObjects.Physical.PhysicalStructure(Me)
                newStructure.LoadData(frmApplication.Simulation, oXml)

                m_aryStructures.Add(newStructure.ID, newStructure)
            Next
            oXml.OutOfElem() 'Outof Structures Element

            'Now lets initialize after the load.
            If Not m_doGround Is Nothing Then m_doGround.InitializeAfterLoad(frmApplication.Simulation)
            If Not m_doWater Is Nothing Then m_doWater.InitializeAfterLoad(frmApplication.Simulation)

            For Each deEntry As DictionaryEntry In m_aryOrganisms
                newStructure = DirectCast(deEntry.Value, PhysicalStructure)
                newStructure.InitializeAfterLoad(frmApplication.Simulation)
            Next

            For Each deEntry As DictionaryEntry In m_aryStructures
                newStructure = DirectCast(deEntry.Value, PhysicalStructure)
                newStructure.InitializeAfterLoad(frmApplication.Simulation)
            Next

            m_doCamera.LoadData(frmApplication.Simulation, oXml)

            oXml.OutOfElem() 'Outof Environment Element

        End Sub

        Public Overridable Overloads Sub SaveData(ByRef frmApplication As Forms.AnimatApplication, ByRef oXml As Interfaces.StdXml)

            oXml.AddChildElement("Environment")
            oXml.IntoElem()

            m_snPhysicsTimeStep.SaveData(oXml, "PhysicsTimeStep")
            m_snFluidDensity.SaveData(oXml, "FluidDensity")
            m_snMaxSurfaceFriction.SaveData(oXml, "MaxSurfaceFriction")
            m_snGravity.SaveData(oXml, "Gravity")
            m_snMouseSpringStiffness.SaveData(oXml, "MouseSpringStiffness")
            m_snMouseSpringDamping.SaveData(oXml, "MouseSpringDamping")

            m_snLinearCompliance.SaveData(oXml, "LinearCompliance")
            m_snLinearDamping.SaveData(oXml, "LinearDamping")
            m_snAngularCompliance.SaveData(oXml, "AngularCompliance")
            m_snAngularDamping.SaveData(oXml, "AngularDamping")
            m_snLinearKineticLoss.SaveData(oXml, "LinearKineticLoss")
            m_snAngularKineticLoss.SaveData(oXml, "AngularKineticLoss")

            m_snMaxHydroForce.SaveData(oXml, "MaxHydroForce")
            m_snMaxHydroTorque.SaveData(oXml, "MaxHydroTorque")

            oXml.AddChildElement("PlaybackRate", m_fltPlaybackRate)
            oXml.AddChildElement("SimulateHydrodynamics", m_bSimulateHydrodynamics)

            oXml.AddChildElement("MassUnits", m_eMassUnits.ToString())
            oXml.AddChildElement("DistanceUnits", m_eDistanceUnits.ToString())

            oXml.AddChildElement("UseAlphaBlending", m_bUseAlphaBlending)

            oXml.AddChildElement("AutoGenerateRandomSeed", m_bAutoGenerateRandomSeed)
            oXml.AddChildElement("ManualRandomSeed", m_iManualRandomSeed)

            m_doCamera.SaveData(frmApplication.Simulation, oXml)

            'If we are saving the config file for downloading to a  robot then we should only
            'save the selected organism and none of the other organisms or structures.
            If Util.RobotOrganism Is Nothing Then
                If Not m_doGround Is Nothing Then
                    m_doGround.SaveData(frmApplication.Simulation, oXml)
                End If

                If Not m_doWater Is Nothing Then
                    m_doWater.SaveData(frmApplication.Simulation, oXml)
                End If

                If m_aryOdorTypes.Count > 0 Then
                    oXml.AddChildElement("OdorTypes")
                    oXml.IntoElem()
                    Dim doOdor As DataObjects.Physical.OdorType
                    For Each deEntry As DictionaryEntry In m_aryOdorTypes
                        doOdor = DirectCast(deEntry.Value, DataObjects.Physical.OdorType)
                        doOdor.SaveData(frmApplication.Simulation, oXml)
                    Next
                    oXml.OutOfElem() 'Outof Organisms Element
                End If

                oXml.AddChildElement("Organisms")
                oXml.IntoElem()
                Dim doOrganism As DataObjects.Physical.Organism
                For Each deEntry As DictionaryEntry In m_aryOrganisms
                    doOrganism = DirectCast(deEntry.Value, DataObjects.Physical.Organism)
                    doOrganism.SaveData(frmApplication.Simulation, oXml)
                Next
                oXml.OutOfElem() 'Outof Organisms Element

                oXml.AddChildElement("Structures")
                oXml.IntoElem()
                Dim doStructure As DataObjects.Physical.PhysicalStructure
                For Each deEntry As DictionaryEntry In m_aryStructures
                    doStructure = DirectCast(deEntry.Value, DataObjects.Physical.PhysicalStructure)
                    doStructure.SaveData(frmApplication.Simulation, oXml)
                Next
                oXml.OutOfElem() 'Outof Structures Element
            Else
                oXml.AddChildElement("Organisms")
                oXml.IntoElem()
                Util.RobotOrganism.SaveData(frmApplication.Simulation, oXml)
                oXml.OutOfElem() 'Outof Organisms Element
            End If

            oXml.OutOfElem() 'Outof Environment Element

        End Sub

        Public Overrides Function FindObjectByID(ByVal strID As String) As Framework.DataObject

            Dim doObject As AnimatTools.Framework.DataObject = MyBase.FindObjectByID(strID)
            If doObject Is Nothing AndAlso Not m_aryOrganisms Is Nothing Then doObject = m_aryOrganisms.FindObjectByID(strID)
            If doObject Is Nothing AndAlso Not m_aryStructures Is Nothing Then doObject = m_aryStructures.FindObjectByID(strID)
            If doObject Is Nothing AndAlso Not m_aryOdorTypes Is Nothing Then doObject = m_aryOdorTypes.FindObjectByID(strID)
            If doObject Is Nothing AndAlso Not m_doGround Is Nothing Then doObject = m_doGround.FindObjectByID(strID)
            If doObject Is Nothing AndAlso Not m_doWater Is Nothing Then doObject = m_doWater.FindObjectByID(strID)
            If doObject Is Nothing AndAlso Not m_doCamera Is Nothing Then doObject = m_doCamera.FindObjectByID(strID)
            Return doObject

        End Function

#End Region

#End Region

#Region " Events "

        Protected Sub OnNewOrganism(ByVal sender As Object, ByVal e As System.EventArgs)

            Try
                Dim doOrganism As New DataObjects.Physical.Organism(Me)

                m_iNewOrganismCount = m_iNewOrganismCount + 1
                doOrganism.Name = "Organism_" & m_iNewOrganismCount
                Me.Organisms.Add(doOrganism.ID, doOrganism)

                doOrganism.CreateWorkspaceTreeView(Util.Simulation, Util.Application.ProjectWorkspace)
                doOrganism.WorkspaceStructureNode.ExpandAll()
                Util.Application.ProjectWorkspace.TreeView.SelectedNode = doOrganism.WorkspaceStructureNode
                doOrganism.CreateFiles()

                'If this is the first organism then lets set the camer to track it.
                If Me.Organisms.Count = 1 Then
                    Util.Environment.Camera.AutoTrack(doOrganism)
                End If

                Util.Application.SaveProject(Util.Application.ProjectFile)

            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try

        End Sub

        Protected Sub OnNewStructure(ByVal sender As Object, ByVal e As System.EventArgs)

            Try
                Dim doStructure As New DataObjects.Physical.PhysicalStructure(Me)

                m_iNewStructureCount = m_iNewStructureCount + 1
                doStructure.Name = "Structure_" & m_iNewStructureCount
                Me.Structures.Add(doStructure.ID, doStructure)

                doStructure.CreateWorkspaceTreeView(Util.Simulation, Util.Application.ProjectWorkspace)
                Util.Application.ProjectWorkspace.TreeView.SelectedNode = doStructure.WorkspaceStructureNode
                doStructure.CreateFiles()

                Util.Application.SaveProject(Util.Application.ProjectFile)

            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try

        End Sub

        Protected Sub OnAddGround(ByVal sender As Object, ByVal e As System.EventArgs)

            Try
                If Not Util.Environment.GroundSurface Is Nothing Then
                    Throw New System.Exception("You can only add one ground surface")
                End If

                m_doGround = New DataObjects.Physical.GroundSurface(Me)
                m_doGround.ID = "Ground"
                m_doGround.Name = "Ground"

                m_doGround.CreateWorkspaceTreeView(Util.Simulation, Util.Application.ProjectWorkspace)
                Util.Application.ProjectWorkspace.TreeView.SelectedNode = m_doGround.WorkspaceStructureNode

                Util.Application.EnableDefaultMenuItem("Edit", "Add Ground", False)
                Util.Application.EnableDefaultToolbarItem("Add Ground", False)

                Util.Application.SaveProject(Util.Application.ProjectFile)

            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try

        End Sub

        Protected Sub OnAddWater(ByVal sender As Object, ByVal e As System.EventArgs)

            Try
                If Not Util.Environment.WaterSurface Is Nothing Then
                    Throw New System.Exception("You can only add one water surface")
                End If

                m_doWater = New DataObjects.Physical.WaterSurface(Me)
                m_doWater.ID = "Water"
                m_doWater.Name = "Water"

                m_doWater.CreateWorkspaceTreeView(Util.Simulation, Util.Application.ProjectWorkspace)
                Util.Application.ProjectWorkspace.TreeView.SelectedNode = m_doWater.WorkspaceStructureNode
                m_bSimulateHydrodynamics = True

                Util.Application.EnableDefaultMenuItem("Edit", "Add Water", False)
                Util.Application.EnableDefaultToolbarItem("Add Water", False)

                Util.Application.SaveProject(Util.Application.ProjectFile)

            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try

        End Sub

#End Region

    End Class

End Namespace

