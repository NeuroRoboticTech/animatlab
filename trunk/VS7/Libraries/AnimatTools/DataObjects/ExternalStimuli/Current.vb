Imports System
Imports System.Drawing
Imports System.Collections
Imports System.ComponentModel
Imports System.Windows.Forms
Imports System.Diagnostics
Imports System.IO
Imports System.Xml
Imports Crownwood.Magic.Common
Imports Crownwood.Magic.Controls
Imports Crownwood.Magic.Docking
Imports Crownwood.Magic.Menus
Imports AnimatTools.Framework

Namespace DataObjects.ExternalStimuli

    Public MustInherit Class Current
        Inherits AnimatTools.DataObjects.ExternalStimuli.NodeStimulus

#Region " Attributes "

        Protected m_snCurrentOn As AnimatTools.Framework.ScaledNumber
        Protected m_snCurrentOff As AnimatTools.Framework.ScaledNumber
        Protected m_snCurrentBurstOff As AnimatTools.Framework.ScaledNumber

        Protected m_snCycleOnDuration As AnimatTools.Framework.ScaledNumber
        Protected m_snCycleOffDuration As AnimatTools.Framework.ScaledNumber
        Protected m_snBurstOnDuration As AnimatTools.Framework.ScaledNumber
        Protected m_snBurstOffDuration As AnimatTools.Framework.ScaledNumber

#End Region

#Region " Properties "

        <Browsable(False)> _
        Public Overridable Property CurrentOn() As AnimatTools.Framework.ScaledNumber
            Get
                Return m_snCurrentOn
            End Get
            Set(ByVal Value As AnimatTools.Framework.ScaledNumber)
                m_snCurrentOn.CopyData(Value)
            End Set
        End Property

        <Browsable(False)> _
        Public Overridable Property CurrentOff() As AnimatTools.Framework.ScaledNumber
            Get
                Return m_snCurrentOff
            End Get
            Set(ByVal Value As AnimatTools.Framework.ScaledNumber)
                m_snCurrentOff.CopyData(Value)
            End Set
        End Property

        <Browsable(False)> _
        Public Overridable Property CurrentBurstOff() As AnimatTools.Framework.ScaledNumber
            Get
                Return m_snCurrentBurstOff
            End Get
            Set(ByVal Value As AnimatTools.Framework.ScaledNumber)
                m_snCurrentBurstOff.CopyData(Value)
            End Set
        End Property

        <Browsable(False)> _
        Public Overridable Property CycleOnDuration() As AnimatTools.Framework.ScaledNumber
            Get
                Return m_snCycleOnDuration
            End Get
            Set(ByVal Value As AnimatTools.Framework.ScaledNumber)
                If Value.ActualValue <= 0 Then
                    Throw New System.Exception("The cycle on duration must be greater than 0.")
                End If

                m_snCycleOnDuration.CopyData(Value)
            End Set
        End Property

        <Browsable(False)> _
        Public Overridable Property CycleOffDuration() As AnimatTools.Framework.ScaledNumber
            Get
                Return m_snCycleOffDuration
            End Get
            Set(ByVal Value As AnimatTools.Framework.ScaledNumber)
                If Value.ActualValue < 0 Then
                    Throw New System.Exception("The cycle off duration must be greater than or equal to 0.")
                End If

                m_snCycleOffDuration.CopyData(Value)
            End Set
        End Property

        <Browsable(False)> _
        Public Overridable Property BurstOnDuration() As AnimatTools.Framework.ScaledNumber
            Get
                Return m_snBurstOnDuration
            End Get
            Set(ByVal Value As AnimatTools.Framework.ScaledNumber)
                If Value.ActualValue <= 0 Then
                    Throw New System.Exception("The burst on duration must be greater than 0.")
                End If

                m_snBurstOnDuration.CopyData(Value)
            End Set
        End Property

        <Browsable(False)> _
        Public Overridable Property BurstOffDuration() As AnimatTools.Framework.ScaledNumber
            Get
                Return m_snBurstOffDuration
            End Get
            Set(ByVal Value As AnimatTools.Framework.ScaledNumber)
                If Value.ActualValue < 0 Then
                    Throw New System.Exception("The burst off duration must be greater than or equal to 0.")
                End If

                m_snBurstOffDuration.CopyData(Value)
            End Set
        End Property

        Public Overrides ReadOnly Property StimulusClassType() As String
            Get
                Return "Current"
            End Get
        End Property

        Public MustOverride ReadOnly Property CurrentType() As String

        <Browsable(False)> _
        Public Overrides ReadOnly Property StimulusNoLongerValid() As Boolean
            Get

                Try
                    m_doNode = m_doOrganism.FindBehavioralNode(m_doNode.ID, False)
                Catch ex As System.Exception
                End Try

                If m_doNode Is Nothing Then
                    Return True
                Else
                    Return False
                End If
            End Get
        End Property

