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

Namespace Forms.Tools

    Public MustInherit Class DataChart
        Inherits ToolForm

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
        <System.Diagnostics.DebuggerStepThrough()> Private Sub InitializeComponent()
            components = New System.ComponentModel.Container
            Me.Title = "DataChart"
        End Sub

#End Region

#Region " Attributes "

        Protected m_gnGain As AnimatTools.DataObjects.Gain
        'protected m_dcChart as 

        Protected m_bAutoCollectDataInterval As Boolean = True

        Protected m_snCollectDataInterval As AnimatTools.Framework.ScaledNumber
        Protected m_snCollectTimeWindow As AnimatTools.Framework.ScaledNumber
        Protected m_snUpdateDataInterval As AnimatTools.Framework.ScaledNumber

        Protected m_bSetStartEndTime As Boolean = True
        Protected m_snCollectStartTime As AnimatTools.Framework.ScaledNumber
        Protected m_snCollectEndTime As AnimatTools.Framework.ScaledNumber

        Protected m_aryAxisList As New Collections.SortedAxisList(m_doFormHelper)

        Protected m_aryAutoFillColors(20) As System.Drawing.Color
        Protected m_iAutoFillColor As Integer = 0

#End Region

#Region " Properties "

        Public Overridable Property Gain() As AnimatTools.DataObjects.Gain
            Get
                Return m_gnGain
            End Get
            Set(ByVal Value As AnimatTools.DataObjects.Gain)
                m_gnGain = Value
                DrawGainChart()
            End Set
        End Property

        Public Overridable Property AutoCollectDataInterval() As Boolean
            Get
                Return m_bAutoCollectDataInterval
            End Get
            Set(ByVal Value As Boolean)
                m_bAutoCollectDataInterval = Value
                If m_bAutoCollectDataInterval Then
                    ResetCollectDataInterval()
                End If
                Me.Viewer.RefreshProperties()
            End Set
        End Property

        Public Overridable Property CollectDataInterval() As AnimatTools.Framework.ScaledNumber
            Get
                Return m_snCollectDataInterval
            End Get
            Set(ByVal Value As AnimatTools.Framework.ScaledNumber)

                If Value.ActualValue <= 0 Then
                    Throw New System.Exception("The collect data interval must be greater than zero.")
                End If

                If Value.ActualValue >= m_snUpdateDataInterval.ActualValue Then
                    Throw New System.Exception("The collect data interval must be less than the update data interval.")
                End If

                m_snCollectDataInterval.CopyData(Value)

                If Not Util.Application.SimulationView Is Nothing AndAlso Not Util.Application.SimulationView.Simulator Is Nothing _
                   AndAlso Util.Application.SimulationView.Simulator.Loaded AndAlso Util.Application.SimulationView.Simulator.Paused Then
                    Util.Application.SimulationView.Simulator.ModifyDataChart(m_strID, CSng(m_snCollectTimeWindow.ActualValue), CSng(m_snCollectDataInterval.ActualValue))
                End If

            End Set
        End Property

        Public Overridable Property CollectTimeWindow() As AnimatTools.Framework.ScaledNumber
            Get
                Return m_snCollectTimeWindow
            End Get
            Set(ByVal Value As AnimatTools.Framework.ScaledNumber)

                If Value.ActualValue <= m_snCollectDataInterval.ActualValue Then
                    Throw New System.Exception("The collect time window must be greater than the collect interval.")
                End If

                m_snCollectTimeWindow.CopyData(Value)

                If Not Util.Application.SimulationView Is Nothing AndAlso Not Util.Application.SimulationView.Simulator Is Nothing _
                   AndAlso Util.Application.SimulationView.Simulator.Loaded AndAlso Util.Application.SimulationView.Simulator.Paused Then
                    If Util.Application.SimulationView.Simulator.FindDataChart(m_strID, False) Then
                        Util.Application.SimulationView.Simulator.ModifyDataChart(m_strID, CSng(m_snCollectTimeWindow.ValueFromDefaultScale), CSng(m_snCollectDataInterval.ValueFromDefaultScale))
                    End If
                End If
            End Set
        End Property

        Public Overridable Property UpdateDataInterval() As AnimatTools.Framework.ScaledNumber
            Get
                Return m_snUpdateDataInterval
            End Get
            Set(ByVal Value As AnimatTools.Framework.ScaledNumber)

                If Value.ActualValue <= 0 Then
                    Throw New System.Exception("The update data interval must be greater than zero.")
                End If

                If Value.ActualValue <= m_snCollectDataInterval.ActualValue Then
                    Throw New System.Exception("The update data interval must be greater than the collect data interval.")
                End If

                m_snUpdateDataInterval.CopyData(Value)
            End Set
        End Property

        Public Overridable Property SetStartEndTime() As Boolean
            Get
                Return m_bSetStartEndTime
            End Get
            Set(ByVal Value As Boolean)
                m_bSetStartEndTime = Value
                Me.Viewer.RefreshProperties()
            End Set
        End Property

        Public Overridable Property CollectStartTime() As AnimatTools.Framework.ScaledNumber
            Get
                Return m_snCollectStartTime
            End Get
            Set(ByVal Value As AnimatTools.Framework.ScaledNumber)

                If Value.ActualValue < 0 Then
                    Throw New System.Exception("The start time must be greater than zero.")
                End If

                If Value.ActualValue >= m_snCollectEndTime.ActualValue Then
                    Throw New System.Exception("The start time must not be greater than the end time.")
                End If

                m_snCollectStartTime.CopyData(Value)
            End Set
        End Property

        Public Overridable Property CollectEndTime() As AnimatTools.Framework.ScaledNumber
            Get
                Return m_snCollectEndTime
            End Get
            Set(ByVal Value As AnimatTools.Framework.ScaledNumber)

                If Value.ActualValue <= 0 Then
                    Throw New System.Exception("The end time must be greater than zero.")
                End If

                If Value.ActualValue <= m_snCollectStartTime.ActualValue Then
                    Throw New System.Exception("The end time must not be less than the start time.")
                End If

                m_snCollectEndTime.CopyData(Value)
            End Set
        End Property

        Public Overridable ReadOnly Property AxisList() As Collections.SortedAxisList
            Get
                Return m_aryAxisList
            End Get
        End Property

        Public Overridable ReadOnly Property DataColumnCount() As Integer
            Get
                Dim iCount As Integer = 0
                Dim doAxis As DataObjects.Charting.Axis
                For Each deEntry As DictionaryEntry In m_aryAxisList
                    doAxis = DirectCast(deEntry.Value, DataObjects.Charting.Axis)
                    iCount = iCount + doAxis.DataColumns.Count
                Next

                Return iCount
            End Get
        End Property

        Public Overridable ReadOnly Property AutoFillColors() As System.Drawing.Color()
            Get
                Return m_aryAutoFillColors
            End Get
        End Property

        Public Overridable ReadOnly Property NextAutoFillColor() As System.Drawing.Color
            Get
                Return m_aryAutoFillColors(m_iAutoFillColor)
            End Get
        End Property

        Public Overridable ReadOnly Property RequiresAutoDataCollectInterval() As Boolean
            Get
                Dim doAxis As DataObjects.Charting.Axis
                For Each deEntry As DictionaryEntry In m_aryAxisList
                    doAxis = DirectCast(deEntry.Value, DataObjects.Charting.Axis)
                    If doAxis.RequiresAutoDataCollectInterval Then
                        Return True
                    End If
                Next
            End Get
        End Property

        Public MustOverride Property MainTitle() As String
        Public MustOverride Property SubTitle() As String
        Public MustOverride Property XAxisLabel() As String
        Public MustOverride Property YAxisLabel() As String
        Public MustOverride Property AutoScaleData() As Boolean
        Public MustOverride Property XAxisSize() As PointF
        Public MustOverride Property YAxisSize() As PointF
        Public MustOverride ReadOnly Property Chart() As Object

