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

Namespace DataObjects.Behavior

    Public Class LinkedSynapse
        Inherits AnimatTools.Framework.DataObject

#Region " Attributes "

        Protected m_bnNode As AnimatTools.DataObjects.Behavior.Node
        Protected m_blLink As AnimatTools.DataObjects.Behavior.Link

#End Region

#Region " Properties "

        <Browsable(False)> _
        Public Property Node() As AnimatTools.DataObjects.Behavior.Node
            Get
                Return m_bnNode
            End Get
            Set(ByVal Value As AnimatTools.DataObjects.Behavior.Node)
                m_bnNode = Value
            End Set
        End Property

        <Browsable(False)> _
        Public Property Link() As AnimatTools.DataObjects.Behavior.Link
            Get
                Return m_blLink
            End Get
            Set(ByVal Value As AnimatTools.DataObjects.Behavior.Link)
                m_blLink = Value
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

        Public Sub New(ByRef bnNode As AnimatTools.DataObjects.Behavior.Node, ByRef blLink As AnimatTools.DataObjects.Behavior.Link)
            MyBase.New(blLink)

            m_bnNode = bnNode
            m_blLink = blLink
        End Sub

        Public Overrides Function Clone(ByVal doParent As AnimatTools.Framework.DataObject, ByVal bCutData As Boolean, _
                                        ByVal doRoot As AnimatTools.Framework.DataObject) As AnimatTools.Framework.DataObject
            Dim oNew As New LinkedSynapse(doParent)
            oNew.Node = Me.Node
            oNew.Link = Me.Link
            Return oNew
        End Function

        Protected Overrides Sub BuildProperties()

        End Sub

        Public Overrides Sub BuildPropertyDropDown(ByRef ctrlDropDown As System.Windows.Forms.Control)
            If m_bnNode Is Nothing Then Return

            If TypeOf m_bnNode Is AnimatTools.DataObjects.Behavior.Nodes.OffPage Then
                Dim doNode As AnimatTools.DataObjects.Behavior.Nodes.OffPage = DirectCast(m_bnNode, AnimatTools.DataObjects.Behavior.Nodes.OffPage)
                If doNode.LinkedNode Is Nothing OrElse doNode.LinkedNode.Node Is Nothing Then Return

                m_bnNode = doNode.LinkedNode.Node
            End If

            If Not TypeOf (ctrlDropDown) Is ListBox Then
                Throw New System.Exception("The control passed into LinkedSynapse.BuildPropertyDropDown is not a listbox type")
            End If

            Dim lbList As ListBox = DirectCast(ctrlDropDown, ListBox)

            lbList.BeginUpdate()
            lbList.Items.Clear()
            Dim lbSelectedItem As AnimatTools.TypeHelpers.DropDownEntry = Nothing
            For Each oLink As DictionaryEntry In m_bnNode.InLinks
                Dim blLink As AnimatTools.DataObjects.Behavior.Link = DirectCast(oLink.Value, AnimatTools.DataObjects.Behavior.Link)

                If TypeOf (blLink) Is Synapses.Normal Then
                    Dim lsSynapse As AnimatTools.Framework.DataObject = New LinkedSynapse(m_bnNode, blLink)
                    Dim leItem As New AnimatTools.TypeHelpers.DropDownEntry((blLink.Origin.Text & " (" & Replace(blLink.Text, vbCrLf, " ") & ")"), lsSynapse)

                    lbList.Items.Add(leItem)

                    If Not m_blLink Is Nothing AndAlso m_blLink.ID = blLink.ID Then
                        lbSelectedItem = leItem
                    End If
                End If
            Next

            If Not lbSelectedItem Is Nothing Then lbList.SelectedItem = lbSelectedItem
            lbList.DisplayMember = "Display"
            lbList.ValueMember = "Value"

            MyBase.FormatDropDownList(lbList)

            lbList.EndUpdate()
            lbList.Invalidate()

        End Sub

#End Region

    End Class

End Namespace

