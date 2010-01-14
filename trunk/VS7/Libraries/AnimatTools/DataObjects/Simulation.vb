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

    Public MustInherit Class Simulation
        Inherits Framework.DataObject

#Region " Attributes "

        Protected m_strAnimatModule As String = "VortexAnimatLibrary_vc7.dll"
        Protected m_iUpdateDataInterval As Integer = 250
        Protected m_bStartPaused As Boolean = False
        Protected m_bUseReleaseLibraries As Boolean = True
        Protected m_bEnableSimRecording As Boolean = True

        Protected m_doEnvironment As New DataObjects.Physical.Environment(Me)

        Protected m_strFilePath As String = ""
        Protected m_strFileName As String = ""

        Protected m_nodeSimulation As TreeNode
        Protected m_nodePlaybackControl As TreeNode

        Protected m_strExternalStimuli As String = ""

        Protected m_bSetSimEnd As Boolean = False
        Protected m_snSimEndTime As ScaledNumber

        Protected m_strAPI_File As String = ""

#End Region

#Region " Properties "

        Public Property AnimatModule() As String
            Get
                Return m_strAnimatModule
            End Get
            Set(ByVal Value As String)
                m_strAnimatModule = Value
            End Set
        End Property

        Public Property UpdateDataInterval() As Integer
            Get
                Return m_iUpdateDataInterval
            End Get
            Set(ByVal Value As Integer)
                If Value <= 0 Then
                    Throw New System.Exception("You can not set the update data interval to be less than or equal to zero!")
                End If

                m_iUpdateDataInterval = Value
            End Set
        End Property

        Public Property StartPaused() As Boolean
            Get
                Return m_bStartPaused
            End Get
            Set(ByVal Value As Boolean)
                m_bStartPaused = Value
            End Set
        End Property

        Public Property UseReleaseLibraries() As Boolean
            Get
                Return m_bUseReleaseLibraries
            End Get
            Set(ByVal Value As Boolean)
                m_bUseReleaseLibraries = Value

                If m_bUseReleaseLibraries Then
                    Me.AnimatModule = "VortexAnimatLibrary_vc7.dll"
                Else
                    Me.AnimatModule = "VortexAnimatLibrary_vc7D.dll"
                End If

            End Set
        End Property

        Public Property EnableSimRecording() As Boolean
            Get
                Return m_bEnableSimRecording
            End Get
            Set(ByVal Value As Boolean)
                m_bEnableSimRecording = Value
            End Set
        End Property

        Public Overridable Property LogLevel() As AnimatTools.Interfaces.Logger.enumLogLevel
            Get
                Return Util.Logger.TraceLevel
            End Get
            Set(ByVal Value As AnimatTools.Interfaces.Logger.enumLogLevel)
                Util.Logger.TraceLevel = Value
            End Set
        End Property
        Public Overridable ReadOnly Property Environment() As DataObjects.Physical.Environment
            Get
                Return m_doEnvironment
            End Get
        End Property

        Public Overridable ReadOnly Property SimulationTreeNode() As TreeNode
            Get
                Return m_nodeSimulation
            End Get
        End Property

        Public ReadOnly Property PlaybackControlTreeNode() As TreeNode
            Get
                Return m_nodePlaybackControl
            End Get
        End Property

        Public Overridable Property SetSimulationEnd() As Boolean
            Get
                Return m_bSetSimEnd
            End Get
            Set(ByVal Value As Boolean)
                m_bSetSimEnd = Value

                'Refresh the property grid 
                Util.Application.ProjectWorkspace.PropertyData = Me.Properties
            End Set
        End Property

        Public Overridable Property SimulationEndTime() As ScaledNumber
            Get
                Return m_snSimEndTime
            End Get
            Set(ByVal Value As ScaledNumber)
                If Value.ActualValue <= 0 Then
                    Throw New System.Exception("The simulation end time must be greater than 0.")
                End If

                m_snSimEndTime.CopyData(Value)
            End Set
        End Property

        Public Overridable Property APIFile() As String
            Get
                Return m_strAPI_File
            End Get
            Set(ByVal Value As String)
                m_strAPI_File = Value
            End Set
        End Property

#End Region

