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

Namespace DataObjects.Charting

    Public Class Axis
        Inherits AnimatTools.DataObjects.DragObject

#Region " Attributes "

        Protected m_frmParentChart As Forms.Tools.DataChart
        Protected m_iWorkingAxis As Integer = -1
        Protected m_tnTreeNode As TreeNode
        Protected m_aryDataColumns As New AnimatTools.Collections.SortedDataColumns(Me)

#End Region

#Region " Properties "

        Public Overrides Property Name() As String
            Get
                Return m_strName
            End Get
            Set(ByVal Value As String)
                If Value.Trim.Length = 0 Then
                    Throw New System.Exception("The axis name must not be blank.")
                End If

                'Check to see if it is the same as an existing name.
                If m_frmParentChart.AxisList.Contains(Value.Trim) Then
                    Throw New System.Exception("There is already an axis with the name '" & Value & "'")
                End If

                If m_frmParentChart.AxisList.Contains(m_strName) Then
                    m_frmParentChart.AxisList.Remove(m_strName)
                End If

                m_strName = Value

                m_frmParentChart.AxisList.Add(m_strName, Me)

                If Not m_tnTreeNode Is Nothing Then
                    m_tnTreeNode.Remove()
                End If
                Me.CreateTreeView()

            End Set
        End Property

        Public Overridable Property WorkingAxis() As Integer
            Get
                Return m_iWorkingAxis
            End Get
            Set(ByVal Value As Integer)
                If Value < 0 OrElse Value > 5 OrElse ((Me.ParentChart.AxisList.Count > 0) AndAlso (Value > (Me.ParentChart.AxisList.Count - 1))) Then
                    Throw New System.Exception("The working axis value must be between 0 and " & (Me.ParentChart.AxisList.Count - 1) & ".")
                End If

                'Lets find the working axis that is currently using this value.
                Dim doSwitchAxis As AnimatTools.DataObjects.Charting.Axis = Me.ParentChart.FindWorkingAxis(Value)

                If Not doSwitchAxis Is Nothing Then
                    doSwitchAxis.WorkingAxisInternal = m_iWorkingAxis
                End If

                m_iWorkingAxis = Value

                Me.ParentChart.UpdateChartConfiguration(True)
            End Set
        End Property

        Public Overridable Property WorkingAxisInternal() As Integer
            Get
                Return m_iWorkingAxis
            End Get
            Set(ByVal Value As Integer)
                m_iWorkingAxis = Value
            End Set
        End Property

        Public Overridable Property TreeNode() As TreeNode
            Get
                Return m_tnTreeNode
            End Get
            Set(ByVal Value As TreeNode)
                m_tnTreeNode = Value
            End Set
        End Property

        Public Overridable ReadOnly Property DataColumns() As AnimatTools.Collections.SortedDataColumns
            Get
                Return m_aryDataColumns
            End Get
        End Property

        Public Overridable Property ParentChart() As Forms.Tools.DataChart
            Get
                Return m_frmParentChart
            End Get
            Set(ByVal Value As Forms.Tools.DataChart)
                m_frmParentChart = Value
            End Set
        End Property

        <Browsable(False)> _
        Public Overrides ReadOnly Property RootForm() As System.Windows.Forms.Form
            Get
                If Not m_frmParentChart Is Nothing AndAlso Not m_frmParentChart.Viewer Is Nothing Then
                    Return m_frmParentChart.Viewer
                Else
                    Return Nothing
                End If
            End Get
        End Property

        Public Overrides ReadOnly Property ImageName() As String
            Get
                Return "AnimatTools.Y.gif"
            End Get
        End Property

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

            End Set
        End Property

        Public Overrides ReadOnly Property TimeStep() As Double
            Get
                Dim dblTimeStep As Double = -1
                Dim dblTemp As Double

                Dim doColumn As DataObjects.Charting.DataColumn
                For Each deEntry As DictionaryEntry In m_aryDataColumns
                    doColumn = DirectCast(deEntry.Value, DataObjects.Charting.DataColumn)

                    If Not doColumn.DataItem Is Nothing Then
                        dblTemp = doColumn.DataItem.TimeStep

                        If dblTimeStep < 0 OrElse dblTemp < dblTimeStep Then
                            dblTimeStep = dblTemp
                        End If
                    End If
                Next

                If dblTimeStep < 0 Then
                    Return Util.Environment.PhysicsTimeStep.ActualValue
                Else
                    Return dblTimeStep
                End If
            End Get
        End Property

        Public Overrides ReadOnly Property RequiresAutoDataCollectInterval() As Boolean
            Get
                Dim doColumn As DataObjects.Charting.DataColumn
                For Each deEntry As DictionaryEntry In m_aryDataColumns
                    doColumn = DirectCast(deEntry.Value, DataObjects.Charting.DataColumn)
                    If doColumn.RequiresAutoDataCollectInterval Then
                        Return True
                    End If
                Next
            End Get
        End Property

