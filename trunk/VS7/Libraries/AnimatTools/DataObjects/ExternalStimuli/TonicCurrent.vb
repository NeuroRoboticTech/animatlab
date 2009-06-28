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

    Public Class TonicCurrent
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

                m_snCycleOnDuration.ActualValue = m_snEndTime.ActualValue - m_snStartTime.ActualValue
                m_snCycleOffDuration.ActualValue = 0
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

                m_snCycleOnDuration.ActualValue = m_snEndTime.ActualValue - m_snStartTime.ActualValue
                m_snCycleOffDuration.ActualValue = 0
                m_snBurstOnDuration.ActualValue = m_snEndTime.ActualValue - m_snStartTime.ActualValue
                m_snBurstOffDuration.ActualValue = 0
            End Set
        End Property

        Public Overrides ReadOnly Property TypeName() As String
            Get
                Return "Tonic Current"
            End Get
        End Property

        Public Overrides ReadOnly Property ImageName() As String
            Get
                Return "AnimatTools.TonicCurrentLarge.gif"
            End Get
        End Property

        Public Overrides ReadOnly Property Description() As String
            Get
                Return "This stimulus injects a constant current into the selected cell for a specific amount of time, or continuously."
            End Get
        End Property

        Public Overrides ReadOnly Property WorkspaceNodeImageName() As String
            Get
                Return "AnimatTools.TonicCurrent.gif"
            End Get
        End Property

        Public Overrides ReadOnly Property CurrentType() As String
            Get
                Return "Tonic"
            End Get
        End Property

#End Region

#Region " Methods "

        Public Sub New(ByVal doParent As AnimatTools.Framework.DataObject)
            MyBase.New(doParent)

            m_snCycleOnDuration = New AnimatTools.Framework.ScaledNumber(Me, "CycleOnDuration", 5, AnimatTools.Framework.ScaledNumber.enumNumericScale.None, "seconds", "s")
            m_snCycleOffDuration = New AnimatTools.Framework.ScaledNumber(Me, "CycleOffDuration", 0, AnimatTools.Framework.ScaledNumber.enumNumericScale.milli, "seconds", "s")
            m_snBurstOnDuration = New AnimatTools.Framework.ScaledNumber(Me, "BurstOnDuration", 5, AnimatTools.Framework.ScaledNumber.enumNumericScale.None, "seconds", "s")
            m_snBurstOffDuration = New AnimatTools.Framework.ScaledNumber(Me, "BurstOffDuration", 0, AnimatTools.Framework.ScaledNumber.enumNumericScale.milli, "seconds", "s")

        End Sub

        Public Overrides Function Clone(ByVal doParent As AnimatTools.Framework.DataObject, ByVal bCutData As Boolean, _
                                        ByVal doRoot As AnimatTools.Framework.DataObject) As AnimatTools.Framework.DataObject
            Dim doStim As TonicCurrent = New TonicCurrent(doParent)
            CloneInternal(doStim, bCutData, doRoot)
            If Not doRoot Is Nothing AndAlso doRoot Is Me Then doStim.AfterClone(Me, bCutData, doRoot, doStim)
            Return doStim
        End Function

#Region " DataObject Methods "

        Protected Overrides Sub BuildProperties()
            MyBase.BuildProperties()

            m_Properties.Properties.Remove("Cycle On Current")
            m_Properties.Properties.Remove("Cycle Off Current")
            m_Properties.Properties.Remove("Burst Off Current")

            m_Properties.Properties.Remove("Cycle On Duration")
            m_Properties.Properties.Remove("Cycle Off Duration")
            m_Properties.Properties.Remove("Burst On Duration")
            m_Properties.Properties.Remove("Burst Off Duration")

            If m_eValueType = enumValueType.Equation Then
                m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Current", m_strEquation.GetType(), "Equation", _
                                            "Stimulus Properties", "If setup to use equations, then this is the one used.", m_strEquation))
            Else
                Dim pbNumberBag As Crownwood.Magic.Controls.PropertyBag = m_snCurrentOn.Properties
                m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Current", pbNumberBag.GetType(), "CurrentOn", _
                                            "Stimulus Properties", "The current applied during the stimulus.", pbNumberBag, _
                                            "", GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)))
            End If



        End Sub

#End Region

#End Region

    End Class

End Namespace
