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

Namespace Forms

    Public Class SimulationController
        Inherits AnimatForm

#Region " Windows Form Designer generated code "

        Public Sub New()
            MyBase.New()

            'This call is required by the Windows Form Designer.
            InitializeComponent()

            'Add any initialization after the InitializeComponent() call

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
        Friend WithEvents ctrlTimeRuler As Crownwood.Magic.Controls.TimeRuler
        Friend WithEvents lblCursorTime As System.Windows.Forms.Label
        Friend WithEvents lblCursorLabel As System.Windows.Forms.Label
        Friend WithEvents grpSimControls As System.Windows.Forms.GroupBox
        Friend WithEvents btnSimGotoStart As System.Windows.Forms.Button
        Friend WithEvents btnSimStepBack As System.Windows.Forms.Button
        Friend WithEvents btnSimStart As System.Windows.Forms.Button
        Friend WithEvents btnSimStop As System.Windows.Forms.Button
        Friend WithEvents btnSimStepForward As System.Windows.Forms.Button
        Friend WithEvents btnSimGotoEnd As System.Windows.Forms.Button
        Friend WithEvents grpVideoControls As System.Windows.Forms.GroupBox
        Friend WithEvents btnVideoGotoStart As System.Windows.Forms.Button
        Friend WithEvents btnVideoStepBack As System.Windows.Forms.Button
        Friend WithEvents btnVideoStart As System.Windows.Forms.Button
        Friend WithEvents btnVideoStepForward As System.Windows.Forms.Button
        Friend WithEvents btnVideoGotoEnd As System.Windows.Forms.Button
        Friend WithEvents lblVideoStart As System.Windows.Forms.Label
        Friend WithEvents lblVideoEnd As System.Windows.Forms.Label
        Friend WithEvents lblVideoCurrent As System.Windows.Forms.Label
        Friend WithEvents txtVideoStart As System.Windows.Forms.TextBox
        Friend WithEvents txtVideoEnd As System.Windows.Forms.TextBox
        Friend WithEvents txtVideoCurrent As System.Windows.Forms.TextBox
        Friend WithEvents btnVideoSave As System.Windows.Forms.Button
        Friend WithEvents grpSnapshotControls As System.Windows.Forms.GroupBox
        Friend WithEvents lblSnapshotTime As System.Windows.Forms.Label
        Friend WithEvents txtSnapshotTime As System.Windows.Forms.TextBox
        Friend WithEvents m_Timer As New Timer

        <System.Diagnostics.DebuggerStepThrough()> Private Sub InitializeComponent()
            Me.ctrlTimeRuler = New Crownwood.Magic.Controls.TimeRuler

            Me.grpSimControls = New System.Windows.Forms.GroupBox
            Me.btnSimGotoStart = New System.Windows.Forms.Button
            Me.btnSimStepBack = New System.Windows.Forms.Button
            Me.btnSimStart = New System.Windows.Forms.Button
            Me.btnSimStop = New System.Windows.Forms.Button
            Me.btnSimStepForward = New System.Windows.Forms.Button
            Me.btnSimGotoEnd = New System.Windows.Forms.Button

            Me.grpVideoControls = New System.Windows.Forms.GroupBox
            Me.btnVideoGotoStart = New System.Windows.Forms.Button
            Me.btnVideoStepBack = New System.Windows.Forms.Button
            Me.btnVideoStart = New System.Windows.Forms.Button
            Me.btnVideoStepForward = New System.Windows.Forms.Button
            Me.btnVideoGotoEnd = New System.Windows.Forms.Button
            Me.lblVideoStart = New System.Windows.Forms.Label
            Me.lblVideoEnd = New System.Windows.Forms.Label
            Me.lblVideoCurrent = New System.Windows.Forms.Label
            Me.txtVideoStart = New System.Windows.Forms.TextBox
            Me.txtVideoEnd = New System.Windows.Forms.TextBox
            Me.txtVideoCurrent = New System.Windows.Forms.TextBox
            Me.btnVideoSave = New System.Windows.Forms.Button

            Me.grpSnapshotControls = New System.Windows.Forms.GroupBox
            Me.lblSnapshotTime = New System.Windows.Forms.Label
            Me.txtSnapshotTime = New System.Windows.Forms.TextBox

            Me.grpSimControls.SuspendLayout()
            Me.grpVideoControls.SuspendLayout()
            Me.grpSnapshotControls.SuspendLayout()

            Me.SuspendLayout()

            m_Timer.Enabled = False
            m_Timer.Interval = 100

            '
            'ctrlTimeRuler
            '
            Me.ctrlTimeRuler.ActualMillisecond = CType(0, Long)
            Me.ctrlTimeRuler.ActualTimeColor = System.Drawing.Color.LightBlue
            Me.ctrlTimeRuler.Anchor = CType((((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Bottom) _
                        Or System.Windows.Forms.AnchorStyles.Left) _
                        Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
            Me.ctrlTimeRuler.AutomaticTimeScale = True
            Me.ctrlTimeRuler.CurrentMillisecond = CType(0, Long)
            Me.ctrlTimeRuler.CurrentTimeColor = System.Drawing.Color.Blue
            Me.ctrlTimeRuler.DivisionMarkFactor = 4
            Me.ctrlTimeRuler.Divisions = 10
            Me.ctrlTimeRuler.EndMillisecond = CType(5000, Long)
            Me.ctrlTimeRuler.ForeColor = System.Drawing.Color.Black
            Me.ctrlTimeRuler.Location = New System.Drawing.Point(200, 5)
            Me.ctrlTimeRuler.MajorInterval = 1
            Me.ctrlTimeRuler.MiddleMarkFactor = 3
            Me.ctrlTimeRuler.MouseTrackingOn = True
            Me.ctrlTimeRuler.Name = "ctrlTimeRuler"
            Me.ctrlTimeRuler.Orientation = Crownwood.Magic.Controls.enumOrientation.orHorizontal
            Me.ctrlTimeRuler.ProgressBarScale = 40
            Me.ctrlTimeRuler.RulerAlignment = Crownwood.Magic.Controls.enumRulerAlignment.raBottomOrRight
            Me.ctrlTimeRuler.Size = New System.Drawing.Size(100, 110)
            Me.ctrlTimeRuler.StartMillisecond = CType(0, Long)
            Me.ctrlTimeRuler.TabIndex = 0
            Me.ctrlTimeRuler.Text = "Time Ruler"
            Me.ctrlTimeRuler.TimeScale = Crownwood.Magic.Controls.enumTimeScale.Minutes
            Me.ctrlTimeRuler.VerticalNumbers = False
            '
            'grpSimControls
            '
            Me.grpSimControls.Controls.Add(Me.btnSimGotoStart)
            Me.grpSimControls.Controls.Add(Me.btnSimStepBack)
            Me.grpSimControls.Controls.Add(Me.btnSimStart)
            Me.grpSimControls.Controls.Add(Me.btnSimStop)
            Me.grpSimControls.Controls.Add(Me.btnSimStepForward)
            Me.grpSimControls.Controls.Add(Me.btnSimGotoEnd)
            Me.grpSimControls.Location = New System.Drawing.Point(5, 2)
            Me.grpSimControls.Name = "grpSimControls"
            Me.grpSimControls.Size = New System.Drawing.Size(190, 55)
            Me.grpSimControls.TabIndex = 0
            Me.grpSimControls.TabStop = False
            Me.grpSimControls.Text = "Simulation Controls"
            '
            'btnSimGotoStart
            '
            Me.btnSimGotoStart.Location = New System.Drawing.Point(5, 20)
            Me.btnSimGotoStart.Name = "btnSimGotoStart"
            Me.btnSimGotoStart.Size = New System.Drawing.Size(27, 27)
            Me.btnSimGotoStart.TabIndex = 0
            Me.btnSimGotoStart.Text = ""
            Me.btnSimGotoStart.ImageAlign = ContentAlignment.MiddleCenter
            Me.btnSimGotoStart.Enabled = False
            '
            'btnSimStepBack
            '
            Me.btnSimStepBack.Location = New System.Drawing.Point(35, 20)
            Me.btnSimStepBack.Name = "btnSimStepBack"
            Me.btnSimStepBack.Size = New System.Drawing.Size(27, 27)
            Me.btnSimStepBack.TabIndex = 0
            Me.btnSimStepBack.Text = ""
            Me.btnSimStepBack.ImageAlign = ContentAlignment.MiddleCenter
            Me.btnSimStepBack.Enabled = False
            '
            'btnSimStart
            '
            Me.btnSimStart.Location = New System.Drawing.Point(65, 20)
            Me.btnSimStart.Name = "btnSimStart"
            Me.btnSimStart.Size = New System.Drawing.Size(27, 27)
            Me.btnSimStart.TabIndex = 0
            Me.btnSimStart.Text = ""
            Me.btnSimStart.ImageAlign = ContentAlignment.MiddleCenter
            Me.btnSimStart.Enabled = False
            '
            'btnSimSotp
            '
            Me.btnSimStop.Location = New System.Drawing.Point(95, 20)
            Me.btnSimStop.Name = "btnSimStop"
            Me.btnSimStop.Size = New System.Drawing.Size(27, 27)
            Me.btnSimStop.TabIndex = 0
            Me.btnSimStop.Text = ""
            Me.btnSimStop.ImageAlign = ContentAlignment.MiddleCenter
            Me.btnSimStop.Enabled = False
            '
            'btnSimStepForward
            '
            Me.btnSimStepForward.Location = New System.Drawing.Point(125, 20)
            Me.btnSimStepForward.Name = "btnSimStepForward"
            Me.btnSimStepForward.Size = New System.Drawing.Size(27, 27)
            Me.btnSimStepForward.TabIndex = 0
            Me.btnSimStepForward.Text = ""
            Me.btnSimStepForward.ImageAlign = ContentAlignment.MiddleCenter
            Me.btnSimStepForward.Enabled = False
            '
            'btnSimGotoEnd
            '
            Me.btnSimGotoEnd.Location = New System.Drawing.Point(155, 20)
            Me.btnSimGotoEnd.Name = "btnSimStepForward"
            Me.btnSimGotoEnd.Size = New System.Drawing.Size(27, 27)
            Me.btnSimGotoEnd.TabIndex = 0
            Me.btnSimGotoEnd.Text = ""
            Me.btnSimGotoEnd.ImageAlign = ContentAlignment.MiddleCenter
            Me.btnSimGotoEnd.Enabled = False
            '
            'grpVideoControls
            '
            Me.grpVideoControls.Controls.Add(Me.btnVideoGotoStart)
            Me.grpVideoControls.Controls.Add(Me.btnVideoStepBack)
            Me.grpVideoControls.Controls.Add(Me.btnVideoStart)
            Me.grpVideoControls.Controls.Add(Me.btnVideoStepForward)
            Me.grpVideoControls.Controls.Add(Me.btnVideoGotoEnd)
            Me.grpVideoControls.Controls.Add(Me.lblVideoStart)
            Me.grpVideoControls.Controls.Add(Me.lblVideoEnd)
            Me.grpVideoControls.Controls.Add(Me.lblVideoCurrent)
            Me.grpVideoControls.Controls.Add(Me.txtVideoStart)
            Me.grpVideoControls.Controls.Add(Me.txtVideoEnd)
            Me.grpVideoControls.Controls.Add(Me.txtVideoCurrent)
            Me.grpVideoControls.Controls.Add(Me.btnVideoSave)
            Me.grpVideoControls.Location = New System.Drawing.Point(5, 60)
            Me.grpVideoControls.Name = "grpVideoControls"
            Me.grpVideoControls.Size = New System.Drawing.Size(190, 140)
            Me.grpVideoControls.TabIndex = 0
            Me.grpVideoControls.TabStop = False
            Me.grpVideoControls.Text = "Video Controls"
            Me.grpVideoControls.Visible = False
            '
            'btnVideoGotoStart
            '
            Me.btnVideoGotoStart.Location = New System.Drawing.Point(5, 20)
            Me.btnVideoGotoStart.Name = "btnVideoGotoStart"
            Me.btnVideoGotoStart.Size = New System.Drawing.Size(27, 27)
            Me.btnVideoGotoStart.TabIndex = 0
            Me.btnVideoGotoStart.Text = ""
            Me.btnVideoGotoStart.ImageAlign = ContentAlignment.MiddleCenter
            '
            'btnVideoStepBack
            '
            Me.btnVideoStepBack.Location = New System.Drawing.Point(35, 20)
            Me.btnVideoStepBack.Name = "btnVideoStepBack"
            Me.btnVideoStepBack.Size = New System.Drawing.Size(27, 27)
            Me.btnVideoStepBack.TabIndex = 0
            Me.btnVideoStepBack.Text = ""
            Me.btnVideoStepBack.ImageAlign = ContentAlignment.MiddleCenter
            '
            'btnVideoStart
            '
            Me.btnVideoStart.Location = New System.Drawing.Point(65, 20)
            Me.btnVideoStart.Name = "btnVideoStart"
            Me.btnVideoStart.Size = New System.Drawing.Size(27, 27)
            Me.btnVideoStart.TabIndex = 0
            Me.btnVideoStart.Text = ""
            Me.btnVideoStart.ImageAlign = ContentAlignment.MiddleCenter
            '
            'btnVideoStepForward
            '
            Me.btnVideoStepForward.Location = New System.Drawing.Point(95, 20)
            Me.btnVideoStepForward.Name = "btnVideoStepForward"
            Me.btnVideoStepForward.Size = New System.Drawing.Size(27, 27)
            Me.btnVideoStepForward.TabIndex = 0
            Me.btnVideoStepForward.Text = ""
            Me.btnVideoStepForward.ImageAlign = ContentAlignment.MiddleCenter
            '
            'btnVideoGotoEnd
            '
            Me.btnVideoGotoEnd.Location = New System.Drawing.Point(125, 20)
            Me.btnVideoGotoEnd.Name = "btnVideoGotoEnd"
            Me.btnVideoGotoEnd.Size = New System.Drawing.Size(27, 27)
            Me.btnVideoGotoEnd.TabIndex = 0
            Me.btnVideoGotoEnd.Text = ""
            Me.btnVideoGotoEnd.ImageAlign = ContentAlignment.MiddleCenter
            '
            'lblVideoStart
            '
            Me.lblVideoStart.Location = New System.Drawing.Point(5, 50)
            Me.lblVideoStart.Name = "lblVideoStart"
            Me.lblVideoStart.Size = New System.Drawing.Size(75, 20)
            Me.lblVideoStart.TabIndex = 0
            Me.lblVideoStart.Text = "Start Time"
            Me.lblVideoStart.TextAlign = ContentAlignment.MiddleCenter
            '
            'lblVideoEnd
            '
            Me.lblVideoEnd.Location = New System.Drawing.Point(80, 50)
            Me.lblVideoEnd.Name = "lblVideoEnd"
            Me.lblVideoEnd.Size = New System.Drawing.Size(75, 20)
            Me.lblVideoEnd.TabIndex = 0
            Me.lblVideoEnd.Text = "End Time"
            Me.lblVideoEnd.TextAlign = ContentAlignment.MiddleCenter
            '
            '
            'txtVideoStart
            '
            Me.txtVideoStart.Location = New System.Drawing.Point(5, 70)
            Me.txtVideoStart.Name = "txtVideoStart"
            Me.txtVideoStart.Size = New System.Drawing.Size(75, 27)
            Me.txtVideoStart.TabIndex = 0
            Me.txtVideoStart.Text = ""
            '
            'txtVideoEnd
            '
            Me.txtVideoEnd.Location = New System.Drawing.Point(80, 70)
            Me.txtVideoEnd.Name = "txtVideoEnd"
            Me.txtVideoEnd.Size = New System.Drawing.Size(75, 27)
            Me.txtVideoEnd.TabIndex = 0
            Me.txtVideoEnd.Text = ""
            '
            'lblVideoCurrent
            '
            Me.lblVideoCurrent.Location = New System.Drawing.Point(5, 90)
            Me.lblVideoCurrent.Name = "lblVideoCurrent"
            Me.lblVideoCurrent.Size = New System.Drawing.Size(75, 20)
            Me.lblVideoCurrent.TabIndex = 0
            Me.lblVideoCurrent.Text = "Current Time"
            Me.lblVideoCurrent.TextAlign = ContentAlignment.MiddleCenter
            '
            'txtVideoCurrent
            '
            Me.txtVideoCurrent.Location = New System.Drawing.Point(5, 110)
            Me.txtVideoCurrent.Name = "txtVideoCurrent"
            Me.txtVideoCurrent.Size = New System.Drawing.Size(75, 27)
            Me.txtVideoCurrent.TabIndex = 0
            Me.txtVideoCurrent.Text = ""
            '
            'btnVideoSave
            '
            Me.btnVideoSave.Location = New System.Drawing.Point(80, 100)
            Me.btnVideoSave.Name = "btnVideoSave"
            Me.btnVideoSave.Size = New System.Drawing.Size(75, 30)
            Me.btnVideoSave.TabIndex = 0
            Me.btnVideoSave.Text = "Save    " & vbCrLf & "Video"
            Me.btnVideoSave.TextAlign = ContentAlignment.MiddleRight
            Me.btnVideoSave.ImageAlign = ContentAlignment.MiddleLeft
            '
            'grpSnapshotControls
            '
            Me.grpSnapshotControls.Controls.Add(Me.lblSnapshotTime)
            Me.grpSnapshotControls.Controls.Add(Me.txtSnapshotTime)
            Me.grpSnapshotControls.Location = New System.Drawing.Point(5, 60)
            Me.grpSnapshotControls.Name = "grpSnapshotControls"
            Me.grpSnapshotControls.Size = New System.Drawing.Size(190, 140)
            Me.grpSnapshotControls.TabIndex = 0
            Me.grpSnapshotControls.TabStop = False
            Me.grpSnapshotControls.Text = "Snapshot Controls"
            Me.grpSnapshotControls.Visible = False
            '
            'lblSnapshotTime
            '
            Me.lblSnapshotTime.Location = New System.Drawing.Point(5, 20)
            Me.lblSnapshotTime.Name = "lblSnapshotTime"
            Me.lblSnapshotTime.Size = New System.Drawing.Size(75, 20)
            Me.lblSnapshotTime.TabIndex = 0
            Me.lblSnapshotTime.Text = "Current Time"
            Me.lblSnapshotTime.TextAlign = ContentAlignment.MiddleCenter
            '
            'txtSnapshotTime
            '
            Me.txtSnapshotTime.Location = New System.Drawing.Point(5, 40)
            Me.txtSnapshotTime.Name = "txtSnapshotTime"
            Me.txtSnapshotTime.Size = New System.Drawing.Size(75, 27)
            Me.txtSnapshotTime.TabIndex = 0
            Me.txtSnapshotTime.Text = ""

            '
            'SimulationController
            '
            Me.AutoScaleBaseSize = New System.Drawing.Size(5, 13)
            Me.ClientSize = New System.Drawing.Size(872, 125)
            Me.Controls.Add(Me.grpSimControls)
            Me.Controls.Add(Me.grpVideoControls)
            Me.Controls.Add(Me.grpSnapshotControls)
            Me.Controls.Add(Me.ctrlTimeRuler)
            Me.Name = "SimulationController"
            Me.Text = "SimulationController"
            Me.ResumeLayout(False)

        End Sub

#End Region

#Region " Attributes "

        Protected m_simView As AnimatTools.Forms.SimulatorView
        Protected m_selKeyFrame As KeyFrame

        Protected m_snTimeBarInterval As ScaledNumber

        Protected m_snStartingBarTime As AnimatTools.Framework.ScaledNumber
        Protected m_snEndingBarTime As AnimatTools.Framework.ScaledNumber

        'If the onSimEnding event is fired then this is set to true and
        'the next time the timer event fires it causes the simulation to end.
        Protected m_bSimulationEnding As Boolean = False

#End Region

#Region " Properties "

        Public Overrides ReadOnly Property IconName() As String
            Get
                Return "AnimatTools.SimulationController.gif"
            End Get
        End Property

        Public Overridable Property SimulatorView() As AnimatTools.Forms.SimulatorView
            Get
                Return m_simView
            End Get
            Set(ByVal Value As SimulatorView)

                'If this is set then lets remove the handlers associated with this view
                If Not m_simView Is Nothing Then
                    RemoveHandler m_simView.Simulator.OnStartSimulation, AddressOf Me.OnStartSimulation
                    RemoveHandler m_simView.Simulator.OnPauseSimulation, AddressOf Me.OnPauseSimulation
                    RemoveHandler m_simView.Simulator.OnResetSimulation, AddressOf Me.OnResetSimulation
                    RemoveHandler m_simView.Simulator.OnUpdateData, AddressOf Me.OnUpdateData
                    RemoveHandler m_simView.Simulator.OnSimulationError, AddressOf Me.OnSimulationError
                    RemoveHandler m_simView.Simulator.OnEndingSimulation, AddressOf Me.OnSimultionEnding
                End If

                m_simView = Value

                If Not Value Is Nothing Then
                    AddHandler m_simView.Simulator.OnStartSimulation, AddressOf Me.OnStartSimulation
                    AddHandler m_simView.Simulator.OnPauseSimulation, AddressOf Me.OnPauseSimulation
                    AddHandler m_simView.Simulator.OnResetSimulation, AddressOf Me.OnResetSimulation
                    AddHandler m_simView.Simulator.OnUpdateData, AddressOf Me.OnUpdateData
                    AddHandler m_simView.Simulator.OnSimulationError, AddressOf Me.OnSimulationError
                    AddHandler m_simView.Simulator.OnEndingSimulation, AddressOf Me.OnSimultionEnding
                Else
                    Me.ctrlTimeRuler.ResetTime()
                End If

                Me.btnSimStart.ImageIndex = m_frmApplication.LargeImages.GetImageIndex("AnimatTools.PlayLarge.gif")
                Me.btnSimStop.Enabled = False

            End Set
        End Property

        Public Property StartingBarTime() As AnimatTools.Framework.ScaledNumber
            Get
                Return m_snStartingBarTime
            End Get
            Set(ByVal Value As AnimatTools.Framework.ScaledNumber)
                If Value.ActualValue < 0 Then
                    Throw New System.Exception("The starting bar time must be greater than zero.")
                End If

                If Value.ActualValue >= m_snEndingBarTime.ActualValue Then
                    Throw New System.Exception("The starting bar time must be greater than the ending bar time.")
                End If

                m_snStartingBarTime.CopyData(Value)
                Me.ctrlTimeRuler.StartMillisecond = CLng(m_snStartingBarTime.ValueFromDefaultScale)
            End Set
        End Property

        Public Property EndingBarTime() As AnimatTools.Framework.ScaledNumber
            Get
                Return m_snEndingBarTime
            End Get
            Set(ByVal Value As AnimatTools.Framework.ScaledNumber)
                If Value.ActualValue < 0 Then
                    Throw New System.Exception("The ending bar time must be greater than zero.")
                End If

                If Value.ActualValue <= m_snStartingBarTime.ActualValue Then
                    Throw New System.Exception("The ending bar time must be greater than the starting bar time.")
                End If

                m_snEndingBarTime.CopyData(Value)
                Me.ctrlTimeRuler.EndMillisecond = CLng(m_snEndingBarTime.ValueFromDefaultScale)
            End Set
        End Property

        Public Property CurrentTimeColor() As System.Drawing.Color
            Get
                Return Me.ctrlTimeRuler.CurrentTimeColor
            End Get
            Set(ByVal Value As System.Drawing.Color)
                Me.ctrlTimeRuler.CurrentTimeColor = Value
            End Set
        End Property

        Public Property AutomaticTimeScale() As Boolean
            Get
                Return Me.ctrlTimeRuler.AutomaticTimeScale
            End Get
            Set(ByVal Value As Boolean)
                Me.ctrlTimeRuler.AutomaticTimeScale = Value
            End Set
        End Property

        Public Property ActualTimeColor() As System.Drawing.Color
            Get
                Return Me.ctrlTimeRuler.ActualTimeColor
            End Get
            Set(ByVal Value As System.Drawing.Color)
                Me.ctrlTimeRuler.ActualTimeColor = Value
            End Set
        End Property

        Public Property DivisionMarkFactor() As Integer
            Get
                Return Me.ctrlTimeRuler.DivisionMarkFactor
            End Get
            Set(ByVal Value As Integer)
                Me.ctrlTimeRuler.DivisionMarkFactor = Value
            End Set
        End Property

        Public Property Divisions() As Integer
            Get
                Return Me.ctrlTimeRuler.Divisions
            End Get
            Set(ByVal Value As Integer)
                Me.ctrlTimeRuler.Divisions = Value
            End Set
        End Property

        Public Property MajorInterval() As Integer
            Get
                Return Me.ctrlTimeRuler.MajorInterval
            End Get
            Set(ByVal Value As Integer)
                Me.ctrlTimeRuler.MajorInterval = Value
            End Set
        End Property

        Public Property MiddleMarkFactor() As Integer
            Get
                Return Me.ctrlTimeRuler.MiddleMarkFactor
            End Get
            Set(ByVal Value As Integer)
                Me.ctrlTimeRuler.MiddleMarkFactor = Value
            End Set
        End Property

        Public Property TimeScale() As Crownwood.Magic.Controls.enumTimeScale
            Get
                Return Me.ctrlTimeRuler.TimeScale
            End Get
            Set(ByVal Value As Crownwood.Magic.Controls.enumTimeScale)
                Me.ctrlTimeRuler.TimeScale = Value
            End Set
        End Property

        Public Property TimeBarInterval() As ScaledNumber
            Get
                Return m_snTimeBarInterval
            End Get
            Set(ByVal Value As ScaledNumber)
                If Not Value Is Nothing Then
                    If Value.ActualValue < 0 Then
                        Throw New System.Exception("The time bar interval must be greater than zero.")
                    End If

                    m_snTimeBarInterval.CopyData(Value)
                    m_Timer.Interval = CInt(Value.ActualValue * 1000)
                End If
            End Set
        End Property

        Public Property PlaybackRate() As Single
            Get
                Return Util.Environment.PlaybackRate
            End Get
            Set(ByVal Value As Single)
                Util.Environment.PlaybackRate = Value
            End Set
        End Property

        Public Overridable Property EnableSimRecording() As Boolean
            Get
                Return Util.Simulation.EnableSimRecording
            End Get
            Set(ByVal Value As Boolean)
                Util.Simulation.EnableSimRecording = Value
            End Set
        End Property

        Public Overridable Property StartPaused() As Boolean
            Get
                Return Util.Simulation.StartPaused
            End Get
            Set(ByVal Value As Boolean)
                Util.Simulation.StartPaused = Value
            End Set
        End Property

#End Region

#Region " Methods "

        Public Overrides Sub Initialize(ByRef frmApplication As AnimatApplication, _
                                        Optional ByVal frmMdiParent As MdiChild = Nothing, _
                                        Optional ByVal frmParent As AnimatForm = Nothing)
            MyBase.Initialize(frmApplication, frmMdiParent, frmParent)

            m_snStartingBarTime = New AnimatTools.Framework.ScaledNumber(Me.FormHelper, "StartingBarTime", 0, AnimatTools.Framework.ScaledNumber.enumNumericScale.milli, "seconds", "s")
            m_snEndingBarTime = New AnimatTools.Framework.ScaledNumber(Me.FormHelper, "EndingBarTime", 5000, AnimatTools.Framework.ScaledNumber.enumNumericScale.milli, "seconds", "s")
            m_snTimeBarInterval = New AnimatTools.Framework.ScaledNumber(Me.FormHelper, "TimeBarInterval", 1, AnimatTools.Framework.ScaledNumber.enumNumericScale.None, "seconds", "s")

            Dim myAssembly As System.Reflection.Assembly
            myAssembly = System.Reflection.Assembly.Load("AnimatTools")
            Me.UseDocking = False

            AddHandler Me.ctrlTimeRuler.HooverValue, AddressOf Me.OnCursorMoved
            AddHandler Me.ctrlTimeRuler.KeyFrameSelected, AddressOf Me.OnKeyFrameSelected
            AddHandler Me.ctrlTimeRuler.KeyFrameAdded, AddressOf Me.OnKeyFrameAdded
            AddHandler Me.ctrlTimeRuler.KeyFrameRemoved, AddressOf Me.OnKeyFrameRemoved
            AddHandler Me.ctrlTimeRuler.KeyFrameMoved, AddressOf Me.OnKeyFrameMoved
            AddHandler Me.ctrlTimeRuler.KeyFrameMoving, AddressOf Me.OnKeyFrameMoving
            AddHandler Me.ctrlTimeRuler.CurrentFrameMoved, AddressOf Me.OnCurrentFrameMoved
            AddHandler m_frmApplication.ProjectCreated, AddressOf Me.OnProjectCreated
            AddHandler m_frmApplication.ProjectClosed, AddressOf Me.OnProjectClosed
            AddHandler m_frmApplication.ProjectLoaded, AddressOf Me.OnProjectLoaded

            Me.Icon = m_frmApplication.SmallImages.LoadIcon(myAssembly, "AnimatTools.SimulationController.ico")

            If Not frmApplication.Simulation Is Nothing AndAlso Not frmApplication.Simulation.PlaybackControlTreeNode Is Nothing Then
                frmApplication.Simulation.PlaybackControlTreeNode.Tag = Me
            End If

            m_frmApplication.LargeImages.AddImage(myAssembly, "AnimatTools.PlayLarge.gif")
            m_frmApplication.LargeImages.AddImage(myAssembly, "AnimatTools.PauseLarge.gif")
            m_frmApplication.LargeImages.AddImage(myAssembly, "AnimatTools.StopLarge.gif")
            m_frmApplication.LargeImages.AddImage(myAssembly, "AnimatTools.GotoEndLarge.gif")
            m_frmApplication.LargeImages.AddImage(myAssembly, "AnimatTools.GotoStartLarge.gif")
            m_frmApplication.LargeImages.AddImage(myAssembly, "AnimatTools.StepForwardLarge.gif")
            m_frmApplication.LargeImages.AddImage(myAssembly, "AnimatTools.StepBackLarge.gif")
            m_frmApplication.LargeImages.AddImage(myAssembly, "AnimatTools.SaveVideoLarge.gif")

            Me.btnSimGotoStart.ImageList = m_frmApplication.LargeImages.ImageList
            Me.btnSimGotoStart.ImageIndex = m_frmApplication.LargeImages.GetImageIndex("AnimatTools.GotoStartLarge.gif")

            Me.btnSimStepBack.ImageList = m_frmApplication.LargeImages.ImageList
            Me.btnSimStepBack.ImageIndex = m_frmApplication.LargeImages.GetImageIndex("AnimatTools.StepBackLarge.gif")

            Me.btnSimStepBack.ImageList = m_frmApplication.LargeImages.ImageList
            Me.btnSimStepBack.ImageIndex = m_frmApplication.LargeImages.GetImageIndex("AnimatTools.StepBackLarge.gif")

            Me.btnSimStart.ImageList = m_frmApplication.LargeImages.ImageList
            Me.btnSimStart.ImageIndex = m_frmApplication.LargeImages.GetImageIndex("AnimatTools.PlayLarge.gif")

            Me.btnSimStop.ImageList = m_frmApplication.LargeImages.ImageList
            Me.btnSimStop.ImageIndex = m_frmApplication.LargeImages.GetImageIndex("AnimatTools.StopLarge.gif")

            Me.btnSimStepForward.ImageList = m_frmApplication.LargeImages.ImageList
            Me.btnSimStepForward.ImageIndex = m_frmApplication.LargeImages.GetImageIndex("AnimatTools.StepForwardLarge.gif")

            Me.btnSimGotoEnd.ImageList = m_frmApplication.LargeImages.ImageList
            Me.btnSimGotoEnd.ImageIndex = m_frmApplication.LargeImages.GetImageIndex("AnimatTools.GotoEndLarge.gif")


            Me.btnVideoGotoStart.ImageList = m_frmApplication.LargeImages.ImageList
            Me.btnVideoGotoStart.ImageIndex = m_frmApplication.LargeImages.GetImageIndex("AnimatTools.GotoStartLarge.gif")

            Me.btnVideoStepBack.ImageList = m_frmApplication.LargeImages.ImageList
            Me.btnVideoStepBack.ImageIndex = m_frmApplication.LargeImages.GetImageIndex("AnimatTools.StepBackLarge.gif")

            Me.btnVideoStepBack.ImageList = m_frmApplication.LargeImages.ImageList
            Me.btnVideoStepBack.ImageIndex = m_frmApplication.LargeImages.GetImageIndex("AnimatTools.StepBackLarge.gif")

            Me.btnVideoStart.ImageList = m_frmApplication.LargeImages.ImageList
            Me.btnVideoStart.ImageIndex = m_frmApplication.LargeImages.GetImageIndex("AnimatTools.PlayLarge.gif")

            Me.btnVideoStepForward.ImageList = m_frmApplication.LargeImages.ImageList
            Me.btnVideoStepForward.ImageIndex = m_frmApplication.LargeImages.GetImageIndex("AnimatTools.StepForwardLarge.gif")

            Me.btnVideoGotoEnd.ImageList = m_frmApplication.LargeImages.ImageList
            Me.btnVideoGotoEnd.ImageIndex = m_frmApplication.LargeImages.GetImageIndex("AnimatTools.GotoEndLarge.gif")

            Me.btnVideoSave.ImageList = m_frmApplication.LargeImages.ImageList
            Me.btnVideoSave.ImageIndex = m_frmApplication.LargeImages.GetImageIndex("AnimatTools.SaveVideoLarge.gif")
        End Sub

        Public Sub StartSimulation(ByVal bPaused As Boolean)

            Try
                Me.Cursor = System.Windows.Forms.Cursors.WaitCursor

                m_bSimulationEnding = False
                m_Timer.Interval = CInt(m_snTimeBarInterval.ActualValue * 1000)

                Dim bDone As Boolean = False
                If m_simView Is Nothing Then
                    Util.Logger.LogMsg(Interfaces.Logger.enumLogLevel.Info, "Attempting to start a new simulation")

                    If m_frmApplication.OpenSimulationWindow(bPaused) Then
                        If m_simView Is Nothing Then
                            Throw New System.Exception("Unable to obtain a handle to the simulation window")
                        End If

                        If Not bPaused Then
                            Me.btnSimStart.ImageIndex = m_frmApplication.LargeImages.GetImageIndex("AnimatTools.PauseLarge.gif")
                            Me.btnSimStop.Enabled = True
                            Me.ctrlTimeRuler.AllowKeyFrameSelection = False
                            Me.m_Timer.Enabled = True
                        End If
                    Else
                        'If we fail to open the simulation window then make sure the sim view pointer here is nullified.
                        Me.SimulatorView = Nothing
                        Me.m_Timer.Enabled = False
                    End If
                Else
                    Util.Logger.LogMsg(Interfaces.Logger.enumLogLevel.Info, "Attempting to resume a simulation")

                    'If the current millisecond is less than the actual millisecond then we have moved the 
                    'keyframe to a new location.
                    If m_simView.Simulator.Paused AndAlso Me.ctrlTimeRuler.CurrentMillisecond < Me.ctrlTimeRuler.ActualMillisecond Then
                        Me.ctrlTimeRuler.ActualMillisecond = Me.ctrlTimeRuler.CurrentMillisecond
                    End If

                    m_simView.ToggleSimulation()
                    Me.m_Timer.Enabled = Not Me.m_Timer.Enabled
                End If

                Util.Logger.LogMsg(Interfaces.Logger.enumLogLevel.Info, "Simulation was started or resumed.")

            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            Finally
                Me.Cursor = System.Windows.Forms.Cursors.Default
            End Try

        End Sub

        Protected Function GenerateStatusBarText(ByVal dblCursorTime As Double) As String
            Dim strStatus As String = "Current Time: " & Me.ctrlTimeRuler.CurrentMillisecond.ToString & " (ms) "

            If Me.ctrlTimeRuler.CurrentMillisecond < Me.ctrlTimeRuler.ActualMillisecond Then
                strStatus += " Actual Time: " & Me.ctrlTimeRuler.ActualMillisecond.ToString & " (ms) "
            End If

            If dblCursorTime >= 0 Then
                strStatus += " Cursor Time: " & dblCursorTime & " (ms) "
            End If

            Return strStatus
        End Function

        Protected Overrides Sub BuildProperties()

            Dim pbNumberBag As Crownwood.Magic.Controls.PropertyBag = m_snStartingBarTime.Properties
            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Start Time", pbNumberBag.GetType(), "StartingBarTime", _
                                        "Time Bar Settings", "Sets the starting time used for the controller time bar.", _
                                        pbNumberBag, "", GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)))

            pbNumberBag = m_snEndingBarTime.Properties
            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("End Time", pbNumberBag.GetType(), "EndingBarTime", _
                                        "Time Bar Settings", "Sets the ending time used for the controller time bar.", _
                                        pbNumberBag, "", GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)))

            pbNumberBag = m_snTimeBarInterval.Properties
            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Time Bar Interval", pbNumberBag.GetType(), "TimeBarInterval", _
                                        "Time Bar Settings", "Determines how often the the time bar updates its information.", _
                                        pbNumberBag, "", GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)))

            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Actual Time Color", GetType(System.Drawing.Color), "ActualTimeColor", _
                                        "Time Bar Settings", "Sets the color used for the actual time in the controller time bar.", Me.ctrlTimeRuler.ActualTimeColor))

            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Current Time Color", GetType(System.Drawing.Color), "CurrentTimeColor", _
                                        "Time Bar Settings", "Sets the color used for the current time in the controller time bar.", Me.ctrlTimeRuler.CurrentTimeColor))

            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Automatic Time Scale", GetType(Boolean), "AutomaticTimeScale", _
                                        "Time Bar Settings", "Determines whether the time bar will automatically scale itself when the time nears " & _
                                        "the end of bar.", Me.ctrlTimeRuler.AutomaticTimeScale))

            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Division Mark Height", Me.ctrlTimeRuler.DivisionMarkFactor.GetType, "DivisionMarkFactor", _
                                        "Time Bar Settings", "Sets the height of the division mark factor in the time bar.", Me.ctrlTimeRuler.DivisionMarkFactor))

            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Divisions", Me.ctrlTimeRuler.Divisions.GetType, "Divisions", _
                                        "Time Bar Settings", "Sets the number of divisions used for the time bar.", Me.ctrlTimeRuler.Divisions))

            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Major Interval", Me.ctrlTimeRuler.Divisions.GetType, "MajorInterval", _
                                        "Time Bar Settings", "Sets the major interval used for the time bar.", Me.ctrlTimeRuler.MajorInterval))

            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Middle Mark Height", Me.ctrlTimeRuler.MiddleMarkFactor.GetType, "MiddleMarkFactor", _
                                        "Time Bar Settings", "Sets the height of the middle mark factor in the time bar.", Me.ctrlTimeRuler.MiddleMarkFactor))

            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Time Scale", Me.ctrlTimeRuler.TimeScale.GetType, "TimeScale", _
                                        "Time Bar Settings", "Sets the time scale used for the time bar.", Me.ctrlTimeRuler.TimeScale))

            'm_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Update Data Interval", m_iUpdateDataInterval.GetType(), "UpdateDataInterval", _
            '                            "Playback Control", "This controls how often the data is updated during the run of the simulation. " & _
            '                            "For example, it will control how often the progress toolbar is refreshed.", m_iUpdateDataInterval))

            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Start Paused", GetType(Boolean), "StartPaused", _
                                        "Playback Control", "This determines whether the simulation is paused when " & _
                                        "it starts or if it begins running immediately.", Util.Simulation.StartPaused))

            'm_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Enable Sim Recording", GetType(Boolean), "EnableSimRecording", _
            '                            "Playback Control", "If this is true then the simulation recording and playback feature is enabled. " & _
            '                            "If this is turned off it may slightly improve the simulation speed.", Util.Simulation.EnableSimRecording))

            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Playback Rate", Util.Environment.PlaybackRate.GetType, "PlaybackRate", _
                                        "Playback Control", "This controls the rate at which the simulation plays. Increasing the value speeds up the " & _
                                        "the playback, decreasing the value below 1 slows it down. This value must be between 0 and 1.", Util.Environment.PlaybackRate))
        End Sub


        Public Overrides Sub LoadData(ByRef oXml As AnimatTools.Interfaces.StdXml)
            MyBase.LoadData(oXml)

            oXml.IntoElem() 'Into Form Element

            m_snStartingBarTime.LoadData(oXml, "StartingBarTime")
            m_snEndingBarTime.LoadData(oXml, "EndingBarTime")

            If ScaledNumber.IsValidXml(oXml, "TimeBarInterval") Then
                m_snTimeBarInterval.LoadData(oXml, "TimeBarInterval")
            Else
                Dim iTimeBarInterval As Integer = oXml.GetChildInt("TimeBarInterval", CInt(m_snTimeBarInterval.ActualValue * 1000))
                m_snTimeBarInterval.ActualValue = (iTimeBarInterval / 1000)
            End If
            m_Timer.Interval = CInt(m_snTimeBarInterval.ActualValue * 1000)

            Me.ctrlTimeRuler.StartMillisecond = CLng(m_snStartingBarTime.ValueFromDefaultScale)
            Me.ctrlTimeRuler.EndMillisecond = CLng(m_snEndingBarTime.ValueFromDefaultScale)

            Me.ctrlTimeRuler.ActualTimeColor = Color.FromArgb(oXml.GetChildLong("ActualTimeColor", Me.ctrlTimeRuler.ActualTimeColor.ToArgb))
            Me.ctrlTimeRuler.CurrentTimeColor = Color.FromArgb(oXml.GetChildLong("CurrentTimeColor", Me.ctrlTimeRuler.CurrentTimeColor.ToArgb))
            Me.ctrlTimeRuler.AutomaticTimeScale = oXml.GetChildBool("AutomaticTimeScale", Me.ctrlTimeRuler.AutomaticTimeScale)
            Me.ctrlTimeRuler.DivisionMarkFactor = oXml.GetChildInt("DivisionMarkFactor", Me.ctrlTimeRuler.DivisionMarkFactor)
            Me.ctrlTimeRuler.Divisions = oXml.GetChildInt("Divisions", Me.ctrlTimeRuler.Divisions)
            Me.ctrlTimeRuler.MajorInterval = oXml.GetChildInt("MajorInterval", Me.ctrlTimeRuler.MajorInterval)
            Me.ctrlTimeRuler.MiddleMarkFactor = oXml.GetChildInt("MiddleMarkFactor", Me.ctrlTimeRuler.MiddleMarkFactor)
            Me.ctrlTimeRuler.TimeScale = DirectCast([Enum].Parse(GetType(Crownwood.Magic.Controls.enumTimeScale), oXml.GetChildString("TimeScale"), True), Crownwood.Magic.Controls.enumTimeScale)
            oXml.AddChildElement("TimeScale", Me.ctrlTimeRuler.TimeScale.ToString)

            oXml.OutOfElem() 'Outof Form Element

        End Sub

        Public Overrides Sub SaveData(ByRef oXml As AnimatTools.Interfaces.StdXml)
            MyBase.SaveData(oXml)

            oXml.IntoElem()

            m_snStartingBarTime.SaveData(oXml, "StartingBarTime")
            m_snEndingBarTime.SaveData(oXml, "EndingBarTime")
            m_snTimeBarInterval.SaveData(oXml, "TimeBarInterval")

            oXml.AddChildElement("ActualTimeColor", Me.ctrlTimeRuler.ActualTimeColor.ToArgb)
            oXml.AddChildElement("CurrentTimeColor", Me.ctrlTimeRuler.CurrentTimeColor.ToArgb)
            oXml.AddChildElement("AutomaticTimeScale", Me.ctrlTimeRuler.AutomaticTimeScale)
            oXml.AddChildElement("DivisionMarkFactor", Me.ctrlTimeRuler.DivisionMarkFactor)
            oXml.AddChildElement("Divisions", Me.ctrlTimeRuler.Divisions)
            oXml.AddChildElement("MajorInterval", Me.ctrlTimeRuler.MajorInterval)
            oXml.AddChildElement("MiddleMarkFactor", Me.ctrlTimeRuler.MiddleMarkFactor)
            oXml.AddChildElement("TimeScale", Me.ctrlTimeRuler.TimeScale.ToString)

            oXml.OutOfElem()

        End Sub

