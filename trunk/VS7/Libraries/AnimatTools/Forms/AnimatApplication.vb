Imports System
Imports System.Drawing
Imports System.Collections
Imports System.ComponentModel
Imports System.Windows.Forms
Imports System.Diagnostics
Imports System.IO
Imports System.Text
imports System.Runtime.Remoting
Imports System.Threading
Imports System.Globalization
Imports Crownwood.Magic.Common
Imports Crownwood.Magic.Controls
Imports Crownwood.Magic.Docking
Imports Crownwood.Magic.Menus
Imports Crownwood.Magic.Toolbars
Imports AnimatTools.Framework

Namespace Forms

	Public Class AnimatApplication
        Inherits AnimatForm

#Region " Windows Form Designer generated code "

		Public Sub New()
			MyBase.New()

            ShowSplashScreen()

            Me.WindowState = FormWindowState.Maximized
            Me.IsMdiContainer = True
            Me.AllowDrop = True

            'This call is required by the Windows Form Designer.
			InitializeComponent()
            Initialize(Nothing)
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

        Friend WithEvents m_ctrlStatusBar As System.Windows.Forms.StatusBar
        Friend WithEvents m_pnlMessages As System.Windows.Forms.StatusBarPanel
        Friend WithEvents m_pnlInfo As System.Windows.Forms.StatusBarPanel

        'NOTE: The following procedure is required by the Windows Form Designer
        'It can be modified using the Windows Form Designer.  
        'Do not modify it using the code editor.

        <System.Diagnostics.DebuggerStepThrough()> Private Sub InitializeComponent()

            Me.m_ctrlStatusBar = New System.Windows.Forms.StatusBar
            Me.m_pnlMessages = New System.Windows.Forms.StatusBarPanel
            Me.m_pnlInfo = New System.Windows.Forms.StatusBarPanel

            CType(Me.m_pnlMessages, System.ComponentModel.ISupportInitialize).BeginInit()
            CType(Me.m_pnlInfo, System.ComponentModel.ISupportInitialize).BeginInit()

            Me.SuspendLayout()

            '
            'ctrlStatusBar
            '
            Me.m_ctrlStatusBar.Location = New System.Drawing.Point(0, 234)
            Me.m_ctrlStatusBar.Name = "ctrlStatusBar"
            Me.m_ctrlStatusBar.Panels.AddRange(New System.Windows.Forms.StatusBarPanel() {Me.m_pnlMessages, Me.m_pnlInfo})
            Me.m_ctrlStatusBar.ShowPanels = True
            Me.m_ctrlStatusBar.Size = New System.Drawing.Size(292, 32)
            Me.m_ctrlStatusBar.TabIndex = 1
            '
            'pnlMessages
            '
            Me.m_pnlMessages.AutoSize = System.Windows.Forms.StatusBarPanelAutoSize.Spring
            Me.m_pnlMessages.Width = 176
            '
            'AnimatApplication
            '
            Me.AutoScaleBaseSize = New System.Drawing.Size(5, 13)
            Me.ClientSize = New System.Drawing.Size(200, 200)
            Me.Controls.Add(Me.m_ctrlStatusBar)
            Me.Name = "AnimatApplication"
            Me.Title = "AnimatLab"
            CType(Me.m_pnlMessages, System.ComponentModel.ISupportInitialize).EndInit()
            CType(Me.m_pnlInfo, System.ComponentModel.ISupportInitialize).EndInit()
            Me.ResumeLayout(False)

        End Sub

#End Region

#Region " Enums "

        Public Enum enumAutoUpdateInterval
            Never
            Daily
            Weekly
            Monthly
        End Enum

#End Region

#Region " Attributes "

        Private Declare Function WebUpdate Lib "wuw4.dll" (ByVal URL As String) As Long

        Protected m_mgrSmallImages As AnimatTools.Framework.ImageManager
        Protected m_mgrLargeImages As AnimatTools.Framework.ImageManager

        Protected m_menuDefault As MenuControl
        Protected m_menuCurrent As MenuControl

        Protected m_barDefault As Crownwood.Magic.Toolbars.ToolbarControl
        Protected m_barCurrent As Crownwood.Magic.Toolbars.ToolbarControl

        Protected m_aryDefaultMenus As New ArrayList
        Protected m_aryDefaultToolbars As New ArrayList

        Protected m_dockManager As DockingManager
        Protected m_mdiClient As Control
        Protected m_frmSimulationWnd As AnimatTools.Forms.MdiChild

        Protected m_strProjectPath As String = ""
        Protected m_strProjectFile As String = ""
        Protected m_strProjectName As String = ""
        Protected m_strPhysicsAssemblyName As String = ""
        Protected m_strPhysicsClassName As String = ""
        Protected m_strSimulationFile As String = ""
        Protected m_strLogDirectory As String = ""

        'This is temporary and should be removed
        Protected m_iWindow As Integer = 0

        Protected m_dsSimulation As DataObjects.Simulation

        Protected m_aryAllDataObjects As New Collections.DataObjects(Nothing)
        Protected m_aryNeuralModules As New Collections.SortedNeuralModules(Nothing)
        Protected m_aryPlugInAssemblies As New Collections.SortedAssemblies(Nothing)
        Protected m_aryBehavioralNodes As New Collections.Nodes(Nothing)
        Protected m_aryBehavioralLinks As New Collections.Links(Nothing)
        Protected m_aryBodyPartTypes As New Collections.BodyParts(Nothing)
        Protected m_aryRigidBodyTypes As New Collections.BodyParts(Nothing)
        Protected m_aryJointTypes As New Collections.BodyParts(Nothing)
        Protected m_aryBehavioralPanels As New Collections.SortedPanels(Nothing)
        Protected m_aryAlphabeticalBehavioralPanels As New ArrayList
        Protected m_aryToolPlugins As New Collections.Tools(Nothing)
        Protected m_aryGainTypes As New Collections.Gains(Nothing)
        Protected m_aryProgramModules As New Collections.ProgramModules(Nothing)
        Protected m_aryMicrocontrollers As New Collections.Microcontrollers(Nothing)
        Protected m_aryExternalStimuli As New Collections.Stimuli(Nothing)

        Protected m_wcWorkspaceContent As Crownwood.Magic.Docking.WindowContent
        Protected m_frmWorkspace As Forms.ProjectWorkspace
        Protected m_frmToolbox As Forms.Toolbox
        Protected m_frmSimulationController As Forms.SimulationController

        Protected m_ipToolPanel As New IconPanel

        Protected m_aryToolHolders As New Collections.SortedToolHolders(m_doFormHelper)
        Protected m_iNewToolHolderIndex As Integer = 0

        Protected m_aryProjectStimuli As New Collections.SortedStimuli(m_doFormHelper)
        Protected m_iNewStimuliIndex As Integer = 0

        Protected m_ptSimWindowLocation As System.Drawing.Point
        Protected m_szSimWindowSize As System.Drawing.Size

        Protected m_bProjectIsOpen As Boolean = False

        Protected m_Logger As New AnimatTools.Interfaces.Logger
        Protected m_ModificationHistory As New AnimatTools.Framework.UndoSystem.ModificationHistory

        Friend WithEvents m_SplashTimer As New System.Windows.Forms.Timer

        Protected m_eAutoUpdateInterval As enumAutoUpdateInterval = enumAutoUpdateInterval.Daily
        Protected m_dtLastAutoUpdateTime As DateTime

        Protected m_bAnnouceUpdates As Boolean = False

#End Region

#Region " Properties "

        Public Overridable Property AutoUpdateInterval() As enumAutoUpdateInterval
            Get
                Return m_eAutoUpdateInterval
            End Get
            Set(ByVal Value As enumAutoUpdateInterval)
                m_eAutoUpdateInterval = Value
            End Set
        End Property

        Public Overridable Property LastAutoUpdateTime() As DateTime
            Get
                Return m_dtLastAutoUpdateTime
            End Get
            Set(ByVal Value As DateTime)
                m_dtLastAutoUpdateTime = Value
            End Set
        End Property

        Public Overridable Property CurrentMenu() As MenuControl
            Get
                Return m_menuCurrent
            End Get
            Set(ByVal Value As MenuControl)
                If Not m_menuCurrent Is Nothing Then Controls.Remove(m_menuCurrent)

                If Not Value Is Nothing Then
                    Controls.Add(Value)
                    m_menuCurrent = Value
                Else
                    Controls.Add(m_menuDefault)
                    m_menuCurrent = m_menuDefault
                End If
            End Set
        End Property

        Public Overridable Property CurrentToolbar() As Crownwood.Magic.Toolbars.ToolbarControl
            Get
                Return m_barCurrent
            End Get
            Set(ByVal Value As Crownwood.Magic.Toolbars.ToolbarControl)
                If Not m_barCurrent Is Nothing Then Controls.Remove(m_barCurrent)

                If Not Value Is Nothing Then
                    Controls.Add(Value)
                    m_barCurrent = Value
                Else
                    Controls.Add(m_barDefault)
                    m_barCurrent = m_barDefault
                End If
            End Set
        End Property

        Public Overridable ReadOnly Property SmallImages() As AnimatTools.Framework.ImageManager
            Get
                Return m_mgrSmallImages
            End Get
        End Property

        Public Overridable ReadOnly Property LargeImages() As AnimatTools.Framework.ImageManager
            Get
                Return m_mgrLargeImages
            End Get
        End Property

        Public Overridable ReadOnly Property DockingManager() As DockingManager
            Get
                Return m_dockManager
            End Get
        End Property

        Public Overridable ReadOnly Property ProjectWorkspace() As Forms.ProjectWorkspace
            Get
                Return m_frmWorkspace
            End Get
        End Property

        Public Overridable ReadOnly Property ProjectToolbox() As Forms.Toolbox
            Get
                Return m_frmToolbox
            End Get
        End Property

        Public Overridable ReadOnly Property SimulationController() As Forms.SimulationController
            Get
                Return m_frmSimulationController
            End Get
        End Property

        Public Overridable ReadOnly Property ApplicationDirectory() As String
            Get
                Dim strPath As String, strFile As String
                Util.SplitPathAndFile(Application.ExecutablePath, strPath, strFile)

                Return strPath
            End Get
        End Property

        Public Property LogDirectory() As String
            Get
                Return m_strLogDirectory
            End Get
            Set(ByVal Value As String)
                m_strLogDirectory = Value

                m_Logger.LogPrefix = m_strLogDirectory & "\AnimatLab"
            End Set
        End Property

        Public ReadOnly Property Logger() As AnimatTools.Interfaces.Logger
            Get
                Return m_Logger
            End Get
        End Property

        Public Overridable ReadOnly Property ToolsDirectory() As String
            Get
                Return ApplicationDirectory
                'If InStr(ApplicationDirectory, "AnimatTools\") > 0 Then
                '    Return ApplicationDirectory
                'Else
                '    Return ApplicationDirectory & "AnimatTools\"
                'End If
            End Get
        End Property

        Public ReadOnly Property MdiChildrenCount() As Integer
            Get
                Dim iCount As Integer
                For Each oChild As Form In Me.MdiChildren
                    iCount = iCount + 1
                Next
                Return iCount
            End Get
        End Property

        Public ReadOnly Property MdiClient() As Control
            Get
                Return m_mdiClient
            End Get
        End Property

        Public Property ProjectPath() As String
            Get
                Return m_strProjectPath
            End Get
            Set(ByVal Value As String)
                m_strProjectPath = Value
            End Set
        End Property

        Public Property ProjectFile() As String
            Get
                Return m_strProjectFile
            End Get
            Set(ByVal Value As String)
                m_strProjectFile = Value
            End Set
        End Property

        Public Property ProjectName() As String
            Get
                Return m_strProjectName
            End Get
            Set(ByVal Value As String)
                m_strProjectName = Value
            End Set
        End Property

        Public Property PhysicsAssemblyName() As String
            Get
                Return m_strPhysicsAssemblyName
            End Get
            Set(ByVal Value As String)
                m_strPhysicsAssemblyName = Value
            End Set
        End Property

        Public Property SimulationFile() As String
            Get
                Return m_strSimulationFile
            End Get
            Set(ByVal Value As String)
                m_strSimulationFile = Value
            End Set
        End Property

        Public Property SimulationWindow() As AnimatTools.Forms.MdiChild
            Get
                Return m_frmSimulationWnd
            End Get
            Set(ByVal Value As AnimatTools.Forms.MdiChild)
                m_frmSimulationWnd = Value
            End Set
        End Property

        Public ReadOnly Property SimulationView() As AnimatTools.Forms.SimulatorView
            Get
                If Not m_frmSimulationWnd Is Nothing Then
                    Return DirectCast(m_frmSimulationWnd.Control, AnimatTools.Forms.SimulatorView)
                End If
            End Get
        End Property

        Public ReadOnly Property StatusBar() As StatusBar
            Get
                Return m_ctrlStatusBar
            End Get
        End Property

        Public Overridable Property Simulation() As DataObjects.Simulation
            Get
                Return m_dsSimulation
            End Get
            Set(ByVal Value As DataObjects.Simulation)
                m_dsSimulation = Value
            End Set
        End Property

        Public Overridable ReadOnly Property NeuralModules() As Collections.SortedNeuralModules
            Get
                Return m_aryNeuralModules
            End Get
        End Property

        Public Overridable ReadOnly Property PlugInAssemblies() As Collections.SortedAssemblies
            Get
                Return m_aryPlugInAssemblies
            End Get
        End Property

        Public Overridable ReadOnly Property BehavioralNodes() As Collections.Nodes
            Get
                Return m_aryBehavioralNodes
            End Get
        End Property

        Public Overridable ReadOnly Property BehavioralLinks() As Collections.Links
            Get
                Return m_aryBehavioralLinks
            End Get
        End Property

        Public Overridable ReadOnly Property BodyPartTypes() As Collections.BodyParts
            Get
                Return m_aryBodyPartTypes
            End Get
        End Property

        Public Overridable ReadOnly Property RigidBodyTypes() As Collections.BodyParts
            Get
                Return m_aryRigidBodyTypes
            End Get
        End Property

        Public Overridable ReadOnly Property JointTypes() As Collections.BodyParts
            Get
                Return m_aryJointTypes
            End Get
        End Property

        Public Overridable ReadOnly Property BehavioralPanels() As Collections.SortedPanels
            Get
                Return m_aryBehavioralPanels
            End Get
        End Property

        Public Overridable ReadOnly Property AlphabeticalBehavioralPanels() As ArrayList
            Get
                Return m_aryAlphabeticalBehavioralPanels
            End Get
        End Property

        Public Overridable ReadOnly Property ToolHolders() As Collections.SortedToolHolders
            Get
                Return m_aryToolHolders
            End Get
        End Property

        Public Overridable ReadOnly Property GainTypes() As Collections.Gains
            Get
                Return m_aryGainTypes
            End Get
        End Property

        Public Overridable ReadOnly Property ProgramModules() As Collections.ProgramModules
            Get
                Return m_aryProgramModules
            End Get
        End Property

        Public Overridable ReadOnly Property Microcontrollers() As Collections.Microcontrollers
            Get
                Return m_aryMicrocontrollers
            End Get
        End Property

        Public Overridable ReadOnly Property ExternalStimuli() As Collections.Stimuli
            Get
                Return m_aryExternalStimuli
            End Get
        End Property

        Public Overridable ReadOnly Property ToolPlugins() As Collections.Tools
            Get
                Return m_aryToolPlugins
            End Get
        End Property

        Public Overridable Property NewToolHolderIndex() As Integer
            Get
                Return m_iNewToolHolderIndex
            End Get
            Set(ByVal Value As Integer)
                m_iNewToolHolderIndex = Value
            End Set
        End Property

        Public Overridable ReadOnly Property ProjectStimuli() As Collections.SortedStimuli
            Get
                Return m_aryProjectStimuli
            End Get
        End Property

        Public Overridable Property NewStimuliIndex() As Integer
            Get
                Return m_iNewStimuliIndex
            End Get
            Set(ByVal Value As Integer)
                m_iNewStimuliIndex = Value
            End Set
        End Property

        Public Overridable Property SimWindowLocation() As System.Drawing.Point
            Get
                Return m_ptSimWindowLocation
            End Get
            Set(ByVal Value As System.Drawing.Point)
                m_ptSimWindowLocation = Value
            End Set
        End Property

        Public Overridable Property SimWindowSize() As System.Drawing.Size
            Get
                Return m_szSimWindowSize
            End Get
            Set(ByVal Value As System.Drawing.Size)
                m_szSimWindowSize = Value
            End Set
        End Property

        Public Overridable ReadOnly Property ProjectIsOpen() As Boolean
            Get
                Return m_bProjectIsOpen
            End Get
        End Property

        Public Overridable ReadOnly Property ModificationHistory() As AnimatTools.Framework.UndoSystem.ModificationHistory
            Get
                Return m_ModificationHistory
            End Get
        End Property


#End Region

#Region " Methods "

#Region " Initialization "

        Public Overrides Sub Initialize(ByRef frmApplication As AnimatApplication, _
                                        Optional ByVal frmMdiParent As MdiChild = Nothing, _
                                        Optional ByVal frmParent As AnimatForm = Nothing)

            Try
                Util.Application = Me

                'Reset the culture info to be invariant english. I was getting problems 
                'with foriegn culture infos not parsing the xml files correctly.
                Thread.CurrentThread.CurrentCulture = New CultureInfo("")
                'Thread.CurrentThread.CurrentUICulture = New CultureInfo("")

                InitLogging()
                FindMdiClient()
                CatalogPluginModules()
                CheckSimRegistryEntry()
                ResetProject(False)
                LoadUserConfig()
                AutoUpdate()

                AddHandler m_mdiClient.Move, AddressOf Me.OnClientMove
                AddHandler m_dockManager.ContentClosing, AddressOf Me.OnDockContentClosing

            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try
        End Sub

        Private Sub AutoUpdate()
            Try