#End Region

#Region " Methods "

        Public Sub New(ByVal doParent As AnimatTools.Framework.DataObject)
            MyBase.New(doParent)
        End Sub

        Public Sub New(ByVal frmChart As AnimatTools.Forms.Tools.ToolForm)
            MyBase.New(frmChart.FormHelper)

            If TypeOf frmChart Is Forms.Tools.DataChart Then
                m_frmParentChart = DirectCast(frmChart, Forms.Tools.DataChart)
            End If
        End Sub

        Public Overridable Sub InitializeChartData()
        End Sub

        Public Overridable Sub UpdateChartConfiguration(ByRef iSubSet As Integer)
        End Sub

        Public Overridable Sub ReconfigureChartData(ByRef aryOldX(,) As Single, ByRef aryOldY(,) As Single, ByRef aryNewX(,) As Single, ByRef aryNewY(,) As Single)
        End Sub

        Public Overridable Sub RemoveInvalidDataColumns()

            Dim doColumn As AnimatTools.DataObjects.Charting.DataColumn
            Dim aryDelete As New Collection
            For Each deEntry As DictionaryEntry In m_aryDataColumns
                doColumn = DirectCast(deEntry.Value, AnimatTools.DataObjects.Charting.DataColumn)

                If Not doColumn.IsValidColumn Then
                    aryDelete.Add(doColumn)
                End If
            Next

            For Each doColumn In aryDelete
                doColumn.RemoveFromAxis()
            Next
        End Sub

        Public Overridable Sub PrepareForCharting()

            'First lets go through and remove any invalid data columns. This is something that
            'could happen if the referneced part in that data was deleted.
            RemoveInvalidDataColumns()

            Dim doColumn As AnimatTools.DataObjects.Charting.DataColumn
            For Each deEntry As DictionaryEntry In m_aryDataColumns
                doColumn = DirectCast(deEntry.Value, AnimatTools.DataObjects.Charting.DataColumn)

                If doColumn.IsValidColumn Then
                    doColumn.PrepareForCharting()
                End If
            Next

        End Sub

        Public Overridable Sub AddColumnToSimulation(ByVal doColumn As DataObjects.Charting.DataColumn, ByVal bPreparingChart As Boolean)

            If Not Util.Application.SimulationView Is Nothing AndAlso Not Util.Application.SimulationView.Simulator Is Nothing _
               AndAlso Util.Application.SimulationView.Simulator.Loaded AndAlso Util.Application.SimulationView.Simulator.Paused Then

                If Util.Application.SimulationView.Simulator.FindDataChart(Me.ParentChart.ID, False) Then
                    If Not Util.Application.SimulationView.Simulator.FindDataColumn(Me.ParentChart.ID, doColumn.ID, False) Then
                        Util.DisableDirtyFlags = True
                        Dim strXml As String = doColumn.SaveDataColumnToXml()
                        Util.Application.SimulationView.Simulator.AddDataColumn(Me.ParentChart.ID, doColumn.ID, doColumn.ColumnModuleName, doColumn.ColumnClassType, strXml)

                        If Not bPreparingChart Then
                            Me.ParentChart.UpdateChartConfiguration(True)
                        End If

                        Util.DisableDirtyFlags = False
                    End If
                End If
            End If

        End Sub

        Public Overridable Sub RemoveColumnFromSimulation(ByVal doColumn As DataObjects.Charting.DataColumn)

            If Not Util.Application.SimulationView Is Nothing AndAlso Not Util.Application.SimulationView.Simulator Is Nothing _
               AndAlso Util.Application.SimulationView.Simulator.Loaded AndAlso Util.Application.SimulationView.Simulator.Paused Then

                If Util.Application.SimulationView.Simulator.FindDataColumn(Me.ParentChart.ID, doColumn.ID, False) Then
                    Util.Application.SimulationView.Simulator.RemoveDataColumn(Me.ParentChart.ID, doColumn.ID)
                    Me.ParentChart.UpdateChartConfiguration(True)
                End If
            End If

            'If Me.DataColumns.Count = 0 Then
            '    DeleteAxis()
            'End If

        End Sub

        Public Overridable Sub DeleteAxis(Optional ByVal bUpdateChart As Boolean = True)
            If Not m_frmParentChart Is Nothing AndAlso m_frmParentChart.AxisList.Contains(Me.Name) Then
                m_frmParentChart.AxisList.Remove(Me.Name)

                'Now lets go through and decrement the working axis value for all remaining axises greater than this one.
                Dim doAxis As Axis
                For Each deEntry As DictionaryEntry In m_frmParentChart.AxisList
                    doAxis = DirectCast(deEntry.Value, Axis)

                    If doAxis.WorkingAxis > Me.WorkingAxis Then
                        doAxis.WorkingAxisInternal = doAxis.WorkingAxis - 1
                    End If
                Next

                'Remove all data columns if they exist.
                If Not Util.Application.SimulationView Is Nothing AndAlso Not Util.Application.SimulationView.Simulator Is Nothing _
                   AndAlso Util.Application.SimulationView.Simulator.Loaded AndAlso Util.Application.SimulationView.Simulator.Paused Then

                    Dim doColumn As AnimatTools.DataObjects.Charting.DataColumn
                    For Each deEntry As DictionaryEntry In m_aryDataColumns
                        doColumn = DirectCast(deEntry.Value, AnimatTools.DataObjects.Charting.DataColumn)
                        If Util.Application.SimulationView.Simulator.FindDataColumn(Me.ParentChart.ID, doColumn.ID, False) Then
                            Util.Application.SimulationView.Simulator.RemoveDataColumn(Me.ParentChart.ID, doColumn.ID)
                        End If
                    Next

                End If

                If Not m_tnTreeNode Is Nothing Then
                    m_tnTreeNode.Remove()
                End If

                If bUpdateChart Then
                    m_frmParentChart.UpdateChartConfiguration(True)
                End If
            End If
        End Sub

        Public Overridable Sub AddDataItem()
            OnAddDataItem(Me, New System.EventArgs)
        End Sub