#End Region

#Region " Events "

#Region " Simulator Events "

        Protected Overrides Sub WndProc(ByRef m As Message)

            Try
                If (m.Msg = Util.WindowsMessages.WM_AM_UPDATE_DATA) Then
                    Me.ctrlTimeRuler.CurrentMillisecond = m_simView.Simulator.CurrentMillisecond
                    m_frmApplication.StatusBar.Panels(0).Text = GenerateStatusBarText(-1)
                ElseIf (m.Msg = Util.WindowsMessages.WM_AM_SIMULATION_ERROR) Then
                    Me.btnSimStart.ImageIndex = m_frmApplication.LargeImages.GetImageIndex("AnimatTools.PlayLarge.gif")
                    Me.btnSimStop.Enabled = False

                    Me.Cursor = System.Windows.Forms.Cursors.Default
                    m_Timer.Enabled = False
                    If Not m_simView Is Nothing Then
                        m_simView.SimulationErrorOccured()
                        MessageBox.Show(m_simView.Simulator.ErrorMessage, "Simulation Error", MessageBoxButtons.OK, MessageBoxIcon.Error)

                        Util.Application.SimulationWindow.Close()
                    End If

                End If
                MyBase.WndProc(m)

            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try

        End Sub

        Private Sub OnStartSimulation()

            Try
                Me.btnSimStart.ImageIndex = m_frmApplication.LargeImages.GetImageIndex("AnimatTools.PauseLarge.gif")
                Me.ctrlTimeRuler.AllowKeyFrameSelection = False
                Me.btnSimStop.Enabled = True

            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try

        End Sub

        Private Sub OnPauseSimulation()

            Try
                Me.btnSimStart.ImageIndex = m_frmApplication.LargeImages.GetImageIndex("AnimatTools.PlayLarge.gif")
                Me.ctrlTimeRuler.AllowKeyFrameSelection = True
                Me.btnSimStop.Enabled = True

            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try

        End Sub

        Private Sub OnStopSimulation()

            Try
                Me.btnSimStart.ImageIndex = m_frmApplication.LargeImages.GetImageIndex("AnimatTools.PlayLarge.gif")
                Me.ctrlTimeRuler.AllowKeyFrameSelection = True
                Me.btnSimStop.Enabled = True

            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try

        End Sub

        Private Sub OnResetSimulation()

        End Sub

        Private Sub OnUpdateData()
            'This is sending a message so the bar processes the message asynchronously and does not lock up on this task. The message is 
            'processed in the WndProc method above.
            AnimatTools.Framework.Util.PostMessage(Me.Handle.ToInt32(), AnimatTools.Framework.Util.WindowsMessages.WM_AM_UPDATE_DATA, 0, "")
        End Sub

        Private Delegate Sub OnSimulationErrorCallback()

        Private Sub OnSimulationError()
            If Me.InvokeRequired Then
                Dim simErrorCB As New OnSimulationErrorCallback(AddressOf OnSimulationError)
                Me.Invoke(simErrorCB, Nothing)
            Else
                AnimatTools.Framework.Util.PostMessage(Me.Handle.ToInt32(), AnimatTools.Framework.Util.WindowsMessages.WM_AM_SIMULATION_ERROR, 0, "")
            End If

        End Sub

        Private Sub OnSimultionEnding()
            Try
                m_bSimulationEnding = True
            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try

        End Sub

        Private Sub m_Timer_Tick(ByVal sender As Object, ByVal e As System.EventArgs) Handles m_Timer.Tick
            Try

                If Not m_simView Is Nothing AndAlso Not m_simView.Simulator Is Nothing Then
                    Me.ctrlTimeRuler.CurrentMillisecond = m_simView.Simulator.CurrentMillisecond
                    m_frmApplication.StatusBar.Panels(0).Text = GenerateStatusBarText(-1)
                End If

                'If this is true then the OnSimulationEnding event has fired and we need to shutdownt the simulation
                If m_bSimulationEnding Then
                    btnSimStop_Click(Me, New System.EventArgs)
                End If

            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
                m_Timer.Enabled = False
            End Try

        End Sub

