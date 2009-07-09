Imports System
Imports System.Drawing
Imports System.Collections
Imports System.ComponentModel
Imports System.Windows.Forms
Imports System.Diagnostics
Imports System.IO
Imports System.Xml
Imports Crownwood.Magic.Controls

Namespace Framework

    Public MustInherit Class DataObject

#Region " Enums "

#End Region

#Region " Attributes "

        Protected m_strName As String = ""
        Protected m_strID As String = System.Guid.NewGuid().ToString()
        Protected m_strCloneID As String = ""

        Protected m_doParent As Framework.DataObject
        Protected m_bFixedProperties As Boolean
        Protected m_Properties As PropertyTable
        Protected m_bIsDirty As Boolean = False
        Protected m_bEnabled As Boolean = True

        Protected m_bUndoRedoInProgress As Boolean = False
        Protected m_bSetValueInProgress As Boolean = False

#End Region

#Region " Properties "

        <Browsable(False)> _
        Public Overridable Property Name() As String
            Get
                Return m_strName
            End Get
            Set(ByVal Value As String)
                m_strName = Value
            End Set
        End Property

        <Browsable(False)> _
        Public Overridable Property ID() As String
            Get
                Return m_strID
            End Get
            Set(ByVal Value As String)
                m_strID = Value

                If Value.Trim.Length = 0 Then
                    Throw New System.Exception("The id property can not be blank.")
                End If
            End Set
        End Property

        <Browsable(False)> _
        Public Overridable ReadOnly Property CloneID() As String
            Get
                Return m_strCloneID
            End Get
        End Property

        <Browsable(False)> _
        Public Overridable Property Parent() As Framework.DataObject
            Get
                Return m_doParent
            End Get
            Set(ByVal Value As Framework.DataObject)
                m_doParent = Value
            End Set
        End Property

        <Browsable(False)> _
        Public Overridable Property Enabled() As Boolean
            Get
                Return m_bEnabled
            End Get
            Set(ByVal Value As Boolean)
                m_bEnabled = Value
            End Set
        End Property

        <Browsable(False)> _
        Public Overridable ReadOnly Property RootForm() As System.Windows.Forms.Form
            Get
                If Not m_doParent Is Nothing Then
                    Return m_doParent.RootForm()
                Else
                    Return Util.Application
                End If
            End Get
        End Property

        <Browsable(False)> _
        Public Overridable Property FixedProperties() As Boolean
            Get
                Return m_bFixedProperties
            End Get
            Set(ByVal Value As Boolean)
                m_bFixedProperties = Value
            End Set
        End Property

        'If this is true and it is a submember of another databoject then when you view the properties it will 
        'really be viewing the propertybag of this sub object.
        <Browsable(False)> _
        Public Overridable Property ViewSubProperties() As Boolean
            Get
                Return True
            End Get
            Set(ByVal Value As Boolean)
            End Set
        End Property

        <Browsable(False)> _
        Public Overridable ReadOnly Property Properties() As PropertyBag
            Get
                If m_Properties Is Nothing OrElse Not m_bFixedProperties Then
                    CreateProperties()
                End If

                Return m_Properties
            End Get
        End Property

        <Browsable(False)> _
        Public Overridable ReadOnly Property AssemblyFile() As String
            Get
                Dim strPath As String, strFile As String
                Util.SplitPathAndFile(Me.GetType.Assembly.Location, strPath, strFile)
                Return strFile
            End Get
        End Property

        <Browsable(False)> _
        Public Overridable ReadOnly Property AssemblyModuleName() As String
            Get
                Return Util.RootNamespace(Me.ClassName)
            End Get
        End Property

        <Browsable(False)> _
        Public Overridable ReadOnly Property ModuleName() As String
            Get
                Return ""
            End Get
        End Property

        <Browsable(False)> _
        Public Overridable ReadOnly Property ClassName() As String
            Get
                Return Me.GetType.ToString
            End Get
        End Property

        <Browsable(False)> _
        Public Overridable Property IsDirty() As Boolean
            Get
                Return m_bIsDirty
            End Get
            Set(ByVal Value As Boolean)
                If Not Util.DisableDirtyFlags Then
                    m_bIsDirty = Value

                    If m_bIsDirty AndAlso Not m_doParent Is Nothing Then
                        m_doParent.IsDirty = True
                    End If
                End If
            End Set
        End Property

        <Browsable(False)> _
        Public Overridable ReadOnly Property SetValueInProgress() As Boolean
            Get
                Return m_bSetValueInProgress
            End Get
        End Property

        <Browsable(False)> _
        Public Overridable Property UndoRedoInProgress() As Boolean
            Get
                Return m_bUndoRedoInProgress
            End Get
            Set(ByVal Value As Boolean)
                m_bUndoRedoInProgress = Value
            End Set
        End Property

