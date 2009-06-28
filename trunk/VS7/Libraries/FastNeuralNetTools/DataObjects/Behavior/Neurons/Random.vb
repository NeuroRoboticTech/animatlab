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

    Public Class Random
        Inherits Neurons.Normal

#Region " Attributes "

        Protected m_snIl As AnimatTools.Framework.ScaledNumber
        Protected m_gnCurrentDistribution As AnimatTools.DataObjects.Gain
        Protected m_gnBurstLengthDistribution As AnimatTools.DataObjects.Gain
        Protected m_gnInterbusrtLengthDistribution As AnimatTools.DataObjects.Gain

#End Region

#Region " Properties "

        Public Overrides ReadOnly Property TypeName() As String
            Get
                Return "Random Firing Rate Neuron"
            End Get
        End Property

        Public Overridable Property Il() As AnimatTools.Framework.ScaledNumber
            Get
                Return m_snIl
            End Get
            Set(ByVal Value As AnimatTools.Framework.ScaledNumber)
                m_snIl.CopyData(Value)
            End Set
        End Property

        Public Overridable Property CurrentDistribution() As AnimatTools.DataObjects.Gain
            Get
                Return m_gnCurrentDistribution
            End Get
            Set(ByVal Value As AnimatTools.DataObjects.Gain)
                If Not m_gnCurrentDistribution Is Nothing Then m_gnCurrentDistribution.ParentData = Nothing
                m_gnCurrentDistribution = Value
                If Not m_gnCurrentDistribution Is Nothing Then
                    m_gnCurrentDistribution.ParentData = Me
                    m_gnCurrentDistribution.GainPropertyName = "CurrentDistribution"
                End If
            End Set
        End Property

        Public Overridable Property BurstLengthDistribution() As AnimatTools.DataObjects.Gain
            Get
                Return m_gnBurstLengthDistribution
            End Get
            Set(ByVal Value As AnimatTools.DataObjects.Gain)
                If Not m_gnBurstLengthDistribution Is Nothing Then m_gnBurstLengthDistribution.ParentData = Nothing
                m_gnBurstLengthDistribution = Value
                If Not m_gnBurstLengthDistribution Is Nothing Then
                    m_gnBurstLengthDistribution.ParentData = Me
                    m_gnBurstLengthDistribution.GainPropertyName = "BurstLengthDistribution"
                End If
            End Set
        End Property

        Public Overridable Property InterbusrtLengthDistribution() As AnimatTools.DataObjects.Gain
            Get
                Return m_gnInterbusrtLengthDistribution
            End Get
            Set(ByVal Value As AnimatTools.DataObjects.Gain)
                If Not m_gnInterbusrtLengthDistribution Is Nothing Then m_gnInterbusrtLengthDistribution.ParentData = Nothing
                m_gnInterbusrtLengthDistribution = Value
                If Not m_gnInterbusrtLengthDistribution Is Nothing Then
                    m_gnInterbusrtLengthDistribution.ParentData = Me
                    m_gnInterbusrtLengthDistribution.GainPropertyName = "InterbusrtLengthDistribution"
                End If
            End Set
        End Property

        Public Overrides ReadOnly Property NeuronType() As String
            Get
                Return "Random"
            End Get
        End Property

        Public Overrides ReadOnly Property ImageName() As String
            Get
                Return "FastNeuralNetTools.RandomNeuron.gif"
            End Get
        End Property

#End Region

