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

Namespace DataObjects.Gains

    Public Class IonChannelSigmoid
        Inherits AnimatTools.DataObjects.Gain

#Region " Attributes "

        Protected m_snA As AnimatTools.Framework.ScaledNumber
        Protected m_snB As AnimatTools.Framework.ScaledNumber
        Protected m_snC As AnimatTools.Framework.ScaledNumber
        Protected m_snD As AnimatTools.Framework.ScaledNumber
        Protected m_snE As AnimatTools.Framework.ScaledNumber
        Protected m_snF As AnimatTools.Framework.ScaledNumber
        Protected m_snG As AnimatTools.Framework.ScaledNumber

#End Region

#Region " Properties "

        <Browsable(False)> _
        Public Overrides ReadOnly Property GainType() As String
            Get
                Return "Ion Channel Sigmoid Curve"
            End Get
        End Property

        <Browsable(False)> _
        Public Overrides ReadOnly Property Type() As String
            Get
                Return "IonChannelSigmoid"
            End Get
        End Property

        <Browsable(False)> _
        Public Overrides ReadOnly Property ModuleName() As String
            Get
                Return "RealisticNeuralNet"
            End Get
        End Property

        Public Overrides ReadOnly Property GainEquation() As String
            Get
                Return "Y = A + B/(1 + e^(C*(V+D)) + E*e^(F*(V+G))"
            End Get
        End Property

        <Category("Equation Parameters"), _
         Description("Sets the Y offset for the sigmoid for the ion-channel curve."), _
         TypeConverter(GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter))> _
        Public Overridable Property A() As AnimatTools.Framework.ScaledNumber
            Get
                Return m_snA
            End Get
            Set(ByVal Value As AnimatTools.Framework.ScaledNumber)
                Dim snOrig As ScaledNumber = DirectCast(m_snA.Clone(m_snA.Parent, False, Nothing), ScaledNumber)
                If Not Value Is Nothing Then m_snA.CopyData(Value)

                Dim snNew As ScaledNumber = DirectCast(m_snA.Clone(m_snA.Parent, False, Nothing), ScaledNumber)
                Me.ManualAddPropertyHistory("A", snOrig, snNew, True)
                RecalculuateLimits()
            End Set
        End Property

        <Category("Equation Parameters"), _
         Description("Sets the amplitude for the sigmoid for the ion-channel curve."), _
         TypeConverter(GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter))> _
        Public Overridable Property B() As AnimatTools.Framework.ScaledNumber
            Get
                Return m_snB
            End Get
            Set(ByVal Value As AnimatTools.Framework.ScaledNumber)
                Dim snOrig As ScaledNumber = DirectCast(m_snB.Clone(m_snB.Parent, False, Nothing), ScaledNumber)
                If Not Value Is Nothing Then m_snB.CopyData(Value)

                Dim snNew As ScaledNumber = DirectCast(m_snB.Clone(m_snB.Parent, False, Nothing), ScaledNumber)
                Me.ManualAddPropertyHistory("B", snOrig, snNew, True)
                RecalculuateLimits()
            End Set
        End Property

        <Category("Equation Parameters"), _
         Description("Sets how fast the first exponential in the sigmoid rises for the ion-channel curve."), _
         TypeConverter(GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter))> _
        Public Overridable Property C() As AnimatTools.Framework.ScaledNumber
            Get
                Return m_snC
            End Get
            Set(ByVal Value As AnimatTools.Framework.ScaledNumber)
                Dim snOrig As ScaledNumber = DirectCast(m_snC.Clone(m_snC.Parent, False, Nothing), ScaledNumber)
                If Not Value Is Nothing Then m_snC.CopyData(Value)

                Dim snNew As ScaledNumber = DirectCast(m_snC.Clone(m_snC.Parent, False, Nothing), ScaledNumber)
                Me.ManualAddPropertyHistory("C", snOrig, snNew, True)
                RecalculuateLimits()
            End Set
        End Property

        <Category("Equation Parameters"), _
         Description("Sets the half-activation level for the first exponential in the sigmoid for the ion-channel curve."), _
         TypeConverter(GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter))> _
        Public Overridable Property D() As AnimatTools.Framework.ScaledNumber
            Get
                Return m_snD
            End Get
            Set(ByVal Value As AnimatTools.Framework.ScaledNumber)
                Dim snOrig As ScaledNumber = DirectCast(m_snD.Clone(m_snD.Parent, False, Nothing), ScaledNumber)
                If Not Value Is Nothing Then m_snD.CopyData(Value)

                Dim snNew As ScaledNumber = DirectCast(m_snD.Clone(m_snD.Parent, False, Nothing), ScaledNumber)
                Me.ManualAddPropertyHistory("D", snOrig, snNew, True)
                RecalculuateLimits()
            End Set
        End Property

        <Category("Equation Parameters"), _
         Description("Determines whether the second exponential is used in the sigmoidal ion-channel curve."), _
         TypeConverter(GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter))> _
        Public Overridable Property E() As AnimatTools.Framework.ScaledNumber
            Get
                Return m_snE
            End Get
            Set(ByVal Value As AnimatTools.Framework.ScaledNumber)
                Dim snOrig As ScaledNumber = DirectCast(m_snE.Clone(m_snE.Parent, False, Nothing), ScaledNumber)
                If Not Value Is Nothing Then m_snE.CopyData(Value)

                Dim snNew As ScaledNumber = DirectCast(m_snE.Clone(m_snE.Parent, False, Nothing), ScaledNumber)
                Me.ManualAddPropertyHistory("E", snOrig, snNew, True)
                RecalculuateLimits()
            End Set
        End Property


        <Category("Equation Parameters"), _
         Description("Sets how fast the second exponential in the sigmoid rises for the ion-channel curve."), _
         TypeConverter(GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter))> _
        Public Overridable Property F() As AnimatTools.Framework.ScaledNumber
            Get
                Return m_snF
            End Get
            Set(ByVal Value As AnimatTools.Framework.ScaledNumber)
                Dim snOrig As ScaledNumber = DirectCast(m_snF.Clone(m_snF.Parent, False, Nothing), ScaledNumber)
                If Not Value Is Nothing Then m_snF.CopyData(Value)

                Dim snNew As ScaledNumber = DirectCast(m_snF.Clone(m_snF.Parent, False, Nothing), ScaledNumber)
                Me.ManualAddPropertyHistory("F", snOrig, snNew, True)
                RecalculuateLimits()
            End Set
        End Property


        <Category("Equation Parameters"), _
         Description("Sets the half-activation level for the second exponential in the sigmoid for the ion-channel curve."), _
         TypeConverter(GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter))> _
        Public Overridable Property G() As AnimatTools.Framework.ScaledNumber
            Get
                Return m_snG
            End Get
            Set(ByVal Value As AnimatTools.Framework.ScaledNumber)
                Dim snOrig As ScaledNumber = DirectCast(m_snG.Clone(m_snG.Parent, False, Nothing), ScaledNumber)
                If Not Value Is Nothing Then m_snG.CopyData(Value)

                Dim snNew As ScaledNumber = DirectCast(m_snG.Clone(m_snG.Parent, False, Nothing), ScaledNumber)
                Me.ManualAddPropertyHistory("G", snOrig, snNew, True)
                RecalculuateLimits()
            End Set
        End Property

        '<Category("Gain Limits"), _
        ' Description("The tension value at the minimum limit."), _
        ' TypeConverter(GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter))> _
        Public Overridable ReadOnly Property ValAtMin() As AnimatTools.Framework.ScaledNumber
            Get
                Dim snVal As New AnimatTools.Framework.ScaledNumber(Me, "ValAtMin", 0, AnimatTools.Framework.ScaledNumber.enumNumericScale.None, "", "")

                Dim dblGain As Double = CalculateGain(m_snLowerLimit.ActualValue)
                snVal.SetFromValue(dblGain)
                Return snVal
            End Get
        End Property

        '<Category("Gain Limits"), _
        ' Description("The tension value at the maximum limit."), _
        ' TypeConverter(GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter))> _
        Public Overridable ReadOnly Property ValAtMax() As AnimatTools.Framework.ScaledNumber
            Get
                Dim snVal As New AnimatTools.Framework.ScaledNumber(Me, "ValAtMax", 0, AnimatTools.Framework.ScaledNumber.enumNumericScale.None, "", "")

                Dim dblGain As Double = CalculateGain(m_snUpperLimit.ActualValue)
                snVal.SetFromValue(dblGain)
                Return snVal
            End Get
        End Property

        <Browsable(False)> _
        Public Overrides ReadOnly Property SelectableGain() As Boolean
            Get
                Return True
            End Get
        End Property

        <Browsable(False)> _
        Public Overrides ReadOnly Property BarAssemblyFile() As String
            Get
                Return "AnimatTools.dll"
            End Get
        End Property

        <Browsable(False)> _
        Public Overrides ReadOnly Property BarClassName() As String
            Get
                Return "AnimatTools.Forms.Gain.ConfigureGain"
            End Get
        End Property

        <Browsable(False)> _
        Public Overrides ReadOnly Property ImageName() As String
            Get
                Return "RealisticNeuralNetTools.IonChannelGain.gif"
            End Get
        End Property