#End Region

#Region " Methods "

        Public Overrides Sub Initialize(ByRef frmApplication As AnimatApplication, _
                                        Optional ByVal frmMdiParent As MdiChild = Nothing, _
                                        Optional ByVal frmParent As AnimatForm = Nothing)
            MyBase.Initialize(frmApplication, frmMdiParent, frmParent)

            m_snCollectDataInterval = New AnimatTools.Framework.ScaledNumber(Me.FormHelper, "CollectDataInterval", 5, AnimatTools.Framework.ScaledNumber.enumNumericScale.milli, "seconds", "s")
            m_snCollectTimeWindow = New AnimatTools.Framework.ScaledNumber(Me.FormHelper, "CollectTimeWindow", 3, AnimatTools.Framework.ScaledNumber.enumNumericScale.None, "seconds", "s")
            m_snUpdateDataInterval = New AnimatTools.Framework.ScaledNumber(Me.FormHelper, "UpdateDataInterval", 200, AnimatTools.Framework.ScaledNumber.enumNumericScale.milli, "seconds", "s")

            m_snCollectStartTime = New AnimatTools.Framework.ScaledNumber(Me.FormHelper, "CollectStartTime", 0, AnimatTools.Framework.ScaledNumber.enumNumericScale.None, "seconds", "s")
            m_snCollectEndTime = New AnimatTools.Framework.ScaledNumber(Me.FormHelper, "CollectEndTime", 10, AnimatTools.Framework.ScaledNumber.enumNumericScale.None, "seconds", "s")

            m_aryAutoFillColors(0) = System.Drawing.Color.Red
            m_aryAutoFillColors(1) = System.Drawing.Color.Lime
            m_aryAutoFillColors(2) = System.Drawing.Color.Blue
            m_aryAutoFillColors(3) = System.Drawing.Color.Yellow
            m_aryAutoFillColors(4) = System.Drawing.Color.Orange
            m_aryAutoFillColors(5) = System.Drawing.Color.Cyan
            m_aryAutoFillColors(6) = System.Drawing.Color.Magenta
            m_aryAutoFillColors(7) = System.Drawing.Color.Gold
            m_aryAutoFillColors(8) = System.Drawing.Color.Turquoise
            m_aryAutoFillColors(9) = System.Drawing.Color.Red
            m_aryAutoFillColors(10) = System.Drawing.Color.LightSteelBlue

            m_aryAutoFillColors(11) = System.Drawing.Color.Tomato
            m_aryAutoFillColors(12) = System.Drawing.Color.DarkViolet
            m_aryAutoFillColors(13) = System.Drawing.Color.Pink
            m_aryAutoFillColors(14) = System.Drawing.Color.SeaGreen
            m_aryAutoFillColors(15) = System.Drawing.Color.DeepPink
            m_aryAutoFillColors(16) = System.Drawing.Color.Crimson
            m_aryAutoFillColors(17) = System.Drawing.Color.Violet
            m_aryAutoFillColors(18) = System.Drawing.Color.SpringGreen
            m_aryAutoFillColors(19) = System.Drawing.Color.Navy
            m_aryAutoFillColors(20) = System.Drawing.Color.OliveDrab

        End Sub

        Protected Overrides Sub BuildProperties()

            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("MainTitle", GetType(String), "MainTitle", _
                                        "Gain Limits", "Sets the main title of this graph.", MainTitle))

        End Sub

        Public Overridable Sub DrawGainChart()
        End Sub

        Public Overridable Sub IncrementAutoFillColor()
            m_iAutoFillColor = m_iAutoFillColor + 1
            If m_iAutoFillColor > 20 Then m_iAutoFillColor = 0
        End Sub