#End Region

#Region " Methods "

        Public Sub New(ByVal doParent As AnimatTools.Framework.DataObject)
            MyBase.New(doParent)

            m_snCurrentOn = New AnimatTools.Framework.ScaledNumber(Me, "CurrentOn", 10, AnimatTools.Framework.ScaledNumber.enumNumericScale.nano, "Amps", "A")
            m_snCurrentOff = New AnimatTools.Framework.ScaledNumber(Me, "CurrentOff", 0, AnimatTools.Framework.ScaledNumber.enumNumericScale.nano, "Amps", "A")
            m_snCurrentBurstOff = New AnimatTools.Framework.ScaledNumber(Me, "CurrentBurstOff", 0, AnimatTools.Framework.ScaledNumber.enumNumericScale.nano, "Amps", "A")

            m_snCycleOnDuration = New AnimatTools.Framework.ScaledNumber(Me, "CycleOnDuration", 20, AnimatTools.Framework.ScaledNumber.enumNumericScale.milli, "seconds", "s")
            m_snCycleOffDuration = New AnimatTools.Framework.ScaledNumber(Me, "CycleOffDuration", 0, AnimatTools.Framework.ScaledNumber.enumNumericScale.milli, "seconds", "s")
            m_snBurstOnDuration = New AnimatTools.Framework.ScaledNumber(Me, "BurstOnDuration", 20, AnimatTools.Framework.ScaledNumber.enumNumericScale.milli, "seconds", "s")
            m_snBurstOffDuration = New AnimatTools.Framework.ScaledNumber(Me, "BurstOffDuration", 0, AnimatTools.Framework.ScaledNumber.enumNumericScale.milli, "seconds", "s")

        End Sub

        Protected Overrides Sub CloneInternal(ByVal doOriginal As AnimatTools.Framework.DataObject, ByVal bCutData As Boolean, _
                                            ByVal doRoot As AnimatTools.Framework.DataObject)
            MyBase.CloneInternal(doOriginal, bCutData, doRoot)

            Dim doCurrent As DataObjects.ExternalStimuli.Current = DirectCast(doOriginal, DataObjects.ExternalStimuli.Current)

            m_snCurrentOn = DirectCast(doCurrent.m_snCurrentOn.Clone(Me, bCutData, doRoot), ScaledNumber)
            m_snCurrentOff = DirectCast(doCurrent.m_snCurrentOff.Clone(Me, bCutData, doRoot), ScaledNumber)
            m_snCurrentBurstOff = DirectCast(doCurrent.m_snCurrentBurstOff.Clone(Me, bCutData, doRoot), ScaledNumber)

            m_snCycleOnDuration = DirectCast(doCurrent.m_snCycleOnDuration.Clone(Me, bCutData, doRoot), ScaledNumber)
            m_snCycleOffDuration = DirectCast(doCurrent.m_snCycleOffDuration.Clone(Me, bCutData, doRoot), ScaledNumber)
            m_snBurstOnDuration = DirectCast(doCurrent.m_snBurstOnDuration.Clone(Me, bCutData, doRoot), ScaledNumber)
            m_snBurstOffDuration = DirectCast(doCurrent.m_snBurstOffDuration.Clone(Me, bCutData, doRoot), ScaledNumber)
        End Sub

        Public Overrides Function SaveStimulusToXml() As String

            Dim oXml As New AnimatTools.Interfaces.StdXml

            If m_doOrganism Is Nothing Then
                Throw New System.Exception("No organism was defined for the stimulus '" & m_strName & "'.")
            End If

            If Not m_doNode Is Nothing Then
                m_doNode = m_doOrganism.FindBehavioralNode(m_doNode.ID, False)
            End If

            'We need to get a new reference to the neuron here because it may be different than the one we originally got. The reason is that when the
            'project is first loaded we load in a list of the neurons. But if the user opens the behavioral editor then we need to reload that list because
            'we have to seperate out the neurons further by subsystem. So the second time they are loaded they would be a different object. Items like the 
            'ID should not be different, but changes to the node index would be different.
            If Not m_doNode Is Nothing Then
                oXml.AddElement("Stimuli")
                SaveXml(oXml)
            End If

            Return oXml.Serialize()
        End Function

        Public Overrides Sub SaveXml(ByRef oXml As Interfaces.StdXml)

            If m_doOrganism Is Nothing Then
                Throw New System.Exception("No organism was defined for the stimulus '" & m_strName & "'.")
            End If

            If Not m_doNode Is Nothing Then
                m_doNode = m_doOrganism.FindBehavioralNode(m_doNode.ID, False)
            End If

            'We need to get a new reference to the neuron here because it may be different than the one we originally got. The reason is that when the
            'project is first loaded we load in a list of the neurons. But if the user opens the behavioral editor then we need to reload that list because
            'we have to seperate out the neurons further by subsystem. So the second time they are loaded they would be a different object. Items like the 
            'ID should not be different, but changes to the node index would be different.
            If Not m_doNode Is Nothing Then

                oXml.AddChildElement("Stimulus")

                oXml.IntoElem()
                oXml.AddChildElement("ID", m_strID)
                oXml.AddChildElement("Name", m_strName)
                oXml.AddChildElement("AlwaysActive", m_bAlwaysActive)

                oXml.AddChildElement("ModuleName", Me.StimulusModuleName)
                oXml.AddChildElement("Type", Me.StimulusClassType)

                oXml.AddChildElement("CurrentType", Me.CurrentType)

                oXml.AddChildElement("OrganismID", m_doOrganism.ID)
                oXml.AddChildElement("TargetNodeID", m_doNode.NodeIndex)

                oXml.AddChildElement("StartTime", m_snStartTime.ActualValue)
                oXml.AddChildElement("EndTime", m_snEndTime.ActualValue)

                oXml.AddChildElement("CycleOnDuration", m_snCycleOnDuration.ActualValue)
                oXml.AddChildElement("CycleOffDuration", m_snCycleOffDuration.ActualValue)
                oXml.AddChildElement("BurstOnDuration", m_snBurstOnDuration.ActualValue)
                oXml.AddChildElement("BurstOffDuration", m_snBurstOffDuration.ActualValue)

                oXml.AddChildElement("CurrentOn", m_snCurrentOn.ActualValue)
                oXml.AddChildElement("CurrentOff", m_snCurrentOff.ActualValue)
                oXml.AddChildElement("CurrentBurstOff", m_snCurrentBurstOff.ActualValue)

                If m_eValueType = enumValueType.Equation Then
                    'We need to convert the infix equation to postfix
                    Dim oMathEval As New MathStringEval
                    oMathEval.AddVariable("t")
                    oMathEval.Equation = m_strEquation
                    oMathEval.Parse()
                    oXml.AddChildElement("CurrentOnEquation", oMathEval.PostFix)
                End If

                oXml.OutOfElem()
            End If

        End Sub

