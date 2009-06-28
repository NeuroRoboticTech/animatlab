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

Namespace DataObjects

    Public MustInherit Class Gain
        Inherits Framework.DataObject

#Region " Attributes "

        Protected m_bUseLimits As Boolean
        Protected m_bLimitsReadOnly As Boolean
        Protected m_bLimitOutputsReadOnly As Boolean
        Protected m_bUseParentIncomingDataType As Boolean

        Protected m_snLowerLimit As AnimatTools.Framework.ScaledNumber
        Protected m_snUpperLimit As AnimatTools.Framework.ScaledNumber
        Protected m_snLowerOutput As AnimatTools.Framework.ScaledNumber
        Protected m_snUpperOutput As AnimatTools.Framework.ScaledNumber

        Protected m_strIndependentUnits As String = ""
        Protected m_strDependentUnits As String = ""

        Protected m_bdParentData As Behavior.Data
        Protected m_strGainPropertyName As String = ""

        Protected m_Image As Image

#End Region

#Region " Properties "

        <Browsable(False)> _
        Public MustOverride ReadOnly Property GainType() As String

        <Browsable(False)> _
        Public MustOverride ReadOnly Property Type() As String

        <Browsable(False)> _
        Public MustOverride ReadOnly Property GainEquation() As String

        <Browsable(False)> _
        Public Overridable Property GainPropertyName() As String
            Get
                Return m_strGainPropertyName
            End Get
            Set(ByVal Value As String)
                m_strGainPropertyName = Value
            End Set
        End Property

        <Browsable(False)> _
        Public Overridable Property UseLimits() As Boolean
            Get
                Return m_bUseLimits
            End Get
            Set(ByVal Value As Boolean)
                Me.ManualAddPropertyHistory("UseLimits", m_bUseLimits, Value, True)

                m_bUseLimits = Value
            End Set
        End Property

        <Browsable(False)> _
        Public Overridable Property LimitsReadOnly() As Boolean
            Get
                Return m_bLimitsReadOnly
            End Get
            Set(ByVal Value As Boolean)
                Me.ManualAddPropertyHistory("LimitsReadOnly", m_bLimitsReadOnly, Value, True)

                m_bLimitsReadOnly = Value
                If m_bLimitsReadOnly Then m_bUseLimits = True
            End Set
        End Property

        <Browsable(False)> _
        Public Overridable Property LimitOutputsReadOnly() As Boolean
            Get
                Return m_bLimitOutputsReadOnly
            End Get
            Set(ByVal Value As Boolean)
                Me.ManualAddPropertyHistory("LimitOutputsReadOnly", m_bLimitOutputsReadOnly, Value, True)

                m_bLimitOutputsReadOnly = Value
                If m_bLimitsReadOnly Then m_bUseLimits = True
            End Set
        End Property

        '<Category("Gain Limits"), _
        ' Description("Sets the lower limit of the x variable."), _
        ' TypeConverter(GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter))> _
        <Browsable(False)> _
        Public Overridable Property LowerLimit() As AnimatTools.Framework.ScaledNumber
            Get
                Return m_snLowerLimit
            End Get
            Set(ByVal Value As AnimatTools.Framework.ScaledNumber)
                Dim snOrig As ScaledNumber = DirectCast(m_snLowerLimit.Clone(m_snLowerLimit.Parent, False, Nothing), ScaledNumber)
                If Not Value Is Nothing Then m_snLowerLimit.CopyData(Value)

                Dim snNew As ScaledNumber = DirectCast(m_snLowerLimit.Clone(m_snLowerLimit.Parent, False, Nothing), ScaledNumber)
                Me.ManualAddPropertyHistory("LowerLimit", snOrig, snNew, True)
            End Set
        End Property

        '<Category("Gain Limits"), _
        ' Description("Sets the upper limit of the x variable."), _
        ' TypeConverter(GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter))> _
        <Browsable(False)> _
        Public Overridable Property UpperLimit() As AnimatTools.Framework.ScaledNumber
            Get
                Return m_snUpperLimit
            End Get
            Set(ByVal Value As AnimatTools.Framework.ScaledNumber)
                Dim snOrig As ScaledNumber = DirectCast(m_snUpperLimit.Clone(m_snUpperLimit.Parent, False, Nothing), ScaledNumber)
                If Not Value Is Nothing Then m_snUpperLimit.CopyData(Value)

                Dim snNew As ScaledNumber = DirectCast(m_snUpperLimit.Clone(m_snUpperLimit.Parent, False, Nothing), ScaledNumber)
                Me.ManualAddPropertyHistory("UpperLimit", snOrig, snNew, True)
            End Set
        End Property

        '<Category("Gain Limits"), _
        ' Description("Sets the output value to use when the x value is less than the lower limit."), _
        ' TypeConverter(GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter))> _
        <Browsable(False)> _
        Public Overridable Property LowerOutput() As AnimatTools.Framework.ScaledNumber
            Get
                Return m_snLowerOutput
            End Get
            Set(ByVal Value As AnimatTools.Framework.ScaledNumber)
                Dim snOrig As ScaledNumber = DirectCast(m_snLowerOutput.Clone(m_snLowerOutput.Parent, False, Nothing), ScaledNumber)
                If Not Value Is Nothing Then m_snLowerOutput.CopyData(Value)

                Dim snNew As ScaledNumber = DirectCast(m_snLowerOutput.Clone(m_snLowerOutput.Parent, False, Nothing), ScaledNumber)
                Me.ManualAddPropertyHistory("LowerOutput", snOrig, snNew, True)
            End Set
        End Property

        '<Category("Gain Limits"), _
        ' Description("Sets the output value to use when the x value is more than the upper limit."), _
        ' TypeConverter(GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter))> _
        <Browsable(False)> _
        Public Overridable Property UpperOutput() As AnimatTools.Framework.ScaledNumber
            Get
                Return m_snUpperOutput
            End Get
            Set(ByVal Value As AnimatTools.Framework.ScaledNumber)
                Dim snOrig As ScaledNumber = DirectCast(m_snUpperOutput.Clone(m_snUpperOutput.Parent, False, Nothing), ScaledNumber)
                If Not Value Is Nothing Then m_snUpperOutput.CopyData(Value)

                Dim snNew As ScaledNumber = DirectCast(m_snUpperOutput.Clone(m_snUpperOutput.Parent, False, Nothing), ScaledNumber)
                Me.ManualAddPropertyHistory("UpperOutput", snOrig, snNew, True)
            End Set
        End Property

        <Browsable(False)> _
        Public Overridable Property UseParentIncomingDataType() As Boolean
            Get
                Return m_bUseParentIncomingDataType
            End Get
            Set(ByVal Value As Boolean)
                Me.ManualAddPropertyHistory("UseParentIncomingDataType", m_bUseParentIncomingDataType, Value, True)

                m_bUseParentIncomingDataType = Value
            End Set
        End Property

        <Browsable(False)> _
        Public Overrides Property ViewSubProperties() As Boolean
            Get
                Return False
            End Get
            Set(ByVal Value As Boolean)
            End Set
        End Property

        <Browsable(False)> _
        Public Overridable Property IndependentUnits() As String
            Get
                Return m_strIndependentUnits
            End Get
            Set(ByVal Value As String)
                m_strIndependentUnits = Value
            End Set
        End Property

        <Browsable(False)> _
        Public Overridable Property DependentUnits() As String
            Get
                Return m_strDependentUnits
            End Get
            Set(ByVal Value As String)
                m_strDependentUnits = Value
            End Set
        End Property

        <Browsable(False)> _
        Public Property ParentData() As AnimatTools.DataObjects.Behavior.Data
            Get
                Return m_bdParentData
            End Get
            Set(ByVal Value As AnimatTools.DataObjects.Behavior.Data)
                m_bdParentData = Value
            End Set
        End Property

        <Browsable(False)> _
        Public Overridable ReadOnly Property SelectableGain() As Boolean
            Get
                Return True
            End Get
        End Property

        <Browsable(False)> _
        Public Overridable ReadOnly Property BarAssemblyFile() As String
            Get
                Return "AnimatTools.dll"
            End Get
        End Property

        <Browsable(False)> _
        Public Overridable ReadOnly Property BarClassName() As String
            Get
                Return "AnimatTools.Forms.Gain.SelectGainType"
            End Get
        End Property

        <Browsable(False)> _
        Public Overridable ReadOnly Property ImageName() As String
            Get
                Return ""
            End Get
        End Property

        <Browsable(False)> _
        Public Overridable ReadOnly Property Image() As Image
            Get
                If m_Image Is Nothing AndAlso Me.ImageName.Trim.Length > 0 Then
                    Dim myAssembly As System.Reflection.Assembly
                    myAssembly = System.Reflection.Assembly.Load(Me.AssemblyModuleName)
                    m_Image = ImageManager.LoadImage(myAssembly, Me.ImageName)
                End If

                Return m_Image
            End Get
        End Property

        <Browsable(False)> _
        Public Overridable ReadOnly Property DraggableParent() As AnimatTools.DataObjects.DragObject
            Get
                If Not m_doParent Is Nothing AndAlso TypeOf m_doParent Is AnimatTools.DataObjects.DragObject Then
                    Return DirectCast(m_doParent, AnimatTools.DataObjects.DragObject)
                Else
                    Return Nothing
                End If
            End Get
        End Property