#Region " Methods "

        Public Sub New(ByVal doParent As AnimatTools.Framework.DataObject)
            MyBase.New(doParent)

            Try

                Shape = AnimatTools.DataObjects.Behavior.Node.enumShape.Ellipse
                Size = New SizeF(40, 40)
                Me.DrawColor = Color.Black
                Me.FillColor = Color.Red

                Dim myAssembly As System.Reflection.Assembly
                myAssembly = System.Reflection.Assembly.Load("FastNeuralNetTools")

                Me.Image = AnimatTools.Framework.ImageManager.LoadImage(myAssembly, "FastNeuralNetTools.RandomNeuron.gif", False)
                Me.Name = "Random Firing Rate Neuron"
                Me.Description = "A firing rate neuron type in the fast neural network library that provides random firing frequencies."

                m_snIl = New AnimatTools.Framework.ScaledNumber(Me, "Il", 0, AnimatTools.Framework.ScaledNumber.enumNumericScale.nano, "Amps", "A")
                m_gnCurrentDistribution = New AnimatTools.DataObjects.Gains.Polynomial(Me, "CurrentDistribution", "Random Variable", "Amps", False, False, False)
                m_gnBurstLengthDistribution = New AnimatTools.DataObjects.Gains.Polynomial(Me, "BurstLengthDistribution", "Random Variable", "Seconds", False, False, False)
                m_gnInterbusrtLengthDistribution = New AnimatTools.DataObjects.Gains.Polynomial(Me, "InterbusrtLengthDistribution", "Random Variable", "Seconds", False, False, False)


                m_gnCurrentDistribution.LowerLimit.Value = 0
                m_gnCurrentDistribution.UpperLimit.Value = 100
                m_gnBurstLengthDistribution.LowerLimit.Value = 0
                m_gnBurstLengthDistribution.UpperLimit.Value = 100
                m_gnInterbusrtLengthDistribution.LowerLimit.Value = 0
                m_gnInterbusrtLengthDistribution.UpperLimit.Value = 100

            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try

        End Sub

        Public Overrides Function Clone(ByVal doParent As AnimatTools.Framework.DataObject, ByVal bCutData As Boolean, _
                                        ByVal doRoot As AnimatTools.Framework.DataObject) As AnimatTools.Framework.DataObject
            Dim oNewNode As New Neurons.Random(doParent)
            oNewNode.CloneInternal(Me, bCutData, doRoot)
            If Not doRoot Is Nothing AndAlso doRoot Is Me Then oNewNode.AfterClone(Me, bCutData, doRoot, oNewNode)
            Return oNewNode
        End Function

        Protected Overrides Sub CloneInternal(ByVal doOriginal As AnimatTools.Framework.DataObject, ByVal bCutData As Boolean, _
                                            ByVal doRoot As AnimatTools.Framework.DataObject)
            MyBase.CloneInternal(doOriginal, bCutData, doRoot)

            Dim bnOrig As Neurons.Random = DirectCast(doOriginal, Neurons.Random)

            m_snIl = DirectCast(bnOrig.m_snIl.Clone(Me, bCutData, doRoot), ScaledNumber)
            m_gnCurrentDistribution = DirectCast(bnOrig.m_gnCurrentDistribution.Clone(Me, bCutData, doRoot), AnimatTools.DataObjects.Gain)
            m_gnBurstLengthDistribution = DirectCast(bnOrig.m_gnBurstLengthDistribution.Clone(Me, bCutData, doRoot), AnimatTools.DataObjects.Gain)
            m_gnInterbusrtLengthDistribution = DirectCast(bnOrig.m_gnInterbusrtLengthDistribution.Clone(Me, bCutData, doRoot), AnimatTools.DataObjects.Gain)
        End Sub

        Public Overrides Sub SaveNetwork(ByRef oXml As AnimatTools.Interfaces.StdXml, ByRef nmModule As AnimatTools.DataObjects.Behavior.NeuralModule)
            MyBase.SaveNetwork(oXml, nmModule)

            oXml.IntoElem() 'Into Neuron element

            oXml.AddChildElement("Il", m_snIl.ActualValue)
            m_gnCurrentDistribution.SaveNetwork(oXml, "CurrentGraph")
            m_gnBurstLengthDistribution.SaveNetwork(oXml, "BurstGraph")
            m_gnInterbusrtLengthDistribution.SaveNetwork(oXml, "InterBurstGraph")

            oXml.OutOfElem() 'Outof Neuron

        End Sub