#End Region

#Region " KeyFrame Events "

        Private Sub OnCursorMoved(ByVal sender As Object, ByVal e As Crownwood.Magic.Controls.TimeRuler.HooverValueEventArgs)
            Try
                m_frmApplication.StatusBar.Panels(0).Text = GenerateStatusBarText(e.Value)

            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
                m_Timer.Enabled = False
            End Try

        End Sub

        Private Sub OnKeyFrameSelected(ByVal sender As Object, ByVal e As Crownwood.Magic.Controls.TimeRuler.KeyFrameEventArgs)

            Try
                'First if the currently selected keyframe is a video type then we need to disable 
                'video playback option.
                If Not m_selKeyFrame Is Nothing Then
                    If m_selKeyFrame.KeyFrameType = KeyFrame.enumKeyFrameType.Video Then
                        If Not m_simView Is Nothing AndAlso Not m_simView.Simulator Is Nothing Then
                            m_simView.Simulator.DisableVideoPlayback()
                        End If
                    End If
                End If

                m_selKeyFrame = e.SelectedKeyFrame

                txtVideoStart.Text = ""
                txtVideoEnd.Text = ""
                txtVideoCurrent.Text = ""
                txtSnapshotTime.Text = ""

                If m_selKeyFrame Is Nothing Then
                    grpVideoControls.Visible = False
                    grpSnapshotControls.Visible = False
                Else
                    If m_selKeyFrame.KeyFrameType = KeyFrame.enumKeyFrameType.Snapshot Then
                        grpVideoControls.Visible = False
                        grpSnapshotControls.Visible = True
                        txtSnapshotTime.Text = m_selKeyFrame.StartMillisecond.ToString()
                    ElseIf m_selKeyFrame.KeyFrameType = KeyFrame.enumKeyFrameType.Video Then
                        grpSnapshotControls.Visible = False
                        grpVideoControls.Visible = True
                        txtVideoStart.Text = m_selKeyFrame.StartMillisecond.ToString()
                        txtVideoEnd.Text = m_selKeyFrame.EndMillisecond.ToString()

                        If Not m_simView Is Nothing AndAlso Not m_simView.Simulator Is Nothing Then
                            m_simView.Simulator.EnableVideoPlayback(m_selKeyFrame.ID)
                        End If
                    End If
                End If

            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
                m_Timer.Enabled = False
            End Try

        End Sub

        Private Sub OnKeyFrameAdded(ByVal sender As Object, ByVal e As Crownwood.Magic.Controls.TimeRuler.KeyFrameEventArgs)
            Try
                If Not e.SelectedKeyFrame Is Nothing AndAlso Not m_simView Is Nothing AndAlso Not m_simView.Simulator Is Nothing Then
                    e.SelectedKeyFrame.ID = m_simView.Simulator.AddKeyFrame(e.SelectedKeyFrame.KeyFrameType.ToString(), _
                                                                            CInt(e.SelectedKeyFrame.StartMillisecond), _
                                                                            CInt(e.SelectedKeyFrame.EndMillisecond))
                End If
            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try
        End Sub

        Private Sub OnKeyFrameRemoved(ByVal sender As Object, ByVal e As Crownwood.Magic.Controls.TimeRuler.KeyFrameEventArgs)
            Try
                If Not e.SelectedKeyFrame Is Nothing AndAlso Not m_simView Is Nothing AndAlso Not m_simView.Simulator Is Nothing Then
                    m_simView.Simulator.RemoveKeyFrame(e.SelectedKeyFrame.ID)
                End If
            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try
        End Sub

        Private Sub OnKeyFrameMoved(ByVal sender As Object, ByVal e As Crownwood.Magic.Controls.TimeRuler.KeyFrameEventArgs)
            Try
                If Not e.SelectedKeyFrame Is Nothing AndAlso Not m_simView Is Nothing AndAlso Not m_simView.Simulator Is Nothing _
                   AndAlso e.SelectedKeyFrame.KeyFrameType <> KeyFrame.enumKeyFrameType.CurrentFrame Then
                    e.SelectedKeyFrame.ID = m_simView.Simulator.MoveKeyFrame(e.SelectedKeyFrame.ID, _
                                                                             CInt(e.SelectedKeyFrame.StartMillisecond), _
                                                                             CInt(e.SelectedKeyFrame.EndMillisecond))
                End If
            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try
        End Sub

        Private Sub OnKeyFrameMoving(ByVal sender As Object, ByVal e As Crownwood.Magic.Controls.TimeRuler.KeyFrameEventArgs)
            Try
                m_selKeyFrame = e.SelectedKeyFrame

                If Not m_selKeyFrame Is Nothing Then
                    If m_selKeyFrame.KeyFrameType = KeyFrame.enumKeyFrameType.Snapshot Then
                        Me.txtSnapshotTime.Text = m_selKeyFrame.StartMillisecond.ToString()
                    ElseIf m_selKeyFrame.KeyFrameType = KeyFrame.enumKeyFrameType.Video Then
                        Me.txtVideoStart.Text = m_selKeyFrame.StartMillisecond.ToString()
                        Me.txtVideoEnd.Text = m_selKeyFrame.EndMillisecond.ToString()
                    End If
                End If

            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try

        End Sub

        Private Sub OnCurrentFrameMoved(ByVal sender As Object, ByVal e As Crownwood.Magic.Controls.TimeRuler.KeyFrameEventArgs)
            Try
                If Not e.SelectedKeyFrame Is Nothing AndAlso Not m_simView Is Nothing AndAlso Not m_simView.Simulator Is Nothing _
                   AndAlso e.SelectedKeyFrame.KeyFrameType <> KeyFrame.enumKeyFrameType.CurrentFrame Then
                    m_simView.Simulator.MoveSimulationToKeyFrame(e.SelectedKeyFrame.ID)
                Else
                    m_simView.Simulator.MoveSimulationToKeyFrame("")
                End If
            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try
        End Sub