#End Region

#Region " Methods "

        Public Sub New(ByVal doParent As Framework.DataObject)
            MyBase.New(doParent)

            m_snLowerLimit = New AnimatTools.Framework.ScaledNumber(Me, "LowerLimit", 0, AnimatTools.Framework.ScaledNumber.enumNumericScale.None, "", "")
            m_snUpperLimit = New AnimatTools.Framework.ScaledNumber(Me, "UpperLimit", 1, AnimatTools.Framework.ScaledNumber.enumNumericScale.None, "", "")
            m_snLowerOutput = New AnimatTools.Framework.ScaledNumber(Me, "LowerOutput", 0, AnimatTools.Framework.ScaledNumber.enumNumericScale.None, "", "")
            m_snUpperOutput = New AnimatTools.Framework.ScaledNumber(Me, "UpperOutput", 1, AnimatTools.Framework.ScaledNumber.enumNumericScale.None, "", "")
            m_bUseParentIncomingDataType = True
        End Sub

        Public Sub New(ByVal doParent As Framework.DataObject, ByVal strIndependentUnits As String, ByVal strDependentUnits As String, _
                       ByVal bLimitsReadOnly As Boolean, ByVal bLimitOutputsReadOnly As Boolean, ByVal bUseParentIncomingDataType As Boolean)
            MyBase.New(doParent)

            m_snLowerLimit = New AnimatTools.Framework.ScaledNumber(Me, "LowerLimit", 0, AnimatTools.Framework.ScaledNumber.enumNumericScale.None, "", "")
            m_snUpperLimit = New AnimatTools.Framework.ScaledNumber(Me, "UpperLimit", 1, AnimatTools.Framework.ScaledNumber.enumNumericScale.None, "", "")
            m_snLowerOutput = New AnimatTools.Framework.ScaledNumber(Me, "LowerOutput", 0, AnimatTools.Framework.ScaledNumber.enumNumericScale.None, "", "")
            m_snUpperOutput = New AnimatTools.Framework.ScaledNumber(Me, "UpperOutput", 1, AnimatTools.Framework.ScaledNumber.enumNumericScale.None, "", "")

            m_bUseParentIncomingDataType = bUseParentIncomingDataType
            m_strIndependentUnits = strIndependentUnits
            m_strDependentUnits = strDependentUnits
            m_bLimitsReadOnly = bLimitsReadOnly
            m_bLimitOutputsReadOnly = bLimitOutputsReadOnly
        End Sub

        Public Overridable Function InLimits(ByVal dblInput As Double) As Boolean
            If m_bUseLimits AndAlso ((dblInput < m_snLowerLimit.ActualValue) OrElse (dblInput > m_snUpperLimit.ActualValue)) Then
                Return False
            Else
                Return True
            End If
        End Function

        Public Overridable Function CalculateLimitOutput(ByVal dblInput As Double) As Double
            If dblInput < m_snLowerLimit.ActualValue Then Return m_snLowerOutput.ActualValue

            If dblInput > m_snUpperLimit.ActualValue Then Return m_snUpperOutput.ActualValue

            Return 0
        End Function

        Public MustOverride Function CalculateGain(ByVal dblInput As Double) As Double

        Public Overridable Sub RecalculuateLimits()
        End Sub

        Protected Overrides Sub CloneInternal(ByVal doOriginal As AnimatTools.Framework.DataObject, ByVal bCutData As Boolean, _
                                            ByVal doRoot As AnimatTools.Framework.DataObject)
            MyBase.CloneInternal(doOriginal, bCutData, doRoot)

            Dim OrigNode As DataObjects.Gain = DirectCast(doOriginal, DataObjects.Gain)

            m_strGainPropertyName = OrigNode.m_strGainPropertyName
            m_bUseLimits = OrigNode.m_bUseLimits
            m_bLimitsReadOnly = OrigNode.m_bLimitsReadOnly
            m_bLimitOutputsReadOnly = OrigNode.m_bLimitOutputsReadOnly
            m_bUseParentIncomingDataType = OrigNode.m_bUseParentIncomingDataType

            m_snLowerLimit = DirectCast(OrigNode.m_snLowerLimit.Clone(Me, bCutData, doRoot), ScaledNumber)
            m_snUpperLimit = DirectCast(OrigNode.m_snUpperLimit.Clone(Me, bCutData, doRoot), ScaledNumber)
            m_snLowerOutput = DirectCast(OrigNode.m_snLowerOutput.Clone(Me, bCutData, doRoot), ScaledNumber)
            m_snUpperOutput = DirectCast(OrigNode.m_snUpperOutput.Clone(Me, bCutData, doRoot), ScaledNumber)

            m_strIndependentUnits = OrigNode.m_strIndependentUnits
            m_strDependentUnits = OrigNode.m_strDependentUnits
        End Sub

        Public Overrides Function ToString() As String
            Return Me.Type
        End Function

