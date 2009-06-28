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

    Public Class Nodes
        Inherits AnimatCollectionBase

        Public Sub New(ByVal doParent As Framework.DataObject)
            MyBase.New(doParent)
        End Sub

        Default Public Property Item(ByVal index As Integer) As Behavior.Node
            Get
                Return CType(List(index), Behavior.Node)
            End Get
            Set(ByVal Value As Behavior.Node)
                List(index) = Value
            End Set
        End Property


        Public Function Add(ByVal value As Behavior.Node) As Integer
            Me.IsDirty = True
            Return List.Add(value)
        End Function 'Add

        Public Function IndexOf(ByVal value As Behavior.Node) As Integer
            Return List.IndexOf(value)
        End Function 'IndexOf


        Public Sub Insert(ByVal index As Integer, ByVal value As Behavior.Node)
            Me.IsDirty = True
            List.Insert(index, value)
        End Sub 'Insert


        Public Sub Remove(ByVal value As Behavior.Node)
            Me.IsDirty = True
            List.Remove(value)
        End Sub 'Remove


        Public Function Contains(ByVal value As Behavior.Node) As Boolean
            ' If value is not of type Int16, this will return false.
            Return List.Contains(value)
        End Function 'Contains


        Protected Overrides Sub OnInsert(ByVal index As Integer, ByVal value As [Object])
            If Not TypeOf (value) Is Behavior.Node Then
                Throw New ArgumentException("value must be of type Behavior.Node.", "value")
            End If
        End Sub 'OnInsert


        Protected Overrides Sub OnRemove(ByVal index As Integer, ByVal value As [Object])
            If Not TypeOf (value) Is Behavior.Node Then
                Throw New ArgumentException("value must be of type Behavior.Node.", "value")
            End If
        End Sub 'OnRemove


        Protected Overrides Sub OnSet(ByVal index As Integer, ByVal oldValue As [Object], ByVal newValue As [Object])
            If Not TypeOf (newValue) Is Behavior.Node Then
                Throw New ArgumentException("newValue must be of type Behavior.Node.", "newValue")
            End If
        End Sub 'OnSet


        Protected Overrides Sub OnValidate(ByVal value As [Object])
            If Not TypeOf (value) Is Behavior.Node Then
                Throw New ArgumentException("value must be of type Behavior.Node.")
            End If
        End Sub 'OnValidate 

        Public Overrides Function Copy() As AnimatCollectionBase
            Dim aryList As New Nodes(m_doParent)
            aryList.CopyInternal(Me)
            Return aryList
        End Function

        Public Overrides Function Clone(ByVal doParent As AnimatTools.Framework.DataObject, ByVal bCutData As Boolean, _
                                           ByVal doRoot As AnimatTools.Framework.DataObject) As AnimatCollectionBase
            Dim aryList As New Nodes(doParent)
            aryList.CloneInternal(Me, doParent, bCutData, doRoot)
            Return aryList
        End Function

    End Class 'NodesCollection


    Public Class SortedNodes
        Inherits AnimatDictionaryBase

        Public Sub New(ByVal doParent As Framework.DataObject)
            MyBase.New(doParent)
        End Sub

        Default Public Property Item(ByVal key As [String]) As AnimatTools.DataObjects.Behavior.Node
            Get
                Return CType(Dictionary(key), AnimatTools.DataObjects.Behavior.Node)
            End Get
            Set(ByVal Value As AnimatTools.DataObjects.Behavior.Node)
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

        Public Sub Add(ByVal key As [String], ByVal value As AnimatTools.DataObjects.Behavior.Node)
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

            If Not TypeOf (value) Is AnimatTools.DataObjects.Behavior.Node Then
                Throw New ArgumentException("value must be of type AnimatTools.DataObjects.Behavior.Node.", "value")
            End If

            Dim diImage As AnimatTools.DataObjects.Behavior.Node = DirectCast(value, AnimatTools.DataObjects.Behavior.Node)

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

            If Not TypeOf (newValue) Is AnimatTools.DataObjects.Behavior.Node Then
                Throw New ArgumentException("newValue must be of type AnimatTools.DataObjects.Behavior.Node.", "newValue")
            End If
        End Sub 'OnSet

        Protected Overrides Sub OnValidate(ByVal key As [Object], ByVal value As [Object])
            If Not key.GetType() Is Type.GetType("System.String") Then
                Throw New ArgumentException("key must be of type String.", "key")
            End If

            If Not TypeOf (value) Is AnimatTools.DataObjects.Behavior.Node Then
                Throw New ArgumentException("value must be of type AnimatTools.DataObjects.Behavior.Node.", "value")
            End If
        End Sub 'OnValidate 

        Public Overrides Function Copy() As AnimatDictionaryBase
            Dim aryList As New SortedNodes(m_doParent)
            aryList.CopyInternal(Me)
            Return aryList
        End Function

        Public Overrides Function Clone(ByVal doParent As AnimatTools.Framework.DataObject, ByVal bCutData As Boolean, _
                                           ByVal doRoot As AnimatTools.Framework.DataObject) As AnimatDictionaryBase
            Dim aryList As New SortedNodes(doParent)
            aryList.CloneInternal(Me, doParent, bCutData, doRoot)
            Return aryList
        End Function

    End Class

    Namespace Comparers

        Public Class CompareNodeNames
            Implements IComparer

            ' Calls CaseInsensitiveComparer.Compare with the parameters reversed.
            Function Compare(ByVal x As [Object], ByVal y As [Object]) As Integer Implements IComparer.Compare
                If Not (TypeOf x Is AnimatTools.DataObjects.Behavior.Node AndAlso TypeOf y Is AnimatTools.DataObjects.Behavior.Node) Then Return 0

                Dim bnX As AnimatTools.DataObjects.Behavior.Node = DirectCast(x, AnimatTools.DataObjects.Behavior.Node)
                Dim bnY As AnimatTools.DataObjects.Behavior.Node = DirectCast(y, AnimatTools.DataObjects.Behavior.Node)

                Return New CaseInsensitiveComparer().Compare(bnX.Name, bnY.Name)

            End Function 'IComparer.Compare

        End Class

    End Namespace

End Namespace

