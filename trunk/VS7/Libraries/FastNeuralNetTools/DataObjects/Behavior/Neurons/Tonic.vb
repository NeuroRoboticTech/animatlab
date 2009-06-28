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

Namespace DataObjects.Behavior.Neurons

    Public Class Tonic
        Inherits Neurons.Normal

#Region " Attributes "

        Protected m_snIh As AnimatTools.Framework.ScaledNumber

#End Region

#Region " Properties "

        Public Overrides ReadOnly Property TypeName() As String
            Get
                Return "Tonic Firing Rate Neuron"
            End Get
        End Property

        Public Overridable Property Ih() As AnimatTools.Framework.ScaledNumber
            Get
                Return m_snIh
            End Get
            Set(ByVal Value As AnimatTools.Framework.ScaledNumber)
                m_snIh.CopyData(Value)
            End Set
        End Property

        Public Overrides ReadOnly Property NeuronType() As String
            Get
                Return "Tonic"
            End Get
        End Property

        Public Overrides ReadOnly Property ImageName() As String
            Get
                Return "FastNeuralNetTools.TonicNeuron.gif"
            End Get
        End Property

#End Region

#Region " Methods "

        Public Sub New(ByVal doParent As AnimatTools.Framework.DataObject)
            MyBase.New(doParent)

            Try

                m_snIh = New AnimatTools.Framework.ScaledNumber(Me, "Ih", 10, AnimatTools.Framework.ScaledNumber.enumNumericScale.nano, "Amps", "A")

                Shape = AnimatTools.DataObjects.Behavior.Node.enumShape.Ellipse
                Size = New SizeF(40, 40)
                Me.DrawColor = Color.Black
                Me.FillColor = Color.Green
                Me.TextColor = Color.White

                Dim myAssembly As System.Reflection.Assembly
                myAssembly = System.Reflection.Assembly.Load("FastNeuralNetTools")

                Me.Image = AnimatTools.Framework.ImageManager.LoadImage(myAssembly, "FastNeuralNetTools.TonicNeuron.gif", False)
                Me.Name = "Tonic Firing Rate Neuron"
                Me.Description = "A firing rate neuron type in the fast neural network library that provides a tonic firing frequency."

            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try

        End Sub

        Public Overrides Function Clone(ByVal doParent As AnimatTools.Framework.DataObject, ByVal bCutData As Boolean, _
                                        ByVal doRoot As AnimatTools.Framework.DataObject) As AnimatTools.Framework.DataObject
            Dim oNewNode As New Neurons.Tonic(doParent)
            oNewNode.CloneInternal(Me, bCutData, doRoot)
            If Not doRoot Is Nothing AndAlso doRoot Is Me Then oNewNode.AfterClone(Me, bCutData, doRoot, oNewNode)
            Return oNewNode
        End Function

        Protected Overrides Sub CloneInternal(ByVal doOriginal As AnimatTools.Framework.DataObject, ByVal bCutData As Boolean, _
                                            ByVal doRoot As AnimatTools.Framework.DataObject)
            MyBase.CloneInternal(doOriginal, bCutData, doRoot)

            Dim bnOrig As Neurons.Tonic = DirectCast(doOriginal, Neurons.Tonic)

            m_snIh = DirectCast(bnOrig.m_snIh.Clone(Me, bCutData, doRoot), ScaledNumber)
        End Sub

        Public Overrides Sub SaveNetwork(ByRef oXml As AnimatTools.Interfaces.StdXml, ByRef nmModule As AnimatTools.DataObjects.Behavior.NeuralModule)
            MyBase.SaveNetwork(oXml, nmModule)

            oXml.IntoElem() 'Into Neuron element

            oXml.AddChildElement("Ih", m_snIh.ActualValue)

            oXml.OutOfElem() 'Outof Neuron

        End Sub

#Region " DataObject Methods "

        Protected Overrides Sub BuildProperties()
            MyBase.BuildProperties()

            Dim pbNumberBag As Crownwood.Magic.Controls.PropertyBag = m_snIh.Properties
            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Ih", pbNumberBag.GetType(), "Ih", _
                                        "Neural Properties", "Sets the depolarizing current that raises the " & _
                                        "membrane potential and causes the neuron to fire.", pbNumberBag, _
                                        "", GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)))
        End Sub

        Public Overrides Sub ClearIsDirty()
            MyBase.ClearIsDirty()

            If Not m_snIh Is Nothing Then m_snIh.ClearIsDirty()
        End Sub

        Public Overrides Sub LoadData(ByRef oXml As AnimatTools.Interfaces.StdXml)
            MyBase.LoadData(oXml)

            oXml.IntoElem()

            m_snIh.LoadData(oXml, "Ih")

            oXml.OutOfElem()

        End Sub

        Public Overrides Sub SaveData(ByRef oXml As AnimatTools.Interfaces.StdXml)
            MyBase.SaveData(oXml)

            oXml.IntoElem() 'Into Node Element

            m_snIh.SaveData(oXml, "Ih")

            oXml.OutOfElem() ' Outof Node Element

        End Sub

#End Region

#End Region

    End Class

End Namespace

