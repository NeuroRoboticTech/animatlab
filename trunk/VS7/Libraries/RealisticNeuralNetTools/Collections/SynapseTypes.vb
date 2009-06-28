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

    Public Class SynapseTypes
        Inherits AnimatTools.Collections.AnimatDictionaryBase

        Public Sub New(ByVal doParent As AnimatTools.Framework.DataObject)
            MyBase.New(doParent)
        End Sub

        Default Public Property Item(ByVal key As [String]) As RealisticNeuralNetTools.DataObjects.Behavior.SynapseType
            Get
                Return CType(Dictionary(key), RealisticNeuralNetTools.DataObjects.Behavior.SynapseType)
            End Get
            Set(ByVal Value As RealisticNeuralNetTools.DataObjects.Behavior.SynapseType)
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

        Public Sub Add(ByVal key As [String], ByVal value As RealisticNeuralNetTools.DataObjects.Behavior.SynapseType)
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

            If Not TypeOf (value) Is RealisticNeuralNetTools.DataObjects.Behavior.SynapseType Then
                Throw New ArgumentException("value must be of type RealisticNeuralNetTools.DataObjects.Behavior.SynapseType.", "value")
            End If

            Dim diImage As RealisticNeuralNetTools.DataObjects.Behavior.SynapseType = DirectCast(value, RealisticNeuralNetTools.DataObjects.Behavior.SynapseType)

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

            If Not TypeOf (newValue) Is RealisticNeuralNetTools.DataObjects.Behavior.SynapseType Then
                Throw New ArgumentException("newValue must be of type RealisticNeuralNetTools.DataObjects.Behavior.SynapseType.", "newValue")
            End If
        End Sub 'OnSet

        Protected Overrides Sub OnValidate(ByVal key As [Object], ByVal value As [Object])
            If Not key.GetType() Is Type.GetType("System.String") Then
                Throw New ArgumentException("key must be of type String.", "key")
            End If

            If Not TypeOf (value) Is RealisticNeuralNetTools.DataObjects.Behavior.SynapseType Then
                Throw New ArgumentException("value must be of type RealisticNeuralNetTools.DataObjects.Behavior.SynapseType.", "value")
            End If
        End Sub 'OnValidate 

        Public Overrides Function Copy() As AnimatTools.Collections.AnimatDictionaryBase
            Dim aryList As New SynapseTypes(m_doParent)
            aryList.CopyInternal(Me)
            Return aryList
        End Function

        Public Overrides Function Clone(ByVal doParent As AnimatTools.Framework.DataObject, ByVal bCutData As Boolean, _
                                           ByVal doRoot As AnimatTools.Framework.DataObject) As AnimatTools.Collections.AnimatDictionaryBase
            Dim aryList As New SynapseTypes(doParent)
            aryList.CloneInternal(Me, doParent, bCutData, doRoot)
            Return aryList
        End Function

    End Class 'DiagramsDictionary

End Namespace
