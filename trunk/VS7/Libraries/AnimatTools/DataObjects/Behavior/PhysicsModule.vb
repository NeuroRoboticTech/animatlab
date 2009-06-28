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

    Public Class PhysicsModule
        Inherits NeuralModule

#Region " Properties "

        Public Overrides ReadOnly Property NetworkFilename() As String
            Get
                Return ""
            End Get
        End Property

        Public Overrides ReadOnly Property ModuleFilename() As String
            Get
                Return ""
            End Get
        End Property

#End Region

#Region " Methods "

        Public Sub New(ByVal doParent As AnimatTools.Framework.DataObject)
            MyBase.New(doParent)

            m_snTimeStep = New AnimatTools.Framework.ScaledNumber(Me, "TimeStep", 10, AnimatTools.Framework.ScaledNumber.enumNumericScale.milli, "seconds", "s")
        End Sub

        Public Overrides Function Clone(ByVal doParent As AnimatTools.Framework.DataObject, ByVal bCutData As Boolean, _
                                        ByVal doRoot As AnimatTools.Framework.DataObject) As AnimatTools.Framework.DataObject
            Dim oNewModule As New DataObjects.Behavior.PhysicsModule(doParent)
            oNewModule.CloneInternal(Me, bCutData, doRoot)
            If Not doRoot Is Nothing AndAlso doRoot Is Me Then oNewModule.AfterClone(Me, bCutData, doRoot, oNewModule)
            Return oNewModule
        End Function

        Public Overrides Sub SaveNetwork(ByRef oXml As Interfaces.StdXml)
            PopulateModuleData()
            Me.Organism.AddContactAdapters(Me, m_aryNodes)

            If m_aryNodes.Count > 0 Then

                oXml.IntoElem() 'Into the Adapters Element
                Dim bnNode As DataObjects.Behavior.Node
                Dim iNodeIndex As Integer = 0
                For Each deEntry As DictionaryEntry In m_aryNodes
                    bnNode = DirectCast(deEntry.Value, DataObjects.Behavior.Node)
                    bnNode.NodeIndex = iNodeIndex
                    bnNode.SaveNetwork(oXml, Me)
                    iNodeIndex = iNodeIndex + 1
                Next
                oXml.OutOfElem()

            End If

        End Sub

        Protected Overloads Overrides Sub SaveNetworkFile(ByRef oXml As AnimatTools.Interfaces.StdXml)
        End Sub

#Region " DataObject Methods "

#End Region

#End Region

    End Class

End Namespace