#Region " TreeView Methods "

        Public Overridable Sub CreateTreeView()

            If Not m_frmParentChart Is Nothing AndAlso Not m_frmParentChart.Viewer Is Nothing AndAlso Not m_frmParentChart.Viewer.HierarchyBar Is Nothing _
               AndAlso Not m_frmParentChart.TreeNode Is Nothing Then

                m_frmParentChart.Viewer.HierarchyBar.ImageManager.AddImage(Me.ImageName, Me.Image)
                m_tnTreeNode = m_frmParentChart.TreeNode.Nodes.Add(Me.Name)
                m_tnTreeNode.ImageIndex = m_frmParentChart.Viewer.HierarchyBar.ImageManager.GetImageIndex(Me.ImageName)
                m_tnTreeNode.SelectedImageIndex = m_frmParentChart.Viewer.HierarchyBar.ImageManager.GetImageIndex(Me.ImageName)
                m_tnTreeNode.Tag = Me

                Dim doColumn As DataObjects.Charting.DataColumn
                For Each deEntry As DictionaryEntry In m_aryDataColumns
                    doColumn = DirectCast(deEntry.Value, DataObjects.Charting.DataColumn)
                    doColumn.CreateTreeView()
                Next

            End If

        End Sub

        Public Overridable Function TreeviewPopupMenu(ByRef tnSelectedNode As TreeNode, ByVal ptPoint As Point) As Boolean

            If tnSelectedNode Is m_tnTreeNode Then
                ' Create the menu items
                Dim mcDelete As New MenuCommand("Delete Axis", "DeleteAxis", Util.Application.SmallImages.ImageList, _
                                                  Util.Application.SmallImages.GetImageIndex("AnimatTools.Delete.gif"), _
                                                  New EventHandler(AddressOf Me.OnDeleteAxis))

                Dim mcAddItem As New MenuCommand("Add Item", "AddItem", Util.Application.SmallImages.ImageList, _
                                                  Util.Application.SmallImages.GetImageIndex("AnimatTools.AddChartItem.gif"), _
                                                  New EventHandler(AddressOf Me.OnAddDataItem))

                Dim mcSepExpand As MenuCommand = New MenuCommand("-")
                Dim mcExpandAll As New MenuCommand("Expand All", tnSelectedNode, _
                                                  New EventHandler(AddressOf Me.OnExpandAll))
                Dim mcCollapseAll As New MenuCommand("Collapse All", tnSelectedNode, _
                                                  New EventHandler(AddressOf Me.OnCollapseAll))

                mcExpandAll.ImageList = Util.Application.SmallImages.ImageList
                mcExpandAll.ImageIndex = Util.Application.SmallImages.GetImageIndex("AnimatTools.Expand.gif")
                mcCollapseAll.ImageList = Util.Application.SmallImages.ImageList
                mcCollapseAll.ImageIndex = Util.Application.SmallImages.GetImageIndex("AnimatTools.Collapse.gif")

                ' Create the popup menu object
                Dim popup As New PopupMenu
                popup.MenuCommands.Add(mcDelete)
                popup.MenuCommands.Add(mcAddItem)
                popup.MenuCommands.AddRange(New MenuCommand() {mcSepExpand, mcExpandAll, mcCollapseAll})

                ' Show it!
                Dim selected As MenuCommand = popup.TrackPopup(ptPoint)

                Return True
            Else
                Dim doColumn As DataObjects.Charting.DataColumn
                For Each deEntry As DictionaryEntry In m_aryDataColumns
                    doColumn = DirectCast(deEntry.Value, DataObjects.Charting.DataColumn)
                    If doColumn.TreeviewPopupMenu(tnSelectedNode, ptPoint) Then
                        Return True
                    End If
                Next
            End If

            Return False
        End Function