#End Region

#Region " Methods "

        Public Sub New(ByVal doParent As AnimatTools.Framework.DataObject)
            MyBase.New(doParent)

            m_snA = New AnimatTools.Framework.ScaledNumber(Me, "A", 0, AnimatTools.Framework.ScaledNumber.enumNumericScale.milli, "", "")
            m_snB = New AnimatTools.Framework.ScaledNumber(Me, "B", 1, AnimatTools.Framework.ScaledNumber.enumNumericScale.None, "", "")
            m_snC = New AnimatTools.Framework.ScaledNumber(Me, "C", 100, AnimatTools.Framework.ScaledNumber.enumNumericScale.None, "", "")
            m_snD = New AnimatTools.Framework.ScaledNumber(Me, "D", 40, AnimatTools.Framework.ScaledNumber.enumNumericScale.None.milli, "", "")
            m_snE = New AnimatTools.Framework.ScaledNumber(Me, "E", 0, AnimatTools.Framework.ScaledNumber.enumNumericScale.None, "", "")
            m_snF = New AnimatTools.Framework.ScaledNumber(Me, "F", 0, AnimatTools.Framework.ScaledNumber.enumNumericScale.None, "", "")
            m_snG = New AnimatTools.Framework.ScaledNumber(Me, "G", 0, AnimatTools.Framework.ScaledNumber.enumNumericScale.None.milli, "", "")

            Me.UseLimits = True
            m_snLowerLimit = New AnimatTools.Framework.ScaledNumber(Me, "LowerLimit", -100, AnimatTools.Framework.ScaledNumber.enumNumericScale.milli, "", "V")
            m_snUpperLimit = New AnimatTools.Framework.ScaledNumber(Me, "UpperLimit", 0, AnimatTools.Framework.ScaledNumber.enumNumericScale.milli, "", "")
            m_snLowerOutput = New AnimatTools.Framework.ScaledNumber(Me, "LowerOutput", 0, AnimatTools.Framework.ScaledNumber.enumNumericScale.None, "", "")
            m_snUpperOutput = New AnimatTools.Framework.ScaledNumber(Me, "UpperOutput", 1, AnimatTools.Framework.ScaledNumber.enumNumericScale.None, "", "")

            RecalculuateLimits()

            m_strGainPropertyName = "Ion Channel Gain"
            m_strIndependentUnits = "Membrane Voltage (V)"
            m_strDependentUnits = "Probability"

        End Sub

        Public Sub New(ByVal doParent As AnimatTools.Framework.DataObject, ByVal strGainPropertyName As String, ByVal strIndependentUnits As String, ByVal strDependentUnits As String, _
                       Optional ByVal bLimitsReadOnly As Boolean = False, Optional ByVal bLimitOutputsReadOnly As Boolean = False, Optional ByVal bUseParentIncomingDataType As Boolean = True)
            MyBase.New(doParent)

            m_snA = New AnimatTools.Framework.ScaledNumber(Me, "A", 0, AnimatTools.Framework.ScaledNumber.enumNumericScale.milli, "", "")
            m_snB = New AnimatTools.Framework.ScaledNumber(Me, "B", 1, AnimatTools.Framework.ScaledNumber.enumNumericScale.None, "", "")
            m_snC = New AnimatTools.Framework.ScaledNumber(Me, "C", 100, AnimatTools.Framework.ScaledNumber.enumNumericScale.None, "", "")
            m_snD = New AnimatTools.Framework.ScaledNumber(Me, "D", 40, AnimatTools.Framework.ScaledNumber.enumNumericScale.None.milli, "", "")
            m_snE = New AnimatTools.Framework.ScaledNumber(Me, "E", 0, AnimatTools.Framework.ScaledNumber.enumNumericScale.None, "", "")
            m_snF = New AnimatTools.Framework.ScaledNumber(Me, "F", 0, AnimatTools.Framework.ScaledNumber.enumNumericScale.None, "", "")
            m_snG = New AnimatTools.Framework.ScaledNumber(Me, "G", 0, AnimatTools.Framework.ScaledNumber.enumNumericScale.None.milli, "", "")

            Me.UseLimits = True
            m_snLowerLimit = New AnimatTools.Framework.ScaledNumber(Me, "LowerLimit", -100, AnimatTools.Framework.ScaledNumber.enumNumericScale.milli, "", "")
            m_snUpperLimit = New AnimatTools.Framework.ScaledNumber(Me, "UpperLimit", 0, AnimatTools.Framework.ScaledNumber.enumNumericScale.milli, "", "")
            m_snLowerOutput = New AnimatTools.Framework.ScaledNumber(Me, "LowerOutput", 0, AnimatTools.Framework.ScaledNumber.enumNumericScale.None, "", "")
            m_snUpperOutput = New AnimatTools.Framework.ScaledNumber(Me, "UpperOutput", 1, AnimatTools.Framework.ScaledNumber.enumNumericScale.None, "", "")

            RecalculuateLimits()

            m_strGainPropertyName = strGainPropertyName
            m_strIndependentUnits = strIndependentUnits
            m_strDependentUnits = strDependentUnits
            m_bLimitsReadOnly = bLimitsReadOnly
            m_bLimitOutputsReadOnly = bLimitOutputsReadOnly
            m_bUseParentIncomingDataType = bUseParentIncomingDataType
        End Sub

        Public Overrides Function CalculateGain(ByVal dblInput As Double) As Double

            If (InLimits(dblInput)) Then
                Return (m_snA.ActualValue + (m_snB.ActualValue / (1 + Math.Exp(m_snC.ActualValue * (dblInput + m_snD.ActualValue)) + m_snE.ActualValue * Math.Exp(m_snF.ActualValue * (dblInput * m_snG.ActualValue)))))
            Else
                Return CalculateLimitOutput(dblInput)
            End If

        End Function

        Public Overrides Sub RecalculuateLimits()

            'If Not m_bUseLimits Then
            '    Dim dblVal As Double = m_snC.ActualValue
            '    If dblVal < 0 Then dblVal = dblVal * -1
            '    If dblVal = 0 Then dblVal = 1
            '    Dim dblExtent As Double = (15.88 * Math.Pow(dblVal, -0.8471)) - 1.475
            '    If dblExtent <= 0 Then
            '        dblExtent = (55.44 * Math.Pow(dblVal, -1.387)) + 0.1134
            '        If dblExtent < 0 Then dblExtent = 0.1
            '    End If

            '    dblExtent = (dblExtent + (dblExtent * 0.3)) / 2

            '    m_snLowerLimit.ActualValue = m_snD.ActualValue - dblExtent
            '    m_snUpperLimit.ActualValue = m_snD.ActualValue + dblExtent
            'End If

        End Sub

        Public Overrides Function Clone(ByVal doParent As AnimatTools.Framework.DataObject, ByVal bCutData As Boolean, _
                                        ByVal doRoot As AnimatTools.Framework.DataObject) As AnimatTools.Framework.DataObject
            Dim oNew As New DataObjects.Gains.IonChannelSigmoid(doParent)
            oNew.CloneInternal(Me, bCutData, doRoot)
            If Not doRoot Is Nothing AndAlso doRoot Is Me Then oNew.AfterClone(Me, bCutData, doRoot, oNew)
            Return oNew
        End Function

        Protected Overrides Sub CloneInternal(ByVal doOriginal As AnimatTools.Framework.DataObject, ByVal bCutData As Boolean, _
                                            ByVal doRoot As AnimatTools.Framework.DataObject)
            MyBase.CloneInternal(doOriginal, bCutData, doRoot)

            Dim gnOrig As DataObjects.Gains.IonChannelSigmoid = DirectCast(doOriginal, DataObjects.Gains.IonChannelSigmoid)

            m_snA = DirectCast(gnOrig.m_snA.Clone(Me, bCutData, doRoot), ScaledNumber)
            m_snB = DirectCast(gnOrig.m_snB.Clone(Me, bCutData, doRoot), ScaledNumber)
            m_snC = DirectCast(gnOrig.m_snC.Clone(Me, bCutData, doRoot), ScaledNumber)
            m_snD = DirectCast(gnOrig.m_snD.Clone(Me, bCutData, doRoot), ScaledNumber)
            m_snE = DirectCast(gnOrig.m_snE.Clone(Me, bCutData, doRoot), ScaledNumber)
            m_snF = DirectCast(gnOrig.m_snF.Clone(Me, bCutData, doRoot), ScaledNumber)
            m_snG = DirectCast(gnOrig.m_snG.Clone(Me, bCutData, doRoot), ScaledNumber)

        End Sub

        Public Overloads Overrides Sub LoadData(ByRef oXml As AnimatTools.Interfaces.StdXml, ByVal strName As String, ByVal strGainPropertyName As String)
            Try
                MyBase.LoadData(oXml, strName, strGainPropertyName)

                oXml.IntoElem()

                m_snA.LoadData(oXml, "A")
                m_snB.LoadData(oXml, "B")
                m_snC.LoadData(oXml, "C")
                m_snD.LoadData(oXml, "D")

                m_snE.LoadData(oXml, "E")
                m_snF.LoadData(oXml, "F")
                m_snG.LoadData(oXml, "G")

                RecalculuateLimits()

                oXml.OutOfElem()

                m_strIndependentUnits = "Membrane Voltage (V)"
                m_strDependentUnits = ""
            Catch ex As System.Exception
                oXml.OutOfElem()
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try

        End Sub

        Public Overloads Overrides Sub SaveData(ByRef oXml As AnimatTools.Interfaces.StdXml, ByVal strName As String)
            Try
                MyBase.SaveData(oXml, strName)

                oXml.IntoElem()

                m_snA.SaveData(oXml, "A")
                m_snB.SaveData(oXml, "B")
                m_snC.SaveData(oXml, "C")
                m_snD.SaveData(oXml, "D")
                m_snE.SaveData(oXml, "E")
                m_snF.SaveData(oXml, "F")
                m_snG.SaveData(oXml, "G")

                oXml.OutOfElem()

            Catch ex As System.Exception
                oXml.OutOfElem()
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try

        End Sub

        Public Overrides Sub SaveNetwork(ByRef oXml As AnimatTools.Interfaces.StdXml, ByVal strName As String)
            MyBase.SaveNetwork(oXml, strName)

            oXml.IntoElem()

            oXml.AddChildElement("A", m_snA.ActualValue())
            oXml.AddChildElement("B", m_snB.ActualValue())
            oXml.AddChildElement("C", m_snC.ActualValue())
            oXml.AddChildElement("D", m_snD.ActualValue())
            oXml.AddChildElement("E", m_snE.ActualValue())
            oXml.AddChildElement("F", m_snF.ActualValue())
            oXml.AddChildElement("G", m_snG.ActualValue())

            oXml.OutOfElem()

        End Sub

