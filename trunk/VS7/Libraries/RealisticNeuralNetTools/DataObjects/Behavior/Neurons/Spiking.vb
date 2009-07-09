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

    Public Class Spiking
        Inherits AnimatTools.DataObjects.Behavior.Nodes.Neuron

#Region " Attributes "

        'Basic Properties
        Protected m_snRestingPotential As AnimatTools.Framework.ScaledNumber
        Protected m_snRelativeSize As AnimatTools.Framework.ScaledNumber
        Protected m_snTimeConstant As AnimatTools.Framework.ScaledNumber
        Protected m_snInitialThreshold As AnimatTools.Framework.ScaledNumber
        Protected m_snRelativeAccomodation As AnimatTools.Framework.ScaledNumber
        Protected m_snAccomodationTimeConstant As AnimatTools.Framework.ScaledNumber
        Protected m_snAHPConductance As AnimatTools.Framework.ScaledNumber
        Protected m_snAHPTimeConstant As AnimatTools.Framework.ScaledNumber

        'Bursting Properties
        Protected m_snMaxCaConductance As AnimatTools.Framework.ScaledNumber
        Protected m_atCaActivation As ActivationType
        Protected m_atCaDeactivation As ActivationType

        'Tonic Input
        Protected m_snTonicStimulus As AnimatTools.Framework.ScaledNumber
        Protected m_snTonicNoise As AnimatTools.Framework.ScaledNumber

        Protected m_aryIonChannels As Collections.IonChannels

#End Region

#Region " Properties "

        Public Overrides ReadOnly Property TypeName() As String
            Get
                Return "Spiking Neuron"
            End Get
        End Property

        Public Overrides ReadOnly Property NeuralModuleType() As System.Type
            Get
                Return GetType(RealisticNeuralNetTools.DataObjects.Behavior.NeuralModule)
            End Get
        End Property

        Public Overridable Property RestingPotential() As AnimatTools.Framework.ScaledNumber
            Get
                Return m_snRestingPotential
            End Get
            Set(ByVal Value As AnimatTools.Framework.ScaledNumber)
                If Value.ActualValue < -0.1 OrElse Value.ActualValue > -0.02 Then
                    Throw New System.Exception("The resting potential must be between the range -100 mV and -20 mV.")
                End If

                m_snRestingPotential.CopyData(Value)
            End Set
        End Property

        Public Overridable Property RelativeSize() As AnimatTools.Framework.ScaledNumber
            Get
                Return m_snRelativeSize
            End Get
            Set(ByVal Value As AnimatTools.Framework.ScaledNumber)
                If Value.ActualValue < 0.00000001 OrElse Value.ActualValue > 10000 Then
                    Throw New System.Exception("The relative electrical size must be between the range 0.0001 and 10000.")
                End If

                m_snRelativeSize.CopyData(Value)
            End Set
        End Property

        Public Overridable Property TimeConstant() As AnimatTools.Framework.ScaledNumber
            Get
                Return m_snTimeConstant
            End Get
            Set(ByVal Value As AnimatTools.Framework.ScaledNumber)
                If Value.ActualValue < 0.00001 OrElse Value.ActualValue > 1 Then
                    Throw New System.Exception("The membrane time constant must be between the range 0.01 mS and 1000 mS.")
                End If

                m_snTimeConstant.CopyData(Value)
            End Set
        End Property

        Public Overridable Property InitialThreshold() As AnimatTools.Framework.ScaledNumber
            Get
                Return m_snInitialThreshold
            End Get
            Set(ByVal Value As AnimatTools.Framework.ScaledNumber)
                If Value.ActualValue < -0.1 OrElse Value.ActualValue > 0.1 Then
                    Throw New System.Exception("The initial threshold must be between the range -100 mV and 100 mV.")
                End If

                m_snInitialThreshold.CopyData(Value)
            End Set
        End Property

        Public Overridable Property RelativeAccomodation() As AnimatTools.Framework.ScaledNumber
            Get
                Return m_snRelativeAccomodation
            End Get
            Set(ByVal Value As AnimatTools.Framework.ScaledNumber)
                If Value.ActualValue < 0 OrElse Value.ActualValue > 1 Then
                    Throw New System.Exception("The relative accomodation must be between the range 0 and 1.")
                End If

                m_snRelativeAccomodation.CopyData(Value)
            End Set
        End Property

        Public Overridable Property AccomodationTimeConstant() As AnimatTools.Framework.ScaledNumber
            Get
                Return m_snAccomodationTimeConstant
            End Get
            Set(ByVal Value As AnimatTools.Framework.ScaledNumber)
                If Value.ActualValue < 0.001 OrElse Value.ActualValue > 40 Then
                    Throw New System.Exception("The accomodation time constant must be between the range 1 ms and 40 s.")
                End If

                m_snAccomodationTimeConstant.CopyData(Value)
            End Set
        End Property

        Public Overridable Property AHPConductance() As AnimatTools.Framework.ScaledNumber
            Get
                Return m_snAHPConductance
            End Get
            Set(ByVal Value As AnimatTools.Framework.ScaledNumber)
                If Value.ActualValue < 0 OrElse Value.ActualValue > 0.0001 Then
                    Throw New System.Exception("The after-hyperpolarization conductance must be between the range 0 S and 100 uS.")
                End If

                m_snAHPConductance.CopyData(Value)
            End Set
        End Property

        Public Overridable Property AHPTimeConstant() As AnimatTools.Framework.ScaledNumber
            Get
                Return m_snAHPTimeConstant
            End Get
            Set(ByVal Value As AnimatTools.Framework.ScaledNumber)
                If Value.ActualValue < 0.001 OrElse Value.ActualValue > 0.1 Then
                    Throw New System.Exception("The after-hyperpolarization time constant must be between the range 1 ms and 100 ms.")
                End If

                m_snAHPTimeConstant.CopyData(Value)
            End Set
        End Property

        Public Overridable Property MaxCaConductance() As AnimatTools.Framework.ScaledNumber
            Get
                Return m_snMaxCaConductance
            End Get
            Set(ByVal Value As AnimatTools.Framework.ScaledNumber)
                If Value.ActualValue < 0 OrElse Value.ActualValue > 0.0001 Then
                    Throw New System.Exception("The maximum calcium conductance must be between the range 0 S and 100 uS.")
                End If

                m_snMaxCaConductance.CopyData(Value)
            End Set
        End Property

        Public Overridable Property CaActivation() As ActivationType
            Get
                Return m_atCaActivation
            End Get
            Set(ByVal Value As ActivationType)
                m_atCaActivation = Value
            End Set
        End Property

        Public Overridable Property CaDeactivation() As ActivationType
            Get
                Return m_atCaDeactivation
            End Get
            Set(ByVal Value As ActivationType)
                m_atCaDeactivation = Value
            End Set
        End Property

        Public Overridable Property TonicStimulus() As AnimatTools.Framework.ScaledNumber
            Get
                Return m_snTonicStimulus
            End Get
            Set(ByVal Value As AnimatTools.Framework.ScaledNumber)
                If Value.ActualValue < -0.000001 OrElse Value.ActualValue > 0.000001 Then
                    Throw New System.Exception("The tonic stimulus current must be between the range -1000 nA and 1000 nA.")
                End If

                m_snTonicStimulus.CopyData(Value)
            End Set
        End Property

        Public Overridable Property TonicNoise() As AnimatTools.Framework.ScaledNumber
            Get
                Return m_snTonicNoise
            End Get
            Set(ByVal Value As AnimatTools.Framework.ScaledNumber)
                If Value.ActualValue < 0 OrElse Value.ActualValue > 0.005 Then
                    Throw New System.Exception("The tonic membrane noise must be between the range 0 mV and 5 mV.")
                End If

                m_snTonicNoise.CopyData(Value)
            End Set
        End Property

        <Browsable(False)> _
        Public Overridable Property IonChannels() As Collections.IonChannels
            Get
                Return m_aryIonChannels
            End Get
            Set(ByVal Value As Collections.IonChannels)
                m_aryIonChannels = Value
            End Set
        End Property

        Public Overrides ReadOnly Property ImageName() As String
            Get
                Return "RealisticNeuralNetTools.SpikingNeuron.gif"
            End Get
        End Property

        <Browsable(False)> _
        Public Overrides ReadOnly Property DataColumnModuleName() As String
            Get
                Return "RealisticNeuralNet"
            End Get
        End Property

        <Browsable(False)> _
        Public Overrides ReadOnly Property DataColumnClassType() As String
            Get
                Return "NeuronData"
            End Get
        End Property

        <Browsable(False)> _
        Public Overrides ReadOnly Property RequiresAutoDataCollectInterval() As Boolean
            Get
                Return True
            End Get
        End Property