#End Region

#Region " DragDrop Methods "

        Public Overridable Function DroppedDragData(ByVal doDrag As Framework.DataDragHelper) As Boolean
            Dim bVal As Boolean = True

            If TypeOf doDrag.m_doData Is DataObjects.DragObject Then
                Dim doItem As DataObjects.DragObject = DirectCast(doDrag.m_doData, DataObjects.DragObject)
                If doItem.CanBeCharted Then
                    DroppedItem(doItem)
                End If
            ElseIf TypeOf doDrag.m_doData Is AnimatTools.DataObjects.Charting.DataColumn Then
                Dim doColumn As AnimatTools.DataObjects.Charting.DataColumn = DirectCast(doDrag.m_doData, AnimatTools.DataObjects.Charting.DataColumn)
                MovedDataColumn(doColumn)
            Else
                bVal = False
            End If

            'Cause the entire tree view to reapaint itself because if not it leaves weird stuff on the screen.
            m_frmParentChart.Viewer.HierarchyBar.ctrlTreeView.Invalidate()

            If bVal Then
                m_frmParentChart.UpdateChartConfiguration(True)
            End If

            Return bVal
        End Function

        Public Overridable Function CreateDataColumn(ByVal doItem As AnimatTools.DataObjects.DragObject, Optional ByVal bAutoAddToAxis As Boolean = True) As AnimatTools.DataObjects.Charting.DataColumn
            Return Nothing
        End Function

        Protected Overridable Sub DroppedItem(ByVal doItem As DataObjects.DragObject)
        End Sub

        Protected Overridable Sub DroppedItem(ByVal doColumn As DataObjects.Charting.DataColumn)
        End Sub

        Public Overridable Sub MovedDataColumn(ByVal doColumn As DataObjects.Charting.DataColumn)
            If Not doColumn.ParentAxis Is Me Then
                doColumn.RemoveFromAxis()
                doColumn.AddtoAxis(Me)
            End If
        End Sub