#Region " Methods "

        Public Sub New()
            MyBase.New(Nothing)

            m_snSimEndTime = New AnimatTools.Framework.ScaledNumber(Me, "SimEndTime", 1, AnimatTools.Framework.ScaledNumber.enumNumericScale.None, "seconds", "s")

        End Sub

        Public Sub New(ByVal doParent As Framework.DataObject)
            MyBase.New(doParent)

            m_snSimEndTime = New AnimatTools.Framework.ScaledNumber(Me, "SimEndTime", 1, AnimatTools.Framework.ScaledNumber.enumNumericScale.None, "seconds", "s")
        End Sub

        Public Overridable Overloads Function CreateObject(ByRef oXml As Interfaces.StdXml, ByVal strClassType As String, _
                                                           ByVal doParent As AnimatTools.Framework.DataObject) As Framework.DataObject
            oXml.IntoElem()

            'If we have a partype listed then it gives the complete assemblyname and classname that we 
            'can use to load with the Util.LoadClass function. Otherwise load things the old, non-modular way.
            If oXml.FindChildElement("PartType", False) Then
                Dim strClass As String = oXml.GetChildString("PartType")
                Dim aryClassName() As String = Split(strClass, ".")
                Dim strAssembly As String = aryClassName(0)
                oXml.OutOfElem()

                Dim doObject As Framework.DataObject = DirectCast(Util.LoadClass(strAssembly, strClass, doParent), Framework.DataObject)
                Return doObject
            Else
                Dim strObjectType As String = oXml.GetChildString("Type")
                oXml.OutOfElem()
                Return CreateObject(strClassType, strObjectType, doParent)
            End If

        End Function

        Public MustOverride Overloads Function CreateObject(ByVal strClassType As String, ByVal strObjectType As String, _
                                                           ByVal doParent As AnimatTools.Framework.DataObject) As Framework.DataObject

        Public Overridable Sub CreateWorkspaceTreeView(ByVal frmWorkspace As Forms.ProjectWorkspace)

            If Not m_nodeSimulation Is Nothing Then
                m_nodeSimulation.Remove()
                m_nodeSimulation = Nothing
            End If

            Dim myAssembly As System.Reflection.Assembly
            myAssembly = System.Reflection.Assembly.Load("AnimatTools")

            frmWorkspace.ImageManager.AddImage(myAssembly, "AnimatTools.DefaultObject.gif")
            frmWorkspace.ImageManager.AddImage(myAssembly, "AnimatTools.Simulation.gif")
            frmWorkspace.ImageManager.AddImage(myAssembly, "AnimatTools.RemoteControl.gif")

            m_nodeSimulation = frmWorkspace.TreeView.Nodes.Add("Simulation")
            m_nodeSimulation.ImageIndex = frmWorkspace.ImageManager.GetImageIndex("AnimatTools.Simulation.gif")
            m_nodeSimulation.SelectedImageIndex = frmWorkspace.ImageManager.GetImageIndex("AnimatTools.Simulation.gif")
            m_nodeSimulation.Tag = Me

            m_nodePlaybackControl = m_nodeSimulation.Nodes.Add("Playback Control")
            m_nodePlaybackControl.ImageIndex = frmWorkspace.ImageManager.GetImageIndex("AnimatTools.RemoteControl.gif")
            m_nodePlaybackControl.SelectedImageIndex = frmWorkspace.ImageManager.GetImageIndex("AnimatTools.RemoteControl.gif")
            m_nodePlaybackControl.Tag = Util.Application.SimulationController

            m_doEnvironment.CreateWorkspaceTreeView(Me, frmWorkspace)

        End Sub

        Public Overridable Function WorkspaceTreeviewPopupMenu(ByRef tnSelectedNode As TreeNode, ByVal ptPoint As Point) As Boolean
            If m_doEnvironment.WorkspaceTreeviewPopupMenu(tnSelectedNode, ptPoint) Then Return True

            If tnSelectedNode Is m_nodeSimulation Then
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

                Return True
            End If

            Return False
        End Function

        Public Overridable Function WorkspaceTreeviewDoubleClick(ByRef tnSelectedNode As TreeNode) As Boolean
            If m_doEnvironment.WorkspaceTreeviewDoubleClick(tnSelectedNode) Then Return True
            Return False
        End Function

        Public Overridable Sub SetReleaseLibraries()
