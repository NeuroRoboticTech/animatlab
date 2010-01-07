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

    Public Class VoltageClamp
        Inherits AnimatTools.DataObjects.ExternalStimuli.NodeStimulus

#Region " Attributes "

        Protected m_snVtarget As AnimatTools.Framework.ScaledNumber

#End Region

#Region " Properties "

        Public Overrides ReadOnly Property TypeName() As String
            Get
                Return "Voltage Clamp"
            End Get
        End Property

        Public Overrides ReadOnly Property ImageName() As String
            Get
                Return "AnimatTools.Clamp_Large.gif"
            End Get
        End Property

        Public Overrides ReadOnly Property Description() As String
            Get
                Return "This clamps the voltage of a neuron at a target level."
            End Get
        End Property

        Public Overrides ReadOnly Property WorkspaceNodeImageName() As String
            Get
                Return "AnimatTools.Clamp.gif"
            End Get
        End Property

        <Browsable(False)> _
        Public Overridable Property Vtarget() As AnimatTools.Framework.ScaledNumber
            Get
                Return m_snVtarget
            End Get
            Set(ByVal Value As AnimatTools.Framework.ScaledNumber)
                m_snVtarget.CopyData(Value)
            End Set
        End Property

        Public Overrides ReadOnly Property StimulusClassType() As String
            Get
                Return "VoltageClamp"
            End Get
        End Property

        Public Overrides ReadOnly Property ControlType() As String
            Get
                Return "Current"
            End Get
        End Property

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

        Public Overrides ReadOnly Property CanBeCharted() As Boolean
            Get
                Return True
            End Get
        End Property

        Public Overrides ReadOnly Property DragImageName() As String
            Get
                Return Me.ImageName
            End Get
        End Property

        Public Overrides ReadOnly Property DataColumnClassType() As String
            Get
                Return "StimulusData"
            End Get
        End Property

#End Region

#Region " Methods "

        Public Sub New(ByVal doParent As AnimatTools.Framework.DataObject)
            MyBase.New(doParent)

            m_snVtarget = New AnimatTools.Framework.ScaledNumber(Me, "Vtarget", -70, AnimatTools.Framework.ScaledNumber.enumNumericScale.milli, "Volts", "V")

            m_thDataTypes.DataTypes.Add(New AnimatTools.DataObjects.DataType("ClampCurrent", "Clamp Current", "Amps", "A", -100, 100, ScaledNumber.enumNumericScale.nano, ScaledNumber.enumNumericScale.nano))
            m_thDataTypes.ID = "ClampCurrent"

        End Sub

        Public Overrides Function Clone(ByVal doParent As AnimatTools.Framework.DataObject, ByVal bCutData As Boolean, _
                                        ByVal doRoot As AnimatTools.Framework.DataObject) As AnimatTools.Framework.DataObject
            Dim doStim As VoltageClamp = New VoltageClamp(doParent)
            CloneInternal(doStim, bCutData, doRoot)
            If Not doRoot Is Nothing AndAlso doRoot Is Me Then doStim.AfterClone(Me, bCutData, doRoot, doStim)
            Return doStim
        End Function

        Protected Overrides Sub CloneInternal(ByVal doOriginal As AnimatTools.Framework.DataObject, ByVal bCutData As Boolean, _
                                            ByVal doRoot As AnimatTools.Framework.DataObject)
            MyBase.CloneInternal(doOriginal, bCutData, doRoot)

            Dim doCurrent As DataObjects.ExternalStimuli.VoltageClamp = DirectCast(doOriginal, DataObjects.ExternalStimuli.VoltageClamp)

            m_snVtarget = DirectCast(doCurrent.m_snVtarget.Clone(Me, bCutData, doRoot), ScaledNumber)
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

                oXml.AddChildElement("OrganismID", m_doOrganism.ID)
                oXml.AddChildElement("TargetNodeID", m_doNode.NodeIndex)

                oXml.AddChildElement("StartTime", m_snStartTime.ActualValue)
                oXml.AddChildElement("EndTime", m_snEndTime.ActualValue)

                oXml.AddChildElement("Vtarget", m_snVtarget.ActualValue)

                oXml.OutOfElem()
            End If

        End Sub

#Region " DataObject Methods "

        Protected Overrides Sub BuildProperties()
            MyBase.BuildProperties()

            Dim pbNumberBag As Crownwood.Magic.Controls.PropertyBag
            pbNumberBag = m_snVtarget.Properties
            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Clamp Voltage", pbNumberBag.GetType(), "Vtarget", _
                                        "Stimulus Properties", "The voltage to clamp the neuron at.", pbNumberBag, _
                                        "", GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)))

        End Sub

        Public Overrides Sub ClearIsDirty()
            MyBase.ClearIsDirty()

            If Not m_snVtarget Is Nothing Then m_snVtarget.ClearIsDirty()

        End Sub


        Public Overrides Sub LoadData(ByRef oXml As AnimatTools.Interfaces.StdXml)
            MyBase.LoadData(oXml)

            oXml.IntoElem()

            m_snVtarget.LoadData(oXml, "Vtarget")

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

                m_snVtarget.SaveData(oXml, "Vtarget")

                oXml.OutOfElem() ' Outof Node Element
            End If

        End Sub

        Public Overrides Sub SaveDataColumnToXml(ByRef oXml As AnimatTools.Interfaces.StdXml)
            oXml.IntoElem()
            oXml.AddChildElement("StimulusID", Me.ID)
            oXml.OutOfElem()
        End Sub

#End Region

#End Region

    End Class

End Namespace