#End Region

#Region " DataObject Methods "

        Protected Overrides Sub BuildProperties()

            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Name", m_strName.GetType(), "Name", _
                                        "Axis Properties", "The name of this axis.", m_strName))

            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Chart Axis", m_iWorkingAxis.GetType(), "WorkingAxis", _
                                        "Axis Properties", "Determines which axis on the chart that this one represents.", m_iWorkingAxis))

        End Sub

        Public Overrides Function EnsureParentMdiShown() As AnimatTools.Forms.MdiChild

            If Not Me.ParentChart Is Nothing AndAlso Me.ParentChart.Viewer Is Nothing AndAlso Not Me.ParentChart.ToolHolder Is Nothing Then
                Return Util.Application.DisplayToolViewer(Me.ParentChart.ToolHolder)
            End If

        End Function

        Public Overrides Sub LoadData(ByRef oXml As AnimatTools.Interfaces.StdXml)

            oXml.IntoElem() 'Into Axis Element

            m_strName = oXml.GetChildString("Name")
            m_iWorkingAxis = oXml.GetChildInt("WorkingAxis")

            If oXml.FindChildElement("DataColumns", False) Then
                oXml.IntoChildElement("DataColumns")
                Dim strAssemblyFile As String
                Dim strClassname As String
                Dim doColumn As AnimatTools.DataObjects.Charting.DataColumn
                Dim iCount As Integer = oXml.NumberOfChildren() - 1
                For iIndex As Integer = 0 To iCount
                    oXml.FindChildByIndex(iIndex)

                    oXml.IntoElem()
                    strAssemblyFile = oXml.GetChildString("AssemblyFile")
                    strClassname = oXml.GetChildString("ClassName")
                    oXml.OutOfElem()

                    doColumn = DirectCast(Util.LoadClass(strAssemblyFile, strClassname, Me), AnimatTools.DataObjects.Charting.DataColumn)
                    doColumn.LoadData(oXml)

                    If doColumn.IsValidColumn Then
                        m_aryDataColumns.Add(doColumn.ID, doColumn)
                    End If
                Next
                oXml.OutOfElem()  'Outof InLinks Element
            End If

            oXml.OutOfElem() 'Outof Form Element
        End Sub


        Public Overrides Sub SaveData(ByRef oXml As AnimatTools.Interfaces.StdXml)

            oXml.AddChildElement("Axis")
            oXml.IntoElem()

            oXml.AddChildElement("AssemblyFile", Me.AssemblyFile)
            oXml.AddChildElement("ClassName", Me.ClassName)
            oXml.AddChildElement("Name", m_strName)
            oXml.AddChildElement("WorkingAxis", m_iWorkingAxis)

            oXml.AddChildElement("DataColumns")
            oXml.IntoElem()

            Dim aryRemove As New Collection
            Dim doColumn As AnimatTools.DataObjects.Charting.DataColumn
            For Each deEntry As DictionaryEntry In m_aryDataColumns
                doColumn = DirectCast(deEntry.Value, AnimatTools.DataObjects.Charting.DataColumn)
                If doColumn.IsValidColumn() Then
                    doColumn.SaveData(oXml)
                Else
                    'MessageBox.Show("About to remove a column that is showing as invalid. Column: " & doColumn.Name)
                    aryRemove.Add(doColumn)
                End If
            Next

            For Each doColumn In aryRemove
                m_aryDataColumns.Remove(doColumn.Name)
                If Not doColumn.TreeNode Is Nothing Then
                    doColumn.TreeNode.Remove()
                End If
            Next

            oXml.OutOfElem()

            oXml.OutOfElem()

        End Sub

        Protected Overrides Sub CloneInternal(ByVal doOriginal As AnimatTools.Framework.DataObject, ByVal bCutData As Boolean, _
                                            ByVal doRoot As AnimatTools.Framework.DataObject)
            MyBase.CloneInternal(doOriginal, bCutData, doRoot)

            Dim doOrigAxis As Axis = DirectCast(doOriginal, Axis)

            m_frmParentChart = doOrigAxis.m_frmParentChart
            m_strName = doOrigAxis.m_strName
            m_iWorkingAxis = doOrigAxis.m_iWorkingAxis
            m_tnTreeNode = doOrigAxis.m_tnTreeNode
            m_aryDataColumns = DirectCast(doOrigAxis.m_aryDataColumns.Clone(), Collections.SortedDataColumns)
            m_Image = DirectCast(doOrigAxis.m_Image.Clone, Image)
        End Sub

        Public Overrides Function Clone(ByVal doParent As AnimatTools.Framework.DataObject, ByVal bCutData As Boolean, _
                                        ByVal doRoot As AnimatTools.Framework.DataObject) As AnimatTools.Framework.DataObject
            Dim doAxis As New Axis(doParent)
            doAxis.CloneInternal(Me, bCutData, doRoot)
            If Not doRoot Is Nothing AndAlso doRoot Is Me Then doAxis.AfterClone(Me, bCutData, doRoot, doAxis)
            Return doAxis
        End Function

