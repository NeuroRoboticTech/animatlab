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

Namespace DataObjects.ProgramModules

    Public Class LinearInerconnectedNeuralNet
        Inherits AnimatTools.DataObjects.ProgramModule

#Region " Attributes "

#End Region

#Region " Properties "

        Public Overrides ReadOnly Property Description() As String
            Get
                Return "Creates a set of N neurons that are all interconnected using a sine function for thier synaptic strengths."
            End Get
        End Property

#End Region

#Region " Methods "

        Public Sub New(ByVal doParent As AnimatTools.Framework.DataObject)
            MyBase.New(doParent)

            m_strName = "Linear Interconnected Neural Network"
        End Sub

        Public Overrides Sub ShowDialog()
            Try
                Dim frmNet As New Forms.ProgramModules.LinearInterconnectedNeuralNet
                frmNet.ShowDialog()
            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try

        End Sub

#Region " DataObject Methods "

        Protected Overrides Sub BuildProperties()

        End Sub

        Public Overrides Function Clone(ByVal doParent As AnimatTools.Framework.DataObject, ByVal bCutData As Boolean, _
                                        ByVal doRoot As AnimatTools.Framework.DataObject) As AnimatTools.Framework.DataObject

        End Function

#End Region

#End Region

    End Class

End Namespace