#If Not Debug Then
                Dim bDoUpdate As Boolean = False
                If (m_eAutoUpdateInterval = enumAutoUpdateInterval.Daily) Then
                    Dim temp As TimeSpan = Now.Subtract(m_dtLastAutoUpdateTime)
                    If (Now.Subtract(m_dtLastAutoUpdateTime).Days >= 1) Then
                        bDoUpdate = True
                    End If
                ElseIf (m_eAutoUpdateInterval = enumAutoUpdateInterval.Weekly) Then
                    If (Now.Subtract(m_dtLastAutoUpdateTime).Days > 6) Then
                        bDoUpdate = True
                    End If
                ElseIf (m_eAutoUpdateInterval = enumAutoUpdateInterval.Monthly) Then
                    If (Now.Subtract(m_dtLastAutoUpdateTime).Days > 29) Then
                        bDoUpdate = True
                    End If
                ElseIf (m_eAutoUpdateInterval = enumAutoUpdateInterval.Never) Then
                    bDoUpdate = False
                End If

                If bDoUpdate = True Then
                    CheckForUpdates(False)
                End If
#End If
            Catch ex As System.Exception
            End Try

        End Sub

        Public Sub CheckForUpdates(ByVal bAnnouceUpdates As Boolean)

            'For some reason if the user did not have internet access then the auto-update system could
            'lock up when the app starts. So I am having it start a new thread to check for the update
            'so the main app can go ahead.
            m_bAnnouceUpdates = bAnnouceUpdates
            Dim threadUpdates As New Threading.Thread(AddressOf Me.CheckForUpdatesThread)
            threadUpdates.Start()
            Threading.Thread.Sleep(0)

        End Sub

        Private Sub CheckForUpdatesThread()

            'First lets try and ping the server to see if this person is online.
            'If they are not then lets skip trying to check for updates.
            Dim netMon As New Crownwood.Magic.Network.Ping
            Dim response As Crownwood.Magic.Network.PingResponse = netMon.PingHost("http://www.animatlab.com", 4)

            If Not response Is Nothing AndAlso response.PingResult = Crownwood.Magic.Network.PingResponseType.Ok Then
                Dim myURL As String
                myURL = "http://www.animatlab.com/animatLab_Update.txt"
                WebUpdate(myURL)
                Util.UpdateConfigFile(m_eAutoUpdateInterval.ToString(), Now)
            ElseIf m_bAnnouceUpdates = True Then
                MessageBox.Show("Unable to ping host!")
            End If

        End Sub

        Private Sub LoadUserConfig()
            Try
                Dim oXml As New AnimatTools.Interfaces.StdXml
                oXml.Load(Me.ApplicationDirectory() & "AnimatLab.config")

                oXml.FindElement("AnimatLabConfig")
                oXml.FindChildElement("")

                m_eAutoUpdateInterval = DirectCast([Enum].Parse(GetType(enumAutoUpdateInterval), oXml.GetChildString("UpdateFrequency", enumAutoUpdateInterval.Daily.ToString()), True), enumAutoUpdateInterval)

                Try
                    Dim strDate As String = oXml.GetChildString("UpdateTime", "1/1/2001") '"27/12/2007 18:45:21" 
                    m_dtLastAutoUpdateTime = Date.Parse(strDate)
                Catch exDate As System.Exception
                    'If for some reason it fails on the parsing of the update time then set it to some time way in the past.
                    m_dtLastAutoUpdateTime = DateTime.Parse("1/1/2001")
                End Try

            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try
        End Sub

        Protected Overridable Sub InitLogging()
            Try
                If Directory.Exists(Me.ApplicationDirectory & "Logs") Then
                    Me.LogDirectory = Me.ApplicationDirectory & "Logs\"
                Else
                    Me.LogDirectory = Me.ApplicationDirectory
                End If

                Me.Logger.TraceLevel = Interfaces.Logger.enumLogLevel.Error
                Me.Logger.LogMsg(Interfaces.Logger.enumLogLevel.Info, "Initialized Logging")

                'Dim frame As New StackFrame(True)
                'Dim iVal As Integer
                'Me.Logger.LogMsg(Interfaces.Logger.enumLogLevel.Info, "File: " & frame.GetFileName & " Line: " & frame.GetFileLineNumber)
                'iVal = frame.GetFileLineNumber()

            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try
        End Sub

        Protected Overridable Sub FindMdiClient()
            Dim c As Control

            For Each c In Me.Controls
                If TypeOf c Is MdiClient Then
                    m_mdiClient = c
                End If
            Next

            AddHandler m_mdiClient.Click, AddressOf Me.OnMdiClientClicked

            Me.Logger.LogMsg(Interfaces.Logger.enumLogLevel.Info, "Found Mdi Client Window")
        End Sub

        Public Overridable Sub CheckSimRegistryEntry()

            Try

                'MessageBox.Show("I am about to open the 'software' registry subkey for read-only access!")

                Dim rkSoftware As Microsoft.Win32.RegistryKey
                Try
                    rkSoftware = Microsoft.Win32.Registry.LocalMachine.OpenSubKey("Software", False)
                Catch ex As System.Exception
                    'MessageBox.Show("Error opening 'software' for read-only access: " & ex.Message)
                    'If we get an error here then assume that we can not open the registry and jump out.
                    Return
                End Try

                'MessageBox.Show("I am about to open the 'FLEXlm License Manager' registry subkey for read-only access!")

                Dim rkKey As Microsoft.Win32.RegistryKey = rkSoftware.OpenSubKey("FLEXlm License Manager", False)

                If rkKey Is Nothing Then

                    'MessageBox.Show("'FLEXlm License Manager' subkey was not found. I am opening it for write access")

                    Try
                        rkSoftware = Microsoft.Win32.Registry.LocalMachine.OpenSubKey("Software", True)
                    Catch ex As System.Exception
                        'MessageBox.Show("Error opening 'software' for write access: " & ex.Message)
                        'If we get an error here then assume that we can not open the registry for writing and jump out.
                        Return
                    End Try

                    rkKey = rkSoftware.CreateSubKey("FLEXlm License Manager")
                End If

                'MessageBox.Show("I am attempting to get the item names under 'FLEXlm License Manager'")

                Dim aryNames As String() = rkKey.GetValueNames()

                For Each strName As String In aryNames
                    If strName.Trim.ToUpper() = "MATHNGIN_LICENSE_FILE" Then
                        'MessageBox.Show("'MATHNGIN_LICENSE_FILE' was found. I am exiting")
                        Return
                    End If
                Next

                'We may have gotten here by only opening the system to read, now lets try and open it for writing
                Try
                    'MessageBox.Show("I am about to open the 'FLEXlm License Manager' registry subkey for write access!")

                    rkKey = rkSoftware.OpenSubKey("FLEXlm License Manager", True)
                Catch ex As System.Exception
                    'If we get an error here then assume that we can not open the registry for writing and jump out.
                    Return
                End Try

                'MessageBox.Show("I am about to write the 'MATHNGIN_LICENSE_FILE' item")

                'If we get here then the license file registry entry does not exist so lets add it.
                Dim strDir As String = Util.Application.ApplicationDirectory
                strDir = strDir.Substring(0, strDir.Length - 1)
                rkKey.SetValue("MATHNGIN_LICENSE_FILE", strDir)

                'MessageBox.Show("I am finished messing with the registry")

            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try
        End Sub

        Public Overrides Sub ClearIsDirty()
            MyBase.ClearIsDirty()

            If Not m_dsSimulation Is Nothing Then m_dsSimulation.ClearIsDirty()
            m_aryNeuralModules.ClearIsDirty()
            m_aryPlugInAssemblies.ClearIsDirty()
            m_aryBehavioralNodes.ClearIsDirty()
            m_aryBehavioralLinks.ClearIsDirty()
            m_aryBehavioralPanels.ClearIsDirty()
            m_aryBodyPartTypes.ClearIsDirty()
            m_aryRigidBodyTypes.ClearIsDirty()
            m_aryJointTypes.ClearIsDirty()
            m_aryProjectStimuli.ClearIsDirty()

            Dim mdiChild As Forms.MdiChild
            For Each oChild As Form In Me.MdiChildren
                If TypeOf oChild Is Forms.MdiChild Then
                    mdiChild = DirectCast(oChild, Forms.MdiChild)
                    mdiChild.ClearIsDirty()
                End If
            Next

        End Sub

        Public Overridable Sub ChangeUnits(ByVal eNewMass As AnimatTools.DataObjects.Physical.Environment.enumMassUnits, _
                                           ByVal eNewDistance As AnimatTools.DataObjects.Physical.Environment.enumDistanceUnits)

            Dim ePrevDist As AnimatTools.DataObjects.Physical.Environment.enumDistanceUnits
            Dim ePrevMass As AnimatTools.DataObjects.Physical.Environment.enumMassUnits

            Try
                Util.ModificationHistory.AllowAddHistory = False

                ePrevDist = Util.Environment.DistanceUnits
                ePrevMass = Util.Environment.MassUnits

                If Util.Environment.MassUnits <> eNewMass Then
                    Util.Environment.MassUnits = eNewMass
                End If

                If Util.Environment.DistanceUnits <> eNewDistance Then
                    Util.Environment.DistanceUnits = eNewDistance
                End If

                Dim iMassDiff As Integer = CInt(eNewMass) - CInt(ePrevMass)
                Dim iDistDiff As Integer = CInt(eNewDistance) - CInt(ePrevDist)
                Dim fltMassChange As Single = CSng(10 ^ iMassDiff)
                Dim fltDistanceChange As Single = CSng(10 ^ iDistDiff)

                'Now lets go through and set the untis changed for the core simulation objects.
                Util.Simulation.UnitsChanged(ePrevMass, eNewMass, fltMassChange, ePrevDist, eNewDistance, fltDistanceChange)

                Dim doStim As AnimatTools.Framework.DataObject
                For Each deEntry As DictionaryEntry In Me.ProjectStimuli
                    doStim = DirectCast(deEntry.Value, AnimatTools.Framework.DataObject)
                    doStim.UnitsChanged(ePrevMass, eNewMass, fltMassChange, ePrevDist, eNewDistance, fltDistanceChange)
                Next

                'Now inform any interested part that the units have changed
                RaiseEvent UnitsChanged(ePrevMass, eNewMass, fltMassChange, ePrevDist, eNewDistance, fltDistanceChange)

            Catch ex As System.Exception
                Throw ex
            Finally
                Util.ModificationHistory.AllowAddHistory = True
            End Try

            If Not Me.UndoRedoInProgress Then
                Util.ModificationHistory.AddHistoryEvent(New Framework.UndoSystem.ScaleUnitChangedEvent(Me, ePrevMass, eNewMass, ePrevDist, eNewDistance))
            End If
        End Sub

        Public Overridable Function FindStimulusByName(ByVal strName As String, Optional ByVal bThrowError As Boolean = True) As DataObjects.ExternalStimuli.Stimulus

            Dim doStim As DataObjects.ExternalStimuli.Stimulus
            For Each deEntry As DictionaryEntry In Me.ProjectStimuli
                doStim = DirectCast(deEntry.Value, DataObjects.ExternalStimuli.Stimulus)

                If doStim.Name = strName Then
                    Return doStim
                End If
            Next

            If bThrowError Then
                Throw New System.Exception("No stimulus with the name '" & strName & "' was found.")
            End If

            Return Nothing
        End Function

#End Region