#Region " TreeView Methods "

        Public Overrides Sub CreateTreeView()
            MyBase.CreateTreeView()

            If Not m_frmViewer Is Nothing AndAlso Not m_frmViewer.HierarchyBar Is Nothing Then

                Dim doAxis As DataObjects.Charting.Axis
                For Each deEntry As DictionaryEntry In m_aryAxisList
                    doAxis = DirectCast(deEntry.Value, DataObjects.Charting.Axis)
                    doAxis.CreateTreeView()
                Next

                m_frmViewer.HierarchyBar.ctrlTreeView.ExpandAll()
            End If

        End Sub

        Public Overrides Function TreeviewPopupMenu(ByRef tnSelectedNode As TreeNode, ByVal ptPoint As Point) As Boolean

            If tnSelectedNode Is m_tnTreeNode Then
                ' Create the menu items
                Dim mcDelete As New MenuCommand("Delete Chart", "DeleteChart", m_frmApplication.SmallImages.ImageList, _
                                                  m_frmApplication.SmallImages.GetImageIndex("AnimatTools.Delete.gif"), _
                                                  New EventHandler(AddressOf Me.OnDeleteChart))
                Dim mcAddAxis As New MenuCommand("Add Axis", "AddAxis", m_frmApplication.SmallImages.ImageList, _
                                                  m_frmApplication.SmallImages.GetImageIndex("AnimatTools.AddChartAxis.gif"), _
                                                  New EventHandler(AddressOf Me.OnAddAxis))
                Dim mcClearCharts As New MenuCommand("Clear Charts", "ClearCharts", _
                                                  New EventHandler(AddressOf Me.OnClearCharts))

                Dim mcSepExpand As MenuCommand = New MenuCommand("-")
                Dim mcExpandAll As New MenuCommand("Expand All", tnSelectedNode, _
                                                  New EventHandler(AddressOf Me.OnExpandAll))
                Dim mcCollapseAll As New MenuCommand("Collapse All", tnSelectedNode, _
                                                  New EventHandler(AddressOf Me.OnCollapseAll))

                mcExpandAll.ImageList = m_frmApplication.SmallImages.ImageList
                mcExpandAll.ImageIndex = m_frmApplication.SmallImages.GetImageIndex("AnimatTools.Expand.gif")
                mcCollapseAll.ImageList = m_frmApplication.SmallImages.ImageList
                mcCollapseAll.ImageIndex = m_frmApplication.SmallImages.GetImageIndex("AnimatTools.Collapse.gif")

                ' Create the popup menu object
                Dim popup As New PopupMenu
                popup.MenuCommands.Add(mcDelete)
                If m_aryAxisList.Count < 6 Then
                    popup.MenuCommands.Add(mcAddAxis)
                End If
                popup.MenuCommands.Add(mcClearCharts)
                popup.MenuCommands.AddRange(New MenuCommand() {mcSepExpand, mcExpandAll, mcCollapseAll})

                ' Show it!
                Dim selected As MenuCommand = popup.TrackPopup(ptPoint)

                Return True
            Else
                Dim doAxis As DataObjects.Charting.Axis
                For Each deEntry As DictionaryEntry In m_aryAxisList
                    doAxis = DirectCast(deEntry.Value, DataObjects.Charting.Axis)
                    If doAxis.TreeviewPopupMenu(tnSelectedNode, ptPoint) Then
                        Return True
                    End If
                Next
            End If

            Return False
        End Function

