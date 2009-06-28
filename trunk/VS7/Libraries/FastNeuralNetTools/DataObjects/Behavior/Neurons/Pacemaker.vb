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

    Public Class Pacemaker
        Inherits Neurons.Normal

#Region " Attributes "

        Protected m_snIl As AnimatTools.Framework.ScaledNumber
        Protected m_snIh As AnimatTools.Framework.ScaledNumber
        Protected m_snVssm As AnimatTools.Framework.ScaledNumber
        Protected m_snMtl As AnimatTools.Framework.ScaledNumber
        Protected m_snBtl As AnimatTools.Framework.ScaledNumber
        Protected m_snTh As AnimatTools.Framework.ScaledNumber

#End Region

#Region " Properties "

        Public Overrides ReadOnly Property TypeName() As String
            Get
                Return "Pacemaker Firing Rate Neuron"
            End Get
        End Property

        Public Overridable Property Il() As AnimatTools.Framework.ScaledNumber
            Get
                Return m_snIl
            End Get
            Set(ByVal Value As AnimatTools.Framework.ScaledNumber)
                m_snIl.CopyData(Value)
            End Set
        End Property

        Public Overridable Property Ih() As AnimatTools.Framework.ScaledNumber
            Get
                Return m_snIh
            End Get
            Set(ByVal Value As AnimatTools.Framework.ScaledNumber)
                m_snIh.CopyData(Value)
            End Set
        End Property

        Public Overridable Property Vssm() As AnimatTools.Framework.ScaledNumber
            Get
                Return m_snVssm
            End Get
            Set(ByVal Value As AnimatTools.Framework.ScaledNumber)
                m_snVssm.CopyData(Value)
            End Set
        End Property

        Public Overridable Property Mtl() As AnimatTools.Framework.ScaledNumber
            Get
                Return m_snMtl
            End Get
            Set(ByVal Value As AnimatTools.Framework.ScaledNumber)
                m_snMtl.CopyData(Value)
            End Set
        End Property

        Public Overridable Property Btl() As AnimatTools.Framework.ScaledNumber
            Get
                Return m_snBtl
            End Get
            Set(ByVal Value As AnimatTools.Framework.ScaledNumber)
                m_snBtl.CopyData(Value)
            End Set
        End Property

        Public Overridable Property Th() As AnimatTools.Framework.ScaledNumber
            Get
                Return m_snTh
            End Get
            Set(ByVal Value As AnimatTools.Framework.ScaledNumber)
                If Value.Value <= 0 Then
                    Throw New System.Exception("You can not set the active time constant to a value less than or equal to 0.")
                End If

                m_snTh.CopyData(Value)
            End Set
        End Property

        Public Overrides ReadOnly Property NeuronType() As String
            Get
                Return "Pacemaker"
            End Get
        End Property

        Public Overrides ReadOnly Property ImageName() As String
            Get
                Return "FastNeuralNetTools.PacemakerNeuron.gif"
            End Get
        End Property

#End Region

