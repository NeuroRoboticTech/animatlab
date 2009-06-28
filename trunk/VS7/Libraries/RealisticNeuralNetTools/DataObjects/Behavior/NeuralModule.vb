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

        Protected m_snAHPEquilibriumPotential As AnimatTools.Framework.ScaledNumber
        Protected m_snSpikePeak As AnimatTools.Framework.ScaledNumber
        Protected m_fltSpikeStrength As Single
        Protected m_snCaEquilibriumPotential As AnimatTools.Framework.ScaledNumber
        Protected m_snRefractoryPeriod As AnimatTools.Framework.ScaledNumber
        Protected m_bUseCriticalPeriod As Boolean
        Protected m_snStartCriticalPeriod As AnimatTools.Framework.ScaledNumber
        Protected m_snEndCriticalPeriod As AnimatTools.Framework.ScaledNumber
        Protected m_bTTX As Boolean
        Protected m_bCd As Boolean
        Protected m_bHodgkinHuxley As Boolean

        Protected m_arySynapseTypes As New Collections.SynapseTypes(Me)

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
                    Return m_doOrganism.Name & ".arnn"
                End If
            End Get
        End Property

        Public Overridable Property AHPEquilibriumPotential() As AnimatTools.Framework.ScaledNumber
            Get
                Return m_snAHPEquilibriumPotential
            End Get
            Set(ByVal Value As AnimatTools.Framework.ScaledNumber)
                If Value.ActualValue < -0.15 OrElse Value.ActualValue > -0.01 Then
                    Throw New System.Exception("The after-hyperpolarizing equilibrium potential must be between the range -150 to -10 mV.")
                End If

                m_snAHPEquilibriumPotential.CopyData(Value)
            End Set
        End Property

        Public Overridable Property SpikePeak() As AnimatTools.Framework.ScaledNumber
            Get
                Return m_snSpikePeak
            End Get
            Set(ByVal Value As AnimatTools.Framework.ScaledNumber)
                If Value.ActualValue < -0.03 OrElse Value.ActualValue > 0.05 Then
                    Throw New System.Exception("The spike peak must be between the range -30 to 50 mV.")
                End If

                m_snSpikePeak.CopyData(Value)
            End Set
        End Property

        Public Overridable Property SpikeStrength() As Single
            Get
                Return m_fltSpikeStrength
            End Get
            Set(ByVal Value As Single)
                If Value < 1 OrElse Value > 1000 Then
                    Throw New System.Exception("The spike strength must be between the range 1 to 1000.")
                End If

                m_fltSpikeStrength = Value
            End Set
        End Property

        Public Overridable Property CaEquilibriumPotential() As AnimatTools.Framework.ScaledNumber
            Get
                Return m_snCaEquilibriumPotential
            End Get
            Set(ByVal Value As AnimatTools.Framework.ScaledNumber)
                If Value.ActualValue < -0.1 OrElse Value.ActualValue > 0.5 Then
                    Throw New System.Exception("The calcium equilibrium potential must be between the range -100 to 500 mV.")
                End If

                m_snCaEquilibriumPotential.CopyData(Value)
            End Set
        End Property

        Public Overridable Property RefractoryPeriod() As AnimatTools.Framework.ScaledNumber
            Get
                Return m_snRefractoryPeriod
            End Get
            Set(ByVal Value As AnimatTools.Framework.ScaledNumber)
                If Value.ActualValue < 0.001 OrElse Value.ActualValue > 0.05 Then
                    Throw New System.Exception("The absolute refractory period must be between the range 1 to 50 ms.")
                End If

                m_snRefractoryPeriod.CopyData(Value)
            End Set
        End Property

        Public Overridable Property UseCriticalPeriod() As Boolean
            Get
                Return m_bUseCriticalPeriod
            End Get
            Set(ByVal Value As Boolean)
                m_bUseCriticalPeriod = Value
            End Set
        End Property

        Public Overridable Property ApplyTTX() As Boolean
            Get
                Return m_bTTX
            End Get
            Set(ByVal Value As Boolean)
                m_bTTX = Value
            End Set
        End Property

        Public Overridable Property ApplyCd() As Boolean
            Get
                Return m_bCd
            End Get
            Set(ByVal Value As Boolean)
                m_bCd = Value
            End Set
        End Property

        Public Overridable Property HodgkinHuxley() As Boolean
            Get
                Return m_bHodgkinHuxley
            End Get
            Set(ByVal Value As Boolean)
                m_bHodgkinHuxley = Value
            End Set
        End Property

        Public Overridable Property StartCriticalPeriod() As AnimatTools.Framework.ScaledNumber
            Get
                Return m_snStartCriticalPeriod
            End Get
            Set(ByVal Value As AnimatTools.Framework.ScaledNumber)
                If Value.ActualValue < 0 OrElse Value.ActualValue > 1000 Then
                    Throw New System.Exception("The critical period start time must be between the range 0 to 10000 s.")
                End If

                If Value.ActualValue >= m_snEndCriticalPeriod.ActualValue Then
                    Throw New System.Exception("The critical period start time must be less than the end time.")
                End If

                m_snStartCriticalPeriod.CopyData(Value)
            End Set
        End Property

        Public Overridable Property EndCriticalPeriod() As AnimatTools.Framework.ScaledNumber
            Get
                Return m_snStartCriticalPeriod
            End Get
            Set(ByVal Value As AnimatTools.Framework.ScaledNumber)
                If Value.ActualValue < 0 OrElse Value.ActualValue > 1000 Then
                    Throw New System.Exception("The critical period end time must be between the range 0 to 10000 s.")
                End If

                If Value.ActualValue <= m_snStartCriticalPeriod.ActualValue Then
                    Throw New System.Exception("The critical period end time must be greater than the start time.")
                End If

                m_snEndCriticalPeriod.CopyData(Value)
            End Set
        End Property

        Public Overridable ReadOnly Property SynapseTypes() As Collections.SynapseTypes
            Get
                Return m_arySynapseTypes
            End Get
        End Property

        Public Overrides ReadOnly Property ModuleFilename() As String
            Get
                'If we are saving a robot control file then we will need to 
                'use the CE versions of the libraries instead of the normal windows versions
                If Util.RobotOrganism Is Nothing Then
                    If Util.Simulation.UseReleaseLibraries Then
                        Return "RealisticNeuralNet_vc7.dll"
                    Else
                        Return "RealisticNeuralNet_vc7D.dll"
                    End If
                Else
                    If Util.Simulation.UseReleaseLibraries Then
                        Return "RealisticNeuralNet_CE.dll"
                    Else
                        Return "RealisticNeuralNet_CED.dll"
                    End If
                End If
            End Get
        End Property