#Region " DataObject Methods "

        Protected Overrides Sub BuildProperties()

            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Use Limits", m_bUseLimits.GetType(), "UseLimits", _
                                        "Gain Limits", "Sets the whether to use the upper and lower limits on the x variable.", m_bUseLimits, _
                                        (m_bLimitsReadOnly Or m_bLimitOutputsReadOnly)))

            Dim pbNumberBag As Crownwood.Magic.Controls.PropertyBag = m_snLowerLimit.Properties
            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Lower Limit", pbNumberBag.GetType(), "LowerLimit", _
                                        "Gain Limits", "Sets the lower limit of the x variable.", pbNumberBag, _
                                        "", GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter), m_bLimitsReadOnly))

            pbNumberBag = m_snUpperLimit.Properties
            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Upper Limit", pbNumberBag.GetType(), "UpperLimit", _
                                        "Gain Limits", "Sets the upper limit of the x variable.", pbNumberBag, _
                                        "", GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter), m_bLimitsReadOnly))

            pbNumberBag = m_snLowerOutput.Properties
            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Value @ Lower Limit", pbNumberBag.GetType(), "LowerOutput", _
                                        "Gain Limits", "Sets the output value to use when the x value is less than the lower limit.", pbNumberBag, _
                                        "", GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter), m_bLimitOutputsReadOnly))

            pbNumberBag = m_snUpperOutput.Properties
            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Value @ Upper Limit", pbNumberBag.GetType(), "UpperOutput", _
                                        "Gain Limits", "Sets the output value to use when the x value is more than the upper limit.", pbNumberBag, _
                                        "", GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter), m_bLimitOutputsReadOnly))

        End Sub

        Public Overridable Overloads Sub LoadData(ByRef oXml As AnimatTools.Interfaces.StdXml, ByVal strName As String, ByVal strGainPropertyName As String)

            m_strGainPropertyName = strGainPropertyName

            oXml.IntoChildElement(strName)

            m_bUseLimits = oXml.GetChildBool("UseLimits")

            m_bLimitsReadOnly = oXml.GetChildBool("LimitsReadOnly", m_bLimitsReadOnly)
            m_bLimitOutputsReadOnly = oXml.GetChildBool("LimitOutputsReadOnly", m_bLimitOutputsReadOnly)
            m_bUseParentIncomingDataType = oXml.GetChildBool("UseParentIncomingDataType", m_bUseParentIncomingDataType)

            If oXml.FindChildElement("LowerLimitScale", False) Then m_snLowerLimit.LoadData(oXml, "LowerLimitScale")
            If oXml.FindChildElement("UpperLimitScale", False) Then m_snUpperLimit.LoadData(oXml, "UpperLimitScale")
            If oXml.FindChildElement("LowerOutputScale", False) Then m_snLowerOutput.LoadData(oXml, "LowerOutputScale")
            If oXml.FindChildElement("UpperOutputScale", False) Then m_snUpperOutput.LoadData(oXml, "UpperOutputScale")

            Dim strIndependentUnits As String = oXml.GetChildString("IndependentUnits", "")
            Dim strDependentUnits As String = oXml.GetChildString("DependentUnits", "")

            If m_strIndependentUnits.Trim.Length > 0 Then m_strIndependentUnits = strIndependentUnits
            If strDependentUnits.Trim.Length > 0 Then m_strDependentUnits = strDependentUnits

            oXml.OutOfElem()

        End Sub


        Public Overridable Overloads Sub SaveData(ByRef oXml As AnimatTools.Interfaces.StdXml, ByVal strName As String)

            oXml.AddChildElement(strName)
            oXml.IntoElem()

            oXml.AddChildElement("Type", Me.Type())
            oXml.AddChildElement("AssemblyFile", Me.AssemblyFile)
            oXml.AddChildElement("ClassName", Me.ClassName)

            oXml.AddChildElement("UseLimits", m_bUseLimits)

            oXml.AddChildElement("LimitsReadOnly", m_bLimitsReadOnly)
            oXml.AddChildElement("LimitOutputsReadOnly", m_bLimitOutputsReadOnly)
            oXml.AddChildElement("UseParentIncomingDataType", m_bUseParentIncomingDataType)

            m_snLowerLimit.SaveData(oXml, "LowerLimitScale")
            m_snUpperLimit.SaveData(oXml, "UpperLimitScale")
            m_snLowerOutput.SaveData(oXml, "LowerOutputScale")
            m_snUpperOutput.SaveData(oXml, "UpperOutputScale")

            oXml.AddChildElement("IndependentUnits", m_strIndependentUnits)
            oXml.AddChildElement("DependentUnits", m_strDependentUnits)

            oXml.OutOfElem()

        End Sub

        Public Overridable Sub SaveNetwork(ByRef oXml As AnimatTools.Interfaces.StdXml, ByVal strName As String)

            oXml.AddChildElement(strName)
            oXml.IntoElem()

            If Me.ModuleName.Trim.Length > 0 Then
                oXml.AddChildElement("ModuleName", Me.ModuleName())
            End If

            oXml.AddChildElement("Type", Me.Type())
            oXml.AddChildElement("UseLimits", m_bUseLimits)
            If m_bUseLimits Then
                oXml.AddChildElement("LowerLimit", m_snLowerLimit.ActualValue)
                oXml.AddChildElement("UpperLimit", m_snUpperLimit.ActualValue)
                oXml.AddChildElement("LowerOutput", m_snLowerOutput.ActualValue)
                oXml.AddChildElement("UpperOutput", m_snUpperOutput.ActualValue)
            End If

            oXml.OutOfElem()

        End Sub

#End Region

#End Region

    End Class

End Namespace
