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

    Public Class DataTypes
        Inherits AnimatSortedList

        Public Sub New(ByVal doParent As Framework.DataObject)
            MyBase.New(doParent)
        End Sub

        Default Public Overloads Property Item(ByVal key As [String]) As AnimatTools.DataObjects.DataType
            Get
                Return CType(MyBase.Item(key), AnimatTools.DataObjects.DataType)
            End Get
            Set(ByVal Value As AnimatTools.DataObjects.DataType)
                MyBase.Item(key) = Value
            End Set
        End Property

        Public Overloads Sub Add(ByVal value As AnimatTools.DataObjects.DataType)
            MyBase.Add(value.ID, value)
            Me.IsDirty = True
        End Sub 'Add

        Public Overrides Function Copy() As AnimatSortedList
            Dim aryList As New DataTypes(m_doParent)
            aryList.CopyInternal(Me)
            Return aryList
        End Function

        Public Overrides Function CloneList() As AnimatSortedList
            Dim aryList As New DataTypes(m_doParent)
            aryList.CloneInternal(Me)
            Return aryList
        End Function

    End Class

End Namespace

