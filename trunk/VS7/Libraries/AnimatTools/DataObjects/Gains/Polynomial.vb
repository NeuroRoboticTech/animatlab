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

    Public Class Polynomial
        Inherits DataObjects.Gain

#Region " Attributes "

        Protected m_snA As AnimatTools.Framework.ScaledNumber
        Protected m_snB As AnimatTools.Framework.ScaledNumber
        Protected m_snC As AnimatTools.Framework.ScaledNumber
        Protected m_snD As AnimatTools.Framework.ScaledNumber

#End Region

#Region " Properties "

        <Browsable(False)> _
        Public Overrides ReadOnly Property GainType() As String
            Get
                Return "Polynomial Curve"
            End Get
        End Property

        <Browsable(False)> _
        Public Overrides ReadOnly Property Type() As String
            Get
                Return "Polynomial"
            End Get
        End Property

        Public Overrides ReadOnly Property GainEquation() As String
            Get
                Return "Y = A*X^3 + B*X^2 + C*X + D"
            End Get
        End Property

        <Category("Equation Parameters"), _
         Description("Sets the X^3 paramater of the polynomial gain curve."), _
         TypeConverter(GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter))> _
        Public Overridable Property A() As AnimatTools.Framework.ScaledNumber
            Get
                Return m_snA
            End Get
            Set(ByVal Value As AnimatTools.Framework.ScaledNumber)
                Dim gnOrig As Gain = DirectCast(Me.Clone(m_snA.Parent, False, Nothing), Gain)
                If Not Value Is Nothing Then m_snA.CopyData(Value)

                Dim gnNew As Gain = DirectCast(Me.Clone(m_snA.Parent, False, Nothing), Gain)
                Me.ManualAddPropertyHistory(Me.Parent, m_strGainPropertyName, gnOrig, gnNew, True)
                'RecalculuateLimits()
            End Set
        End Property

        <Category("Equation Parameters"), _
         Description("Sets the X^2 paramater of the polynomial gain curve."), _
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
                'RecalculuateLimits()
            End Set
        End Property

        <Category("Equation Parameters"), _
         Description("Sets the X paramater of the polynomial gain curve."), _
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
                'RecalculuateLimits()
            End Set
        End Property

        <Category("Equation Parameters"), _
         Description("Sets the constant paramater of the polynomial gain curve."), _
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
                'RecalculuateLimits()
            End Set
        End Property

#End Region

