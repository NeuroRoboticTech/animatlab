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

    Public Class NeuralModule
        Inherits AnimatTools.DataObjects.Behavior.NeuralModule

#Region " Attributes "

#End Region

#Region " Properties "

        Public Overrides Property Organism() As AnimatTools.DataObjects.Physical.Organism
            Get
                Return m_doOrganism
            End Get
            Set(ByVal Value As AnimatTools.DataObjects.Physical.Organism)
                m_doOrganism = Value
            End Set
        End Property

        Public Overrides ReadOnly Property NetworkFilename() As String
            Get
                If Not m_doOrganism Is Nothing Then
                    Return m_doOrganism.Name & ".afnn"
                Else
                    Return ""
                End If
            End Get
        End Property

        Public Overrides ReadOnly Property ModuleFilename() As String
            Get
                'If we are saving a robot control file then we will need to 
                'use the CE versions of the libraries instead of the normal windows versions
                If Util.RobotOrganism Is Nothing Then
                    If Util.Simulation.UseReleaseLibraries Then
                        Return "FastNeuralNet_vc7.dll"
                    Else
                        Return "FastNeuralNet_vc7D.dll"
                    End If
                Else
                    If Util.Simulation.UseReleaseLibraries Then
                        Return "FastNeuralNet_CE.dll"
                    Else
                        Return "FastNeuralNet_CED.dll"
                    End If
                End If

            End Get
        End Property

#End Region

#Region " Methods "

        Public Sub New(ByVal doParent As AnimatTools.Framework.DataObject)
            MyBase.New(doParent)

            m_strModuleName = "FastNeuralNet"
            m_strModuleType = "FastNeuralModule"
        End Sub

        Public Overrides Function Clone(ByVal doParent As AnimatTools.Framework.DataObject, ByVal bCutData As Boolean, _
                                        ByVal doRoot As AnimatTools.Framework.DataObject) As AnimatTools.Framework.DataObject
            Dim oNewModule As New DataObjects.Behavior.NeuralModule(doParent)
            oNewModule.CloneInternal(Me, bCutData, doRoot)
            If Not doRoot Is Nothing AndAlso doRoot Is Me Then oNewModule.AfterClone(Me, bCutData, doRoot, oNewModule)
            Return oNewModule
        End Function

        Protected Overloads Overrides Sub SaveNetworkFile(ByRef oXml As AnimatTools.Interfaces.StdXml)

            oXml.AddChildElement("TimeStep", Me.TimeStep.ActualValue)
            Util.SaveVector(oXml, "NetworkSize", New AnimatTools.Framework.Vec3i(Me, m_aryNodes.Count, 1, 1))

            'First we need to go through and set the neuron indexes for all of the neurons in this module.
            Dim bnNode As DataObjects.Behavior.Neurons.Normal
            Dim iNeuronIndex As Integer = 0
            For Each deEntry As DictionaryEntry In m_aryNodes
                If Util.IsTypeOf(deEntry.Value.GetType(), GetType(DataObjects.Behavior.Neurons.Normal), False) Then
                    bnNode = DirectCast(deEntry.Value, DataObjects.Behavior.Neurons.Normal)
                    bnNode.NodeIndex = iNeuronIndex
                    iNeuronIndex = iNeuronIndex + 1
                Else
                    Throw New System.Exception("There was a node in the fast neural module that was not derived from a normal neuron?")
                End If
            Next

            'Now we can save the neurons
            oXml.AddChildElement("Neurons")
            oXml.IntoElem()
            For Each deEntry As DictionaryEntry In m_aryNodes
                bnNode = DirectCast(deEntry.Value, DataObjects.Behavior.Neurons.Normal)
                bnNode.SaveNetwork(oXml, Me)
            Next
            oXml.OutOfElem()

        End Sub

#Region " DataObject Methods "

#End Region

#End Region

    End Class

End Namespace
