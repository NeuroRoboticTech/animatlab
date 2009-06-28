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

Namespace Forms

	Public MustInherit Class AnimatForm
		Inherits System.Windows.Forms.Form

#Region " Attributes "

        Protected m_strID As String = System.Guid.NewGuid().ToString()
        Protected m_frmApplication As AnimatApplication
        Protected m_frmParent As AnimatForm
        Protected m_ctContent As Crownwood.Magic.Docking.Content
        Protected m_frmMdiParent As MdiChild
        Protected m_bUseDocking As Boolean = True
        Protected m_strTabPageName As String = ""
        Protected m_strTitle As String = ""

        Protected m_bFixedProperties As Boolean
        Protected m_Properties As PropertyTable

        'This is the root dataobject for any data in this form. It helps the
        'form keep track of whether data items are dirty.
        Protected m_doFormHelper As New DataObjects.FormHelper(Me)

        Protected m_bUndoRedoInProgress As Boolean = False
        Protected m_bSetValueInProgress As Boolean = False

#End Region

#Region " Properties "

        Public Overridable Property ID() As String
            Get
                Return m_strID
            End Get
            Set(ByVal Value As String)
                m_strID = Value
            End Set
        End Property

        Public Overridable ReadOnly Property AnimatApplication() As AnimatApplication
            Get
                Return m_frmApplication
            End Get
        End Property

        Public Overridable ReadOnly Property ParentAnimatForm() As AnimatForm
            Get
                Return m_frmParent
            End Get
        End Property

        <Browsable(False)> _
        Public Overridable ReadOnly Property RootForm() As System.Windows.Forms.Form
            Get
                Return Util.Application
            End Get
        End Property

        Public Overridable Property Content() As Crownwood.Magic.Docking.Content
            Get
                Return m_ctContent
            End Get
            Set(ByVal Value As Content)
                m_ctContent = Value
            End Set
        End Property

        Public Overridable ReadOnly Property AssemblyFile() As String
            Get
                Dim strPath As String, strFile As String
                Util.SplitPathAndFile(Me.GetType.Assembly.Location, strPath, strFile)
                Return strFile
            End Get
        End Property

        Public Overridable ReadOnly Property ClassName() As String
            Get
                Return Me.GetType.ToString
            End Get
        End Property

        Public Overridable ReadOnly Property IconName() As String
            Get
                Return ""
            End Get
        End Property

        Public Overridable Property TabPageName() As String
            Get
                Return m_strTabPageName
            End Get
            Set(ByVal Value As String)
                m_strTabPageName = Value
            End Set
        End Property

        Public Overridable Property UseDocking() As Boolean
            Get
                Return m_bUseDocking
            End Get
            Set(ByVal Value As Boolean)
                m_bUseDocking = Value
            End Set
        End Property

        Public Overridable ReadOnly Property DockingTabAppearance() As Crownwood.Magic.Controls.TabControl.VisualAppearance
            Get
                Return Crownwood.Magic.Controls.TabControl.VisualAppearance.MultiDocument
            End Get
        End Property

        Public Overridable ReadOnly Property DockingHideTabsMode() As Crownwood.Magic.Controls.TabControl.HideTabsModes
            Get
                Return Crownwood.Magic.Controls.TabControl.HideTabsModes.HideAlways
            End Get
        End Property

        Public Property FixedProperties() As Boolean
            Get
                Return m_bFixedProperties
            End Get
            Set(ByVal Value As Boolean)
                m_bFixedProperties = Value
            End Set
        End Property

        Public ReadOnly Property Properties() As PropertyBag
            Get
                If m_Properties Is Nothing OrElse Not m_bFixedProperties Then
                    CreateProperties()
                End If

                Return m_Properties
            End Get
        End Property

        Public ReadOnly Property FormHelper() As DataObjects.FormHelper
            Get
                Return m_doFormHelper
            End Get
        End Property

        Public Overridable Property Title() As String
            Get
                Return m_strTitle
            End Get
            Set(ByVal Value As String)
                m_strTitle = Value
                Me.Text = m_strTitle
            End Set
        End Property

        Public Overrides Property Text() As String
            Get
                Return MyBase.Text
            End Get
            Set(ByVal Value As String)
                If Not m_doFormHelper Is Nothing AndAlso m_doFormHelper.IsDirty Then
                    MyBase.Text = m_strTitle & " *"
                Else
                    MyBase.Text = m_strTitle
                End If
            End Set
        End Property

        Public Overridable Property IsDirty() As Boolean
            Get
                Return m_doFormHelper.IsDirty
            End Get
            Set(ByVal Value As Boolean)
                If Not Util.DisableDirtyFlags AndAlso m_doFormHelper.IsDirty <> Value Then
                    m_doFormHelper.IsDirty = Value
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

        <Browsable(False)> _
        Public ReadOnly Property IsCtrlKeyPressed() As Boolean
            Get
                If Control.ModifierKeys = Keys.Control Then
                    Return True
                Else
                    Return False
                End If
            End Get
        End Property

        <Browsable(False)> _
        Public ReadOnly Property IsShiftKeyPressed() As Boolean
            Get
                If Control.ModifierKeys = Keys.Shift Then
                    Return True
                Else
                    Return False
                End If
            End Get
        End Property

        <Browsable(False)> _
        Public ReadOnly Property IsAltKeyPressed() As Boolean
            Get
                If Control.ModifierKeys = Keys.Alt Then
                    Return True
                Else
                    Return False
                End If
            End Get
        End Property