#Region " DataObject Methods "

        Protected Overrides Sub BuildProperties()
            MyBase.BuildProperties()


            Dim pbNumberBag As Crownwood.Magic.Controls.PropertyBag = m_snA.Properties
            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("A", pbNumberBag.GetType(), "A", _
                                        "Equation Parameters", "Sets the Y offset for the sigmoid for the ion-channel curve.", pbNumberBag, _
                                        "", GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)))

            pbNumberBag = m_snB.Properties
            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("B", pbNumberBag.GetType(), "B", _
                                        "Equation Parameters", "Sets the amplitude for the sigmoid for the ion-channel curve.", pbNumberBag, _
                                        "", GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)))

            pbNumberBag = m_snC.Properties
            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("C", pbNumberBag.GetType(), "C", _
                                        "Equation Parameters", "Sets how fast the first exponential in the sigmoid rises for the ion-channel curve.", pbNumberBag, _
                                        "", GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)))
            pbNumberBag = m_snD.Properties
            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("D", pbNumberBag.GetType(), "D", _
                                        "Equation Parameters", "Sets the half-activation level for the first exponential in the sigmoid for the ion-channel curve.", pbNumberBag, _
                                        "", GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)))
            pbNumberBag = m_snE.Properties
            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("E", pbNumberBag.GetType(), "E", _
                                        "Equation Parameters", "Determines whether the second exponential is used in the sigmoidal ion-channel curve.", pbNumberBag, _
                                        "", GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)))

            pbNumberBag = m_snF.Properties
            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("F", pbNumberBag.GetType(), "F", _
                                        "Equation Parameters", "Sets how fast the second exponential in the sigmoid rises for the ion-channel curve.", pbNumberBag, _
                                        "", GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)))

            pbNumberBag = m_snG.Properties
            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("G", pbNumberBag.GetType(), "G", _
                                        "Equation Parameters", "Sets the half-activation level for the second exponential in the sigmoid for the ion-channel curve.", pbNumberBag, _
                                        "", GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)))

            pbNumberBag = Me.ValAtMin.Properties
            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("ValAtMin", pbNumberBag.GetType(), "ValAtMin", _
                                        "Gain Limits", "The tension value at the minimum limit.", pbNumberBag, _
                                        "", GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)))

            pbNumberBag = Me.ValAtMax.Properties
            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("ValAtMax", pbNumberBag.GetType(), "ValAtMax", _
                                        "Gain Limits", "The tension value at the maximum limit.", pbNumberBag, _
                                        "", GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)))

        End Sub

        Public Overrides Sub ClearIsDirty()
            MyBase.ClearIsDirty()

            If Not m_snA Is Nothing Then m_snA.ClearIsDirty()
            If Not m_snB Is Nothing Then m_snB.ClearIsDirty()
            If Not m_snC Is Nothing Then m_snC.ClearIsDirty()
            If Not m_snD Is Nothing Then m_snD.ClearIsDirty()
            If Not m_snE Is Nothing Then m_snE.ClearIsDirty()
            If Not m_snF Is Nothing Then m_snF.ClearIsDirty()
            If Not m_snG Is Nothing Then m_snG.ClearIsDirty()

        End Sub

#End Region

#End Region

    End Class

End Namespace

