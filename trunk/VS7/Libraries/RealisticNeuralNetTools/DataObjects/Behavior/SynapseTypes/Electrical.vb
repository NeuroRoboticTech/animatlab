Imports System
Imports System.Drawing
Imports System.Collections
Imports System.ComponentModel
Imports System.ComponentModel.Design.Serialization
Imports System.Windows.Forms
Imports System.Diagnostics
Imports System.IO
Imports System.Xml
Imports Crownwood.Magic.Controls
Imports AnimatTools.Framework

Namespace DataObjects.Behavior.SynapseTypes

    Public Class Electrical
        Inherits DataObjects.Behavior.SynapseType

#Region " Attributes "

        Protected m_snLowCoupling As AnimatTools.Framework.ScaledNumber
        Protected m_snHighCoupling As AnimatTools.Framework.ScaledNumber
        Protected m_snTurnOnThreshold As AnimatTools.Framework.ScaledNumber
        Protected m_snTurnOnSaturate As AnimatTools.Framework.ScaledNumber

#End Region

#Region " Properties "

        <Browsable(False)> _
        Public Overrides ReadOnly Property TypeName() As String
            Get
                Return "Electrical Synapse"
            End Get
        End Property

        <Browsable(False)> _
        Public Overridable Property LowCoupling() As AnimatTools.Framework.ScaledNumber
            Get
                Return m_snLowCoupling
            End Get
            Set(ByVal Value As AnimatTools.Framework.ScaledNumber)
                If Value.ActualValue < 0 OrElse Value.ActualValue > 0.0001 Then
                    Throw New System.Exception("The low coupling must be between the range 0 to 100 uS.")
                End If

                m_snLowCoupling.CopyData(Value)
            End Set
        End Property

        <Browsable(False)> _
        Public Overridable Property HighCoupling() As AnimatTools.Framework.ScaledNumber
            Get
                Return m_snHighCoupling
            End Get
            Set(ByVal Value As AnimatTools.Framework.ScaledNumber)
                If Value.ActualValue < 0 OrElse Value.ActualValue > 0.0001 Then
                    Throw New System.Exception("The high coupling must be between the range 0 to 100 uS.")
                End If

                m_snHighCoupling.CopyData(Value)
            End Set
        End Property

        <Browsable(False)> _
        Public Overridable Property TurnOnThreshold() As AnimatTools.Framework.ScaledNumber
            Get
                Return m_snTurnOnThreshold
            End Get
            Set(ByVal Value As AnimatTools.Framework.ScaledNumber)
                If Value.ActualValue < -0.1 OrElse Value.ActualValue > 0.3 Then
                    Throw New System.Exception("The turn on junctional threshold must be between the range -100 to 300 mV.")
                End If

                m_snTurnOnThreshold.CopyData(Value)
            End Set
        End Property

        <Browsable(False)> _
        Public Overridable Property TurnOnSaturate() As AnimatTools.Framework.ScaledNumber
            Get
                Return m_snTurnOnSaturate
            End Get
            Set(ByVal Value As AnimatTools.Framework.ScaledNumber)
                If Value.ActualValue < -0.1 OrElse Value.ActualValue > 0.3 Then
                    Throw New System.Exception("The saturation junctional potential must be between the range -100 to 300 mV.")
                End If

                m_snTurnOnSaturate.CopyData(Value)
            End Set
        End Property

        <Browsable(False)> _
        Public Overrides ReadOnly Property SynapseType() As Integer
            Get
                Return 2 'For electrical synapse
            End Get
        End Property

        <Browsable(False)> _
        Public Overrides ReadOnly Property ImageName() As String
            Get
                Return "RealisticNeuralNetTools.ElectricalSynapse.gif"
            End Get
        End Property

#End Region