#End Region

#Region " Methods "

        Public Sub New(ByVal doParent As AnimatTools.Framework.DataObject)
            MyBase.New(doParent)

            Try

                m_bEnabled = True
                m_snRestingPotential = New AnimatTools.Framework.ScaledNumber(Me, "RestingPotential", -60, AnimatTools.Framework.ScaledNumber.enumNumericScale.milli, "Volts", "V")
                m_snRelativeSize = New AnimatTools.Framework.ScaledNumber(Me, "RelativeSize", 1, AnimatTools.Framework.ScaledNumber.enumNumericScale.None, "", "")
                m_snTimeConstant = New AnimatTools.Framework.ScaledNumber(Me, "TimeConstant", 5, AnimatTools.Framework.ScaledNumber.enumNumericScale.milli, "seconds", "s")
                m_snInitialThreshold = New AnimatTools.Framework.ScaledNumber(Me, "InitialThreshold", -40, AnimatTools.Framework.ScaledNumber.enumNumericScale.milli, "Volts", "V")
                m_snRelativeAccomodation = New AnimatTools.Framework.ScaledNumber(Me, "RelativeAccomodation", 0.3, AnimatTools.Framework.ScaledNumber.enumNumericScale.None, "", "")
                m_snAccomodationTimeConstant = New AnimatTools.Framework.ScaledNumber(Me, "AccomodationTimeConstant", 10, AnimatTools.Framework.ScaledNumber.enumNumericScale.milli, "seconds", "s")
                m_snAHPConductance = New AnimatTools.Framework.ScaledNumber(Me, "AHPConductance", 1, AnimatTools.Framework.ScaledNumber.enumNumericScale.micro, "Siemens", "S")
                m_snAHPTimeConstant = New AnimatTools.Framework.ScaledNumber(Me, "AHPTimeConstant", 3, AnimatTools.Framework.ScaledNumber.enumNumericScale.milli, "seconds", "s")

                m_snMaxCaConductance = New AnimatTools.Framework.ScaledNumber(Me, "MaxCaConductance", 0, AnimatTools.Framework.ScaledNumber.enumNumericScale.micro, "Siemens", "S")
                m_atCaActivation = New ActivationType(Me, True)
                m_atCaDeactivation = New ActivationType(Me, False)

                m_snTonicStimulus = New AnimatTools.Framework.ScaledNumber(Me, "TonicStimulus", 0, AnimatTools.Framework.ScaledNumber.enumNumericScale.nano, "Amps", "A")
                m_snTonicNoise = New AnimatTools.Framework.ScaledNumber(Me, "TonicNoise", 0, AnimatTools.Framework.ScaledNumber.enumNumericScale.milli, "Volts", "V")

                Shape = AnimatTools.DataObjects.Behavior.Node.enumShape.Termination
                Size = New SizeF(40, 40)
                Me.DrawColor = Color.Black
                Me.FillColor = Color.Gold

                Dim myAssembly As System.Reflection.Assembly
                myAssembly = System.Reflection.Assembly.Load("RealisticNeuralNetTools")

                Me.Image = AnimatTools.Framework.ImageManager.LoadImage(myAssembly, "RealisticNeuralNetTools.SpikingNeuron.gif", False)
                Me.Name = "Spiking Neuron"

                Me.Font = New Font("Arial", 14, FontStyle.Bold)
                Me.Description = "A spiking type of neuron in the realistic neural network library."

                AddCompatibleLink(New AnimatTools.DataObjects.Behavior.Links.Adapter(Nothing))
                AddCompatibleLink(New SynapseTypes.NonSpikingChemical(Nothing))
                AddCompatibleLink(New SynapseTypes.SpikingChemical(Nothing))
                AddCompatibleLink(New SynapseTypes.Electrical(Nothing))

                'Lets add the data types that this node understands.
                'm_thDataTypes.DataTypes.Add("IntrinsicCurrent", "Intrinsic Current")
                m_thDataTypes.DataTypes.Add(New AnimatTools.DataObjects.DataType("AdapterCurrent", "Adapter Current", "Amps", "A", -100, 100, ScaledNumber.enumNumericScale.nano, ScaledNumber.enumNumericScale.nano))
                m_thDataTypes.DataTypes.Add(New AnimatTools.DataObjects.DataType("ExternalCurrent", "External Current", "Amps", "A", -100, 100, ScaledNumber.enumNumericScale.nano, ScaledNumber.enumNumericScale.nano))
                m_thDataTypes.DataTypes.Add(New AnimatTools.DataObjects.DataType("IonChannelCurrent", "Ion Channel Current", "Amps", "A", -100, 100, ScaledNumber.enumNumericScale.nano, ScaledNumber.enumNumericScale.nano))
                m_thDataTypes.DataTypes.Add(New AnimatTools.DataObjects.DataType("ElectricalSynapticCurrent", "Electrical Synaptic Current", "Amps", "A", -100, 100, ScaledNumber.enumNumericScale.nano, ScaledNumber.enumNumericScale.nano))
                m_thDataTypes.DataTypes.Add(New AnimatTools.DataObjects.DataType("SpikingSynapticCurrent", "Spiking Synaptic Current", "Amps", "A", -100, 100, ScaledNumber.enumNumericScale.nano, ScaledNumber.enumNumericScale.nano))
                m_thDataTypes.DataTypes.Add(New AnimatTools.DataObjects.DataType("NonSpikingSynapticCurrent", "NonSpiking Synaptic Current", "Amps", "A", -100, 100, ScaledNumber.enumNumericScale.nano, ScaledNumber.enumNumericScale.nano))
                m_thDataTypes.DataTypes.Add(New AnimatTools.DataObjects.DataType("MembraneVoltage", "Membrane Voltage", "Volts", "V", -100, 100, ScaledNumber.enumNumericScale.milli, ScaledNumber.enumNumericScale.milli))
                m_thDataTypes.DataTypes.Add(New AnimatTools.DataObjects.DataType("FiringFrequency", "Firing Frequency", "Hertz", "Hz", 0, 1000))
                m_thDataTypes.DataTypes.Add(New AnimatTools.DataObjects.DataType("Threshold", "Threshold", "Volts", "V", -100, 100, ScaledNumber.enumNumericScale.milli, ScaledNumber.enumNumericScale.milli))
                m_thDataTypes.DataTypes.Add(New AnimatTools.DataObjects.DataType("Spike", "Spike", "", "", 1, 0, ScaledNumber.enumNumericScale.None, ScaledNumber.enumNumericScale.None))
                m_thDataTypes.ID = "MembraneVoltage"

                m_thIncomingDataType = New AnimatTools.DataObjects.DataType("ExternalCurrent", "External Current", "Amps", "A", -100, 100, ScaledNumber.enumNumericScale.nano, ScaledNumber.enumNumericScale.nano)

                m_aryIonChannels = New Collections.IonChannels(Me)

            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try

        End Sub

        Public Overrides Sub InitAfterAppStart()
            MyBase.InitAfterAppStart()
            AddCompatibleStimulusType("Current")
        End Sub

        Public Overrides Function Clone(ByVal doParent As AnimatTools.Framework.DataObject, ByVal bCutData As Boolean, _
                                        ByVal doRoot As AnimatTools.Framework.DataObject) As AnimatTools.Framework.DataObject
            Dim oNewNode As New Neurons.Spiking(doParent)
            oNewNode.CloneInternal(Me, bCutData, doRoot)
            If Not doRoot Is Nothing AndAlso doRoot Is Me Then oNewNode.AfterClone(Me, bCutData, doRoot, oNewNode)
            Return oNewNode
        End Function

        Protected Overrides Sub CloneInternal(ByVal doOriginal As AnimatTools.Framework.DataObject, ByVal bCutData As Boolean, _
                                            ByVal doRoot As AnimatTools.Framework.DataObject)
            MyBase.CloneInternal(doOriginal, bCutData, doRoot)

            Dim bnOrig As Neurons.Spiking = DirectCast(doOriginal, Neurons.Spiking)

            m_bEnabled = bnOrig.m_bEnabled
            m_snRestingPotential = DirectCast(bnOrig.m_snRestingPotential.Clone(Me, bCutData, doRoot), ScaledNumber)
            m_snRelativeSize = DirectCast(bnOrig.m_snRelativeSize.Clone(Me, bCutData, doRoot), ScaledNumber)
            m_snTimeConstant = DirectCast(bnOrig.m_snTimeConstant.Clone(Me, bCutData, doRoot), ScaledNumber)
            m_snInitialThreshold = DirectCast(bnOrig.m_snInitialThreshold.Clone(Me, bCutData, doRoot), ScaledNumber)
            m_snRelativeAccomodation = DirectCast(bnOrig.m_snRelativeAccomodation.Clone(Me, bCutData, doRoot), ScaledNumber)
            m_snAccomodationTimeConstant = DirectCast(bnOrig.m_snAccomodationTimeConstant.Clone(Me, bCutData, doRoot), ScaledNumber)
            m_snAHPConductance = DirectCast(bnOrig.m_snAHPConductance.Clone(Me, bCutData, doRoot), ScaledNumber)
            m_snAHPTimeConstant = DirectCast(bnOrig.m_snAHPTimeConstant.Clone(Me, bCutData, doRoot), ScaledNumber)

            m_snMaxCaConductance = DirectCast(bnOrig.m_snMaxCaConductance.Clone(Me, bCutData, doRoot), ScaledNumber)
            m_atCaActivation = DirectCast(bnOrig.m_atCaActivation.Clone(Me, bCutData, doRoot), ActivationType)
            m_atCaDeactivation = DirectCast(bnOrig.m_atCaDeactivation.Clone(Me, bCutData, doRoot), ActivationType)

            m_snTonicStimulus = DirectCast(bnOrig.m_snTonicStimulus.Clone(Me, bCutData, doRoot), ScaledNumber)
            m_snTonicNoise = DirectCast(bnOrig.m_snTonicNoise.Clone(Me, bCutData, doRoot), ScaledNumber)

            m_aryIonChannels = DirectCast(bnOrig.m_aryIonChannels.Clone(Me, bCutData, doRoot), Collections.IonChannels)

        End Sub


        Public Overrides Function SelectLinkType(ByRef bnOrigin As AnimatTools.DataObjects.Behavior.Node, _
                                                 ByRef bnDestination As AnimatTools.DataObjects.Behavior.Node, _
                                                 ByRef blLink As AnimatTools.DataObjects.Behavior.Link, _
                                                 ByRef aryCompatibleLinks As AnimatTools.Collections.Links) As Boolean

            Dim frmSynapseType As New Forms.SelectSynapseType
            frmSynapseType.Origin = bnOrigin
            frmSynapseType.Destination = bnDestination
            frmSynapseType.CompatibleLinks = aryCompatibleLinks
            frmSynapseType.NeuralModule = DirectCast(Me.Organism.NeuralModules(Me.NeuralModuleType.FullName), DataObjects.Behavior.NeuralModule)

            If frmSynapseType.ShowDialog(m_ParentEditor) = DialogResult.OK Then
                blLink = frmSynapseType.SelectedLink
                Return True
            Else
                Return False
            End If

        End Function

        Public Overrides Function CreateNewAdapter(ByRef bnOrigin As AnimatTools.DataObjects.Behavior.Node, _
                                                   ByRef doParent As AnimatTools.Framework.DataObject) As AnimatTools.DataObjects.Behavior.Node

            ''If it does require an adapter then lets add the pieces.
            Dim bnAdapter As AnimatTools.DataObjects.Behavior.Node
            If bnOrigin.IsPhysicsEngineNode AndAlso Not Me.IsPhysicsEngineNode Then
                'If the origin is physics node and the destination is a regular node
                bnAdapter = New AnimatTools.DataObjects.Behavior.Nodes.PhysicalToNodeAdapter(doParent)
            ElseIf Not bnOrigin.IsPhysicsEngineNode AndAlso Me.IsPhysicsEngineNode Then
                'If the origin is regular node and the destination is a physics node
                bnAdapter = New AnimatTools.DataObjects.Behavior.Nodes.NodeToPhysicalAdapter(doParent)
            ElseIf Not bnOrigin.IsPhysicsEngineNode AndAlso Not Me.IsPhysicsEngineNode Then
                'If both the origin and destination are regular nodes.
                If Util.IsTypeOf(bnOrigin.GetType, "FastNeuralNetTools.DataObjects.Behavior.Neurons.Normal", False) Then
                    'If the origin is of the type FastNeuralNetTools.DataObjects.Neurons.Normal then use our special adapter.
                    bnAdapter = New DataObjects.Behavior.Neurons.FiringRateAdapter(doParent)
                Else
                    bnAdapter = New AnimatTools.DataObjects.Behavior.Nodes.NodeToNodeAdapter(doParent)
                End If
            Else
                'If both the origin and destination are physics nodes.
                Throw New System.Exception("You can only link two physics nodes using a graphical link.")
            End If

            Return bnAdapter
        End Function

        Public Overrides Sub SaveNetwork(ByRef oXml As AnimatTools.Interfaces.StdXml, ByRef nmModule As AnimatTools.DataObjects.Behavior.NeuralModule)

            oXml.AddChildElement("Neuron")
            oXml.IntoElem()

            oXml.AddChildElement("ID", Me.NodeIndex)
            oXml.AddChildElement("Name", Me.Text)
            oXml.AddChildElement("Zapped", Not m_bEnabled)
            oXml.AddChildElement("TonicStimulus", m_snTonicStimulus.ValueFromDefaultScale)
            oXml.AddChildElement("Noise", m_snTonicNoise.ValueFromDefaultScale)
            oXml.AddChildElement("RestingPot", m_snRestingPotential.ValueFromDefaultScale)
            oXml.AddChildElement("Size", m_snRelativeSize.ValueFromDefaultScale)
            oXml.AddChildElement("TimeConst", m_snTimeConstant.ValueFromDefaultScale)
            oXml.AddChildElement("InitialThresh", m_snInitialThreshold.ValueFromDefaultScale)
            oXml.AddChildElement("RelativeAccom", m_snRelativeAccomodation.ValueFromDefaultScale)
            oXml.AddChildElement("AccomTimeConst", m_snAccomodationTimeConstant.ValueFromDefaultScale)
            oXml.AddChildElement("AHPAmp", m_snAHPConductance.ValueFromDefaultScale)
            oXml.AddChildElement("AHPTimeConst", m_snAHPTimeConstant.ValueFromDefaultScale)
            oXml.AddChildElement("GMaxCa", m_snMaxCaConductance.ValueFromDefaultScale)
            oXml.AddChildElement("VM", m_atCaActivation.MidPoint.ValueFromDefaultScale)
            oXml.AddChildElement("SM", m_atCaActivation.Slope.ValueFromDefaultScale)
            oXml.AddChildElement("MTimeConst", m_atCaActivation.TimeConstant.ValueFromDefaultScale)
            oXml.AddChildElement("VH", m_atCaDeactivation.MidPoint.ValueFromDefaultScale)
            oXml.AddChildElement("SH", m_atCaDeactivation.Slope.ValueFromDefaultScale)
            oXml.AddChildElement("HTimeConst", m_atCaDeactivation.TimeConstant.ValueFromDefaultScale)

            'If there are ion channels then save them out as well.
            If m_aryIonChannels.Count > 0 Then
                oXml.AddChildElement("IonChannels")
                oXml.IntoElem()

                Dim doChannel As DataObjects.Behavior.Neurons.IonChannel
                For Each deEntry As DictionaryEntry In m_aryIonChannels
                    doChannel = DirectCast(deEntry.Value, DataObjects.Behavior.Neurons.IonChannel)
                    doChannel.SaveNetwork(oXml, nmModule)
                Next

                oXml.OutOfElem() 'Outof IonChannels Element
            End If

            oXml.OutOfElem() 'Outof Neuron

        End Sub

        Public Overrides Sub SaveDataColumnToXml(ByRef oXml As AnimatTools.Interfaces.StdXml)

            oXml.IntoElem()
            oXml.AddChildElement("OrganismID", Me.Organism.ID)
            oXml.AddChildElement("NeuronIndex", Me.NodeIndex)
            oXml.OutOfElem()

        End Sub

        Public Overrides Function FindDragObject(ByVal strStructureName As String, ByVal strDataItemID As String, Optional ByVal bThrowError As Boolean = True) As AnimatTools.DataObjects.DragObject

            Dim doObject As AnimatTools.DataObjects.DragObject = MyBase.FindDragObject(strStructureName, strDataItemID, False)

            If Not doObject Is Nothing Then
                Return doObject
            End If

            'If not found then look in the ion channels.
            Dim doChannel As IonChannel
            For Each deEntry As DictionaryEntry In m_aryIonChannels
                doChannel = DirectCast(deEntry.Value, IonChannel)
                doObject = doChannel.FindDragObject(strStructureName, strDataItemID, False)

                If Not doObject Is Nothing Then
                    Return doObject
                End If
            Next

            If bThrowError AndAlso doObject Is Nothing Then
                Throw New System.Exception("The drag object with id '" & strDataItemID & "' was not found.")
            End If

            Return doObject

        End Function

        Public Overrides Function CreateDataItemTreeView(ByVal frmDataItem As AnimatTools.Forms.Tools.SelectDataItem, ByVal tnParent As TreeNode, ByVal tpTemplatePartType As Type) As TreeNode

            Dim tnNode As TreeNode = MyBase.CreateDataItemTreeView(frmDataItem, tnParent, tpTemplatePartType)

            'Add the ion channels
            If Not tnNode Is Nothing Then
                Dim doChannel As IonChannel
                For Each deEntry As DictionaryEntry In m_aryIonChannels
                    doChannel = DirectCast(deEntry.Value, IonChannel)
                    doChannel.CreateDataItemTreeView(frmDataItem, tnNode, tpTemplatePartType)
                Next
            End If

            Return tnNode
        End Function