#End Region

#End Region

#Region " Events "

        Protected Overridable Sub OnDeleteAxis(ByVal sender As Object, ByVal e As System.EventArgs)

            Try
                If MessageBox.Show("Are you certain that you want to permanently delete this " & _
                                    "axis and all its data columns?", "Delete Axis", MessageBoxButtons.YesNo) = DialogResult.Yes Then
                    DeleteAxis()
                End If

            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try

        End Sub

        Protected Overridable Sub OnAddDataItem(ByVal sender As Object, ByVal e As System.EventArgs)

            'Try
            '    Util.DisableDirtyFlags = True

            '    Dim frmSelectItem As New Forms.Tools.SelectDataItem

            '    frmSelectItem.Axis = Me
            '    frmSelectItem.BuildTreeView()
            '    If frmSelectItem.ShowDialog(Me.ParentChart) = DialogResult.OK Then
            '        Util.DisableDirtyFlags = False
            '        Me.DroppedItem(frmSelectItem.DataColumn)
            '        m_frmParentChart.UpdateChartConfiguration(True)
            '    End If

            'Catch ex As System.Exception
            '    AnimatTools.Framework.Util.DisplayError(ex)
            'Finally
            '    Util.DisableDirtyFlags = False
            'End Try
        End Sub

#End Region

#Region " Drag Object Overrides "

        Public Overrides ReadOnly Property CanBeCharted() As Boolean
            Get
                Return False
            End Get
        End Property

        Public Overrides ReadOnly Property DataColumnClassType() As String
            Get
                Return ""
            End Get
        End Property

        Public Overrides ReadOnly Property DataColumnModuleName() As String
            Get
                Return ""
            End Get
        End Property

        Public Overrides Property DragImage() As System.Drawing.Image
            Get
                Return Me.Image
            End Get
            Set(ByVal Value As System.Drawing.Image)
            End Set
        End Property

        Public Overrides ReadOnly Property DragImageName() As String
            Get
                Return Me.ImageName
            End Get
        End Property

        Public Overrides Function FindDragObject(ByVal strStructureName As String, ByVal strDataItemID As String, Optional ByVal bThrowError As Boolean = True) As DragObject
            Return Nothing
        End Function

        Public Overrides Property ItemName() As String
            Get
                Return Me.Name
            End Get
            Set(ByVal Value As String)
                Me.Name = Value
            End Set
        End Property

        Public Overrides Sub SaveDataColumnToXml(ByRef oXml As Interfaces.StdXml)

        End Sub

        Public Overrides Sub SaveXml(ByRef oXml As Interfaces.StdXml)

            Dim doColumn As AnimatTools.DataObjects.Charting.DataColumn
            For Each deEntry As DictionaryEntry In m_aryDataColumns
                doColumn = DirectCast(deEntry.Value, AnimatTools.DataObjects.Charting.DataColumn)
                doColumn.SaveXml(oXml)
            Next

        End Sub

        Public Overrides ReadOnly Property StructureID() As String
            Get
                Return ""
            End Get
        End Property

#End Region

    End Class

End Namespace
