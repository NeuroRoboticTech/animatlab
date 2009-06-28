Imports System
Imports System.Drawing
Imports System.Collections
Imports System.ComponentModel
Imports System.Windows.Forms
Imports System.Diagnostics
Imports System.IO
Imports System.Xml
Imports Crownwood.Magic.Controls

Namespace Framework.UndoSystem

    Public MustInherit Class HistoryEvent

#Region " Attributes "

        Protected m_mdiParent As AnimatTools.Forms.MdiChild
        Protected m_frmParent As AnimatTools.Forms.AnimatForm

#End Region

#Region " Properties "

        Public Overridable Property MdiParent() As AnimatTools.Forms.MdiChild
            Get
                Return m_mdiParent
            End Get
            Set(ByVal Value As AnimatTools.Forms.MdiChild)
                m_mdiParent = Value
            End Set
        End Property

        Public Overridable Property AnimatParent() As AnimatTools.Forms.AnimatForm
            Get
                Return m_frmParent
            End Get
            Set(ByVal Value As AnimatTools.Forms.AnimatForm)
                m_frmParent = Value
            End Set
        End Property

#End Region

#Region " Methods "

        Public Sub New(ByVal frmParent As System.Windows.Forms.Form)

            If TypeOf frmParent Is AnimatTools.Forms.MdiChild Then
                m_mdiParent = DirectCast(frmParent, AnimatTools.Forms.MdiChild)
            ElseIf TypeOf frmParent Is AnimatTools.Forms.AnimatForm Then
                m_frmParent = DirectCast(frmParent, AnimatTools.Forms.AnimatForm)
            End If

        End Sub

        Public MustOverride Sub Undo()
        Public MustOverride Sub Redo()

#End Region

    End Class

End Namespace