#Region " DataObject Methods "

        Protected Overrides Sub BuildProperties()
            MyBase.BuildProperties()

            'First lets remove the 'Text' property for node base classs
            m_Properties.Properties.Remove("Text")
            m_Properties.Properties.Remove("Node Type")
            m_Properties.Properties.Remove("Description")

            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Neuron Type", GetType(String), "TypeName", _
                                        "Neural Properties", "Returns the type of this neuron.", TypeName(), True))

            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Description", m_strDescription.GetType(), "ToolTip", _
                                        "Neural Properties", "Sets the description for this neuron.", m_strToolTip, _
                                        GetType(AnimatTools.TypeHelpers.MultiLineStringTypeEditor)))

            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Enabled", m_bEnabled.GetType, "Enabled", _
                                        "Neural Properties", "Sets whether this neuron is to participate in the simulation. " & _
                                        "This basically kills the neuron, and thus prevents it participating in any circuit " & _
                                        "interactions, but does not delete it from the circuit.", m_bEnabled))

            'Now lets add properties for name and description
            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Name", m_strText.GetType(), "Text", _
                                        "Neural Properties", "Sets the name of this neuron.", m_strText, _
                                        GetType(AnimatTools.TypeHelpers.MultiLineStringTypeEditor)))

            ''Now lets add the properties for this neuron
            Dim pbNumberBag As Crownwood.Magic.Controls.PropertyBag = m_snRestingPotential.Properties
            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Resting Potential", pbNumberBag.GetType(), "RestingPotential", _
                                        "Neural Properties", "Sets the membrane potential in the absence of any " & _
                                        "stimulus or synaptic input. Acceptable values are in the range -100 to -20 mV.", pbNumberBag, _
                                        "", GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)))

            pbNumberBag = m_snRelativeSize.Properties
            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Relative Size", pbNumberBag.GetType(), "RelativeSize", _
                                        "Neural Properties", "Sets the relative electrical size.  This is essentially equivalent " & _
                                        "to the resting input resistance of the neuron. Acceptable values are in the range 1e-6 to 100.", _
                                        pbNumberBag, "", GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)))

            pbNumberBag = m_snTimeConstant.Properties
            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Time Constant", pbNumberBag.GetType(), "TimeConstant", _
                                        "Neural Properties", "Sets the value of the membrane time constant. " & _
                                        "Acceptable values are in the range 1 to 100 ms.", pbNumberBag, _
                                        "", GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)))

            pbNumberBag = m_snInitialThreshold.Properties
            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Initial Threshold", pbNumberBag.GetType(), "InitialThreshold", _
                                        "Neural Properties", "Sets the value of the initial threshold. This is the threshold prior to " & _
                                        "any spike threshold accommodation that may occur. Acceptable values are in the range -100 to 100 mV.", _
                                        pbNumberBag, "", GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)))

            pbNumberBag = m_snRelativeAccomodation.Properties
            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Relative Accomodation", pbNumberBag.GetType(), "RelativeAccomodation", _
                                        "Neural Properties", "Sets the value of the relative accommodation. This sets the degree to which the " & _
                                        "spike threshold accommodates (increases) in response to depolarization. Acceptable values are in the range 0 to 1.", _
                                        pbNumberBag, "", GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)))

            pbNumberBag = m_snAccomodationTimeConstant.Properties
            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Accomodation Time Constant", pbNumberBag.GetType(), "AccomodationTimeConstant", _
                                        "Neural Properties", "Sets the value of the accommodation time constant. This sets the rate at which the " & _
                                        "threshold adjusts to its new value as the membrane potential changes. Acceptable values are in the range 1 to 1000 ms.", _
                                        pbNumberBag, "", GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)))

            pbNumberBag = m_snAHPConductance.Properties
            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("AHP Conductance", pbNumberBag.GetType(), "AHPConductance", _
                                        "Neural Properties", "Sets the value of the afterhyperpolarisation conductance. This sets the " & _
                                        "value of the conductance increase which occurs immediately following a spike. Acceptable values are " & _
                                        "in the range 0 to 100 uS.", pbNumberBag, "", GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)))

            pbNumberBag = m_snAHPTimeConstant.Properties
            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("AHP Time Constant", pbNumberBag.GetType(), "AHPTimeConstant", _
                                        "Neural Properties", "Sets the value of the afterhyperpolarisation time constant. This sets the rate " & _
                                        "at which the conductance increase which follows a spike decays. Acceptable values are " & _
                                        "in the range 1 to 100 ms.", pbNumberBag, "", GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)))

            pbNumberBag = m_snMaxCaConductance.Properties
            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Max Ca Conductance", pbNumberBag.GetType(), "MaxCaConductance", _
                                        "Busrting Properties", "Sets the value of the maximum calcium conductance. This sets the Ca conductance " & _
                                        "when all Ca channels are fully activated. A value of 15 will induce endogenous bursting if the other parameters " & _
                                        "are left at their default values. Acceptable values are in the range 0 to 100 uS.", pbNumberBag, "", _
                                        GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)))

            pbNumberBag = m_atCaActivation.Properties
            m_Properties.Properties.Add(New PropertySpec("Ca Activation", pbNumberBag.GetType(), "CaActivation", _
                                        "Busrting Properties", "Sets the activation properties of the calcium current.", pbNumberBag, _
                                        "", GetType(Crownwood.Magic.Controls.ExpandablePropBagConverter)))

            pbNumberBag = m_atCaDeactivation.Properties
            m_Properties.Properties.Add(New PropertySpec("Ca Deactivation", pbNumberBag.GetType(), "CaDeactivation", _
                                        "Busrting Properties", "Sets the deactivation properties of the calcium current.", pbNumberBag, _
                                        "", GetType(Crownwood.Magic.Controls.ExpandablePropBagConverter)))

            pbNumberBag = m_snTonicStimulus.Properties
            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Tonic Stimulus", pbNumberBag.GetType(), "TonicStimulus", _
                                        "Tonic Stimulus Properties", "Sets the value of the constant current which will be injected into this neuron. " & _
                                        "Acceptable values are in the range -1000 to 1000 nA.", pbNumberBag, "", _
                                        GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)))

            pbNumberBag = m_snTonicNoise.Properties
            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Tonic Noise", pbNumberBag.GetType(), "TonicNoise", _
                                        "Tonic Stimulus Properties", "Sets the value of the membrane noise. This causes random " & _
                                        "(uniform distribution) changes in membrane potential within the limits set by this parameter. " & _
                                        "Acceptable values are in the range 0 to 5 mV.", pbNumberBag, "", _
                                        GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)))

            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Ion Channels", m_aryIonChannels.GetType(), "IonChannels", _
                                        "Neural Properties", "Edit the ion channels for this neuron.", m_aryIonChannels, _
                                        GetType(TypeHelpers.IonChannelTypeEditor), GetType(TypeHelpers.IonChannelTypeConverter)))

        End Sub

        Public Overrides Sub ClearIsDirty()
            MyBase.ClearIsDirty()

            If Not m_snRestingPotential Is Nothing Then m_snRestingPotential.ClearIsDirty()
            If Not m_snRelativeSize Is Nothing Then m_snRelativeSize.ClearIsDirty()
            If Not m_snTimeConstant Is Nothing Then m_snTimeConstant.ClearIsDirty()
            If Not m_snInitialThreshold Is Nothing Then m_snInitialThreshold.ClearIsDirty()
            If Not m_snRelativeAccomodation Is Nothing Then m_snRelativeAccomodation.ClearIsDirty()
            If Not m_snAccomodationTimeConstant Is Nothing Then m_snAccomodationTimeConstant.ClearIsDirty()
            If Not m_snAHPConductance Is Nothing Then m_snAHPConductance.ClearIsDirty()
            If Not m_snAHPTimeConstant Is Nothing Then m_snAHPTimeConstant.ClearIsDirty()
            If Not m_snMaxCaConductance Is Nothing Then m_snMaxCaConductance.ClearIsDirty()
            If Not m_atCaActivation Is Nothing Then m_atCaActivation.ClearIsDirty()
            If Not m_atCaDeactivation Is Nothing Then m_atCaDeactivation.ClearIsDirty()
            If Not m_snTonicStimulus Is Nothing Then m_snTonicStimulus.ClearIsDirty()
            If Not m_snTonicNoise Is Nothing Then m_snTonicNoise.ClearIsDirty()
            If Not m_aryIonChannels Is Nothing Then m_aryIonChannels.ClearIsDirty()

        End Sub

        Public Overrides Sub LoadData(ByRef oXml As AnimatTools.Interfaces.StdXml)
            MyBase.LoadData(oXml)

            oXml.IntoElem()

            m_bEnabled = oXml.GetChildBool("Enabled", m_bEnabled)
            m_snRestingPotential.LoadData(oXml, "RestingPotential")
            m_snRelativeSize.LoadData(oXml, "RelativeSize")
            m_snTimeConstant.LoadData(oXml, "TimeConstant")
            m_snInitialThreshold.LoadData(oXml, "InitialThreshold")
            m_snRelativeAccomodation.LoadData(oXml, "RelativeAccomodation")
            m_snAccomodationTimeConstant.LoadData(oXml, "AccomodationTimeConstant")
            m_snAHPConductance.LoadData(oXml, "AHPConductance")
            m_snAHPTimeConstant.LoadData(oXml, "AHPTimeConstant")

            m_snMaxCaConductance.LoadData(oXml, "MaxCaConductance")
            m_atCaActivation.LoadData(oXml, "CaActivation")
            m_atCaDeactivation.LoadData(oXml, "CaDeactivation")

            m_snTonicStimulus.LoadData(oXml, "TonicStimulus")
            m_snTonicNoise.LoadData(oXml, "TonicNoise")

            m_aryIonChannels.Clear()
            If oXml.FindChildElement("IonChannels", False) Then
                Dim doChannel As DataObjects.Behavior.Neurons.IonChannel
                oXml.IntoChildElement("IonChannels") 'Into IonChannels Element
                Dim iCount As Integer = oXml.NumberOfChildren() - 1

                For iIndex As Integer = 0 To iCount
                    oXml.FindChildByIndex(iIndex)

                    doChannel = New DataObjects.Behavior.Neurons.IonChannel(Me)
                    doChannel.LoadData(oXml)

                    m_aryIonChannels.Add(doChannel.ID, doChannel)
                Next
                oXml.OutOfElem() 'Outof IonChannels Element
            End If

            oXml.OutOfElem()

        End Sub

        Public Overrides Sub SaveData(ByRef oXml As AnimatTools.Interfaces.StdXml)
            MyBase.SaveData(oXml)

            oXml.IntoElem() 'Into Node Element

            oXml.AddChildElement("Enabled", m_bEnabled)
            m_snRestingPotential.SaveData(oXml, "RestingPotential")
            m_snRelativeSize.SaveData(oXml, "RelativeSize")
            m_snTimeConstant.SaveData(oXml, "TimeConstant")
            m_snInitialThreshold.SaveData(oXml, "InitialThreshold")
            m_snRelativeAccomodation.SaveData(oXml, "RelativeAccomodation")
            m_snAccomodationTimeConstant.SaveData(oXml, "AccomodationTimeConstant")
            m_snAHPConductance.SaveData(oXml, "AHPConductance")
            m_snAHPTimeConstant.SaveData(oXml, "AHPTimeConstant")

            m_snMaxCaConductance.SaveData(oXml, "MaxCaConductance")
            m_atCaActivation.SaveData(oXml, "CaActivation")
            m_atCaDeactivation.SaveData(oXml, "CaDeactivation")

            m_snTonicStimulus.SaveData(oXml, "TonicStimulus")
            m_snTonicNoise.SaveData(oXml, "TonicNoise")

            If m_aryIonChannels.Count > 0 Then
                oXml.AddChildElement("IonChannels")
                oXml.IntoElem()
                Dim doChannel As DataObjects.Behavior.Neurons.IonChannel
                For Each deEntry As DictionaryEntry In m_aryIonChannels
                    doChannel = DirectCast(deEntry.Value, DataObjects.Behavior.Neurons.IonChannel)
                    doChannel.SaveData(oXml)
                Next
                oXml.OutOfElem() 'Outof IonChannels Element
            End If

            oXml.OutOfElem() ' Outof Node Element

        End Sub