#Region " Methods "

        Public Sub New(ByVal doParent As AnimatTools.Framework.DataObject)
            MyBase.New(doParent)

            Try

                m_snIl = New AnimatTools.Framework.ScaledNumber(Me, "Il", -2, AnimatTools.Framework.ScaledNumber.enumNumericScale.nano, "Amps", "A")
                m_snIh = New AnimatTools.Framework.ScaledNumber(Me, "Ih", 2, AnimatTools.Framework.ScaledNumber.enumNumericScale.nano, "Amps", "A")
                m_snVssm = New AnimatTools.Framework.ScaledNumber(Me, "Vssm", -69.9, AnimatTools.Framework.ScaledNumber.enumNumericScale.milli, "Volts", "V")
                m_snMtl = New AnimatTools.Framework.ScaledNumber(Me, "Mtl", -100, AnimatTools.Framework.ScaledNumber.enumNumericScale.None, "", "")
                m_snBtl = New AnimatTools.Framework.ScaledNumber(Me, "Btl", 5, AnimatTools.Framework.ScaledNumber.enumNumericScale.None, "seconds", "s")
                m_snTh = New AnimatTools.Framework.ScaledNumber(Me, "Th", 1, AnimatTools.Framework.ScaledNumber.enumNumericScale.None, "seconds", "s")

                m_snGm = New AnimatTools.Framework.ScaledNumber(Me, "Gm", 100, AnimatTools.Framework.ScaledNumber.enumNumericScale.nano, "Siemens", "S")
                m_snGain = New AnimatTools.Framework.ScaledNumber(Me, "Gain", 50, AnimatTools.Framework.ScaledNumber.enumNumericScale.None, "V^-1", "V^-1")

                m_thDataTypes.DataTypes.Add(New AnimatTools.DataObjects.DataType("SteadyStateVoltage", "Steady State Voltage", "Volts", "V", -100, 100, ScaledNumber.enumNumericScale.milli, ScaledNumber.enumNumericScale.milli))
                m_thDataTypes.DataTypes.Add(New AnimatTools.DataObjects.DataType("InterburstInterval", "Interburst Interval", "seconds", "s", -100, 100, ScaledNumber.enumNumericScale.milli, ScaledNumber.enumNumericScale.milli))
                m_thDataTypes.DataTypes.Add(New AnimatTools.DataObjects.DataType("InterburstTimer", "Interburst Timer", "seconds", "s", -100, 100, ScaledNumber.enumNumericScale.milli, ScaledNumber.enumNumericScale.milli))

                Shape = AnimatTools.DataObjects.Behavior.Node.enumShape.Ellipse
                Size = New SizeF(40, 40)
                Me.DrawColor = Color.Black
                Me.FillColor = Color.Blue
                Me.TextColor = Color.White

                Dim myAssembly As System.Reflection.Assembly
                myAssembly = System.Reflection.Assembly.Load("FastNeuralNetTools")

                Me.Image = AnimatTools.Framework.ImageManager.LoadImage(myAssembly, "FastNeuralNetTools.PacemakerNeuron.gif", False)
                Me.Name = "Pacemaker Firing Rate Neuron"
                Me.Description = "A firing rate neuron type that can exhibit endogenous bursting behavior from the fast neural network library."

            Catch ex As System.Exception
                AnimatTools.Framework.Util.DisplayError(ex)
            End Try

        End Sub

        Public Overrides Function Clone(ByVal doParent As AnimatTools.Framework.DataObject, ByVal bCutData As Boolean, _
                                        ByVal doRoot As AnimatTools.Framework.DataObject) As AnimatTools.Framework.DataObject
            Dim oNewNode As New Neurons.Pacemaker(doParent)
            oNewNode.CloneInternal(Me, bCutData, doRoot)
            If Not doRoot Is Nothing AndAlso doRoot Is Me Then oNewNode.AfterClone(Me, bCutData, doRoot, oNewNode)
            Return oNewNode
        End Function

        Protected Overrides Sub CloneInternal(ByVal doOriginal As AnimatTools.Framework.DataObject, ByVal bCutData As Boolean, _
                                            ByVal doRoot As AnimatTools.Framework.DataObject)
            MyBase.CloneInternal(doOriginal, bCutData, doRoot)

            Dim bnOrig As Neurons.Pacemaker = DirectCast(doOriginal, Neurons.Pacemaker)

            m_snIl = DirectCast(bnOrig.m_snIl.Clone(Me, bCutData, doRoot), ScaledNumber)
            m_snIh = DirectCast(bnOrig.m_snIh.Clone(Me, bCutData, doRoot), ScaledNumber)
            m_snVssm = DirectCast(bnOrig.m_snVssm.Clone(Me, bCutData, doRoot), ScaledNumber)
            m_snMtl = DirectCast(bnOrig.m_snMtl.Clone(Me, bCutData, doRoot), ScaledNumber)
            m_snBtl = DirectCast(bnOrig.m_snBtl.Clone(Me, bCutData, doRoot), ScaledNumber)
            m_snTh = DirectCast(bnOrig.m_snTh.Clone(Me, bCutData, doRoot), ScaledNumber)
        End Sub

        Public Overrides Sub SaveNetwork(ByRef oXml As AnimatTools.Interfaces.StdXml, ByRef nmModule As AnimatTools.DataObjects.Behavior.NeuralModule)
            MyBase.SaveNetwork(oXml, nmModule)

            oXml.IntoElem() 'Into Neuron element

            oXml.AddChildElement("Il", m_snIl.ActualValue)
            oXml.AddChildElement("Ih", m_snIh.ActualValue)

            Dim dblVssm As Double = (m_snVssm.ActualValue - m_snVrest.ActualValue)
            oXml.AddChildElement("Vssm", dblVssm)
            oXml.AddChildElement("Mtl", m_snMtl.ActualValue)
            oXml.AddChildElement("Btl", m_snBtl.ActualValue)
            oXml.AddChildElement("Th", m_snTh.ActualValue)

            oXml.OutOfElem() 'Outof Neuron

        End Sub