#End Region

#Region " Methods "

        Public Sub New(ByVal doParent As AnimatTools.Framework.DataObject)
            MyBase.New(doParent)

            m_strModuleName = "RealisticNeuralNet"
            'm_strModuleFilename = "RealisticNeuralNet_vc7.dll"
            m_strModuleType = "RealisticNeuralModule"

            m_snTimeStep = New AnimatTools.Framework.ScaledNumber(Me, "TimeStep", 0.2, AnimatTools.Framework.ScaledNumber.enumNumericScale.milli, "seconds", "s")
            m_snAHPEquilibriumPotential = New AnimatTools.Framework.ScaledNumber(Me, "AHPEquilibriumPotential", -70, AnimatTools.Framework.ScaledNumber.enumNumericScale.milli, "Volts", "V")
            m_snSpikePeak = New AnimatTools.Framework.ScaledNumber(Me, "SpikePeak", 0, AnimatTools.Framework.ScaledNumber.enumNumericScale.milli, "Volts", "V")
            m_fltSpikeStrength = 1
            m_snCaEquilibriumPotential = New AnimatTools.Framework.ScaledNumber(Me, "CaEquilibriumPotential", 200, AnimatTools.Framework.ScaledNumber.enumNumericScale.milli, "Volts", "V")
            m_snRefractoryPeriod = New AnimatTools.Framework.ScaledNumber(Me, "RefractoryPeriod", 2, AnimatTools.Framework.ScaledNumber.enumNumericScale.milli, "seconds", "s")
            m_bUseCriticalPeriod = False
            m_snStartCriticalPeriod = New AnimatTools.Framework.ScaledNumber(Me, "StartCriticalPeriod", 0, AnimatTools.Framework.ScaledNumber.enumNumericScale.None, "seconds", "s")
            m_snEndCriticalPeriod = New AnimatTools.Framework.ScaledNumber(Me, "EndCriticalPeriod", 5, AnimatTools.Framework.ScaledNumber.enumNumericScale.None, "seconds", "s")
            m_bTTX = False
            m_bCd = False
            m_bHodgkinHuxley = False

            'Now lets create the default link types.
            Dim slSynapse As DataObjects.Behavior.SynapseType

            Dim scSynapse As DataObjects.Behavior.SynapseTypes.SpikingChemical
            slSynapse = New DataObjects.Behavior.SynapseTypes.SpikingChemical(Me)
            slSynapse.ArrowDestination.Style = AnimatTools.DataObjects.Behavior.Link.enumArrowStyle.Fork
            slSynapse.Name = "Nicotinic ACh"
            slSynapse.NeuralModule = Me
            m_arySynapseTypes.Add(slSynapse.ID, slSynapse)

            slSynapse = New DataObjects.Behavior.SynapseTypes.SpikingChemical(Me)
            scSynapse = DirectCast(slSynapse, DataObjects.Behavior.SynapseTypes.SpikingChemical)
            scSynapse.ArrowDestination.Style = AnimatTools.DataObjects.Behavior.Link.enumArrowStyle.Circle
            scSynapse.ArrowDestination.Filled = True
            scSynapse.Name = "Hyperpolarizing IPSP"
            scSynapse.EquilibriumPotential.ValueManual = -70
            scSynapse.DecayRate.ValueManual = 10
            slSynapse.NeuralModule = Me
            m_arySynapseTypes.Add(slSynapse.ID, slSynapse)

            slSynapse = New DataObjects.Behavior.SynapseTypes.SpikingChemical(Me)
            scSynapse = DirectCast(slSynapse, DataObjects.Behavior.SynapseTypes.SpikingChemical)
            scSynapse.ArrowDestination.Style = AnimatTools.DataObjects.Behavior.Link.enumArrowStyle.Circle
            scSynapse.ArrowDestination.Filled = False
            scSynapse.Name = "Depolarizing IPSP"
            scSynapse.EquilibriumPotential.ValueManual = -55
            scSynapse.RelativeFacilitation.ValueManual = 1
            slSynapse.NeuralModule = Me
            m_arySynapseTypes.Add(slSynapse.ID, slSynapse)

            slSynapse = New DataObjects.Behavior.SynapseTypes.SpikingChemical(Me)
            scSynapse = DirectCast(slSynapse, DataObjects.Behavior.SynapseTypes.SpikingChemical)
            scSynapse.ArrowDestination.Style = AnimatTools.DataObjects.Behavior.Link.enumArrowStyle.Circle
            scSynapse.ArrowDestination.Filled = False
            scSynapse.Name = "NMDA type"
            scSynapse.SynapticConductance.ValueManual = 0.1
            scSynapse.DecayRate.ValueManual = 50
            scSynapse.RelativeFacilitation.ValueManual = 1
            scSynapse.FacilitationDecay.ValueManual = 1
            scSynapse.VoltageDependent = True
            scSynapse.MaxRelativeConductance.ValueManual = 10
            slSynapse.NeuralModule = Me
            m_arySynapseTypes.Add(slSynapse.ID, slSynapse)

            slSynapse = New DataObjects.Behavior.SynapseTypes.SpikingChemical(Me)
            scSynapse = DirectCast(slSynapse, DataObjects.Behavior.SynapseTypes.SpikingChemical)
            scSynapse.ArrowDestination.Style = AnimatTools.DataObjects.Behavior.Link.enumArrowStyle.Circle
            scSynapse.ArrowDestination.Filled = False
            scSynapse.Name = "Hebbian ACh type"
            scSynapse.SynapticConductance.ValueManual = 0.4
            scSynapse.RelativeFacilitation.ValueManual = 1
            scSynapse.FacilitationDecay.ValueManual = 1
            scSynapse.Hebbian = True
            scSynapse.VoltageDependent = True
            scSynapse.LearningIncrement = 0.02
            scSynapse.MaxAugmentedConductance.ValueManual = 2
            scSynapse.LearningTimeWindow.ValueManual = 40
            scSynapse.ConsolidationFactor = 20
            slSynapse.NeuralModule = Me
            m_arySynapseTypes.Add(slSynapse.ID, slSynapse)

            Dim ncSynapse As New DataObjects.Behavior.SynapseTypes.NonSpikingChemical(Me)
            slSynapse = ncSynapse
            ncSynapse.Name = "Nicotinic ACh type"
            slSynapse.NeuralModule = Me
            m_arySynapseTypes.Add(slSynapse.ID, slSynapse)

            ncSynapse = New DataObjects.Behavior.SynapseTypes.NonSpikingChemical(Me)
            slSynapse = ncSynapse
            ncSynapse.Name = "Hyperpolarising IPSP"
            ncSynapse.ArrowDestination.Style = AnimatTools.DataObjects.Behavior.Link.enumArrowStyle.Circle
            ncSynapse.ArrowDestination.Filled = True
            ncSynapse.ArrowMiddle.Style = AnimatTools.DataObjects.Behavior.Link.enumArrowStyle.One
            ncSynapse.EquilibriumPotential.ValueManual = -70
            ncSynapse.MaxSynapticConductance.ValueManual = 0.5
            ncSynapse.PreSynapticThreshold.ValueManual = -55
            ncSynapse.PreSynapticSaturationLevel.ValueManual = -40
            slSynapse.NeuralModule = Me
            m_arySynapseTypes.Add(slSynapse.ID, slSynapse)

            ncSynapse = New DataObjects.Behavior.SynapseTypes.NonSpikingChemical(Me)
            slSynapse = ncSynapse
            ncSynapse.Name = "Depolarising IPSP"
            ncSynapse.ArrowDestination.Style = AnimatTools.DataObjects.Behavior.Link.enumArrowStyle.Circle
            ncSynapse.ArrowDestination.Filled = True
            ncSynapse.ArrowMiddle.Style = AnimatTools.DataObjects.Behavior.Link.enumArrowStyle.One
            ncSynapse.EquilibriumPotential.ValueManual = -55
            ncSynapse.MaxSynapticConductance.ValueManual = 0.5
            ncSynapse.PreSynapticThreshold.ValueManual = -60
            ncSynapse.PreSynapticSaturationLevel.ValueManual = -40
            slSynapse.NeuralModule = Me
            m_arySynapseTypes.Add(slSynapse.ID, slSynapse)

            Dim esSynapse As New DataObjects.Behavior.SynapseTypes.Electrical(Me)
            slSynapse = esSynapse
            esSynapse.Name = "Non-Rectifying Synapse"
            esSynapse.LowCoupling.ValueManual = 0.2
            esSynapse.HighCoupling.ValueManual = 0.2
            esSynapse.TurnOnThreshold.ValueManual = 0
            esSynapse.TurnOnSaturate.ValueManual = 0
            slSynapse.NeuralModule = Me
            m_arySynapseTypes.Add(slSynapse.ID, slSynapse)

            esSynapse = New DataObjects.Behavior.SynapseTypes.Electrical(Me)
            slSynapse = esSynapse
            esSynapse.Name = "Rectifying Synapse"
            esSynapse.LowCoupling.ValueManual = 0.1
            esSynapse.HighCoupling.ValueManual = 0.3
            esSynapse.TurnOnThreshold.ValueManual = 10
            esSynapse.TurnOnSaturate.ValueManual = 30
            slSynapse.NeuralModule = Me
            m_arySynapseTypes.Add(slSynapse.ID, slSynapse)

        End Sub

        Public Overrides Function Clone(ByVal doParent As AnimatTools.Framework.DataObject, ByVal bCutData As Boolean, _
                                        ByVal doRoot As AnimatTools.Framework.DataObject) As AnimatTools.Framework.DataObject
            Dim oNewModule As New DataObjects.Behavior.NeuralModule(doParent)
            oNewModule.CloneInternal(Me, bCutData, doRoot)
            If Not doRoot Is Nothing AndAlso doRoot Is Me Then oNewModule.AfterClone(Me, bCutData, doRoot, oNewModule)
            Return oNewModule
        End Function

        Protected Overrides Sub CloneInternal(ByVal doOriginal As AnimatTools.Framework.DataObject, ByVal bCutData As Boolean, _
                                            ByVal doRoot As AnimatTools.Framework.DataObject)
            MyBase.CloneInternal(doOriginal, bCutData, doRoot)

            Dim nmOrig As DataObjects.Behavior.NeuralModule = DirectCast(doOriginal, DataObjects.Behavior.NeuralModule)

            m_snAHPEquilibriumPotential = DirectCast(nmOrig.m_snAHPEquilibriumPotential.Clone(Me, bCutData, doRoot), ScaledNumber)
            m_snSpikePeak = DirectCast(nmOrig.m_snSpikePeak.Clone(Me, bCutData, doRoot), ScaledNumber)
            m_fltSpikeStrength = nmOrig.m_fltSpikeStrength
            m_snCaEquilibriumPotential = DirectCast(nmOrig.m_snCaEquilibriumPotential.Clone(Me, bCutData, doRoot), ScaledNumber)
            m_snRefractoryPeriod = DirectCast(nmOrig.m_snRefractoryPeriod.Clone(Me, bCutData, doRoot), ScaledNumber)
            m_bUseCriticalPeriod = nmOrig.m_bUseCriticalPeriod
            m_snStartCriticalPeriod = DirectCast(nmOrig.m_snStartCriticalPeriod.Clone(Me, bCutData, doRoot), ScaledNumber)
            m_snEndCriticalPeriod = DirectCast(nmOrig.m_snEndCriticalPeriod.Clone(Me, bCutData, doRoot), ScaledNumber)
            m_bTTX = nmOrig.m_bTTX
            m_bCd = nmOrig.m_bCd
            m_bHodgkinHuxley = nmOrig.m_bHodgkinHuxley

        End Sub

        Protected Overloads Overrides Sub SaveNetworkFile(ByRef oXml As AnimatTools.Interfaces.StdXml)

            oXml.AddChildElement("TimeStep", Me.TimeStep.ActualValue)
            oXml.AddChildElement("SpikePeak", m_snSpikePeak.ValueFromDefaultScale)
            oXml.AddChildElement("SpikeStrength", m_fltSpikeStrength)
            oXml.AddChildElement("AHPEquilPot", m_snAHPEquilibriumPotential.ValueFromDefaultScale)
            oXml.AddChildElement("CaEquilPot", m_snCaEquilibriumPotential.ValueFromDefaultScale)
            oXml.AddChildElement("AbsoluteRefr", m_snRefractoryPeriod.ValueFromDefaultScale)

            oXml.AddChildElement("UseCriticalPeriod ", m_bUseCriticalPeriod)
            oXml.AddChildElement("StartCriticalPeriod", m_snStartCriticalPeriod.ValueFromDefaultScale)
            oXml.AddChildElement("EndCriticalPeriod", m_snEndCriticalPeriod.ValueFromDefaultScale)

            oXml.AddChildElement("TTX", m_bTTX)
            oXml.AddChildElement("Cd", m_bCd)
            oXml.AddChildElement("HH", m_bHodgkinHuxley)

            'First we need to get lists of the different types of synaptic types.
            Dim iIndex As Integer = 0

            Dim aryElectrical As New ArrayList
            Dim arySpiking As New ArrayList
            Dim aryNonSpiking As New ArrayList
            For Each deEntry As DictionaryEntry In m_arySynapseTypes
                If deEntry.Value.GetType() Is GetType(DataObjects.Behavior.SynapseTypes.SpikingChemical) Then
                    arySpiking.Add(deEntry.Value)
                ElseIf deEntry.Value.GetType() Is GetType(DataObjects.Behavior.SynapseTypes.NonSpikingChemical) Then
                    aryNonSpiking.Add(deEntry.Value)
                ElseIf deEntry.Value.GetType() Is GetType(DataObjects.Behavior.SynapseTypes.Electrical) Then
                    aryElectrical.Add(deEntry.Value)
                Else
                    Throw New System.Exception("An unknown system type was found '" & deEntry.Value.GetType.FullName & "'")
                End If
            Next

            'Go through and save each of the different types of synapses.
            oXml.AddChildElement("Synapses")
            oXml.IntoElem()

            iIndex = 0
            oXml.AddChildElement("SpikingSynapses")
            oXml.IntoElem()
            For Each stSynapse As DataObjects.Behavior.SynapseType In arySpiking
                stSynapse.LinkIndex = iIndex
                stSynapse.SaveNetwork(oXml, Me)
                iIndex = iIndex + 1
            Next
            oXml.OutOfElem() 'Outof SpikingSynapses

            iIndex = 0
            oXml.AddChildElement("NonSpikingSynapses")
            oXml.IntoElem()
            For Each stSynapse As DataObjects.Behavior.SynapseType In aryNonSpiking
                stSynapse.LinkIndex = iIndex
                stSynapse.SaveNetwork(oXml, Me)
                iIndex = iIndex + 1
            Next
            oXml.OutOfElem() 'Outof NonSpikingSynapses

            iIndex = 0
            oXml.AddChildElement("ElectricalSynapses")
            oXml.IntoElem()
            For Each stSynapse As DataObjects.Behavior.SynapseType In aryElectrical
                stSynapse.LinkIndex = iIndex
                stSynapse.SaveNetwork(oXml, Me)
                iIndex = iIndex + 1
            Next
            oXml.OutOfElem() 'Outof ElectricalSynapses

            oXml.OutOfElem() 'Outof Synapses

            'Go through and save the neurons
            iIndex = 0
            Dim bnNode As AnimatTools.DataObjects.Behavior.Node
            oXml.AddChildElement("Neurons")
            oXml.IntoElem()
            For Each deEntry As DictionaryEntry In m_aryNodes
                If Util.IsTypeOf(deEntry.Value.GetType(), GetType(RealisticNeuralNetTools.DataObjects.Behavior.Neurons.Spiking), False) Then
                    bnNode = DirectCast(deEntry.Value, AnimatTools.DataObjects.Behavior.Node)
                    bnNode.NodeIndex = iIndex
                    bnNode.SaveNetwork(oXml, Me)
                    iIndex = iIndex + 1
                Else
                    Throw New System.Exception("There was a node in the realistic neural module that was not derived from a spiking neuron?")
                End If
            Next
            oXml.OutOfElem()

            'Go through and save the connections
            iIndex = 0
            Dim blLink As RealisticNeuralNetTools.DataObjects.Behavior.Synapse
            oXml.AddChildElement("Connexions")
            oXml.IntoElem()
            For Each deEntry As DictionaryEntry In m_aryLinks
                If Util.IsTypeOf(deEntry.Value.GetType(), GetType(RealisticNeuralNetTools.DataObjects.Behavior.Synapse), False) Then
                    blLink = DirectCast(deEntry.Value, RealisticNeuralNetTools.DataObjects.Behavior.Synapse)
                    blLink.LinkIndex = iIndex
                    blLink.SaveNetwork(oXml, Me)
                    iIndex = iIndex + 1
                Else
                    Throw New System.Exception("There was a link in the realistic neural module that was not derived from a synapse?")
                End If
            Next
            oXml.OutOfElem()


        End Sub