#If Not Debug Then
            Me.UseReleaseLibraries = True
#Else
            Me.UseReleaseLibraries = False
#End If
        End Sub

#Region " DataObject Methods "

        Protected Overrides Sub BuildProperties()

            'm_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Animat Module", m_strAnimatModule.GetType(), "AnimatModule", _
            '                            "Simulation Modules", "This determines the animat plug-in module that will be used throughout the simulation. " & _
            '                            "This plug-in will control the physics portion of the simulation.", m_strAnimatModule))

            'm_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Use Release Libraries", m_bUseReleaseLibraries.GetType(), "UseReleaseLibraries", _
            '                            "Playback Control", "Determines if the debug or release libraries should be used for running the simulator.", m_bUseReleaseLibraries))

            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("ID", Me.ID.GetType(), "ID", _
                                        "Settings", "ID", Me.ID, True))

            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("API File", m_strAPI_File.GetType(), "APIFile", _
                                        "Settings", "APIFile", m_strAPI_File))

            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Log Level", GetType(AnimatTools.Interfaces.Logger.enumLogLevel), "LogLevel", _
                                        "Logging", "Sets the level of logging in the application.", Me.LogLevel))

            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Set Sim To End", m_bSetSimEnd.GetType(), "SetSimulationEnd", _
                                        "Playback Control", "If this is true then the simulation will automatically end at the Sim End Time.", m_bSetSimEnd))

            Dim pbNumberBag As Crownwood.Magic.Controls.PropertyBag = m_snSimEndTime.Properties
            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Sim End Time", pbNumberBag.GetType(), "SimulationEndTime", _
                                        "Playback Control", "Sets the time at which the simulation will end if the SetSimEnd property is true.", pbNumberBag, _
                                        "", GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter), Not m_bSetSimEnd))

        End Sub

        Public Overrides Sub ClearIsDirty()
            MyBase.ClearIsDirty()
            m_doEnvironment.ClearIsDirty()
        End Sub

        Public Overrides Sub UnitsChanged(ByVal ePrevMass As AnimatTools.DataObjects.Physical.Environment.enumMassUnits, _
                                          ByVal eNewMass As AnimatTools.DataObjects.Physical.Environment.enumMassUnits, _
                                          ByVal fltMassChange As Single, _
                                          ByVal ePrevDistance As AnimatTools.DataObjects.Physical.Environment.enumDistanceUnits, _
                                          ByVal eNewDistance As AnimatTools.DataObjects.Physical.Environment.enumDistanceUnits, _
                                          ByVal fltDistanceChange As Single)
            m_doEnvironment.UnitsChanged(ePrevMass, eNewMass, fltMassChange, ePrevDistance, eNewDistance, fltDistanceChange)
        End Sub

        Public Overridable Overloads Sub LoadData(ByRef frmApplication As Forms.AnimatApplication, ByVal strFilename As String)
            Dim oXml As New AnimatTools.Interfaces.StdXml

            Util.SplitPathAndFile(strFilename, m_strFilePath, m_strFileName)

            oXml.Load(strFilename)

            oXml.FindElement("Simulation")
            oXml.FindChildElement("")

            LoadData(frmApplication, oXml)
        End Sub

        Public Overridable Overloads Sub LoadData(ByRef frmApplication As Forms.AnimatApplication, ByRef oXml As Interfaces.StdXml)

            Me.UpdateDataInterval = oXml.GetChildInt("UpdateDataInterval", m_iUpdateDataInterval)
            Me.StartPaused = oXml.GetChildBool("StartPaused", m_bStartPaused)
            'Me.UseReleaseLibraries = oXml.GetChildBool("UseReleaseLibraries", m_bUseReleaseLibraries)
            Me.EnableSimRecording = oXml.GetChildBool("EnableSimRecording", m_bEnableSimRecording)
            Me.AnimatModule = oXml.GetChildString("AnimatModule", m_strAnimatModule)
            m_strExternalStimuli = oXml.GetChildString("ExternalStimuli", "")
            m_strAPI_File = oXml.GetChildString("APIFile", "")

            If oXml.FindChildElement("SetSimEnd", False) Then
                m_bSetSimEnd = oXml.GetChildBool("SetSimEnd", m_bSetSimEnd)
                m_snSimEndTime.LoadData(oXml, "SimEndTime")
            End If

            m_doEnvironment.LoadData(frmApplication, oXml)

        End Sub

        Public Overridable Overloads Sub SaveData(ByRef frmApplication As Forms.AnimatApplication, ByVal strFilename As String)
            Dim oXml As New AnimatTools.Interfaces.StdXml

            oXml.AddElement("Simulation")

            SaveData(frmApplication, oXml)

            oXml.Save(Util.GetFilePath(Util.Application.ProjectPath, strFilename))
        End Sub

        Public Overridable Overloads Sub SaveData(ByRef frmApplication As Forms.AnimatApplication, ByRef oXml As Interfaces.StdXml)

            'If it is standalone then do not save the project path. This is typically used for grid applicaitons, 
            'and the project path needs to be the current directory the exe is located within.
            If Not Util.ExportForStandAloneSim Then
                oXml.AddChildElement("ProjectPath", Util.Application.ProjectPath)
            Else
                oXml.AddChildElement("ProjectPath", "")
            End If

            oXml.AddChildElement("AnimatModule", m_strAnimatModule)
            oXml.AddChildElement("UpdateDataInterval", m_iUpdateDataInterval)
            oXml.AddChildElement("StartPaused", m_bStartPaused)
            'oXml.AddChildElement("UseReleaseLibraries", m_bUseReleaseLibraries)
            oXml.AddChildElement("EnableSimRecording", m_bEnableSimRecording)
            oXml.AddChildElement("SetSimEnd", m_bSetSimEnd)
            oXml.AddChildElement("APIFile", m_strAPI_File)
            m_snSimEndTime.SaveData(oXml, "SimEndTime")

            m_doEnvironment.SaveData(frmApplication, oXml)

            'This must go after the environment is saved because the indices of the neurons may have changed.
            'If this is a robot organism or if we are saving the config file to use
            'in a stand-alone simulation then we need to save out the data charts and
            'the stimuli so they can be used in those cases.
            If Not Util.RobotOrganism Is Nothing OrElse Util.ExportForStandAloneSim Then
                Dim strXml As String
                Dim doStim As DataObjects.ExternalStimuli.Stimulus

                If Util.ExportStimsInStandAloneSim Then
                    If Util.Application.ProjectStimuli.Count > 0 Then
                        oXml.AddChildElement("ExternalStimuli")
                        oXml.IntoElem()

                        For Each deEntry As DictionaryEntry In Util.Application.ProjectStimuli
                            doStim = DirectCast(deEntry.Value, DataObjects.ExternalStimuli.Stimulus)
                            If doStim.Enabled Then
                                doStim.SaveXml(oXml)
                            End If
                        Next

                        oXml.OutOfElem()
                    End If
                Else
                    If m_strExternalStimuli.Trim.Length > 0 Then
                        oXml.AddChildElement("ExternalStimuli", m_strExternalStimuli)
                    End If
                End If

                'Lets go through and look for open toolviewer windows.
                'Any toolviewers we find we will save out datacharts for those
                'tools.
                If Util.ExportChartsInStandAloneSim Then
                    oXml.AddChildElement("DataCharts")
                    oXml.IntoElem()
                    For Each frmWindow As System.Windows.Forms.Form In frmApplication.MdiChildren
                        If TypeOf frmWindow Is Forms.Tools.Viewer Then
                            Dim frmViewer As Forms.Tools.Viewer = DirectCast(frmWindow, Forms.Tools.Viewer)
                            frmViewer.SaveXml(oXml)
                        End If
                    Next
                    oXml.OutOfElem()
                End If

            Else
                If m_strExternalStimuli.Trim.Length > 0 Then
                    oXml.AddChildElement("ExternalStimuli", m_strExternalStimuli)
                End If
            End If

        End Sub

        Public Overrides Function FindObjectByID(ByVal strID As String) As Framework.DataObject

            Dim doObject As AnimatTools.Framework.DataObject = MyBase.FindObjectByID(strID)
            If doObject Is Nothing AndAlso Not m_doEnvironment Is Nothing Then doObject = m_doEnvironment.FindObjectByID(strID)
            Return doObject

        End Function

#End Region

#End Region

#Region " Events "

#End Region

    End Class

End Namespace
