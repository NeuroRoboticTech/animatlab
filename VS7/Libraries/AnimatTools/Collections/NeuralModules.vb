Imports System
Imports System.Drawing
Imports System.Collections
Imports System.ComponentModel
Imports System.Windows.Forms
Imports System.Diagnostics
Imports System.IO
Imports System.Xml
Imports Crownwood.Magic.Controls
Imports AnimatTools.DataObjects

Namespace Collections

    Public Class NeuralModules
        Inherits AnimatCollectionBase

        Public Sub New(ByVal doParent As Framework.DataObject)
            MyBase.New(doParent)
        End Sub

        Default Public Property Item(ByVal index As Integer) As AnimatTools.DataObjects.Behavior.NeuralModule
            Get
                Return CType(List(index), AnimatTools.DataObjects.Behavior.NeuralModule)
            End Get
            Set(ByVal Value As AnimatTools.DataObjects.Behavior.NeuralModule)
                List(index) = Value
            End Set
        End Property


        Public Function Add(ByVal value As AnimatTools.DataObjects.Behavior.NeuralModule) As Integer
            Me.IsDirty = True
            Return List.Add(value)
        End Function 'Add

        Public Function IndexOf(ByVal value As AnimatTools.DataObjects.Behavior.NeuralModule) As Integer
            Return List.IndexOf(value)
        End Function 'IndexOf


        Public Sub Insert(ByVal index As Integer, ByVal value As AnimatTools.DataObjects.Behavior.NeuralModule)
            Me.IsDirty = True
            List.Insert(index, value)
        End Sub 'Insert


        Public Sub Remove(ByVal value As AnimatTools.DataObjects.Behavior.NeuralModule)
            Me.IsDirty = True
            List.Remove(value)
        End Sub 'Remove


        Public Function Contains(ByVal value As AnimatTools.DataObjects.Behavior.NeuralModule) As Boolean
            ' If value is not of type Int16, this will return false.
            Return List.Contains(value)
        End Function 'Contains


        Protected Overrides Sub OnInsert(ByVal index As Integer, ByVal value As [Object])
            If Not TypeOf (value) Is AnimatTools.DataObjects.Behavior.NeuralModule Then
                Throw New ArgumentException("value must be of type AnimatTools.DataObjects.Behavior.NeuralModule.", "value")
            End If
        End Sub 'OnInsert


        Protected Overrides Sub OnRemove(ByVal index As Integer, ByVal value As [Object])
            If Not TypeOf (value) Is AnimatTools.DataObjects.Behavior.NeuralModule Then
                Throw New ArgumentException("value must be of type AnimatTools.DataObjects.Behavior.NeuralModule.", "value")
            End If
        End Sub 'OnRemove


        Protected Overrides Sub OnSet(ByVal index As Integer, ByVal oldValue As [Object], ByVal newValue As [Object])
            If Not TypeOf (newValue) Is AnimatTools.DataObjects.Behavior.NeuralModule Then
                Throw New ArgumentException("newValue must be of type AnimatTools.DataObjects.Behavior.NeuralModule.", "newValue")
            End If
        End Sub 'OnSet

        Protected Overrides Sub OnValidate(ByVal value As [Object])
            If Not TypeOf (value) Is AnimatTools.DataObjects.Behavior.NeuralModule Then
                Throw New ArgumentException("value must be of type AnimatTools.DataObjects.Behavior.NeuralModule.")
            End If
        End Sub 'OnValidate 

        Public Overrides Function Copy() As AnimatCollectionBase
            Dim aryList As New NeuralModules(m_doParent)
            aryList.CopyInternal(Me)
            Return aryList
        End Function

        Public Overrides Function Clone(ByVal doParent As AnimatTools.Framework.DataObject, ByVal bCutData As Boolean, _
                                           ByVal doRoot As AnimatTools.Framework.DataObject) As AnimatCollectionBase
            Dim aryList As New NeuralModules(doParent)
            aryList.CloneInternal(Me, doParent, bCutData, doRoot)
            Return aryList
        End Function

    End Class 'NodesCollection

    Public Class SortedNeuralModules
        Inherits AnimatDictionaryBase

        Public Sub New(ByVal doParent As Framework.DataObject)
            MyBase.New(doParent)
        End Sub

        Default Public Property Item(ByVal key As [String]) As AnimatTools.DataObjects.Behavior.NeuralModule
            Get
                Return CType(Dictionary(key), AnimatTools.DataObjects.Behavior.NeuralModule)
            End Get
            Set(ByVal Value As AnimatTools.DataObjects.Behavior.NeuralModule)
                Dictionary(key) = Value
            End Set
        End Property

        Public ReadOnly Property Keys() As ICollection
            Get
                Return Dictionary.Keys
            End Get
        End Property

        Public ReadOnly Property Values() As ICollection
            Get
                Return Dictionary.Values
            End Get
        End Property

        Public Sub Add(ByVal key As [String], ByVal value As AnimatTools.DataObjects.Behavior.NeuralModule)
            Dictionary.Add(key, value)
            Me.IsDirty = True
        End Sub 'Add

        Public Function Contains(ByVal key As [String]) As Boolean
            Return Dictionary.Contains(key)
        End Function 'Contains

        Public Sub Remove(ByVal key As [String])
            Dictionary.Remove(key)
            Me.IsDirty = True
        End Sub 'Remove

        Protected Overrides Sub OnInsert(ByVal key As [Object], ByVal value As [Object])
            If Not key.GetType() Is Type.GetType("System.String") Then
                Throw New ArgumentException("key must be of type String.", "key")
            End If

            If Not TypeOf (value) Is AnimatTools.DataObjects.Behavior.NeuralModule Then
                Throw New ArgumentException("value must be of type AnimatTools.DataObjects.Behavior.NeuralModule.", "value")
            End If

            Dim diImage As AnimatTools.DataObjects.Behavior.NeuralModule = DirectCast(value, AnimatTools.DataObjects.Behavior.NeuralModule)

        End Sub 'OnInsert

        Protected Overrides Sub OnRemove(ByVal key As [Object], ByVal value As [Object])
            If Not key.GetType() Is Type.GetType("System.String") Then
                Throw New ArgumentException("key must be of type String.", "key")
            End If
        End Sub 'OnRemove

        Protected Overrides Sub OnSet(ByVal key As [Object], ByVal oldValue As [Object], ByVal newValue As [Object])
            If Not key.GetType() Is Type.GetType("System.String") Then
                Throw New ArgumentException("key must be of type String.", "key")
            End If

            If Not TypeOf (newValue) Is AnimatTools.DataObjects.Behavior.NeuralModule Then
                Throw New ArgumentException("newValue must be of type AnimatTools.DataObjects.Behavior.NeuralModule.", "newValue")
            End If
        End Sub 'OnSet

        Protected Overrides Sub OnValidate(ByVal key As [Object], ByVal value As [Object])
            If Not key.GetType() Is Type.GetType("System.String") Then
                Throw New ArgumentException("key must be of type String.", "key")
            End If

            If Not TypeOf (value) Is AnimatTools.DataObjects.Behavior.NeuralModule Then
                Throw New ArgumentException("value must be of type AnimatTools.DataObjects.Behavior.NeuralModule.", "value")
            End If
        End Sub 'OnValidate 

        Public Overrides Function Copy() As AnimatDictionaryBase
            Dim aryList As New SortedNeuralModules(m_doParent)
            aryList.CopyInternal(Me)
            Return aryList
        End Function

        Public Overrides Function Clone(ByVal doParent As AnimatTools.Framework.DataObject, ByVal bCutData As Boolean, _
                                           ByVal doRoot As AnimatTools.Framework.DataObject) As AnimatDictionaryBase
            Dim aryModules As New SortedNeuralModules(doParent)

            Dim nmModule As AnimatTools.DataObjects.Behavior.NeuralModule
            Dim nmNewModule As AnimatTools.DataObjects.Behavior.NeuralModule
            For Each deEntry As DictionaryEntry In Me
                nmModule = DirectCast(deEntry.Value, AnimatTools.DataObjects.Behavior.NeuralModule)
                nmNewModule = DirectCast(nmModule.Clone(doParent, bCutData, doRoot), AnimatTools.DataObjects.Behavior.NeuralModule)
                aryModules.Add(nmNewModule.ClassName, nmNewModule)
            Next

            Return aryModules
        End Function

    End Class 'DiagramsDictionary

End Namespace