#End Region

#Region " Simulation Group Events "

        Private Sub btnSimStart_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles btnSimStart.Click
            Me.StartSimulation(False)
        End Sub

        Private Sub btnSimStop_Click(ByVal sender As Object, ByVal e As System.EventArgs) Handles btnSimStop.Click
            Try
                If Not Util.Application.SimulationWindow Is Nothing Then
                    Util.Application.SimulationWindow.Close()
                End If
                btnSimStop.Enabled = False
                Me.m_Timer.Enabled = False

            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try
        End Sub

#End Region

#Region " Video Group Events "

        Private Sub btnVideoStart_Click(ByVal sender As Object, ByVal e As System.EventArgs) Handles btnVideoStart.Click
            Try

                If m_simView Is Nothing Then
                    Throw New System.Exception("There is no simulation view defined.")
                End If

                If m_selKeyFrame Is Nothing Then
                    Throw New System.Exception("There is no key frame currently selected.")
                End If

                If m_selKeyFrame.KeyFrameType <> KeyFrame.enumKeyFrameType.Video Then
                    Throw New System.Exception("The currently selected keyframe is not a video type.")
                End If

                If Not m_selKeyFrame.Playing Then
                    Me.btnVideoStart.ImageIndex = m_frmApplication.LargeImages.GetImageIndex("AnimatTools.PauseLarge.gif")
                    m_simView.Simulator.StartVideoPlayback()
                    m_selKeyFrame.Playing = True
                Else
                    Me.btnVideoStart.ImageIndex = m_frmApplication.LargeImages.GetImageIndex("AnimatTools.PlayLarge.gif")
                    m_simView.Simulator.StopVideoPlayback()
                    m_selKeyFrame.Playing = False
                End If

            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try

        End Sub

        Private Sub btnVideoStepBack_Click(ByVal sender As Object, ByVal e As System.EventArgs) Handles btnVideoStepBack.Click
            Try
                If m_simView Is Nothing Then
                    Throw New System.Exception("There is no simulation view defined.")
                End If

                If m_selKeyFrame Is Nothing Then
                    Throw New System.Exception("There is no key frame currently selected.")
                End If

                If m_selKeyFrame.KeyFrameType <> KeyFrame.enumKeyFrameType.Video Then
                    Throw New System.Exception("The currently selected keyframe is not a video type.")
                End If

                If m_selKeyFrame.Playing Then
                    Throw New System.Exception("You can not step the video back while it is currently playing.")
                End If

                m_simView.Simulator.StepVideoPlayback(-1)

            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try

        End Sub

        Private Sub btnVideoStepForward_Click(ByVal sender As Object, ByVal e As System.EventArgs) Handles btnVideoStepForward.Click

            Try
                If m_simView Is Nothing Then
                    Throw New System.Exception("There is no simulation view defined.")
                End If

                If m_selKeyFrame Is Nothing Then
                    Throw New System.Exception("There is no key frame currently selected.")
                End If

                If m_selKeyFrame.KeyFrameType <> KeyFrame.enumKeyFrameType.Video Then
                    Throw New System.Exception("The currently selected keyframe is not a video type.")
                End If

                If m_selKeyFrame.Playing Then
                    Throw New System.Exception("You can not step the video forward while it is currently playing.")
                End If

                m_simView.Simulator.StepVideoPlayback(1)

            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try

        End Sub

        Private Sub txtVideoStart_Leave(ByVal sender As Object, ByVal e As System.EventArgs) Handles txtVideoStart.Leave
            Try

                If Not m_selKeyFrame Is Nothing Then
                    If txtVideoStart.Text.Trim.Length = 0 Then
                        Throw New System.Exception("The start time of the video must be numeric.")
                    End If

                    If Not IsNumeric(txtVideoStart.Text) Then
                        Throw New System.Exception("The start time of the video must be numeric.")
                    End If

                    Dim lStartTime As Long = CLng(txtVideoStart.Text)
                    Dim lEndTime As Long = CLng(txtVideoEnd.Text)

                    If lStartTime < 0 Then
                        Throw New System.Exception("The start time must be greater than zero.")
                    End If

                    If lStartTime >= lEndTime Then
                        Throw New System.Exception("The start time can not be greater than or equal to the end time.")
                    End If

                    m_selKeyFrame.MoveFrame(lStartTime, lEndTime, Me.ctrlTimeRuler)

                    If Not m_selKeyFrame Is Nothing AndAlso Not m_simView Is Nothing AndAlso Not m_simView.Simulator Is Nothing Then
                        m_selKeyFrame.ID = m_simView.Simulator.MoveKeyFrame(m_selKeyFrame.ID, _
                                                                            CInt(m_selKeyFrame.StartMillisecond), _
                                                                            CInt(m_selKeyFrame.EndMillisecond))
                    End If

                End If

            Catch ex As System.Exception
                txtVideoStart.Text = m_selKeyFrame.StartMillisecond.ToString()
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try

        End Sub

        Private Sub txtVideoEnd_Leave(ByVal sender As Object, ByVal e As System.EventArgs) Handles txtVideoEnd.Leave
            Try

                If Not m_selKeyFrame Is Nothing Then
                    If txtVideoEnd.Text.Trim.Length = 0 Then
                        Throw New System.Exception("The end time of the video must be numeric.")
                    End If

                    If Not IsNumeric(txtVideoEnd.Text) Then
                        Throw New System.Exception("The end time of the video must be numeric.")
                    End If

                    Dim lStartTime As Long = CLng(txtVideoStart.Text)
                    Dim lEndTime As Long = CLng(txtVideoEnd.Text)

                    If lStartTime < 0 Then
                        Throw New System.Exception("The end time must be greater than zero.")
                    End If

                    If lStartTime >= lEndTime Then
                        Throw New System.Exception("The end time can not be greater than or equal to the end time.")
                    End If

                    m_selKeyFrame.MoveFrame(lStartTime, lEndTime, Me.ctrlTimeRuler)

                    If Not m_selKeyFrame Is Nothing AndAlso Not m_simView Is Nothing AndAlso Not m_simView.Simulator Is Nothing Then
                        m_selKeyFrame.ID = m_simView.Simulator.MoveKeyFrame(m_selKeyFrame.ID, _
                                                                            CInt(m_selKeyFrame.StartMillisecond), _
                                                                            CInt(m_selKeyFrame.EndMillisecond))
                    End If

                End If

            Catch ex As System.Exception
                txtVideoEnd.Text = m_selKeyFrame.StartMillisecond.ToString()
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try

        End Sub

        Private Sub btnVideoSave_Click(ByVal sender As Object, ByVal e As System.EventArgs) Handles btnVideoSave.Click

            Try
                If Not m_selKeyFrame Is Nothing AndAlso Not m_simView Is Nothing AndAlso Not m_simView.Simulator Is Nothing Then
                    Dim dlgBrowser As New FolderBrowserDialog
                    ' Set the Help text description for the FolderBrowserDialog.
                    dlgBrowser.Description = "Select the directory where you want to save the video image frames."

                    Dim result As DialogResult = dlgBrowser.ShowDialog()

                    If result = DialogResult.OK Then
                        m_simView.Simulator.SaveVideo(dlgBrowser.SelectedPath)
                    End If
                End If

            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try

        End Sub