#Region " DataObject Methods "

        Protected Overrides Sub BuildProperties()
            MyBase.BuildProperties()

            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Value Type", m_eValueType.GetType(), "ValueType", _
                                        "Stimulus Properties", "Determines if a constant or an equation is used to determine the tonic current.", m_eValueType))

            Dim pbNumberBag As Crownwood.Magic.Controls.PropertyBag
            If m_eValueType = enumValueType.Equation Then
                m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Cycle On Current", m_strEquation.GetType(), "Equation", _
                                            "Stimulus Properties", "The current applied when a cycle is on.", m_strEquation))
            Else
                pbNumberBag = m_snCurrentOn.Properties
                m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Cycle On Current", pbNumberBag.GetType(), "CurrentOn", _
                                            "Stimulus Properties", "The current applied when a cycle is on.", pbNumberBag, _
                                            "", GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)))
            End If

            pbNumberBag = m_snCurrentOff.Properties
            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Cycle Off Current", pbNumberBag.GetType(), "CurrentOff", _
                                        "Stimulus Properties", "The current applied when a cycle is off.", pbNumberBag, _
                                        "", GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)))

            pbNumberBag = m_snCurrentBurstOff.Properties
            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Burst Off Current", pbNumberBag.GetType(), "CurrentBurstOff", _
                                        "Stimulus Properties", "The current applied when a burst is off.", pbNumberBag, _
                                        "", GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)))


            pbNumberBag = m_snCycleOnDuration.Properties
            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Cycle On Duration", pbNumberBag.GetType(), "CycleOnDuration", _
                                        "Stimulus Properties", "The duration of the on portion of the cycle.", pbNumberBag, _
                                        "", GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)))

            pbNumberBag = m_snCycleOffDuration.Properties
            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Cycle Off Duration", pbNumberBag.GetType(), "CycleOffDuration", _
                                        "Stimulus Properties", "The duration of the off portion of the cycle.", pbNumberBag, _
                                        "", GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)))

            pbNumberBag = m_snBurstOnDuration.Properties
            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Burst On Duration", pbNumberBag.GetType(), "BurstOnDuration", _
                                        "Stimulus Properties", "The duration of the on portion of the burst.", pbNumberBag, _
                                        "", GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)))

            pbNumberBag = m_snBurstOffDuration.Properties
            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Burst Off Duration", pbNumberBag.GetType(), "BurstOffDuration", _
                                        "Stimulus Properties", "The duration of the off portion of the burst.", pbNumberBag, _
                                        "", GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)))

        End Sub

        Public Overrides Sub ClearIsDirty()
            MyBase.ClearIsDirty()

            If Not m_snCurrentOn Is Nothing Then m_snCurrentOn.ClearIsDirty()
            If Not m_snCurrentOff Is Nothing Then m_snCurrentOff.ClearIsDirty()
            If Not m_snCurrentBurstOff Is Nothing Then m_snCurrentBurstOff.ClearIsDirty()

            If Not m_snCycleOnDuration Is Nothing Then m_snCycleOnDuration.ClearIsDirty()
            If Not m_snCycleOffDuration Is Nothing Then m_snCycleOffDuration.ClearIsDirty()
            If Not m_snBurstOnDuration Is Nothing Then m_snBurstOnDuration.ClearIsDirty()
            If Not m_snBurstOffDuration Is Nothing Then m_snBurstOffDuration.ClearIsDirty()

        End Sub


        Public Overrides Sub LoadData(ByRef oXml As AnimatTools.Interfaces.StdXml)
            MyBase.LoadData(oXml)

            oXml.IntoElem()

            m_snCurrentOn.LoadData(oXml, "CurrentOn")
            m_snCurrentOff.LoadData(oXml, "CurrentOff")
            m_snCurrentBurstOff.LoadData(oXml, "CurrentBurstOff")

            m_snCycleOnDuration.LoadData(oXml, "CycleOnDuration")
            m_snCycleOffDuration.LoadData(oXml, "CycleOffDuration")
            m_snBurstOnDuration.LoadData(oXml, "BurstOnDuration")
            m_snBurstOffDuration.LoadData(oXml, "BurstOffDuration")

            oXml.OutOfElem()

            Me.IsDirty = False
        End Sub

        Public Overrides Sub SaveData(ByRef oXml As AnimatTools.Interfaces.StdXml)

            If Not m_doNode Is Nothing Then
                m_doNode = m_doOrganism.FindBehavioralNode(m_doNode.ID, False)
            End If

            If Not m_doNode Is Nothing Then
                MyBase.SaveData(oXml)

                oXml.IntoElem()

                m_snCurrentOn.SaveData(oXml, "CurrentOn")
                m_snCurrentOff.SaveData(oXml, "CurrentOff")
                m_snCurrentBurstOff.SaveData(oXml, "CurrentBurstOff")

                m_snCycleOnDuration.SaveData(oXml, "CycleOnDuration")
                m_snCycleOffDuration.SaveData(oXml, "CycleOffDuration")
                m_snBurstOnDuration.SaveData(oXml, "BurstOnDuration")
                m_snBurstOffDuration.SaveData(oXml, "BurstOffDuration")

                oXml.OutOfElem() ' Outof Node Element
            End If

        End Sub

#End Region

#End Region

    End Class

End Namespace
