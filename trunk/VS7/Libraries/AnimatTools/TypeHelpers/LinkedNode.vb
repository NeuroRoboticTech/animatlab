Imports System
Imports System.Drawing
Imports System.Collections
Imports System.ComponentModel
Imports System.Windows.Forms
Imports System.Diagnostics
Imports System.IO
Imports System.Xml
Imports Crownwood.Magic.Controls
Imports AnimatTools.Framework

Namespace TypeHelpers

    Public Class LinkedNode
        Inherits AnimatTools.Framework.DataObject

#Region " Attributes "

        Protected m_beEditor As AnimatTools.Forms.Behavior.Editor
        Protected m_bnLinkedNode As AnimatTools.DataObjects.Behavior.Node

#End Region

#Region " Properties "

        <Browsable(False)> _
        Public Property Editor() As AnimatTools.Forms.Behavior.Editor
            Get
                Return m_beEditor
            End Get
            Set(ByVal Value As AnimatTools.Forms.Behavior.Editor)
                m_beEditor = Value
            End Set
        End Property

        <Browsable(False)> _
        Public Property Node() As AnimatTools.DataObjects.Behavior.Node
            Get
                Return m_bnLinkedNode
            End Get
            Set(ByVal Value As AnimatTools.DataObjects.Behavior.Node)
                m_bnLinkedNode = Value
            End Set
        End Property

        <Browsable(False)> _
        Public Overrides Property ViewSubProperties() As Boolean
            Get
                Return False
            End Get
            Set(ByVal Value As Boolean)
            End Set
        End Property

#End Region

#Region " Methods "

        Public Sub New(ByVal doParent As AnimatTools.Framework.DataObject)
            MyBase.New(doParent)
        End Sub

        Public Sub New(ByVal beEditor As AnimatTools.Forms.Behavior.Editor, _
                       ByVal bnLinkedNode As AnimatTools.DataObjects.Behavior.Node)
            MyBase.New(bnLinkedNode)

            m_beEditor = beEditor
            m_bnLinkedNode = bnLinkedNode
        End Sub

        Public Overrides Function Clone(ByVal doParent As AnimatTools.Framework.DataObject, ByVal bCutData As Boolean, _
                                        ByVal doRoot As AnimatTools.Framework.DataObject) As AnimatTools.Framework.DataObject
            Dim origNode As New LinkedNode(doParent)
            origNode.m_beEditor = m_beEditor
            origNode.m_bnLinkedNode = m_bnLinkedNode
            Return origNode
        End Function

        Protected Overrides Sub BuildProperties()
        End Sub

        Public Overrides Sub BuildPropertyDropDown(ByRef ctrlDropDown As System.Windows.Forms.Control)
            If m_beEditor Is Nothing Then Return

            If Not TypeOf (ctrlDropDown) Is TreeView Then
                Throw New System.Exception("The control passed into LinkedSynapse.BuildPropertyDropDown is not a treeview type")
            End If

            Dim tvTree As TreeView = DirectCast(ctrlDropDown, TreeView)

            tvTree.BeginUpdate()
            tvTree.Nodes.Clear()

            m_beEditor.CreateDiagramDropDownTree(tvTree)

            tvTree.Width = 300
            tvTree.ExpandAll()

            MyBase.FormatDropDownTree(tvTree, 8)

            tvTree.EndUpdate()
            tvTree.Invalidate()

        End Sub

#End Region

    End Class

End Namespace