#Region " DataObject Methods "

        Protected Overrides Sub BuildProperties()
            MyBase.BuildProperties()

            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Spike Strength", m_fltSpikeStrength.GetType(), "SpikeStrength", _
                                        "Global Neural Properties", "Spikes drive current through electrical synapses, but because the spike " & _
                                        "is not modelled accurately, it may sometimes be necessary to increase the effective amplitude of the " & _
                                        "spike in order to drive a more realistic amount of current, This is accomplished by setting the spike " & _
                                        "'strength' to some value above 1. Acceptable values are in the range 1 to 1000.", m_fltSpikeStrength))

            Dim pbNumberBag As Crownwood.Magic.Controls.PropertyBag = m_snAHPEquilibriumPotential.Properties
            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("AHP Equil Potential", pbNumberBag.GetType(), "AHPEquilibriumPotential", _
                                        "Global Neural Properties", "Sets  the value of the afterhyperpolarisation equilibrium potential. This sets the " & _
                                        "equilibrium potential for the conductance increase (presumably to potassium) which follows a spike. " & _
                                        " Acceptable values are in the range -150 to -10 mV.", pbNumberBag, _
                                        "", GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)))

            pbNumberBag = m_snSpikePeak.Properties
            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Spike Peak", pbNumberBag.GetType(), "SpikePeak", _
                                        "Global Neural Properties", "Sets the value of the membrane potential at the peak of the spike. " & _
                                        "Acceptable values are in the range -30 to 50 mV.", pbNumberBag, "", _
                                        GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)))

            pbNumberBag = m_snCaEquilibriumPotential.Properties
            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Ca Equil Potential", pbNumberBag.GetType(), "CaEquilibriumPotential", _
                                        "Global Neural Properties", "Sets the value of the Ca equilibrium potential. " & _
                                        "Acceptable values are in the range -100 to 500 mV.", pbNumberBag, "", _
                                        GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)))

            pbNumberBag = m_snRefractoryPeriod.Properties
            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Refractory Period", pbNumberBag.GetType(), "RefractoryPeriod", _
                                        "Global Neural Properties", "Sets the value of the absolute refractory period. This sets the time period " & _
                                        "following a spike during which it is impossible to elicit another spike. " & _
                                        "Acceptable values are in the range 1 to 50 ms.", pbNumberBag, "", _
                                        GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)))

            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Use Critical Period", m_bUseCriticalPeriod.GetType(), "UseCriticalPeriod", _
                                        "Global Neural Properties", "Determines whether a critical period during which hebbian learning and forgetting occur in an experiment.", m_bUseCriticalPeriod))

            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Apply TTX", m_bTTX.GetType(), "ApplyTTX", _
                                        "Global Neural Properties", "Applies TTX to the nuerons in the network. This blocks spiking synapses from firing action potentials.", m_bTTX))

            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Apply Cadmium", m_bCd.GetType(), "ApplyCd", _
                                        "Global Neural Properties", "Applies Cadmium to the nuerons in the network. Cadmium blocks calcium currents. " & _
                                        "This blocks all chemical synapses (both spiking and non-spiking). It also blocks any calcium current defined in the neuron.", m_bCd))

            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Hodgkin Huxley", m_bHodgkinHuxley.GetType(), "HodgkinHuxley", _
                                        "Global Neural Properties", "If this is true the integrate-and-fire DE is replaced with a standard HH DE.", m_bHodgkinHuxley))

            pbNumberBag = m_snStartCriticalPeriod.Properties
            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Critical Period Start", pbNumberBag.GetType(), "StartCriticalPeriod", _
                                        "Global Neural Properties", "Sets the start time of the critical period in seconds. " & _
                                        "Acceptable values are in the range 0 to 10000 s.", pbNumberBag, "", _
                                        GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)))

            pbNumberBag = m_snEndCriticalPeriod.Properties
            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Critical Period End", pbNumberBag.GetType(), "EndCriticalPeriod", _
                                        "Global Neural Properties", "Sets the end time of the critical period in seconds. " & _
                                        "This value must be larger than the critical period start time. " & _
                                        "Acceptable values are in the range 0 to 10000 s.", pbNumberBag, "", _
                                        GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)))

        End Sub

        Public Overrides Sub ClearIsDirty()
            MyBase.ClearIsDirty()

            If Not m_snAHPEquilibriumPotential Is Nothing Then m_snAHPEquilibriumPotential.ClearIsDirty()
            If Not m_snSpikePeak Is Nothing Then m_snSpikePeak.ClearIsDirty()
            If Not m_snCaEquilibriumPotential Is Nothing Then m_snCaEquilibriumPotential.ClearIsDirty()
            If Not m_snRefractoryPeriod Is Nothing Then m_snRefractoryPeriod.ClearIsDirty()
            If Not m_snStartCriticalPeriod Is Nothing Then m_snStartCriticalPeriod.ClearIsDirty()
            If Not m_snEndCriticalPeriod Is Nothing Then m_snEndCriticalPeriod.ClearIsDirty()
            m_arySynapseTypes.ClearIsDirty()
        End Sub

        Public Overrides Sub LoadData(ByRef oXml As AnimatTools.Interfaces.StdXml)
            MyBase.LoadData(oXml)

            oXml.IntoElem()  'Into Module Element

            m_snAHPEquilibriumPotential.LoadData(oXml, "AHPEquilibriumPotential")
            m_snSpikePeak.LoadData(oXml, "SpikePeak")
            m_fltSpikeStrength = oXml.GetChildFloat("SpikeStrength")
            m_snCaEquilibriumPotential.LoadData(oXml, "CaEquilibriumPotential")
            m_snRefractoryPeriod.LoadData(oXml, "RefractoryPeriod")

            m_bUseCriticalPeriod = oXml.GetChildBool("UseCriticalPeriod")
            m_snStartCriticalPeriod.LoadData(oXml, "StartCriticalPeriod")
            m_snEndCriticalPeriod.LoadData(oXml, "EndCriticalPeriod")

            m_bTTX = oXml.GetChildBool("TTX", m_bTTX)
            m_bCd = oXml.GetChildBool("Cd", m_bCd)
            m_bHodgkinHuxley = oXml.GetChildBool("HH", m_bHodgkinHuxley)

            m_arySynapseTypes.Clear()
            oXml.IntoChildElement("SynapseTypes")
            Dim iCount As Integer = oXml.NumberOfChildren() - 1
            Dim strAssemblyFile As String
            Dim strClassName As String
            Dim stType As SynapseType
            For iIndex As Integer = 0 To iCount
                oXml.FindChildByIndex(iIndex)
                oXml.IntoElem() 'Into Node Element
                strAssemblyFile = oXml.GetChildString("AssemblyFile")
                strClassName = oXml.GetChildString("ClassName")
                oXml.OutOfElem() 'Outof Node Element

                stType = DirectCast(Util.LoadClass(strAssemblyFile, strClassName, Me), SynapseType)
                stType.LoadData(oXml)
                stType.NeuralModule = Me

                m_arySynapseTypes.Add(stType.ID, stType)
            Next
            oXml.OutOfElem()

            oXml.OutOfElem()  'Outof Module Element

        End Sub

        Public Overrides Sub SaveData(ByRef oXml As AnimatTools.Interfaces.StdXml)
            MyBase.SaveData(oXml)

            oXml.IntoElem()  'Into Module Element

            m_snAHPEquilibriumPotential.SaveData(oXml, "AHPEquilibriumPotential")
            m_snSpikePeak.SaveData(oXml, "SpikePeak")
            oXml.AddChildElement("SpikeStrength", m_fltSpikeStrength)
            m_snCaEquilibriumPotential.SaveData(oXml, "CaEquilibriumPotential")
            m_snRefractoryPeriod.SaveData(oXml, "RefractoryPeriod")

            oXml.AddChildElement("UseCriticalPeriod", m_bUseCriticalPeriod)
            m_snStartCriticalPeriod.SaveData(oXml, "StartCriticalPeriod")
            m_snEndCriticalPeriod.SaveData(oXml, "EndCriticalPeriod")

            oXml.AddChildElement("TTX", m_bTTX)
            oXml.AddChildElement("Cd", m_bCd)
            oXml.AddChildElement("HH", m_bHodgkinHuxley)

            oXml.AddChildElement("SynapseTypes")
            oXml.IntoElem()
            Dim stType As SynapseType
            For Each deEntry As DictionaryEntry In m_arySynapseTypes
                stType = DirectCast(deEntry.Value, SynapseType)
                stType.SaveData(oXml)
            Next
            oXml.OutOfElem()

            oXml.OutOfElem()  'Outof Module Element

        End Sub

#End Region

#End Region

    End Class

End Namespace
