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
Imports AnimatTools.Framework

Namespace Collections

    Public Class DataColumns
        Inherits AnimatCollectionBase

        Public Sub New(ByVal doParent As Framework.DataObject)
            MyBase.New(doParent)
        End Sub

        Default Public Property Item(ByVal index As Integer) As AnimatTools.DataObjects.Charting.DataColumn
            Get
                Return CType(List(index), AnimatTools.DataObjects.Charting.DataColumn)
            End Get
            Set(ByVal Value As AnimatTools.DataObjects.Charting.DataColumn)
                List(index) = Value
            End Set
        End Property


        Public Function Add(ByVal value As AnimatTools.DataObjects.Charting.DataColumn) As Integer
            Me.IsDirty = True
            Return List.Add(value)
        End Function 'Add

        Public Function IndexOf(ByVal value As AnimatTools.DataObjects.Charting.DataColumn) As Integer
            Return List.IndexOf(value)
        End Function 'IndexOf


        Public Sub Insert(ByVal index As Integer, ByVal value As AnimatTools.DataObjects.Charting.DataColumn)
            Me.IsDirty = True
            List.Insert(index, value)
        End Sub 'Insert


        Public Sub Remove(ByVal value As AnimatTools.DataObjects.Charting.DataColumn)
            Me.IsDirty = True
            List.Remove(value)
        End Sub 'Remove


        Public Function Contains(ByVal value As AnimatTools.DataObjects.Charting.DataColumn) As Boolean
            ' If value is not of type Int16, this will return false.
            Return List.Contains(value)
        End Function 'Contains


        Protected Overrides Sub OnInsert(ByVal index As Integer, ByVal value As [Object])
            If Not TypeOf (value) Is AnimatTools.DataObjects.Charting.DataColumn Then
                Throw New ArgumentException("value must be of type AnimatTools.DataObjects.Charting.DataColumn.", "value")
            End If
        End Sub 'OnInsert


        Protected Overrides Sub OnRemove(ByVal index As Integer, ByVal value As [Object])
            If Not TypeOf (value) Is AnimatTools.DataObjects.Charting.DataColumn Then
                Throw New ArgumentException("value must be of type AnimatTools.DataObjects.Charting.DataColumn.", "value")
            End If
        End Sub 'OnRemove


        Protected Overrides Sub OnSet(ByVal index As Integer, ByVal oldValue As [Object], ByVal newValue As [Object])
            If Not TypeOf (newValue) Is AnimatTools.DataObjects.Charting.DataColumn Then
                Throw New ArgumentException("newValue must be of type AnimatTools.DataObjects.Charting.DataColumn.", "newValue")
            End If
        End Sub 'OnSet


        Protected Overrides Sub OnValidate(ByVal value As [Object])
            If Not TypeOf (value) Is AnimatTools.DataObjects.Charting.DataColumn Then
                Throw New ArgumentException("value must be of type AnimatTools.DataObjects.Charting.DataColumn.")
            End If
        End Sub 'OnValidate 

        Public Overrides Function Copy() As AnimatCollectionBase
            Dim aryList As New DataColumns(m_doParent)
            aryList.CopyInternal(Me)
            Return aryList
        End Function

        Public Overrides Function Clone(ByVal doParent As AnimatTools.Framework.DataObject, ByVal bCutData As Boolean, _
                                           ByVal doRoot As AnimatTools.Framework.DataObject) As AnimatCollectionBase
            Dim aryList As New DataColumns(doParent)
            aryList.CloneInternal(Me, doParent, bCutData, doRoot)
            Return aryList
        End Function

    End Class 'NodesCollection

    Public Class SortedDataColumns
        Inherits AnimatSortedList

        Public Sub New(ByVal doParent As Framework.DataObject)
            MyBase.New(doParent)
        End Sub

        Default Public Overloads Property Item(ByVal key As [String]) As AnimatTools.DataObjects.Charting.DataColumn
            Get
                Return CType(MyBase.Item(key), AnimatTools.DataObjects.Charting.DataColumn)
            End Get
            Set(ByVal Value As AnimatTools.DataObjects.Charting.DataColumn)
                MyBase.Item(key) = Value
            End Set
        End Property

        Public Overloads Sub Add(ByVal key As [String], ByVal value As AnimatTools.DataObjects.Charting.DataColumn)
            MyBase.Add(key, value)
            Me.IsDirty = True
        End Sub 'Add

        Public Overrides Function Copy() As AnimatSortedList
            Dim aryList As New SortedDataColumns(m_doParent)
            aryList.CopyInternal(Me)
            Return aryList
        End Function

        Public Overrides Function CloneList() As AnimatSortedList
            Dim aryList As New SortedDataColumns(m_doParent)
            aryList.CloneInternal(Me)
            Return aryList
        End Function

    End Class

End Namespace