#End Region

#Region " Snapshot Group Events "

        Private Sub txtSnapshotTime_Leave(ByVal sender As Object, ByVal e As System.EventArgs) Handles txtSnapshotTime.Leave
            Try

                If Not m_selKeyFrame Is Nothing Then
                    If txtSnapshotTime.Text.Trim.Length = 0 Then
                        Throw New System.Exception("The time of the snapshot must be numeric.")
                    End If

                    If Not IsNumeric(txtSnapshotTime.Text) Then
                        Throw New System.Exception("The time of the snapshot must be numeric.")
                    End If

                    Dim lTime As Long = CLng(txtSnapshotTime.Text)

                    If lTime < 0 Then
                        Throw New System.Exception("The time must be greater than zero.")
                    End If

                    m_selKeyFrame.MoveFrame(lTime, lTime, Me.ctrlTimeRuler)

                    If Not m_selKeyFrame Is Nothing AndAlso Not m_simView Is Nothing AndAlso Not m_simView.Simulator Is Nothing Then
                        m_selKeyFrame.ID = m_simView.Simulator.MoveKeyFrame(m_selKeyFrame.ID, _
                                                                            CInt(m_selKeyFrame.StartMillisecond), _
                                                                            CInt(m_selKeyFrame.EndMillisecond))
                    End If

                End If

            Catch ex As System.Exception
                txtSnapshotTime.Text = m_selKeyFrame.StartMillisecond.ToString()
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try

        End Sub

#End Region

#Region " Project Events "

        Private Sub OnProjectCreated()
            Try
                'btnSimGotoStart.Enabled = True
                'btnSimStepBack.Enabled = True
                btnSimStart.Enabled = True
                btnSimStop.Enabled = False
                'btnSimStepForward.Enabled = True
                'btnSimGotoEnd.Enabled = True

            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try

        End Sub

        Private Sub OnProjectClosed()
            Try
                btnSimGotoStart.Enabled = False
                btnSimStepBack.Enabled = False
                btnSimStart.Enabled = False
                btnSimStop.Enabled = False
                btnSimStepForward.Enabled = False
                btnSimGotoEnd.Enabled = False

            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try

        End Sub

        Private Sub OnProjectLoaded()
            Try
                'btnSimGotoStart.Enabled = True
                'btnSimStepBack.Enabled = True
                btnSimStart.Enabled = True
                btnSimStop.Enabled = False
                'btnSimStepForward.Enabled = True
                'btnSimGotoEnd.Enabled = True

            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try

        End Sub

#End Region

#End Region


    End Class

End Namespace