#Region " Methods "

        Public Sub New(ByVal doParent As AnimatTools.Framework.DataObject)
            MyBase.New(doParent)

            m_snLowCoupling = New AnimatTools.Framework.ScaledNumber(Me, "LowCoupling", 0.2, AnimatTools.Framework.ScaledNumber.enumNumericScale.micro, "Siemens", "S")
            m_snHighCoupling = New AnimatTools.Framework.ScaledNumber(Me, "HighCoupling", 0.2, AnimatTools.Framework.ScaledNumber.enumNumericScale.micro, "Siemens", "S")
            m_snTurnOnThreshold = New AnimatTools.Framework.ScaledNumber(Me, "TurnOnThreshold", 0, AnimatTools.Framework.ScaledNumber.enumNumericScale.milli, "Volts", "V")
            m_snTurnOnSaturate = New AnimatTools.Framework.ScaledNumber(Me, "TurnOnSaturate", 0, AnimatTools.Framework.ScaledNumber.enumNumericScale.milli, "Volts", "V")

            Dim myAssembly As System.Reflection.Assembly
            myAssembly = System.Reflection.Assembly.Load("RealisticNeuralNetTools")

            Me.Image = AnimatTools.Framework.ImageManager.LoadImage(myAssembly, "RealisticNeuralNetTools.ElectricalSynapse.gif", False)
            Me.Name = "Electrical Synapse"

            Me.Font = New Font("Arial", 12)
            Me.Description = "Adds an electrical synapse between two neurons. An electrical synapse is modelled as a non-specific " & _
                            "electrical conductance linking two neurons. Current therefore flows from one neuron to the other whenever " & _
                            "there is a difference between the membrane potentials of the two neurons and the junctional (electrical synapse) " & _
                            "conductance is greater than zero.  There are two types of electrical synapse."


        End Sub

        Public Overrides Function Clone(ByVal doParent As AnimatTools.Framework.DataObject, ByVal bCutData As Boolean, _
                                        ByVal doRoot As AnimatTools.Framework.DataObject) As AnimatTools.Framework.DataObject
            Dim oNewLink As New Electrical(doParent)
            oNewLink.CloneInternal(Me, bCutData, doRoot)
            If Not doRoot Is Nothing AndAlso doRoot Is Me Then oNewLink.AfterClone(Me, bCutData, doRoot, oNewLink)
            Return oNewLink
        End Function

        Protected Overrides Sub CloneInternal(ByVal doOriginal As AnimatTools.Framework.DataObject, ByVal bCutData As Boolean, _
                                            ByVal doRoot As AnimatTools.Framework.DataObject)
            MyBase.CloneInternal(doOriginal, bCutData, doRoot)

            Dim blOrig As SynapseTypes.Electrical = DirectCast(doOriginal, SynapseTypes.Electrical)

            m_snLowCoupling = DirectCast(blOrig.m_snLowCoupling.Clone(Me, bCutData, doRoot), ScaledNumber)
            m_snHighCoupling = DirectCast(blOrig.m_snHighCoupling.Clone(Me, bCutData, doRoot), ScaledNumber)
            m_snTurnOnThreshold = DirectCast(blOrig.m_snTurnOnThreshold.Clone(Me, bCutData, doRoot), ScaledNumber)
            m_snTurnOnSaturate = DirectCast(blOrig.m_snTurnOnSaturate.Clone(Me, bCutData, doRoot), ScaledNumber)

        End Sub

        Public Overrides Sub SaveNetwork(ByRef oXml As AnimatTools.Interfaces.StdXml, ByRef nmModule As AnimatTools.DataObjects.Behavior.NeuralModule)

            oXml.AddChildElement("ElectricalSynapse")
            oXml.IntoElem()

            oXml.AddChildElement("Name", m_strName)
            oXml.AddChildElement("ID", Me.LinkIndex)
            oXml.AddChildElement("LowCoup", m_snLowCoupling.ValueFromDefaultScale)
            oXml.AddChildElement("HiCoup", m_snHighCoupling.ValueFromDefaultScale)
            oXml.AddChildElement("TurnOnV", m_snTurnOnThreshold.ValueFromDefaultScale)
            oXml.AddChildElement("SaturateV", m_snTurnOnSaturate.ValueFromDefaultScale)

            oXml.OutOfElem() 'Outof Neuron

        End Sub