#End Region

#End Region

#Region " ActivationType Class "

        Public Class ActivationType
            Inherits AnimatTools.Framework.DataObject

#Region " Attributes "

            Protected m_snMidPoint As AnimatTools.Framework.ScaledNumber
            Protected m_snSlope As AnimatTools.Framework.ScaledNumber
            Protected m_snTimeConstant As AnimatTools.Framework.ScaledNumber
            Protected m_bActivationType As Boolean = True

            Protected m_bnParentNode As AnimatTools.DataObjects.Behavior.Node

#End Region

#Region " Properties "

            <Browsable(False)> _
            Public Overridable Property MidPoint() As AnimatTools.Framework.ScaledNumber
                Get
                    Return m_snMidPoint
                End Get
                Set(ByVal Value As AnimatTools.Framework.ScaledNumber)
                    If Value.ActualValue < -0.2 OrElse Value.ActualValue > 0.2 Then
                        If m_bActivationType Then
                            Throw New System.Exception("The Ca activation level must be between the range -200 mV and 200 mV.")
                        Else
                            Throw New System.Exception("The Ca deactivation level must be between the range -200 mV and 200 mV.")
                        End If
                    End If

                    m_snMidPoint.CopyData(Value)
                    UpdateChart()
                End Set
            End Property

            <Browsable(False)> _
            Public Overridable Property Slope() As AnimatTools.Framework.ScaledNumber
                Get
                    Return m_snSlope
                End Get
                Set(ByVal Value As AnimatTools.Framework.ScaledNumber)
                    If m_bActivationType Then
                        If Value.ActualValue < 0.01 OrElse Value.ActualValue > 10 Then
                            Throw New System.Exception("The Ca activation slope must be between the range 0.01 and 10.")
                        End If
                    Else
                        If Value.ActualValue < -10 OrElse Value.ActualValue > -0.01 Then
                            Throw New System.Exception("The Ca deactivation slope must be between the range -10 and -0.01.")
                        End If
                    End If

                    m_snSlope.CopyData(Value)
                    UpdateChart()
                End Set
            End Property

            <Browsable(False)> _
            Public Overridable Property TimeConstant() As AnimatTools.Framework.ScaledNumber
                Get
                    Return m_snTimeConstant
                End Get
                Set(ByVal Value As AnimatTools.Framework.ScaledNumber)
                    If Value.ActualValue < 0.001 OrElse Value.ActualValue > 100.0 Then
                        If m_bActivationType Then
                            Throw New System.Exception("The Ca activation time constant must be between the range 1 ms and 100000 ms.")
                        Else
                            Throw New System.Exception("The Ca deactivation time constant must be between the range 1 ms and 100000 ms.")
                        End If
                    End If

                    m_snTimeConstant.CopyData(Value)
                    UpdateChart()
                End Set
            End Property

            <Browsable(False)> _
            Public Property ActivationType() As Boolean
                Get
                    Return m_bActivationType
                End Get
                Set(ByVal Value As Boolean)
                    m_bActivationType = Value
                End Set
            End Property

            <Browsable(False)> _
            Public Property ParentNode() As AnimatTools.DataObjects.Behavior.Node
                Get
                    Return m_bnParentNode
                End Get
                Set(ByVal Value As AnimatTools.DataObjects.Behavior.Node)
                    m_bnParentNode = Value
                End Set
            End Property

            '<Browsable(False)> _
            'Public Overrides Property IsDirty() As Boolean
            '    Get
            '        Return m_bIsDirty
            '    End Get
            '    Set(ByVal Value As Boolean)
            '        m_bIsDirty = True
            '        If Not m_bnParentNode Is Nothing Then m_bnParentNode.IsDirty = True
            '    End Set
            'End Property