#End Region

#Region " Methods "

        Public Sub New(ByVal doParent As Framework.DataObject)
            m_doParent = doParent
        End Sub

        Protected Overridable Sub CreateProperties()

            m_Properties = New PropertyTable
            m_Properties.Tag = Me

            AddHandler m_Properties.GetValue, AddressOf Me.Properties_GetValue
            AddHandler m_Properties.SetValue, AddressOf Me.Properties_SetValue

            BuildProperties()
        End Sub

        Protected MustOverride Sub BuildProperties()

        Public Overridable Sub BuildPropertyDropDown(ByRef ctrlDropDown As System.Windows.Forms.Control)
        End Sub

        Protected Overridable Sub FormatDropDownList(ByRef lbList As ListBox)

            lbList.IntegralHeight = True ' resize to avoid partial items
            If lbList.ItemHeight > 0 Then
                If lbList.Height / lbList.ItemHeight < lbList.Items.Count Then
                    ' try to keep the listbox small but sufficient
                    Dim adjHei As Integer = lbList.Items.Count * lbList.ItemHeight
                    If adjHei > 200 Then adjHei = 200
                    lbList.Height = adjHei
                End If
            Else ' safeguard, although it shouldn't happen
                lbList.Height = 200
            End If

            Dim iMaxLength As Integer = -1
            For Each deEntry As TypeHelpers.DropDownEntry In lbList.Items
                If deEntry.Display.Length > iMaxLength OrElse iMaxLength = -1 Then
                    iMaxLength = deEntry.Display.Length
                End If
            Next

            If iMaxLength > -1 Then
                lbList.Width = CInt(iMaxLength * lbList.Font.SizeInPoints)
                If lbList.Width > 400 Then
                    lbList.Width = 400
                End If
            End If

            lbList.Sorted = True ' present in alphabetical order

        End Sub

        Protected Overridable Sub FormatDropDownTree(ByRef tvTree As TreeView, ByVal iTotalNodes As Integer)

            tvTree.HideSelection = False
            If tvTree.ItemHeight > 0 Then
                If tvTree.Height / tvTree.ItemHeight < iTotalNodes Then
                    ' try to keep the listbox small but sufficient
                    Dim adjHei As Integer = (iTotalNodes + 2) * tvTree.ItemHeight
                    If adjHei > 500 Then adjHei = 500
                    tvTree.Height = adjHei
                End If
            Else ' safeguard, although it shouldn't happen
                tvTree.Height = 200
            End If

            tvTree.Sorted = True ' present in alphabetical order

        End Sub

        Protected Overridable Sub Properties_GetValue(ByVal sender As Object, ByVal e As PropertySpecEventArgs)
            Try
                Dim propInfo As System.Reflection.PropertyInfo = Me.GetType().GetProperty(e.Property.PropertyName)
                If Not propInfo Is Nothing Then
                    If propInfo.CanRead Then
                        e.Value = propInfo.GetValue(Me, Nothing)
                    Else
                        Throw New System.Exception("The property '" & propInfo.Name & "' is write only.")
                    End If

                    If TypeOf (e.Value) Is Framework.DataObject Then
                        Dim doData As Framework.DataObject = DirectCast(e.Value, Framework.DataObject)

                        If doData.ViewSubProperties Then
                            e.Value = doData.Properties
                        Else
                            e.Value = doData
                        End If
                    End If
                Else
                    If e.Property.PropertyName.Trim.Length = 0 Then
                        e.Value = Nothing
                    Else
                        Throw New System.Exception("No property info returned for the property name '" & e.Property.PropertyName & "'.")
                    End If
                End If

            Catch ex As System.Exception
                If Not ex.InnerException Is Nothing AndAlso ex.InnerException.Message.Trim.Length > 0 Then
                    AnimatTools.Framework.Util.DisplayError(ex.InnerException)
                Else
                    AnimatTools.Framework.Util.DisplayError(ex)
                End If
            End Try
        End Sub

        Protected Overridable Function GetOriginalValueForHistory(ByVal propInfo As System.Reflection.PropertyInfo) As Object
            Dim origValue As Object

            If propInfo.CanRead AndAlso Util.ModificationHistory.AllowAddHistory Then
                Dim tempValue As Object = propInfo.GetValue(Me, Nothing)

                If Not tempValue Is Nothing AndAlso TypeOf tempValue Is AnimatTools.Framework.DataObject Then
                    Dim doTemp As AnimatTools.Framework.DataObject = DirectCast(tempValue, AnimatTools.Framework.DataObject)
                    origValue = doTemp.Clone(doTemp.Parent, False, doTemp)
                ElseIf Not tempValue Is Nothing AndAlso TypeOf tempValue Is AnimatTools.Collections.AnimatSortedList Then
                    Dim doTemp As AnimatTools.Collections.AnimatSortedList = DirectCast(tempValue, AnimatTools.Collections.AnimatSortedList)
                    origValue = doTemp.Clone()
                ElseIf Not tempValue Is Nothing AndAlso TypeOf tempValue Is AnimatTools.Collections.AnimatCollectionBase Then
                    Dim doTemp As AnimatTools.Collections.AnimatCollectionBase = DirectCast(tempValue, AnimatTools.Collections.AnimatCollectionBase)
                    origValue = doTemp.Clone(doTemp.Parent, False, Nothing)
                ElseIf Not tempValue Is Nothing AndAlso TypeOf tempValue Is AnimatTools.Collections.AnimatDictionaryBase Then
                    Dim doTemp As AnimatTools.Collections.AnimatDictionaryBase = DirectCast(tempValue, AnimatTools.Collections.AnimatDictionaryBase)
                    origValue = doTemp.Clone(doTemp.Parent, False, Nothing)
                Else
                    origValue = tempValue
                End If
            End If

            Return origValue
        End Function

        Protected Overridable Sub SaveChangeHistory(ByVal e As PropertySpecEventArgs, ByVal propInfo As System.Reflection.PropertyInfo, ByVal origValue As Object)
            Dim newValue As Object

            If Not e.Value Is Nothing AndAlso TypeOf e.Value Is AnimatTools.Framework.DataObject Then
                Dim doTemp As AnimatTools.Framework.DataObject = DirectCast(e.Value, AnimatTools.Framework.DataObject)
                newValue = doTemp.Clone(doTemp.Parent, False, doTemp)
            ElseIf Not e.Value Is Nothing AndAlso TypeOf e.Value Is AnimatTools.Collections.AnimatSortedList Then
                Dim doTemp As AnimatTools.Collections.AnimatSortedList = DirectCast(e.Value, AnimatTools.Collections.AnimatSortedList)
                newValue = doTemp.Clone()
            ElseIf Not e.Value Is Nothing AndAlso TypeOf e.Value Is AnimatTools.Collections.AnimatCollectionBase Then
                Dim doTemp As AnimatTools.Collections.AnimatCollectionBase = DirectCast(e.Value, AnimatTools.Collections.AnimatCollectionBase)
                newValue = doTemp.Clone(doTemp.Parent, False, Nothing)
            ElseIf Not e.Value Is Nothing AndAlso TypeOf e.Value Is AnimatTools.Collections.AnimatDictionaryBase Then
                Dim doTemp As AnimatTools.Collections.AnimatDictionaryBase = DirectCast(e.Value, AnimatTools.Collections.AnimatDictionaryBase)
                newValue = doTemp.Clone(doTemp.Parent, False, Nothing)
            Else
                newValue = e.Value
            End If

            Dim frmRoot As System.Windows.Forms.Form = Me.RootForm
            Util.ModificationHistory.AddHistoryEvent(New UndoSystem.PropertyChangedEvent(frmRoot, Me, propInfo, origValue, newValue))
        End Sub

        Protected Overridable Sub Properties_SetValue(ByVal sender As Object, ByVal e As PropertySpecEventArgs)
            Try
                Dim propInfo As System.Reflection.PropertyInfo = Me.GetType().GetProperty(e.Property.PropertyName)

                If Not propInfo Is Nothing Then
                    If propInfo.CanWrite Then
                        Dim lModificationCount As Long = Util.ModificationHistory.ModificationCount
                        Dim origValue As Object = GetOriginalValueForHistory(propInfo)

                        m_bSetValueInProgress = True
                        propInfo.SetValue(Me, e.Value, Nothing)
                        m_bSetValueInProgress = False
                        Me.IsDirty = True

                        'Just in case, lets directly set the isdirty flag of the animatapplication. If for some reason the
                        'isdirty signal does not propogate up properly then this flag will still be set and things will 
                        'be saved correctly.
                        Util.Application.IsDirty = True

                        'Only add the history for this propchange if the property did not already add it itself.
                        If lModificationCount = Util.ModificationHistory.ModificationCount AndAlso Util.ModificationHistory.AllowAddHistory Then
                            SaveChangeHistory(e, propInfo, origValue)
                        End If
                    Else
                        Throw New System.Exception("The property '" & propInfo.Name & "' is read only.")
                    End If
                Else
                    Throw New System.Exception("No property info returned for the property name '" & e.Property.PropertyName & "'.")
                End If

            Catch ex As System.Exception
                If Not ex.InnerException Is Nothing AndAlso ex.InnerException.Message.Trim.Length > 0 Then
                    If TypeOf ex.InnerException Is System.Reflection.TargetInvocationException AndAlso Not ex.InnerException.InnerException Is Nothing Then
                        AnimatTools.Framework.Util.DisplayError(ex.InnerException.InnerException)
                    Else
                        AnimatTools.Framework.Util.DisplayError(ex.InnerException)
                    End If
                Else
                    AnimatTools.Framework.Util.DisplayError(ex)
                End If

                'If we got an error while trying to set the value to a new value then we need to change back the
                'value that is currently displayed to the previous value.
                Try
                    e.Value = Me.GetType().GetProperty(e.Property.PropertyName).GetValue(Me, Nothing)
                Catch InnerEx As System.Exception
                    'If we could not fix it then do nothing
                End Try
            Finally
                m_bSetValueInProgress = False
            End Try
        End Sub

        Public Overridable Sub ManualAddPropertyHistory(ByVal strPropertyName As String, ByVal origValue As Object, ByVal newValue As Object, Optional ByVal bOverrideSetValue As Boolean = False)

            Try
                If (Not Me.SetValueInProgress OrElse (Me.SetValueInProgress AndAlso bOverrideSetValue)) AndAlso Not Me.UndoRedoInProgress AndAlso Util.ModificationHistory.AllowAddHistory Then
                    Dim propInfo As System.Reflection.PropertyInfo = Me.GetType().GetProperty(strPropertyName)
                    Dim frmRoot As System.Windows.Forms.Form = Me.RootForm
                    Util.ModificationHistory.AddHistoryEvent(New UndoSystem.PropertyChangedEvent(frmRoot, Me, propInfo, origValue, newValue))
                End If

            Catch ex As System.Exception

            End Try

        End Sub

        Public Overridable Sub ManualAddPropertyHistory(ByVal oObject As Object, ByVal strPropertyName As String, ByVal origValue As Object, ByVal newValue As Object, Optional ByVal bOverrideSetValue As Boolean = False)

            Try
                If (Not Me.SetValueInProgress OrElse (Me.SetValueInProgress AndAlso bOverrideSetValue)) AndAlso Not Me.UndoRedoInProgress AndAlso Util.ModificationHistory.AllowAddHistory Then
                    Dim propInfo As System.Reflection.PropertyInfo = oObject.GetType().GetProperty(strPropertyName)

                    Dim frmRoot As System.Windows.Forms.Form
                    If TypeOf oObject Is AnimatTools.Framework.DataObject Then
                        Dim doObject As AnimatTools.Framework.DataObject = DirectCast(oObject, AnimatTools.Framework.DataObject)
                        frmRoot = doObject.RootForm
                    ElseIf TypeOf oObject Is AnimatTools.Forms.AnimatForm Then
                        Dim frmObject As AnimatTools.Forms.AnimatForm = DirectCast(oObject, AnimatTools.Forms.AnimatForm)
                        frmRoot = frmObject.RootForm
                    End If

                    Util.ModificationHistory.AddHistoryEvent(New UndoSystem.PropertyChangedEvent(frmRoot, oObject, propInfo, origValue, newValue))
                End If

            Catch ex As System.Exception

            End Try

        End Sub

        Public Overridable Sub ManualAddHistory(ByVal doEvent As AnimatTools.Framework.UndoSystem.HistoryEvent)

            Try
                If Not Me.UndoRedoInProgress AndAlso Util.ModificationHistory.AllowAddHistory Then
                    Util.ModificationHistory.AddHistoryEvent(doEvent)
                End If

            Catch ex As System.Exception

            End Try

        End Sub

        Public Overridable Sub FindChildrenOfType(ByVal tpTemplate As Type, ByRef colDataObjects As Collections.DataObjects)
            If tpTemplate Is Nothing OrElse Util.IsTypeOf(Me.GetType(), tpTemplate, False) Then
                colDataObjects.Add(Me)
            End If
        End Sub

        Public Overridable Sub ClearIsDirty()
            Me.IsDirty = False
        End Sub

        Protected Overridable Sub ScaleValueByUnit(ByVal strPropName As String, ByRef fltOrigValue As Single, ByVal fltScale As Single)
            Dim fltNewValue As Single = fltOrigValue * fltScale
            Me.ManualAddPropertyHistory(strPropName, fltOrigValue, fltNewValue)
            fltOrigValue = fltNewValue
        End Sub

        Protected Overridable Sub ScaleValueByUnit(ByVal strPropName As String, ByRef dblOrigValue As Double, ByVal fltScale As Single)
            Dim dblNewValue As Double = dblOrigValue * fltScale
            Me.ManualAddPropertyHistory(strPropName, dblOrigValue, dblNewValue)
            dblOrigValue = dblNewValue
        End Sub

        Protected Overridable Sub ScaleValueByUnit(ByVal strPropName As String, ByRef vOrigValue As AnimatTools.Framework.Vec3d, ByVal fltScale As Single)
            Dim vNewValue As Vec3d = DirectCast(vOrigValue.Clone(vOrigValue.Parent, False, Nothing), Vec3d)
            vNewValue.X = vNewValue.X * fltScale
            vNewValue.Y = vNewValue.Y * fltScale
            vNewValue.Z = vNewValue.Z * fltScale

            Me.ManualAddPropertyHistory(strPropName, vOrigValue, vNewValue)
            vOrigValue = vNewValue
        End Sub

        Public Overridable Sub UnitsChanged(ByVal ePrevMass As AnimatTools.DataObjects.Physical.Environment.enumMassUnits, _
                                            ByVal eNewMass As AnimatTools.DataObjects.Physical.Environment.enumMassUnits, _
                                            ByVal fltMassChange As Single, _
                                            ByVal ePrevDistance As AnimatTools.DataObjects.Physical.Environment.enumDistanceUnits, _
                                            ByVal eNewDistance As AnimatTools.DataObjects.Physical.Environment.enumDistanceUnits, _
                                            ByVal fltDistanceChange As Single)

        End Sub

        'How Clone works
        'Clone is called on the object that is to be cloned. That object then creates a copy of itself and passes this 
        'object to the CloneInternal method. CloneInternal is a generic method that uses the underlying DataObject base class.
        'This allows each base class to have its own CloneInternal method that only needs to set the values that it knows about.
        'Once all of the params have been cloned we exit Clone Internal, and we have a clone version of the original part.
        'However, some objects like muscle have references to other parts that may have been cloned as well. we do not want the
        'reference to continue pointing to the old object, but instead it should point to the new cloned version. An example of this i
        'how a muscle keeps a list of attachment points. If we simply clone those points as is, then when the muscle is cloned it will
        'have exactly the same attachments specified, even though we may have cloned a number of those attachments, and they should be
        'hooked to those new version instead of the old. This is where the AfterClone method comes in. This method should ONLY be called
        'initially from the root object that is being cloned. Then it progresses down the tree of objects. This makes sure that the original
        'root and the new cloned root are passed into the method. This allows us to search the new cloned root to see if any of these other
        'parts, like the attachments, have also been cloned. If they have then we need to swap them out for the old parts. Also, whenever
        'a part is cloned the old ID it had before the clone is stored in the CloneID param. This allows us to know which parts came from where.

        Public MustOverride Function Clone(ByVal doParent As AnimatTools.Framework.DataObject, ByVal bCutData As Boolean, _
                                           ByVal doRoot As AnimatTools.Framework.DataObject) As AnimatTools.Framework.DataObject

        Protected Overridable Sub CloneInternal(ByVal doOriginal As AnimatTools.Framework.DataObject, ByVal bCutData As Boolean, _
                                            ByVal doRoot As AnimatTools.Framework.DataObject)
            m_strCloneID = doOriginal.m_strID
            If bCutData Then
                m_strID = doOriginal.m_strID
            End If
            m_strName = doOriginal.m_strName
            m_bEnabled = doOriginal.m_bEnabled
            'm_doParent = doOriginal.m_doParent
        End Sub

        Public Overridable Sub AfterClone(ByVal doParent As AnimatTools.Framework.DataObject, ByVal bCutData As Boolean, _
        ByVal doOriginal As AnimatTools.Framework.DataObject, ByVal doClone As AnimatTools.Framework.DataObject)

        End Sub

        Public Overridable Function EnsureParentMdiShown() As AnimatTools.Forms.MdiChild
            Return Nothing
        End Function

        Public Overridable Sub LoadData(ByRef oXml As AnimatTools.Interfaces.StdXml)
            Me.IsDirty = False
        End Sub

        Public Overridable Sub SaveData(ByRef oXml As AnimatTools.Interfaces.StdXml)
        End Sub

        Public Overrides Function ToString() As String
            Return Me.Name
        End Function

        Public Overridable Sub InitAfterAppStart()

        End Sub

