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

    Public MustInherit Class AnimatCollectionBase
        Inherits CollectionBase

        Protected m_doParent As Framework.DataObject
        Protected m_bIsDirty As Boolean = False

        <Browsable(False)> _
        Public Property Parent() As Framework.DataObject
            Get
                Return m_doParent
            End Get
            Set(ByVal Value As Framework.DataObject)
                m_doParent = Value
            End Set
        End Property

        <Browsable(False)> _
        Public Property IsDirty() As Boolean
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

        Public Sub New(ByVal doParent As Framework.DataObject)
            m_doParent = doParent
        End Sub

        Public Sub ClearIsDirty()
            Me.IsDirty = False

            Dim doObject As Framework.DataObject
            For Each deObject As Object In Me
                If TypeOf deObject Is Framework.DataObject Then
                    doObject = DirectCast(deObject, Framework.DataObject)
                    doObject.ClearIsDirty()
                End If
            Next

        End Sub

        Protected Overridable Sub CopyInternal(ByVal aryOrig As AnimatCollectionBase)

            Me.Clear()

            Dim doItem As AnimatTools.Framework.DataObject
            For Each doItem In aryOrig
                If TypeOf doItem Is AnimatTools.Framework.DataObject Then
                    List.Add(doItem)
                End If
            Next

        End Sub

        Protected Overridable Sub CloneInternal(ByVal aryOrig As AnimatCollectionBase, ByVal doParent As AnimatTools.Framework.DataObject, ByVal bCutData As Boolean, _
                                           ByVal doRoot As AnimatTools.Framework.DataObject)

            Me.Clear()

            Dim doOrig As AnimatTools.Framework.DataObject
            Dim doItem As AnimatTools.Framework.DataObject
            For Each doOrig In aryOrig
                If TypeOf doOrig Is AnimatTools.Framework.DataObject Then
                    doItem = doOrig.Clone(doParent, bCutData, doRoot)
                    List.Add(doItem)
                End If
            Next

        End Sub

        Public Overridable Function Copy() As AnimatCollectionBase
        End Function

        Public Overridable Function Clone(ByVal doParent As AnimatTools.Framework.DataObject, ByVal bCutData As Boolean, _
                                           ByVal doRoot As AnimatTools.Framework.DataObject) As AnimatCollectionBase
        End Function

    End Class

End Namespace