#Region " Plug-in-Module Management "

        Protected Overridable Sub CatalogPluginModules()
            Dim tpClass As Type
            Dim strFile As String
            Dim strFailedLoad As String = ""
            Dim iFailedLoad As Integer = 0

            Try
                Me.Logger.LogMsg(Interfaces.Logger.enumLogLevel.Debug, "Beginning to catalog plugin modules")

                Dim aryFileNames As New ArrayList
                Dim bAddModule As Boolean = False

                m_aryAllDataObjects.Clear()
                m_aryNeuralModules.Clear()
                m_aryNeuralModules.Clear()
                m_aryPlugInAssemblies.Clear()
                m_aryBehavioralNodes.Clear()
                m_aryBehavioralLinks.Clear()
                m_aryBodyPartTypes.Clear()
                m_aryRigidBodyTypes.Clear()
                m_aryJointTypes.Clear()
                m_aryToolPlugins.Clear()
                m_aryGainTypes.Clear()
                m_aryProgramModules.Clear()
                m_aryMicrocontrollers.Clear()
                m_aryExternalStimuli.Clear()

                'First find a list of all possible assemblies. It may be one or it may be a standard win32 dll. We will have to see later.
                Util.FindAssemblies(Me.ApplicationDirectory(), aryFileNames)

                Me.Logger.LogMsg(Interfaces.Logger.enumLogLevel.Debug, "Util.FindAssemblies")

                For Each oFile As Object In aryFileNames
                    strFile = DirectCast(oFile, String)
                    bAddModule = False

                    Try
                        Dim assemModule As System.Reflection.Assembly = Util.LoadAssembly(Util.GetFilePath(Me.ApplicationDirectory, strFile), False)
                        If Not assemModule Is Nothing Then

                            Me.Logger.LogMsg(Interfaces.Logger.enumLogLevel.Debug, "About to get types: " & assemModule.FullName)

                            Dim aryTypes() As Type = assemModule.GetTypes()

                            Me.Logger.LogMsg(Interfaces.Logger.enumLogLevel.Debug, "Starting to loop through: " & assemModule.FullName)

                            If assemModule.GetName().Name <> "UI Components" Then

                                For Each tpClass In aryTypes

                                    If Util.IsTypeOf(tpClass, GetType(AnimatTools.DataObjects.Behavior.Node)) Then
                                        Dim bnNode As DataObjects.Behavior.Node = CreateNode(assemModule, tpClass, Nothing)
                                        If Not bnNode Is Nothing Then
                                            m_aryBehavioralNodes.Add(bnNode)
                                            m_aryAllDataObjects.Add(bnNode)
                                            bAddModule = True
                                        End If
                                    ElseIf Util.IsTypeOf(tpClass, GetType(AnimatTools.DataObjects.Behavior.Link)) Then
                                        Dim blLink As DataObjects.Behavior.Link = CreateLink(assemModule, tpClass, Nothing)
                                        If Not blLink Is Nothing Then
                                            m_aryBehavioralLinks.Add(blLink)
                                            m_aryAllDataObjects.Add(blLink)
                                            bAddModule = True
                                        End If
                                    ElseIf Util.IsTypeOf(tpClass, GetType(AnimatTools.DataObjects.Behavior.NeuralModule)) Then
                                        Dim nmModule As DataObjects.Behavior.NeuralModule = CreateNeuralModule(assemModule, tpClass, Nothing)
                                        If Not nmModule Is Nothing Then
                                            m_aryNeuralModules.Add(nmModule.ClassName, nmModule)
                                            m_aryAllDataObjects.Add(nmModule)
                                            bAddModule = True
                                        End If
                                    ElseIf Util.IsTypeOf(tpClass, GetType(AnimatTools.Forms.Tools.ToolForm)) Then
                                        Dim frmTool As Forms.Tools.ToolForm = CreateToolForm(assemModule, tpClass, Nothing)
                                        If Not frmTool Is Nothing Then
                                            m_aryToolPlugins.Add(frmTool)
                                        End If
                                    ElseIf Util.IsTypeOf(tpClass, GetType(AnimatTools.DataObjects.Physical.RigidBody)) Then
                                        Try
                                            Dim doPart As AnimatTools.DataObjects.Physical.RigidBody = CreateRigidBody(assemModule, tpClass, Nothing)
                                            If Not doPart Is Nothing Then

                                                m_aryBodyPartTypes.Add(doPart)
                                                m_aryRigidBodyTypes.Add(doPart)
                                                m_aryAllDataObjects.Add(doPart)
                                                bAddModule = True
                                            End If
                                        Catch ex As System.Exception
                                            MessageBox.Show("Error loading rigid body part")
                                        End Try
                                    ElseIf Util.IsTypeOf(tpClass, GetType(AnimatTools.DataObjects.Physical.Joint)) Then
                                        Try
                                            Dim doPart As AnimatTools.DataObjects.Physical.Joint = CreateJoint(assemModule, tpClass, Nothing)
                                            If Not doPart Is Nothing Then

                                                m_aryBodyPartTypes.Add(doPart)
                                                m_aryJointTypes.Add(doPart)
                                                m_aryAllDataObjects.Add(doPart)
                                                bAddModule = True
                                            End If
                                        Catch ex As System.Exception
                                            MessageBox.Show("Error loading joint part")
                                        End Try
                                    ElseIf Util.IsTypeOf(tpClass, GetType(AnimatTools.DataObjects.Gain)) Then
                                        Dim doGain As DataObjects.Gain = CreateGain(assemModule, tpClass, Nothing)
                                        If Not doGain Is Nothing Then
                                            m_aryGainTypes.Add(doGain)
                                            m_aryAllDataObjects.Add(doGain)
                                        End If
                                    ElseIf Util.IsTypeOf(tpClass, GetType(AnimatTools.DataObjects.ProgramModule)) Then
                                        Dim doModule As DataObjects.ProgramModule = CreateProgramModule(assemModule, tpClass, Nothing)
                                        If Not doModule Is Nothing Then
                                            m_aryProgramModules.Add(doModule)
                                            m_aryAllDataObjects.Add(doModule)
                                        End If
                                    ElseIf Util.IsTypeOf(tpClass, GetType(AnimatTools.DataObjects.Physical.Microcontroller), False) Then
                                        Dim doModule As DataObjects.Physical.Microcontroller = CreateMicrocontroller(assemModule, tpClass, Nothing)
                                        If Not doModule Is Nothing Then
                                            m_aryMicrocontrollers.Add(doModule)
                                            m_aryAllDataObjects.Add(doModule)
                                        End If
                                    ElseIf Util.IsTypeOf(tpClass, GetType(AnimatTools.DataObjects.ExternalStimuli.Stimulus), False) Then
                                        Dim doStim As DataObjects.ExternalStimuli.Stimulus = CreateExternalStimuli(assemModule, tpClass, Nothing)
                                        If Not doStim Is Nothing Then
                                            m_aryExternalStimuli.Add(doStim)
                                            m_aryAllDataObjects.Add(doStim)
                                        End If
                                    End If
                                Next

                                tpClass = Nothing
                            End If

                            Me.Logger.LogMsg(Interfaces.Logger.enumLogLevel.Debug, "Finished looping through: " & assemModule.FullName)
                        End If

                        If bAddModule Then
                            m_aryPlugInAssemblies.Add(Util.RootNamespace(assemModule), assemModule)
                        End If

                    Catch ex As System.Exception
                        iFailedLoad = iFailedLoad + 1
                        If Not tpClass Is Nothing Then
                            strFailedLoad = strFailedLoad + vbTab & tpClass.FullName & vbCrLf
                        End If
                    End Try

                Next

                Me.Logger.LogMsg(Interfaces.Logger.enumLogLevel.Debug, "Initialize dataobjects after Application Start")

                InitializeDataObjectsAfterAppStart()

                Me.Logger.LogMsg(Interfaces.Logger.enumLogLevel.Debug, "Finished Looping through assemblies")

                CreateBehavioralPanels()

                Me.Logger.LogMsg(Interfaces.Logger.enumLogLevel.Info, "Finished cataloging plugin modules")

            Catch ex As System.Exception
                If Not tpClass Is Nothing Then
                    Me.Logger.LogMsg(Interfaces.Logger.enumLogLevel.Debug, "Error in CatalogPluginModules " & tpClass.FullName)
                Else
                    Me.Logger.LogMsg(Interfaces.Logger.enumLogLevel.Debug, "Error in CatalogPluginModules. Type is nothing")
                End If

                AnimatTools.Framework.Util.DisplayError(ex)
            Finally
                If iFailedLoad > 0 Then
                    Dim strMessage As String = "One or more of the vb.net plug-in modules for this application failed to load correctly. " & _
                                 "This could signal an error for a specific type of module, but more often this type of error means " & _
                                 "that the .Net framework or DirectX were not installed correctly on this computer. If this is a new " & _
                                 "installation of Animatlab then this is almost certainly the cause of this error. To fix this please go " & _
                                 "http:\\www.AnimatLab.com\Troubleshooting.html. There you can download both the .Net framework and " & _
                                 "DirectX and install them manually to fix this problem."
                    If strFailedLoad.Trim.Length > 0 Then
                        strMessage = strMessage & vbCrLf & vbCrLf & "Below is a list of the plug-ins that failed to load. If there are " & _
                                     "only a few items, and they appear to be in the same dll then this means that this problem is most " & _
                                     "likely isolated to a specific plug-in module, and you need to replace or repair that module in order " & _
                                     "to fix this error." & vbCrLf & strFailedLoad
                    End If

                    MessageBox.Show(strMessage)
                End If
            End Try

        End Sub

        Protected Overridable Sub InitializeDataObjectsAfterAppStart()

            Try
                For Each doObj As Framework.DataObject In m_aryAllDataObjects
                    doObj.InitAfterAppStart()
                Next

            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try
        End Sub

        Protected Overridable Function CreateNode(ByVal assemModule As System.Reflection.Assembly, ByVal tpClass As System.Type, ByVal doParent As AnimatTools.Framework.DataObject) As DataObjects.Behavior.Node

            Try
                If Not tpClass.IsAbstract Then
                    Dim bnNode As DataObjects.Behavior.Node = DirectCast(Util.LoadClass(assemModule, tpClass.FullName, doParent), DataObjects.Behavior.Node)
                    Return bnNode
                End If
            Catch ex As System.Exception
                If ex.Message <> "Cannot create an abstract class." Then
                    MessageBox.Show("CreateNode: " & tpClass.FullName)
                    AnimatTools.Framework.Util.DisplayError(ex)
                End If
            End Try

        End Function

        Protected Overridable Function CreateLink(ByVal assemModule As System.Reflection.Assembly, ByVal tpClass As System.Type, ByVal doParent As AnimatTools.Framework.DataObject) As DataObjects.Behavior.Link

            Try
                If Not tpClass.IsAbstract Then
                    Dim blLink As DataObjects.Behavior.Link = DirectCast(Util.LoadClass(assemModule, tpClass.FullName, doParent), DataObjects.Behavior.Link)
                    Return blLink
                End If
            Catch ex As System.Exception
                If ex.Message <> "Cannot create an abstract class." Then
                    MessageBox.Show("CreateLink: " & tpClass.FullName)
                    AnimatTools.Framework.Util.DisplayError(ex)
                End If
            End Try

        End Function

        Protected Overridable Function CreateNeuralModule(ByVal assemModule As System.Reflection.Assembly, ByVal tpClass As System.Type, ByVal doParent As AnimatTools.Framework.DataObject) As DataObjects.Behavior.NeuralModule

            Try
                If Not tpClass.IsAbstract Then
                    Dim nmModule As DataObjects.Behavior.NeuralModule = DirectCast(Util.LoadClass(assemModule, tpClass.FullName, doParent), DataObjects.Behavior.NeuralModule)
                    Return nmModule
                End If
            Catch ex As System.Exception
                If ex.Message <> "Cannot create an abstract class." Then
                    MessageBox.Show("CreateNeuralModule: " & tpClass.FullName)
                    AnimatTools.Framework.Util.DisplayError(ex)
                End If
            End Try

        End Function

        Protected Overridable Function CreateToolForm(ByVal assemModule As System.Reflection.Assembly, ByVal tpClass As System.Type, ByVal doParent As AnimatTools.Framework.DataObject) As Forms.Tools.ToolForm

            Try
                If Not tpClass.IsAbstract Then
                    Dim frmTool As Forms.Tools.ToolForm = DirectCast(Util.LoadClass(assemModule, tpClass.FullName), Forms.Tools.ToolForm)
                    Return frmTool
                End If
            Catch ex As System.Exception
                If ex.Message <> "Cannot create an abstract class." Then
                    MessageBox.Show("CreateToolForm: " & tpClass.FullName)
                    AnimatTools.Framework.Util.DisplayError(ex)
                End If
            End Try

        End Function

        Protected Overridable Function CreateRigidBody(ByVal assemModule As System.Reflection.Assembly, ByVal tpClass As System.Type, ByVal doParent As AnimatTools.Framework.DataObject) As DataObjects.Physical.RigidBody

            Try
                If Not tpClass.IsAbstract Then
                    Dim doBody As DataObjects.Physical.RigidBody = DirectCast(Util.LoadClass(assemModule, tpClass.FullName, doParent), DataObjects.Physical.RigidBody)
                    Return doBody
                End If
            Catch ex As System.Exception
                If ex.Message <> "Cannot create an abstract class." Then
                    MessageBox.Show("CreateRigidBody: " & tpClass.FullName)
                    AnimatTools.Framework.Util.DisplayError(ex)
                End If
            End Try

        End Function

        Protected Overridable Function CreateJoint(ByVal assemModule As System.Reflection.Assembly, ByVal tpClass As System.Type, ByVal doParent As AnimatTools.Framework.DataObject) As DataObjects.Physical.Joint

            Try
                If Not tpClass.IsAbstract Then
                    Dim doJoint As DataObjects.Physical.Joint = DirectCast(Util.LoadClass(assemModule, tpClass.FullName, doParent), DataObjects.Physical.Joint)
                    Return doJoint
                End If
            Catch ex As System.Exception
                If ex.Message <> "Cannot create an abstract class." Then
                    MessageBox.Show("CreateJoint: " & tpClass.FullName)
                    AnimatTools.Framework.Util.DisplayError(ex)
                End If
            End Try

        End Function

        Protected Overridable Function CreateGain(ByVal assemModule As System.Reflection.Assembly, ByVal tpClass As System.Type, ByVal doParent As AnimatTools.Framework.DataObject) As DataObjects.Gain

            Try
                If Not tpClass.IsAbstract Then
                    Dim doGain As DataObjects.Gain = DirectCast(Util.LoadClass(assemModule, tpClass.FullName, doParent), DataObjects.Gain)
                    Return doGain
                End If
            Catch ex As System.Exception
                If ex.Message <> "Cannot create an abstract class." Then
                    MessageBox.Show("CreateGain: " & tpClass.FullName)
                    AnimatTools.Framework.Util.DisplayError(ex)
                End If
            End Try

        End Function

        Protected Overridable Function CreateProgramModule(ByVal assemModule As System.Reflection.Assembly, ByVal tpClass As System.Type, ByVal doParent As AnimatTools.Framework.DataObject) As DataObjects.ProgramModule

            Try
                If Not tpClass.IsAbstract Then
                    Dim doModule As DataObjects.ProgramModule = DirectCast(Util.LoadClass(assemModule, tpClass.FullName, doParent), DataObjects.ProgramModule)
                    Return doModule
                End If
            Catch ex As System.Exception
                If ex.Message <> "Cannot create an abstract class." Then
                    MessageBox.Show("CreateProgramModule: " & tpClass.FullName)
                    AnimatTools.Framework.Util.DisplayError(ex)
                End If
            End Try

        End Function

        Protected Overridable Function CreateMicrocontroller(ByVal assemModule As System.Reflection.Assembly, ByVal tpClass As System.Type, ByVal doParent As AnimatTools.Framework.DataObject) As DataObjects.Physical.Microcontroller

            Try
                If Not tpClass.IsAbstract Then
                    Dim doModule As DataObjects.Physical.Microcontroller = DirectCast(Util.LoadClass(assemModule, tpClass.FullName, doParent), DataObjects.Physical.Microcontroller)
                    Return doModule
                End If
            Catch ex As System.Exception
                If ex.Message <> "Cannot create an abstract class." Then
                    MessageBox.Show("CreateProgramModule: " & tpClass.FullName)
                    AnimatTools.Framework.Util.DisplayError(ex)
                End If
            End Try

        End Function

        Protected Overridable Function CreateExternalStimuli(ByVal assemModule As System.Reflection.Assembly, ByVal tpClass As System.Type, ByVal doParent As AnimatTools.Framework.DataObject) As DataObjects.ExternalStimuli.Stimulus

            Try
                If Not tpClass.IsAbstract Then
                    Dim doStim As DataObjects.ExternalStimuli.Stimulus = DirectCast(Util.LoadClass(assemModule, tpClass.FullName, doParent), DataObjects.ExternalStimuli.Stimulus)
                    Return doStim
                End If
            Catch ex As System.Exception
                If ex.Message <> "Cannot create an abstract class." Then
                    MessageBox.Show("CreateExternalStimuli: " & tpClass.FullName)
                    AnimatTools.Framework.Util.DisplayError(ex)
                End If
            End Try

        End Function

        Protected Overridable Sub CreateBehavioralPanels()

            Try
                m_aryBehavioralPanels.Clear()
                m_aryAlphabeticalBehavioralPanels.Clear()

                'Lets create the alphabetical band
                Dim pdAlphabetical As New AnimatTools.DataObjects.Behavior.PanelData
                pdAlphabetical.m_assemModule = Nothing
                m_aryBehavioralPanels.Add("AnimatTools.Alphabetical", pdAlphabetical)
                pdAlphabetical.m_strPanelName = "Alphabetical Tools"

                'Now lets go through and add icon panels for each of those modules.
                Dim assemModule As System.Reflection.Assembly
                Dim pdPanel As DataObjects.Behavior.PanelData
                For Each deEntry As DictionaryEntry In m_aryPlugInAssemblies
                    assemModule = DirectCast(deEntry.Value, System.Reflection.Assembly)

                    pdPanel = New DataObjects.Behavior.PanelData
                    pdPanel.m_strPanelName = Util.ModuleName(assemModule)
                    pdPanel.m_assemModule = assemModule
                    m_aryBehavioralPanels.Add(DirectCast(deEntry.Key, String), pdPanel)
                Next

                'Now lets make lists of the nodes that will go into the panels
                For Each bnNode As DataObjects.Behavior.Node In m_aryBehavioralNodes
                    If bnNode.IsDisplayedInIconPanel Then
                        pdPanel = DirectCast(m_aryBehavioralPanels(bnNode.AssemblyModuleName), DataObjects.Behavior.PanelData)
                        pdPanel.m_aryNodes.Add(bnNode)
                        pdAlphabetical.m_aryNodes.Add(bnNode)
                    End If
                Next

                'Now we need to go through and sort each node list in the panel datas so that they are alphabetical
                Dim aryIDs As New ArrayList
                For Each deEntry As DictionaryEntry In m_aryBehavioralPanels
                    pdPanel = DirectCast(deEntry.Value, DataObjects.Behavior.PanelData)

                    If pdPanel.m_aryNodes.Count > 0 Then
                        pdPanel.m_aryNodes.Sort(New AnimatTools.Collections.Comparers.CompareNodeNames)
                        m_aryAlphabeticalBehavioralPanels.Add(pdPanel)
                    Else
                        Dim strID As String = DirectCast(deEntry.Key, String)
                        If strID <> "AnimatTools.Alphabetical" Then
                            aryIDs.Add(deEntry.Key)
                        End If
                    End If
                Next

                'Remove any panels that do not have any nodes in them.
                For Each strID As String In aryIDs
                    m_aryBehavioralPanels.Remove(strID)
                Next

                'Now sort the panels
                m_aryAlphabeticalBehavioralPanels.Sort(New AnimatTools.Collections.Comparers.ComparePanelNames)


            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try

        End Sub

        Protected Sub PopulateToolbox()
            Try
                If m_frmToolbox Is Nothing Then Return

                'Only add this if it has not already been added.
                If m_frmToolbox.ctrlOutlookbar.Controls.Count = 0 Then
                    m_ipToolPanel = New IconPanel
                    'ipPanel.IconHeight = 55
                    m_frmToolbox.ctrlOutlookbar.AddBand("Toolbox", m_ipToolPanel)

                    For Each frmTool As Forms.Tools.ToolForm In m_aryToolPlugins
                        Dim panelIcon As PanelIcon = m_ipToolPanel.AddIcon(frmTool.Name, frmTool.Image, frmTool.DragImage, frmTool)
                        AddHandler panelIcon.DoubleClickIcon, AddressOf Me.OnDoubleClickTool
                    Next

                End If

            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try

        End Sub

#End Region

