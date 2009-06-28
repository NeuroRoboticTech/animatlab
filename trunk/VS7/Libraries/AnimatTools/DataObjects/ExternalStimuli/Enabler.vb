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

    Public Class Enabler
        Inherits AnimatTools.DataObjects.ExternalStimuli.BodyPartStimulus

#Region " Attributes "

        Protected m_bEnableWhenActive As Boolean = True

#End Region

#Region " Properties "

        Public Overridable Property EnableWhenActive() As Boolean
            Get
                Return m_bEnableWhenActive
            End Get
            Set(ByVal Value As Boolean)
                m_bEnableWhenActive = Value
            End Set
        End Property

        Public Overrides ReadOnly Property TypeName() As String
            Get
                Return "Enabler"
            End Get
        End Property

        Public Overrides ReadOnly Property ImageName() As String
            Get
                Return "AnimatTools.EnablerStimulus.gif"
            End Get
        End Property

        Public Overrides ReadOnly Property Description() As String
            Get
                Return "This stimulus enables or disables a body part like a muscle for a given period of time."
            End Get
        End Property

        Public Overrides ReadOnly Property StimulusClassType() As String
            Get
                Return "EnablerInput"
            End Get
        End Property

        Public Overrides ReadOnly Property WorkspaceNodeImageName() As String
            Get
                Return "AnimatTools.EnablerStimulus_Small.gif"
            End Get
        End Property

#End Region

#Region " Methods "

        Public Sub New(ByVal doParent As AnimatTools.Framework.DataObject)
            MyBase.New(doParent)

        End Sub

        Protected Overrides Sub CloneInternal(ByVal doOriginal As AnimatTools.Framework.DataObject, ByVal bCutData As Boolean, _
                                            ByVal doRoot As AnimatTools.Framework.DataObject)
            MyBase.CloneInternal(doOriginal, bCutData, doRoot)

            Dim doPart As DataObjects.ExternalStimuli.Enabler = DirectCast(doOriginal, DataObjects.ExternalStimuli.Enabler)

            m_bEnableWhenActive = doPart.m_bEnableWhenActive

        End Sub

        Public Overrides Function Clone(ByVal doParent As AnimatTools.Framework.DataObject, ByVal bCutData As Boolean, _
                                        ByVal doRoot As AnimatTools.Framework.DataObject) As AnimatTools.Framework.DataObject
            Dim doStim As DataObjects.ExternalStimuli.Enabler = New DataObjects.ExternalStimuli.Enabler(doParent)
            CloneInternal(doStim, bCutData, doRoot)
            If Not doRoot Is Nothing AndAlso doRoot Is Me Then doStim.AfterClone(Me, bCutData, doRoot, doStim)
            Return doStim
        End Function

        Public Overrides Function SaveStimulusToXml() As String

            Dim oXml As New AnimatTools.Interfaces.StdXml

            If m_doStructure Is Nothing Then
                Throw New System.Exception("No structure was defined for the stimulus '" & m_strName & "'.")
            End If

            If m_doBodyPart Is Nothing Then
                Throw New System.Exception("No bodypart was defined for the stimulus '" & m_strName & "'.")
            End If

            oXml.AddElement("Stimuli")
            SaveXml(oXml)

            Return oXml.Serialize()
        End Function

        Public Overrides Sub SaveXml(ByRef oXml As Interfaces.StdXml)

            If m_doStructure Is Nothing Then
                Throw New System.Exception("No structure was defined for the stimulus '" & m_strName & "'.")
            End If

            If m_doBodyPart Is Nothing Then
                Throw New System.Exception("No bodypart was defined for the stimulus '" & m_strName & "'.")
            End If

            oXml.AddChildElement("Stimulus")

            oXml.IntoElem()
            oXml.AddChildElement("ID", m_strID)
            oXml.AddChildElement("Name", m_strName)
            oXml.AddChildElement("AlwaysActive", m_bAlwaysActive)

            oXml.AddChildElement("ModuleName", Me.StimulusModuleName)
            oXml.AddChildElement("Type", Me.StimulusClassType)

            oXml.AddChildElement("StructureID", m_doStructure.ID)
            oXml.AddChildElement("BodyID", m_doBodyPart.ID)

            oXml.AddChildElement("StartTime", m_snStartTime.ActualValue)
            oXml.AddChildElement("EndTime", m_snEndTime.ActualValue)

            oXml.AddChildElement("EnableWhenActive", m_bEnableWhenActive)

            oXml.OutOfElem()

        End Sub

#Region " DataObject Methods "

        Protected Overrides Sub BuildProperties()
            MyBase.BuildProperties()

            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Enable When Active", m_bEnableWhenActive.GetType(), "EnableWhenActive", _
                                        "Stimulus Properties", "If this is true then the body part will be enabled while this stimulus is active and disabled when it is finished. " & _
                                        "If it is false then it will be disabled when the stimulus is active and enabled when it is finished.", m_bEnableWhenActive))

        End Sub

        Public Overrides Sub LoadData(ByRef oXml As AnimatTools.Interfaces.StdXml)
            MyBase.LoadData(oXml)

            oXml.IntoElem()
            m_bEnableWhenActive = oXml.GetChildBool("EnableWhenActive", m_bEnableWhenActive)
            oXml.OutOfElem()

            Me.IsDirty = False
        End Sub

        Public Overrides Sub SaveData(ByRef oXml As AnimatTools.Interfaces.StdXml)
            MyBase.SaveData(oXml)

            oXml.IntoElem()
            oXml.AddChildElement("EnableWhenActive", m_bEnableWhenActive)
            oXml.OutOfElem() ' Outof Node Element

        End Sub

#End Region

#End Region

    End Class

End Namespace