#End Region

#Region " Methods "

            Public Sub New(ByVal doParent As AnimatTools.Framework.DataObject)
                MyBase.New(doParent)

                m_snMidPoint = New AnimatTools.Framework.ScaledNumber(Me, "MidPoint", -30, AnimatTools.Framework.ScaledNumber.enumNumericScale.milli, "Volts", "V")
                m_snSlope = New AnimatTools.Framework.ScaledNumber(Me, "Slope", 0.1, AnimatTools.Framework.ScaledNumber.enumNumericScale.None, "", "")
                m_snTimeConstant = New AnimatTools.Framework.ScaledNumber(Me, "TimeConstant", 20, AnimatTools.Framework.ScaledNumber.enumNumericScale.milli, "seconds", "s")
                m_bActivationType = True
            End Sub

            Public Sub New(ByRef bnParent As AnimatTools.DataObjects.Behavior.Node, ByVal bActivationType As Boolean)
                MyBase.New(bnParent)
                m_bnParentNode = bnParent
                m_bActivationType = bActivationType

                If m_bActivationType Then
                    m_snMidPoint = New AnimatTools.Framework.ScaledNumber(Me, "MidPoint", -30, AnimatTools.Framework.ScaledNumber.enumNumericScale.milli, "Volts", "V")
                    m_snSlope = New AnimatTools.Framework.ScaledNumber(Me, "Slope", 0.1, AnimatTools.Framework.ScaledNumber.enumNumericScale.None, "", "")
                    m_snTimeConstant = New AnimatTools.Framework.ScaledNumber(Me, "TimeConstant", 20, AnimatTools.Framework.ScaledNumber.enumNumericScale.milli, "seconds", "s")
                Else
                    m_snMidPoint = New AnimatTools.Framework.ScaledNumber(Me, "MidPoint", -90, AnimatTools.Framework.ScaledNumber.enumNumericScale.milli, "Volts", "V")
                    m_snSlope = New AnimatTools.Framework.ScaledNumber(Me, "Slope", -0.1, AnimatTools.Framework.ScaledNumber.enumNumericScale.None, "", "")
                    m_snTimeConstant = New AnimatTools.Framework.ScaledNumber(Me, "TimeConstant", 7500, AnimatTools.Framework.ScaledNumber.enumNumericScale.milli, "seconds", "s")
                End If

            End Sub

            Public Overrides Function Clone(ByVal doParent As AnimatTools.Framework.DataObject, ByVal bCutData As Boolean, _
                                            ByVal doRoot As AnimatTools.Framework.DataObject) As AnimatTools.Framework.DataObject
                Dim newType As New ActivationType(doParent)
                newType.m_snMidPoint = DirectCast(Me.m_snMidPoint.Clone(newType, bCutData, doRoot), ScaledNumber)
                newType.m_snSlope = DirectCast(Me.m_snSlope.Clone(newType, bCutData, doRoot), ScaledNumber)
                newType.m_snTimeConstant = DirectCast(Me.m_snTimeConstant.Clone(newType, bCutData, doRoot), ScaledNumber)
                newType.m_bActivationType = Me.m_bActivationType
                newType.m_bnParentNode = Me.m_bnParentNode

                Return newType
            End Function

            Protected Overridable Sub UpdateChart()
                If Not m_bnParentNode Is Nothing AndAlso Not m_bnParentNode.ParentDiagram Is Nothing Then
                    m_bnParentNode.ParentDiagram.UpdateChart(DirectCast(m_bnParentNode, AnimatTools.DataObjects.Behavior.Node))
                End If
            End Sub

            Protected Overrides Sub BuildProperties()

                Dim strActivation As String = "Activation"
                If m_bActivationType Then strActivation = "Deactivation"

                Dim pbNumberBag As Crownwood.Magic.Controls.PropertyBag = m_snMidPoint.Properties
                m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("MidPoint", pbNumberBag.GetType(), "MidPoint", _
                                            ("Ca " & strActivation), "Sets the voltage at which, after allowing " & _
                                            "sufficient time to achieve steady state, the Ca " & strActivation & " variable has a value of 0.5." & _
                                            "Acceptable values are in the range -200 to 200 mV.", pbNumberBag, _
                                            "", GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)))

                pbNumberBag = m_snSlope.Properties
                m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Slope", pbNumberBag.GetType(), "Slope", _
                                            ("Ca " & strActivation), "Sets the slope of the Ca " & _
                                            strActivation & " variable against voltage, at the mid-point." & _
                                            "Acceptable values are in the range 0.01 to 10.", pbNumberBag, _
                                            "", GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)))

                pbNumberBag = m_snTimeConstant.Properties
                m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("TimeConstant", pbNumberBag.GetType(), "TimeConstant", _
                                            ("Ca " & strActivation), "Sets the maximum rate (time constant) with which the " & _
                                            strActivation & " variable achieves a new value after a change in voltage." & _
                                            "Acceptable values are in the range 1 to 100000 ms.", pbNumberBag, _
                                            "", GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)))

            End Sub

            Public Overloads Sub LoadData(ByRef oXml As AnimatTools.Interfaces.StdXml, ByVal strName As String)

                oXml.IntoChildElement(strName)

                m_snMidPoint.LoadData(oXml, "MidPoint")
                m_snSlope.LoadData(oXml, "Slope")
                m_snTimeConstant.LoadData(oXml, "TimeConstant")
                m_bActivationType = oXml.GetChildBool("ActivationType")

                oXml.OutOfElem()
            End Sub

            Public Overloads Sub SaveData(ByRef oXml As AnimatTools.Interfaces.StdXml, ByVal strName As String)
                oXml.AddChildElement(strName)
                oXml.IntoElem()

                m_snMidPoint.SaveData(oXml, "MidPoint")
                m_snSlope.SaveData(oXml, "Slope")
                m_snTimeConstant.SaveData(oXml, "TimeConstant")
                oXml.AddChildElement("ActivationType", m_bActivationType)

                oXml.OutOfElem()
            End Sub

#End Region

        End Class

#End Region

    End Class

End Namespace