#Region " DataObject Methods "

        Protected Overrides Sub BuildProperties()
            MyBase.BuildProperties()

            Dim pbNumberBag As Crownwood.Magic.Controls.PropertyBag = m_snIl.Properties
            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Il", pbNumberBag.GetType(), "Il", _
                                        "Neural Properties", "Sets the hyperpolarizing current that brings the " & _
                                        "membrane potential back down after it has been firing.", pbNumberBag, _
                                        "", GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)))

            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Current Distribution", GetType(AnimatTools.DataObjects.Gain), "CurrentDistribution", _
                                        "Neural Properties", "Sets the gain that controls the probability distribution " & _
                                        "for the size of the random current.", m_gnCurrentDistribution, _
                                        GetType(AnimatTools.TypeHelpers.GainTypeEditor), _
                                        GetType(AnimatTools.TypeHelpers.GainTypeConverter)))

            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Burst Length", GetType(AnimatTools.DataObjects.Gain), "BurstLengthDistribution", _
                                        "Neural Properties", "Sets the gain that controls the probability distribution " & _
                                        "for the length of the random burst.", m_gnBurstLengthDistribution, _
                                        GetType(AnimatTools.TypeHelpers.GainTypeEditor), _
                                        GetType(AnimatTools.TypeHelpers.GainTypeConverter)))

            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("InterBurst Length", GetType(AnimatTools.DataObjects.Gain), "InterbusrtLengthDistribution", _
                                        "Neural Properties", "Sets the gain that controls the probability distribution " & _
                                        "for the length between bursts.", m_gnInterbusrtLengthDistribution, _
                                        GetType(AnimatTools.TypeHelpers.GainTypeEditor), _
                                        GetType(AnimatTools.TypeHelpers.GainTypeConverter)))

        End Sub

        Public Overrides Sub ClearIsDirty()
            MyBase.ClearIsDirty()

            If Not m_snIl Is Nothing Then m_snIl.ClearIsDirty()
            If Not m_gnCurrentDistribution Is Nothing Then m_gnCurrentDistribution.ClearIsDirty()
            If Not m_gnBurstLengthDistribution Is Nothing Then m_gnBurstLengthDistribution.ClearIsDirty()
            If Not m_gnInterbusrtLengthDistribution Is Nothing Then m_gnInterbusrtLengthDistribution.ClearIsDirty()
        End Sub

        Public Overrides Sub LoadData(ByRef oXml As AnimatTools.Interfaces.StdXml)
            MyBase.LoadData(oXml)

            oXml.IntoElem()

            m_snIl.LoadData(oXml, "Il")

            If oXml.FindChildElement("CurrentDistribution", False) Then
                oXml.IntoChildElement("CurrentDistribution")
                Dim strAssemblyFile As String = oXml.GetChildString("AssemblyFile")
                Dim strClassName As String = oXml.GetChildString("ClassName")
                oXml.OutOfElem()

                m_gnCurrentDistribution = DirectCast(Util.LoadClass(strAssemblyFile, strClassName, Me), AnimatTools.DataObjects.Gain)
                m_gnCurrentDistribution.LoadData(oXml, "CurrentDistribution", "CurrentDistribution")
            End If

            If oXml.FindChildElement("BurstLengthDistribution", False) Then
                oXml.IntoChildElement("BurstLengthDistribution")
                Dim strAssemblyFile As String = oXml.GetChildString("AssemblyFile")
                Dim strClassName As String = oXml.GetChildString("ClassName")
                oXml.OutOfElem()

                m_gnBurstLengthDistribution = DirectCast(Util.LoadClass(strAssemblyFile, strClassName, Me), AnimatTools.DataObjects.Gain)
                m_gnBurstLengthDistribution.LoadData(oXml, "BurstLengthDistribution", "BurstLengthDistribution")
            End If

            If oXml.FindChildElement("InterbusrtLengthDistribution", False) Then
                oXml.IntoChildElement("InterbusrtLengthDistribution")
                Dim strAssemblyFile As String = oXml.GetChildString("AssemblyFile")
                Dim strClassName As String = oXml.GetChildString("ClassName")
                oXml.OutOfElem()

                m_gnInterbusrtLengthDistribution = DirectCast(Util.LoadClass(strAssemblyFile, strClassName, Me), AnimatTools.DataObjects.Gain)
                m_gnInterbusrtLengthDistribution.LoadData(oXml, "InterbusrtLengthDistribution", "InterbusrtLengthDistribution")
            End If

            oXml.OutOfElem()

        End Sub

        Public Overrides Sub SaveData(ByRef oXml As AnimatTools.Interfaces.StdXml)
            MyBase.SaveData(oXml)

            oXml.IntoElem() 'Into Node Element

            m_snIl.SaveData(oXml, "Il")
            m_gnCurrentDistribution.SaveData(oXml, "CurrentDistribution")
            m_gnBurstLengthDistribution.SaveData(oXml, "BurstLengthDistribution")
            m_gnInterbusrtLengthDistribution.SaveData(oXml, "InterbusrtLengthDistribution")

            oXml.OutOfElem() ' Outof Node Element

        End Sub

#End Region

#End Region

    End Class

End Namespace