#Region " Menu/Toolbar Creation "

        Protected Overridable Sub CreateImageManager()

            Dim myAssembly As System.Reflection.Assembly
            myAssembly = System.Reflection.Assembly.Load("AnimatTools")

            m_mgrSmallImages = New AnimatTools.Framework.ImageManager
            m_mgrSmallImages.ImageList.ImageSize = New Size(16, 16)
            m_mgrSmallImages.AddImage(myAssembly, "AnimatTools.Open_Toolbar.gif")
            m_mgrSmallImages.AddImage(myAssembly, "AnimatTools.Save_Toolbar.gif")
            m_mgrSmallImages.AddImage(myAssembly, "AnimatTools.NewProject.gif")
            m_mgrSmallImages.AddImage(myAssembly, "AnimatTools.ProjectWorkspace.gif")
            m_mgrSmallImages.AddImage(myAssembly, "AnimatTools.ProjectProperties.gif")
            m_mgrSmallImages.AddImage(myAssembly, "AnimatTools.Simulate.gif")
            m_mgrSmallImages.AddImage(myAssembly, "AnimatTools.SimulationController.gif")
            m_mgrSmallImages.AddImage(myAssembly, "AnimatTools.Undo.gif")
            m_mgrSmallImages.AddImage(myAssembly, "AnimatTools.UndoGrey.gif")
            m_mgrSmallImages.AddImage(myAssembly, "AnimatTools.Redo.gif")
            m_mgrSmallImages.AddImage(myAssembly, "AnimatTools.RedoGrey.gif")
            m_mgrSmallImages.AddImage(myAssembly, "AnimatTools.HelpContents.gif")
            m_mgrSmallImages.AddImage(myAssembly, "AnimatTools.HelpSearch.gif")
            m_mgrSmallImages.AddImage(myAssembly, "AnimatTools.TechnicalSupport.gif")
            m_mgrSmallImages.AddImage(myAssembly, "AnimatTools.ExportStandalone.gif")
            m_mgrSmallImages.AddImage(myAssembly, "AnimatTools.CutFromWorkspace.gif")
            m_mgrSmallImages.AddImage(myAssembly, "AnimatTools.AddDataTool.gif")
            m_mgrSmallImages.AddImage(myAssembly, "AnimatTools.AddOrganism.gif")
            m_mgrSmallImages.AddImage(myAssembly, "AnimatTools.AddStructure.gif")
            m_mgrSmallImages.AddImage(myAssembly, "AnimatTools.AddGround.gif")
            m_mgrSmallImages.AddImage(myAssembly, "AnimatTools.AddWater.gif")
            m_mgrSmallImages.AddImage(myAssembly, "AnimatTools.Expand.gif")
            m_mgrSmallImages.AddImage(myAssembly, "AnimatTools.Collapse.gif")
            m_mgrSmallImages.AddImage(myAssembly, "AnimatTools.AddStimulus.gif")
            m_mgrSmallImages.AddImage(myAssembly, "AnimatTools.Delete.gif")
            m_mgrSmallImages.AddImage(myAssembly, "AnimatTools.Cut.gif")
            m_mgrSmallImages.AddImage(myAssembly, "AnimatTools.Copy.gif")
            m_mgrSmallImages.AddImage(myAssembly, "AnimatTools.Paste.gif")
            m_mgrSmallImages.AddImage(myAssembly, "AnimatTools.preference.gif")
            m_mgrSmallImages.AddImage(myAssembly, "AnimatTools.Swap.gif")

            Me.Icon = m_mgrSmallImages.LoadIcon(myAssembly, "AnimatTools.Crayfish.ico")

            'm_mgrLargeImages = New AnimatTools.Framework.ImageManager
            'm_mgrLargeImages.ImageList.ImageSize = New Size(25, 25)
            'm_mgrLargeImages.AddImage(myAssembly, "AnimatTools.ProjectOpenLarge.gif")
            'm_mgrLargeImages.AddImage(myAssembly, "AnimatTools.ProjectSaveLarge.gif")
            'm_mgrLargeImages.AddImage(myAssembly, "AnimatTools.SimulateLarge.gif")

            m_mgrLargeImages = New AnimatTools.Framework.ImageManager
            m_mgrLargeImages.ImageList.ImageSize = New Size(16, 16)
            m_mgrLargeImages.AddImage(myAssembly, "AnimatTools.NewProject.gif")
            m_mgrLargeImages.AddImage(myAssembly, "AnimatTools.Open_Toolbar.gif")
            m_mgrLargeImages.AddImage(myAssembly, "AnimatTools.Save_Toolbar.gif")
            m_mgrLargeImages.AddImage(myAssembly, "AnimatTools.CutFromWorkspace.gif")
            m_mgrLargeImages.AddImage(myAssembly, "AnimatTools.AddDataTool.gif")
            m_mgrLargeImages.AddImage(myAssembly, "AnimatTools.AddOrganism.gif")
            m_mgrLargeImages.AddImage(myAssembly, "AnimatTools.AddStructure.gif")
            m_mgrLargeImages.AddImage(myAssembly, "AnimatTools.AddGround.gif")
            m_mgrLargeImages.AddImage(myAssembly, "AnimatTools.AddWater.gif")
            m_mgrLargeImages.AddImage(myAssembly, "AnimatTools.Expand.gif")
            m_mgrLargeImages.AddImage(myAssembly, "AnimatTools.Collapse.gif")
            m_mgrLargeImages.AddImage(myAssembly, "AnimatTools.AddStimulus.gif")
            m_mgrLargeImages.AddImage(myAssembly, "AnimatTools.Delete.gif")
            m_mgrLargeImages.AddImage(myAssembly, "AnimatTools.preference.gif")
            'm_mgrLargeImages.AddImage(myAssembly, "AnimatTools.SimulateLarge.gif")

        End Sub

        Public Overrides Function CreateMenu() As MenuControl
            Return CreateDefaultMenu()
        End Function

        Public Overrides Function CreateToolbar(ByRef menuDefault As MenuControl) As Crownwood.Magic.Toolbars.ToolbarControl
            Return CreateDefaultToolbar(menuDefault)
        End Function

        Public Overridable Sub ResetToolbars(Optional ByRef menuNew As MenuControl = Nothing, _
                                             Optional ByRef barNew As Crownwood.Magic.Toolbars.ToolbarControl = Nothing)
            Me.SuspendLayout()

            If Not barNew Is Nothing Then
                CurrentToolbar = barNew
            Else
                CurrentToolbar = m_barDefault
            End If

            If Not menuNew Is Nothing Then
                CurrentMenu = menuNew
            Else
                CurrentMenu = m_menuDefault
            End If

            Me.ResumeLayout()
        End Sub

        Public Overridable Function CreateDefaultMenu() As MenuControl
            Dim newMenu As New MenuControl

            newMenu.MdiContainer = Me

            'Lets create the file menu
            Dim mcFile As New MenuCommand("File", "File")
            mcFile.Description = "File Menu Commands"

            Dim mcNewProject As New MenuCommand("New Project", "NewProject", m_mgrSmallImages.ImageList, _
                     m_mgrSmallImages.GetImageIndex("AnimatTools.NewProject.gif"), Shortcut.CtrlN, _
                     New EventHandler(AddressOf Me.OnNewProject))

            Dim mcOpenProject As New MenuCommand("Open Project", "OpenProject", m_mgrSmallImages.ImageList, _
                     m_mgrSmallImages.GetImageIndex("AnimatTools.Open_Toolbar.gif"), Shortcut.CtrlO, _
                     New EventHandler(AddressOf Me.OnOpenProject))

            Dim mcCloseProject As New MenuCommand("Close Project", "CloseProject", New EventHandler(AddressOf Me.OnCloseProject))

            Dim mcSaveProject As New MenuCommand("Save Project", "SaveProject", m_mgrSmallImages.ImageList, _
                     m_mgrSmallImages.GetImageIndex("AnimatTools.Save_Toolbar.gif"), Shortcut.CtrlS, _
                     New EventHandler(AddressOf Me.OnSaveProject))

            Dim mcSaveProjectAs As New MenuCommand("Save Project As", "SaveProjectAs", New EventHandler(AddressOf Me.OnSaveProjectAs))

            Dim mcExportForStandAloneSim As New MenuCommand("Export Standalone Sim", "ExportStandaloneSim", m_mgrSmallImages.ImageList, _
                                              m_mgrSmallImages.GetImageIndex("AnimatTools.ExportStandalone.gif"), New EventHandler(AddressOf Me.OnExportStandaloneSim))

            Dim mcSep1 As MenuCommand = New MenuCommand("-")
            Dim mcExit As New MenuCommand("Exit", "Exit", New EventHandler(AddressOf Me.OnExit))

            'Dim mcNewFloat As New MenuCommand("New Float", New EventHandler(AddressOf Me.OnNewFloat))
            'Dim mcNewGraph As New MenuCommand("New Graph", New EventHandler(AddressOf Me.OnNewGraph))
            'Dim mcNewBehavioral As New MenuCommand("New Behavioral Editor", New EventHandler(AddressOf Me.OnNewBehavioralEditor))
            'Dim mcNewSimulator As New MenuCommand("NewSimulator", New EventHandler(AddressOf Me.OnNewSimulator))

            'mcFile.MenuCommands.AddRange(New MenuCommand() {mcNewProject, mcOpenProject, mcSaveProject, mcNewFloat, mcNewGraph, mcNewBehavioral, mcNewSimulator})
            mcFile.MenuCommands.AddRange(New MenuCommand() {mcNewProject, mcOpenProject, mcSaveProject, mcSaveProjectAs, mcCloseProject, mcExportForStandAloneSim, mcSep1, mcExit})
            newMenu.MenuCommands.Add(mcFile)

            Dim mcEdit As New MenuCommand("Edit", "Edit")
            mcEdit.Description = "Edit Diagram Commands"
            Dim mcUndo As New MenuCommand("Undo", "Undo", m_mgrSmallImages.ImageList, _
                                              m_mgrSmallImages.GetImageIndex("AnimatTools.Undo.gif"), _
                                              System.Windows.Forms.Shortcut.CtrlZ, New EventHandler(AddressOf Me.OnUndo))
            Dim mcRedo As New MenuCommand("Redo", "Redo", m_mgrSmallImages.ImageList, _
                                              m_mgrSmallImages.GetImageIndex("AnimatTools.Redo.gif"), _
                                              System.Windows.Forms.Shortcut.CtrlY, New EventHandler(AddressOf Me.OnRedo))
            Dim mcCutWorkspace As New MenuCommand("Cut Workspace", "CutWorkspace", m_mgrSmallImages.ImageList, _
                                              m_mgrSmallImages.GetImageIndex("AnimatTools.CutFromWorkspace.gif"), _
                                              New EventHandler(AddressOf Me.OnCutWorkspace))
            Dim mcSep2 As MenuCommand = New MenuCommand("-")
            Dim mcAddOrganism As New MenuCommand("Add Organism", "AddOrganism", m_mgrSmallImages.ImageList, _
                                              m_mgrSmallImages.GetImageIndex("AnimatTools.AddOrganism.gif"), _
                                              New EventHandler(AddressOf Me.OnAddOrganism))
            Dim mcAddStructure As New MenuCommand("Add Structure", "AddStructure", m_mgrSmallImages.ImageList, _
                                              m_mgrSmallImages.GetImageIndex("AnimatTools.AddStructure.gif"), _
                                              New EventHandler(AddressOf Me.OnAddStructure))
            Dim mcAddGround As New MenuCommand("Add Ground", "AddGround", m_mgrSmallImages.ImageList, _
                                              m_mgrSmallImages.GetImageIndex("AnimatTools.AddGround.gif"), _
                                              New EventHandler(AddressOf Me.OnAddGround))
            Dim mcAddWater As New MenuCommand("Add Water", "AddWater", m_mgrSmallImages.ImageList, _
                                              m_mgrSmallImages.GetImageIndex("AnimatTools.AddWater.gif"), _
                                              New EventHandler(AddressOf Me.OnAddWater))
            Dim mcAddDataTool As New MenuCommand("Add Data Tool", "AddDataTool", m_mgrSmallImages.ImageList, _
                                              m_mgrSmallImages.GetImageIndex("AnimatTools.AddDataTool.gif"), _
                                              New EventHandler(AddressOf Me.OnAddDataTool))

            Dim mcProgramModule As New MenuCommand("Run Program Module", "ProgramModule", New EventHandler(AddressOf Me.OnRunProgramModule))

            If Not Util.Environment Is Nothing AndAlso Not Util.Environment.GroundSurface Is Nothing Then
                mcAddGround.Enabled = False
            End If

            If Not Util.Environment Is Nothing AndAlso Not Util.Environment.WaterSurface Is Nothing Then
                mcAddWater.Enabled = False
            End If

            mcEdit.MenuCommands.AddRange(New MenuCommand() {mcUndo, mcRedo, mcProgramModule, mcCutWorkspace, mcSep2, mcAddOrganism, mcAddStructure, mcAddGround, mcAddWater, mcAddDataTool})
            newMenu.MenuCommands.Add(mcEdit)

            'Lets create the view menu
            Dim mcView As New MenuCommand("View", "View")
            mcView.Description = "View Toolbar Properties"

            Dim mcpreference As New MenuCommand("Preferences", "Preferences", m_mgrSmallImages.ImageList, _
                                  m_mgrSmallImages.GetImageIndex("AnimatTools.preference.gif"), _
                                  New EventHandler(AddressOf Me.Onpreference))

            Dim mcRunSimulation As New MenuCommand("Run Simulation", "RunSimulation", m_mgrSmallImages.ImageList, _
                     m_mgrSmallImages.GetImageIndex("AnimatTools.Simulate.gif"), Shortcut.F5, _
                     New EventHandler(AddressOf Me.OnRunSimulation))

            Dim mcSimualateWnd As New MenuCommand("Open Simulation Window", "SimulationWindow", m_mgrSmallImages.ImageList, _
                     m_mgrSmallImages.GetImageIndex("AnimatTools.Simulate.gif"), Shortcut.F6, _
                     New EventHandler(AddressOf Me.OnOpenSimulationWindow))

            Dim mcToggleSimualation As New MenuCommand("Toggle Simulation", "ToggleSimulation", Shortcut.F7, New EventHandler(AddressOf Me.OnPauseSimulation))

            mcView.MenuCommands.Add(mcSimualateWnd)
            mcView.MenuCommands.Add(mcRunSimulation)
            mcView.MenuCommands.Add(mcToggleSimualation)
            mcView.MenuCommands.Add(mcpreference)

            If m_bProjectIsOpen Then
                mcSaveProject.Enabled = True
                mcSaveProjectAs.Enabled = True
                mcCloseProject.Enabled = True
                mcExportForStandAloneSim.Enabled = True
                mcSimualateWnd.Enabled = True
                mcRunSimulation.Enabled = True
                mcToggleSimualation.Enabled = True
                mcUndo.Enabled = True
                mcRedo.Enabled = True
                mcProgramModule.Enabled = True
                mcCutWorkspace.Enabled = True
                mcAddOrganism.Enabled = True
                mcAddStructure.Enabled = True
                mcAddGround.Enabled = True
                mcAddWater.Enabled = True
                mcAddDataTool.Enabled = True
                mcpreference.Enabled = True
            Else
                mcSaveProject.Enabled = False
                mcSaveProjectAs.Enabled = False
                mcCloseProject.Enabled = False
                mcExportForStandAloneSim.Enabled = False
                mcSimualateWnd.Enabled = False
                mcRunSimulation.Enabled = False
                mcToggleSimualation.Enabled = False
                mcUndo.Enabled = False
                mcRedo.Enabled = False
                mcProgramModule.Enabled = False
                mcCutWorkspace.Enabled = False
                mcAddOrganism.Enabled = False
                mcAddStructure.Enabled = False
                mcAddGround.Enabled = False
                mcAddWater.Enabled = False
                mcAddDataTool.Enabled = False
                mcpreference.Enabled = True
            End If

            For Each ctDock As Content In m_dockManager.Contents
                If ctDock.BackgroundForm Then
                    Dim mcViewForm As New MenuCommand(ctDock.Title, ctDock.Title, ctDock.ImageList, _
                             ctDock.ImageIndex, New EventHandler(AddressOf Me.OnViewDockingForm))
                    mcView.MenuCommands.Add(mcViewForm)
                End If
            Next

            newMenu.MenuCommands.Add(mcView)

            Dim mcHelp As New MenuCommand("Help", "Help")
            mcHelp.Description = "Help Commands"
            Dim mcContents As New MenuCommand("Contents", "Contents", m_mgrSmallImages.ImageList, _
                                              m_mgrSmallImages.GetImageIndex("AnimatTools.HelpContents.gif"), _
                                              System.Windows.Forms.Shortcut.CtrlF1, New EventHandler(AddressOf Me.OnHelpContents))
            Dim mcSearch As New MenuCommand("Search", "Search", m_mgrSmallImages.ImageList, _
                                              m_mgrSmallImages.GetImageIndex("AnimatTools.HelpSearch.gif"), _
                                              System.Windows.Forms.Shortcut.CtrlF3, New EventHandler(AddressOf Me.OnHelpSearch))
            Dim mcTechSupport As New MenuCommand("Technical Support", "TechnicalSupport", m_mgrSmallImages.ImageList, _
                                              m_mgrSmallImages.GetImageIndex("AnimatTools.TechnicalSupport.gif"), New EventHandler(AddressOf Me.OnTechnicalSupport))
            Dim mcAbout As New MenuCommand("About AnimatLab", "AboutAnimatLab", New EventHandler(AddressOf Me.OnAboutAnimatLab))

            mcHelp.MenuCommands.AddRange(New MenuCommand() {mcContents, mcSearch, mcTechSupport, mcAbout})
            newMenu.MenuCommands.Add(mcHelp)

            m_aryDefaultMenus.Add(newMenu)

            Return newMenu
        End Function

        Public Overridable Sub AddItemToDefaultMenu(ByVal strMainMenu As String, _
                                                    ByVal strSubMenu As String, _
                                                    ByVal strMenuText As String, _
                                                    ByVal strMenuTag As String, _
                                                    ByVal strMenuImage As String, _
                                                    ByRef menuHandler As EventHandler)

            Dim mcMain As MenuCommand, mcSubMenu As MenuCommand, mcNew As MenuCommand
            Dim iIndex As Integer
            For Each menuDefault As MenuControl In m_aryDefaultMenus
                'Now lets try and find the specified position where we want to add this menu command
                'First find the main menu item
                Try
                    mcMain = menuDefault.MenuCommands(strMainMenu)
                Catch ex As System.Exception
                    Throw New System.Exception("The main menu '" & strMainMenu & "' was not found while attempting to add " & _
                                               "the '" & strMenuText & "' menu command to the default menus.")
                End Try

                If strSubMenu.Trim.Length > 0 Then
                    Try
                        mcSubMenu = mcMain.MenuCommands(strSubMenu)
                    Catch ex As System.Exception
                        Throw New System.Exception("The main menu '" & strMainMenu & "' was not found while attempting to add " & _
                                                   "the '" & strMenuText & "' menu command to the default menus.")
                    End Try

                    iIndex = mcMain.MenuCommands.IndexOf(mcSubMenu)
                Else
                    iIndex = 0
                End If

                If strMenuImage.Trim.Length > 0 Then
                    mcNew = New MenuCommand(strMenuText, strMenuTag, m_mgrSmallImages.ImageList, _
                                            m_mgrSmallImages.GetImageIndex(strMenuImage), menuHandler)

                Else
                    mcNew = New MenuCommand(strMenuText, strMenuTag, menuHandler)
                End If

                mcMain.MenuCommands.Insert(iIndex, mcNew)
            Next

        End Sub

        Public Overridable Sub EnableDefaultMenuItem(ByVal strMainMenu As String, _
                                                     ByVal strMenuItem As String, _
                                                     ByVal bEnable As Boolean)

            Dim mcMain As MenuCommand, mcSubMenu As MenuCommand, mcNew As MenuCommand
            Dim iIndex As Integer
            For Each menuDefault As MenuControl In m_aryDefaultMenus
                'Now lets try and find the specified position where we want to add this menu command
                'First find the main menu item
                Try
                    mcMain = menuDefault.MenuCommands(strMainMenu)
                Catch ex As System.Exception
                    Throw New System.Exception("The main menu '" & strMainMenu & "' was not found while attempting to enable item.")
                End Try

                mcMain.MenuCommands(strMenuItem).Enabled = bEnable
            Next

        End Sub

        Public Overridable Sub EnableDefaultToolbarItem(ByVal strID As String, _
                                                        ByVal bEnable As Boolean)

            Dim iIndex As Integer
            For Each barMain As Crownwood.Magic.Toolbars.ToolbarControl In m_aryDefaultToolbars

                For Each btButton As ToolBarButton In barMain.Buttons
                    If btButton.ToolTipText = strID Then
                        btButton.Enabled = bEnable
                    End If
                Next
            Next

        End Sub

        Public Overridable Function CreateDefaultToolbar(ByRef menuDefault As MenuControl) As Crownwood.Magic.Toolbars.ToolbarControl
            Dim newBar As New ToolbarControl

            If menuDefault Is Nothing Then
                Throw New System.Exception("The default menu was not defined. " & _
                  "You can not create a default toolbar without a reference to the menu control.")
            End If

            newBar.ImageList = m_mgrLargeImages.ImageList

            Dim btnNewProject As New ToolBarButton
            btnNewProject.ImageIndex = m_mgrLargeImages.GetImageIndex("AnimatTools.NewProject.gif")
            btnNewProject.ToolTipText = "New Project"

            Dim btnOpenProject As New ToolBarButton
            btnOpenProject.ImageIndex = m_mgrLargeImages.GetImageIndex("AnimatTools.Open_Toolbar.gif")
            btnOpenProject.ToolTipText = "Open Project"

            Dim btnSaveProject As New ToolBarButton
            btnSaveProject.ImageIndex = m_mgrLargeImages.GetImageIndex("AnimatTools.Save_Toolbar.gif")
            btnSaveProject.ToolTipText = "Save Project"

            Dim btnCutWorkspace As New ToolBarButton
            btnCutWorkspace.ImageIndex = m_mgrLargeImages.GetImageIndex("AnimatTools.CutFromWorkspace.gif")
            btnCutWorkspace.ToolTipText = "Cut Item from Workspace"

            Dim btnAddOrganism As New ToolBarButton
            btnAddOrganism.ImageIndex = m_mgrLargeImages.GetImageIndex("AnimatTools.AddOrganism.gif")
            btnAddOrganism.ToolTipText = "Add Organism"

            Dim btnAddStructure As New ToolBarButton
            btnAddStructure.ImageIndex = m_mgrLargeImages.GetImageIndex("AnimatTools.AddStructure.gif")
            btnAddStructure.ToolTipText = "Add Structure"

            Dim btnAddGround As New ToolBarButton
            btnAddGround.ImageIndex = m_mgrLargeImages.GetImageIndex("AnimatTools.AddGround.gif")
            btnAddGround.ToolTipText = "Add Ground"

            Dim btnAddWater As New ToolBarButton
            btnAddWater.ImageIndex = m_mgrLargeImages.GetImageIndex("AnimatTools.AddWater.gif")
            btnAddWater.ToolTipText = "Add Water"

            Dim btnAddDataTool As New ToolBarButton
            btnAddDataTool.ImageIndex = m_mgrLargeImages.GetImageIndex("AnimatTools.AddDataTool.gif")
            btnAddDataTool.ToolTipText = "Add Data Tool"

            Dim btnPreference As New ToolBarButton
            btnPreference.ImageIndex = m_mgrLargeImages.GetImageIndex("AnimatTools.preference.gif")
            btnPreference.ToolTipText = "Preferences"

            If m_bProjectIsOpen Then
                btnSaveProject.Enabled = True
                btnCutWorkspace.Enabled = True
                btnAddOrganism.Enabled = True
                btnAddStructure.Enabled = True
                btnAddGround.Enabled = True
                btnAddWater.Enabled = True
                btnAddDataTool.Enabled = True
            Else
                btnSaveProject.Enabled = False
                btnCutWorkspace.Enabled = False
                btnAddOrganism.Enabled = False
                btnAddStructure.Enabled = False
                btnAddGround.Enabled = False
                btnAddWater.Enabled = False
                btnAddDataTool.Enabled = False
            End If

            If Not Util.Environment Is Nothing AndAlso Not Util.Environment.GroundSurface Is Nothing Then
                btnAddGround.Enabled = False
            End If

            If Not Util.Environment Is Nothing AndAlso Not Util.Environment.WaterSurface Is Nothing Then
                btnAddWater.Enabled = False
            End If

            'Dim btnSimulationWnd As New ToolBarButton
            'btnSimulationWnd.ImageIndex = m_mgrLargeImages.GetImageIndex("AnimatTools.SimulateLarge.gif")
            'btnSimulationWnd.ToolTipText = "Opens the simulation window"

            newBar.Appearance = ToolBarAppearance.Flat
            newBar.Buttons.AddRange(New ToolBarButton() {btnNewProject, btnOpenProject, btnSaveProject, btnAddOrganism, btnAddStructure, btnAddGround, btnAddWater, btnAddDataTool, btnCutWorkspace})

            'You can only set the menu items for the buttons AFTER you add the buttons to the toolbar!
            newBar.ButtonManager.SetButtonMenuItem(btnNewProject, menuDefault.MenuCommands.FindMenuCommand("NewProject"))
            newBar.ButtonManager.SetButtonMenuItem(btnOpenProject, menuDefault.MenuCommands.FindMenuCommand("OpenProject"))
            newBar.ButtonManager.SetButtonMenuItem(btnSaveProject, menuDefault.MenuCommands.FindMenuCommand("SaveProject"))
            newBar.ButtonManager.SetButtonMenuItem(btnCutWorkspace, menuDefault.MenuCommands.FindMenuCommand("CutWorkspace"))
            newBar.ButtonManager.SetButtonMenuItem(btnAddOrganism, menuDefault.MenuCommands.FindMenuCommand("AddOrganism"))
            newBar.ButtonManager.SetButtonMenuItem(btnAddStructure, menuDefault.MenuCommands.FindMenuCommand("AddStructure"))
            newBar.ButtonManager.SetButtonMenuItem(btnAddGround, menuDefault.MenuCommands.FindMenuCommand("AddGround"))
            newBar.ButtonManager.SetButtonMenuItem(btnAddWater, menuDefault.MenuCommands.FindMenuCommand("AddWater"))
            newBar.ButtonManager.SetButtonMenuItem(btnAddDataTool, menuDefault.MenuCommands.FindMenuCommand("AddDataTool"))
            newBar.ButtonManager.SetButtonMenuItem(btnPreference, menuDefault.MenuCommands.FindMenuCommand("Preferences"))
            'newBar.ButtonManager.SetButtonMenuItem(btnSimulationWnd, menuDefault.MenuCommands.FindMenuCommand("SimulationWindow"))

            newBar.DropDownArrows = True
            newBar.Location = New System.Drawing.Point(0, 0)
            newBar.Name = "DefaultToolbar"
            newBar.ShowToolTips = True
            newBar.Size = New System.Drawing.Size(292, 42)
            newBar.TabIndex = 1

            m_aryDefaultToolbars.Add(newBar)

            Return newBar
        End Function