#Region " DataObject Methods "

        Protected Overrides Sub BuildProperties()
            MyBase.BuildProperties()

            Dim pbNumberBag As Crownwood.Magic.Controls.PropertyBag = m_snIl.Properties
            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Il", pbNumberBag.GetType(), "Il", _
                                        "Neural Properties", "Sets the hyperpolarizing current that brings the " & _
                                        "membrane potential back down after it has been firing.", pbNumberBag, _
                                        "", GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)))

            pbNumberBag = m_snIh.Properties
            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Ih", pbNumberBag.GetType(), "Ih", _
                                        "Neural Properties", "Sets the depolarizing current that raises the " & _
                                        "membrane potential and causes the neuron to fire.", pbNumberBag, _
                                        "", GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)))

            pbNumberBag = m_snVssm.Properties
            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Vssm", pbNumberBag.GetType(), "Vssm", _
                                        "Neural Properties", "Sets the lower steady state threshold. " & _
                                        "If the steady state voltage of the neuron goes below this value then the " & _
                                        "Il current is locked on until that voltage rises above this threshold.", pbNumberBag, _
                                        "", GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)))

            pbNumberBag = m_snMtl.Properties
            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Mtl", pbNumberBag.GetType(), "Mtl", _
                                        "Neural Properties", "Sets the slope of the linear function that is " & _
                                        "used to calculate the length of time that Il current remains active.", pbNumberBag, _
                                        "", GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)))

            pbNumberBag = m_snBtl.Properties
            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Btl", pbNumberBag.GetType(), "Btl", _
                                        "Neural Properties", "Sets the intercept of the linear function that " & _
                                        "is used to calculate the length of time that Il current remains active.", pbNumberBag, _
                                        "", GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)))

            pbNumberBag = m_snTh.Properties
            m_Properties.Properties.Add(New Crownwood.Magic.Controls.PropertySpec("Th", pbNumberBag.GetType(), "Th", _
                                        "Neural Properties", "Sets the length of time that the Ih current remains active.", pbNumberBag, _
                                        "", GetType(AnimatTools.Framework.ScaledNumber.ScaledNumericPropBagConverter)))

        End Sub

        Public Overrides Sub ClearIsDirty()
            MyBase.ClearIsDirty()

            If Not m_snIl Is Nothing Then m_snIl.ClearIsDirty()
            If Not m_snIh Is Nothing Then m_snIh.ClearIsDirty()
            If Not m_snVssm Is Nothing Then m_snVssm.ClearIsDirty()
            If Not m_snMtl Is Nothing Then m_snMtl.ClearIsDirty()
            If Not m_snBtl Is Nothing Then m_snBtl.ClearIsDirty()
            If Not m_snTh Is Nothing Then m_snTh.ClearIsDirty()
        End Sub

        Public Overrides Sub LoadData(ByRef oXml As AnimatTools.Interfaces.StdXml)
            MyBase.LoadData(oXml)

            oXml.IntoElem()

            m_snIl.LoadData(oXml, "Il")
            m_snIh.LoadData(oXml, "Ih")
            m_snVssm.LoadData(oXml, "Vssm")
            m_snMtl.LoadData(oXml, "Mtl")
            m_snBtl.LoadData(oXml, "Btl")
            m_snTh.LoadData(oXml, "Th")

            oXml.OutOfElem()

        End Sub

        Public Overrides Sub SaveData(ByRef oXml As AnimatTools.Interfaces.StdXml)
            MyBase.SaveData(oXml)

            oXml.IntoElem() 'Into Node Element

            m_snIl.SaveData(oXml, "Il")
            m_snIh.SaveData(oXml, "Ih")
            m_snVssm.SaveData(oXml, "Vssm")
            m_snMtl.SaveData(oXml, "Mtl")
            m_snBtl.SaveData(oXml, "Btl")
            m_snTh.SaveData(oXml, "Th")

            oXml.OutOfElem() ' Outof Node Element

        End Sub

#End Region

#End Region

    End Class

End Namespace