#Region " Methods "

        Public Sub New(ByVal doParent As AnimatTools.Framework.DataObject)
            MyBase.New(doParent)

            m_snA = New AnimatTools.Framework.ScaledNumber(Me, "A", 0, AnimatTools.Framework.ScaledNumber.enumNumericScale.None, "", "")
            m_snB = New AnimatTools.Framework.ScaledNumber(Me, "B", 0, AnimatTools.Framework.ScaledNumber.enumNumericScale.None, "", "")
            m_snC = New AnimatTools.Framework.ScaledNumber(Me, "C", 1, AnimatTools.Framework.ScaledNumber.enumNumericScale.None, "", "")
            m_snD = New AnimatTools.Framework.ScaledNumber(Me, "D", 0, AnimatTools.Framework.ScaledNumber.enumNumericScale.None, "", "")
        End Sub

        Public Sub New(ByVal doParent As AnimatTools.Framework.DataObject, ByVal strGainPropertyName As String, ByVal strIndependentUnits As String, ByVal strDependentUnits As String, _
                       Optional ByVal bLimitsReadOnly As Boolean = False, Optional ByVal bLimitOutputsReadOnly As Boolean = False, Optional ByVal bUseParentIncomingDataType As Boolean = True)
            MyBase.New(doParent)

            m_snA = New AnimatTools.Framework.ScaledNumber(Me, "A", 0, AnimatTools.Framework.ScaledNumber.enumNumericScale.None, "", "")
            m_snB = New AnimatTools.Framework.ScaledNumber(Me, "B", 0, AnimatTools.Framework.ScaledNumber.enumNumericScale.None, "", "")
            m_snC = New AnimatTools.Framework.ScaledNumber(Me, "C", 1, AnimatTools.Framework.ScaledNumber.enumNumericScale.None, "", "")
            m_snD = New AnimatTools.Framework.ScaledNumber(Me, "D", 0, AnimatTools.Framework.ScaledNumber.enumNumericScale.None, "", "")

            m_strGainPropertyName = strGainPropertyName
            m_strIndependentUnits = strIndependentUnits
            m_strDependentUnits = strDependentUnits
            m_bLimitsReadOnly = bLimitsReadOnly
            m_bLimitOutputsReadOnly = bLimitOutputsReadOnly
            m_bUseParentIncomingDataType = bUseParentIncomingDataType
        End Sub

        Public Overrides Function CalculateGain(ByVal dblInput As Double) As Double
            'Gain = A*x^3 + B*x^2 + C*x + D
            If (InLimits(dblInput)) Then
                Return ((m_snA.ActualValue * dblInput * dblInput * dblInput) + (m_snB.ActualValue * dblInput * dblInput) + (m_snC.ActualValue * dblInput) + m_snD.ActualValue)
            Else
                Return CalculateLimitOutput(dblInput)
            End If
        End Function

        Public Overrides Function Clone(ByVal doParent As AnimatTools.Framework.DataObject, ByVal bCutData As Boolean, _
                                        ByVal doRoot As AnimatTools.Framework.DataObject) As AnimatTools.Framework.DataObject
            Dim oNew As New DataObjects.Gains.Polynomial(doParent)
            oNew.CloneInternal(Me, bCutData, doRoot)
            If Not doRoot Is Nothing AndAlso doRoot Is Me Then oNew.AfterClone(Me, bCutData, doRoot, oNew)
            Return oNew
        End Function

        Protected Overrides Sub CloneInternal(ByVal doOriginal As AnimatTools.Framework.DataObject, ByVal bCutData As Boolean, _
                                            ByVal doRoot As AnimatTools.Framework.DataObject)
            MyBase.CloneInternal(doOriginal, bCutData, doRoot)

            Dim gnOrig As DataObjects.Gains.Polynomial = DirectCast(doOriginal, DataObjects.Gains.Polynomial)

            m_snA = DirectCast(gnOrig.m_snA.Clone(Me, bCutData, doRoot), ScaledNumber)
            m_snB = DirectCast(gnOrig.m_snB.Clone(Me, bCutData, doRoot), ScaledNumber)
            m_snC = DirectCast(gnOrig.m_snC.Clone(Me, bCutData, doRoot), ScaledNumber)
            m_snD = DirectCast(gnOrig.m_snD.Clone(Me, bCutData, doRoot), ScaledNumber)

        End Sub

        Public Overloads Overrides Sub LoadData(ByRef oXml As AnimatTools.Interfaces.StdXml, ByVal strName As String, ByVal strGainPropertyName As String)
            MyBase.LoadData(oXml, strName, strGainPropertyName)

            oXml.IntoElem()

            If oXml.FindChildElement("AScale", False) Then
                m_snA.LoadData(oXml, "AScale")
                m_snB.LoadData(oXml, "BScale")
                m_snC.LoadData(oXml, "CScale")
                m_snD.LoadData(oXml, "DScale")
            Else
                m_snA.LoadData(oXml, "A")
                m_snB.LoadData(oXml, "B")
                m_snC.LoadData(oXml, "C")
                m_snD.LoadData(oXml, "D")
            End If

            oXml.OutOfElem()

        End Sub


        Public Overloads Overrides Sub SaveData(ByRef oXml As AnimatTools.Interfaces.StdXml, ByVal strName As String)
            MyBase.SaveData(oXml, strName)

            oXml.IntoElem()

            m_snA.SaveData(oXml, "A")
            m_snB.SaveData(oXml, "B")
            m_snC.SaveData(oXml, "C")
            m_snD.SaveData(oXml, "D")

            oXml.OutOfElem()

        End Sub

        Public Overrides Sub SaveNetwork(ByRef oXml As AnimatTools.Interfaces.StdXml, ByVal strName As String)
            MyBase.SaveNetwork(oXml, strName)

            oXml.IntoElem()

            oXml.AddChildElement("A", m_snA.ActualValue())
            oXml.AddChildElement("B", m_snB.ActualValue())
            oXml.AddChildElement("C", m_snC.ActualValue())
            oXml.AddChildElement("D", m_snD.ActualValue())

            oXml.OutOfElem()

        End Sub

#Region " DataObject Methods "

        Protected Overrides Sub BuildProperties()
            MyBase.BuildProperties()

            Dim pbNumberBag As Crownwood.Magic.Controls.PropertyBag = m_snA.Properties
            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("A", pbNumberBag.GetType(), "A", _
                                        "Equation Parameters", "Sets the X^3 paramater of the polynomial gain curve.", pbNumberBag, _
                                        "", GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)))

            pbNumberBag = m_snB.Properties
            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("B", pbNumberBag.GetType(), "B", _
                                        "Equation Parameters", "Sets the X^2 paramater of the polynomial gain curve.", pbNumberBag, _
                                        "", GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)))

            pbNumberBag = m_snC.Properties
            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("C", pbNumberBag.GetType(), "C", _
                                        "Equation Parameters", "Sets the X paramater of the polynomial gain curve.", pbNumberBag, _
                                        "", GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)))

            pbNumberBag = m_snD.Properties
            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("D", pbNumberBag.GetType(), "D", _
                                        "Equation Parameters", "Sets the constant paramater of the polynomial gain curve.", pbNumberBag, _
                                        "", GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)))

        End Sub

        Public Overrides Sub ClearIsDirty()
            MyBase.ClearIsDirty()

            If Not m_snA Is Nothing Then m_snA.ClearIsDirty()
            If Not m_snB Is Nothing Then m_snB.ClearIsDirty()
            If Not m_snC Is Nothing Then m_snC.ClearIsDirty()
            If Not m_snD Is Nothing Then m_snD.ClearIsDirty()
        End Sub

#End Region

#End Region

    End Class

End Namespace