#End Region

#Region " Project Creation "

        Public Overridable Sub ResetProject(ByVal bNewProject As Boolean)
            CloseProject(bNewProject)

            Dim afForm As AnimatForm

            'm_dsSimulation = DirectCast(Util.LoadClass(m_strPhysicsAssemblyName, m_strPhysicsClassName), AnimatTools.DataObjects.Simulation)
            m_wcWorkspaceContent = Nothing

            If bNewProject Then
                afForm = CreateDockingForm(m_dockManager, "AnimatTools.dll", _
                                  "AnimatTools.Forms.ProjectWorkspace", _
                                  "Project Workspace", "Workspace", True, _
                                  , m_wcWorkspaceContent, , , , 200)
                m_frmWorkspace = DirectCast(afForm, Forms.ProjectWorkspace)
                m_frmWorkspace.ctrlTreeView.Height = CInt(m_frmWorkspace.Height * 0.6)

                afForm = CreateDockingForm(m_dockManager, "AnimatTools.dll", _
                                "AnimatTools.Forms.Toolbox", _
                                "Project Toolbox", "Toolbox", True, , _
                                Nothing, m_wcWorkspaceContent)
                m_frmToolbox = DirectCast(afForm, Forms.Toolbox)

                afForm = CreateDockingForm(m_dockManager, "AnimatTools.dll", _
                                "AnimatTools.Forms.SimulationController", _
                                "Simulation Controller", "SimulationController", _
                                True, State.DockBottom, , , , , , 100)
                m_frmSimulationController = DirectCast(afForm, Forms.SimulationController)

                If bNewProject Then PopulateToolbox()

                m_dockManager.AutoHideWindow(m_wcWorkspaceContent)
            End If

            Me.Logger.LogMsg(Interfaces.Logger.enumLogLevel.Info, "Reset Project")
        End Sub


        'It is possible that if a project was saved with an earlier version of animatlab that a new toolbar may have been
        'added. If that is true it will not be part of the project config file. The docking bar created in ResetToolbars
        'will be deleted and it will not be recreated during the load. This method checks each of the major docking bars
        'after the load and makes sure they still exist. If it does not then it recreates it.
        Protected Sub VerifyToolbarsAfterLoad()

            Dim afForm As AnimatForm

            Dim ctDock As Content = m_dockManager.Contents("Project Workspace")
            If ctDock Is Nothing Then
                afForm = CreateDockingForm(m_dockManager, "AnimatTools.dll", _
                                  "AnimatTools.Forms.ProjectWorkspace", _
                                  "Project Workspace", "Workspace", True, _
                                  , m_wcWorkspaceContent, , , , 200)
                m_frmWorkspace = DirectCast(afForm, Forms.ProjectWorkspace)
                m_frmWorkspace.ctrlTreeView.Height = CInt(m_frmWorkspace.Height * 0.6)
            End If

            ctDock = m_dockManager.Contents("Project Toolbox")
            If ctDock Is Nothing Then
                afForm = CreateDockingForm(m_dockManager, "AnimatTools.dll", _
                                  "AnimatTools.Forms.Toolbox", _
                                  "Project Toolbox", "Toolbox", True, , _
                                  Nothing, Nothing)
                m_frmToolbox = DirectCast(afForm, Forms.Toolbox)
            End If

            ctDock = m_dockManager.Contents("Simulation Controller")
            If ctDock Is Nothing Then
                afForm = CreateDockingForm(m_dockManager, "AnimatTools.dll", _
                                  "AnimatTools.Forms.SimulationController", _
                                  "Simulation Controller", "SimulationController", _
                                  True, State.DockBottom, , , , , , 150)
                m_frmSimulationController = DirectCast(afForm, Forms.SimulationController)
            End If

            PopulateToolbox()
        End Sub

        Public Overridable Sub CloseProject(ByVal bOpeningProject As Boolean)
            If SaveIfDirty() = DialogResult.Cancel Then
                Return
            End If

            m_bProjectIsOpen = bOpeningProject

            If Not m_dockManager Is Nothing Then
                m_dockManager.ClearContents()
            End If

            For Each mdiChild As Form In Me.MdiChildren
                mdiChild.Close()
            Next

            CreateImageManager()

            ' Create the object that manages the docking state
            m_dockManager = New DockingManager(Me, VisualStyle.IDE)
            AddHandler m_dockManager.ContentClosing, AddressOf Me.OnDockContentClosing
            m_dockManager.OuterControl = m_ctrlStatusBar

            m_menuDefault = CreateMenu()
            m_barDefault = CreateToolbar(m_menuDefault)
            ResetToolbars(m_menuDefault, m_barDefault)

            'Lets remove all of the toolholders and reset the count.
            m_aryToolHolders.Clear()
            m_iNewToolHolderIndex = 0

            'Lets remove all of the stimuli and reset the count.
            m_aryProjectStimuli.Clear()
            m_iNewStimuliIndex = 0

            m_frmWorkspace = Nothing
            m_frmToolbox = Nothing
            m_frmSimulationController = Nothing

            m_strPhysicsAssemblyName = "VortexAnimatTools.dll"
            m_strPhysicsClassName = "VortexAnimatTools.DataObjects.Simulation"
            m_dsSimulation = DirectCast(Util.LoadClass(m_strPhysicsAssemblyName, m_strPhysicsClassName, Me.FormHelper), AnimatTools.DataObjects.Simulation)
            Me.Simulation.SetReleaseLibraries()

            m_ModificationHistory = New AnimatTools.Framework.UndoSystem.ModificationHistory

            Me.Logger.LogMsg(Interfaces.Logger.enumLogLevel.Info, "Closed current project")
        End Sub

        Public Overridable Function SaveStandAlone(ByVal bSaveCharts As Boolean, ByVal bSaveStims As Boolean) As AnimatTools.Interfaces.StdXml

            Try
                Me.Cursor = System.Windows.Forms.Cursors.WaitCursor

                Util.DisableDirtyFlags = True
                Util.ExportForStandAloneSim = True
                Util.ExportChartsInStandAloneSim = bSaveCharts
                Util.ExportStimsInStandAloneSim = bSaveStims

                Dim oXml As New AnimatTools.Interfaces.StdXml

                oXml.AddElement("Simulation")
                Util.Simulation.SaveData(Me, oXml)

                Return oXml
            Catch ex As System.Exception
                Me.Logger.LogMsg(Interfaces.Logger.enumLogLevel.Info, "Unable to save project:")
                Throw ex
            Finally
                Util.DisableDirtyFlags = False
                Util.ExportForStandAloneSim = False
                Util.ExportChartsInStandAloneSim = False
                Util.ExportStimsInStandAloneSim = False
                Me.Cursor = System.Windows.Forms.Cursors.Arrow
            End Try

        End Function

        Public Overridable Sub SaveProject(ByVal strFilename As String)

            Try
                Me.Logger.LogMsg(Interfaces.Logger.enumLogLevel.Info, "Starting Save of project: '" & Util.Application.ProjectPath & "\" & strFilename & "'")

                If Not m_bProjectIsOpen Then
                    Throw New System.Exception("You must have an open project before you can save it.")
                End If

                Me.Cursor = System.Windows.Forms.Cursors.WaitCursor

                Dim oXml As New AnimatTools.Interfaces.StdXml

                Me.ClearIsDirty()

                If Util.IsFullPath(strFilename) Then
                    Util.SplitPathAndFile(strFilename, m_strProjectPath, m_strProjectFile)
                End If

                If m_strProjectName.Trim.Length = 0 Then
                    m_strProjectName = m_strProjectFile.Substring(0, m_strProjectFile.Length - 6)
                End If

                Util.DisableDirtyFlags = True
                SaveData(oXml)
                oXml.Save(Util.GetFilePath(Util.Application.ProjectPath, strFilename))
                Util.DisableDirtyFlags = False

                Me.Logger.LogMsg(Interfaces.Logger.enumLogLevel.Info, "Finished successful save of project: '" & Util.Application.ProjectPath & "\" & strFilename & "'")

            Catch ex As System.Exception
                Me.Logger.LogMsg(Interfaces.Logger.enumLogLevel.Info, "Unable to save project: '" & Util.Application.ProjectPath & "\" & strFilename & "'")
                Throw ex
            Finally
                Util.DisableDirtyFlags = False
                Me.Cursor = System.Windows.Forms.Cursors.Arrow
            End Try
        End Sub

        Public Overridable Sub LoadProject(ByVal strFilename As String)

            Try
                Me.Logger.LogMsg(Interfaces.Logger.enumLogLevel.Info, "Starting load of project: '" & strFilename & "'")

                Me.Cursor = System.Windows.Forms.Cursors.WaitCursor

                Dim oXml As New AnimatTools.Interfaces.StdXml

                Util.SplitPathAndFile(strFilename, m_strProjectPath, m_strProjectFile)

                oXml.Load(strFilename)

                oXml.FindElement("Project")
                oXml.FindChildElement("")

                Util.DisableDirtyFlags = True
                LoadData(oXml)
                VerifyToolbarsAfterLoad()
                Util.DisableDirtyFlags = False

                Me.Simulation.SetReleaseLibraries()

                Me.ClearIsDirty()

                Me.Title = Me.ProjectName & " Project"

                Me.Logger.LogMsg(Interfaces.Logger.enumLogLevel.Info, "Finished successful load of project: '" & strFilename & "'")

            Catch ex As System.Exception
                Me.Logger.LogMsg(Interfaces.Logger.enumLogLevel.Info, "Unable to load project: '" & strFilename & "'")
                Throw ex
            Finally
                Util.DisableDirtyFlags = False
                Me.Cursor = System.Windows.Forms.Cursors.Arrow
            End Try
        End Sub


        Public Overridable Sub SaveRobotDownload(ByVal doSelOrganism As DataObjects.Physical.Organism)
            Dim strProjectPath As String = Util.Application.ProjectPath

            Try
                Me.Logger.LogMsg(Interfaces.Logger.enumLogLevel.Info, "Starting Save of robot config files: " & doSelOrganism.Name)

                If Not m_bProjectIsOpen Then
                    Throw New System.Exception("You must have an open project before you can save it.")
                End If

                If doSelOrganism.Microcontrollers.Count = 0 Then
                    Throw New System.Exception("There are no defined microcontrollers for this organism. You must have inputs and outputs defined in order to download to a robot!")
                End If

                Me.Cursor = System.Windows.Forms.Cursors.WaitCursor

                Me.ClearIsDirty()

                Dim strFilename As String = doSelOrganism.Name & ".asim"

                Util.DisableDirtyFlags = True

                If Not Directory.Exists(Util.Application.ProjectPath & doSelOrganism.Name) Then
                    Directory.CreateDirectory(Util.Application.ProjectPath & doSelOrganism.Name)
                End If

                Util.Application.ProjectPath = Util.Application.ProjectPath & doSelOrganism.Name
                Util.RobotOrganism = doSelOrganism
                Util.ExportChartsInStandAloneSim = True
                Util.ExportStimsInStandAloneSim = True
                Util.Simulation.SaveData(Me, strFilename)

                Util.DisableDirtyFlags = False

                MessageBox.Show("Robot Control Files Created")

                Me.Logger.LogMsg(Interfaces.Logger.enumLogLevel.Info, "Finished successful save of robot config files: " & doSelOrganism.Name)

            Catch ex As System.Exception
                Me.Logger.LogMsg(Interfaces.Logger.enumLogLevel.Info, "Unable to save robot config files: " & doSelOrganism.Name)
                Throw ex
            Finally
                Util.Application.ProjectPath = strProjectPath
                Util.RobotOrganism = Nothing
                Util.ExportChartsInStandAloneSim = False
                Util.ExportStimsInStandAloneSim = False
                Util.DisableDirtyFlags = False
                Me.Cursor = System.Windows.Forms.Cursors.Arrow
            End Try
        End Sub

        Protected Overridable Sub OnExportStandaloneSim(ByVal sender As Object, ByVal e As EventArgs)

            Try
                Me.Logger.LogMsg(Interfaces.Logger.enumLogLevel.Info, "Starting Save of stand alone config file.")

                If Not m_bProjectIsOpen Then
                    Throw New System.Exception("You must have an open project before you can save it.")
                End If

                Me.Cursor = System.Windows.Forms.Cursors.WaitCursor

                Me.ClearIsDirty()

                Dim strFilename As String = Me.ProjectName & "_Standalone.asim"

                Util.DisableDirtyFlags = True
                Util.ExportForStandAloneSim = True
                Util.ExportChartsInStandAloneSim = True
                Util.ExportStimsInStandAloneSim = True

                Util.Simulation.SaveData(Me, strFilename)

                Util.DisableDirtyFlags = False

                MessageBox.Show("Standalone Control Files Created")

                Me.Logger.LogMsg(Interfaces.Logger.enumLogLevel.Info, "Finished successful save of standalone config file")

            Catch ex As System.Exception
                Me.Logger.LogMsg(Interfaces.Logger.enumLogLevel.Info, "Unable to save standalone config file.")
                Throw ex
            Finally
                Util.RobotOrganism = Nothing
                Util.ExportForStandAloneSim = False
                Util.DisableDirtyFlags = False
                Util.ExportChartsInStandAloneSim = False
                Util.ExportStimsInStandAloneSim = False
                Me.Cursor = System.Windows.Forms.Cursors.Arrow
            End Try
        End Sub

#End Region

#Region " Load/Save Methods "

        Public Overloads Overrides Sub LoadData(ByRef oXml As AnimatTools.Interfaces.StdXml)

            CloseProject(True)

            Me.Cursor = System.Windows.Forms.Cursors.WaitCursor

            m_strProjectName = oXml.GetChildString("ProjectName")
            m_strPhysicsAssemblyName = oXml.GetChildString("PhysicsAssemblyName", m_strPhysicsAssemblyName)
            m_strPhysicsClassName = oXml.GetChildString("PhysicsClassName", m_strPhysicsClassName)
            m_strSimulationFile = oXml.GetChildString("SimulationFile", "")
            Dim eLogLevel As AnimatTools.Interfaces.Logger.enumLogLevel = DirectCast([Enum].Parse(GetType(AnimatTools.Interfaces.Logger.enumLogLevel), oXml.GetChildString("LogLevel", "None"), True), AnimatTools.Interfaces.Logger.enumLogLevel)

            If eLogLevel <> Me.Logger.TraceLevel Then
                Me.Logger.TraceLevel = eLogLevel
            End If

            If oXml.FindChildElement("SimWindowLocation", False) Then
                m_ptSimWindowLocation = Util.LoadPoint(oXml, "SimWindowLocation")
            End If
            If oXml.FindChildElement("SimWindowSize", False) Then
                m_szSimWindowSize = Util.LoadSize(oXml, "SimWindowSize")
            End If

            If m_strSimulationFile.Trim.Length > 0 Then
                Try
                    m_dsSimulation = DirectCast(Util.LoadClass(m_strPhysicsAssemblyName, m_strPhysicsClassName, Me.FormHelper), AnimatTools.DataObjects.Simulation)
                    m_dsSimulation.LoadData(Me, m_strSimulationFile)
                Catch ex As System.Exception
                    AnimatTools.Framework.Util.DisplayError(ex)
                End Try
            Else
                m_dsSimulation = DirectCast(Util.LoadClass(Me.ApplicationDirectory & "VortexAnimatTools.dll", "VortexAnimatTools.DataObjects.Simulation", Me.FormHelper), AnimatTools.DataObjects.Simulation)
            End If

            LoadToolHolders(oXml)
            LoadStimuli(oXml)

            oXml.IntoChildElement("DockingForms") 'Into DockingForms Element
            Dim iCount As Integer = oXml.NumberOfChildren() - 1
            For iIndex As Integer = 0 To iCount
                oXml.FindChildByIndex(iIndex)
                LoadDockingForm(m_dockManager, oXml)
            Next
            oXml.OutOfElem()   'Outof DockingForms Element

            LoadDockingConfig(m_dockManager, oXml)

            oXml.IntoChildElement("ChildForms") 'Into ChildForms Element
            iCount = oXml.NumberOfChildren() - 1
            For iIndex As Integer = 0 To iCount
                oXml.FindChildByIndex(iIndex)
                LoadChildForm(oXml)
            Next
            oXml.OutOfElem()   'Outof ChildForms Element

        End Sub

        Public Overridable Sub LoadDockingConfig(ByRef dockManager As DockingManager, _
                                                 ByRef oXml As AnimatTools.Interfaces.StdXml)

            Try
                Me.Cursor = System.Windows.Forms.Cursors.WaitCursor

                If dockManager.Contents.Count > 0 Then
                    If oXml.FindChildElement("DockingConfig", False) Then
                        Dim strDockXml As String = oXml.GetChildDoc()
                        Dim stringReader As System.IO.StringReader = New System.IO.StringReader(strDockXml)
                        Dim xmlReader As System.Xml.XmlTextReader = New System.Xml.XmlTextReader(stringReader)
                        dockManager.LoadConfigFromXml(xmlReader)
                    End If
                End If

                Application.DoEvents()

            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try

        End Sub

        Public Overridable Sub LoadDockingForm(ByRef dockManager As DockingManager, _
                                               ByRef oXml As AnimatTools.Interfaces.StdXml)

            Try
                Me.Cursor = System.Windows.Forms.Cursors.WaitCursor

                oXml.IntoElem()   'Into Form Element

                'Lets get the assembly file and class name
                Dim strFile As String = ToolsDirectory & oXml.GetChildString("AssemblyFile")
                Dim strClass As String = oXml.GetChildString("ClassName")
                Dim strTitle As String = oXml.GetChildString("Title")
                Dim strTabTitle As String = oXml.GetChildString("TabTitle", strTitle)
                Dim bBackgroundForm As Boolean = oXml.GetChildBool("BackgroundForm", False)
                oXml.OutOfElem()   'Outof Form Element

                ''We need to get rid of this eventually. It is here right now to convert any existing projects that still had a seperate
                ''project property window to use the new consolidated one. 
                If strTitle = "Project Properties" Then
                    Return
                End If

                Dim frmDock As AnimatForm = CreateDockingForm(dockManager, strFile, strClass, strTitle, strTabTitle, bBackgroundForm)

                If strClass = "AnimatTools.Forms.ProjectWorkspace" Then
                    m_frmWorkspace = DirectCast(frmDock, Forms.ProjectWorkspace)
                ElseIf strClass = "AnimatTools.Forms.Toolbox" Then
                    m_frmToolbox = DirectCast(frmDock, Forms.Toolbox)
                ElseIf strClass = "AnimatTools.Forms.SimulationController" Then
                    m_frmSimulationController = DirectCast(frmDock, Forms.SimulationController)
                End If

                frmDock.LoadData(oXml)

                Application.DoEvents()

            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try

        End Sub

        Public Overridable Sub LoadChildForm(ByRef oXml As AnimatTools.Interfaces.StdXml)
            Dim frmMdi As AnimatTools.Forms.MdiChild

            Try
                Me.Cursor = System.Windows.Forms.Cursors.WaitCursor

                'Lets get the assembly file and class name
                oXml.IntoElem() 'Into Child Element
                Dim strMdiFile As String = ToolsDirectory & oXml.GetChildString("AssemblyFile", "AnimatTools.dll")
                Dim strMdiClass As String = oXml.GetChildString("ClassName", "AnimatTools.Forms.MdiChild")

                oXml.IntoChildElement("Form")  'Into the Form element
                Dim strFile As String = ToolsDirectory & oXml.GetChildString("AssemblyFile")
                Dim strClass As String = oXml.GetChildString("ClassName")
                Dim strTitle As String = oXml.GetChildString("Title")
                oXml.OutOfElem()  'OutOf the Form element
                oXml.OutOfElem()  'OutOf the Child element

                frmMdi = DirectCast(Util.LoadClass(strMdiFile, strMdiClass), AnimatTools.Forms.MdiChild)
                Dim frmBase As AnimatForm = CreateForm(strFile, strClass, strTitle, frmMdi, False)
                frmMdi.Initialize(Me, frmBase)
                frmMdi.LoadData(oXml)

                If strTitle.Trim.Length > 0 Then
                    frmMdi.Title = strTitle
                End If

                frmMdi.Show()

                frmMdi.Location = frmMdi.InitialLocation

                Application.DoEvents()

            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)

                Try
                    If Not frmMdi Is Nothing Then
                        frmMdi.Close()
                    End If
                Catch ex2 As System.Exception
                End Try

            End Try

        End Sub

        Protected Overridable Sub LoadStimuli(ByRef oXml As AnimatTools.Interfaces.StdXml)

            Try

                Dim strAssemblyFile As String
                Dim strClassname As String
                Dim doStim As DataObjects.ExternalStimuli.Stimulus
                If oXml.FindChildElement("Stimuli", False) Then
                    oXml.IntoChildElement("Stimuli")
                    Dim iCount As Integer = oXml.NumberOfChildren() - 1
                    For iIndex As Integer = 0 To iCount
                        oXml.FindChildByIndex(iIndex)

                        oXml.IntoElem()
                        strAssemblyFile = oXml.GetChildString("AssemblyFile")
                        strClassname = oXml.GetChildString("ClassName")
                        oXml.OutOfElem()

                        doStim = DirectCast(Util.LoadClass(strAssemblyFile, strClassname, m_doFormHelper), DataObjects.ExternalStimuli.Stimulus)
                        doStim.LoadData(oXml)

                        If Not doStim.StimulusNoLongerValid Then
                            m_aryProjectStimuli.Add(doStim.ID, doStim)
                        End If
                    Next

                    oXml.OutOfElem()
                End If

            Catch ex As System.Exception
                'If we hit an erorr then it most likely happened while adding or loading the stimulus.
                'If that is the case then we need to step out of the two elements we stepped into.
                oXml.OutOfElem()
                oXml.OutOfElem()
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try

        End Sub

        Protected Overridable Sub LoadToolHolders(ByRef oXml As AnimatTools.Interfaces.StdXml)

            Try

                Dim iCount As Integer
                Dim doTool As DataObjects.ToolHolder
                If oXml.FindChildElement("ToolViewers", False) Then
                    oXml.IntoChildElement("ToolViewers")

                    iCount = oXml.NumberOfChildren() - 1
                    For iIndex As Integer = 0 To iCount
                        oXml.FindChildByIndex(iIndex)
                        doTool = New DataObjects.ToolHolder(m_doFormHelper)
                        doTool.LoadData(oXml)
                        m_aryToolHolders.Add(doTool.ID, doTool)
                    Next

                    oXml.OutOfElem()
                End If

            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try

        End Sub

        Public Overloads Overrides Sub SaveData(ByRef oXml As AnimatTools.Interfaces.StdXml)

            oXml.AddElement("Project")

            If Me.SimulationFile.Trim.Length = 0 Then
                Me.SimulationFile = m_strProjectName & ".asim"
            End If

            oXml.AddChildElement("ProjectName", m_strProjectName)
            oXml.AddChildElement("PhysicsAssemblyName", m_strPhysicsAssemblyName)
            oXml.AddChildElement("PhysicsClassName", m_strPhysicsClassName)
            oXml.AddChildElement("SimulationFile", Me.SimulationFile)
            oXml.AddChildElement("LogLevel", Me.Logger.TraceLevel.ToString)

            Util.SavePoint(oXml, "SimWindowLocation", m_ptSimWindowLocation)
            Util.SaveSize(oXml, "SimWindowSize", m_szSimWindowSize)

            m_dsSimulation.SaveData(Me, Me.SimulationFile)

            oXml.AddChildElement("DockingForms")
            oXml.IntoElem()   'Into DockingForms Element

            'First lets save all Docking Forms associated with this application.
            Dim frmAnimat As AnimatForm
            For Each conWindow As Content In m_dockManager.Contents
                frmAnimat = DirectCast(conWindow.Control, AnimatForm)
                frmAnimat.SaveData(oXml)
            Next
            oXml.OutOfElem()   'Outof DockingForms Element

            oXml.AddChildElement("ChildForms")
            oXml.IntoElem()   'Into ChildForms Element

            For Each mdiWindow As MdiChild In Me.MdiChildren
                mdiWindow.SaveData(oXml)
            Next
            oXml.OutOfElem()   'Outof ChildForms Element

            oXml.AddChildElement("ToolViewers")
            oXml.IntoElem()

            Dim doTool As DataObjects.ToolHolder
            For Each deEntry As DictionaryEntry In m_aryToolHolders
                doTool = DirectCast(deEntry.Value, DataObjects.ToolHolder)
                doTool.SaveData(oXml)
            Next

            oXml.OutOfElem()   'Outof ToolViewers element

            oXml.AddChildElement("Stimuli")
            oXml.IntoElem()

            Dim aryStimsToDelete As New Collection
            Dim doStim As DataObjects.ExternalStimuli.Stimulus
            For Each deEntry As DictionaryEntry In m_aryProjectStimuli
                doStim = DirectCast(deEntry.Value, DataObjects.ExternalStimuli.Stimulus)

                If doStim.StimulusNoLongerValid Then
                    aryStimsToDelete.Add(doStim)
                Else
                    doStim.SaveData(oXml)
                End If
            Next

            oXml.OutOfElem()   'Outof Stimuli element

            oXml.OutOfElem()   'Outof Project Element

            SaveDockingConfig(m_dockManager, oXml)

            'Now lets delete any stims that are no longer valid
            For Each doStim In aryStimsToDelete
                m_aryProjectStimuli.Remove(doStim.ID)

                If Not doStim.WorkspaceTreeNode Is Nothing Then
                    doStim.WorkspaceTreeNode.Remove()
                End If
            Next

        End Sub

        Public Overridable Sub SaveDockingConfig(ByRef dockManager As DockingManager, _
                                                 ByRef oXml As AnimatTools.Interfaces.StdXml)
            'Save the docking manager configuration
            Dim sb As System.Text.StringBuilder = New System.Text.StringBuilder
            Dim stringWriter As System.IO.StringWriter = New System.IO.StringWriter(sb)
            Dim xmlWriter As System.Xml.XmlTextWriter = New System.Xml.XmlTextWriter(stringWriter)
            dockManager.SaveConfigToXml(xmlWriter)
            Dim strDockXml As String = sb.ToString & vbCrLf
            oXml.AddChildDoc(strDockXml)
        End Sub

        Protected Overridable Function SaveIfDirty(Optional ByVal e As System.ComponentModel.CancelEventArgs = Nothing) As DialogResult
            Dim eResult As System.Windows.Forms.DialogResult = DialogResult.OK

            'First check to see if the application is dirty. If it is then ask to save the project
            If Me.IsDirty Then
                eResult = MessageBox.Show("There are unsaved changes in the project. " & _
                                                                                    "Do you want to save them before you exit?", _
                                                                                    "Save Changes", MessageBoxButtons.YesNoCancel)
                If eResult = DialogResult.Cancel Then
                    If Not e Is Nothing Then e.Cancel = True
                    Return eResult
                ElseIf eResult = DialogResult.Yes Then
                    Me.SaveProject(Me.ProjectFile)
                Else
                    Me.ClearIsDirty()
                End If
            End If

            Return eResult
        End Function

#End Region

#Region " Docking Form Management "

        Public Overridable Function CreateDockingForm(ByRef dockManager As DockingManager, _
                                                      ByVal strAssemblyFile As String, _
                                                      ByVal strClassname As String, _
                                                      ByVal strPageTitle As String, _
                                                      Optional ByVal strTabTitle As String = "", _
                                                      Optional ByVal bBackgroundForm As Boolean = False, _
                                                      Optional ByVal eState As Crownwood.Magic.Docking.State = State.DockLeft, _
                                                      Optional ByRef wcCreatedWindow As Crownwood.Magic.Docking.WindowContent = Nothing, _
                                                      Optional ByRef wcAddToWindow As Crownwood.Magic.Docking.WindowContent = Nothing, _
                                                      Optional ByRef wzAddToZone As Crownwood.Magic.Docking.Zone = Nothing, _
                                                      Optional ByVal iZoneIndex As Integer = 0, _
                                                      Optional ByVal iDisplaySizeX As Integer = 150, _
                                                      Optional ByVal iDisplaySizeY As Integer = 150) As AnimatForm

            'First lets make sure that there is not already a docking window with this title.
            'Title is a unique id used by the dockmanager.
            Dim ctFound As Content = m_dockManager.Contents.Item(strPageTitle)

            If ctFound Is Nothing Then
                Dim frmDock As AnimatForm = CreateForm(strAssemblyFile, strClassname, strPageTitle)

                Dim iImageIndex As Integer = m_mgrSmallImages.FindImageName(frmDock.IconName)
                If iImageIndex > -1 Then
                    frmDock.Content = dockManager.Contents.Add(frmDock, frmDock.Title, m_mgrSmallImages.ImageList, iImageIndex)
                Else
                    frmDock.Content = dockManager.Contents.Add(frmDock, frmDock.Title)
                End If
                frmDock.Content.BackgroundForm = bBackgroundForm
                frmDock.Content.UserData = frmDock

                frmDock.Content.DisplaySize = New System.Drawing.Size(iDisplaySizeX, iDisplaySizeY)
                frmDock.Content.AutoHideSize = New System.Drawing.Size(iDisplaySizeX, iDisplaySizeY)

                If strTabTitle.Trim.Length <> 0 Then frmDock.Content.TabTitle = strTabTitle

                If Not wcAddToWindow Is Nothing Then
                    wcCreatedWindow = dockManager.AddContentToWindowContent(frmDock.Content, wcAddToWindow)
                ElseIf Not wzAddToZone Is Nothing Then
                    dockManager.AddContentToZone(frmDock.Content, wzAddToZone, iZoneIndex)
                Else
                    wcCreatedWindow = dockManager.AddContentWithState(frmDock.Content, eState)
                End If

                If bBackgroundForm Then
                    AddItemToDefaultMenu("View", "", frmDock.Text, frmDock.Text, _
                                         frmDock.IconName, New EventHandler(AddressOf Me.OnViewDockingForm))
                End If

                Return frmDock
            Else
                Throw New System.Exception("You can not insert docking forms with duplicate titles. " & _
                                           "A docking form with the title of '" & strPageTitle & "'.")
            End If

        End Function

        Public Overridable Function AddDockingForm(ByRef dockManager As DockingManager, _
                                                      ByVal frmDock As AnimatForm, _
                                                      ByVal frmMdiParent As MdiChild, _
                                                      ByVal strPageTitle As String, _
                                                      Optional ByVal strTabTitle As String = "", _
                                                      Optional ByVal bBackgroundForm As Boolean = False, _
                                                      Optional ByVal eState As Crownwood.Magic.Docking.State = State.DockLeft, _
                                                      Optional ByRef wcCreatedWindow As Crownwood.Magic.Docking.WindowContent = Nothing, _
                                                      Optional ByRef wcAddToWindow As Crownwood.Magic.Docking.WindowContent = Nothing, _
                                                      Optional ByRef wzAddToZone As Crownwood.Magic.Docking.Zone = Nothing, _
                                                      Optional ByVal iZoneIndex As Integer = 0, _
                                                      Optional ByVal iDisplaySizeX As Integer = 150, _
                                                      Optional ByVal iDisplaySizeY As Integer = 150) As AnimatForm

            'First lets make sure that there is not already a docking window with this title.
            'Title is a unique id used by the dockmanager.
            Dim ctFound As Content = m_dockManager.Contents.Item(strPageTitle)

            If ctFound Is Nothing Then
                frmDock.Initialize(Me, frmMdiParent)
                frmDock.Title = strPageTitle

                frmDock.OnBeforeFormAdded()

                Dim iImageIndex As Integer = m_mgrSmallImages.FindImageName(frmDock.IconName)
                If iImageIndex > -1 Then
                    frmDock.Content = dockManager.Contents.Add(frmDock, frmDock.Title, m_mgrSmallImages.ImageList, iImageIndex)
                Else
                    frmDock.Content = dockManager.Contents.Add(frmDock, frmDock.Title)
                End If
                frmDock.Content.BackgroundForm = bBackgroundForm
                frmDock.Content.UserData = frmDock

                frmDock.Content.DisplaySize = New System.Drawing.Size(iDisplaySizeX, iDisplaySizeY)

                If strTabTitle.Trim.Length <> 0 Then frmDock.Content.TabTitle = strTabTitle

                If Not wcAddToWindow Is Nothing Then
                    wcCreatedWindow = dockManager.AddContentToWindowContent(frmDock.Content, wcAddToWindow)
                ElseIf Not wzAddToZone Is Nothing Then
                    dockManager.AddContentToZone(frmDock.Content, wzAddToZone, iZoneIndex)
                Else
                    wcCreatedWindow = dockManager.AddContentWithState(frmDock.Content, eState)
                End If

                If bBackgroundForm Then
                    AddItemToDefaultMenu("View", "", frmDock.Text, frmDock.Text, _
                                         frmDock.IconName, New EventHandler(AddressOf Me.OnViewDockingForm))
                End If

                frmDock.OnAfterFormAdded()

                Return frmDock
            Else
                Throw New System.Exception("You can not insert docking forms with duplicate titles. " & _
                                           "A docking form with the title of '" & strPageTitle & "'.")
            End If

        End Function

        Public Overridable Function CreateForm(ByVal strAssemblyFile As String, _
                                               ByVal strClassname As String, _
                                               ByVal strTitle As String, _
                                               Optional ByVal frmMdiParent As AnimatTools.Forms.MdiChild = Nothing, _
                                               Optional ByVal bInitialize As Boolean = True) As AnimatForm

            Dim oAssembly As System.Reflection.Assembly = System.Reflection.Assembly.LoadFrom(Util.GetFilePath(Me.ApplicationDirectory, strAssemblyFile))
            Dim frmAnimat As AnimatForm = DirectCast(oAssembly.CreateInstance(strClassname), AnimatForm)

            If frmAnimat Is Nothing Then
                Throw New System.Exception("The system was unable to create the form '" & strClassname & _
                                           "' from the assembly '" & strAssemblyFile & "'.")
            End If

            If bInitialize Then
                If frmMdiParent Is Nothing Then
                    frmAnimat.Initialize(Me, Nothing, Util.Application)
                Else
                    frmAnimat.Initialize(Me, frmMdiParent)
                End If
            End If
            frmAnimat.Title = strTitle

            Dim iImageIndex As Integer = -1
            If frmAnimat.IconName.Trim.Length > 0 Then
                If m_mgrSmallImages.AddImage(oAssembly, frmAnimat.IconName) Then
                    iImageIndex = m_mgrSmallImages.GetImageIndex(frmAnimat.IconName)
                End If
            End If

            Return frmAnimat
        End Function

        Public Overridable Sub RemoveDockingForm(ByRef dockManager As DockingManager, _
                                                 ByVal strTitle As String, _
                                                 Optional ByVal bThrowError As Boolean = True)
            Dim ctContent As Content = dockManager.Contents(strTitle)
            If Not ctContent Is Nothing Then
                dockManager.Contents.Remove(ctContent)
            ElseIf bThrowError Then
                Throw New System.Exception("No docking form with the title '" & strTitle & "' was found to remove.")
            End If
        End Sub

        Public Overridable Sub RemoveDockingForm(ByRef dockManager As DockingManager, _
                                                 ByRef ctContent As Content, _
                                                 Optional ByVal bThrowError As Boolean = True)
            If Not ctContent Is Nothing Then
                dockManager.Contents.Remove(ctContent)
            ElseIf bThrowError Then
                Throw New System.Exception("The docking form to remove was not defined.")
            End If

        End Sub

#End Region

#Region " Splash Screen Methods "

        Protected Sub ShowSplashScreen()
            Try

                '#If Not Debug Then
                '                Dim imgSplash As Image = ImageManager.LoadImage("AnimatTools", "AnimatTools.Splash.jpg")

                '                If Not imgSplash Is Nothing AndAlso TypeOf imgSplash Is Bitmap Then
                '                    Dim bmpSplash As Bitmap = DirectCast(imgSplash, Bitmap)
                '                    Crownwood.Magic.Forms.SplashForm.StartSplash(bmpSplash, Color.FromArgb(64, 0, 63))
                '                End If
                '#End If

            Catch ex As System.Exception

            End Try
        End Sub

        Protected Sub CloseSplashScreen()
            Try

#If Not Debug Then
                m_SplashTimer.Interval = 2000
                m_SplashTimer.Enabled = True
#End If
            Catch ex As System.Exception

            End Try
        End Sub

#End Region

#Region " Windows Management "

        Public Overridable Function OpenSimulationWindow(ByVal bPaused As Boolean) As Boolean

            Try
                If m_frmSimulationWnd Is Nothing Then
                    Util.Logger.LogMsg(Interfaces.Logger.enumLogLevel.Info, "Opening a simulation window.")

                    'We must first save changes so that the simulator can load in the latest configuration data.
                    'If Me.IsDirty Then'    'Only resave the project if we are dirty.
                    'DWC Temporary fix. For some reason the diry flag sporadically stops working. So your changes
                    'do not get saved. For the meantime I a switching back to just always saving when you run the sim.
                    'Me.SaveProject(Me.ProjectFile)
                    'End If

                    m_frmSimulationWnd = New AnimatTools.Forms.SimulationMdi
                    Dim frmBase As AnimatTools.Forms.SimulatorView = New AnimatTools.Forms.SimulatorView

                    Util.Logger.LogMsg(Interfaces.Logger.enumLogLevel.Debug, "About to initialize the window.")

                    m_frmSimulationWnd.Initialize(Me, DirectCast(frmBase, AnimatForm))
                    m_frmSimulationWnd.Title = "Simulation "

                    AddHandler frmBase.SimulationStarting, AddressOf Me.OnSimViewSimulationStarting
                    AddHandler frmBase.SimulationResuming, AddressOf Me.OnSimViewSimulationResuming
                    AddHandler frmBase.SimulationStarted, AddressOf Me.OnSimViewSimulationStarted
                    AddHandler frmBase.SimulationPaused, AddressOf Me.OnSimViewSimulationPaused
                    AddHandler frmBase.SimulationStopped, AddressOf Me.OnSimViewSimulationStopped

                    Util.Logger.LogMsg(Interfaces.Logger.enumLogLevel.Debug, "About to show the window.")

                    m_frmSimulationWnd.Show()

                    If m_szSimWindowSize.Width > 100 AndAlso m_szSimWindowSize.Height > 100 Then
                        m_frmSimulationWnd.Location = m_ptSimWindowLocation
                        m_frmSimulationWnd.Size = m_szSimWindowSize
                    Else
                        m_frmSimulationWnd.Width = 640
                        m_frmSimulationWnd.Height = 480
                    End If

                    'If we are recording a video then use the sizes preset by the user.
                    If Util.Environment.Camera.RecordVideo Then
                        m_frmSimulationWnd.Width = Util.Environment.Camera.VideoWidth + 8
                        m_frmSimulationWnd.Height = Util.Environment.Camera.VideoHeight + 34
                    End If

                    Util.Logger.LogMsg(Interfaces.Logger.enumLogLevel.Debug, "About to call simulate.")

                    If frmBase.Simulate(bPaused) Then
                        Return True
                    Else
                        Util.Logger.LogMsg(Interfaces.Logger.enumLogLevel.Info, "Simulate returned false. Attempting to close the simulation window.")

                        If Not m_frmSimulationWnd Is Nothing Then
                            m_frmSimulationWnd.MdiParent = Nothing
                            m_frmSimulationWnd.Close()
                            m_frmSimulationWnd = Nothing
                        End If

                        Return False
                    End If

                End If

                Return False
            Catch ex As System.Exception
                Util.Logger.LogMsg(Interfaces.Logger.enumLogLevel.Info, "An Error occurred. Attempting to close the simulation window." & vbCrLf & ex.Message)

                'Lets go through and see if we actually created a simulation window.
                'If we did then we need to get rid of it so that the next time we
                'try and open the window we can.
                If Not m_frmSimulationWnd Is Nothing Then
                    m_frmSimulationWnd.MdiParent = Nothing
                    m_frmSimulationWnd.Close()
                    m_frmSimulationWnd = Nothing
                End If

                Throw ex
            End Try

        End Function

        Public Overridable Function EditBehavioralSystem(ByVal doOrganism As DataObjects.Physical.Organism) As AnimatTools.Forms.MdiChild

            Try
                'First lets verify that there is not already an open window for this organism.
                Dim frmEditor As Forms.Behavior.Editor
                For Each oChild As Form In Util.Application.MdiChildren
                    If TypeOf oChild Is Forms.Behavior.Editor Then
                        frmEditor = DirectCast(oChild, Forms.Behavior.Editor)
                        If frmEditor.Organism Is doOrganism Then
                            frmEditor.MakeVisible()
                            Return frmEditor
                        End If
                    End If
                Next

                Dim frmMdi As New AnimatTools.Forms.Behavior.Editor
                Dim frmBase As AnimatForm

                Me.Cursor = System.Windows.Forms.Cursors.WaitCursor

                frmMdi.Organism = doOrganism
                frmMdi.Initialize(Me, frmBase)
                frmMdi.Title = "Edit " & doOrganism.Name

                If System.IO.File.Exists(Util.GetFilePath(Util.Application.ProjectPath, doOrganism.BehavioralEditorFile)) Then
                    frmMdi.LoadEditorFile(doOrganism.BehavioralEditorFile)
                End If

                frmMdi.Show()

                doOrganism.BehaviorEditor = frmMdi

                Me.Cursor = System.Windows.Forms.Cursors.Arrow

                Return frmMdi
            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            Finally
                Me.Cursor = System.Windows.Forms.Cursors.Arrow
            End Try

        End Function

        Public Overridable Function EditBodyPlan(ByVal doStructure As DataObjects.Physical.PhysicalStructure) As AnimatTools.Forms.MdiChild

            Try
                'First lets verify that there is not already an open window for this organism.
                Dim frmEditor As Forms.BodyPlan.Editor
                For Each oChild As Form In Util.Application.MdiChildren
                    If TypeOf oChild Is Forms.BodyPlan.Editor Then
                        frmEditor = DirectCast(oChild, Forms.BodyPlan.Editor)
                        If frmEditor.PhysicalStructure Is doStructure Then
                            frmEditor.MakeVisible()
                            Return frmEditor
                        End If
                    End If
                Next

                Dim frmMdi As New AnimatTools.Forms.BodyPlan.Editor
                Dim frmBase As AnimatForm

                Me.Cursor = System.Windows.Forms.Cursors.WaitCursor

                frmMdi.PhysicalStructure = doStructure
                frmMdi.Initialize(Me, frmBase)
                frmMdi.Title = "Edit " & doStructure.Name

                If System.IO.File.Exists(Util.GetFilePath(Util.Application.ProjectPath, doStructure.BodyPlanEditorFile)) Then
                    frmMdi.LoadEditorFile(doStructure.BodyPlanEditorFile)
                End If

                frmMdi.Show()

                doStructure.BodyEditor = frmMdi

                Me.Cursor = System.Windows.Forms.Cursors.Arrow

                Return frmMdi
            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            Finally
                Me.Cursor = System.Windows.Forms.Cursors.Arrow
            End Try

        End Function


        Public Overridable Function DisplayToolViewer(ByVal doTool As DataObjects.ToolHolder) As AnimatTools.Forms.MdiChild

            Try
                'First lets verify that there is not already an open window for this organism.
                Dim frmViewer As Forms.Tools.Viewer
                For Each oChild As Form In Util.Application.MdiChildren
                    If TypeOf oChild Is Forms.Tools.Viewer Then
                        frmViewer = DirectCast(oChild, Forms.Tools.Viewer)
                        If frmViewer.ToolHolder Is doTool Then
                            frmViewer.MakeVisible()
                            Return frmViewer
                        End If
                    End If
                Next

                'There should be an existing file or something is wrong.
                Dim frmMdi As New AnimatTools.Forms.Tools.Viewer
                If System.IO.File.Exists(Util.GetFilePath(Util.Application.ProjectPath, doTool.ToolViewerFilename)) Then

                    Me.Cursor = System.Windows.Forms.Cursors.WaitCursor

                    Dim frmTool As Forms.Tools.ToolForm = DirectCast(Me.CreateForm(doTool.BaseAssemblyFile, doTool.BaseClassName, doTool.Name, Nothing, False), Forms.Tools.ToolForm)
                    frmMdi.Initialize(Me, frmTool)
                    frmMdi.LoadEditorFile(doTool.ToolViewerFilename)
                    frmMdi.Title = doTool.Name
                    frmMdi.Show()

                    Me.Cursor = System.Windows.Forms.Cursors.Arrow

                Else
                    Throw New System.Exception("No file was found for the tool viewer '" & doTool.Name & "'.")
                End If

                Return frmMdi
            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            Finally
                Me.Cursor = System.Windows.Forms.Cursors.Arrow
            End Try

        End Function


        Public Overrides Sub UndoRedoRefresh(ByVal doRefresh As AnimatTools.Framework.DataObject)

            If Not doRefresh Is Nothing Then
                Me.ProjectWorkspace.SelectDataObject(doRefresh, False)
                Me.ProjectWorkspace.ctrlProperties.Refresh()
            End If

        End Sub

#End Region

#End Region

#Region " Application Events "

        Public Event ProjectLoaded()
        Public Event ProjectSaved()
        Public Event ProjectClosed()
        Public Event ProjectCreated()
        Public Event ApplicationExiting()
        Public Event SimulationStarting()
        Public Event SimulationResuming()
        Public Event SimulationStarted()
        Public Event SimulationPaused()
        Public Event SimulationStopped()
        Public Event UnitsChanged(ByVal ePrevMass As AnimatTools.DataObjects.Physical.Environment.enumMassUnits, _
                                  ByVal eNewMass As AnimatTools.DataObjects.Physical.Environment.enumMassUnits, _
                                  ByVal fltMassChange As Single, _
                                  ByVal ePrevDistance As AnimatTools.DataObjects.Physical.Environment.enumDistanceUnits, _
                                  ByVal eNewDistance As AnimatTools.DataObjects.Physical.Environment.enumDistanceUnits, _
                                  ByVal fltDistanceChange As Single)

#End Region

#Region " Event Handlers "

        Protected Overridable Sub OnNewProject(ByVal sender As Object, ByVal e As EventArgs)

            Try
                If SaveIfDirty() = DialogResult.Cancel Then
                    Return
                End If

                Dim frmNewProject As New Forms.NewProject

                frmNewProject.txtProjectName.Text = "NewProject"
                If frmNewProject.ShowDialog = DialogResult.OK Then
                    m_dsSimulation = DirectCast(Util.LoadClass(m_strPhysicsAssemblyName, m_strPhysicsClassName, Me.FormHelper), AnimatTools.DataObjects.Simulation)
                    Me.Simulation.SetReleaseLibraries()
                    Util.Application.ProjectPath = frmNewProject.txtLocation.Text & "\" & frmNewProject.txtProjectName.Text
                    Util.Application.ProjectName = frmNewProject.txtProjectName.Text
                    Util.Application.ProjectFile = Util.Application.ProjectName & ".aproj"
                    Util.Application.SimulationFile = Util.Application.ProjectName & ".asim"
                    Me.Title = Me.ProjectName & " Project"

                    Me.Logger.LogMsg(Interfaces.Logger.enumLogLevel.Info, "Creating a new Project: '" & Util.Application.ProjectPath & "\" & Util.Application.ProjectFile)

                    Me.Cursor = System.Windows.Forms.Cursors.WaitCursor

                    'Create the project directory
                    System.IO.Directory.CreateDirectory(Util.Application.ProjectPath)

                    Util.DisableDirtyFlags = True
                    ResetProject(True)
                    Util.DisableDirtyFlags = False

                    SaveProject(Util.Application.ProjectFile)

                    RaiseEvent ProjectCreated()
                End If

            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            Finally
                Me.Cursor = System.Windows.Forms.Cursors.Arrow
                Util.DisableDirtyFlags = False
            End Try
        End Sub

        Protected Overridable Sub OnOpenProject(ByVal sender As Object, ByVal e As EventArgs)

            Try
                If SaveIfDirty() = DialogResult.Cancel Then
                    Return
                End If

                Me.Cursor = System.Windows.Forms.Cursors.WaitCursor

                Dim openFileDialog As New OpenFileDialog
                openFileDialog.Filter = "AnimatLab Project|*.aproj"
                openFileDialog.Title = "Open an AnimatLab Project"

                If openFileDialog.ShowDialog() = DialogResult.OK Then
                    LoadProject(openFileDialog.FileName)
                    RaiseEvent ProjectLoaded()
                End If

            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            Finally
                Me.Cursor = System.Windows.Forms.Cursors.Arrow
            End Try
        End Sub

        Protected Overridable Sub OnSaveProject(ByVal sender As Object, ByVal e As EventArgs)

            Try
                SaveProject(Me.ProjectFile)
                RaiseEvent ProjectSaved()

            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try
        End Sub

        Protected Overridable Sub OnSaveProjectAs(ByVal sender As Object, ByVal e As EventArgs)

            Try
                If Not m_bProjectIsOpen Then
                    Throw New System.Exception("You must have an open project before you can save it.")
                End If

                Dim frmSave As New Forms.NewProject
                frmSave.txtProjectName.Text = Me.ProjectName
                frmSave.txtLocation.Text = Util.RemoveStringSections(Me.ProjectPath, "\", 1)
                frmSave.lblPhysicsModule.Visible = False
                frmSave.ctrlPhysicsEngines.Visible = False
                frmSave.Text = "Save Project As .."
                frmSave.Size = New Size(318, 168)

                If frmSave.ShowDialog = DialogResult.OK Then
                    'Copy the current project directory
                    Dim strNewPath As String = frmSave.txtLocation.Text & "\" & frmSave.txtProjectName.Text
                    Dim strNewProjFile As String = frmSave.txtProjectName.Text & ".aproj"
                    Dim strNewSimFile As String = frmSave.txtProjectName.Text & ".asim"

                    strNewPath = strNewPath.Replace("\\", "\")

                    Me.Logger.LogMsg(Interfaces.Logger.enumLogLevel.Info, "Saving project: '" & Util.Application.ProjectPath & "\" & Util.Application.ProjectFile & "' as '" & strNewPath)

                    Util.CopyDirectory(Util.Application.ProjectPath, strNewPath)

                    'Rename the project files in the new folder.
                    If File.Exists(strNewPath & "\" & Util.Application.ProjectFile) Then File.Delete(strNewPath & "\" & Util.Application.ProjectFile)
                    If File.Exists(strNewPath & "\" & Util.Application.SimulationFile) Then File.Delete(strNewPath & "\" & Util.Application.SimulationFile)

                    Util.Application.ProjectPath = frmSave.txtLocation.Text & "\" & frmSave.txtProjectName.Text & "\"
                    Util.Application.ProjectName = frmSave.txtProjectName.Text
                    Util.Application.ProjectFile = Util.Application.ProjectName & ".aproj"
                    Util.Application.SimulationFile = Util.Application.ProjectName & ".asim"
                    Me.Title = Me.ProjectName & " Project"

                    Me.Cursor = System.Windows.Forms.Cursors.WaitCursor

                    SaveProject(Util.Application.ProjectFile)

                    RaiseEvent ProjectSaved()
                End If

            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            Finally
                Me.Cursor = System.Windows.Forms.Cursors.Default
            End Try
        End Sub

        Protected Overridable Sub OnCloseProject(ByVal sender As Object, ByVal e As EventArgs)

            Try
                ResetProject(False)
                RaiseEvent ProjectClosed()

            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try
        End Sub

        Protected Overridable Sub OnExit(ByVal sender As Object, ByVal e As EventArgs)

            Try
                Me.Close()

            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try
        End Sub

        Protected Overridable Sub OnOpenSimulationWindow(ByVal sender As Object, ByVal e As EventArgs)

            Try
                If Not m_frmSimulationController Is Nothing Then
                    m_frmSimulationController.StartSimulation(True)
                End If

            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try
        End Sub

        Protected Overridable Sub OnRunSimulation(ByVal sender As Object, ByVal e As EventArgs)

            Try
                If Not m_frmSimulationController Is Nothing Then
                    m_frmSimulationController.StartSimulation(False)
                End If
            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try
        End Sub
        Protected Overridable Sub Onpreference(ByVal sender As Object, ByVal e As EventArgs)

            Try
                Dim frmPreference As New AnimatTools.Forms.Tools.Preference
                'frmCompare.PhysicalStructure = Me.PhysicalStructure

                ''frmPreference.SelectedItems().Clear()
                'For Each doItem As AnimatTools.Framework.DataObject In m_eAutoUpdateInterval.'m_PropertiesBar.SelectedParts
                '    frmPreference.SelectedItems.Add(doItem)
                'Next

                frmPreference.ShowDialog()

            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try
        End Sub

        Protected Overridable Sub OnPauseSimulation(ByVal sender As Object, ByVal e As EventArgs)

            Try
                If Not Me.SimulationView Is Nothing Then
                    Me.SimulationView.ToggleSimulation()
                End If
            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try
        End Sub

        Protected Sub OnUndo(ByVal sender As Object, ByVal e As System.EventArgs)

            Try
                Me.ModificationHistory.Undo()
            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try

        End Sub

        Protected Sub OnRedo(ByVal sender As Object, ByVal e As System.EventArgs)

            Try
                Me.ModificationHistory.Redo()
            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try

        End Sub

        Protected Sub OnRunProgramModule(ByVal sender As Object, ByVal e As System.EventArgs)

            Try
                Dim frmProgramModules As New Forms.SelectProgramModule

                If frmProgramModules.ShowDialog() = DialogResult.OK AndAlso Not frmProgramModules.SelectedModule Is Nothing Then
                    frmProgramModules.SelectedModule.ShowDialog()
                End If

            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try
        End Sub

        Protected Sub OnCutWorkspace(ByVal sender As Object, ByVal e As System.EventArgs)

            Try
                Dim tnNode As TreeNode = m_frmWorkspace.TreeView.SelectedNode
                If Not tnNode Is Nothing AndAlso Not tnNode.Tag Is Nothing Then
                    If TypeOf tnNode.Tag Is AnimatTools.DataObjects.Physical.PhysicalStructure Then
                        Dim doStruct As DataObjects.Physical.PhysicalStructure = DirectCast(tnNode.Tag, AnimatTools.DataObjects.Physical.PhysicalStructure)
                        doStruct.Delete()
                    ElseIf TypeOf tnNode.Tag Is AnimatTools.DataObjects.ToolHolder Then
                        Dim doTool As AnimatTools.DataObjects.ToolHolder = DirectCast(tnNode.Tag, AnimatTools.DataObjects.ToolHolder)
                        doTool.Delete()
                    ElseIf TypeOf tnNode.Tag Is AnimatTools.DataObjects.ExternalStimuli.Stimulus Then
                        Dim doStim As AnimatTools.DataObjects.ExternalStimuli.Stimulus = DirectCast(tnNode.Tag, AnimatTools.DataObjects.ExternalStimuli.Stimulus)
                        doStim.Delete()
                    End If

                End If

            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try

        End Sub

        Protected Sub OnAddOrganism(ByVal sender As Object, ByVal e As System.EventArgs)

            Try
                Util.Environment.AddOrganism()
            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try

        End Sub

        Protected Sub OnAddStructure(ByVal sender As Object, ByVal e As System.EventArgs)

            Try
                Util.Environment.AddStructure()
            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try

        End Sub

        Protected Sub OnAddGround(ByVal sender As Object, ByVal e As System.EventArgs)

            Try
                Util.Environment.AddGround()
            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try

        End Sub

        Protected Sub OnAddWater(ByVal sender As Object, ByVal e As System.EventArgs)

            Try
                Util.Environment.AddWater()
            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try

        End Sub

        Protected Sub OnAddDataTool(ByVal sender As Object, ByVal e As System.EventArgs)

            Try
                Dim frmSelectTool As New Forms.Tools.SelectToolType

                If frmSelectTool.ShowDialog() = DialogResult.OK Then
                    Me.AddNewTool(frmSelectTool.SelectedToolType)
                End If

            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try

        End Sub

        Protected Sub OnHelpContents(ByVal sender As Object, ByVal e As System.EventArgs)
            Try
                Help.ShowHelp(Me, "http:\\www.animatlab.com\Help.htm")
            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try
        End Sub

        Protected Sub OnHelpSearch(ByVal sender As Object, ByVal e As System.EventArgs)
            Try
                Help.ShowHelp(Me, "http:\\www.animatlab.com\Search.htm")
            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try
        End Sub

        Protected Sub OnTechnicalSupport(ByVal sender As Object, ByVal e As System.EventArgs)
            Try
                Help.ShowHelp(Me, "http:\\www.animatlab.com\Contact.htm")
            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try
        End Sub

        Protected Sub OnAboutAnimatLab(ByVal sender As Object, ByVal e As System.EventArgs)
            Try
                Dim frmAbout As New Forms.About
                frmAbout.ShowDialog()
            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try
        End Sub

#Region " Simulation Viewer Events "

        Protected Sub OnSimViewSimulationStarting()
            RaiseEvent SimulationStarting()

            Try
                'Lets setup the external stimuli
                Dim aryDelete As New Collection
                Dim doStim As DataObjects.ExternalStimuli.Stimulus
                For Each deEntry As DictionaryEntry In m_aryProjectStimuli
                    doStim = DirectCast(deEntry.Value, DataObjects.ExternalStimuli.Stimulus)

                    'If the stimulus is no longer valid then delete it.
                    If Not doStim.StimulusNoLongerValid Then
                        doStim.PrepareForSimulation()
                    Else
                        aryDelete.Add(doStim)
                    End If
                Next

                For Each doStim In aryDelete
                    doStim.Delete(False)
                Next

            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try

        End Sub

        Protected Sub OnSimViewSimulationResuming()
            RaiseEvent SimulationResuming()

            Try
                'Lets setup the external stimuli
                Dim doStim As DataObjects.ExternalStimuli.Stimulus
                For Each deEntry As DictionaryEntry In m_aryProjectStimuli
                    doStim = DirectCast(deEntry.Value, DataObjects.ExternalStimuli.Stimulus)
                    doStim.PrepareForSimulation()
                Next

                If Not Me.SimulationView Is Nothing AndAlso Not Me.SimulationView.Simulator Is Nothing AndAlso Me.SimulationView.Simulator.Loaded AndAlso Me.SimulationView.Simulator.Paused Then
                    Me.SimulationView.Simulator.ReInitializeSimulation()
                End If
            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try
        End Sub

        Protected Sub OnSimViewSimulationStarted()
            RaiseEvent SimulationStarted()
        End Sub

        Protected Sub OnSimViewSimulationPaused()
            RaiseEvent SimulationPaused()
        End Sub

        Protected Sub OnSimViewSimulationStopped()
            RaiseEvent SimulationStopped()
        End Sub

#End Region

        Protected Sub OnDoubleClickTool(ByVal oIcon As PanelIcon)
            Try
                'Now lets create a new viewer window for the tool they double clicked.
                If Not oIcon.Data Is Nothing AndAlso TypeOf oIcon.Data Is Forms.Tools.ToolForm Then
                    Dim frmTool As Forms.Tools.ToolForm = DirectCast(oIcon.Data, Forms.Tools.ToolForm)
                    AddNewTool(frmTool)
                End If

            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try
        End Sub

        Public Sub AddNewTool(ByVal frmTool As Forms.Tools.ToolForm)

            Try
                'Now lets create a new viewer window for the tool they double clicked.
                If Not frmTool Is Nothing Then
                    m_iNewToolHolderIndex = m_iNewToolHolderIndex + 1
                    Dim strName As String = "DataTool_" & m_iNewToolHolderIndex

                    Dim frmMdi As New AnimatTools.Forms.Tools.Viewer
                    Dim frmBase As AnimatForm = frmTool.Clone()

                    Me.Cursor = System.Windows.Forms.Cursors.WaitCursor

                    frmMdi.Title = strName
                    frmBase.Title = strName
                    frmMdi.Initialize(Me, frmBase)

                    frmMdi.Show()

                    Dim doHolder As New DataObjects.ToolHolder(m_doFormHelper)
                    frmMdi.ToolHolder = doHolder
                    doHolder.BaseAssemblyFile = frmTool.AssemblyFile
                    doHolder.BaseClassName = frmTool.ClassName
                    doHolder.Name = strName
                    Util.Application.ToolHolders.Add(doHolder.ID, doHolder)
                    doHolder.CreateWorkspaceTreeView(Me.Simulation, Me.ProjectWorkspace)
                    Util.Application.ProjectWorkspace.TreeView.SelectedNode = doHolder.ViewerTreeNode

                    Application.DoEvents()

                    'Save the file so we will have it there if for some reason they decide to close
                    'the currently open window without saving it.
                    Dim oXml As New AnimatTools.Interfaces.StdXml
                    oXml.AddElement("Editor")
                    frmMdi.SaveData(oXml)

                    'Util.Application.SaveProject(Util.Application.ProjectFile)

                    Me.Cursor = System.Windows.Forms.Cursors.Arrow

                End If

            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            Finally
                Me.Cursor = System.Windows.Forms.Cursors.Arrow
            End Try
        End Sub

        Protected Overridable Sub OnViewDockingForm(ByVal sender As Object, ByVal e As EventArgs)

            Try
                Dim menuCommand As MenuCommand = DirectCast(sender, MenuCommand)

                'Now lets try and find a docking content with that title.
                Dim ctDock As Content = m_dockManager.Contents(DirectCast(menuCommand.Tag, String))

                If Not ctDock Is Nothing Then
                    If ctDock.Visible Then
                        m_dockManager.HideContent(ctDock)
                    Else
                        m_dockManager.ShowContent(ctDock)
                    End If
                End If

            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try
        End Sub

        Protected Sub OnDockContentClosing(ByVal c As Content, ByVal e As System.ComponentModel.CancelEventArgs)

            Try

                If c.BackgroundForm Then
                    m_dockManager.HideContent(c)
                    e.Cancel = True
                Else
                    e.Cancel = False

                    If Not c.UserData Is Nothing Then
                        Dim frmAnimat As AnimatForm = DirectCast(c.UserData, AnimatForm)
                        If Not frmAnimat Is Nothing Then
                            frmAnimat.OnContentClosing(e)
                        End If
                    End If
                End If

            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try
        End Sub

        Protected Sub AnimatApplication_Closing(ByVal sender As Object, ByVal e As System.ComponentModel.CancelEventArgs) Handles MyBase.Closing

            Try
                Me.Logger.LogMsg(Interfaces.Logger.enumLogLevel.Info, "Exiting the application. Project: '" & Util.Application.ProjectPath & "\" & Util.Application.ProjectFile)

                'First check to see if the application is dirty. If it is then ask to save the project
                SaveIfDirty(e)

                If e.Cancel Then Return

                RaiseEvent ApplicationExiting()

                m_frmSimulationWnd = Nothing

                'Lets close all of the children first.
                'We have to do this to make certain that all of the controls are closed 
                'down in the appropriate order.
                For Each oChild As Form In Me.MdiChildren
                    oChild.Close()
                Next

                'Clear the list of default menus and toolbars
                m_aryDefaultMenus.Clear()
                m_aryDefaultToolbars.Clear()

                m_barDefault = Nothing
                m_barCurrent = Nothing
                m_mdiClient = Nothing
                m_frmSimulationWnd = Nothing
                m_dsSimulation = Nothing
                m_ipToolPanel = Nothing
                m_wcWorkspaceContent = Nothing
                m_frmWorkspace = Nothing
                m_frmToolbox = Nothing
                m_frmSimulationController = Nothing

                m_aryNeuralModules.Clear()
                m_aryPlugInAssemblies.Clear()
                m_aryBehavioralNodes.Clear()
                m_aryBehavioralLinks.Clear()
                m_aryBodyPartTypes.Clear()
                m_aryRigidBodyTypes.Clear()
                m_aryJointTypes.Clear()
                m_aryBehavioralPanels.Clear()
                m_aryAlphabeticalBehavioralPanels.Clear()
                m_aryToolPlugins.Clear()
                m_aryToolHolders.Clear()
                m_aryProjectStimuli.Clear()

                m_dockManager.Dispose()

                Me.Logger.LogMsg(Interfaces.Logger.enumLogLevel.Debug, "Finished cleanup for application exit.")
            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try

        End Sub


        'The purpose behind these two OnMove handlers is to repost the move windows message to the simulator 
        'window. It uses some windows tricks to subclass a child window created using direct3d. It is created
        'inside its own thread. This means that the messages it gets have to be relayed using the sublassed 
        'windows procedure in the VortexAnimatLibrary module. However, there was a problem with moving the 
        'window. If you moved the parent, client, or frame window the child d3d window would remain where it
        'was originally painted. I managed to come up with some code in the subclassed windproc that gets the
        'screen coordinates of the parent window and uses that to manually move the window so it is positioned
        'correctly. But this still left a problem when the client or frame windows moved. So adding these two
        'overrides informs that window that either the frame or client has moved and that it needs to recalculate
        'its position on the screen.
        Protected Overrides Sub OnMove(ByVal e As System.EventArgs)
            For Each frmWindow As Form In Me.MdiChildren
                AnimatTools.Framework.Util.PostMessage(frmWindow.Handle.ToInt32(), AnimatTools.Framework.Util.WindowsMessages.WM_MOVE, 0, "")
            Next
        End Sub

        Protected Sub OnClientMove(ByVal sender As System.Object, ByVal e As System.EventArgs)
            OnMove(e)
        End Sub

        Private Sub AnimatApplication_Load(ByVal sender As Object, ByVal e As System.EventArgs) Handles MyBase.Load
            CloseSplashScreen()

            'Lets resize the workspace bar.
            If Not m_frmWorkspace Is Nothing Then
                m_frmWorkspace.ctrlTreeView.Height = CInt(Me.Height * 0.5)
            End If

        End Sub

        Private Sub OnMdiClientClicked(ByVal sender As Object, ByVal e As System.EventArgs)
            Try

                For Each ctDock As Content In m_dockManager.Contents
                    If Not ctDock.AutoHidePanel Is Nothing Then
                        ctDock.AutoHidePanel.RestoreToHiddenState()
                    End If
                Next

            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try
        End Sub

        Private Sub m_SplashTimer_Tick(ByVal sender As Object, ByVal e As System.EventArgs) Handles m_SplashTimer.Tick
            Try
                Crownwood.Magic.Forms.SplashForm.CloseSplash()
                'Me.TopMost = True

            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            Finally
                m_SplashTimer.Enabled = False
            End Try
        End Sub

#End Region


    End Class

End Namespace