#End Region

#Region " Events "

        Protected Overridable Sub OnExpandAll(ByVal sender As Object, ByVal e As System.EventArgs)

            Try
                If Not sender Is Nothing AndAlso TypeOf sender Is Crownwood.Magic.Menus.MenuCommand Then
                    Dim mcCommand As Crownwood.Magic.Menus.MenuCommand = DirectCast(sender, Crownwood.Magic.Menus.MenuCommand)

                    If Not mcCommand.Tag Is Nothing AndAlso TypeOf mcCommand.Tag Is TreeNode Then
                        Dim tnNode As TreeNode = DirectCast(mcCommand.Tag, TreeNode)
                        tnNode.ExpandAll()
                    End If
                End If

            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try

        End Sub

        Protected Overridable Sub OnCollapseAll(ByVal sender As Object, ByVal e As System.EventArgs)

            Try
                If Not sender Is Nothing AndAlso TypeOf sender Is Crownwood.Magic.Menus.MenuCommand Then
                    Dim mcCommand As Crownwood.Magic.Menus.MenuCommand = DirectCast(sender, Crownwood.Magic.Menus.MenuCommand)

                    If Not mcCommand.Tag Is Nothing AndAlso TypeOf mcCommand.Tag Is TreeNode Then
                        Dim tnNode As TreeNode = DirectCast(mcCommand.Tag, TreeNode)
                        tnNode.Collapse()
                    End If
                End If

            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try

        End Sub

#End Region

    End Class

End Namespace


