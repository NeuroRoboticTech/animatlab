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

    Public Class RepetitiveCurrent
        Inherits AnimatTools.DataObjects.ExternalStimuli.Current

#Region " Attributes "

#End Region

#Region " Properties "

        Public Overrides Property StartTime() As AnimatTools.Framework.ScaledNumber
            Get
                Return m_snStartTime
            End Get
            Set(ByVal Value As AnimatTools.Framework.ScaledNumber)
                If Value.ActualValue < 0 Then
                    Throw New System.Exception("The start time must be greater than 0.")
                End If

                If Value.ActualValue >= m_snEndTime.ActualValue Then
                    Throw New System.Exception("The start time must be less than the end time.")
                End If

                m_snStartTime.CopyData(Value)

                m_snBurstOnDuration.ActualValue = m_snEndTime.ActualValue - m_snStartTime.ActualValue
                m_snBurstOffDuration.ActualValue = 0

            End Set
        End Property

        Public Overrides Property EndTime() As AnimatTools.Framework.ScaledNumber
            Get
                Return m_snEndTime
            End Get
            Set(ByVal Value As AnimatTools.Framework.ScaledNumber)
                If Value.ActualValue < 0 Then
                    Throw New System.Exception("The end time must be greater than 0.")
                End If

                If Value.ActualValue <= m_snStartTime.ActualValue Then
                    Throw New System.Exception("The end time must be greater than the end time.")
                End If

                m_snEndTime.CopyData(Value)

                m_snBurstOnDuration.ActualValue = m_snEndTime.ActualValue - m_snStartTime.ActualValue
                m_snBurstOffDuration.ActualValue = 0
            End Set
        End Property

        Public Overrides ReadOnly Property TypeName() As String
            Get
                Return "Repetitive Current"
            End Get
        End Property

        Public Overrides ReadOnly Property ImageName() As String
            Get
                Return "AnimatTools.RepetitiveCurrentLarge.gif"
            End Get
        End Property

        Public Overrides ReadOnly Property Description() As String
            Get
                Return "This stimulus applies an on and off current in repetitive cycles. So you could have an " & _
                       "On current of 20 na for 50ms and then have an off current of -10na for 20 ms. You can apply " & _
                       "the current for a spefic duration, or continuously."
            End Get
        End Property

        Public Overrides ReadOnly Property WorkspaceNodeImageName() As String
            Get
                Return "AnimatTools.RepetitiveCurrent.gif"
            End Get
        End Property

        Public Overrides ReadOnly Property CurrentType() As String
            Get
                Return "Repetitive"
            End Get
        End Property

#End Region

#Region " Methods "

        Public Sub New(ByVal doParent As AnimatTools.Framework.DataObject)
            MyBase.New(doParent)

            m_snBurstOnDuration = New AnimatTools.Framework.ScaledNumber(Me, "BurstOnDuration", 5, AnimatTools.Framework.ScaledNumber.enumNumericScale.None, "seconds", "s")
            m_snBurstOffDuration = New AnimatTools.Framework.ScaledNumber(Me, "BurstOffDuration", 0, AnimatTools.Framework.ScaledNumber.enumNumericScale.milli, "seconds", "s")
        End Sub

        Public Overrides Function Clone(ByVal doParent As AnimatTools.Framework.DataObject, ByVal bCutData As Boolean, _
                                        ByVal doRoot As AnimatTools.Framework.DataObject) As AnimatTools.Framework.DataObject
            Dim doStim As RepetitiveCurrent = New RepetitiveCurrent(doParent)
            CloneInternal(doStim, bCutData, doRoot)
            If Not doRoot Is Nothing AndAlso doRoot Is Me Then doStim.AfterClone(Me, bCutData, doRoot, doStim)
            Return doStim
        End Function

        Public Overrides Function SaveStimulusToXml() As String

            If Me.AlwaysActive Then
                m_snBurstOnDuration.ActualValue = 20000
                m_snBurstOffDuration.ActualValue = 0
            Else
                m_snBurstOnDuration.ActualValue = m_snEndTime.ActualValue - m_snStartTime.ActualValue
                m_snBurstOffDuration.ActualValue = 0
            End If

            Return MyBase.SaveStimulusToXml()
        End Function

#Region " DataObject Methods "

        Protected Overrides Sub BuildProperties()
            MyBase.BuildProperties()

            m_Properties.Properties.Remove("Burst Off Current")
            m_Properties.Properties.Remove("Burst On Duration")
            m_Properties.Properties.Remove("Burst Off Duration")

        End Sub

#End Region

#End Region

    End Class

End Namespace