#Region " DataObject Methods "

        Protected Overrides Sub BuildProperties()
            MyBase.BuildProperties()

            ''Now lets add the properties for this neuron
            Dim pbNumberBag As Crownwood.Magic.Controls.PropertyBag = m_snLowCoupling.Properties
            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Low Coupling", pbNumberBag.GetType(), "LowCoupling", _
                                        "Synapse Properties", "Sets the minimum coupling conductance for this synaptic type. " & _
                                        "Acceptable values are in the range 0 to 100 uS.", pbNumberBag, _
                                        "", GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)))

            pbNumberBag = m_snHighCoupling.Properties
            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("High Coupling", pbNumberBag.GetType(), "HighCoupling", _
                                        "Synapse Properties", "Sets  the maximum coupling conductance (which is present when the junctional " & _
                                        "potential exceeds the saturation voltage) for this synaptic type. " & _
                                        "Acceptable values are in the range 0 to 100 uS.", _
                                        pbNumberBag, "", GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)))

            pbNumberBag = m_snTurnOnThreshold.Properties
            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Turn-On Threshold", pbNumberBag.GetType(), "TurnOnThreshold", _
                                        "Synapse Properties", "Sets the threshold junctional potential at which the junctional conductance " & _
                                        "defined above starts to increase above the minimum level. " & _
                                        "Acceptable values are in the range -100 to 300 mV.", pbNumberBag, _
                                        "", GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)))

            pbNumberBag = m_snTurnOnSaturate.Properties
            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Turn-On Saturate", pbNumberBag.GetType(), "TurnOnSaturate", _
                                        "Synapse Properties", "Sets the junctional potential at which the junctional conductance defined above " & _
                                        "reaches its maximum value Acceptable values are in the range -100 to 300 mV.", _
                                        pbNumberBag, "", GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)))

        End Sub

        Public Overrides Sub ClearIsDirty()
            MyBase.ClearIsDirty()

            If Not m_snLowCoupling Is Nothing Then m_snLowCoupling.ClearIsDirty()
            If Not m_snHighCoupling Is Nothing Then m_snHighCoupling.ClearIsDirty()
            If Not m_snTurnOnThreshold Is Nothing Then m_snTurnOnThreshold.ClearIsDirty()
            If Not m_snTurnOnSaturate Is Nothing Then m_snTurnOnSaturate.ClearIsDirty()
        End Sub

        Public Overrides Sub LoadData(ByRef oXml As AnimatTools.Interfaces.StdXml)
            MyBase.LoadData(oXml)

            oXml.IntoElem()

            m_snLowCoupling.LoadData(oXml, "LowCoupling")
            m_snHighCoupling.LoadData(oXml, "HighCoupling")
            m_snTurnOnThreshold.LoadData(oXml, "TurnOnThreshold")
            m_snTurnOnSaturate.LoadData(oXml, "TurnOnSaturate")

            oXml.OutOfElem()

        End Sub

        Public Overrides Sub SaveData(ByRef oXml As AnimatTools.Interfaces.StdXml)

            Try
                MyBase.SaveData(oXml)

                oXml.IntoElem() 'Into Node Element

                m_snLowCoupling.SaveData(oXml, "LowCoupling")
                m_snHighCoupling.SaveData(oXml, "HighCoupling")
                m_snTurnOnThreshold.SaveData(oXml, "TurnOnThreshold")
                m_snTurnOnSaturate.SaveData(oXml, "TurnOnSaturate")

                oXml.OutOfElem() ' Outof Node Element
            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try

        End Sub

#End Region

#End Region

    End Class

End Namespace