#End Region

#Region " Methods "

        Public Overridable Sub Initialize(ByRef frmApplication As AnimatApplication, _
                                          Optional ByVal frmMdiParent As MdiChild = Nothing, _
                                          Optional ByVal frmParent As AnimatForm = Nothing)
            m_frmApplication = frmApplication
            m_frmParent = frmParent
            m_frmMdiParent = frmMdiParent
            m_strTabPageName = "Page 1"

            If Not m_frmMdiParent Is Nothing Then
                m_doFormHelper.Parent = m_frmMdiParent.FormHelper
            ElseIf Not m_frmParent Is Nothing Then
                m_doFormHelper.Parent = m_frmParent.FormHelper
            Else
                m_doFormHelper.Parent = Nothing
            End If
        End Sub

        Public Overridable Function CreateMenu() As MenuControl

        End Function

        Public Overridable Function CreateToolbar(ByRef menuDefault As MenuControl) As Crownwood.Magic.Toolbars.ToolbarControl

        End Function

        Public Overridable Overloads Sub LoadData(ByRef oXml As AnimatTools.Interfaces.StdXml)

        End Sub

        Public Overridable Sub SaveData(ByRef oXml As AnimatTools.Interfaces.StdXml)

            oXml.AddChildElement("Form")
            oXml.IntoElem() 'Into Form Element

            oXml.AddChildElement("Title", Me.Title)
            oXml.AddChildElement("AssemblyFile", Me.AssemblyFile)
            oXml.AddChildElement("ClassName", Me.ClassName)

            If Not Me.Content Is Nothing Then
                oXml.AddChildElement("TabTitle", Me.Content.TabTitle)
                oXml.AddChildElement("BackgroundForm", Me.Content.BackgroundForm)
            End If

            oXml.OutOfElem()  'Outof Form Element
        End Sub

        Public Overridable Sub OnBeforeFormAdded()
        End Sub

        Public Overridable Sub OnAfterFormAdded()
        End Sub

        Public Overridable Sub OnAfterMdiParentInitialized()
        End Sub

        'This is here so view windows can cancel their close events.
        Public Overridable Sub OnContentClosing(ByVal e As System.ComponentModel.CancelEventArgs)
        End Sub

        Protected Overridable Sub CreateProperties()

            m_Properties = New PropertyTable

            AddHandler m_Properties.GetValue, AddressOf Me.Properties_GetValue
            AddHandler m_Properties.SetValue, AddressOf Me.Properties_SetValue

            BuildProperties()
        End Sub

        Protected Overridable Sub BuildProperties()

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
                    Throw New System.Exception("No property info returned for the property name '" & e.Property.PropertyName & "'.")
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

        Public Overridable Sub ClearIsDirty()
            Me.IsDirty = False
        End Sub

        Public Overridable Sub RefreshTitle()
            Me.Title = Me.Title
        End Sub

        Public Overridable Sub UndoRedoRefresh(ByVal doRefresh As AnimatTools.Framework.DataObject)
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