#End Region

        Protected Overrides Sub DroppedDragData(ByVal doDrag As Framework.DataDragHelper)

            'If there is not y axis then add on.
            If m_aryAxisList.Count = 0 Then
                Dim aryAxis As New AnimatTools.DataObjects.Charting.Axis(Me)
                aryAxis.Name = "Y Axis 1"
                'Setting the name adds it to the list
            End If

            'Lets find the first axis object.
            Dim doAxis As AnimatTools.DataObjects.Charting.Axis = DirectCast(m_aryAxisList.GetItem(0), AnimatTools.DataObjects.Charting.Axis)
            doAxis.DroppedDragData(doDrag)
        End Sub

        Protected Overridable Function GetSimulatorView() As AnimatTools.Forms.SimulatorView

            If Util.Application.SimulationWindow Is Nothing OrElse Util.Application.SimulationWindow.Control Is Nothing Then
                Throw New System.Exception("Unable to get a pointer to the simulator interface")
            End If

            Dim frmSim As AnimatTools.Forms.SimulatorView = DirectCast(Util.Application.SimulationWindow.Control, AnimatTools.Forms.SimulatorView)

            Return frmSim
        End Function

        Protected Overridable Function FindAvailableWorkingAxis() As Integer

            Dim aryAxis(5) As Boolean

            Dim doAxis As AnimatTools.DataObjects.Charting.Axis
            For Each deEntry As DictionaryEntry In m_aryAxisList
                doAxis = DirectCast(deEntry.Value, AnimatTools.DataObjects.Charting.Axis)
                If doAxis.WorkingAxis >= 0 AndAlso doAxis.WorkingAxis <= 5 Then
                    aryAxis(doAxis.WorkingAxis) = True
                End If
            Next

            For iAxis As Integer = 0 To 5
                If Not aryAxis(iAxis) Then
                    Return iAxis
                End If
            Next

            Throw New System.Exception("No available working axis was found.")

        End Function

        Public Overridable Function FindWorkingAxis(ByVal iAxis As Integer) As AnimatTools.DataObjects.Charting.Axis

            Dim doAxis As AnimatTools.DataObjects.Charting.Axis
            For Each deEntry As DictionaryEntry In m_aryAxisList
                doAxis = DirectCast(deEntry.Value, AnimatTools.DataObjects.Charting.Axis)
                If doAxis.WorkingAxis = iAxis Then
                    Return doAxis
                End If
            Next

            Return Nothing
        End Function

        Protected Overridable Sub RemoveUnusedAxis()

            Dim aryRemove As New Collection
            Dim doAxis As AnimatTools.DataObjects.Charting.Axis
            For Each deEntry As DictionaryEntry In m_aryAxisList
                doAxis = DirectCast(deEntry.Value, AnimatTools.DataObjects.Charting.Axis)

                If doAxis.DataColumns.Count = 0 Then
                    aryRemove.Add(doAxis)
                End If
            Next

            For Each doAxis In aryRemove
                doAxis.DeleteAxis(False)
            Next
        End Sub

        Public Overridable Sub ResetCollectDataInterval()
            Dim dblVal As Double = CalculateCollectDataInterval()
            Dim snVal As New ScaledNumber(Me.m_doFormHelper)
            snVal.SetFromValue(dblVal)
            Me.CollectDataInterval = snVal
        End Sub

        Protected Overridable Function CalculateCollectDataInterval() As Double

            Dim dblTimeStep As Double = Util.Environment.PhysicsTimeStep.Value
            Dim dblTemp As Double

            Dim doAxis As AnimatTools.DataObjects.Charting.Axis
            For Each deEntry As DictionaryEntry In m_aryAxisList
                doAxis = DirectCast(deEntry.Value, AnimatTools.DataObjects.Charting.Axis)

                dblTemp = doAxis.TimeStep()
                If dblTemp < dblTimeStep Then
                    dblTimeStep = dblTemp
                End If
            Next

            Return dblTimeStep
        End Function

        Public Overridable Sub UpdateChartConfiguration(ByVal bReconfigureData As Boolean)

        End Sub

        Public Overrides Sub LoadData(ByRef oXml As AnimatTools.Interfaces.StdXml)

            oXml.IntoElem() 'Into Form Element

            m_strID = Util.LoadID(oXml, "")
            m_bAutoCollectDataInterval = oXml.GetChildBool("AutoCollectInterval", False)
            m_snCollectDataInterval.LoadData(oXml, "CollectDataInterval")
            m_snCollectTimeWindow.LoadData(oXml, "CollectTimeWindow")

            If ScaledNumber.IsValidXml(oXml, "UpdateDataInterval") Then
                m_snUpdateDataInterval.LoadData(oXml, "UpdateDataInterval")
            Else
                m_snUpdateDataInterval.SetFromValue((oXml.GetChildFloat("UpdateDataInterval", 200) * 0.001), ScaledNumber.enumNumericScale.milli) 'It is in milliseconds
            End If

            m_bSetStartEndTime = oXml.GetChildBool("SetStartEndTime", False)
            If oXml.FindChildElement("CollectStartTime", False) Then
                m_snCollectStartTime.LoadData(oXml, "CollectStartTime")
                m_snCollectEndTime.LoadData(oXml, "CollectEndTime")
            End If

            'Load the axis information
            m_aryAxisList.Clear()
            If oXml.FindChildElement("AxisList", False) Then
                oXml.IntoChildElement("AxisList")

                Dim strAssemblyFile As String = ""
                Dim strClassName As String = ""

                Dim doAxis As DataObjects.Charting.Axis
                Dim iCount As Integer = oXml.NumberOfChildren() - 1
                For iIndex As Integer = 0 To iCount
                    oXml.FindChildByIndex(iIndex)

                    oXml.IntoElem()
                    strAssemblyFile = oXml.GetChildString("AssemblyFile")
                    strClassName = oXml.GetChildString("ClassName")
                    oXml.OutOfElem()

                    doAxis = DirectCast(Util.LoadClass(strAssemblyFile, strClassName, Me.FormHelper), AnimatTools.DataObjects.Charting.Axis)
                    doAxis.ParentChart = Me
                    doAxis.LoadData(oXml)
                    m_aryAxisList.Add(doAxis.Name, doAxis)
                Next
                oXml.OutOfElem()  'Outof InLinks Element
            End If

            oXml.OutOfElem() 'Outof Form Element

        End Sub

        Public Overrides Sub SaveData(ByRef oXml As AnimatTools.Interfaces.StdXml)
            MyBase.SaveData(oXml)

            oXml.IntoElem()

            oXml.AddChildElement("ID", m_strID)
            oXml.AddChildElement("AutoCollectInterval", m_bAutoCollectDataInterval)
            m_snCollectDataInterval.SaveData(oXml, "CollectDataInterval")
            m_snCollectTimeWindow.SaveData(oXml, "CollectTimeWindow")
            m_snUpdateDataInterval.SaveData(oXml, "UpdateDataInterval")

            oXml.AddChildElement("SetStartEndTime", m_bSetStartEndTime)
            m_snCollectStartTime.SaveData(oXml, "CollectStartTime")
            m_snCollectEndTime.SaveData(oXml, "CollectEndTime")

            'Save axis list
            oXml.AddChildElement("AxisList")
            oXml.IntoElem()

            Dim doAxis As DataObjects.Charting.Axis
            For Each deEntry As DictionaryEntry In m_aryAxisList
                doAxis = DirectCast(deEntry.Value, DataObjects.Charting.Axis)
                doAxis.SaveData(oXml)
            Next
            oXml.OutOfElem()

            oXml.OutOfElem()

        End Sub

#End Region

#Region " Events "

        Protected Overridable Sub OnDeleteChart(ByVal sender As Object, ByVal e As System.EventArgs)

            Try
                If MessageBox.Show("Are you certain that you want to permanently delete this " & _
                                    "chart?", "Delete Chart", MessageBoxButtons.YesNo) = DialogResult.Yes Then
                End If

            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try

        End Sub

        Protected Overridable Sub OnAddAxis(ByVal sender As Object, ByVal e As System.EventArgs)

            Try
                Dim iIndex As Integer = Util.ExtractIDCount("Y Axis", m_aryAxisList, " ") + 1
                Dim strName As String = "Y Axis " & iIndex

                Dim doAxis As New AnimatTools.DataObjects.Charting.Axis(Me)
                doAxis.Name = strName
                doAxis.CreateTreeView()
                'Setting the name on the axis adds it to the axis list

            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try

        End Sub

        Protected Overridable Sub OnClearCharts(ByVal sender As Object, ByVal e As System.EventArgs)
        End Sub

#End Region

    End Class

End Namespace
