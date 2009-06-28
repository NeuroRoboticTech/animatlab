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

Namespace Framework.UndoSystem

    Public Class DiagramChangedEvent
        Inherits AnimatTools.Framework.UndoSystem.HistoryEvent

#Region " Attributes "

        Protected m_frmEditor As AnimatTools.Forms.Behavior.Editor
        Protected m_frmDiagram As AnimatTools.Forms.Behavior.Diagram
        Protected m_bdAlteredData As AnimatTools.DataObjects.Behavior.Data

#End Region

#Region " Properties "

        Public Overridable Property Diagram() As AnimatTools.Forms.Behavior.Diagram
            Get
                Return m_frmDiagram
            End Get
            Set(ByVal Value As AnimatTools.Forms.Behavior.Diagram)
                If Value Is Nothing Then
                    Throw New System.Exception("A Diagram change event must be associated with a diagram.")
                End If

                m_frmDiagram = Value
            End Set
        End Property

        Public Overridable Property AlteredData() As AnimatTools.DataObjects.Behavior.Data
            Get
                Return m_bdAlteredData
            End Get
            Set(ByVal Value As AnimatTools.DataObjects.Behavior.Data)
                m_bdAlteredData = Value
            End Set
        End Property

#End Region

#Region " Methods "

        Public Sub New(ByVal frmEditor As AnimatTools.Forms.Behavior.Editor, ByVal frmDiagram As AnimatTools.Forms.Behavior.Diagram, Optional ByVal bdAltered As AnimatTools.DataObjects.Behavior.Data = Nothing)
            MyBase.New(frmEditor)

            If frmEditor Is Nothing Then
                Throw New System.Exception("The behavioral editor must not be null.")
            End If

            m_frmEditor = frmEditor
            Me.Diagram = frmDiagram
            Me.AlteredData = bdAltered
        End Sub

        Protected Overridable Sub RefreshParent(ByVal doObject As AnimatTools.Framework.DataObject)

            If Not m_mdiParent Is Nothing Then
                m_mdiParent.MakeVisible()
                m_mdiParent.UndoRedoRefresh(doObject)
            ElseIf Not m_frmParent Is Nothing Then
                m_frmParent.UndoRedoRefresh(doObject)
            End If

        End Sub

        Public Overrides Sub Undo()
            m_frmEditor.SelectedDiagram(m_frmDiagram)
            m_frmDiagram.OnUndo()

            If Not m_bdAlteredData Is Nothing Then
                m_frmDiagram.UpdateData(m_bdAlteredData, False, False)
            End If
            m_frmEditor.PropertiesBar.RefreshProperties()
        End Sub

        Public Overrides Sub Redo()
            m_frmEditor.SelectedDiagram(m_frmDiagram)
            m_frmDiagram.OnRedo()
            m_frmEditor.PropertiesBar.Refresh()

            If Not m_bdAlteredData Is Nothing Then
                m_frmDiagram.UpdateData(m_bdAlteredData, False, False)
            End If
            m_frmEditor.PropertiesBar.RefreshProperties()
        End Sub

#End Region

    End Class

End Namespace

